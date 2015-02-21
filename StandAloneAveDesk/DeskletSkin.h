// DeskletSkin.h: Definition of the CDeskletSkin class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETSKIN_H__3A708D63_7103_4A6C_AE43_D6150DF89139__INCLUDED_)
#define AFX_DESKLETSKIN_H__3A708D63_7103_4A6C_AE43_D6150DF89139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDeskletSkin
//!HERD:better place these in cpp files.
//#include "DeskletSkinEnumerator.h"
//#include "DeskletSkinInfo.h"
class DeskletSkinEnumerator;
class DeskletSkinInfo;

class CDeskletSkin : 
	public IDispatchImpl<IDeskletSkin, &IID_IDeskletSkin, &LIBID_AveDeskLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDeskletSkin,&CLSID_DeskletSkin>
{
protected:
	DeskletSkinEnumerator* m_Enumerator;
	DeskletSkinInfo* m_SkinInfo;
public:
	CDeskletSkin();
	virtual ~CDeskletSkin();

BEGIN_COM_MAP(CDeskletSkin)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IDeskletSkin)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CDeskletSkin) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

//DECLARE_REGISTRY_RESOURCEID(IDR_DeskletSkin)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDeskletSkin
public:
	STDMETHOD(get_Comments)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Author)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Number)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);


	void SetData(DeskletSkinEnumerator* enumerator, DeskletSkinInfo* info);
	DeskletSkinEnumerator* GetEnumerator();
	DeskletSkinInfo* GetSkinInfo();
};

#endif // !defined(AFX_DESKLETSKIN_H__3A708D63_7103_4A6C_AE43_D6150DF89139__INCLUDED_)
