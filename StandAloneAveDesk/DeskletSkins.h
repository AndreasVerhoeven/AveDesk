// DeskletSkins.h: Definition of the CDeskletSkins class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETSKINS_H__91BD02ED_4604_4813_BF7C_38A3880CBAE9__INCLUDED_)
#define AFX_DESKLETSKINS_H__91BD02ED_4604_4813_BF7C_38A3880CBAE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include "DeskletSkinEnumerator.h"
#include <vector>

#include "avedesk.h"

/////////////////////////////////////////////////////////////////////////////
// CDeskletSkins

class CDeskletSkins : 
	public IDispatchImpl<IDeskletSkins, &IID_IDeskletSkins, &LIBID_AveDeskLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDeskletSkins,&CLSID_DeskletSkins>
{
protected:
	DeskletSkinEnumerator* m_Enumerator;
	std::vector<DeskletSkinInfo*> m_Infos;
public:
	CDeskletSkins() : m_Enumerator(NULL) {}
	~CDeskletSkins();

BEGIN_COM_MAP(CDeskletSkins)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IDeskletSkins)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CDeskletSkins) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

//DECLARE_REGISTRY_RESOURCEID(IDR_DeskletSkins)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDeskletSkins
public:

	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(get_Item)(VARIANT* Index, /*[out, retval]*/ IDeskletSkin* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);

	void SetData(DeskletSkinEnumerator* enumerator);
};

#endif // !defined(AFX_DESKLETSKINS_H__91BD02ED_4604_4813_BF7C_38A3880CBAE9__INCLUDED_)
