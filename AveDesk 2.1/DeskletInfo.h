// DeskletInfo.h : Deklaration von CDeskletInfo

#ifndef __DESKLETINFO_H_
#define __DESKLETINFO_H_

#include "resource.h"       // Hauptsymbole
#include "helper_funcs.h"

//class DockletDLL;

#include "avedesk.h"
#include "DeskletSkins.h"

/////////////////////////////////////////////////////////////////////////////
// CDeskletInfo
class ATL_NO_VTABLE CDeskletInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDeskletInfo, &CLSID_DeskletInfo>,
	public ISupportErrorInfo,
	public IDispatchImpl<IDeskletInfo, &IID_IDeskletInfo, &LIBID_AveDeskLib>,
  public CoElementOwnerImpl<DockletDLL>
{
protected:
	CComObject<CDeskletSkins> *m_Skins;

public:
	CDeskletInfo() : m_Skins(NULL)
	{
	}
	~CDeskletInfo();

DECLARE_REGISTRY_RESOURCEID(IDR_DESKLETINFO)
DECLARE_NOT_AGGREGATABLE(CDeskletInfo)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDeskletInfo)
	COM_INTERFACE_ENTRY(IDeskletInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDeskletInfo
public:
	STDMETHOD(get_Skins)(/*[out, retval]*/ IDeskletSkins **pVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ int *pVal);
	STDMETHOD(get_FileName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Comments)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Author)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
};

#endif //__DESKLETINFO_H_
