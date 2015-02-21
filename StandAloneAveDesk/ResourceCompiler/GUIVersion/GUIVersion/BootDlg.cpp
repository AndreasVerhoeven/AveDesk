// MainDlg.cpp : implementation of the CBootDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "BootDlg.h"



LRESULT CBootDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	CheckDlgButton(IDC_LOADFILEONBOOT, ts->loadFileOnBoot);
	CheckDlgButton(IDC_LOADBOOTFILEWHENEMPTY, ts->loadBootWhenEmpty);
	CheckDlgButton(IDC_SAVETHEME, ts->saveTheme);
	CheckDlgButton(IDC_LOADTHEME, ts->loadTheme);
	CheckDlgButton(IDC_SAVESETTINGS, ts->saveSettings);
	CheckDlgButton(IDC_LOADSETTINGS, ts->loadSettings);
	std::wstring themeName=ts->toolName;
	themeName += L".avetheme";
	SetDlgItemText(IDC_THEMENAME,themeName.c_str()); 

	return TRUE;
}

BOOL CBootDlg::OnSetActive() 
{    
	SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
   return TRUE;
}


int CBootDlg::OnWizardNext()
{
	WCHAR themeName[MAX_PATH] = {0};
	GetDlgItemText(IDC_THEMENAME, themeName, MAX_PATH);
	WCHAR settingsFile[MAX_PATH] = {0};
	GetDlgItemText(IDC_SETTINGSFILE, settingsFile, MAX_PATH);
	WCHAR bootFile[MAX_PATH] = {0};
	GetDlgItemText(IDC_BOOTFILE, bootFile, MAX_PATH);

	ts->loadFileOnBoot = IsDlgButtonChecked(IDC_LOADFILEONBOOT);
	ts->loadBootWhenEmpty = IsDlgButtonChecked(IDC_LOADBOOTFILEWHENEMPTY);
	ts->saveTheme = IsDlgButtonChecked(IDC_SAVETHEME);
	ts->loadTheme = IsDlgButtonChecked(IDC_LOADTHEME);
	ts->saveSettings = IsDlgButtonChecked(IDC_SAVESETTINGS);
	ts->loadSettings = IsDlgButtonChecked(IDC_LOADSETTINGS);
	ts->themeName = themeName;
	ts->settingsFile = settingsFile;
	ts->bootFile = bootFile;

	return 0;
}


LRESULT CBootDlg::OnBnClickedBrowsebootfile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	WCHAR wFileName[MAX_PATH] = {0};
	GetDlgItemText(IDC_BOOTFILE,wFileName, MAX_PATH);
	CFileDialog fileDlg(TRUE, _T(".avedesklet;.avetheme;.dll"), wFileName, 4|2,_T("Ave Files\0*.avedesklet;*.avetheme;*.dll\0\0"), m_hWnd);
	if(fileDlg.DoModal())
	{
		SetDlgItemText(IDC_BOOTFILE, fileDlg.m_szFileName);
	}

	return 0;
}

LRESULT CBootDlg::OnBnClickedBrowsesettingsfile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	WCHAR wFileName[MAX_PATH] = {0};
	GetDlgItemText(IDC_SETTINGSFILE,wFileName, MAX_PATH);
	CFileDialog fileDlg(TRUE, _T(".xml"), wFileName, 4|2,_T("Ave Settings (XML file)\0*.xml\0\0"), m_hWnd);
	if(fileDlg.DoModal())
	{
		SetDlgItemText(IDC_SETTINGSFILE, fileDlg.m_szFileName);
	}

	return 0;
}
