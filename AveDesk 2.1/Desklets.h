// Desklets.h : Declaration of the CDesklets

#ifndef __DESKLETS_H_
#define __DESKLETS_H_

#include "resource.h"       // main symbols
#include "avedesk.h"

/////////////////////////////////////////////////////////////////////////////
// CDesklets
class ATL_NO_VTABLE CDesklets : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDesklets, &CLSID_Desklets>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CDesklets>,
	public IDispatchImpl<IDesklets, &IID_IDesklets, &LIBID_AveDeskLib>
{
public:
	CDesklets()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DESKLETS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDesklets)
	COM_INTERFACE_ENTRY(IDesklets)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CDesklets)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDesklets
public:
	STDMETHOD(AddWithSkin)(/*[in]*/IDeskletInfo *What, /*[in]*/IDeskletSkin *Skin, /*[optional]*/VARIANT *Left, /*[optional]*/VARIANT *Top, /*[optional]*/VARIANT *Width, /*[optional]*/VARIANT *Height, /*[out,retval]*/ IDesklet**pVal);
	STDMETHOD(Add)(IDeskletInfo *What, VARIANT *Left, VARIANT *Top, VARIANT *Width, VARIANT *Height, IDesklet **pVal);
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT* Index, /*[out, retval]*/ IDesklet* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif //__DESKLETS_H_
