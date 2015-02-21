// ColorMatrix.cpp : Implementierung von CColorMatrix
#include "stdafx.h"
#include "AveDesk.h"
#include "ColorMatrix.h"

/////////////////////////////////////////////////////////////////////////////
// CColorMatrix


STDMETHODIMP CColorMatrix::get_Value(int x, int y, float *pVal)
{
	if( x < 0 || x > 4 || y < 0 || y > 4 )
    return E_INVALIDARG;
  *pVal = colorMatrix.m[x][y];
	return S_OK;
}

STDMETHODIMP CColorMatrix::put_Value(int x, int y, float newVal)
{
	if( x < 0 || x > 4 || y < 0 || y > 4 )
    return E_INVALIDARG;
  colorMatrix.m[x][y] = newVal;  
	return S_OK;
}

void CColorMatrix::Set(ColorMatrix newVal)
{
  colorMatrix = newVal;
  
}

ColorMatrix CColorMatrix::Get()
{
  return colorMatrix;
}

STDMETHODIMP CColorMatrix::Reset()
{
  colorMatrix = ColorMatrixHelper::GetIdentityMatrix();
  hue = 1.0;
  sat = 1.0;
	return S_OK;
}

STDMETHODIMP CColorMatrix::get_Hue(float *pVal)
{
	*pVal = hue;

	return S_OK;
}

STDMETHODIMP CColorMatrix::put_Hue(float newVal)
{
	hue = newVal;
  ColorMatrixHelper::SetHue(colorMatrix, hue);

	return S_OK;
}

STDMETHODIMP CColorMatrix::get_Saturation(float *pVal)
{
	*pVal = sat;

	return S_OK;
}

STDMETHODIMP CColorMatrix::put_Saturation(float newVal)
{
	sat = newVal;
  ColorMatrixHelper::SetSaturation(colorMatrix, sat);

	return S_OK;
}
