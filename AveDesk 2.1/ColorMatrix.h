// ColorMatrix.h : Deklaration von CColorMatrix

#ifndef __COLORMATRIX_H_
#define __COLORMATRIX_H_

#include "resource.h"       // Hauptsymbole
#include "colormatrixhelper.h"

/////////////////////////////////////////////////////////////////////////////
// CColorMatrix
class ATL_NO_VTABLE CColorMatrix : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IColorMatrix, &IID_IColorMatrix, &LIBID_AveDeskLib>
{
public:
  CColorMatrix()
	{
    Reset();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COLORMATRIX)
DECLARE_NOT_AGGREGATABLE(CColorMatrix)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CColorMatrix)
	COM_INTERFACE_ENTRY(IColorMatrix)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Variables
public:
  ColorMatrix colorMatrix;
private:
  float hue, sat;

// IColorMatrix
public:
	ColorMatrix Get();
	void Set(ColorMatrix newVal);
	STDMETHOD(get_Value)(/*[in]*/ int x, /*[in]*/ int y, /*[out, retval]*/ float *pVal);
	STDMETHOD(put_Value)(/*[in]*/ int x, /*[in]*/ int y, /*[in]*/ float newVal);
	STDMETHOD(get_Saturation)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Saturation)(/*[in]*/ float newVal);
	STDMETHOD(get_Hue)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_Hue)(/*[in]*/ float newVal);
	STDMETHOD(Reset)();

};

#endif //__COLORMATRIX_H_
