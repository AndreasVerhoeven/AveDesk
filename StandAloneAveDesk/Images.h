// Images.h : Deklaration von CImages

#ifndef __IMAGES_H_
#define __IMAGES_H_

#include "resource.h"       // Hauptsymbole
#include "aveimage.h"
#include "helper_funcs.h"
#include <map>

typedef std::map<CComBSTR, CComObject<CImage>*, CComBSTRLessIgnCase> ImagesMap;

/////////////////////////////////////////////////////////////////////////////
// CImages
class ATL_NO_VTABLE CImages : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CImages, &CLSID_AveImages>,
	public ISupportErrorInfo,
	public IDispatchImpl<IAveImages, &IID_IAveImages, &LIBID_AveDeskLib>
{
public:
	CImages()
	{
	}
  ~CImages();

//DECLARE_REGISTRY_RESOURCEID(IDR_IMAGES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CImages)
	COM_INTERFACE_ENTRY(IAveImages)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// variables
ImagesMap imgs;
CComPtr<IEffectLet> parent;


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// non-COM
	BOOL IsKnownImage(
    LPCOLESTR pstrName, 
    DWORD dwReturnMask, 
    IUnknown * * ppiunkItem, 
    ITypeInfo * * ppti);
	void LoadXML(IXMLDOMNode *nod);

// IAveImages
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(get_Item)(VARIANT* Index, /*[out, retval]*/ IAveImage* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif //__IMAGES_H_
