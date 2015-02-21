// filename:		CGlobalSettingsClickSheet.hpp
// Description:		Definition of class CGlobalSettingsClickSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CGlobalSettingsClickSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CGlobalSettingsClickSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

class AveDeskGlobalSettings;

class CGlobalSettingsClickSheet : public CPropertyPageImpl<CGlobalSettingsClickSheet>
{
private:
	AveDeskGlobalSettings& globalSettings;
	bool initMode;
	CComboBox clickModeBox;
public:
    enum { IDD = IDD_CLICKCONFIG };
 
	CGlobalSettingsClickSheet(AveDeskGlobalSettings& globalSettings);
    ~CGlobalSettingsClickSheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
 
	BEGIN_MSG_MAP_EX(CGlobalSettingsSelectSheet)
		COMMAND_ID_HANDLER_EX(IDC_CLICKINGMODE,OnEditValue)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CGlobalSettingsClickSheet>)
    END_MSG_MAP()

    int OnApply();
};
	
#endif//AFX_CGlobalSettingsClickSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_