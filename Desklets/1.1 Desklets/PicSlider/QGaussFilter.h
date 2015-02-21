#pragma once

// QGaussFilter
//
// MFC class to apply Unsharp Mask or Blur to a GDI+ Bitmap.
// Use at your own risk. Comments welcome.
//
// Version 1.0 (c) 2003, Sjaak Priester, Amsterdam.
// mailto:sjaak@sjaakpriester.nl

#define QM_GAUSSFILTER	(WM_APP + 8)
// Special windows message.
// After MakeUnsharpMask() or MakeBlur() is finished, QGaussFilter posts this message to the
// associated window pMsgWnd.
// LPARAM contains a Bitmap *, pointing to a new, convoluted bitmap. The window
// gets ownership of the bitmap and is responsible for it's destruction.

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

class QGaussFilter
{
public:
	QGaussFilter();
	~QGaussFilter();

	enum Flags
	{
		PlaneBlue = 0x01,
		PlaneGreen = 0x02,
		PlaneRed = 0x04,
		PlaneAll = PlaneBlue | PlaneGreen | PlaneRed,
	};
	// flags: any combination of:
	//	- PlaneRed		to convolute the red plane
	//	- PlaneGreen	to convolute the green plane
	//	- PlaneBlue		to convolute the blue plane


	Bitmap * GetUnsharpMask(Bitmap * pSrc, REAL radius, REAL depth,	Rect * pRect = NULL, UINT flags = PlaneAll);
	Bitmap * GetBlur(Bitmap * pSrc, REAL radius,Rect * pRect = NULL, UINT flags = PlaneAll, bool bCopy = true);
	// Calculate and return an Unsharp Mask or a Blur bitmap. The functions return
	// after the result is available.
	//
	// Parameters: identical to the parameters of MakeUnsharpMask() and MakeBlur().
	// - bCopy		if true, copies non-convoluted color planes; used internally, should
	//				be true.

	static void RGBtoYUV(Bitmap * pBitmap) { ToColorSpace(pBitmap, MatRGBtoYUV); }
	static void YUVtoRGB(Bitmap * pBitmap) { ToColorSpace(pBitmap, MatYUVtoRGB); }
	// Utility functions: use these to convert the bitmap pointed to by pBitmap from
	// YUV color space to RGB color space, or vice versa.

	static const ColorMatrix MatRGBtoYUV;
	static const ColorMatrix MatYUVtoRGB;
	// These two GDI+ color matrices are used to convert between RGB and YUV color spaces.
	// This is needed for convolution of the luminance only.

	UINT m_MaxDim;
	// The maximum dimension of the convolution matrix; default value is 51. If you
	// know what you're doing, you may set this to another value.

protected:
	int *m_FilterVector;
	int m_Denominator;
	UINT m_Dim;
	UINT m_Flags;
	UINT m_Message;
	Bitmap *m_pSource;
	REAL m_Radius;
	REAL m_Depth;
	Rect *m_pRect;
	bool m_bUnsharpMask;

	bool SetRadius(REAL radius);
	Bitmap * ConvoluteDimension(Bitmap * pSrc, Rect * pRect = NULL,
		UINT flags = PlaneAll, bool bHorizontal = true, bool bCopy = true);

	static void ToColorSpace(Bitmap * pBitmap, const ColorMatrix& mat);
};
