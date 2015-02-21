// filename:		CGlobalSettingsSelectSheet.hpp
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
#include "GlobalSettingsSelectSheet.hpp"
#include "AveDeskGlobalSettings.hpp"
#include "resource.h"


CGlobalSettingsSelectSheet::CGlobalSettingsSelectSheet(AveDeskGlobalSettings& globalSettings) :
	globalSettings(globalSettings), initMode(true)
{
	
}

CGlobalSettingsSelectSheet::~CGlobalSettingsSelectSheet()
{

}

void CGlobalSettingsSelectSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && (wNotifyCode == EN_CHANGE || wNotifyCode == BN_CLICKED))
		SetModified();
}
 
// Message handlers
BOOL CGlobalSettingsSelectSheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	initMode = true;
	CheckDlgButton(IDC_HOVERSELECTION,globalSettings.GetHoverSelection() ? TRUE : FALSE );
	SetDlgItemInt(IDC_SELECTIONDELAY,globalSettings.GetHoverSelectionDelay() );
	initMode = false;
	return TRUE;	
}
 
int CGlobalSettingsSelectSheet::OnApply()
{
	globalSettings.SetHoverSelection( IsDlgButtonChecked(IDC_HOVERSELECTION) == TRUE);
	globalSettings.SetHoverSelectionDelay ( GetDlgItemInt(IDC_SELECTIONDELAY) );
	return TRUE;
};
