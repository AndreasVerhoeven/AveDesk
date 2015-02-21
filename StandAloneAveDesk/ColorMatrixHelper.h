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

#ifndef COLOR_MATRIX_HELPER_H_2302202
#define COLOR_MATRIX_HELPER_H_2302202

#include <windows.h>
#include <gdiplus.h>

class ColorMatrixHelper
{
	// c-tor and d-tor are private and abstract so this class can never be instantiated
	// and only the static helper functions can be used
	private:
		ColorMatrixHelper(){};
		virtual ~ColorMatrixHelper() = 0;

		const static ColorMatrix hueBeforeMultiplication;
		const static ColorMatrix hueMultiplicationValue;

	public:

		// this function rotates a colour
		 static ColorMatrix RotateColor(ColorMatrix t, REAL phi, int x, int y);

		// this function returns an identity color-matrix
		static ColorMatrix GetIdentityMatrix();

		// this function will return the multiplication of 2 color-matrices
		static ColorMatrix Multiply(const ColorMatrix& m1, const ColorMatrix& m2);

		// this function will return a color-matrix where a saturation has been applied to the old matrix
		static ColorMatrix SetSaturation(const ColorMatrix& matrix, REAL saturation);

		// this function will return a color-matrix where a hue rotation has been applied to the old matrix
		static ColorMatrix ColorMatrixHelper::SetHue(ColorMatrix& matrix, REAL val);
};

#endif//COLOR_MATRIX_HELPER_H_2302202