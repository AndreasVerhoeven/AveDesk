// PidlDropBox.cpp: implementation of the CPidlDropBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PidlDropBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPidlDropBox::CPidlDropBox(HWND hwnd) :
	m_RefCount(0),
	m_NotifyhWnd(hwnd),
	IDropTargetImpl<CPidlDropBox>(true)
{
	m_ShellIdListClipFormat = RegisterClipboardFormat(CFSTR_SHELLIDLIST);
}

CPidlDropBox::~CPidlDropBox()
{

}

ULONG __stdcall CPidlDropBox::AddRef()
{
	return ++m_RefCount;
}

ULONG __stdcall CPidlDropBox::Release()
{
	if(--m_RefCount > 0)
		return m_RefCount;

	delete this;

	return 0;
}
HRESULT __stdcall CPidlDropBox::QueryInterface(REFIID refIdd, void** p)
{

	*p = NULL;


	if (::IsEqualIID(refIdd, IID_IUnknown))
	{
		*p = static_cast<IUnknown*>(this);
	}

	else if (IsEqualIID(refIdd, IID_IDropTarget))
	{
		*p = static_cast<IDropTarget *>(this);
	}
	else
	{
        return E_NOINTERFACE;
    }

    AddRef();

    return S_OK;
}

#define HIDA_GetPIDLFolder(pida) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[0])
#define HIDA_GetPIDLItem(pida, i) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[i+1])

HRESULT STDMETHODCALLTYPE CPidlDropBox::Drop(IDataObject * obj, DWORD keys, POINTL ptl, DWORD *effect)
{
	//IDropTargetImpl<CPidlDropBox>::Drop(obj,keys,ptl,effect);
	//IDropTargetImpl<CPidlDropBox>::Drop(obj,keys,ptl,effect);

	FORMATETC etc = {0};
	etc.cfFormat = (CLIPFORMAT)m_ShellIdListClipFormat;
	etc.dwAspect = DVASPECT_CONTENT;
	etc.lindex = -1;
	etc.tymed = TYMED_HGLOBAL;

	STGMEDIUM medium = {0};
	
	if( SUCCEEDED( obj->GetData(&etc,&medium)) )
	{		
		LPIDA pida = reinterpret_cast<LPIDA>( GlobalLock(medium.hGlobal) );

		if(pida)
		{
			LPCITEMIDLIST	pidlFolder= HIDA_GetPIDLFolder(pida);

			for(UINT i=0; i <pida->cidl; ++i)
			{
				if(pida->cidl > 0)
				{
					LPCITEMIDLIST pidl = HIDA_GetPIDLItem(pida,i);
					LPITEMIDLIST pidlFull = ILCombine(pidlFolder,pidl);
					
					LRESULT ret = SendMessage(m_NotifyhWnd,WM_PIDLDROPNOTIFY,pida->cidl,reinterpret_cast<LPARAM>(pidlFull) );
					
					CoTaskMemFree(reinterpret_cast<void*>(pidlFull));

					if(ret == FALSE)
						break;
					
				}
			}
		}

		GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);
	}

	return S_OK;
}
