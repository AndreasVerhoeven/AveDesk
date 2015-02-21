// filename:		draginterface.h
// Description:		Definition of class Docklet
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					This class is a wrapper class for the OdDockletInstance class
//					and handles additional settings only used by the GUI.
//					In some way, this class is a decorator.
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!


#ifndef AVE_DRAG_INTERFACE_H_303
#define AVE_DRAG_INTERFACE_H_303

#include <windows.h>
#include "docklet.h"



class AveDropTarget : IDropTarget
{
public:
    //static void CreateInstance(IDropTarget **);

private:

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

    virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *, DWORD, POINTL, DWORD *);
    virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD, POINTL, DWORD *);
    virtual HRESULT STDMETHODCALLTYPE DragLeave(void);
    virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *, DWORD, POINTL, DWORD *);

    AveDropTarget(Docklet* d);

    long    m_nRefCnt;
};

#endif//AVE_DRAG_INTERFACE_H_303

