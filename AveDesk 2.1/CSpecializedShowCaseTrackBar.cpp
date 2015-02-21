#include "stdafx.h"
#include "SpecializedShowCaseTrackBar.hpp"
#include "ShowCase1.h"

extern CComObject<CShowCase> *_ShowCase;


LRESULT CSpecializedShowCaseTrackBar::MouseDown(UINT msg, WPARAM w, LPARAM l)
{
	SetCapture();
	HWND hwnd = _ShowCase->GetHWND();
	RECT r = {0};
	::GetWindowRect(::GetDesktopWindow(),&r);
	::SetWindowPos(hwnd,HWND_TOPMOST,r.left,r.top,r.right - r.left,r.bottom - r.top,SWP_SHOWWINDOW);
	_ShowCase->SetAlpha(100);
	return FALSE;
}

LRESULT CSpecializedShowCaseTrackBar::MouseUp(UINT msg, WPARAM w, LPARAM l)
{
	HWND hwnd = _ShowCase->GetHWND();
	::ShowWindow(hwnd,SW_HIDE);
	::SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_HIDEWINDOW);
	ReleaseCapture();
	return FALSE;
}