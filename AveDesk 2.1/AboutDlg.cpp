// AboutDlg.cpp: implementation of the CAboutDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AboutDlg.h"
#include "helper_funcs.h"
#include "constants.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern Bitmap* __stdcall DeskletLoadGDIPlusImageW(const WCHAR*);

CAboutDlg::CAboutDlg()
{
	//m_Image = boost::shared_ptr<Bitmap>( DeskletLoadGDIPlusImageW(L"about.png") );
}


BOOL CALLBACK EnumHideChildProc(

    HWND hwnd,	// handle to child window
    LPARAM lParam 	// application-defined value
   )
{
	ShowWindow(hwnd, SW_HIDE);
	return TRUE;
}

LRESULT CAboutDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bool doEasterEgg = false;

	if(doEasterEgg)
	{
		RECT r = {0};
		GetClientRect(&r);
		int width = r.right - r.left;
		int height = r.bottom - r.top;
		m_Bmp = boost::shared_ptr<Bitmap>(new Bitmap(width, height));
		Graphics g(m_Bmp.get());
//		HDC hdc = g.GetHDC();
	//	::SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM)hdc, (LPARAM)  PRF_CLIENT	| PRF_CHILDREN);
	//	g.ReleaseHDC(hdc);
		g.Clear(Color(255,255,255,255));

		Rect rc(0,0, width, height);
		m_Bmp->LockBits(&rc, ImageLockModeRead, PixelFormat32bppARGB, &m_Data);
		m_Anim = boost::shared_ptr<CWaterAnimation>(new CWaterAnimation(width, height));

		::SetTimer(m_hWnd, 1, 10, NULL);

		EnumChildWindows(m_hWnd, EnumHideChildProc, 0L);
	}

	m_Ave.SetHyperLink(_T("mailto:averhoev.2@hccnet.nl?subject=AveDesk"));
	m_Herd.SetHyperLink(_T("mailto:Hakan.Erduman@web.de?subject=AveDesk"));
	m_KoL.SetHyperLink(_T("mailto:franco_028@hotmail.com?subject=AveDesk"));

	m_Ave.SubclassWindow(GetDlgItem(IDC_AVE));
	m_Herd.SubclassWindow(GetDlgItem(IDC_HERD));
	m_KoL.SubclassWindow(GetDlgItem(IDC_KOL));

	HICON hIcon = (HICON) ::LoadImage(_Module.GetModuleInstance(),MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON, 64, 64,LR_DEFAULTCOLOR);
	m_Icon = GetDlgItem(IDC_ABOUTICON);
	m_Icon.SetIcon(hIcon);

	return 0;
}

LRESULT CAboutDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_Anim.reset();
	if(m_Bmp.get() != NULL)
		m_Bmp->UnlockBits(&m_Data);
	m_Bmp.reset();

	m_Ave.UnsubclassWindow();
	m_Herd.UnsubclassWindow();
	m_KoL.UnsubclassWindow();

	DestroyIcon(m_Icon.GetIcon());

	return 0;
}

LRESULT CAboutDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(1 == wParam)
	{
		if(m_Anim.get() != NULL)
		{
			m_Anim->makeDrip(40,40,48);
			m_Anim->nextAnimationStep();
			::InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}

	return 0;
}

LRESULT CAboutDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_Anim.get() != NULL)
	{
		PAINTSTRUCT ps = {0};
		HDC hdc = ::BeginPaint(m_hWnd, &ps);
		ImagePixelBuffer buf = {0};
		buf.height = m_Bmp->GetHeight();
		buf.width  = m_Bmp->GetWidth();
		buf.pixelBuffer = (DWORD*)m_Data.Scan0;
		buf.stride = m_Data.Stride;

		m_Anim->draw(hdc, 0, 0, &buf);

		::EndPaint(m_hWnd, &ps);

		return 1;
	}
	else
	{
		return ::DefWindowProc(m_hWnd, WM_PAINT, wParam, lParam);
	}
}

LRESULT CAboutDlg::OnCloseDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	EndDialog(TRUE);

	return 0;
}


LRESULT CAboutDlg::OnLeftButtonDoubleClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	ShellExecuteW(NULL,L"open",L"http://avedesk.needlemen.com",NULL,NULL,SW_SHOW);

	return 0;
}

LRESULT CAboutDlg::OnCloseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = TRUE;

	EndDialog(TRUE);

	return 0;
}