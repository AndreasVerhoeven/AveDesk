// CAveDeskThemeShellExt.cpp : Implementation of CCAveDeskThemeShellExt
#include "stdafx.h"
#include "AveDeskShellExt.h"
#include "CAveDeskThemeShellExt.h"

/////////////////////////////////////////////////////////////////////////////
// CCAveDeskThemeShellExt

/**
 * Calculates the optimal thumbnail rectangle
 * @param reqSize: the requested thumbnail size
 * @param bmpSize: the bitmap size
 * @return A GDI+ Rect holding the optimal thumbnail rectangle
 */
Rect CCAveDeskThemeShellExt::CalculateThumbnailRect(const SIZE& reqSize, const SIZE& bmpSize)
{	
	if(reqSize.cx >= bmpSize.cx && reqSize.cy >= bmpSize.cy)
	{
		return Rect(reqSize.cx / 2 - bmpSize.cx / 2, reqSize.cy /2 - bmpSize.cy / 2, bmpSize.cx, bmpSize.cy);
	}
	else
	{
		float useWidth = float(reqSize.cx) / float(bmpSize.cx);
		float useHeight = float(reqSize.cy) / float(bmpSize.cy);
	
		SIZE finalSize = {0};
	
		if(useWidth<useHeight)
		{
			// use width
			finalSize.cx= (int)(float(bmpSize.cx) * useWidth);	
			finalSize.cy = (int)(float(bmpSize.cy)* useWidth);
		}
		else
		{
			// use height
			finalSize.cx = (int) (float(bmpSize.cx) * useHeight);	
			finalSize.cy = (int)(float(bmpSize.cy)  * useHeight);
		}
	
		return Rect(reqSize.cx / 2 - finalSize.cx /2 , reqSize.cy / 2 - finalSize.cy / 2, finalSize.cx, finalSize.cy);
	}
}

HRESULT CCAveDeskThemeShellExt::GetDateStamp(FILETIME *pDateStamp)
{
	// pre:  pDateStamp is not NULL
	// post: the datestamp for this thumbnail has been returned (in pDateStamp)

	if(NULL == pDateStamp)
		return E_INVALIDARG;

	FILETIME ftCreationTime,ftLastAccessTime,ftLastWriteTime;

	HANDLE hFile = CreateFile(m_FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(NULL == hFile)
		return E_FAIL;


	GetFileTime(hFile,&ftCreationTime,&ftLastAccessTime,&ftLastWriteTime);
	CloseHandle(hFile);

	// the last time of writing (editing) will be used
	// as the datestamp for this thumbnail. If the file has been changed,
	// there is a chance the embedded - in the zipfile - thumbnail has been
	// changed too.
	*pDateStamp = ftLastWriteTime;

	return NOERROR; 
}

HRESULT CCAveDeskThemeShellExt::InternalDraw(HDC dc, IStream* stream)
{
	// pre: dc is not NULL, stream is not NULL
	// post: NOERROR has been returned iff successfull, otherwise a COM errorcode

	if(NULL == dc)
		return E_FAIL;

	if(NULL == stream)
		return E_FAIL;

	Graphics g(dc);
	if(g.GetLastStatus() != Ok)
		return E_FAIL;

	// draw the thumbnail with best quality.
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	// load the preview-image from the stream
	Bitmap previewBitmap(stream, FALSE);
	if(previewBitmap.GetLastStatus() != Ok)
		return E_FAIL;

	Status st = Ok;

	// calculate the optimal thumbnail size
	int width  = previewBitmap.GetWidth();
	int height = previewBitmap.GetHeight();
	SIZE bitmapSize = {width,height};
	Rect drawRect = CalculateThumbnailRect(m_Size, bitmapSize);

	// if the colordepth is not 32 bits, alpha-blending can't be used,
	// so we set the background to WHITE.
	if(m_ColorDepth != 32)
	{
		g.Clear(Color(255,255,255,255));
	}

	st = g.DrawImage(&previewBitmap, drawRect, 0,0, width, height, UnitPixel, NULL, NULL, NULL);


	return st == Ok ? NOERROR : E_FAIL;

}

HRESULT CCAveDeskThemeShellExt::DrawPreviewBitmapOnHDC(HDC dc, IStream* stream)
{
	// pre:  dc is not NULL, stream is not NULL
	// post: NOERROR has been returned iff successfull, otherwise a COM errorcode

	if(NULL == dc)
		return E_FAIL;

	if(NULL == stream)
		return E_FAIL;

		// start up GDI+
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	if(st !=  Ok)
		return E_FAIL;

	HRESULT hRes = InternalDraw(dc, stream);

	GdiplusShutdown(gdiplusToken);

	return hRes;
}

STDMETHODIMP CCAveDeskThemeShellExt::Extract(HBITMAP *phBmpImage)
{
	// pre:  phBmpImage is not NULL
	// post: The extracted thumbnail has been returned (in phBmpImage)

	USES_CONVERSION;

	if(NULL == phBmpImage)
		return E_INVALIDARG;

	*phBmpImage = NULL;

	if(!PathFileExists(m_FileName))
		return E_FAIL;


	// an OLE function is going to be used, so init COM and OLE
	CoInitialize(NULL);
	OleInitialize(NULL);

	HRESULT hRes = S_OK;
	if(StgIsStorageFile(m_FileName) == S_OK) // explicit check for S_OK
	{
		CComPtr<IStorage> storage;
		hRes = StgOpenStorageEx(m_FileName, STGM_READ | STGM_DIRECT | STGM_SHARE_DENY_WRITE, STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, reinterpret_cast<void**>(&storage));
		if(SUCCEEDED(hRes) && storage.p != NULL)
		{
			IStream* stream = NULL;
			hRes = storage->OpenStream(L"Preview", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, &stream);
			if(SUCCEEDED(hRes) && stream != NULL)
			{

				//if(SUCCEEDED(writeResult))
				{
					// we create a DIBSection with the requested size and colordepth to
					// draw the thumbnail upon.
					BITMAPINFO bi = {0};
					bi.bmiHeader.biSize = sizeof(bi);
					bi.bmiHeader.biBitCount = (short) m_ColorDepth;
					bi.bmiHeader.biPlanes = 1;
					bi.bmiHeader.biWidth = m_Size.cx;
					// a topdown bitmap is used, thus the height is negative.
					bi.bmiHeader.biHeight = -m_Size.cy; 
					void* ppvBits = NULL;
					HBITMAP hBitmap = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS,&ppvBits, NULL, 0 );

					if(hBitmap != NULL)
					{		
						// create a device context we are going to use
						HDC screenDC = GetDC(NULL);
						HDC dc = CreateCompatibleDC(screenDC);
						if(dc != NULL)
						{
							HBITMAP oldBitmap = (HBITMAP) SelectObject(dc, (HGDIOBJ) hBitmap);
								
							hRes = DrawPreviewBitmapOnHDC(dc, stream);
			
							// clear up the device-context we used
							SelectObject(dc, (HGDIOBJ) oldBitmap);
							DeleteDC(dc);
							ReleaseDC(NULL, screenDC);

							if(FAILED(hRes))
							{
								// the thumbnail was not drawn properly,
								// so it needs to be deleted and an COM errorcode is
								// returned.
								DeleteObject((HGDIOBJ)hBitmap);
								hBitmap = NULL;
							}

							*phBmpImage = hBitmap;

						}
						else
						{
							// the bitmap was created, but we couldn't load it
							// into a device-context,  so it needs to be deleted and 
							// an COM errorcode is returned.
							DeleteObject((HGDIOBJ)hBitmap);
							hBitmap = NULL;
							hRes = E_FAIL;
						}
					}

				}
				//else
				//{
			//		hRes = E_FAIL;
			//	}

				// the stream has ownership of the global memory that was used,
				// so it will free that for us.
				stream->Release();

			}
			else
			{
				hRes = E_FAIL;
			}

		}
		else
		{
			hRes = E_FAIL;
		}
	}
	else
	{
		hRes = E_FAIL;
	}


	// done using COM/OLE, so uninit it.
	CoUninitialize();
	OleUninitialize();

	return hRes;
}

STDMETHODIMP CCAveDeskThemeShellExt::GetLocation( 
    LPWSTR pszPathBuffer,
    DWORD cchMax,
    DWORD *pdwPriority,
    const SIZE *prgSize,
    DWORD dwRecClrDepth,
    DWORD *pdwFlags
)
{
	USES_CONVERSION;

	if(NULL == pszPathBuffer)
		return E_INVALIDARG;

	if(NULL == prgSize)
		return E_INVALIDARG;

	// got to store the request-information for when Extract() is called
	m_ColorDepth = dwRecClrDepth;
	m_Size = *prgSize;
    lstrcpyn ( pszPathBuffer, T2OLE(m_FileName), cchMax - 1);

	if(pdwFlags != NULL)
	{
		// Explorer may happily cache our icon
		// For debugging purposes, this is best commented out.
		*pdwFlags |= IEIFLAG_CACHE;
		m_Flags = *pdwFlags;
	}

	return NOERROR;
}

