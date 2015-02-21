// CAveDeskImageShellExt.cpp : Implementation of CCAveDeskImageShellExt
#include "stdafx.h"
#include "AveDeskShellExt.h"
#include "CAveDeskImageShellExt.h"

#include "unzip/zlib/unzip.h"

//#pragma comment(lib,"unzip/zlib/Unzip.lib")
//#pragma comment(lib,"unzip/zlib/Unzipd.lib")

/////////////////////////////////////////////////////////////////////////////
// CCAveDeskImageShellExt


HRESULT CCAveDeskImageShellExt::GetDateStamp(FILETIME *pDateStamp)
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

// PREVIEWFILENAMES hold the filenames that are looked up in the zipfile (aveinst) file
// to generate a thumbnail for the aveinst-file
// (the used zip-library is using ANSI-string, so
// they are used here too.)
const char* PREVIEWFILENAMES[] = {
					 "avedesk_preview.png",
					 "avedesk_preview.jpg",
					 "avedesk_preview.bmp",
					 "preview.png",
					 "about.png",
					 "about.jpg",
					 "preview.jpg"
		};
// PREVIEWFILENAMES_COUNT must equal the number of strings in PREVIEWFILENAMES
const int PREVIEWFILENAMES_COUNT = 7;


/**
 * Locates a preview file in an aveinst (zip) file
 * @param unzFile: a pointer previously obtained by unzOpen() for the file to look in
 * @return: NOERROR if a preview-file was found, a COM errorcode otherwise.
 * @effect: the currentfile for the unzFile will be changed to the found previewfile
 */
HRESULT LocatePreviewFileInZip(void* unzFile)
{
	if(NULL == unzFile)
		return E_FAIL;

	int ret = !UNZ_OK;
	int i = 0;
	while(ret != UNZ_OK && i < PREVIEWFILENAMES_COUNT)
	{
		// 2 means no casesensitive-compare
		ret = unzLocateFile(unzFile, PREVIEWFILENAMES[i], 2);
		i++;
	}

	return ret == UNZ_OK ? NOERROR : E_FAIL;
}

/**
 * Calculates the optimal thumbnail rectangle
 * @param reqSize: the requested thumbnail size
 * @param bmpSize: the bitmap size
 * @return A GDI+ Rect holding the optimal thumbnail rectangle
 */
Rect CalculateThumbnailRect(const SIZE& reqSize, const SIZE& bmpSize)
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

HRESULT CCAveDeskImageShellExt::InternalDraw(HDC dc, IStream* stream)
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

HRESULT CCAveDeskImageShellExt::DrawPreviewBitmapOnHDC(HDC dc, IStream* stream)
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

STDMETHODIMP CCAveDeskImageShellExt::Extract(HBITMAP *phBmpImage)
{
	// pre:  phBmpImage is not NULL
	// post: The extracted thumbnail has been returned (in phBmpImage)

	USES_CONVERSION;

	if(NULL == phBmpImage)
		return E_INVALIDARG;

	*phBmpImage = NULL;

	if(!PathFileExists(m_FileName))
		return E_FAIL;


	void* unzFile = unzOpen(T2A(m_FileName));
	if(NULL == unzFile)
		return E_FAIL;

	HRESULT hRes = NOERROR;
	hRes = LocatePreviewFileInZip(unzFile);
	if(SUCCEEDED(hRes))
	{
		int ret = unzOpenCurrentFile(unzFile);
		if(UNZ_OK == ret)
		{
			// an OLE function is going to be used, so init COM and OLE
			CoInitialize(NULL);
			OleInitialize(NULL);

			// this stream is going to be used to read the preview image
			// out from the zipfile and then use GDI+ to convert the stream
			// into a GDI+ Bitmap object which can be used to draw the thumbnail then.
			IStream* stream    = NULL;
			HRESULT hRes = CreateStreamOnHGlobal(NULL, TRUE, (LPSTREAM*)&stream);
			if(SUCCEEDED(hRes))
			{
				// read all the bytes from the zipped file into the stream 
				const BUFSIZE = 2048;
				BYTE buffer[BUFSIZE];

				HRESULT writeResult = NOERROR;
				int bytesRead = 0;
				while( SUCCEEDED(writeResult)  && (bytesRead = unzReadCurrentFile(unzFile, buffer, BUFSIZE)) > 0)
				{
					hRes = stream->Write(buffer, bytesRead, NULL);
				}

				if(SUCCEEDED(writeResult))
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
				else
				{
					hRes = E_FAIL;
				}

				// the stream has ownership of the global memory that was used,
				// so it will free that for us.
				stream->Release();

				// done using COM/OLE, so uninit it.
				CoUninitialize();
				OleUninitialize();
			}
			else
			{
				hRes = E_FAIL;
			}

			// done using the zipped (aveinst) file, so close it.
			unzCloseCurrentFile(unzFile);
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


	unzClose(unzFile);

	return hRes;
}

STDMETHODIMP CCAveDeskImageShellExt::GetLocation( 
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


