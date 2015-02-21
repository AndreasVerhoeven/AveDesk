#ifndef DROPTARGETIMPL_HPP_2302939426539436939393
#define DROPTARGETIMPL_HPP_2302939426539436939393


#include <windows.h>
#include <oleidl.h>
#include <shlobj.h>
#include <prsht.h>
#include <atlcom.h>

template<typename T> class ATL_NO_VTABLE IDropTargetImpl : 
	public IDropTarget
{
protected:
	CComPtr<IDropTargetHelper>	m_dropTargetHelper;
	CComPtr<IDataObject>		m_dataObject;
	bool m_showDropSourceImage;

	HWND m_ThishWnd;
	
public:
	IDropTargetImpl(bool showDropShourceImage=true,HWND hWnd=NULL) :
		m_ThishWnd(hWnd),
		m_showDropSourceImage(showDropShourceImage)
	{
			CoCreateInstance(CLSID_DragDropHelper, 0, CLSCTX_INPROC_SERVER,
                     IID_IDropTargetHelper, (LPVOID*)&m_dropTargetHelper);
	}

	void SetWindow(HWND hWnd)
	{
		m_ThishWnd = hWnd;
	}

	HWND GetWindow()const
	{
		return m_ThishWnd;
	}

	void SetShowDropSourceImage(bool val)
	{
		m_showDropSourceImage = val;
	}

	bool GetShowDropSourceImage()const
	{
		return m_showDropSourceImage;
	}

	// IDropTarget
    STDMETHOD(DragEnter)(IDataObject* obj, DWORD dwKeys, POINTL ptl, DWORD* pEffect)
	{
		if(m_showDropSourceImage && m_dropTargetHelper)
		{
			POINT pt = { ptl.x, ptl.y };

			T* t = static_cast<T*>(this);
			m_dropTargetHelper->DragEnter(t->m_ThishWnd, obj, &pt, *pEffect);
			m_dataObject = obj;
		}
		return E_NOTIMPL;
	}

    STDMETHOD(DragOver)(DWORD dwKeys, POINTL ptl, DWORD * pEffect)
	{
		if(m_showDropSourceImage && m_dropTargetHelper)
		{
			POINT pt = { ptl.x, ptl.y };
			m_dropTargetHelper->DragOver(&pt, *pEffect);
		}

		return E_NOTIMPL;
	}

    STDMETHOD(DragLeave)()
	{
		if(m_showDropSourceImage && m_dropTargetHelper)
		{
			m_dropTargetHelper->DragLeave();
			m_dataObject.Release();
		}

		return E_NOTIMPL;
	}

    STDMETHOD(Drop)(IDataObject * obj, DWORD dwKeys, POINTL ptl, DWORD *pEffect)
	{
		if(m_showDropSourceImage && m_dropTargetHelper)
		{
			POINT pt = { ptl.x, ptl.y };
			m_dropTargetHelper->Drop(obj,&pt,*pEffect);
			m_dataObject.Release();
		}

		return E_NOTIMPL;
	}
};

#endif//DROPTARGETIMPL_HPP_2302939426539436939393