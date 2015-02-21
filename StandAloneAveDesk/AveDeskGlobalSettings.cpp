// filename:		AveDeskGlobalSettings.cpp
// Description:		Implementation of class AveDeskGlobalSettings
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
#include "AveDeskGlobalSettings.hpp"
#include "helper_funcs.h"
#include "GlobalSettingsMoveSheet.hpp"
#include "GlobalSettingsSelectSheet.hpp"
#include "GlobalSettingsClickSheet.hpp"
#include "GlobalSettingsTraySheet.hpp"
#include "GlobalSettingsShowCaseSheet.hpp"
#include "GlobalSettingsOtherSheet.hpp"
#include "GlobalSettingsToolsSheet.hpp"
#include "GlobalSettingsEffectSheet.h"
#include "Application.h"

extern CComObject<CApplication>* _App;

AveDeskGlobalSettings::AveDeskGlobalSettings()
{
	// post: an AveDeskGlobalSettings object with default values has been created

	// NOTE: the default values are defined here thus.

	USES_CONVERSION;

	SetNoMultipleUserAwareness(false);


	SetMoveDelayInMs(200);
	SetClickMode(CLICKMODE_SINGLE);
	SetGridX(128);
	SetGridY(128);
	SetGridXMargin(0);
	SetGridYMargin(0);
	SetHoverSelection(false);
	SetHoverSelectionDelay(500);
	SetAsk(true);
	SetShowTrayIcon(true);
	SetAutoSaveThemes(true);
	SetTrayAnimationFolder(L"Rotation");
	SetDoTrayAnimation(true);

	WCHAR tmp[MAX_PATH] = {0};
	GetModuleFileNameW(NULL,tmp,sizeof(tmp)/sizeof(tmp[0]));
	SetIconFile(tmp,1,GetShowTrayIcon() );

	SetAddPreviewInThemeFiles(true);

	m_ThemeFileName = L"CurrentTheme.avetheme";

	CString str;
	str.LoadString(IDS_INITIALTHEMEDIR);

	SetThemeDir(T2W(const_cast<TCHAR*>(str.operator LPCTSTR())));

	SetDisableFlipEffect(false);
	SetFlippingSpeed(10);

	SetDisableCloseEffect(false);
	SetClosingSpeed(25);
	SetFadeOutWhileDoingClosingEffect(true);

	SetFirst13Boot(true);

	SetIconPreviewSize(80);

	SetDeskletBrowserSplitterPos(0);
}

bool AveDeskGlobalSettings::IsNoMultipleUserAwareness()const
{
	// post: true has been returned if AveDesk is not aware of multiple users
	return m_NoMultipleUserAwareness;
}

void AveDeskGlobalSettings::SetNoMultipleUserAwareness(const bool val)
{
	// post: m_NoMultipleUserAwareness has been set to 
	m_NoMultipleUserAwareness = val;
}

bool AveDeskGlobalSettings::SetIconFile(const WCHAR* file, long index, bool showIcon)
{
	// pre: file is not NULL
	// post: true has been returned iff file, index has been set for the current tray icon and
	//		 showIcon defined the visibility of the trayicon.

	if(!file)
		return false;

	bool oldShowIcon = GetShowTrayIcon();

	SetIconFileName(file);
	SetIconIndex(index);
	SetShowTrayIcon(showIcon);

	HWND iconHwnd = NULL;
	if(_App != NULL)
		_App->GetIconWindow().m_hWnd;
	NOTIFYICONDATAW _ncd = {0};
	_ncd.hWnd = iconHwnd;
	if(iconHwnd != NULL)
	{
		if(showIcon)
		{
			_ncd.hIcon = ExtractIconW(_Module.GetModuleInstance(),GetIconFileName().c_str(),GetIconIndex() );
			_ncd.uCallbackMessage = ICON_CALLBACK;
			_ncd.uID = 99;
			_ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

			if(oldShowIcon == false)
				Shell_NotifyIconW(NIM_ADD,&_ncd);
			else
				Shell_NotifyIconW(NIM_MODIFY,&_ncd);

			DestroyIcon(_ncd.hIcon);
		}
		else
		{
			if(oldShowIcon == true)
				Shell_NotifyIconW(NIM_DELETE,&_ncd);
		}
	}

	return true;
}

int AveDeskGlobalSettings::ShowDialog()
{
	// post: the result value of a shown propertysheet for these settings has been returned.
	// side-effect: a modal dialog is shown

	CGlobalSettingsMoveSheet ms(*this);
	CGlobalSettingsSelectSheet ss(*this);
	CGlobalSettingsClickSheet cs(*this);
	CGlobalSettingsTraySheet trs(*this);
	CGlobalSettingsToolsSheet tos;
	CGlobalSettingsShowCaseSheet scs(*this);
	CGlobalSettingsEffectSheet es(*this);
	CGlobalSettingsOtherSheet os(*this);

	CPropertySheet ps(IDS_GLOBALSETTINGSTITLE);
	ps.AddPage (ms);
	ps.AddPage (ss);
	ps.AddPage (cs);
	ps.AddPage (trs);
	ps.AddPage (tos);
	ps.AddPage (scs);
	ps.AddPage (es);
	ps.AddPage (os);

	int res = ps.DoModal();

	return res;

}