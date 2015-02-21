// filename:		CGlobalSettingsShowCaseSheet.hpp
// Description:		Definition of class CGlobalSettingsShowCaseSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CGlobalSettingsShowCaseSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CGlobalSettingsShowCaseSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

#include "ColorButton.h"

#include <memory>

class AveDeskGlobalSettings;

class CGlobalSettingsShowCaseSheet : public CPropertyPageImpl<CGlobalSettingsShowCaseSheet>
{
private:
	AveDeskGlobalSettings& globalSettings;

	CTrackBarCtrl m_Opacity, m_Anim;
	bool initMode;
	boost::shared_ptr<CColorButton>	m_Color;
	CHotKeyCtrl hotKeyNormal, hotKeyAllDesklets;
public:
    enum { IDD = IDD_SHOWCASECONFIG };
 
	CGlobalSettingsShowCaseSheet(AveDeskGlobalSettings& globalSettings);
    ~CGlobalSettingsShowCaseSheet();

	void OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar); 

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
	BOOL OnDestroy();

	BOOL OnEditColor(LPNMHDR notifyData);
 
	BEGIN_MSG_MAP_EX(CGlobalSettingsSelectSheet)
		COMMAND_ID_HANDLER_EX(IDC_ALPHALEVEL,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_HKSHOWCASE,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_HKSHOWCASE2,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_NUMFRAMES,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_DOANIM,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ISENABLED,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_SHOWCASEISCLICKTHRU,OnEditValue)
        MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_HSCROLL(OnHScroll)

		NOTIFY_ID_HANDLER_EX(IDBGCOLOR,OnEditColor)
		/*COMMAND_ID_HANDLER_EX(IDC_QUALITY,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_COLORIZE,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ALPHA,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_XOFFSET,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_YOFFSET,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_IGNORELEFTCLICKS,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_IGNORERIGHTCLICKS,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_DROPSHADOW,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ISCLICKTHRU,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_SNAPTOGRID,OnEditValue)*/

        CHAIN_MSG_MAP(CPropertyPageImpl<CGlobalSettingsShowCaseSheet>)
		REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    int OnApply();
};
	
#endif//AFX_CGlobalSettingsShowCaseSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_