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


#include <memory>
#include "stdafx.h"
#include "image_funcs.h"
#include "ColorMatrixHelper.h"


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

			UINT pxl;
			for (pxl = 0; pxl < d; pxl++)
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

#define MAX_BLURRING_FACTOR 4.0f
#define MIN_BLURRING_FACTOR 0.1f

void AveDrawString(Graphics* graphics, Brush* brush, const WCHAR* str, FontFamily* fontFamily, FontStyle fs, REAL fontSize, Rect& rc, StringFormat* strFormat, bool drawShadow, CShadowProperties& shadowProperties)
{
	GraphicsPath path(FillModeAlternate);

	path.AddString(str,-1,fontFamily,fs,fontSize,rc,strFormat);



	if(drawShadow)
	{
		float blurRatio = max(min(shadowProperties.GetBlurRatio(), MAX_BLURRING_FACTOR), MIN_BLURRING_FACTOR);

		Rect rcShadow( 0, 0, rc.Width, rc.Height);
		Bitmap tmp(rcShadow.Width,rcShadow.Height);
		Graphics g(&tmp);
		g.SetSmoothingMode(graphics->GetSmoothingMode() );
		g.SetInterpolationMode(graphics->GetInterpolationMode() );

		GraphicsPath shadowPath(FillModeAlternate);
		StringFormat strFormat2(0, LANG_NEUTRAL);

		shadowPath.AddString(str,-1,fontFamily,fs,fontSize,rcShadow,strFormat);

		SolidBrush shadowColor( shadowProperties.GetColor() );
		g.FillPath(&shadowColor,&shadowPath);

		Rect rcBounds;
		shadowPath.GetBounds(&rcBounds);

		Bitmap shadow(rcBounds.Width+10,rcBounds.Height+10);
		Graphics g2(&shadow);
		g2.SetSmoothingMode(graphics->GetSmoothingMode() );
		g2.SetInterpolationMode(graphics->GetInterpolationMode() );
		RectF rDraw(5.0f,5.0f,(REAL)rcBounds.Width,(REAL)rcBounds.Height);
		g2.DrawImage(&tmp,rDraw,(REAL)rcBounds.X,(REAL)rcBounds.Y,(REAL)rcBounds.Width,(REAL)rcBounds.Height,UnitPixel,NULL);

		boost::shared_ptr<Bitmap> blurred ( GetBlur(&shadow,blurRatio) );
		if(blurred.get() )
		{
			Bitmap recolouredBmp(blurred->GetWidth(),blurred->GetHeight() );
			Graphics g3(&recolouredBmp);
		
			ImageAttributes ia;	
			ColorMatrix cm = ColorMatrixHelper::GetIdentityMatrix();
			cm.m[4][0] = static_cast<float>GetRValue(shadowProperties.GetColor().ToCOLORREF())	/ 255.0f;
			cm.m[4][1] = static_cast<float>GetGValue(shadowProperties.GetColor().ToCOLORREF())	/ 255.0f;
			cm.m[4][2] = static_cast<float>GetBValue(shadowProperties.GetColor().ToCOLORREF())	/ 255.0f;
			cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = 1.0;
			//cm.m[3][3] = alpha * ((float)shadowProperties.Get/255.0f);
			ia.SetColorMatrix(&cm);
	
			g3.DrawImage(blurred.get(),Rect(0,0,blurred->GetWidth(),blurred->GetHeight() ),0,0,blurred->GetWidth(),blurred->GetHeight(),UnitPixel,&ia,0,0);
		
			UINT intensity = shadowProperties.GetIntensity();
			int intensityMajor = intensity / 10;
			int intensityMinor = intensity % 10;
			for(int i = 0; i < intensityMajor; ++i)
			{
				graphics->DrawImage(&recolouredBmp,rc.X + rcBounds.X -5+ shadowProperties.GetXOffset() /*offset*/ , rc.Y + rcBounds.Y - 5 + shadowProperties.GetYOffset() /*offset*/, blurred->GetWidth() + shadowProperties.GetExtraWidth(), blurred-> GetHeight() + shadowProperties.GetExtraHeight() );
			}

			if(intensityMinor > 0 )
			{
				ColorMatrix matrix = ColorMatrixHelper::GetIdentityMatrix();
				matrix.m[3][3] = (REAL) static_cast<float>(intensityMinor) / 10.0f;
				ImageAttributes attr;
				attr.SetColorMatrix(&matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
	
				graphics->DrawImage(&recolouredBmp,Rect(rc.X + rcBounds.X -5+ shadowProperties.GetXOffset() /*offset*/ ,rc.Y + rcBounds.Y - 5 + shadowProperties.GetYOffset() /*offset*/,blurred->GetWidth() + shadowProperties.GetExtraWidth(), blurred-> GetHeight() + shadowProperties.GetExtraHeight()),0,0,blurred->GetWidth(),blurred->GetHeight(),UnitPixel,&attr,0,0);
	
			}
			//	graphics->DrawImage(blurred,rc.X + rcBounds.X -5+ shadowProperties.GetXOffset() /*offset*/ , rc.Y + rcBounds.Y - 5 + shadowProperties.GetYOffset() /*offset*/, blurred->GetWidth() + shadowProperties.GetExtraWidth(), blurred-> GetHeight() + shadowProperties.GetExtraHeight() );
			//graphics->DrawImage(blurred,rc.X + rcBounds.X -5+ shadowProperties.GetXOffset() /*offset*/ , rc.Y + rcBounds.Y - 5 + shadowProperties.GetYOffset() /*offset*/, blurred->GetWidth() + shadowProperties.GetExtraWidth(), blurred-> GetHeight() + shadowProperties.GetExtraHeight() );
		}
	}
	
	graphics->FillPath(brush,&path);
}