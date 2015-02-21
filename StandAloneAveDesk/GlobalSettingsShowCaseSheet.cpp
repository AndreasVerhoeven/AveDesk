// filename:		CGlobalSettingsShowCaseSheet.hpp
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

#include "stdafx.h"
#include "GlobalSettingsShowCaseSheet.hpp"
#include "AveDeskGlobalSettings.hpp"
#include "resource.h"
#include <atlmisc.h>

#include "ShowCase1.h"

extern CComObject<CShowCase> *_ShowCase;


LRESULT __stdcall SubclassProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{

	if(msg==WM_LBUTTONDOWN)
	{
		if(_ShowCase)
		{
			::SetCapture(hwnd);
			SetPropW(hwnd,L"__AveMouseDown",(HANDLE)TRUE);
			HWND s_hwnd = _ShowCase->GetHWND();
			RECT r = {0};
			::GetWindowRect(::GetDesktopWindow(),&r);
			HDWP hDwp = ::BeginDeferWindowPos(2);
			::DeferWindowPos(hDwp,s_hwnd,HWND_TOPMOST,r.left,r.top,r.right - r.left,r.bottom - r.top,SWP_SHOWWINDOW);
			::DeferWindowPos(hDwp,::GetParent(::GetParent(hwnd)),(HWND)-1,0,0,0,0,SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
			::EndDeferWindowPos(hDwp);
			CTrackBarCtrl* ctrl = (CTrackBarCtrl*)GetPropW(hwnd,L"__AveWTLCtrl");

			//_ShowCase->SetAlpha(ctrl->GetPos());
			::SetLayeredWindowAttributes(s_hwnd,0,(BYTE)ctrl->GetPos(),LWA_ALPHA);
		}
	}
	else if(msg==WM_LBUTTONUP)
	{
		if(_ShowCase)
		{
			HWND s_hwnd = _ShowCase->GetHWND();
			::ShowWindow(s_hwnd,SW_HIDE);
			HDWP hDwp = ::BeginDeferWindowPos(2);
			::DeferWindowPos(hDwp,s_hwnd,HWND_TOPMOST,0,0,0,0,SWP_HIDEWINDOW);
			::DeferWindowPos(hDwp,::GetParent(::GetParent(hwnd)),HWND_NOTOPMOST,0,0,0,0,SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
			::EndDeferWindowPos(hDwp);
			SetPropW(hwnd,L"__AveMouseDown",(HANDLE)FALSE);
			ReleaseCapture();
		}
	}
	else if(msg==WM_MOUSEMOVE)
	{
		if(_ShowCase && (BOOL)GetPropW(hwnd,L"__AveMouseDown"))
		{
			HWND s_hwnd = _ShowCase->GetHWND();
			CTrackBarCtrl* ctrl = (CTrackBarCtrl*)GetPropW(hwnd,L"__AveWTLCtrl");
			//_ShowCase->SetAlpha(ctrl->GetPos());
			::SetLayeredWindowAttributes(s_hwnd,0,(BYTE)ctrl->GetPos(),LWA_ALPHA);
		}
	}

	return CallWindowProc((WNDPROC)GetPropW(hwnd,L"__AveOldProc"),hwnd,msg,w,l);
}

CGlobalSettingsShowCaseSheet::CGlobalSettingsShowCaseSheet(AveDeskGlobalSettings& globalSettings) :
	globalSettings(globalSettings), initMode(true)
{
}

CGlobalSettingsShowCaseSheet::~CGlobalSettingsShowCaseSheet()
{

}
 
// Message handlers
BOOL CGlobalSettingsShowCaseSheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	m_Anim = GetDlgItem(IDC_NUMFRAMES);

	m_Color = boost::shared_ptr<CColorButton>(new CColorButton);
	m_Color->SubclassWindow(GetDlgItem(IDBGCOLOR));


	m_Opacity = GetDlgItem(IDC_ALPHALEVEL);
	m_Opacity.SetRangeMax(255);

	long alpha(0);
	long numOfFrames(0);
	OLE_COLOR oClr = {0};
	BOOL doAnim(FALSE);
	if(_ShowCase)
	{
		_ShowCase->get_MaximumAlpha(&alpha);
		_ShowCase->get_NumberOfAnimationFrames(&numOfFrames);
		_ShowCase->get_Color(&oClr);
		VARIANT_BOOL vbDoAnim = { VARIANT_FALSE };
		_ShowCase->get_DoAnimation(&vbDoAnim);
		doAnim = (vbDoAnim == VARIANT_TRUE) ? TRUE : FALSE;
	}

	m_Opacity.SetPos(alpha);
	m_Anim.SetPos(numOfFrames);
	COLORREF clr = {0};
	OleTranslateColor(oClr,0,&clr);
	m_Color->SetColor(clr);
	m_Color->SetDefaultColor(clr);
	m_Color->SetDefaultText(IDS_CURRENTCOLOR);
	m_Color->SetCustomText(IDS_MORECOLORS);

	CheckDlgButton(IDC_DOANIM,doAnim);

	HWND trackBar = GetDlgItem(IDC_ALPHALEVEL);
	SetPropW(trackBar,L"__AveOldProc",(HANDLE)::GetWindowLong(trackBar,GWL_WNDPROC));
	SetPropW(trackBar,L"__AveWTLCtrl",(HANDLE)&m_Opacity);
	::SetWindowLong(trackBar,GWL_WNDPROC,(LONG)SubclassProc);

	_ShowCase->UnregisterHotKeys();

	hotKeyNormal		= GetDlgItem(IDC_HKSHOWCASE);
	hotKeyAllDesklets	= GetDlgItem(IDC_HKSHOWCASE2);

	HotKey hk  = _ShowCase->GetNormalHotKey();
	hotKeyNormal.SetHotKey(static_cast<WORD>(hk.vk),static_cast<WORD>(hk.modifiers));

	hk  = _ShowCase->GetAllDeskletsHotKey();
	hotKeyAllDesklets.SetHotKey(static_cast<WORD>(hk.vk),static_cast<WORD>(hk.modifiers));

	VARIANT_BOOL scIsEnabled = VARIANT_FALSE;
	_ShowCase->get_Enabled(&scIsEnabled);
	CheckDlgButton(IDC_ISENABLED,scIsEnabled == VARIANT_TRUE);

	VARIANT_BOOL isClickThru = VARIANT_FALSE;
	_ShowCase->get_IsClickThru(&isClickThru);

	CheckDlgButton(IDC_SHOWCASEISCLICKTHRU, isClickThru != VARIANT_FALSE);

	initMode = false;

	return TRUE;	
}
void CGlobalSettingsShowCaseSheet::OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar)
{
	if(!initMode)
		SetModified();
}

void CGlobalSettingsShowCaseSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(wID == IDC_HKSHOWCASE || wID == IDC_HKSHOWCASE2)
	{
		_ShowCase->UnregisterHotKeys();
	}

	if(!initMode && (wNotifyCode ==BN_CLICKED || wNotifyCode == CBN_SELCHANGE || wNotifyCode == EN_CHANGE ) )
	{
		SetModified();
	}
}

BOOL CGlobalSettingsShowCaseSheet::OnEditColor(LPNMHDR notifyData)
{
	if(!initMode && notifyData->code == CPN_SELCHANGE)
	{
		SetModified();
	}
	return TRUE;
}

BOOL CGlobalSettingsShowCaseSheet::OnDestroy()
{
	VARIANT_BOOL scIsEnabled = {FALSE};
	_ShowCase->get_Enabled(&scIsEnabled);

	if(scIsEnabled == VARIANT_TRUE)
		_ShowCase->RegisterHotKeys();

	return 0;
}

 
int CGlobalSettingsShowCaseSheet::OnApply()
{
	if(_ShowCase)
	{
		_ShowCase->put_Color((OLE_COLOR) m_Color->GetColor() );
		_ShowCase->put_MaximumAlpha(m_Opacity.GetPos() );
		_ShowCase->put_NumberOfAnimationFrames(m_Anim.GetPos() );
		_ShowCase->put_DoAnimation(IsDlgButtonChecked(IDC_DOANIM) == BST_CHECKED ? VARIANT_TRUE : VARIANT_FALSE);

		HotKey hk;
		USHORT vk,mod;
		hotKeyNormal.GetHotKey(vk,mod);
		hk.vk = vk;
		hk.modifiers = mod;
		_ShowCase->SetNormalHotKey(hk);

		vk = 0;
		mod = 0;
		hotKeyAllDesklets.GetHotKey(vk,mod);
		hk.vk = vk;
		hk.modifiers = mod;
		_ShowCase->SetAllDeskletsHotKey(hk);

		VARIANT_BOOL scIsEnabled = VARIANT_FALSE;
		if(IsDlgButtonChecked(IDC_ISENABLED))
			scIsEnabled = VARIANT_TRUE;
		_ShowCase->put_Enabled(scIsEnabled);

		VARIANT_BOOL isClickThru = VARIANT_FALSE;
		if(IsDlgButtonChecked(IDC_SHOWCASEISCLICKTHRU))
			isClickThru = VARIANT_TRUE;

		_ShowCase->put_IsClickThru(isClickThru);
	}

	return TRUE;
};
