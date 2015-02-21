

// filename:		DefaultDeskletAdvancedSheet.cpp
// Description:		Implementation of class CDefaultDeskletAdvancedSheet
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
#include "DefaultDeskletAdvancedSheet.hpp"
#include "docklet.h"

CDefaultDeskletAdvancedSheet::CDefaultDeskletAdvancedSheet(_U_STRINGorID title) :
	CPropertyPageImpl<CDefaultDeskletAdvancedSheet>(title),
	initMode(false)
{

}

CDefaultDeskletAdvancedSheet::~CDefaultDeskletAdvancedSheet()
{

}

void CDefaultDeskletAdvancedSheet::OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar)
{
	if(!initMode)
	{
		SetModified();
	}
}

void CDefaultDeskletAdvancedSheet::OnResetButton(UINT wNotifyCode, int wId, HWND hwndCtrl)
{
	if(!initMode)
	{
		m_trackSaturation.SetPos(100);
		m_trackHue.SetPos(0);
		SetModified();
	}
}

void CDefaultDeskletAdvancedSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && (wNotifyCode ==BN_CLICKED || wNotifyCode == CBN_SELCHANGE || wNotifyCode == EN_CHANGE ) )
	{
		SetModified();
	}
}


BOOL CDefaultDeskletAdvancedSheet::OnEditColor(LPNMHDR notifyData)
{
	if(!initMode && notifyData->code == CPN_SELCHANGE)
	{
		SetModified();
	}
	return TRUE;
}

BOOL CDefaultDeskletAdvancedSheet::OnInitDialog (HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	m_comboQuality		= GetDlgItem(IDC_QUALITY);
	m_trackSaturation	= GetDlgItem(IDC_SATURATION);
	m_trackHue			= GetDlgItem(IDC_HUE);

	CString buf;
	buf.LoadString(IDS_QUALITY0);
	m_comboQuality.AddString(buf);
	buf.LoadString(IDS_QUALITY1);
	m_comboQuality.AddString(buf);
	buf.LoadString(IDS_QUALITY2);
	m_comboQuality.AddString(buf);
	buf.LoadString(IDS_QUALITY3);
	m_comboQuality.AddString(buf);
	buf.LoadString(IDS_QUALITY4);
	m_comboQuality.AddString(buf);

	m_comboQuality.SetCurSel(Docklet::defaultQuality );

	CheckDlgButton(IDC_DROPSHADOW,Docklet::defaultDrawShadow);
	SetDlgItemInt(IDC_XOFFSET,Docklet::defaultShadowX,TRUE);
	SetDlgItemInt(IDC_YOFFSET,Docklet::defaultShadowY,TRUE);
	SetDlgItemInt(IDC_ALPHA,(INT)((REAL)Docklet::defaultShadowAlpha),FALSE);

	m_shadowColor.SubclassWindow(GetDlgItem(IDSHADOWCOLOR));
	COLORREF cr = (COLORREF)Docklet::defaultShadowColor;
	m_shadowColor.SetColor(cr);
	m_shadowColor.SetDefaultColor(cr);
	m_shadowColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_shadowColor.SetCustomText(IDS_MORECOLORS);


	CheckDlgButton(IDC_IGNORELEFTCLICKS,Docklet::defaultIgnoreLeftClicks);
	CheckDlgButton(IDC_IGNORERIGHTCLICKS,Docklet::defaultIgnoreRightClicks);
	CheckDlgButton(IDC_ISCLICKTHRU,Docklet::defaultIsClickThru);
	CheckDlgButton(IDC_SNAPTOGRID,Docklet::defaultSnapToGrid);
	m_trackSaturation.SetRange(0,800);
	m_trackSaturation.SetPos((int)(Docklet::defaultSaturation*100.0f));

	m_trackHue.SetRange(-180,180);
	m_trackHue.SetPos(Docklet::defaultHue);

	CheckDlgButton(IDC_COLORIZE,Docklet::defaultUseColorization);
	
	initMode = false;

	return TRUE;
};

int CDefaultDeskletAdvancedSheet::OnApply()
{
	Docklet::defaultSnapToGrid = IsDlgButtonChecked(IDC_SNAPTOGRID)==BST_CHECKED;
	Docklet::defaultIgnoreLeftClicks = IsDlgButtonChecked(IDC_IGNORELEFTCLICKS)==BST_CHECKED;
	Docklet::defaultIgnoreRightClicks = IsDlgButtonChecked(IDC_IGNORERIGHTCLICKS)==BST_CHECKED; 
	Docklet::defaultIsClickThru =IsDlgButtonChecked(IDC_ISCLICKTHRU)==BST_CHECKED; 

	Docklet::defaultDrawShadow = IsDlgButtonChecked(IDC_DROPSHADOW) == BST_CHECKED;
	Docklet::defaultShadowX = GetDlgItemInt(IDC_XOFFSET,0,TRUE);
	Docklet::defaultShadowY = GetDlgItemInt(IDC_YOFFSET,0,TRUE);
	Docklet::defaultShadowGray = 0.0f;
	Docklet::defaultShadowAlpha = GetDlgItemInt(IDC_ALPHA,0,FALSE);
	Docklet::defaultShadowColor = (LONG) m_shadowColor.GetColor();

	Docklet::defaultUseColorization = IsDlgButtonChecked(IDC_COLORIZE)==BST_CHECKED;
	Docklet::defaultSaturation = m_trackSaturation.GetPos() / 100.0f;
	Docklet::defaultHue = m_trackHue.GetPos();

	Docklet::defaultQuality = m_comboQuality.GetCurSel();


	return TRUE;
}
