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

#if !defined(AFX_CGlobalSettingsMoveSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CGlobalSettingsMoveSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

class AveDeskGlobalSettings;

class CGlobalSettingsMoveSheet : public CPropertyPageImpl<CGlobalSettingsMoveSheet>
{
private:
	AveDeskGlobalSettings& globalSettings;
	bool initMode;
public:
    enum { IDD = IDD_MOVECONFIG };

	CGlobalSettingsMoveSheet(AveDeskGlobalSettings& globalSettings);
    ~CGlobalSettingsMoveSheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl);

	BEGIN_MSG_MAP_EX(CGlobalSettingsMoveSheet)
		COMMAND_ID_HANDLER_EX(IDC_DRAGDELAY,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_GRIDWIDTH,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_GRIDHEIGHT,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_GRIDXMARGIN,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_GRIDYMARGIN,OnEditValue)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CGlobalSettingsMoveSheet>)
    END_MSG_MAP()

    int OnApply();
};
	
#endif//AFX_CGlobalSettingsMoveSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_