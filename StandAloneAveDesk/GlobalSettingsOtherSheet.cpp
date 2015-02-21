// filename:		CGlobalSettingsOtherSheet.hpp
// Description:		Definition of class CGlobalSettingsMoveSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "GlobalSettingsOtherSheet.hpp"
#include "AveDeskGlobalSettings.hpp"
#include "resource.h"
#include <atlmisc.h>


CGlobalSettingsOtherSheet::CGlobalSettingsOtherSheet(AveDeskGlobalSettings& globalSettings) :
	globalSettings(globalSettings), initMode(true)
{
}

CGlobalSettingsOtherSheet::~CGlobalSettingsOtherSheet()
{

}

 
// Message handlers
BOOL CGlobalSettingsOtherSheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	CheckDlgButton(IDC_MAKETHUMBNAIL, globalSettings.GetAddPreviewInThemeFiles());
	CheckDlgButton(IDC_ASK,globalSettings.GetAsk() );

	HKEY key = {0};
	if(RegOpenKeyExW(HKEY_CURRENT_USER,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_READ,&key)==ERROR_SUCCESS)
	{
	
		WCHAR text[MAX_PATH] = {0};
		DWORD type(0);
		DWORD bufSize(MAX_PATH);
					
		RegQueryValueExW(key,L"AVEDESK",0,&type,(BYTE*)text,&bufSize);

		WCHAR exePath[MAX_PATH+2] = {0};
		exePath[0]=L'\"';
		GetModuleFileNameW(GetModuleHandle(0),exePath+1,sizeof(exePath)/sizeof(exePath[0]));
		wcscat(exePath,L"\"");
		CheckDlgButton(IDC_STARTUP,_wcsicmp(text,exePath)==0 ? TRUE : FALSE);
		RegCloseKey(key);
	}

	CheckDlgButton(IDC_AUTOSAVE,globalSettings.GetAutoSaveThemes() ? TRUE : FALSE);

	initMode = false;

	return TRUE;	
}
 

void CGlobalSettingsOtherSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && wNotifyCode == BN_CLICKED)
		SetModified();
}
 
int CGlobalSettingsOtherSheet::OnApply()
{

	globalSettings.SetAddPreviewInThemeFiles(IsDlgButtonChecked(IDC_MAKETHUMBNAIL) != FALSE);
	globalSettings.SetAutoSaveThemes(IsDlgButtonChecked(IDC_AUTOSAVE) != FALSE);

	HKEY key = {0};
	if(RegOpenKeyExW(HKEY_CURRENT_USER,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_ALL_ACCESS,&key)==ERROR_SUCCESS)
	{
		RegDeleteValueW(key,L"AVEDESK");
		RegDeleteKeyW(key,L"AVEDESK");
		RegFlushKey(key);

		if(IsDlgButtonChecked(IDC_STARTUP))
		{
			WCHAR exePath[MAX_PATH+2] = {0};
			exePath[0]='\"';
			GetModuleFileNameW(GetModuleHandle(0),exePath+1,sizeof(exePath)/sizeof(exePath[0]));
			wcscat(exePath,L"\"");

			RegSetValueExW(key,L"AVEDESK",0,REG_SZ,(BYTE*)exePath,(wcslen(exePath)+1)*sizeof(WCHAR));
		}
		RegFlushKey(key);
		RegCloseKey(key);
	}

	globalSettings.SetAsk( IsDlgButtonChecked(IDC_ASK) == BST_CHECKED  ? true : false);

	return TRUE;
};
