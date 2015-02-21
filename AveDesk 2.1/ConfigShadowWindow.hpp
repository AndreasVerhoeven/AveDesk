// filename:		ConfigShadowWindow.hpp
// Description:		Definition of class CConfigShadowWindow
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CConfigShadowWindow_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CConfigShadowWindow_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

#include "ColorButton.h"

class Label;

class CConfigShadowWindow : public CDialogImpl<CConfigShadowWindow>
{
private:
	Label& label;

	CColorButton m_butColor;

public:
    enum { IDD = IDD_LABELCONFIG };

	CConfigShadowWindow(Label& l);
    ~CConfigShadowWindow();

    LRESULT OnInitDialog (HWND hwndFocus, LPARAM lParam);
	LRESULT OnClose ();
	void OnOK(UINT wNotifyCode, int wID, HWND hwndCtrl);

	BEGIN_MSG_MAP_EX(CConfigShadowWindow)
		COMMAND_ID_HANDLER_EX(IDOK,OnOK)
		
        MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
        //CHAIN_MSG_MAP(CDialogImpl<CConfigShadowWindow>)
		REFLECT_NOTIFICATIONS()
    END_MSG_MAP();
};
	
#endif//AFX_CConfigShadowWindow_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_