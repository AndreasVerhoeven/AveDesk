// AveInstallFile.h: Definition of the CAveInstallFile class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTALLFILE_H__F41F5109_2CFA_4330_8735_2C2678D01EFF__INCLUDED_)
#define AFX_AVEINSTALLFILE_H__F41F5109_2CFA_4330_8735_2C2678D01EFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "avedesk.h"

/////////////////////////////////////////////////////////////////////////////
// CAveInstallFile

class CAveInstallFile : 
	public IDispatchImpl<IAveInstallFile, &IID_IAveInstallFile, &LIBID_AveDeskLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CAveInstallFile,&CLSID_AveInstallFile>,
	public IDropTarget
{
public:
	CAveInstallFile() {}
BEGIN_COM_MAP(CAveInstallFile)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IAveInstallFile)
	COM_INTERFACE_ENTRY(IDropTarget)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CAveInstallFile) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_AveInstallFile)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAveInstallFile
public:

	// IDropTarget
    STDMETHOD(DragEnter)(IDataObject* obj, DWORD dwKeys, POINTL ptl, DWORD* pEffect);
    STDMETHOD(DragOver)(DWORD dwKeys, POINTL ptl, DWORD * pEffect);
    STDMETHOD(DragLeave)();
    STDMETHOD(Drop)(IDataObject * obj, DWORD dwKeys, POINTL ptl, DWORD *pEffect);
};

#endif // !defined(AFX_AVEINSTALLFILE_H__F41F5109_2CFA_4330_8735_2C2678D01EFF__INCLUDED_)
