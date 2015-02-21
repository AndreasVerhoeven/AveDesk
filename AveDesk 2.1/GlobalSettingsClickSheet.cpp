// filename:		CGlobalSettingsClickSheet.hpp
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
#include "GlobalSettingsClickSheet.hpp"
#include "AveDeskGlobalSettings.hpp"
#include "resource.h"
#include <atlmisc.h>


CGlobalSettingsClickSheet::CGlobalSettingsClickSheet(AveDeskGlobalSettings& globalSettings) :
	globalSettings(globalSettings), initMode(true)
{
}

CGlobalSettingsClickSheet::~CGlobalSettingsClickSheet()
{

}
 
// Message handlers
BOOL CGlobalSettingsClickSheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	initMode = true;
	clickModeBox = GetDlgItem(IDC_CLICKINGMODE);
	ATLASSERT(clickModeBox.IsWindow());

	CString str;
	str.LoadString(IDS_SINGLECLICK);
	clickModeBox.AddString(str);
	str.LoadString(IDS_DOUBLECLICK);
	clickModeBox.AddString(str);
	clickModeBox.SetCurSel( globalSettings.GetClickMode() );
	initMode = false;

	return TRUE;	
}

void CGlobalSettingsClickSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && wNotifyCode == CBN_SELCHANGE)
		SetModified();
}
 
int CGlobalSettingsClickSheet::OnApply()
{
	globalSettings.SetClickMode( clickModeBox.GetCurSel() );
	return TRUE;
};
