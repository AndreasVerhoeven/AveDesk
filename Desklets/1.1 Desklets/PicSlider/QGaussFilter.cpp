// QGaussFilter.cpp
//
// MFC class to apply Unsharp Mask or Blur to a GDI+ Bitmap.
// Use at your own risk. Comments welcome.
//
// Version 1.0 (c) 2003, Sjaak Priester, Amsterdam.
// mailto:sjaak@sjaakpriester.nl

#include "QGaussFilter.h"
#include "Int.h"

#include <math.h>


const ColorMatrix QGaussFilter::MatRGBtoYUV =
{
	0.5f, 0.3086f, -0.1681f, 0, 0,
	-0.4407f, 0.6094f, -0.3391f, 0, 0,
	-0.0593f, 0.082f, 0.5f, 0, 0,
	0, 0, 0, 1, 0,
	0.5f, 0, 0.5f, 0, 1
};

const ColorMatrix QGaussFilter::MatYUVtoRGB =
{
	1.383f, -0.7002f, 0, 0, 0,
	1, 1, 1, 0, 0,
	0, -0.247f, 1.836f, 0, 0,
	0, 0, 0, 1, 0,
	-0.6914f, 0.4736f, -0.918f, 0, 1
};

QGaussFilter::QGaussFilter()
: m_FilterVector(NULL)
, m_Denominator(0)
, m_Dim(0)
, m_MaxDim(51)
, m_Flags(PlaneAll)
, m_pSource(NULL)
, m_bUnsharpMask(false)
, m_pRect(NULL)
{
}

QGaussFilter::~QGaussFilter(void)
{
	delete[] m_FilterVector;
	delete m_pRect;
}

// Calculate and return Blur bitmap
Bitmap * QGaussFilter::GetBlur(Bitmap * pSrc, REAL radius, Rect * pRect, UINT flags, bool bCopy)
{
	if (! SetRadius(radius)) return NULL;

	// Convolute in horizontal direction
	Bitmap * pTemp = ConvoluteDimension(pSrc, pRect, flags, true, bCopy);

	if (! pTemp) return NULL;

	// Convolute the result in vertical direction
	Bitmap * pResult = ConvoluteDimension(pTemp, pRect, flags, false, bCopy);
	delete pTemp;
	return pResult;
}

bool QGaussFilter::SetRadius(REAL radius)
{
	delete[] m_FilterVector;
	m_FilterVector = 0;
	m_Dim = 0;

	// d is the effective diameter in pixels; all weight factors outside d are assumed
	// to be zero. The factor 5.0 is somewhat arbitrary; a value between 4.0 and 6.0
	// is generally recommended.
	UINT d = Int(5.0f * radius + 1.0f);

	if (d > m_MaxDim) return false;	// radius to great
	d |= 1;		// must be odd
	m_Dim = d;

	if (m_Dim == 0) return true;	// radius 0 is acceptable; effectively no convolution

	m_FilterVector = new int[m_Dim];
	d /= 2;

	REAL num = 2 * radius * radius;
	REAL f = expf(d * d / num);
	m_Denominator = Int(f);

	m_FilterVector[d] = m_Denominator;

	for (UINT i = 1; i <= d; i++)
	{
		int i2 = - (int)(i * i);
		int v = Int(f * expf(i2 / num));
		m_FilterVector[d - i] = v;
		m_FilterVector[d + i] = v;
		m_Denominator += 2 * v;
	}

	return true;
}

// Calculate and return Unsharp Mask bitmap
Bitmap * QGaussFilter::GetUnsharpMask(Bitmap * pSrc, REAL radius, REAL depth, Rect * pRect, UINT flags)
{
	// Start with blur
	Bitmap * pResult = GetBlur(pSrc, radius, pRect, flags, false);
	if (pResult)
	{
		// Subtract blurred bitmap from original to get Unsharp Mask
		Rect rcSrc(0, 0, pSrc->GetWidth(), pSrc->GetHeight());
		if (pRect) rcSrc.Intersect(* pRect);

		BitmapData dataSrc;
		Status s = pSrc->LockBits( &rcSrc, ImageLockModeRead,
			PixelFormat32bppARGB, &dataSrc);
		if (s != Ok)
		{
			delete pResult;
			return NULL;
		}

		Rect rcResult(0, 0, pResult->GetWidth(), pResult->GetHeight());

		BitmapData dataResult;
		s = pResult->LockBits(&rcResult, ImageLockModeRead | ImageLockModeWrite,
			PixelFormat32bppARGB, & dataResult);
		if (s != Ok)
		{
			pSrc->UnlockBits(& dataSrc);
			delete pResult;
			return NULL;
		}

		const int nPlanes = 4;

		// On modern systems, the difference is not big, but real math is still somewhat
		// slower than integer math. But if this ever changes, you may define REAL_MATH.

		int denom = 10000;	// use an arbitrary denominator, not too small
		int dpt = Int((REAL) denom * depth);
		int dptplus = dpt + denom;

		BYTE * pStartSrc = (BYTE *) dataSrc.Scan0;
		BYTE * pStartResult = (BYTE *) dataResult.Scan0;

		for (int plane = 0; plane < nPlanes; plane++)	// loop through color planes
		{
			bool bThisPlane = true;//(flags & 1) != 0;
			flags >>= 1;

			BYTE * pLineSrc = pStartSrc;
			BYTE * pLineResult = pStartResult;

			if (bThisPlane)
			{
				for (UINT line = 0; line < dataResult.Height; line++)	// loop through lines
				{
					BYTE * pPixelSrc = pLineSrc;
					BYTE * pPixelResult = pLineResult;

					for (UINT pxl = 0; pxl < dataResult.Width; pxl++)	// loop through pixels
					{
						int v = dptplus * *pPixelSrc - dpt * *pPixelResult;
						v /= denom;

						// Clipping is very essential here. for large values of depth
						// (> 5.0f) more than half of the pixel values are clipped.
						if (v > 255) v = 255;
						if (v < 0) v = 0;

						* pPixelResult = (BYTE) v;
						pPixelSrc += nPlanes;
						pPixelResult += nPlanes;
					}
					pLineSrc += dataSrc.Stride;
					pLineResult += dataResult.Stride;
				}
			}
			else		// no subtraction, just copy
			{
				for (UINT line = 0; line < dataResult.Height; line++)	// loop through lines
				{
					BYTE * pPixelSrc = pLineSrc;
					BYTE * pPixelResult = pLineResult;

					for (UINT pxl = 0; pxl < dataResult.Width; pxl++)
					{
						* pPixelResult = * pPixelSrc;
						pPixelSrc += nPlanes;
						pPixelResult += nPlanes;
					}

					pLineSrc += dataSrc.Stride;
					pLineResult += dataResult.Stride;
				}	// next line
			}

			pStartSrc++;
			pStartResult++;
		}	// next plane

		pResult->UnlockBits(& dataResult);
		pSrc->UnlockBits(& dataSrc);

	}
	return pResult;
}

Bitmap * QGaussFilter::ConvoluteDimension(Bitmap * pSrc, Rect * pRect, UINT flags,
										  bool bHorizontal, bool bCopy)
{
	Rect rc(0, 0, pSrc->GetWidth(), pSrc->GetHeight());
	if (pRect) rc.Intersect(* pRect);

	if (m_Dim <= 1) return pSrc->Clone(rc, PixelFormat32bppRGB);
		// m_Dim == 0 or 1: effectively no convolution, just return a copy

	// LockBits on source
	BitmapData dataSrc;
	Status s = pSrc->LockBits(&rc, ImageLockModeRead, PixelFormat32bppARGB, & dataSrc);
	if (s != Ok) return NULL;

	UINT d = m_Dim / 2;
	const int nPlanes = 4;

	Bitmap * pDest = new Bitmap(rc.Width, rc.Height, PixelFormat32bppARGB);
	if (! pDest)
	{
		pSrc->UnlockBits(& dataSrc);
		return NULL;
	}

	// LockBits on destination
	rc.X = 0;
	rc.Y = 0;
	BitmapData dataDest;
	s = pDest->LockBits(&rc, ImageLockModeRead | ImageLockModeWrite,
		PixelFormat32bppARGB, & dataDest);

	if (s != Ok)
	{
		pSrc->UnlockBits(& dataSrc);
		delete pDest;
		return NULL;
	}

	BYTE * pStartSrc = (BYTE *) dataSrc.Scan0;
	BYTE * pStartDest = (BYTE *) dataDest.Scan0;

	UINT nLines;		// number of lines (horizontal or vertical)
	UINT nPixels;		// number of pixels per line
	UINT dPixelSrc;		// pixel step in source
	UINT dPixelDest;	// pixel step in destination
	UINT dLineSrc;		// line step in source
	UINT dLineDest;		// line step in destination

	if (bHorizontal)
	{
		nLines = dataDest.Height;
		nPixels = dataDest.Width;
		dPixelSrc = nPlanes;
		dPixelDest = nPlanes;
		dLineSrc = dataSrc.Stride;
		dLineDest = dataDest.Stride;
	}
	else
	{
		nLines = dataDest.Width;
		nPixels = dataDest.Height;
		dPixelSrc = dataSrc.Stride;
		dPixelDest = dataDest.Stride;
		dLineSrc = nPlanes;
		dLineDest = nPlanes;
	}

	// This line added in version 1.1: avoid overrun in small bitmaps.
	if (d > nPixels / 2) d = nPixels / 2;

	for (int plane = 0; plane < nPlanes; plane++)	// loop through color planes
	{
		bool bThisPlane = true;//(flags & 1) != 0;

		flags >>= 1;

		BYTE * pLineSrc = pStartSrc;
		BYTE * pLineDest = pStartDest;

		if (bThisPlane)
		{
			for (UINT line = 0; line < nLines; line++)	// loop through lines
			{
				BYTE * pPixelDest = pLineDest;

				for (UINT pxl = 0; pxl < d; pxl++)	// loop through pixels in left/top margin
				{
					int * pFactors = m_FilterVector + d - pxl;

					UINT xEnd = pxl + d;
					if (xEnd > nPixels) xEnd = nPixels;

					int denom = 0;
					int sum = 0;

					BYTE * pPixelSrc = pLineSrc;

					for (UINT x = 0; x < xEnd; x++)
					{
						denom += *pFactors;
						sum += *pFactors++ * *pPixelSrc;
						pPixelSrc += dPixelSrc;
					}

					if (denom) sum /= denom;
					* pPixelDest = (BYTE) sum;

					pPixelDest += dPixelDest;
				}

				for (pxl = d; pxl < nPixels - d; pxl++)	// loop through pixels in main area
				{
					int * pFactors = m_FilterVector;
					int sum = 0;

					UINT xBegin = pxl - d;
					BYTE * pPixelSrc = & pLineSrc[xBegin * dPixelSrc];

					for (UINT x = xBegin; x <= pxl + d; x++)
					{
						sum += *pFactors++ * *pPixelSrc;
						pPixelSrc += dPixelSrc;
					}

					sum /= m_Denominator;
					* pPixelDest = (BYTE) sum;

					pPixelDest += dPixelDest;
				}

				for (pxl = nPixels - d; pxl < nPixels; pxl++)
					// loop through pixels in right/bottom margin
				{
					int * pFactors = m_FilterVector;
					int denom = 0;
					int sum = 0;

					int xBegin = pxl - d;
					if (xBegin < 0) xBegin = 0;
					BYTE * pPixelSrc = & pLineSrc[xBegin * dPixelSrc];

					for (UINT x = xBegin; x < nPixels; x++)
					{
						denom += *pFactors;
						sum += *pFactors++ * *pPixelSrc;
						pPixelSrc += dPixelSrc;
					}

					if (denom) sum /= denom;

					* pPixelDest = (BYTE) sum;

					pPixelDest += dPixelDest;
				}

				pLineSrc += dLineSrc;
				pLineDest += dLineDest;
			}	// next line

		}
		else if (bCopy)		// no convolution, just copy
		{
			for (UINT line = 0; line < nLines; line++)	// loop through lines
			{
				BYTE * pPixelSrc = pLineSrc;
				BYTE * pPixelDest = pLineDest;

				for (UINT pxl = 0; pxl < nPixels; pxl++) // loop through pixels
				{
					* pPixelDest = * pPixelSrc;
					pPixelSrc += dPixelSrc;
					pPixelDest += dPixelDest;
				}

				pLineSrc += dLineSrc;
				pLineDest += dLineDest;
			}	// next line
		}

		pStartSrc++;
		pStartDest++;
	}	// next plane

	pDest->UnlockBits(& dataDest);
	pSrc->UnlockBits(& dataSrc);

	return pDest;
}

/* static */
void QGaussFilter::ToColorSpace(Bitmap * pBitmap, const ColorMatrix& mat)
{
	Rect rc(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());

	ImageAttributes attr;
	attr.SetColorMatrix(& mat);

	// Although undocumented, it seems perfectly legal to draw a bitmap on itself.
	Graphics g(pBitmap);
	g.DrawImage(pBitmap,
		rc,
		0, 0, rc.Width, rc.Height,
		UnitPixel,
		& attr);
}
