// filename:		DefaultDeskletAdvancedSheet.hpp
// Description:		Definition of class CDefaultDeskletAdvancedSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CDefaultDeskletAdvancedSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CDefaultDeskletAdvancedSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

#include "ColorButton.h"

class Docklet;

class CDefaultDeskletAdvancedSheet : public CPropertyPageImpl<CDefaultDeskletAdvancedSheet>
{
private:
	bool initMode;

	CComboBox		m_comboQuality;
	CTrackBarCtrl	m_trackSaturation;
	CTrackBarCtrl	m_trackHue;
	CColorButton	m_shadowColor;

public:
    enum { IDD = IDD_ADVANCED };

	CDefaultDeskletAdvancedSheet(_U_STRINGorID title=IDS_TAB2);
    ~CDefaultDeskletAdvancedSheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnResetButton(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar); 

	BOOL OnEditColor(LPNMHDR notifyData);

	BEGIN_MSG_MAP_EX(CDefaultDeskletAdvancedSheet)
		COMMAND_ID_HANDLER_EX(IDC_RESET,OnResetButton)
		NOTIFY_ID_HANDLER_EX(IDSHADOWCOLOR,OnEditColor)
		COMMAND_ID_HANDLER_EX(IDC_QUALITY,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_COLORIZE,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ALPHA,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_XOFFSET,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_YOFFSET,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_IGNORELEFTCLICKS,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_IGNORERIGHTCLICKS,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_DROPSHADOW,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ISCLICKTHRU,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_SNAPTOGRID,OnEditValue)

		MSG_WM_HSCROLL(OnHScroll)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CDefaultDeskletAdvancedSheet>)
		REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    int OnApply();
};
	
#endif//AFX_CDefaultDeskletAdvancedSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_