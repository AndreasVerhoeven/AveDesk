// filename:		ColorMatrixHelper.h
// Description:		Definition of class ColorMatrixHelper
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "ColorMatrixHelper.h"
#include <gdiplus.h>
#include <cmath>

const ColorMatrix ColorMatrixHelper::hueBeforeMultiplication = 
{
	0.819152f, 0.0f, 0.534387f, 0.0f, 0.0f,
	-0.405580f, 0.707107f, 1.05527f, 0.0f, 0.0f,
	-0.405580f, -0.707107f, 0.141995f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

const ColorMatrix ColorMatrixHelper::hueMultiplicationValue =
{
	0.846593f, -0.377869f, -0.377869f, 0.0f, 0.0f,
	-0.370405f, 0.33052f, -1.808166f, 0.0f, 0.0f,
	0.573576f, 0.579228f, 0.579228f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

ColorMatrix ColorMatrixHelper::GetIdentityMatrix()
{
	// post: an indetity color-matrix has been returned

	ColorMatrix retMatrix;
	::ZeroMemory(retMatrix.m, 25 * sizeof(REAL));
	for (int i = 0; i < 5; i++)
		retMatrix.m[i][i] = 1.0f;	

	return retMatrix;
}

ColorMatrix ColorMatrixHelper::Multiply(const ColorMatrix& m1, const ColorMatrix& m2)
{
	// post: the result of the multiplication of m1 and m2 has been returned

	// create a matrix for return and already assign the values of m1 to it
	ColorMatrix retMatrix = m1;

	// perform a matrix-multiplication
	// NOTE: we don't need to calculate the last row, because a ColorMatrix does have a fixed last row
    for (int y = 0; y < 5 ; y++)
        for (int x = 0; x < 5; x++)
		{
			REAL val = 0;
			for (int i = 0; i < 5; i++)
			{
				val += ( m2.m[x][i] * m1.m[i][y] );
			}
            retMatrix.m[x][y] = val;
        }

	return retMatrix;
}

ColorMatrix ColorMatrixHelper::SetSaturation(const ColorMatrix& matrix, REAL saturation)
{
	// post: a color-matrix where a saturation has been applied to is returned

	// The luminance weight factors for the RGB color space.
	// These values are actually preferable to the better known factors of
	// Y = 0.30R + 0.59G + 0.11B, the formula which is used in color television technique.

	// These values are better than the actual values used with tv's. 
	// Thanks to Sjaak Priester in his Tinter-article
	const REAL luminanceR = 0.3086f;	// 0.30f
	const REAL luminanceG = 0.6094f;	// 0.59f
	const REAL luminanceB = 0.0820f;	// 0.11f

	REAL saturationComplement	= 1.0f - saturation;
	REAL saturationComplementR	= luminanceR * saturationComplement;
	REAL saturationComplementG	= luminanceG * saturationComplement;
	REAL saturationComplementB	= luminanceB * saturationComplement;

	ColorMatrix m =
	{
		saturationComplementR + saturation,	saturationComplementR,	saturationComplementR,	0.0f, 0.0f,
		saturationComplementG,	saturationComplementG + saturation,	saturationComplementG,	0.0f, 0.0f,
		saturationComplementB,	saturationComplementB,	saturationComplementB + saturation,	0.0f, 0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	0.0f,	1.0f
	};
	return Multiply(m,matrix);
}

const REAL pi = 3.14192f;
const REAL rad = pi / 180.0f;

ColorMatrix ColorMatrixHelper::RotateColor(ColorMatrix t, REAL phi, int x, int y)
{
	phi *= rad;
	ColorMatrix m = ColorMatrixHelper::GetIdentityMatrix();

	m.m[x][x] = m.m[y][y] = (REAL) cos(phi);

	REAL s = (REAL) sin(phi);
	m.m[y][x] = s;
	m.m[x][y] = - s;

	return ColorMatrixHelper::Multiply(m,t);
}

ColorMatrix ColorMatrixHelper::SetHue(ColorMatrix& matrix, REAL val)
{
	// post: a color matrix has been returned that where a hue of val has been applied to
	//		 matrix
	//return matrix;

	ColorMatrix retMatrix = Multiply(hueBeforeMultiplication,matrix);


	retMatrix = RotateColor(retMatrix,val,1,0);


	return Multiply(hueMultiplicationValue,retMatrix);
}
