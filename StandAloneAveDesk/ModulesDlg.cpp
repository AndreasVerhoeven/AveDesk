// ModulesDlg.cpp: implementation of the CModulesDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModulesDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModulesDlg::CModulesDlg(CModules* modules) :
	m_Modules(modules)
{

}

CModulesDlg::~CModulesDlg()
{

}

bool CModulesDlg::AddModuleToList(CModule* m)
{
	if(NULL == m)
		return false;

	ILBITEM item = {0};
	item.mask = ILBIF_TEXT | ILBIF_SUBTEXT | ILBIF_IMAGE |   ILBIF_STYLE | ILBIF_FORMAT;
	item.iItem = m_ModulesListBox.GetCount();
	item.pszText = const_cast<LPTSTR>(m->GetName().operator LPCTSTR());
	if(!m->GetDescription().IsEmpty())
		item.pszSubText = const_cast<LPTSTR>(m->GetDescription().operator LPCTSTR());
	else
		item.pszSubText = const_cast<LPTSTR>(m->GetFilePath().operator LPCTSTR());
	item.iImage = item.iSelImage = 0;
	item.style = ILBS_IMGLEFT;
	item.format = DT_LEFT;
	m_ModulesListBox.InsertItem(&item);

	return true;
}

LRESULT CModulesDlg::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	::EnableWindow(GetDlgItem(IDC_UNINSTALL), m_ModulesListBox.GetSelCount() > 0);
	return 0;
}

LRESULT CModulesDlg::OnUninstallButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for( int curSel = m_ModulesListBox.GetCount() - 1; curSel >= 0; curSel-- ) 
    {
        if( m_ModulesListBox.GetSel(curSel) )
        {
			m_Modules->RemoveModule(curSel);
			m_ModulesListBox.DeleteString(curSel);
	    }
    }

    return 0;
}

LRESULT CModulesDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDrop = reinterpret_cast<HDROP>(wParam);
	if(NULL == hDrop)
		return 0;


	int numFiles = DragQueryFile(hDrop,(UINT)-1,NULL,0);
	for(int i = 0; i < numFiles; ++i)
	{
		WCHAR path[MAX_PATH] = {0};
		DragQueryFileW(hDrop,i,path,MAX_PATH);

		if(path[0] != L'\0')
		{
			CModule* m = m_Modules->LoadModule(path);
			if(m)
				AddModuleToList(m);
		}
	}

	return 0;
}

LRESULT CModulesDlg::OnInstallButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	CFileDialog fd(TRUE,												// TRUE for OpenDialog
					_T("dll"),									// default extension
					NULL,												// filename
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,				// flags
					_T("DLL Files(*.dll)\0*.dll\0AveModules(*.avemodule)\0*.avemodule\0All Files(*.*)\0*.*\0\0"), // filter
					m_hWnd);

	if( fd.DoModal() == TRUE)
	{
		CModule* m = m_Modules->LoadModule(fd.m_szFileName);
		if(m)
			AddModuleToList(m);
	}

	return 0;
}

LRESULT CModulesDlg::OnCloseButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(0);

	return 0;
}

	// dialog stuff
LRESULT CModulesDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	LOGFONTW lfBig = {0};
	lfBig.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfBig.lfFaceName,L"Tahoma");
	lfBig.lfWeight = FW_BOLD;
	lfBig.lfHeight = 14;

	LOGFONTW lfSmall = {0};
	lfSmall.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfSmall.lfFaceName,L"Tahoma");
	lfSmall.lfWeight = FW_NORMAL;
	lfSmall.lfHeight = 12;

	bigFont.CreateFontIndirect(&lfBig);
	smallFont.CreateFontIndirect(&lfSmall);


	m_ImageList.Create(48,48,ILC_COLOR32,1, 1);
	ImageList_AddIcon(m_ImageList.m_hImageList, (HICON)LoadIcon(_Module.GetModuleInstance(),MAKEINTRESOURCE(IDI_ICON3)));

	m_ModulesListBox.SubclassWindow(GetDlgItem(IDC_MODULESLIST));
	m_ModulesListBox.SetItemHeight(0, 54);
	m_ModulesListBox.SetImageList(m_ImageList, ILSIL_NORMAL);

	m_ModulesListBox.SetFont(bigFont);
	DeleteObject(m_ModulesListBox.SetSmallFont(smallFont));

	ToolTips_Init();

	std::vector<CModule*> modules = m_Modules->GetCurrentModules();
	for(std::vector<CModule*>::iterator iter = modules.begin(); iter != modules.end(); ++iter)
	{
		AddModuleToList(*iter);
	}

	return 0;
}

LRESULT CModulesDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog(0);
	return 0;
}

LRESULT CModulesDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_ModulesListBox.UnsubclassWindow();
	return 0;
}