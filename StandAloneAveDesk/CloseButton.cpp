// CloseButton.cpp: implementation of the CCloseButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CloseButton.h"
#include "CloseButtonPolicy.h"
#include "helper_funcs.h"
#include "Docklet.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int CLOSEBUTTON_HIDE_TIMER = 7494329; // magic number
UINT _closeButtonHideInterval = 1000;

const int CLOSEBUTTON_FADE_TIMER = 94340; // magic number
UINT _closeButtonFadeInterval  = 10;

CCloseButton::CCloseButton() :
	m_RefCount(0),
	m_Docklet(NULL),
	m_Bitmap(NULL),
	m_HiddenParentHwnd(NULL),
	m_Alpha(0),
	m_ButtonIsVisible(false),
	m_MouseIsOver(false),
	m_IsFadingIn(false),
	m_DonotHide(false)
{
	WCHAR closeButtonPath[MAX_PATH] = {0};
	wcscpy(closeButtonPath, _App->GetAveDeskDataDir().c_str());
	PathAppendW(closeButtonPath, L"Images\\close.png");

	m_Bitmap = new Bitmap(closeButtonPath);

	SetupWindow();
}

void CCloseButton::SetDonotHide(bool val)
{
	m_DonotHide = val;
	if(m_DonotHide)
	{
		KillTimer(CLOSEBUTTON_HIDE_TIMER);
		if(!m_ButtonIsVisible)
			KillTimer(CLOSEBUTTON_FADE_TIMER);
	}
}

bool CCloseButton::GetDonotHide()const
{
	return m_DonotHide;
}

void CCloseButton::SetupWindow()
{
	//m_HiddenParentHwnd = CreateWindow(_T("AveDummyClass"),_T("AveDummyWindow"),0,0,1,1,0,0,0, _Module.GetModuleInstance(),0);

	RECT r = {0};
	Create(m_HiddenParentHwnd,r,_T("AveCloseButton"),WS_POPUP | WS_OVERLAPPED,WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	// we dont want this window to show up in the taskbar
	ModifyStyle(0,WS_POPUP | WS_OVERLAPPED);

	// initially, hide it and don't use the hiddent parent window anymore
	ShowWindow(SW_HIDE);
	//ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW,0);
//	SetParent(NULL);

	EnsureWindowIsLayered();

	if(m_Bitmap != NULL)
	{
		POINT p = {0};
		SIZE s = { m_Bitmap->GetWidth(), m_Bitmap->GetHeight() };
		SetBitmap(*m_Bitmap, m_Alpha, p, s); 
	}
}

LRESULT CCloseButton::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(CLOSEBUTTON_HIDE_TIMER == wParam)
	{
		if(!m_MouseIsOver)
		{
			KillTimer(CLOSEBUTTON_HIDE_TIMER);
			if(!m_DonotHide)
			{
				m_IsFadingIn = false;
				SetTimer(CLOSEBUTTON_FADE_TIMER, _closeButtonFadeInterval);
			}

		}
	}
	else if(CLOSEBUTTON_FADE_TIMER == wParam)
	{
		bool isDone = false;
		if(m_IsFadingIn)
		{
			m_Alpha += 10;
			isDone = (250 <= m_Alpha);
			if(isDone)
				m_Alpha = 255;
		}
		else
		{
			m_Alpha -= 10;
			isDone = (5 >= m_Alpha);
			if(isDone)
				m_Alpha = 0;
		}

		ChangeAlpha(m_Alpha);

		if(isDone)
		{
			KillTimer(CLOSEBUTTON_FADE_TIMER);
			if(0 == m_Alpha)
				ShowWindow(SW_HIDE);

		}
	}

	return 0;
}

LRESULT CCloseButton::OnWindowsPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WINDOWPOS*  wp = (WINDOWPOS*) lParam;
	if(wp != NULL && (wp->flags & SWP_NOZORDER) == 0)
		::SetWindowPos(m_Docklet->GetHWnd(), m_hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	return 0;
}

void CCloseButton::ResetPosition()
{
	SIZE bmpSize = { m_Bitmap->GetWidth(), m_Bitmap->GetHeight() };

	POINT p = m_Docklet->GetDocklet()->GetPos();
	POINT pClose = RemapPoint(m_Docklet->GetClosePoint(), m_Docklet->GetDocklet()->GetSize(), m_Docklet->GetClosePointScale().cx, m_Docklet->GetClosePointScale().cy);
	p.x += pClose.x - bmpSize.cx / 2;
	p.y += pClose.y - bmpSize.cy /2;

	SetWindowPos(NULL,p.x,p.y,0,0,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
}

void CCloseButton::NotifyZOrderingChange(HWND zOrder, HWND parent)
{
	if(::IsWindow(parent))
	{

		::SetParent(m_hWnd,parent);
		ModifyStyle(WS_CHILD,0);
	}
	else
	{
		// remove Program as a parent
		::SetParent(m_hWnd,NULL);
		LONG l = GetWindowLong(GWL_STYLE);
		l |= WS_POPUP;
		//l |= WS_CHILD;
		//l = l ^ WS_CHILD;
		l |= WS_OVERLAPPED;
		SetWindowLong(GWL_STYLE,l);
	}


	SetWindowPos(zOrder, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	//::BringWindowToTop(m_hWnd);
}

void CCloseButton::Show()
{
	if(!m_ButtonIsVisible && m_Docklet->GetUseClosePoint())
	{
		KillTimer(CLOSEBUTTON_HIDE_TIMER);
		m_ButtonIsVisible = true;

		ResetPosition();

		if(m_Alpha != 255)
		{
			m_IsFadingIn = true;
			SetTimer(CLOSEBUTTON_FADE_TIMER, _closeButtonFadeInterval);
		}
		ShowWindow(SW_SHOWNA);
	}	
}

void CCloseButton::Hide()
{
	if(m_ButtonIsVisible && !m_DonotHide)
	{
		m_ButtonIsVisible = false;
		SetTimer(CLOSEBUTTON_HIDE_TIMER, _closeButtonHideInterval);
	}
}

void CCloseButton::ChangeAlpha(BYTE bAlpha)
{
	m_Alpha = bAlpha;
	// work around for bug that setting alpha at 255 makes the window not alpha-blended.
	if(255 == m_Alpha)
	{
		SIZE s = { m_Bitmap->GetWidth(), m_Bitmap->GetHeight() };
		SetBitmap(*m_Bitmap, m_Alpha, NULL, &s); 
	}
	else
		SetAlpha(m_Alpha);
}

LRESULT CCloseButton::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(!m_MouseIsOver)
	{
		m_MouseIsOver = true;
	
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		BOOL bRet = TrackMouseEvent(&tme);

		Show();
	}

	return 0;
}

LRESULT CCloseButton::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_MouseIsOver)
	{
		m_MouseIsOver = false;
		Hide();
	}

	return 0;
}

LRESULT CCloseButton::OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetDonotHide(true);
	m_Docklet->RemoveWithAnimation();
	return 0;
}

LRESULT CCloseButton::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

CCloseButton::~CCloseButton()
{
	DestroyWindow();
	delete m_Bitmap;
}

CCloseButton* CCloseButton::CreateCloseButton(HWND hiddenParent)
{
	CCloseButton* button = new CCloseButton();
	button->m_HiddenParentHwnd = hiddenParent;
	button->AddRef();

	return button;
}

ULONG CCloseButton::AddRef()
{
	return ++m_RefCount;
}

ULONG CCloseButton::Release()
{
	m_RefCount--;
	if(m_RefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_RefCount;
}

void CCloseButton::NotifyChangedClosePoint()
{
	if(m_ButtonIsVisible)
	{
		if(m_Docklet->GetUseClosePoint())
		{
			ResetPosition();
		}
		else
		{
			NotifyDeskletHide();
		}
	}
}

void CCloseButton::NotifyDeskletHide()
{
	KillTimer(CLOSEBUTTON_HIDE_TIMER);
	KillTimer(CLOSEBUTTON_FADE_TIMER);

	SetWindowPos(m_Docklet->GetHwndInsertAfterFromStyle(),0,0,0,0,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
}

void CCloseButton::BindDocklet(Docklet* d)
{
	m_Docklet = d;
}

void CCloseButton::UnbindDocklet()
{
	m_Docklet = NULL;
}
