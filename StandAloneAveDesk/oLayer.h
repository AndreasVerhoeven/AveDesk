// oLayer.h : Deklaration von CoLayer

#ifndef __OLAYER_H_
#define __OLAYER_H_

#include "resource.h"       // Hauptsymbole
#include "helper_funcs.h"

class Layer;
/////////////////////////////////////////////////////////////////////////////
// CoLayer
class ATL_NO_VTABLE CoLayer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CoLayer, &CLSID_oLayer>,
	public IDispatchImpl<ILayer, &IID_ILayer, &LIBID_AveDeskLib>,
  public CoElementOwnerImpl<Layer>
{
public:
	CoLayer()
	{
	}

//DECLARE_REGISTRY_RESOURCEID(IDR_OLAYER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CoLayer)
	COM_INTERFACE_ENTRY(ILayer)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IoLayer
public:
	STDMETHOD(get_CanBeIcon)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_CanBeIcon)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_FileName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
};

#endif //__OLAYER_H_
