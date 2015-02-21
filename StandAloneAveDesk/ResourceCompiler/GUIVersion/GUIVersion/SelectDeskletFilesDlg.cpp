// MainDlg.cpp : implementation of the CSelectDeskletFilesDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "SelectDeskletFilesDlg.h"



LRESULT CSelectDeskletFilesDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWindow parent = GetParent();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	parent.SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	parent.SetIcon(hIconSmall, FALSE);

	WCHAR wDeskletPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, wDeskletPath, MAX_PATH);
	PathRemoveFileSpec(wDeskletPath);
	PathRemoveFileSpec(wDeskletPath);
	PathAppendW(wDeskletPath, L"desklets");


	m_tree.SubclassWindow(GetDlgItem(IDC_TREEFILES));
	m_tree.SetShellStyle(SCT_EX_FILESYSTEMONLY);
	m_tree.Populate(wDeskletPath);
	

	return TRUE;
}

BOOL CSelectDeskletFilesDlg::OnSetActive() 
{ 
	if(HasSelectedFiles(m_tree.GetChildItem(TVI_ROOT)))
		SetWizardButtons(PSWIZB_NEXT);
	else
		SetWizardButtons(PSWIZB_BACK);
   
   return TRUE;
}

LRESULT CSelectDeskletFilesDlg::OnSelChanged(int /*idCtrl*/, 
                       LPNMHDR pnmh, 
                       BOOL& /*bHandled*/)
  {
	if(HasSelectedFiles(m_tree.GetChildItem(TVI_ROOT)))
		SetWizardButtons(PSWIZB_NEXT);
	else
		SetWizardButtons(PSWIZB_BACK);

    return 0;
  }

void CSelectDeskletFilesDlg::GetSelectedFiles(HTREEITEM hItem)
{
	if(NULL == hItem || TVI_ROOT == hItem)
		return;

	INT state = m_tree.GetCheckState(hItem);
	if(TREE_ITEM_UNCHECKED == state)
		return;

	WCHAR path[MAX_PATH] = {0};
	m_tree.GetItemPath(hItem, path);
	m_selectedFiles.push_back(path);

	HTREEITEM child = m_tree.GetChildItem(hItem);
	if(NULL == child)
	{
		m_tree.PopulateChildNode(hItem);
		child = m_tree.GetChildItem(hItem);
	}
	while(child != NULL)
	{
		GetSelectedFiles(child);
		child = m_tree.GetNextSiblingItem(child);
	}
}

BOOL CSelectDeskletFilesDlg::HasSelectedFiles(HTREEITEM hItem)
{
	if(NULL == hItem || TVI_ROOT == hItem)
		return FALSE;

	INT state = m_tree.GetCheckState(hItem);
	if(TREE_ITEM_UNCHECKED == state)
		return FALSE;
	else
		return TRUE;
}

int CSelectDeskletFilesDlg::OnWizardNext()
{
	CWaitCursor cursor;
	m_selectedFiles.clear();
	HTREEITEM parentItem = m_tree.GetChildItem(TVI_ROOT);
	GetSelectedFiles(parentItem);
	ts->deskletFiles = m_selectedFiles;

	return 0;
}
