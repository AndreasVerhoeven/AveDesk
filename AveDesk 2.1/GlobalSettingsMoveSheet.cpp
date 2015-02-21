// filename:		GlobalSettingsMoveSheet.hpp
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
#include "GlobalSettingsMoveSheet.hpp"
#include "AveDeskGlobalSettings.hpp"
#include "resource.h"


CGlobalSettingsMoveSheet::CGlobalSettingsMoveSheet(AveDeskGlobalSettings& globalSettings) :
	globalSettings(globalSettings), initMode(true)
{
	
}

CGlobalSettingsMoveSheet::~CGlobalSettingsMoveSheet()
{

}
 
// Message handlers
BOOL CGlobalSettingsMoveSheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	initMode = true;
	SetDlgItemInt(IDC_DRAGDELAY,globalSettings.GetMoveDelayInMs() );
	SetDlgItemInt(IDC_GRIDWIDTH,globalSettings.GetGridX() );
	SetDlgItemInt(IDC_GRIDHEIGHT,globalSettings.GetGridY() );
	SetDlgItemInt(IDC_GRIDXMARGIN,globalSettings.GetGridXMargin() );
	SetDlgItemInt(IDC_GRIDYMARGIN,globalSettings.GetGridYMargin() );
	initMode = false;
	return TRUE;	
}

void CGlobalSettingsMoveSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && wNotifyCode == EN_CHANGE)
		SetModified();
}
 
int CGlobalSettingsMoveSheet::OnApply()
{
	globalSettings.SetMoveDelayInMs( GetDlgItemInt(IDC_DRAGDELAY) );
	globalSettings.SetGridX( GetDlgItemInt(IDC_GRIDWIDTH) );
	globalSettings.SetGridY( GetDlgItemInt(IDC_GRIDHEIGHT) );
	globalSettings.SetGridXMargin( GetDlgItemInt(IDC_GRIDXMARGIN) );
	globalSettings.SetGridYMargin( GetDlgItemInt(IDC_GRIDYMARGIN) );
	return TRUE;
};
