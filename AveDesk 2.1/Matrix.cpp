// Matrix.cpp : Implementierung von CMatrix
#include "stdafx.h"
#include "AveDesk.h"
#include "Matrix.h"

/////////////////////////////////////////////////////////////////////////////
// CMatrix

STDMETHODIMP CMatrix::get_Value(int x, int y, float *pVal)
{
  if( x < 0 || x > 1 || x < 0 || x > 2 )
    return E_INVALIDARG;
  
  float tmp[2][3] = {0};
  m.GetElements(&tmp[0][0]);
  *pVal = tmp[x][y];
	return S_OK;
}

STDMETHODIMP CMatrix::put_Value(int x, int y, float newVal)
{
  if( x < 0 || x > 1 || x < 0 || x > 2 )
    return E_INVALIDARG;
  
  float tmp[2][3] = {0};
  m.GetElements(&tmp[0][0]);
  tmp[x][y] = newVal;
  m.SetElements(
    tmp[0][0], tmp[0][1], 
    tmp[1][0], tmp[1][1], 
    tmp[2][0], tmp[2][1]);
	return S_OK;
}

void CMatrix::CopyFrom(Matrix *newMatrix)
{
  float tmp[6] = {0};
  newMatrix->GetElements(tmp);
  m.SetElements(
    tmp[0], tmp[1], 
    tmp[2], tmp[3], 
    tmp[4], tmp[5]);
}

void CMatrix::CopyTo(Matrix *targetMatrix)
{
  float tmp[6] = {0};
  m.GetElements(tmp);
  targetMatrix->SetElements(
    tmp[0], tmp[1], 
    tmp[2], tmp[3], 
    tmp[4], tmp[5]);
}

STDMETHODIMP CMatrix::Rotate(float Angle, VARIANT_BOOL Prepend)
{
  m.Rotate(Angle, (Prepend)?MatrixOrderPrepend:MatrixOrderAppend);
	return S_OK;
}

STDMETHODIMP CMatrix::Reset()
{
	m.Reset();
	return S_OK;
}

STDMETHODIMP CMatrix::Translate(float offsetX, float offsetY, VARIANT_BOOL Prepend)
{
  m.Translate(offsetX, offsetY, (Prepend)?MatrixOrderPrepend:MatrixOrderAppend);
	return S_OK;
}

STDMETHODIMP CMatrix::RotateAt(float Angle, float centerX, float centerY, VARIANT_BOOL Prepend)
{
	m.RotateAt(Angle, PointF(centerX, centerY), (Prepend)?MatrixOrderPrepend:MatrixOrderAppend);
	return S_OK;
}
