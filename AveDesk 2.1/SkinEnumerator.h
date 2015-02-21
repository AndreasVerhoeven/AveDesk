// SkinEnumerator.h: Definition of the CSkinEnumerator class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKINENUMERATOR_H__33FF4A12_7DE3_4874_BEC7_FE40F55758B2__INCLUDED_)
#define AFX_SKINENUMERATOR_H__33FF4A12_7DE3_4874_BEC7_FE40F55758B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include "stdafx.h"

#include "avedesk.h"

/////////////////////////////////////////////////////////////////////////////
// CSkinEnumerator

class CSkinEnumerator : 
	public IDispatchImpl<ISkinEnumerator, &IID_ISkinEnumerator, &LIBID_AveDeskLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CSkinEnumerator,&CLSID_SkinEnumerator>
{
public:
	CSkinEnumerator() {}
BEGIN_COM_MAP(CSkinEnumerator)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISkinEnumerator)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CSkinEnumerator) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_SkinEnumerator)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ISkinEnumerator
public:
	STDMETHOD(Next)(BSTR *pVal);
};

#endif // !defined(AFX_SKINENUMERATOR_H__33FF4A12_7DE3_4874_BEC7_FE40F55758B2__INCLUDED_)
