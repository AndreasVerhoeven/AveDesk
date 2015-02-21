// DeskletInfos.h : Deklaration von CDeskletInfos

#ifndef __DESKLETINFOS_H_
#define __DESKLETINFOS_H_

#include "resource.h"       // Hauptsymbole
#include "dockletdll.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CDeskletInfos
class ATL_NO_VTABLE CDeskletInfos : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDeskletInfos, &CLSID_DeskletInfos>,
	public ISupportErrorInfo,
	public IDispatchImpl<IDeskletInfos, &IID_IDeskletInfos, &LIBID_AveDeskLib>
{
public:
	CDeskletInfos()
	{
    Refresh();
	}
  ~CDeskletInfos()
  {
    Clear();
  }

//DECLARE_REGISTRY_RESOURCEID(IDR_DESKLETINFOS)
DECLARE_NOT_AGGREGATABLE(CDeskletInfos)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDeskletInfos)
	COM_INTERFACE_ENTRY(IDeskletInfos)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDeskletInfos
public:
	STDMETHOD(Clear)();
	STDMETHOD(Refresh)();
	STDMETHOD(get_Item)(/*[in]*/ VARIANT *Index, /*[out, retval]*/ IDeskletInfo* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);

private:
//  std::vector<DockletDLL*> docklets;
};

#endif //__DESKLETINFOS_H_
