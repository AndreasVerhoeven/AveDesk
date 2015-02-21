
#ifndef CLAYEREDWINDOWIMPL_HPP_2302939426539436939393
#define CLAYEREDWINDOWIMPL_HPP_2302939426539436939393

#if _WIN32_WINNT < 0x500

#error "CLayeredWindowImpl requires at least _WIN32_WINNT to be 0x500 (Windows 2000 or higher)"

#else

#include "stdafx.h"
#include "resource.h"

//#ifdef USE_GDIPLUS
	#include <gdiplus.h>
	using namespace Gdiplus;
//#endif

template<typename T> class CLayeredWindowImpl : public CWindowImpl<CLayeredWindowImpl<T> >
{

	public:
		BEGIN_MSG_MAP(CLayeredWindowImpl)
		END_MSG_MAP()

		BOOL IsLayered()const
		{
			ATLASSERT( ::IsWindow(m_hWnd) );

			LONG lWinLong = GetWindowLong(GWL_EXSTYLE);
			return  ( (lWinLong & WS_EX_LAYERED) != 0 ) ? TRUE: FALSE;
		}

		BOOL EnsureWindowIsLayered()
		{
			ATLASSERT( ::IsWindow(m_hWnd) );

			ModifyStyleEx(0,WS_EX_LAYERED,0);

			return IsLayered();
		}


		BOOL SetAlpha(BYTE bAlpha)
		{
			ATLASSERT(IsLayered());

			BLENDFUNCTION blend;
            blend.BlendOp = AC_SRC_OVER;
            blend.BlendFlags = 0;
            blend.AlphaFormat = 0;
            blend.SourceConstantAlpha = bAlpha;
 
            return ::UpdateLayeredWindow(m_hWnd, NULL, NULL, NULL, NULL, NULL,
                    NULL, &blend, ULW_ALPHA);
		}

		BYTE GetAlpha()const
		{
			ATLASSERT(IsLayered());

			BYTE bAlpha = 0;
			::GetLayeredWindowAttributes(m_hWnd,0,&bAlpha,LWA_ALPHA);

			return bAlpha;
		}

		BOOL SetHDC(HDC hdcSrc, POINT *pptSrcPos, COLORREF crKey, BLENDFUNCTION* pBlendFunction, DWORD dwFlags, POINT *pptNewPos = NULL, SIZE *pNewSize=NULL)
		{
			ATLASSERT(IsLayered());

			CWindowDC screenDC(NULL);

			return ::UpdateLayeredWindow(m_hWnd,screenDC,pptNewPos,pNewSize,hdcSrc,pptSrcPos, crKey,pBlendFunction,dwFlags);

		}

		BOOL SetBitmap(HBITMAP hBitmap, BYTE bAlpha,POINT& ptNewPos, SIZE& newSize)
		{
			return SetBitmap(hBitmap,bAlpha,&ptNewPos,&newSize);
		}

		BOOL SetBitmap(HBITMAP hBitmap, BYTE bAlpha=255,POINT *pptNewPos = NULL, SIZE *pNewSize=NULL)
		{
			ATLASSERT(IsLayered());
			
			CWindowDC screenDC( NULL );

			CDC dc;
			dc.CreateCompatibleDC(screenDC);
			HBITMAP oldBitmapSelectedInDC = dc.SelectBitmap(hBitmap);

			POINT ptSourcePosition = {0,0};
			BLENDFUNCTION blendFunction = {0};

			blendFunction.BlendOp = AC_SRC_OVER;
			blendFunction.BlendFlags = 0;
			blendFunction.SourceConstantAlpha = bAlpha;
			blendFunction.AlphaFormat = AC_SRC_ALPHA;


			COLORREF crKey = RGB(255,0,255); // dummy colourkey
			BOOL ret = SetHDC(dc,&ptSourcePosition,crKey,&blendFunction,2,pptNewPos, pNewSize);

			// clean up trash
			dc.SelectBitmap(oldBitmapSelectedInDC);

			return ret;
		}


//#ifdef USE_GDIPLUS

		BOOL SetBitmap(Bitmap* bmp, BYTE bAlpha,POINT& ptNewPos, SIZE& newSize)
		{
			return SetBitmap(bmp,bAlpha,&ptNewPos,&newSize);
		}

		BOOL SetBitmap(Bitmap& bmp, BYTE bAlpha,POINT& ptNewPos, SIZE& newSize)
		{
			return SetBitmap(bmp,bAlpha,&ptNewPos,&newSize);
		}

		BOOL SetBitmap(Bitmap* bmp, BYTE bAlpha=255,POINT *pptNewPos = NULL, SIZE *pNewSize=NULL)
		{
			if(!bmp)
				return FALSE;

			return SetBitmap(*bmp,bAlpha,pptNewPos,pNewSize);
		}

		BOOL SetBitmap(Bitmap& bmp, BYTE bAlpha=255,POINT *pptNewPos = NULL, SIZE *pNewSize=NULL)
		{
			CBitmap hBmp;
			bmp.GetHBITMAP(NULL,&hBmp.m_hBitmap);

			return SetBitmap(hBmp,bAlpha,pptNewPos,pNewSize);
		}
//#endif

};

#endif//_WIN32_WINNT < 0x500

#endif//CLAYEREDWINDOWIMPL_HPP_2302939426539436939393