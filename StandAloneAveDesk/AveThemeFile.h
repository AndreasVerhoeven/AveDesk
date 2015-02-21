// AveThemeFile.h: Definition of the CAveThemeFile class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVETHEMEFILE_H__6A933C7E_EB09_4CDC_B2AE_3F636E085BB6__INCLUDED_)
#define AFX_AVETHEMEFILE_H__6A933C7E_EB09_4CDC_B2AE_3F636E085BB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "avedesk.h"

/////////////////////////////////////////////////////////////////////////////
// CAveThemeFile

class CAveThemeFile : 
	public IDispatchImpl<IAveThemeFile, &IID_IAveThemeFile, &LIBID_AveDeskLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CAveThemeFile,&CLSID_AveThemeFile>,
	public IDropTarget
{
public:
	CAveThemeFile() {}
BEGIN_COM_MAP(CAveThemeFile)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IAveThemeFile)
	COM_INTERFACE_ENTRY(IDropTarget)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CAveThemeFile) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

//DECLARE_REGISTRY_RESOURCEID(IDR_AveThemeFile)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAveThemeFile
public:

	// IDropTarget
    STDMETHOD(DragEnter)(IDataObject* obj, DWORD dwKeys, POINTL ptl, DWORD* pEffect);
    STDMETHOD(DragOver)(DWORD dwKeys, POINTL ptl, DWORD * pEffect);
    STDMETHOD(DragLeave)();
    STDMETHOD(Drop)(IDataObject * obj, DWORD dwKeys, POINTL ptl, DWORD *pEffect);
};

#endif // !defined(AFX_AVETHEMEFILE_H__6A933C7E_EB09_4CDC_B2AE_3F636E085BB6__INCLUDED_)
