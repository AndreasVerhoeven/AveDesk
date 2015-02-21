// PidlDropBox.h: interface for the CPidlDropBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIDLDROPBOX_H__3F916D45_858B_4973_AF91_899B595A1CCD__INCLUDED_)
#define AFX_PIDLDROPBOX_H__3F916D45_858B_4973_AF91_899B595A1CCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define WINVER		0x0500
//#define _WIN32_WINNT	0x0400
//#define _WIN32_IE	0x0501
//#define _RICHEDIT_VER	0x0100

#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <shobjidl.h>

#include "DropTargetImpl.hpp"

#define WM_PIDLDROPNOTIFY WM_USER + 43

class CPidlDropBox  :
	public IDropTargetImpl<CPidlDropBox>
{
private:
	ULONG m_RefCount;

	UINT m_ShellIdListClipFormat;

	HWND m_NotifyhWnd;
public:
	CPidlDropBox(HWND hwnd);
	~CPidlDropBox();

	// IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

	// IDropTargetImpl overrides
	virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject * o, DWORD k, POINTL p, DWORD *e) { return IDropTargetImpl<CPidlDropBox>::DragEnter(o,k,p,e);};
    virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD k, POINTL p, DWORD *e) { return IDropTargetImpl<CPidlDropBox>::DragOver(k,p,e); }
    virtual HRESULT STDMETHODCALLTYPE DragLeave(void) { return IDropTargetImpl<CPidlDropBox>::DragLeave(); };
    virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *, DWORD, POINTL, DWORD *);

};

#endif // !defined(AFX_PIDLDROPBOX_H__3F916D45_858B_4973_AF91_899B595A1CCD__INCLUDED_)
