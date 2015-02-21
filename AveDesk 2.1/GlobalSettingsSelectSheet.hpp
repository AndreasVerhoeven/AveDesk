// filename:		CGlobalSettingsSelectSheet.hpp
// Description:		Definition of class CGlobalSettingsSelectSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CGlobalSettingsSelectSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CGlobalSettingsSelectSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

class AveDeskGlobalSettings;

class CGlobalSettingsSelectSheet : public CPropertyPageImpl<CGlobalSettingsSelectSheet>
{
private:
	AveDeskGlobalSettings& globalSettings;
	bool initMode;
public:
    enum { IDD = IDD_SELECTCONFIG };
 
	CGlobalSettingsSelectSheet(AveDeskGlobalSettings& globalSettings);
    ~CGlobalSettingsSelectSheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
 
	BEGIN_MSG_MAP_EX(CGlobalSettingsSelectSheet)
		COMMAND_ID_HANDLER_EX(IDC_HOVERSELECTION,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_SELECTIONDELAY,OnEditValue)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CGlobalSettingsSelectSheet>)
    END_MSG_MAP()

    int OnApply();
};
	
#endif//AFX_CGlobalSettingsSelectSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_