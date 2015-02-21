// filename:		layer.cpp
// Description:		Implementation of class Layer
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "image_funcs.h"

Bitmap* ConvoluteDim(Bitmap * bmp, bool isHorizontal, int* filterVector, UINT dim, int denominator)
{
	Rect rc(0, 0, bmp->GetWidth(), bmp->GetHeight());

	BitmapData dataSrc;
	bmp->LockBits(&rc, ImageLockModeRead, PixelFormat32bppARGB, & dataSrc);

	UINT d = dim / 2;

	Bitmap* destBmp = new Bitmap(rc.Width, rc.Height, PixelFormat32bppARGB);

	BitmapData dataDest;
	destBmp->LockBits(&rc, ImageLockModeRead | ImageLockModeWrite,
		PixelFormat32bppARGB, & dataDest);


	BYTE* startSrc  = (BYTE*) dataSrc.Scan0;
	BYTE* startDest = (BYTE*) dataDest.Scan0;

	UINT nLines		= {0};
	UINT nPixels	= {0};
	UINT dPixelSrc	= {0};
	UINT dPixelDest	= {0};
	UINT dLineSrc	= {0};
	UINT dLineDest	= {0};

	if (isHorizontal)
	{
		nLines = dataDest.Height;
		nPixels = dataDest.Width;
		dPixelSrc = 4;
		dPixelDest = 4;
		dLineSrc = dataSrc.Stride;
		dLineDest = dataDest.Stride;
	}
	else
	{
		nLines = dataDest.Width;
		nPixels = dataDest.Height;
		dPixelSrc = dataSrc.Stride;
		dPixelDest = dataDest.Stride;
		dLineSrc = 4;
		dLineDest = 4;
	}

	if (d > nPixels / 2)
		d = nPixels / 2;

	for (int plane = 0; plane < 4; plane++)
	{

		BYTE* lineSrc = startSrc;
		BYTE* lineDest = startDest;

		for (UINT line = 0; line < nLines; line++)
		{
			BYTE* pPixelDest = lineDest;

			for (UINT pxl = 0; pxl < d; pxl++)
			{
				int * pFactors = filterVector + d - pxl;

				UINT xEnd = pxl + d;
				if (xEnd > nPixels) xEnd = nPixels;

				int denom = 0;
				int sum = 0;

				BYTE * pPixelSrc = lineSrc;

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

			for (pxl = d; pxl < nPixels - d; pxl++)
			{
				int * pFactors = filterVector;
				int sum = 0;
				UINT xBegin = pxl - d;
				BYTE * pPixelSrc = & lineSrc[xBegin * dPixelSrc];

				for (UINT x = xBegin; x <= pxl + d; x++)
				{
					sum += *pFactors++ * *pPixelSrc;
					pPixelSrc += dPixelSrc;
				}

				sum /= denominator;
				*pPixelDest =  (BYTE)sum;
				pPixelDest += dPixelDest;
			}

			for (pxl = nPixels - d; pxl < nPixels; pxl++)
			{
				int * pFactors = filterVector;
				int denom = 0;
				int sum = 0;

				int xBegin = pxl - d;
				if (xBegin < 0) xBegin = 0;
				BYTE * pPixelSrc = & lineSrc[xBegin * dPixelSrc];

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

			lineSrc += dLineSrc;
			lineDest += dLineDest;
		}

		startSrc++;
		startDest++;
	}
	destBmp->UnlockBits(& dataDest);
	bmp->UnlockBits(& dataSrc);

	return destBmp;
}

Bitmap* GetBlur(Bitmap* bmp, REAL radius)
{
	int* filterVector = 0;
	UINT dim = (int) floor(radius + 0.5f + 5.0f * radius + 1.0f);
	dim |= 1;
	UINT d = dim;
	int denominator = 0;

	if(dim != 0)
	{
		filterVector = new int[dim];
		d /= 2;
		REAL num = 2 * radius * radius;
		REAL f = expf(d * d / num);
		denominator = (int) floor(f + 0.5f);
		filterVector[d] = denominator;

		for (UINT i = 1; i <= d; i++)
		{
			int i2 = - (int)(i * i);
			int v = (int) floor((f * expf(i2 / num)) + 0.5f);
			filterVector[d - i] = filterVector[d+i] = v;
			denominator += 2 * v;
		}
	}

	Bitmap* temp = ConvoluteDim(bmp, true,filterVector,dim,denominator);
	Bitmap* result = ConvoluteDim(temp, false, filterVector,dim,denominator);

	delete[] filterVector;
	delete temp;

	return result;
}