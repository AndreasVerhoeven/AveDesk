// AveDeskletFile.h: Definition of the CAveDeskletFile class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEDESKLETFILE_H__34C97302_1144_40F0_8D9D_DD05AC7B7017__INCLUDED_)
#define AFX_AVEDESKLETFILE_H__34C97302_1144_40F0_8D9D_DD05AC7B7017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "avedesk.h"

/////////////////////////////////////////////////////////////////////////////
// CAveDeskletFile

class CAveDeskletFile : 
	public IDispatchImpl<IAveDeskletFile, &IID_IAveDeskletFile, &LIBID_AveDeskLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CAveDeskletFile,&CLSID_AveDeskletFile>,
	public IDropTarget
{
public:
	CAveDeskletFile() {}
BEGIN_COM_MAP(CAveDeskletFile)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IAveDeskletFile)
	COM_INTERFACE_ENTRY(IDropTarget)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CAveDeskletFile) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_AveDeskletFile)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAveDeskletFile
public:

	// IDropTarget
    STDMETHOD(DragEnter)(IDataObject* obj, DWORD dwKeys, POINTL ptl, DWORD* pEffect);
    STDMETHOD(DragOver)(DWORD dwKeys, POINTL ptl, DWORD * pEffect);
    STDMETHOD(DragLeave)();
    STDMETHOD(Drop)(IDataObject * obj, DWORD dwKeys, POINTL ptl, DWORD *pEffect);
};

#endif // !defined(AFX_AVEDESKLETFILE_H__34C97302_1144_40F0_8D9D_DD05AC7B7017__INCLUDED_)
