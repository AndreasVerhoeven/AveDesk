#include "stdafx.h"
#include "DeskletItemDragAnimation.h"
#include "Docklet.h"
#include "ShowCase1.h"

extern CComObject<CShowCase>* _ShowCase;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletItemDragAnimation::CDeskletItemDragAnimation() :
	currentFrame(0),
	currentLargeFrame(0),
	gdipBmp(NULL),
	size(0),
	bDestroyed(FALSE),
	gdipBmpLarge(NULL),
	wasInForbiddenWindowArea(FALSE),
	xRatio(0.0f), yRatio(0.0f),
	maxNumFrames(15),
	maxNumFramesLarge(20),
	item(NULL)
{
	RECT r = {0};
	Create(NULL,r,_T("AveDeskAnimation"),WS_POPUP | WS_OVERLAPPED,WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	// we dont want this window to show up in the taskbar
	SetParent(NULL);
	//ShowWindow(SW_HIDE);
	DWORD newExFlags = WS_EX_TOOLWINDOW;
	ModifyStyleEx(WS_EX_APPWINDOW,newExFlags,0);

	EnsureWindowIsLayered();
}

CDeskletItemDragAnimation::~CDeskletItemDragAnimation()
{
	//SafeDestroy();
	delete gdipBmp;
	delete gdipBmpLarge;
	if(item != NULL)
		item->Release();
}

void CDeskletItemDragAnimation::SafeDestroy()
{
	if(::IsWindow(m_hWnd))
	{
		bDestroyed = TRUE;
		PostMessage(WM_CLOSE);
	}
	else
	{
		delete this;
	}
}

void CDeskletItemDragAnimation::OnFinalMessage( HWND ) 
{ 

	delete this;
 
}

LRESULT CDeskletItemDragAnimation::OnClose(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(bDestroyed)
		bHandled = FALSE;

	return 0;
}

BOOL CDeskletItemDragAnimation::IsInsideForbiddenWindowArea()
{
	if(NULL == forbiddenWindow)
		return FALSE;

	RECT rc = {0};
	::GetWindowRect(forbiddenWindow, &rc);
	POINT pt = { GET_X_LPARAM(GetMessagePos()), GET_Y_LPARAM(GetMessagePos())};
	//POINT pt = {0};
	//GetCursorPos(&pt);

	return PtInRect(&rc, pt);
}

LRESULT CDeskletItemDragAnimation::OnMouseMove(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { GET_X_LPARAM(GetMessagePos()), GET_Y_LPARAM(GetMessagePos())};
	//GetCursorPos(&pt);
	pt.x -= wndSize.cx /2;
	pt.y -= wndSize.cy /2;


	BOOL isInForbiddenArea = IsInsideForbiddenWindowArea();
	if(wasInForbiddenWindowArea && !isInForbiddenArea)
	{
		wasInForbiddenWindowArea = FALSE;
		KillTimer(1);
		KillTimer(2);
		KillTimer(4);
		SetTimer(3,CheckAnimSpeed());
	}
	else if(!wasInForbiddenWindowArea && isInForbiddenArea)
	{
		size = 80;
		wasInForbiddenWindowArea = TRUE;
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);
		SetTimer(4,CheckAnimSpeed());
	}
	
	::UpdateLayeredWindow(m_hWnd, NULL, &pt, NULL, NULL, NULL, 0, NULL, 0);

	return 0;
}


UINT CDeskletItemDragAnimation::CheckAnimSpeed()
{
	UINT interval = 10;
	if(GetKeyState(VK_SHIFT) < 0)
	{
		maxNumFrames = 30;
		maxNumFramesLarge = 60;
	}
	else
	{
		maxNumFrames = 15;
		maxNumFramesLarge = 20;
	}	

	return interval;
}

void CDeskletItemDragAnimation::UpdateLarge(SIZE s, SIZE s2, BYTE largeAlpha, BYTE smallAlpha, BOOL largeFirst)
{
	CDC dc;
	CBitmap bmp;
	CWindowDC screenDC(NULL);

	dc.CreateCompatibleDC(screenDC);
	bmp.CreateCompatibleBitmap(screenDC, s.cx, s.cy);
	dc.SelectBitmap(bmp);

	if(!smallAndLargeAreTheSame)
	{
		CDC dcLargeResized;
		CBitmap bmpLargeResized;
		dcLargeResized.CreateCompatibleDC(screenDC);
		bmpLargeResized.CreateCompatibleBitmap(screenDC, s.cx, s.cy);
		dcLargeResized.SelectBitmap(bmpLargeResized);

		CDC dcSmall;
		CBitmap bmpSmall;
		dcSmall.CreateCompatibleDC(screenDC);
		bmpSmall.CreateCompatibleBitmap(screenDC, s2.cx, s2.cy);
		dcSmall.SelectBitmap(bmpSmall);

		Graphics g(dcLargeResized);
		if(gdipBmpLarge != NULL)
			g.DrawImage(gdipBmpLarge, Rect(0,0, s.cx, s.cy), 0, 0, gdipBmpLarge->GetWidth(), gdipBmpLarge->GetHeight(), UnitPixel, 0, 0, 0);

		Graphics g2(dcSmall);
		if(gdipBmp != NULL)
			g2.DrawImage(gdipBmp, Rect(0,0, s2.cx, s2.cy), 0, 0, gdipBmp->GetWidth(), gdipBmp->GetHeight(), UnitPixel, 0, 0, 0);

		BLENDFUNCTION bf2 = {AC_SRC_OVER, 0, smallAlpha, AC_SRC_ALPHA};
		BLENDFUNCTION bf = {AC_SRC_OVER, 0, largeAlpha, AC_SRC_ALPHA};

		if(!largeFirst)
		{
			dc.AlphaBlend(s.cx /2 - s2.cx/2, s.cy/2 - s2.cy/2, s2.cx, s2.cy, dcSmall, 0, 0, s2.cx, s2.cy, bf2);
			dc.AlphaBlend(0, 0, s.cx, s.cy, dcLargeResized, 0, 0, s.cx, s.cy, bf);
		}
		else
		{
			dc.AlphaBlend(0, 0, s.cx, s.cy, dcLargeResized, 0, 0, s.cx, s.cy, bf);
			dc.AlphaBlend(s.cx /2 - s2.cx/2, s.cy/2 - s2.cy/2, s2.cx, s2.cy, dcSmall, 0, 0, s2.cx, s2.cy, bf2);
		}
	}
	else
	{
		Graphics g(dc);
		if(gdipBmpLarge != NULL)
			g.DrawImage(gdipBmpLarge, Rect(0,0, s.cx, s.cy), 0, 0, gdipBmpLarge->GetWidth(), gdipBmpLarge->GetHeight(), UnitPixel, 0, 0, 0);
	}

	//POINT pt = {0};
	POINT pt = { GET_X_LPARAM(GetMessagePos()), GET_Y_LPARAM(GetMessagePos())};
	//GetCursorPos(&pt);
	pt.x -= s.cx /2;
	pt.y -= s.cy /2;

	POINT ptSourcePosition = {0,0};
	BLENDFUNCTION blendFunction = {0};
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = static_cast<BYTE>(255);
	blendFunction.AlphaFormat = AC_SRC_ALPHA;

	POINT* pptNewPos = &pt;
	SIZE newSize = {s.cx,s.cy};
	SIZE* pNewSize = &newSize;

	COLORREF crKey = RGB(255,0,255); // dummy colourkey
	BOOL ret = SetHDC(dc,&ptSourcePosition,crKey,&blendFunction,2,pptNewPos, pNewSize);

	dc.SelectBitmap(NULL);
}

void CDeskletItemDragAnimation::Update(int size, int alpha)
{
	CDC dc;
	CBitmap bmp;
	CWindowDC screenDC(NULL);

		dc.CreateCompatibleDC(screenDC);
	bmp.CreateCompatibleBitmap(screenDC, size, size);
	dc.SelectBitmap(bmp);
	Graphics g(dc);
	if(gdipBmp != NULL)
		g.DrawImage(gdipBmp, Rect(0,0, size, size), 0, 0, gdipBmp->GetWidth(), gdipBmp->GetHeight(), UnitPixel, 0, 0, 0);

	//POINT pt = {0};
	//GetCursorPos(&pt);
	POINT pt = { GET_X_LPARAM(GetMessagePos()), GET_Y_LPARAM(GetMessagePos())};
	pt.x -= wndSize.cx /2;
	pt.y -= wndSize.cy /2;

	POINT ptSourcePosition = {0,0};
	BLENDFUNCTION blendFunction = {0};
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = static_cast<BYTE>(alpha);
	blendFunction.AlphaFormat = AC_SRC_ALPHA;

	POINT* pptNewPos = &pt;
	SIZE newSize = {size,size};
	SIZE* pNewSize = &newSize;

	COLORREF crKey = RGB(255,0,255); // dummy colourkey
	BOOL ret = SetHDC(dc,&ptSourcePosition,crKey,&blendFunction,2,pptNewPos, pNewSize);

	dc.SelectBitmap(NULL);
}

LRESULT CDeskletItemDragAnimation::OnTimer(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	const double PI25DT = 3.141592653589793238462643f;

	if(1 == wParam || 2 == wParam)
	{
		float ratio = (float)sin(float(currentFrame) / float(maxNumFrames) * PI25DT / 2.0f);
		float alpha = ratio * 255.0f;
		float fSize  = ratio * 80.0f;
		size = static_cast<int>(fSize);
		wndSize.cx = size;
		wndSize.cy = size;

		Update(size, (int)alpha);
	}
	else if(3 == wParam || 4 == wParam)
	{
		if(NULL == gdipBmpLarge)
		{
			HBITMAP hBmp = item->GetBigIcon();
			//entry->WaitForLargeBitmapPreview(INFINITE);
			if(hBmp != NULL)
			{
				SIZE bmpS = item->GetSize();
				smallAndLargeAreTheSame = item->AreSmallAndBigIconTheSame();//entry->AreSmallAndLargePreviewTheSame();
				gdipBmpLarge = new Bitmap(bmpS.cx, bmpS.cy);
				BitmapData data;
				Rect rc(0,0,bmpS.cx, bmpS.cy);
				gdipBmpLarge->LockBits(&rc, ImageLockModeWrite,PixelFormat32bppPARGB, &data);
				GetBitmapBits(hBmp,data.Stride * data.Height, data.Scan0);
				gdipBmpLarge->UnlockBits(&data);
			}
		}


		if(NULL == gdipBmpLarge)
		{
			//KillTimer(3);
			//KillTimer(4);

			//if(3 == wParam)
			//	SetTimer(1, CheckAnimSpeed());
			//else if(4 == wParam)
			//	SetTimer(1, CheckAnimSpeed());
			return 0;
		}

		int w = gdipBmpLarge->GetWidth();
		int h = gdipBmpLarge->GetHeight();

		float ratio = (float)sin(float(currentLargeFrame) / float(maxNumFramesLarge) * PI25DT / 2.0f);
		float alpha = (float)sin(float(currentLargeFrame) / float(maxNumFramesLarge) * PI25DT / 2.0f) * 255.0f;
		float alpha2 = (float)sin(float(currentLargeFrame) / float(maxNumFramesLarge) * PI25DT / 2.0f + PI25DT / 2.0f) * 255.0f;
		float fSizeX = ratio * float(w - size) + size;
		float fSizeY = ratio * float(h - size) + size;
		float fSize2 = float(size) - ratio * float(size);
		SIZE s = {int(fSizeX), int(fSizeY)};
		SIZE s2 = s;
		wndSize = s;
		UpdateLarge(s, s2, static_cast<BYTE>(alpha), static_cast<BYTE>(alpha2), TRUE);
	}

	if(1 == wParam)
	{
		if(currentFrame < maxNumFrames)
		{
			currentFrame += 1;
		}
		else
		{
			KillTimer(1);
		}
	}
	else if(2 == wParam)
	{
		if(currentFrame >= 0)
		{
			currentFrame -= 1;
		}
		else
		{
			KillTimer(2);
			SafeDestroy();
		}
	}
	else if(3 == wParam)
	{
		if(currentLargeFrame < maxNumFramesLarge)
			currentLargeFrame += 1;
		else
			KillTimer(3);
	}
	else if(4 == wParam)
	{
		if(currentLargeFrame >= 0)
			currentLargeFrame -= 1;
		else
		{
			KillTimer(4);
			Update(size, 255);
		}
		
	}

	return 0;
}

LRESULT CDeskletItemDragAnimation::OnCaptureChanged(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SafeDestroy();
	return 0;
}

LRESULT CDeskletItemDragAnimation::OnKeyDown(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nVirtKey = (int) wParam;
	if(VK_ESCAPE == nVirtKey)
		SafeDestroy();

	return 0;
}

LRESULT CDeskletItemDragAnimation::OnLButtonUp(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { GET_X_LPARAM(GetMessagePos()), GET_Y_LPARAM(GetMessagePos())};
	//POINT pt = {0};
	//GetCursorPos(&pt);

	if(IsInsideForbiddenWindowArea())
	{
		//DWORD newExFlags = WS_EX_TRANSPARENT;
		//ModifyStyleEx(0,newExFlags,0);
		//ReleaseCapture();

		KillTimer(1);
		SetTimer(2,CheckAnimSpeed());
	}
	else
	{
		DWORD newExFlags = WS_EX_TRANSPARENT;
		ModifyStyleEx(0,newExFlags,0);

		int style = STYLE_NORMAL;
		HWND hwnd = ::WindowFromPoint(pt);

		HWND progman = FindWindow(_T("Progman"), 0);
		if(NULL == progman)
			progman = FindWindow(_T("DesktopBackgroundClass"), 0);
		if(hwnd == progman || ::GetParent(hwnd) == progman || ::GetParent(::GetParent(hwnd)) == progman)
			style = STYLE_ICON;
		
		VARIANT_BOOL vbShowCaseActive = {VARIANT_FALSE};
		if(_ShowCase != NULL && SUCCEEDED(_ShowCase->get_Active(&vbShowCaseActive)) &&
			vbShowCaseActive != VARIANT_FALSE)
		{
			style = STYLE_SHOWCASE;
		}

		pt.x -= wndSize.cx /2;
		pt.y -= wndSize.cy /2;
		HWND deskletHwnd = item->Instantiate(TRUE, &pt, FALSE, NULL);
		Docklet* d = Docklet::FindDocklet(deskletHwnd);

		//Docklet* d = entry->Instantiate(FALSE, TRUE, &pt);
		//d->WaitForDockletBeingInitialized(INFINITE);
		if(STYLE_SHOWCASE == style)
			d->SetIsShowCased(true);
		d->UpdateLabelPosition();
		d->ApplyStyle(style);
		d->ShowWindow(SW_SHOW);
		d->GetLabel().ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);
		/*
		HDWP hDwp = ::BeginDeferWindowPos(3);

		//if()
		{
			DWORD flags = SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW;
			d->Redraw(true);
			hDwp = ::DeferWindowPos(hDwp, d->GetHWnd(),0,0,0,0,0,flags);
			if(d->GetLabelType() != LABEL_NONE)
			{
				hDwp = ::DeferWindowPos(hDwp, d->GetLabelHwnd(),0,0,0,0,0,flags);
			}
		}

		hDwp = ::DeferWindowPos(hDwp, m_hWnd, 0, 0,0,0, 0 , SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_HIDEWINDOW);

		::EndDeferWindowPos(hDwp);

		//d->FadeIn(-1);
		//ShowWindow(SW_HIDE);
		*/

		SafeDestroy();
	}

	return 0;
}

void CDeskletItemDragAnimation::StartAnimation(CDeskletItem* deskletItem, const SIZE& s, HWND parentHwnd)
{
	forbiddenWindow = parentHwnd;
	item = deskletItem;
	if(NULL == item)
	{
		delete this;
		return;
	}

	item->AddRef();

	wasInForbiddenWindowArea = IsInsideForbiddenWindowArea();

	item->StartCreatingBigIcon(NULL, 0);
	//entry->StartCreatingLargePreviewBitmap(NULL, 0);

	EnsureWindowIsLayered();
	//ShowWindow(SW_SHOWNA);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);

	//HDC nullDC = ::GetDC(NULL);
	//CDC sourceDC;
	//sourceDC.CreateCompatibleDC(nullDC);
	//sourceDC.SelectBitmap(hBmp);

	//::ReleaseDC(NULL, nullDC);

	gdipBmp = new Bitmap(s.cx, s.cy);
	BitmapData data;
	Rect rc(0,0,s.cx, s.cy);
	gdipBmp->LockBits(&rc, ImageLockModeWrite,
		PixelFormat32bppPARGB, &data);

	GetBitmapBits(item->GetSmallIcon(),data.Stride * data.Height, data.Scan0);

	gdipBmp->UnlockBits(&data);

	SetCapture();

	SetTimer(1,CheckAnimSpeed());
}
