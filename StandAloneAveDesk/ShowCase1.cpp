// ShowCase1.cpp : Implementation of CAveDeskApp and DLL registration.

#include "stdafx.h"
#include "AveDesk.h"
#include "ShowCase1.h"
#include "Docklet.h"
#include "helper_funcs.h"
#include "ColorMatrixHelper.h"

#include <map>

#define COMPILE_MULTIMON_STUBS 
#include <multimon.h> 

#pragma pack(push, 1)
typedef struct _DWM_BLURBEHIND {
    DWORD dwFlags;
    BOOL fEnable;
    HRGN hRgnBlur;
    BOOL fTransitionOnMaximized;
} DWM_BLURBEHIND, *PDWM_BLURBEHIND;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
//

CShowCase::CShowCase() :
	doAnimation(VARIANT_TRUE),
	numOfAnimFrames(10),
	maxAlpha(200),
	color(RGB(0,0,0)),
	isActive(VARIANT_FALSE),
	hotKeysAreRegistered(FALSE),
	isAnimating(FALSE),
	normalHotKey(new HotKey),
	allDeskletsHotKey(new HotKey),
	isEnabled(VARIANT_TRUE),
	m_DrawState(DrawNone),
	m_IsActiveOnAllDesklets(false),
	isClickThru(VARIANT_FALSE)
{
	normalHotKey->id			= 2934894;	// BEWARE: MAGIC CONSTANT
	allDeskletsHotKey->id		= 2023919;	// BEWARE: MAGIC CONSTANT

	normalHotKey->modifiers = 0;
	allDeskletsHotKey->modifiers = 0;

	normalHotKey->vk = VK_F8;
	allDeskletsHotKey->vk = VK_F9;

	RECT r = {0};
	Create(_App->GetMainWindowHandle(),r,_T("ShowCase"),WS_POPUP | WS_OVERLAPPED,WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	// we dont want this window to show up in the taskbar
	SetParent(NULL);
	ShowWindow(SW_HIDE);
	DeleteObject( (HGDIOBJ)GetClassLong(m_hWnd,GCL_HBRBACKGROUND));
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)CreateSolidBrush(color) );
	DWORD newExFlags = WS_EX_TOOLWINDOW;
	// this is the only property that cannot be set thru the COM interface
	if(isClickThru != VARIANT_FALSE)
		newExFlags |= WS_EX_TRANSPARENT;
	ModifyStyleEx(WS_EX_APPWINDOW,newExFlags,0);

	EnsureWindowIsLayered();

	typedef HRESULT (CALLBACK *dwmE)(HWND, DWM_BLURBEHIND *);

   dwmE _dwmE;
   HRESULT hr = S_OK;

  HINSTANCE hInstLibrary = LoadLibrary(L"dwmapi.dll");

   if (hInstLibrary != NULL)
   {   
	_dwmE = (dwmE)GetProcAddress(hInstLibrary, "DwmEnableBlurBehindWindow");

	DWM_BLURBEHIND bb = {0};

	bb.dwFlags = 0x00000001 | 2;
	bb.fEnable = TRUE;
	bb.hRgnBlur = NULL;

	hr = _dwmE(m_hWnd, &bb);
	if (SUCCEEDED(hr))
	{
		  // ble ble ble
	}
	FreeLibrary(hInstLibrary);
   }

}

CShowCase::~CShowCase()
{
	if(::IsWindow(m_hWnd))
		DestroyWindow();
}

LRESULT CShowCase::OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Deactivate();
	return 0;
}

LRESULT CShowCase::OnEraseBackground(HDC hdc)
{
	// post: the background has been erased

	// we double-buffer, so don't do anything here.

	RECT rect = {0};
	GetClientRect(&rect);

	CDCHandle dc = hdc;
	dc.FillSolidRect(&rect,RGB(0, 0, 0));
	return 1;
}

void CShowCase::OnPaint(HDC dc)
{
	// pre:  dc must be a valid HDC, m_DrawState must have been set to the required state.
	// post: all desklets that are indicated by m_DrawState have been drawn upon the hdc, along with
	//			ShowCase background.

	int xVirt = -GetSystemMetrics( SM_XVIRTUALSCREEN );
	int yVirt = -GetSystemMetrics( SM_YVIRTUALSCREEN ); 

	RECT r = {0};
	GetWindowRect(&r);
	SIZE s = SIZEFROMRECT(r);
	if(0 == s.cx || 0 == s.cy)
		return;

	ATLTRACE("showcase onpaint: init\n");
	Bitmap buffer(s.cx, s.cy);
	Graphics g(&buffer);
	ATLTRACE("showcase onpaint: GDI+ buffer allocated\n");



	HDC hdc = g.GetHDC();
	CDC paintDC(hdc);//m_hWnd);

	//if(m_DrawState == DrawNone)
	paintDC.FillRect(&r,(HBRUSH)GetClassLong(m_hWnd,GCL_HBRBACKGROUND));
	paintDC.Detach();
	g.ReleaseHDC(hdc);
	ATLTRACE("showcase onpaint: bg filled\n");

	//Graphics g(paintDC);
	boost::shared_ptr<Bitmap> bmp( DeskletLoadGDIPlusImageW(L"Images\\showcasebg.png") );
	ATLTRACE("showcase onpaint: showcasebg.png loaded\n");

	int left = s.cx / 2 - bmp->GetWidth()  / 2;
	int top  = s.cy / 2 - bmp->GetHeight() / 2;

	g.DrawImage(bmp.get(),left,top,bmp->GetWidth(),bmp->GetHeight() );
	ATLTRACE("showcase onpaint: showcasebg.png drawn\n");

	if(m_DrawState != DrawNone)
	{
		std::vector<Docklet*> docklets;

		EnterCriticalSection(&Docklet::cs);
		for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
		{
			docklets.push_back(iter->second);
		}
		LeaveCriticalSection(&Docklet::cs);

		ATLTRACE("showcase onpaint: desklet iterated\n");

		for(std::vector<Docklet*>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
		{
			Docklet* d= *d_iter;
			ATLTRACE("showcase onpaint: desklet drawing init\n");

			bool drawMe = d->GetIsVisible();
			if(m_DrawState == DrawShowCaseOnly)
				drawMe &= (d->GetStyle() == STYLE_SHOWCASE);
			else if(m_DrawState == DrawAllDesklets)
				drawMe &= true;
			else if(m_DrawState == DrawFadeOut)
				drawMe &= d->GetIsShowCased();

			if(drawMe)
			{
				d->SetNoRedraw(true);
				ATLTRACE("showcase onpaint: desklet drawing noredraw=true\n");

				Bitmap* bmp = d->GetDocklet()->Draw();
				POINT p = d->GetDocklet()->GetCorrectedPos();
				SIZE s = d->GetDocklet()->GetCorrectedSize();
				ATLTRACE("showcase onpaint: desklet drawing ->Draw()\n");
	
				int alpha = d->GetDocklet()->GetAlpha();
				if(bmp != NULL && alpha > 5)
				{
					if(alpha > 240)
					{
						g.DrawImage(bmp, p.x, p.y, s.cx, s.cy);
						ATLTRACE("showcase onpaint: bitmap drawn\n");

						if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
						{
							Bitmap* bmp2 = d->GetLabel().GetCachedBitmap();
							if(bmp2)
							{
								g.DrawImage(bmp2,d->GetLabelPos().x + xVirt,d->GetLabelPos().y + yVirt,bmp2->GetWidth(),bmp2->GetHeight());
								ATLTRACE("showcase onpaint: label drawn\n");
							}
						}
					}
					else
					{
						ColorMatrix matrix = ColorMatrixHelper::GetIdentityMatrix();
						matrix.m[3][3] = (REAL)alpha/255;
						ImageAttributes attr;
						attr.SetColorMatrix(&matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);

						g.DrawImage(bmp,Rect(p.x + xVirt ,p.y + yVirt,s.cx,s.cy),0,0,bmp->GetWidth(),bmp->GetHeight(),UnitPixel,&attr,0,0);
						ATLTRACE("showcase onpaint: bitmap drawn with alpha\n");

						if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
						{
							Bitmap* bmp2 = d->GetLabel().GetCachedBitmap();
							if(bmp2)
							{
								g.DrawImage(bmp2,Rect(d->GetLabelPos().x + xVirt,d->GetLabelPos().y + yVirt,bmp2->GetWidth(),bmp2->GetHeight()),0,0,bmp2->GetWidth(),bmp2->GetHeight(),UnitPixel,&attr,0,0);
								ATLTRACE("showcase onpaint: label drawn with alpha\n");
							}
						}
			
					}
				}

				d->SetNoRedraw(false);
				ATLTRACE("showcase onpaint: no redraw = false\n");
			}
		}
	}

	ATLTRACE("showcase onpaint: before final output\n");
	CPaintDC paintDC2(m_hWnd);
	Graphics g2(paintDC2);
	g2.DrawImage(&buffer, 0, 0, s.cx, s.cy);
	ATLTRACE("showcase onpaint: after final output\n");
}

void CShowCase::RecreateBuffer(DrawState m_DrawState,BYTE bAlpha)
{
	RECT r = {0};
	r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
	r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
	r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
	r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 

	int xVirt = -GetSystemMetrics( SM_XVIRTUALSCREEN );
	int yVirt = -GetSystemMetrics( SM_YVIRTUALSCREEN ); 

	SIZE s = { RECTWIDTH(r), RECTHEIGHT(r) };

	backGroundBuffer = boost::shared_ptr<Bitmap>(new Bitmap(s.cx,s.cy) );
	Graphics g(backGroundBuffer.get());
	g.Clear(Color((BYTE)maxAlpha,GetRValue(color),GetGValue(color),GetBValue(color)));

	boost::shared_ptr<Bitmap> bmp( DeskletLoadGDIPlusImageW(L"Images\\showcasebg.png") );

	int left = s.cx / 2 - bmp->GetWidth()  / 2;
	int top  = s.cy / 2 - bmp->GetHeight() / 2;

	g.DrawImage(bmp.get(),left,top,bmp->GetWidth(),bmp->GetHeight() );

	if(m_DrawState != DrawNone)
	{
		std::vector<Docklet*> docklets;

		EnterCriticalSection(&Docklet::cs);
		for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
		{
			docklets.push_back(iter->second);
		}
		LeaveCriticalSection(&Docklet::cs);

		for(std::vector<Docklet*>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
		{
			Docklet* d= *d_iter;

			bool drawMe = d->GetIsVisible();
			if(m_DrawState == DrawShowCaseOnly)
				drawMe &= (d->GetStyle() == STYLE_SHOWCASE);
			else if(m_DrawState == DrawAllDesklets)
				drawMe &= true;
			else if(m_DrawState == DrawFadeOut)
				drawMe &= d->GetIsShowCased();

			if(drawMe)
			{
				Bitmap* bmp = d->GetDocklet()->Draw();
				POINT p = d->GetDocklet()->GetCorrectedPos();
				SIZE s = d->GetDocklet()->GetCorrectedSize();
	
				int alpha = d->GetDocklet()->GetAlpha();
				if(bmp != NULL && alpha > 5)
				{
					if(alpha > 240)
					{
						g.DrawImage(bmp, p.x, p.y, s.cx, s.cy);

						if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
						{
							Bitmap* bmp2 = d->GetLabel().GetCachedBitmap();
							if(bmp2)
								g.DrawImage(bmp2,d->GetLabelPos().x + xVirt,d->GetLabelPos().y + yVirt,bmp2->GetWidth(),bmp2->GetHeight());
						}
					}
					else
					{
						ColorMatrix matrix = ColorMatrixHelper::GetIdentityMatrix();
						matrix.m[3][3] = (REAL)alpha/255;
						ImageAttributes attr;
						attr.SetColorMatrix(&matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);

						g.DrawImage(bmp,Rect(p.x + xVirt,p.y + yVirt,s.cx,s.cy),0,0,bmp->GetWidth(),bmp->GetHeight(),UnitPixel,&attr,0,0);

						if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
						{
							Bitmap* bmp2 = d->GetLabel().GetCachedBitmap();
							if(bmp2)
								g.DrawImage(bmp2,Rect(d->GetLabelPos().x + xVirt,d->GetLabelPos().y + yVirt,bmp2->GetWidth(),bmp->GetHeight()),0,0,bmp2->GetWidth(),bmp2->GetHeight(),UnitPixel,&attr,0,0);
						}
			
					}
				}

				d->GetDocklet()->SetIsDirty(true);
			}
		}
	}

	POINT p ={0};
	SetBitmap(backGroundBuffer.get(),bAlpha,&p,&s);
}

void CShowCase::ShowOverlayWindow(bool doAnim)
{
	// post:  the overlay window for ShowCase has been made visible

	isAnimating = true;

	DeleteObject( (HGDIOBJ)GetClassLong(m_hWnd,GCL_HBRBACKGROUND));
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)CreateSolidBrush(color) );

	RECT r = {0};

	r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
	r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
	r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
	r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 
	//::GetWindowRect(desktopHwnd,&r);
	// TODO r.top += 100;
	
	SetWindowPos(HWND_TOPMOST,&r,SWP_NOREDRAW);

	DeleteObject( (HGDIOBJ)GetClassLong(m_hWnd,GCL_HBRBACKGROUND));
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)CreateSolidBrush(color) );

	//SetLayeredWindowAttributes(
	//RecreateBuffer(m_DrawState,0);
	//SetAlpha(0);
	SetLayeredWindowAttributes(m_hWnd,0,0,LWA_ALPHA);
	SetWindowPos(HWND_TOPMOST,&r,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );

	if(doAnim)
	{
		animFrame = 0;
		SetTimer(2, 1, NULL);
		//RecreateBuffer(m_DrawState,0);
	}
	else
	{
		//RecreateBuffer(m_DrawState,maxAlpha);
		SetWindowPos(HWND_TOP,&r,SWP_SHOWWINDOW);
		SetLayeredWindowAttributes(m_hWnd,0,(BYTE)maxAlpha,LWA_ALPHA);
		isAnimating = false;
	}
}

LRESULT CShowCase::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(1 == wParam)
	{
		if(animFrame < numOfAnimFrames)
		{
			float alphaPerFrame( static_cast<float>(maxAlpha) / static_cast<float>(numOfAnimFrames) ); 

	//	for(int i=1;i<numOfAnimFrames;++i)
			SetLayeredWindowAttributes(m_hWnd,0,(BYTE)(maxAlpha - (animFrame*alphaPerFrame)),LWA_ALPHA);	

			++animFrame;
		}
		else
		{
			RECT r = {0};
			SetWindowPos(0,&r,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE |SWP_HIDEWINDOW);
			isAnimating = false;

			KillTimer(1);
		}
	}
	else if(2 == wParam)
	{
		if(animFrame < numOfAnimFrames)
		{
			float alphaPerFrame( static_cast<float>(maxAlpha) / static_cast<float>(numOfAnimFrames) ); 

		//for(int i=0;i<numOfAnimFrames;++i)
			SetLayeredWindowAttributes(m_hWnd,0,BYTE(animFrame*alphaPerFrame),LWA_ALPHA);
			//SetAlpha(i * alphaPerFrame);

			++animFrame;
		}
		else
		{
			RECT r = {0};
			r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
			r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
			r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
			r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 

			SetWindowPos(HWND_TOP,&r,SWP_SHOWWINDOW);
			SetLayeredWindowAttributes(m_hWnd,0,(BYTE)maxAlpha,LWA_ALPHA);
			isAnimating = false;
			KillTimer(2);
		}
	}

	return 0;
}

void CShowCase::HideOverlayWindow(bool doAnim)
{
	// post:  the overlay window for ShowCase has been hidden

	isAnimating = true;

	if(doAnim)
	{
		animFrame = 0;
		SetTimer(1, 1, NULL);
		//RecreateBuffer(m_DrawState,255);

		//float alphaPerFrame( static_cast<float>(maxAlpha) / static_cast<float>(numOfAnimFrames) ); 

		//for(int i=1;i<numOfAnimFrames;++i)
		//	SetLayeredWindowAttributes(m_hWnd,0,(BYTE)(maxAlpha - (i*alphaPerFrame)),LWA_ALPHA);	
	}
	else
	{
		RECT r = {0};
		SetWindowPos(0,&r,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOMOVE |SWP_HIDEWINDOW);
		isAnimating = false;
	}
}


STDMETHODIMP CShowCase::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IShowCase,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

inline LONG AddWindowStyle(HWND hWnd, LONG style)
{
	return ::SetWindowLong(hWnd, GWL_STYLE, ::GetWindowLong(hWnd,GWL_STYLE) | style);
};

inline LONG RemoveWindowStyle(HWND hWnd, LONG style)
{
	LONG l = ::GetWindowLong(hWnd,GWL_STYLE);
	l = l &  ~style;
	return ::SetWindowLong(hWnd, GWL_STYLE, l);
};

inline HDWP DeferDockletPos(Docklet* d, HDWP hDwp, HWND hWndInsert, DWORD flags)
{
	hDwp = ::DeferWindowPos(hDwp, d->GetHWnd(),hWndInsert,0,0,0,0,flags);

	if(d->GetLabelHwnd())
	{
		if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
			hDwp = ::DeferWindowPos(hDwp, d->GetLabelHwnd(),hWndInsert,0,0,0,0,flags);
		else
			hDwp = ::DeferWindowPos(hDwp, d->GetLabelHwnd(),hWndInsert,0,0,0,0,flags & ~SWP_SHOWWINDOW);
	}

	return hDwp;
}

STDMETHODIMP CShowCase::Activate()
{
	// pre:  isActive is not true
	// post: showcase has been activated

	// update the status of isClickThru flag for the window
	put_IsClickThru(isClickThru);

	if(isActive == VARIANT_FALSE)
	{
		m_IsActiveOnAllDesklets = false;

		DeleteObject( (HGDIOBJ)GetClassLong(m_hWnd,GCL_HBRBACKGROUND));
		SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)CreateSolidBrush(color) );

		// do the funky fade in animation if required.
		isActive = VARIANT_TRUE;

		RECT r = {0};
		GetWindowRect(&r);
		SIZE s= SIZEFROMRECT(r);

		if(doAnimation == VARIANT_TRUE || 0 == s.cx || 0 == s.cy)
		{
			m_DrawState = DrawShowCaseOnly;
			
			RECT r = {0};
			r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
			r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
			r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
			r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 
			// TODO r.top += 100;
			SetWindowPos(HWND_TOPMOST,&r,0);

			RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );
			//CWindowDC dc(m_hWnd);
			//OnPaint(dc);
		}
		else
		{
			m_DrawState = DrawNone;
		}

		
		std::vector<HWND>hwnds;
		std::vector<BYTE> alphas;

		if(doAnimation == VARIANT_TRUE)
		{
			ShowOverlayWindow(doAnimation == VARIANT_TRUE);
		}

		std::vector<Docklet*> docklets;

		EnterCriticalSection(&Docklet::cs);

		for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
		{
			docklets.push_back(iter->second);
		}

		LeaveCriticalSection(&Docklet::cs);

		for(std::vector<Docklet*>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
		{
			Docklet* d= *d_iter;

			if(d->GetStyle() == STYLE_SHOWCASE)
			{
				d->SetIsShowCased(true);

				if(d->GetIsVisible())
				{

					hwnds.push_back(d->GetHWnd() );
					alphas.push_back(d->GetDocklet()->GetAlpha() );

					if(doAnimation)
					{
						d->SetNoRedraw(true);
						d->SetAlpha(1);
					}
				}
			}
		}

		HDWP hDwp = ::BeginDeferWindowPos(Docklet::Size()*2 + 1);

		if(doAnimation == VARIANT_FALSE)
		{
			SetLayeredWindowAttributes(m_hWnd,0,(BYTE)maxAlpha,LWA_ALPHA);

			RECT r = {0};
			r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
			r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
			r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
			r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 

			hDwp = ::DeferWindowPos(hDwp,m_hWnd,HWND_TOPMOST,r.left,r.top,RECTWIDTH(r),RECTHEIGHT(r),0/*SWP_NOREDRAW*/ | SWP_SHOWWINDOW);

		}

		for(std::vector<HWND>::iterator hwndIter = hwnds.begin(); hwndIter != hwnds.end(); ++hwndIter)
		{
			Docklet* d= Docklet::FindDocklet(*hwndIter);
			if(d && d->GetIsVisible() )
				hDwp = ::DeferDockletPos(d,hDwp,(HWND_TOPMOST),SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | SWP_SHOWWINDOW);
		}

		::EndDeferWindowPos(hDwp);

		if(doAnimation)
		{
			/*
			const NUMFRAMES = 11;
			for(int x=0;x<NUMFRAMES;++x)
			{
				for(int i=0;i<hwnds.size();++i)
				{
					Docklet* d = Docklet::FindDocklet(hwnds[i]);
					if(d && x==0)
					{
						//d->SetNoRedraw(true);
						//d->Redraw();
						d->SetAlpha(1);
						d->ShowWindow(SW_SHOWNA);
						if(!d->GetLabelOnlyOnMouseOver() )
							d->GetLabel().ShowWindow(SW_SHOWNA);
					}

					if(d && alphas[i] > maxAlpha)
					{
						float a = float(alphas[i]);
						float add = (a) / (NUMFRAMES);
						d->SetAlpha((add * float(x+1)));
					}
				}
			}*/

			int numFrames = numOfAnimFrames / 4;
			for(int x=0;x<numFrames;++x)
			{
				for(UINT i=0;i<hwnds.size();++i)
				{
					Docklet* d = Docklet::FindDocklet(hwnds[i]);
					if(d && x==0)
					{

						EnterCriticalSection(&Docklet::cs);
						BYTE alpha = d->GetDocklet()->GetAlpha();
						d->GetDocklet()->SetAlpha(0);
						d->SetNoRedraw(false);
						d->SetNoRedraw(true);
						d->GetDocklet()->SetAlpha(alpha);
						LeaveCriticalSection(&Docklet::cs);

						RECT r = {0};
						d->SetAlpha(1);
						d->SetWindowPos(HWND_TOPMOST, &r, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
						if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
							d->GetLabel().SetWindowPos(HWND_TOPMOST, &r, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
					}

					if(d && alphas[i] > maxAlpha)
					{
						float a = float(alphas[i]);
						float add = (a) / (numFrames);
						d->SetAlpha((BYTE)(add * float(x+1)));
					}
				}
			}

			for(UINT i=0;i<hwnds.size();++i)
			{

				Docklet* d = Docklet::FindDocklet(hwnds[i]);
				if(d)
				{
					d->SetAlpha(d->GetDocklet()->GetAlpha());
					d->SetNoRedraw(false);
				}
			}

		}
	/*	else
		{
			m_DrawState = DrawNone;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );
		}
	*/

		//RecreateBuffer(m_DrawState,255);
	}

	m_DrawState = DrawNone;
	RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );

	return S_OK;
}

STDMETHODIMP CShowCase::Deactivate()
{
	// pre:  isActive is true
	// post: ShowCase mode has been deactivated
	// note: see Active method for more info on this method.

	if(isActive == VARIANT_TRUE)
	{
		// make the window clickthru when deactivating, so
		// when a possible animation is busy, the user
		// will be able to already interact with the rest of his 
		// desktop.
		ModifyStyleEx(0,WS_EX_TRANSPARENT,0); // remove flag
		
		if(doAnimation)
		{
			m_DrawState = DrawFadeOut;

			RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );
			//CWindowDC dc(m_hWnd);
			//OnPaint(dc);
		}
		else
		{
			m_DrawState = DrawNone;
		}

		isAnimating = true;

		std::vector<HWND>hwnds;
		std::vector<BYTE> alphas;

		if(doAnimation)
		{

			//CRITICAL_SECTION cs = {0};
			//InitializeCriticalSection(&cs);
			//EnterCriticalSection(&Docklet::cs);

			std::vector<Docklet*> docklets;

			EnterCriticalSection(&Docklet::cs);

			for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
			{
				docklets.push_back(iter->second);
			}
	
			LeaveCriticalSection(&Docklet::cs);

			for(std::vector<Docklet*>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
			{
				Docklet* d= *d_iter;

				if(d->GetIsShowCased() )
				{
					hwnds.push_back(d->GetHWnd());
					alphas.push_back(d->GetDocklet()->GetAlpha() );
					d->SetNoRedraw(true);
				}
			}

			// nobody really noticed this animation on deactivate, so simply don't use it.
			int numFrames = 0;//numOfAnimFrames ;
			for(int x=0;x<numFrames;++x)
			{
				for(UINT i=0;i<hwnds.size();++i)
				{
					Docklet* d = Docklet::FindDocklet(hwnds[i]);
					int thisAlpha = alphas[i];
					if(d && maxAlpha < thisAlpha)
					{
						int add = (thisAlpha-maxAlpha) / (numFrames);
						d->SetAlpha((BYTE)(thisAlpha - (add * (x+1))));
						if(!d->GetLabelOnlyOnMouseOver())
							d->GetLabel().UpdateAlphaOnly((BYTE)(thisAlpha - (add * (x+1))) );
					}
				}
			}
		}
	
		///RECT r = {0};
		///SetWindowPos(0,&r,SWP_HIDEWINDOW);
		
		isAnimating = false;

		HDWP hDwp = ::BeginDeferWindowPos(Docklet::Size()*2 + 1);

		if(doAnimation == VARIANT_FALSE)
			hDwp = ::DeferWindowPos(hDwp, m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | SWP_HIDEWINDOW);

		std::vector<Docklet*> iconStyleDocklets;

		std::vector<Docklet*> docklets;

		EnterCriticalSection(&Docklet::cs);

		for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
		{
			docklets.push_back(iter->second);
		}
		LeaveCriticalSection(&Docklet::cs);


		for(std::vector<Docklet*>::iterator d_iter2 = docklets.begin(); d_iter2 != docklets.end(); ++d_iter2)
		{
			Docklet* d= *d_iter2;


			if(d->GetIsShowCased() )
			{
				d->SetIsShowCased(false);

				hDwp = ::DeferDockletPos(d,hDwp,d->GetHwndInsertAfterFromStyle(),SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | ( (d->GetStyle()  == STYLE_SHOWCASE || d->GetStyle() == STYLE_ICON && d->GetIsVisible()) ? SWP_HIDEWINDOW : 0) ); 

				if(d->GetStyle() == STYLE_ICON)
				{
					iconStyleDocklets.push_back(d);
				}
			}
		}
	

		::EndDeferWindowPos(hDwp);

		isActive = VARIANT_FALSE;

		// BUGGY SOMETIMES...

		// because all docklets with STYLE_ICON are hidden to allow a smooth "hide", we have to make them visible again if they ARE visible.

		for(std::vector<Docklet*>::iterator iconStyleIter = iconStyleDocklets.begin(); iconStyleIter != iconStyleDocklets.end(); ++iconStyleIter)
		{
			Docklet* d = *iconStyleIter;

			d->ApplyStyle(STYLE_ICON,true); ///doNotChangeZOrder
			//if(d->GetIsVisible())d->SetVisibility(true);
			if(d->GetIsVisible() )
			{
				d->ModifyStyle(0,WS_VISIBLE);
				if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
					d->GetLabel().ModifyStyle(0,WS_VISIBLE);
			}

		}
		if( iconStyleDocklets.size() > 0)
		{
			Docklet* d = iconStyleDocklets[0];
			d->ModifyStyle(WS_VISIBLE,0);
			d->ShowWindow(SW_SHOW);
			if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
					d->GetLabel().ShowWindow(SW_SHOW);
		}

		if(doAnimation)
		{
			for(UINT i=0;i<hwnds.size();++i)
			{
				Docklet* d = Docklet::FindDocklet(hwnds[i]);
				if(d)
				{
					d->SetAlpha(d->GetDocklet()->GetAlpha() );
					d->SetNoRedraw(false);
				}
			}
		}
		
	}

	HideOverlayWindow(doAnimation == VARIANT_TRUE);

	m_DrawState = DrawNone;

	return S_OK;
}

STDMETHODIMP CShowCase::get_Active(VARIANT_BOOL *pVal)
{
	// pre:  pVal is not NULL
	// post: pVal has been set to true iff showcase is active

	if(!pVal)return E_INVALIDARG;

	*pVal = isActive;

	return S_OK;
}

STDMETHODIMP CShowCase::put_Active(VARIANT_BOOL newVal)
{
	// pre:  newVal != get_isActive()
	// post: if true, showcase has been activated, if false,
	//		 showcase has been deactived

	if(isActive != newVal)
	{
		isActive = newVal;
		if(isActive == VARIANT_TRUE)
			Activate();
		else
			Deactivate();
	}
	return S_OK;
}

STDMETHODIMP CShowCase::get_MaximumAlpha(long *pVal)
{
	// pre:  pVal is not NULL
	// post: pVal has been set to maximumAlpha

	if(!pVal)return E_INVALIDARG;

	*pVal = maxAlpha;

	return S_OK;
}

STDMETHODIMP CShowCase::put_MaximumAlpha(long newVal)
{
	// post: maxAlpha has been set to newVal

	maxAlpha = newVal;

	if(isActive == VARIANT_TRUE)
		SetAlpha((BYTE)maxAlpha);

	return S_OK;
}

STDMETHODIMP CShowCase::get_NumberOfAnimationFrames(long *pVal)
{
	// pre:  pVal is not NULL
	// post: pVal has been set to numOfAnimFrames

	if(!pVal)return E_INVALIDARG;

	*pVal = numOfAnimFrames;

	return S_OK;
}

STDMETHODIMP CShowCase::put_NumberOfAnimationFrames(long newVal)
{
	// post: numOfAnimFrames has been set to newVal

	numOfAnimFrames = newVal;

	return S_OK;
}

STDMETHODIMP CShowCase::get_DoAnimation(VARIANT_BOOL *pVal)
{
	// pre:  pVal is not NULL
	// post: pVal has been set to doAnimation

	if(!pVal)return E_INVALIDARG;

	*pVal = doAnimation;

	return S_OK;
}

STDMETHODIMP CShowCase::put_DoAnimation(VARIANT_BOOL newVal)
{
	// post: doAnimation has been set to newVal

	doAnimation = newVal;

	return S_OK;
}

STDMETHODIMP CShowCase::get_Color(OLE_COLOR *pVal)
{
	// pre:  pVal is not NULL
	// post: pVal has been set to color

	if(!pVal)return E_INVALIDARG;

	(*pVal) = (OLE_COLOR)color;

	return S_OK;
}

STDMETHODIMP CShowCase::put_Color(OLE_COLOR newVal)
{
	// post: color has been set to newVal

	OleTranslateColor(newVal,0,&color);

	DeleteObject( (HGDIOBJ)GetClassLong(m_hWnd,GCL_HBRBACKGROUND));
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)CreateSolidBrush(color) );

	return S_OK;
}

LRESULT CShowCase::OnWindowsPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
	if(wp != NULL && m_DrawState == DrawNone)
	{
		wp->flags |= SWP_NOZORDER;
	}

	return 0;
}

STDMETHODIMP CShowCase::ActivateOnAllDesklets()
{
	// pre:  isActive is false
	// post: showcase has been activated on all desklets

	// update the status of isClickThru flag for the window
	put_IsClickThru(isClickThru);
	
	if(isActive == VARIANT_FALSE)
	{
		m_IsActiveOnAllDesklets = true;

		DeleteObject( (HGDIOBJ)GetClassLong(m_hWnd,GCL_HBRBACKGROUND));
		SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)CreateSolidBrush(color) );

		// do the funky fade in animation if required.
		isActive = VARIANT_TRUE;

		RECT r = {0};
		GetWindowRect(&r);
		SIZE s= SIZEFROMRECT(r);

		if(doAnimation == VARIANT_TRUE || 0 == s.cx || 0 == s.cy)
		{
			m_DrawState = DrawAllDesklets;


			RECT r = {0};
			r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
			r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
			r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
			r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 
			SetWindowPos(HWND_TOPMOST, &r,0);

			RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );
			//CWindowDC dc(m_hWnd);
			//OnPaint(dc);
		}
		else
		{
			m_DrawState = DrawNone;
		}
		
		std::vector<HWND>hwnds;
		std::vector<BYTE> alphas;

		std::vector<Docklet*> docklets;

		EnterCriticalSection(&Docklet::cs);
		for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
		{
			Docklet* d = iter->second;
			docklets.push_back(d);
		}
		LeaveCriticalSection(&Docklet::cs);

		//::MessageBox(NULL,TEXT("after desklet enumeration"),TEXT("debug"),MB_ICONINFORMATION);


		if(doAnimation == VARIANT_TRUE)
			ShowOverlayWindow(doAnimation == VARIANT_TRUE);


		for(std::vector<Docklet*>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
		{
			Docklet* d= *d_iter;

			d->SetIsShowCased(true);

			// BUGFIX: hidden desklets became visible if showcase was invoked. Forgat to check current
			//         visibility of the desklet.

			if(d->GetIsVisible())
			{
				hwnds.push_back(d->GetHWnd() );
				alphas.push_back(d->GetDocklet()->GetAlpha() );

				if(doAnimation == VARIANT_TRUE)
				{
					d->SetNoRedraw(true);
					d->SetAlpha(1);
				}

				if(d->GetStyle() == STYLE_ICON)
				{
					::ShowWindow(d->GetHWnd(),SW_HIDE);
					::SetParent(d->GetHWnd(),NULL);

					::ShowWindow(d->GetLabelHwnd(),SW_HIDE);
					::SetParent(d->GetLabelHwnd(),NULL);
				}
			}
		}


		HDWP hDwp = ::BeginDeferWindowPos(Docklet::Size()*2 + 1);

		if(doAnimation == VARIANT_FALSE)
		{
			SetLayeredWindowAttributes(m_hWnd,0,(BYTE)maxAlpha,LWA_ALPHA);

			RECT r = {0};
			r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
			r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
			r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
			r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 

			hDwp = ::DeferWindowPos(hDwp,m_hWnd,HWND_TOPMOST,r.left,r.top,RECTWIDTH(r),RECTHEIGHT(r), 0/*SWP_NOREDRAW*/ | SWP_SHOWWINDOW);
		}

		for(std::vector<Docklet*>::iterator d_iter2 = docklets.begin(); d_iter2 != docklets.end(); ++d_iter2)
		{
			Docklet* d= *d_iter2;

			d->SetIsShowCased(true);


			if(d->GetIsVisible() )
			{
				if(doAnimation)
					hDwp = ::DeferDockletPos(d,hDwp,HWND_TOPMOST/*(HWND)-1*/,SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
				else
					hDwp = ::DeferDockletPos(d,hDwp,HWND_TOPMOST/*(HWND)-1*/,SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | SWP_SHOWWINDOW);
			}
		}

		::EndDeferWindowPos(hDwp);

		if(doAnimation == VARIANT_TRUE)
		{
			int numFrames = numOfAnimFrames / 4 + 1;
			for(int x=0;x<numFrames;++x)
			{
				for(UINT i=0;i<hwnds.size();++i)
				{
					Docklet* d = Docklet::FindDocklet(hwnds[i]);
					if(d && x==0)
					{
						d->SetAlpha(1);
						//d->S
						RECT r = {0};
						d->SetWindowPos(HWND_TOPMOST, &r, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
						d->ShowWindow(SW_SHOWNA);
						if(!d->GetLabelOnlyOnMouseOver() && d->GetLabelType() != LABEL_NONE)
							d->GetLabel().SetWindowPos(HWND_TOPMOST, &r, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
							//d->GetLabel().ShowWindow(SW_SHOWNA);
					}

					if(d && alphas[i] > maxAlpha)
					{
						float a = float(alphas[i]);
						float add = (a) / (numFrames);
						d->SetAlpha((BYTE)(add * float(x+1)));
					}
				}
			}

			for(UINT i=0;i<hwnds.size();++i)
			{

				Docklet* d = Docklet::FindDocklet(hwnds[i]);
				if(d)
				{
					d->SetAlpha(d->GetDocklet()->GetAlpha());
					d->SetNoRedraw(false);
				}
			}

		}

	/*	else
		{
			m_DrawState = DrawNone;
			RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );
		}
	*/

		//RecreateBuffer(m_DrawState,255);

		if(hwnds.size() > 0)
		{
			HMODULE hMod = LoadLibrary(_T("user32.dll"));
			VOID (__stdcall *SwitchToThisWindow)(HWND,BOOL) = (VOID (__stdcall *)(HWND,BOOL)) GetProcAddress(hMod,"SwitchToThisWindow");
			for(std::vector<HWND>::iterator hwndIter = hwnds.begin(); hwndIter != hwnds.end(); ++hwndIter)
				SwitchToThisWindow(*hwndIter,FALSE);
			FreeLibrary(hMod);
		}
	}

	m_DrawState = DrawNone;
	RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW );

	return S_OK;
}


BOOL CShowCase::SetNormalHotKey(HotKey& hotKey, BOOL reRegister)
{
	// post: the normal hotkey has been set to hotKey and is
	//		 reRegistered iff reRegister is true

	normalHotKey->modifiers = hotKey.modifiers;
	normalHotKey->vk = hotKey.vk;

	if(reRegister)
	{
		if(GetHotKeysAreRegistered())
			UnregisterHotKeys();

		return RegisterHotKeys();
	}
	else
	{
		return TRUE;
	}
}

HotKey CShowCase::GetNormalHotKey()const
{
	// post: the normal hotkey has been returned
	return *normalHotKey;
}

HotKey CShowCase::GetAllDeskletsHotKey()const
{
	// post: the allDesklets hotkey has been returned
	return *allDeskletsHotKey;
}

BOOL CShowCase::SetAllDeskletsHotKey(HotKey& hotKey, BOOL reRegister)
{
	// post: the allDeskletsHotKey has been set to hotKey and
	//		 is reRegistered iff reRegister is true

	allDeskletsHotKey->modifiers = hotKey.modifiers;
	allDeskletsHotKey->vk = hotKey.vk;

	if(reRegister)
	{
		if(GetHotKeysAreRegistered())
			UnregisterHotKeys();

		return RegisterHotKeys();
	}
	else
	{
		return TRUE;
	}
}

BOOL CShowCase::RegisterHotKeys()
{
	// post: TRUE has been returned iff both hotkeys are registered

	BOOL ret = RegisterHotKey(m_hWnd,normalHotKey->id,normalHotKey->modifiers,normalHotKey->vk);	
	ret &= RegisterHotKey(m_hWnd,allDeskletsHotKey->id,allDeskletsHotKey->modifiers,allDeskletsHotKey->vk);

	hotKeysAreRegistered = ret;

	return ret;
}

BOOL CShowCase::UnregisterHotKeys()
{
	// post: TRUE has been returned iff both hotkeys are unregistered
	hotKeysAreRegistered = FALSE;

	return UnregisterHotKey(m_hWnd,normalHotKey->id) && UnregisterHotKey(m_hWnd,allDeskletsHotKey->id);
}

BOOL CShowCase::GetHotKeysAreRegistered()
{
	// post:  TRUE has been returned iff both hotkeys are registered
	return hotKeysAreRegistered;
}

BOOL CShowCase::OnHotKey(UINT id, UINT modifiers, UINT vk)
{
	if(id == normalHotKey->id)
	{
		// normal hotkey was pressed
		if(isActive == VARIANT_FALSE)
			Activate();
		else
			Deactivate();
	
		return TRUE;
	}
	else if(id == allDeskletsHotKey->id)
	{
		// allDesklets hotkey was pressed
		if(isActive == VARIANT_FALSE)
			ActivateOnAllDesklets();
		else
			Deactivate();	

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

STDMETHODIMP CShowCase::get_Enabled(VARIANT_BOOL *pVal)
{
	// pre:  pVal is not NULL
	// post: pVal has been set to doAnimation

	if(!pVal)return E_INVALIDARG;

	*pVal = isEnabled;

	return S_OK;
}

STDMETHODIMP CShowCase::put_Enabled(VARIANT_BOOL newVal)
{
	// post: isEnabled has been set to newVal

	if(isEnabled != newVal)
	{
		if(isEnabled)
			UnregisterHotKeys();
		else
			RegisterHotKeys();

		isEnabled = newVal;
	}

	return S_OK;
}

STDMETHODIMP CShowCase::get_IsClickThru(VARIANT_BOOL *pVal)
{
	// pre:  pVal is not NULL
	// post: pVas has been set to isClickThru
	if(NULL == pVal)
		return E_INVALIDARG;

	*pVal = isClickThru;

	return S_OK;
}

STDMETHODIMP CShowCase::put_IsClickThru(VARIANT_BOOL newVal)
{
	// post: isClickThru has been set to newVal
	isClickThru = newVal;

	if(::IsWindow(m_hWnd))
	{
		if(isClickThru != VARIANT_FALSE)
			ModifyStyleEx(0,WS_EX_TRANSPARENT,0); // set flag
		else
			ModifyStyleEx(WS_EX_TRANSPARENT,0,0); // remove flag
	}

	return S_OK;
}
