// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"



LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWindow parent = GetParent();
	if(parent)
		parent.CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	parent.SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	parent.SetIcon(hIconSmall, FALSE);

	SetDlgItemText(IDC_NEWTOOLNAME, _T("NewTool"));

	WCHAR wProjectDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, wProjectDir, MAX_PATH);
	PathRemoveFileSpec(wProjectDir);
	PathAppendW(wProjectDir, L"Projects");
	SetDlgItemText(IDC_PROJECTDIR, wProjectDir);

	UpdateOutputDir();

	return TRUE;
}

void CMainDlg::UpdateOutputDir()
{
	BOOL noNext = FALSE;
	WCHAR wBasePath[MAX_PATH] = {0};
	WCHAR wNewDir[MAX_PATH] = {0};
	GetDlgItemText(IDC_PROJECTDIR, wBasePath, MAX_PATH);
	GetDlgItemText(IDC_NEWTOOLNAME, wNewDir, MAX_PATH);

	ts->settingsFolderName = wNewDir;
	ts->toolName = wNewDir;
	if(wcslen(wBasePath) == 0)
		noNext = TRUE;
	if(wcslen(wNewDir) == 0)
		noNext = TRUE;

	PathAppendW(wBasePath, wNewDir);

	if(noNext)
		SetWizardButtons(0);
	else
		SetWizardButtons(PSWIZB_NEXT);

	RECT rc = {0};
	GetDlgItem(IDC_OUTPUTDIR).GetClientRect(&rc);
	ts->outputDir = wBasePath;
	PathCompactPathW(CWindowDC(GetDlgItem(IDC_OUTPUTDIR)), wBasePath, rc.right - rc.left);
	SetDlgItemText(IDC_OUTPUTDIR, wBasePath);
}

BOOL CMainDlg::OnSetActive() 
{    
   UpdateOutputDir();
   return TRUE;
}


int CMainDlg::OnWizardNext()
{
	UpdateOutputDir();
	ts->StartProject();
	return 0;
}
LRESULT CMainDlg::OnBnClickedBrowseprojectdir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFolderDialog folderDlg(m_hWnd, _T("Select Project Directory"),0);
	if(folderDlg.DoModal() && wcslen(folderDlg.m_szFolderPath) > 0)
		SetDlgItemText(IDC_PROJECTDIR, folderDlg.m_szFolderPath);

	return 0;
}

LRESULT CMainDlg::OnEnChangeNewtoolname(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateOutputDir();

	return 0;
}

LRESULT CMainDlg::OnEnChangeProjectdir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateOutputDir();

	return 0;
}
