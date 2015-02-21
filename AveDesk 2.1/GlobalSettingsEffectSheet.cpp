// GlobalSettingsEffectSheet.cpp: implementation of the CGlobalSettingsEffectSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalSettingsEffectSheet.h"

#include "AveDeskGlobalSettings.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalSettingsEffectSheet::CGlobalSettingsEffectSheet(AveDeskGlobalSettings& globalSettings) :
	globalSettings(globalSettings),
	initMode(false)
{

}
   
CGlobalSettingsEffectSheet::~CGlobalSettingsEffectSheet()
{

}

void CGlobalSettingsEffectSheet::ChangeStatusOfFlipControls(BOOL bEnabled)
{
	::EnableWindow(GetDlgItem(IDC_STATIC_FLIP_SPEED), bEnabled);
	::EnableWindow(GetDlgItem(IDC_STATIC_FLIP_SLOW),  bEnabled);
	::EnableWindow(GetDlgItem(IDC_STATIC_FLIP_FAST),  bEnabled);
	::EnableWindow(GetDlgItem(IDC_FLIPSPEED),         bEnabled);
	::EnableWindow(GetDlgItem(IDBUTRESETFLIP),        bEnabled);
}

void CGlobalSettingsEffectSheet::ChangeStatusOfCloseControls(BOOL bEnabled)
{
	::EnableWindow(GetDlgItem(IDC_STATIC_CLOSE_SPEED), bEnabled);
	::EnableWindow(GetDlgItem(IDC_STATIC_CLOSE_SLOW),  bEnabled);
	::EnableWindow(GetDlgItem(IDC_STATIC_CLOSE_FAST),  bEnabled);
	::EnableWindow(GetDlgItem(IDC_CLOSESPEED),         bEnabled);
	::EnableWindow(GetDlgItem(IDBUTRESETCLOSE),        bEnabled);
	::EnableWindow(GetDlgItem(IDC_CLOSEFADEOUT),       bEnabled);
}


BOOL CGlobalSettingsEffectSheet::OnInitDialog (HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	m_FlipSpeed = GetDlgItem(IDC_FLIPSPEED);
	m_FlipSpeed.SetRangeMin(1);
	m_FlipSpeed.SetRangeMax(45);

	m_CloseSpeed = GetDlgItem(IDC_CLOSESPEED);
	m_CloseSpeed.SetRangeMin(5);
	m_CloseSpeed.SetRangeMax(100);

	m_FlipSpeed.SetPos(globalSettings.GetFlippingSpeed() );
	m_CloseSpeed.SetPos(globalSettings.GetClosingSpeed() );

	CheckDlgButton(IDC_ENABLEFLIP, !globalSettings.GetDisableFlipEffect());
	CheckDlgButton(IDC_ENABLECLOSE, !globalSettings.GetDisableCloseEffect());

	CheckDlgButton(IDC_CLOSEFADEOUT, globalSettings.GetFadeOutWhileDoingClosingEffect());

	ChangeStatusOfFlipControls(IsDlgButtonChecked(IDC_ENABLEFLIP));
	ChangeStatusOfCloseControls(IsDlgButtonChecked(IDC_ENABLECLOSE));

	initMode = false;

	return TRUE;
}


void CGlobalSettingsEffectSheet::OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar)
{
	if(!initMode)
	{
		SetModified();
	}
}

void CGlobalSettingsEffectSheet::OnEditFlipValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode)
	{
		ChangeStatusOfFlipControls(IsDlgButtonChecked(IDC_ENABLEFLIP));
		SetModified();
	}
}

void CGlobalSettingsEffectSheet::OnEditCloseValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode)
	{
		ChangeStatusOfCloseControls(IsDlgButtonChecked(IDC_ENABLECLOSE));
		SetModified();
	}
}

void CGlobalSettingsEffectSheet::OnFlipResetButton(UINT wNotifyCode, int wId, HWND hwndCtrl)
{
	m_FlipSpeed.SetPos(10);
	SetModified();
}

void CGlobalSettingsEffectSheet::OnCloseResetButton(UINT wNotifyCode, int wId, HWND hwndCtrl)
{
	m_CloseSpeed.SetPos(25);
	SetModified();
}
 

int CGlobalSettingsEffectSheet::OnApply()
{
	globalSettings.SetDisableFlipEffect(IsDlgButtonChecked(IDC_ENABLEFLIP) != BST_CHECKED);
	globalSettings.SetFlippingSpeed( m_FlipSpeed.GetPos() );

	globalSettings.SetDisableCloseEffect(IsDlgButtonChecked(IDC_ENABLECLOSE) != BST_CHECKED);
	globalSettings.SetClosingSpeed( m_CloseSpeed.GetPos() );
	globalSettings.SetFadeOutWhileDoingClosingEffect( IsDlgButtonChecked(IDC_CLOSEFADEOUT) == BST_CHECKED);

	return TRUE;
}