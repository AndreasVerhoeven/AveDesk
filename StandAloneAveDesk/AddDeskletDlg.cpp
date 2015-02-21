// AddDeskletDlg.cpp: implementation of the CAddDeskletDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddDeskletDlg.h"
#include "helper_funcs.h"
#include "DockletDLL.h"
#include "Docklet.h"
#include "DeskletSkinEnumerator.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddDeskletDlg::CAddDeskletDlg()
{
	SetCursor(LoadCursor(0,IDC_WAIT));

	m_KnownDesklets.clear();
	m_KnownDesklets.reserve(DockletDLL::size() );

	WCHAR appDir[MAX_PATH] = {0};
	wcscpy(appDir, _App->GetAveDeskDataDir().c_str());
	std::wstring dir = lowercaseW(appDir);

	DockletDLL::ScanDirectoryForDesklets(dir+L"Docklets\\",true);
	DockletDLL::ScanDirectoryForDesklets(dir+L"Desklets\\",true);

	for(DockletDLL::DockletsDLLMap::iterator iter = DockletDLL::DockletDLLs.begin();  iter != DockletDLL::DockletDLLs.end(); ++iter)
	{
		KnownDeskletStruct kds;
		kds.dll = iter->second;
		kds.enumerator = DeskletSkinEnumerator::Create(kds.dll);
		m_KnownDesklets.push_back(kds);
	}

	SetCursor(LoadCursor(0,IDC_ARROW));
}

CAddDeskletDlg::~CAddDeskletDlg()
{
	for(std::vector<KnownDeskletStruct>::iterator iter = m_KnownDesklets.begin();iter != m_KnownDesklets.end();++iter)
	{
		KnownDeskletStruct& kds = *iter;
		if(kds.enumerator)
			kds.enumerator->Release();
		if(kds.dll)
			kds.dll->Release();
	}
}

void CAddDeskletDlg::FillListBox()
{
	USES_CONVERSION;

	int i =0;
	for(std::vector<KnownDeskletStruct>::iterator iter = m_KnownDesklets.begin(); iter != m_KnownDesklets.end(); ++iter)
	{
		KnownDeskletStruct& kds = *iter;

		HTREEITEM hItem = m_DeskletsListBox.InsertItem(kds.dll->GetName(),NULL,NULL);
		CTreeItem cItem(hItem,&m_DeskletsListBox);
		cItem.SetData(i++);

		std::wstring skinName;

		DeskletSkinInfo skinInfo;
		int skinIndex(0);
		while( kds.enumerator->Next(&skinInfo) )
		{
			CTreeItem childItem = m_DeskletsListBox.InsertItem(skinInfo.GetName().c_str(),hItem,NULL);
			childItem.SetData(static_cast<DWORD_PTR>(skinIndex));
			skinIndex++;
			kds.skinInfos.push_back(skinInfo);
		}

	}
}

void CAddDeskletDlg::SelectItem(UINT item, int childItemId)
{
	if(item >= m_KnownDesklets.size() )
		return;

	DockletDLL* dll = m_KnownDesklets[item].dll;

	if( -1 == childItemId || childItemId >= static_cast<int>(m_KnownDesklets[item].skinInfos.size()))
	{
		SetDlgItemTextW(IDC_NAME,dll->GetName());
		SetDlgItemTextW(IDC_AUTHOR,dll->GetAuthor());
		SetDlgItemInt(IDC_VERSION,dll->GetVersion());
	
		if(wcsstr(dll->GetComments(),L"Frances") == 0)
			SetDlgItemTextW(IDC_NOTES,dll->GetComments());
		else
			SetDlgItemTextW(IDC_NOTES,L"");
	}
	else
	{
		DeskletSkinInfo& info = m_KnownDesklets[item].skinInfos[childItemId];

		if(info.GetName().empty())
			SetDlgItemTextW(IDC_NAME,dll->GetName());	
		else
			SetDlgItemTextW(IDC_NAME,info.GetName().c_str());

		SetDlgItemTextW(IDC_AUTHOR,info.GetAuthor().c_str());
		if(info.HasVersionAsString())
			SetDlgItemTextW(IDC_VERSION,info.GetVersionAsString().c_str());
		else
			SetDlgItemInt(IDC_VERSION,info.GetVersion());

		if(wcsstr(info.GetNotes().c_str(),L"Frances") == 0)
			SetDlgItemTextW(IDC_NOTES,info.GetNotes().c_str());
		else
			SetDlgItemTextW(IDC_NOTES,L"");
	}
}

void CAddDeskletDlg::AddItem(UINT item, const WCHAR* skin, UINT skinNum)
{
	if(item >= m_KnownDesklets.size() )

	return;

	DockletDLL* dll = m_KnownDesklets[item].dll;

	Docklet* d = NULL;

	if( NULL == skin)
	{
		if(true) // TODO: add code for user setting
		{
			WCHAR dockletFile[MAX_PATH] = {0};
			WCHAR path[MAX_PATH] = {0};
			wcscpy(path, _App->GetAveDeskDataDir().c_str());
			wcscpy(dockletFile,dll->GetFileName());
			lowercaseW(path);
			lowercaseW(dockletFile);
			wcscpy(dockletFile,dockletFile+wcslen(path));

			USES_CONVERSION;
			CString fileName = W2T(dockletFile);
			fileName.Replace(TEXT('\\'),TEXT('_'));
			fileName.Replace(TEXT('/'),TEXT('_'));
	
			WCHAR pathName[MAX_PATH] = {0};
			wcscpy(pathName,_App->GetAveDeskUserPath().c_str());
			PathAppendW(pathName,L"\\Defaults\\");
			PathAppendW(pathName,T2W(const_cast<TCHAR*>(fileName.operator LPCTSTR())));
			PathRenameExtensionW(pathName,L".avedesklet");
			if(FileExistsW(pathName))
			{
				Docklet::LoadFromSingleFile(pathName,&d,TRUE);
			}
			else
			{
				d = Docklet::LoadDocklet(dll->GetFileName(),NULL,NULL,NULL,false);
			}
		}
		else
		{
				d = Docklet::LoadDocklet(dll->GetFileName(),NULL,NULL,NULL,false);
		}
	}
	else
	{
		KnownDeskletStruct& kds = m_KnownDesklets[item]; 
		int skinCount = kds.skinInfos.size();
		SkinInfoWrapper* skinInfo = NULL;
		if( skinNum != -1 && skinNum < static_cast<UINT>(skinCount))
			skinInfo = kds.skinInfos[skinNum].GetSkinInfoWrapper();
		/*Docklet * d = */Docklet::LoadDocklet(dll->GetFileName(),NULL,kds.enumerator,skinInfo,false);
		DeleteSkinInfoWrapper(skinInfo);
	}
}


LRESULT CAddDeskletDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	m_DeskletsListBox = GetDlgItem(IDC_TREE);
	FillListBox();

	return 0;
}

LRESULT CAddDeskletDlg::OnTreeSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	bHandled = TRUE;

	HTREEITEM hItem = m_DeskletsListBox.GetSelectedItem();
	if(!hItem)
		return LB_ERR;

	int childItemId = -1;

	CTreeItem cItem(hItem,&m_DeskletsListBox);
	if( cItem.GetParent() ) 
		childItemId = cItem.GetData();
	

	SelectItem( GetDeskletSelectionID(), childItemId );

	return 0;
}

LRESULT CAddDeskletDlg::OnCloseDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	EndDialog(TRUE);

	return 0;
}


LRESULT CAddDeskletDlg::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;

/*	int sel = m_DeskletsListBox.GetCurSel();
	if(sel != LB_ERR)
	{
		SelectItem(sel);	
	}
	*/

	return 0;
}

LRESULT CAddDeskletDlg::OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;

	/*
	int sel = m_DeskletsListBox.GetCurSel();
	if(sel != LB_ERR)
	{
		SelectItem(sel);
		AddItem(sel);
	}
	*/

	return 0;
}

int CAddDeskletDlg::GetDeskletSelectionID()
{
	HTREEITEM hItem = m_DeskletsListBox.GetSelectedItem();
	if(!hItem)
		return LB_ERR;

	CTreeItem cItem(hItem,&m_DeskletsListBox);
	if( cItem.GetParent() ) 
		cItem = cItem.GetParent();

	return cItem.GetData();
}

LRESULT CAddDeskletDlg::OnAddButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;

	HTREEITEM hItem = m_DeskletsListBox.GetSelectedItem();
	if(!hItem)
		return LB_ERR;

	CTreeItem cItem(hItem,&m_DeskletsListBox);
	if( cItem.GetParent() ) 
	{
		WCHAR skinName[MAX_PATH] = {0};
		cItem.GetText(skinName,MAX_PATH);

		CTreeItem cParentItem = cItem.GetParent();
		AddItem(cParentItem.GetData(),skinName, static_cast<UINT>(cItem.GetData()) );
	}
	else
	{
		AddItem(cItem.GetData());
	}	

	return 0;
}

LRESULT CAddDeskletDlg::OnCloseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;

	EndDialog(TRUE);

	return 0;
}