// filename:		DefaultDeskletLabelSheet.hpp
// Description:		Definition of class CDefaultDeskletLabelSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CDefaultDeskletLabelSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CDefaultDeskletLabelSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

#include "ColorButton.h"

#include "LabelProperties.h"

class CDefaultDeskletLabelSheet : public CPropertyPageImpl<CDefaultDeskletLabelSheet>
{
private:
	bool initMode;

	CComboBox	m_comboLabelType;
	CButton		m_checkOnlyVisibleOnMouseOver;
	CButton		m_checkSubLabel;
	CButton		m_checkShadow;
	CButton		m_checkShadow2;
	CComboBox	m_comboBackground;
	CComboBox	m_comboHorAlign;
	CComboBox	m_comboVerAlign;
	CEdit		m_editRotation;
	CButton		m_checkTopDown;
	CComboBox	m_comboPosition;
	CEdit		m_editMargin;
	CUpDownCtrl	m_spinRotation;
	CEdit		m_editMaxWidth;
	CEdit		m_editMaxHeight;

	CColorButton m_butNormalColor;
	CColorButton m_butSubColor;

	void FillLabelBackgroundComboBox();

	bool SelectFontProps(CLabelProperties& lp);

public:
    enum { IDD = IDD_LABEL };

	CDefaultDeskletLabelSheet(_U_STRINGorID title=IDS_TAB1);
    ~CDefaultDeskletLabelSheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnSelectFont(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnSelectSubFont(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnShadowSettings(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnSubShadowSettings(UINT wNotifyCode, int wID, HWND hwndCtrl);

	BOOL OnEditColor(LPNMHDR notifyData);

	BEGIN_MSG_MAP_EX(CDefaultDeskletLabelSheet)
		COMMAND_ID_HANDLER_EX(IDCONFIGSHADOW,OnShadowSettings)
		COMMAND_ID_HANDLER_EX(IDCONFIGSUBSHADOW,OnSubShadowSettings)
		COMMAND_ID_HANDLER_EX(IDSELECTFONT,OnSelectFont)
		COMMAND_ID_HANDLER_EX(IDSELECTFONT2,OnSelectSubFont)
		NOTIFY_ID_HANDLER_EX(IDSELECTCOLOR,OnEditColor)
		NOTIFY_ID_HANDLER_EX(IDSELECTCOLOR2,OnEditColor)
		COMMAND_ID_HANDLER_EX(IDC_LABELTYPE,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_LABELONLYONMOUSEOVER,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_SUBLABEL,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_SHADOW,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_SHADOW2,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_LABELBACKGROUND,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_HALIGN2,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_VALIGN2,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ROTATION,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_TOPDOWN,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_POSITION,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_MARGIN,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_WIDTH,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_HEIGHT,OnEditValue)
		
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CDefaultDeskletLabelSheet>)
		REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    int OnApply();
};
	
#endif//AFX_CDefaultDeskletLabelSheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_