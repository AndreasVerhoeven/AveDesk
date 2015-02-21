// oLayers.h : Deklaration von CoLayers

#ifndef __OLAYERS_H_
#define __OLAYERS_H_

#include "resource.h"       // Hauptsymbole
class Docklet;

/////////////////////////////////////////////////////////////////////////////
// CoLayers
class ATL_NO_VTABLE CoLayers : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CoLayers, &CLSID_Layers>,
	public IDispatchImpl<ILayers, &IID_ILayers, &LIBID_AveDeskLib>
{
public:
	CoLayers()
	{
	}

//DECLARE_REGISTRY_RESOURCEID(IDR_OLAYERS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CoLayers)
	COM_INTERFACE_ENTRY(ILayers)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ILayers
public:
	STDMETHOD(get_Item)(/*[in]*/ VARIANT*Index, /*[out, retval]*/ ILayer* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ int *pVal);
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
  Docklet *docklet;

};

#endif //__OLAYERS_H_
