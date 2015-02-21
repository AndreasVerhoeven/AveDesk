// MainDlg.cpp : implementation of the CTrayIconDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "TrayIconDlg.h"



LRESULT CTrayIconDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWindow parent = GetParent();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	parent.SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	parent.SetIcon(hIconSmall, FALSE);

	CheckDlgButton(IDC_SHOWTRAYICON, ts->showTrayIcon);
	CheckDlgButton(IDC_SHOWTRAYICONMENU, ts->showTrayIconMenu);
	SetDlgItemText(IDC_TRAYICONTOOLTIP, ts->toolName.c_str());

	return TRUE;
}

void CTrayIconDlg::UpdateIconPath()
{
	CStatic staticIcon = GetDlgItem(IDC_TRAYICONDISPLAY);

	BOOL loadedIcon = FALSE;
	WCHAR wIconPath[MAX_PATH] = {0};
	GetDlgItemText(IDC_TRAYICON, wIconPath, MAX_PATH);
	if(PathFileExistsW(wIconPath))
	{
		const WCHAR* wExt = PathFindExtension(wIconPath);
		if(_wcsicmp(wExt, L".ico") == 0)
		{
			HICON hIcon = (HICON)LoadImage(NULL, wIconPath, IMAGE_ICON, 16, 16,LR_DEFAULTCOLOR | LR_LOADFROMFILE);
			if(hIcon != NULL)
			{
				staticIcon.SetIcon(hIcon);
				loadedIcon = TRUE;
			}
		}
		else
		{

		}
	}

	if(!loadedIcon)
	{
		HICON smallIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_AVEDESKICON), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		if(smallIcon != NULL)
			staticIcon.SetIcon(smallIcon);
	}
}

BOOL CTrayIconDlg::OnSetActive() 
{    
	UpdateIconPath();
	SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
   return TRUE;
}


int CTrayIconDlg::OnWizardNext()
{
	WCHAR wIconPath[MAX_PATH] = {0};
	GetDlgItemText(IDC_TRAYICON, wIconPath, MAX_PATH);
	if(PathFileExists(wIconPath))
		ts->trayIconFile = wIconPath;
	else
		ts->trayIconFile = L"";

	ts->showTrayIcon = IsDlgButtonChecked(IDC_SHOWTRAYICON);
	ts->showTrayIconMenu = IsDlgButtonChecked(IDC_SHOWTRAYICONMENU);
	
	WCHAR tt[1024] = {0};
	GetDlgItemText(IDC_TRAYICONTOOLTIP, tt, 1024);
	ts->trayIconToolTip = tt;

	return 0;
}
LRESULT CTrayIconDlg::OnEnChangeTrayicon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UpdateIconPath();

	return 0;
}

LRESULT CTrayIconDlg::OnBnClickedBrowsetrayicon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	WCHAR wFileName[MAX_PATH] = {0};
	GetDlgItemText(IDC_TRAYICON,wFileName, MAX_PATH);
	CFileDialog fileDlg(TRUE, _T(".ico"), wFileName, 4|2,_T("Icon Files\0*.ico\0\0"), m_hWnd);
	if(fileDlg.DoModal())
	{
		SetDlgItemText(IDC_TRAYICON, fileDlg.m_szFileName);
		UpdateIconPath();
	}

	return 0;
}
