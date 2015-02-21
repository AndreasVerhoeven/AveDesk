// filename:		DeskletAdvancedSheet.cpp
// Description:		Implementation of class CDeskletAdvancedSheet
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
#include "DeskletAdvancedSheet.hpp"
#include "docklet.h"

CDeskletAdvancedSheet::CDeskletAdvancedSheet(Docklet* d,_U_STRINGorID title) :
	CPropertyPageImpl<CDeskletAdvancedSheet>(title),
	docklet(d),
	initMode(false)
{

}

CDeskletAdvancedSheet::~CDeskletAdvancedSheet()
{

}

void CDeskletAdvancedSheet::OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar)
{
	if(!initMode)
		OnApply();
}

void CDeskletAdvancedSheet::OnResetButton(UINT wNotifyCode, int wId, HWND hwndCtrl)
{
	if(!initMode)
	{
		m_trackSaturation.SetPos(100);
		m_trackHue.SetPos(0);
		OnApply();
	}
}


void CDeskletAdvancedSheet::OnEditValue2(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && (wNotifyCode ==BN_CLICKED || wNotifyCode == CBN_SELCHANGE || wNotifyCode == EN_CHANGE ) )
		SetModified();
}

void CDeskletAdvancedSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && (wNotifyCode ==BN_CLICKED || wNotifyCode == CBN_SELCHANGE || wNotifyCode == EN_CHANGE ) )
	{
		OnApply();
	}
}

BOOL CDeskletAdvancedSheet::OnInitDialog (HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	m_comboQuality		= GetDlgItem(IDC_QUALITY);
	m_trackSaturation	= GetDlgItem(IDC_SATURATION);
	m_trackHue			= GetDlgItem(IDC_HUE);
	m_comboClicking     = GetDlgItem(IDC_CLICKING);

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

	m_comboQuality.SetCurSel(docklet->GetDocklet()->GetQuality());


	buf.LoadString(IDS_DESKLETCLICKINGDEFAULT);
	m_comboClicking.AddString(buf);
	buf.LoadString(IDS_DESKLETCLICKINGSINGLE);
	m_comboClicking.AddString(buf);
	buf.LoadString(IDS_DESKLETCLICKINGDOUBLE);
	m_comboClicking.AddString(buf);

	m_comboClicking.SetCurSel(docklet->GetDeskletClickingMode());



	CheckDlgButton(IDC_DROPSHADOW,docklet->GetDocklet()->GetDrawShadow());
	SetDlgItemInt(IDC_XOFFSET,docklet->GetDocklet()->GetShadowXOffset(),TRUE);
	SetDlgItemInt(IDC_YOFFSET,docklet->GetDocklet()->GetShadowYOffset(),TRUE);
	//SetDlgItemInt(IDC_GRAY,100 - (INT)((REAL)docklet->GetDocklet()->GetShadowGray()*100.0f),FALSE);
	SetDlgItemInt(IDC_ALPHA,(INT)((REAL)docklet->GetDocklet()->GetShadowAlpha()),FALSE);

	m_shadowColor.SubclassWindow(GetDlgItem(IDSHADOWCOLOR));
	COLORREF cr = (COLORREF)docklet->GetDocklet()->GetShadowColor();
	m_shadowColor.SetColor(cr);
	m_shadowColor.SetDefaultColor(cr);
	m_shadowColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_shadowColor.SetCustomText(IDS_MORECOLORS);

	CheckDlgButton(IDC_IGNORELEFTCLICKS,docklet->GetIgnoreLeftClicks());
	CheckDlgButton(IDC_IGNORERIGHTCLICKS,docklet->GetIgnoreRightClicks());
	CheckDlgButton(IDC_ISCLICKTHRU,docklet->GetClickThru());
	CheckDlgButton(IDC_SNAPTOGRID,docklet->GetSnapToGrid());
	m_trackSaturation.SetRange(0,800);
	m_trackSaturation.SetPos((int)(docklet->GetDocklet()->GetSaturation()*100.0f));

	m_trackHue.SetRange(-180,180);
	m_trackHue.SetPos(docklet->GetDocklet()->GetHue());

	CheckDlgButton(IDC_COLORIZE,docklet->GetDocklet()->GetUseColorization());
	
	initMode = false;

	return TRUE;
};

BOOL CDeskletAdvancedSheet::OnEditColor(LPNMHDR notifyData)
{
	if(!initMode && notifyData->code == CPN_SELCHANGE)
	{
		SetModified();
		OnApply();
	}
	return TRUE;
}

int CDeskletAdvancedSheet::OnApply()
{
	docklet->GetSnapToGrid() = IsDlgButtonChecked(IDC_SNAPTOGRID)==BST_CHECKED;
	docklet->GetIgnoreLeftClicks() = IsDlgButtonChecked(IDC_IGNORELEFTCLICKS)==BST_CHECKED;
	docklet->SetIgnoreRightClicks(IsDlgButtonChecked(IDC_IGNORERIGHTCLICKS)==BST_CHECKED ); 
	docklet->SetClickThru(IsDlgButtonChecked(IDC_ISCLICKTHRU)==BST_CHECKED); 

	docklet->SetDeskletClickingMode(m_comboClicking.GetCurSel());
	
	docklet->GetDocklet()->SetShadow(IsDlgButtonChecked(IDC_DROPSHADOW) == BST_CHECKED,
										GetDlgItemInt(IDC_XOFFSET,0,TRUE),
										GetDlgItemInt(IDC_YOFFSET,0,TRUE),
										(REAL)0.0f,
										GetDlgItemInt(IDC_ALPHA,0,FALSE),
										(LONG)m_shadowColor.GetColor(),
										false
									);

	docklet->GetDocklet()->SetColorization(IsDlgButtonChecked(IDC_COLORIZE)==BST_CHECKED,
											m_trackSaturation.GetPos() / 100.0f,
											m_trackHue.GetPos(),false
											);

	docklet->GetDocklet()->SetQuality(m_comboQuality.GetCurSel(),false);

	if(!docklet->GetIsDefaultValues())
	{
		docklet->Redraw();
		docklet->UpdateLabelWindow();
	}

	return TRUE;
}