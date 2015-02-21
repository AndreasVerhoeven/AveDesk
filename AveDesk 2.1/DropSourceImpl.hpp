#ifndef DROPSOURCEIMPL_HPP_2302939426539436939393
#define DROPSOURCEIMPL_HPP_2302939426539436939393


#include <windows.h>
#include <oleidl.h>
#include <shlobj.h>
#include <prsht.h>
#include <atlcom.h>

#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;

template<typename T> class ATL_NO_VTABLE IDropSourceImpl : 
	public IDropSource
{
protected:
	CComPtr<IDragSourceHelper> dragSourcerHelper;
public:
	IDropSourceImpl()
	{
		CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER,
                    IID_IDragSourceHelper, reinterpret_cast<void**>(&dragSourcerHelper));	
	}


	// IDropSource
	STDMETHOD(QueryContinueDrag)(BOOL fEscapePressed, DWORD grfKeyState)
	{
		if(fEscapePressed || (MK_LBUTTON | MK_RBUTTON) == (grfKeyState & (MK_LBUTTON | MK_RBUTTON)))
		    return DRAGDROP_S_CANCEL;

		if((grfKeyState & (MK_LBUTTON | MK_RBUTTON)) == 0)
			return DRAGDROP_S_DROP;
		
		return S_OK;
	}

    STDMETHOD(GiveFeedback)(DWORD dwEffect)
	{
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}


	HRESULT SetDragImage(IDataObject* data, Bitmap& bmp, POINT pt)
	{
		// only winXP compatible, will result in a bogus image since
		// w2k doesnt seem to support alpha-blended dragimages.
		// (todo: add a win2k check here)

		SIZE s = {bmp.GetWidth(),bmp.GetHeight()};

		// create a HBITMAP that's compatible with the screen.
		HDC screenDC = GetDC(0);
		HBITMAP hBmp = CreateCompatibleBitmap(screenDC,s.cx, s.cy );
		ReleaseDC(0,screenDC);
					

		// copy the GDI+ bitmap to the GDI HBITMAP in a dirty way.
		BitmapData bmpData;
		Rect rc(0,0, bmp.GetWidth(), bmp.GetHeight() );
		bmp.LockBits(&rc,ImageLockModeRead,PixelFormat32bppARGB,&bmpData);
		SetBitmapBits(hBmp,s.cx * s.cy * 4,bmpData.Scan0);
		bmp.UnlockBits(&bmpData);

					
		// set up a SHDRAGIMAGE structure.
		SHDRAGIMAGE shdi = {0};
		shdi.sizeDragImage = s;
		shdi.ptOffset.x = pt.x;
		shdi.ptOffset.y = pt.y;
		shdi.hbmpDragImage = hBmp;
		shdi.crColorKey = 0xFFFFFFFF;

		HRESULT hRes = dragSourcerHelper->InitializeFromBitmap(&shdi,data);

		// if failed, we need to clean up the  GDI HBITMAP, otherwise ownership goes
		// to the DragSourceHelper.
		if(FAILED(hRes))
				DeleteObject(hBmp);

		return hRes;
	}


	HRESULT BeginDrag(CComPtr<IDataObject> data, DWORD* dwEffect, DWORD dwOkEffect = DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK)
	{
		T* t = static_cast<T*>(this);	

		CComPtr<IDropSource> source;
		HRESULT hRes = t->QueryInterface(IID_IDropSource,(LPVOID*)&source);
		if(FAILED(hRes))
			return E_UNEXPECTED;

		hRes = DoDragDrop(data,source,dwOkEffect,dwEffect);

		return t->EndDrag(hRes);
	}

	HRESULT EndDrag(HRESULT hRes)
	{
		return hRes;
	}
};

#endif//DROPSOURCEIMPL_HPP_2302939426539436939393