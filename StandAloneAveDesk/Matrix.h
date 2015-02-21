// Matrix.h : Deklaration von CMatrix

#ifndef __MATRIX_H_
#define __MATRIX_H_

#include "resource.h"       // Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CMatrix
class ATL_NO_VTABLE CMatrix : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IMatrix, &IID_IMatrix, &LIBID_AveDeskLib>
{
public:
  CMatrix()
	{
    m.Reset();
	}

//DECLARE_REGISTRY_RESOURCEID(IDR_MATRIX)
DECLARE_NOT_AGGREGATABLE(CMatrix)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMatrix)
	COM_INTERFACE_ENTRY(IMatrix)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IMatrix
public:
	STDMETHOD(RotateAt)(/*[in]*/float Angle, /*[in]*/float centerX, /*[in]*/float centerY, /*[in, defaultvalue(-1)]*/VARIANT_BOOL Prepend);
	STDMETHOD(Translate)(/*[in]*/float offsetX, /*[in]*/float offsetY, /*[in, defaultvalue(-1)]*/VARIANT_BOOL Prepend);
	STDMETHOD(Reset)();
	STDMETHOD(Rotate)(/*[in]*/float Angle, /*[in]*/VARIANT_BOOL Prepend);
	void CopyTo(Matrix *targetMatrix);
	void CopyFrom(Matrix *newMatrix);
	STDMETHOD(get_Value)(/*[in]*/ int x, /*[in]*/ int y, /*[out, retval]*/ float *pVal);
	STDMETHOD(put_Value)(/*[in]*/ int x, /*[in]*/ int y, /*[in]*/ float newVal);

  Matrix m;
};

#endif //__MATRIX_H_
