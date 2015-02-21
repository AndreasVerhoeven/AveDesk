// DropSettingsImpl.h: interface for the CDropSettingsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPSETTINGSIMPL_H__06ECAF29_AADC_49EA_BEE6_00808E4F7DCA__INCLUDED_)
#define AFX_DROPSETTINGSIMPL_H__06ECAF29_AADC_49EA_BEE6_00808E4F7DCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "DropTargetImpl.hpp"
#include "DropSourceImpl.hpp"
#include "DataObjectImpl.hpp"
#include "IPersistable.h"
#include "XMLLoader.h"
#include "XMLSaver.h"

#define WM_DROPSETTINGS_NOTIFY WM_USER + 675

template<typename T> class CDropSettingsImpl :
	public IDataObjectImpl<CDropSettingsImpl<T> >,
	public IDropTargetImpl<CDropSettingsImpl<T> >,
	public IDropSourceImpl<CDropSettingsImpl<T> >
	//public CWindowImpl<CDropSettingsImpl>
{
protected:
	T&	m_Type;
	ULONG m_RefCount;
	bool m_CanAccept;
	
	UINT s_DropFormat;

	HWND m_hWnd;

	HWND m_NotifyHWnd;

	FORMATETC GetFORMATETC()
	{
		FORMATETC etc = {0};
		etc.cfFormat = (CLIPFORMAT)s_DropFormat;
		etc.dwAspect = DVASPECT_CONTENT;
		etc.lindex = -1;
		etc.tymed = TYMED_HGLOBAL;
		return etc;
	}

	FORMATETC GetCFTextFORMATETC()
	{
		FORMATETC etc = {0};
		etc.cfFormat = CF_UNICODETEXT;
		etc.dwAspect = DVASPECT_CONTENT;
		etc.lindex = -1;
		etc.tymed = TYMED_HGLOBAL;
		return etc;
	}

	void SetEffectToStatus(DWORD *effect)
	{
		if(effect)
		{
			if(!m_CanAccept)
				(*effect) = DROPEFFECT_NONE;
			else
				(*effect) = DROPEFFECT_COPY;
		}
	}	

public:

	CDropSettingsImpl(T& t) :
		m_Type(t), m_CanAccept(false), m_RefCount(0), m_hWnd(NULL)
	{
		s_DropFormat = RegisterClipboardFormat(_T("AveDeskDropSettings"));
		FORMATETC etc = GetFORMATETC();
		AddFormat(etc);
		FORMATETC etcT = GetCFTextFORMATETC();
		AddFormat(etcT );
	}

	
	void SetNotifyHWnd(HWND hWnd)
	{
		m_NotifyHWnd = hWnd;
	}

	HWND GetNotifyHWnd()
	{
		return m_NotifyHWnd;
	}

		
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)
	{
		if (IsEqualIID(riid, IID_IDataObject))
		{	
			*ppvObject = static_cast<IDataObject *>(this);
		}
		else if (IsEqualIID(riid, IID_IDropSource))
		{	
			*ppvObject = static_cast<IDropSource *>(this);
		}
		else if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget))
		{
			*ppvObject = static_cast<IDropTarget *>(this);
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
	
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		m_RefCount++;
		return m_RefCount;
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		m_RefCount--;
		if(m_RefCount > 0)
			return m_RefCount;

		delete this;

		return 0;
	}

	HRESULT DataRequest(FORMATETC *pFormatetc, STGMEDIUM *pmedium)
	{
		if(!pFormatetc)
			return E_UNEXPECTED;
	
		if(pFormatetc->cfFormat != s_DropFormat && pFormatetc->cfFormat != CF_UNICODETEXT)
			return E_UNEXPECTED;

		CXMLSaver xmlSaver(L"AveDesk");
		xmlSaver.SavePersistableClass(m_Type,m_Type._GetPersistancyName().c_str());

		IXMLDOMDocument* doc = xmlSaver.GetDOMDocument();
		CComBSTR bstrXML;
		doc->get_xml(&bstrXML);
		std::wstring wsXML;
		if(bstrXML)
			wsXML = static_cast<WCHAR*>(bstrXML);


		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,(wsXML.size()+2)*sizeof(WCHAR));
		WCHAR* wXML  = static_cast<WCHAR*>( GlobalLock(hMem));
		wcscpy(wXML,wsXML.c_str());
		GlobalUnlock(hMem);
		pmedium->hGlobal = hMem;
		pmedium->pUnkForRelease = 0;

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DragEnter(IDataObject * obj, DWORD keys, POINTL ptl, DWORD *effect)
	{
		IDropTargetImpl<CDropSettingsImpl>::DragEnter(obj,keys,ptl,effect);
	
		FORMATETC etc = GetFORMATETC();

		m_CanAccept = obj->QueryGetData(&etc) == S_OK;

		SetEffectToStatus(effect);

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DragOver(DWORD keys, POINTL ptl , DWORD *effect)
	{
		IDropTargetImpl<CDropSettingsImpl>::DragOver(keys,ptl,effect);
	
		SetEffectToStatus(effect);
	
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE Drop(IDataObject * obj, DWORD keys, POINTL ptl, DWORD *effect)
	{ 
		IDropTargetImpl<CDropSettingsImpl>::Drop(obj,keys,ptl,effect);
	
		SetEffectToStatus(effect);
	
		if(m_CanAccept)
		{
			FORMATETC etc = GetFORMATETC();
	
			STGMEDIUM medium = {0};
			obj->GetData(&etc,&medium);

			WCHAR* str = static_cast<WCHAR*>(GlobalLock(medium.hGlobal));
			if(str)
			{
				try
				{	
					CXMLLoader xmlLoader(str,L"AveDesk",true);
					xmlLoader.LoadPersistableClass(m_Type,m_Type._GetPersistancyName().c_str());
					if(m_NotifyHWnd)
						::PostMessage(m_NotifyHWnd,WM_DROPSETTINGS_NOTIFY,0L,0L);
				}
				catch(...)
				{
	
				}
			}
			GlobalUnlock(str);
			ReleaseStgMedium(&medium);
		}

		return S_OK;
	}

	LRESULT OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)//, BOOL& bHandled)
	{
		POINT p = {LOWORD(lParam),HIWORD(lParam)};
		::ClientToScreen(m_hWnd,&p);

		if(::DragDetect(m_hWnd,p))
		{
			DWORD dwEffect(0);
			BeginDrag(this,&dwEffect);
			//bHandled = TRUE;
		}
		else
		{
			WNDPROC oldProc = (WNDPROC) GetProp(m_hWnd,_T("_AveOldProc"));
			if(oldProc)
			{
				CallWindowProc(oldProc,m_hWnd,uMsg,wParam,lParam);
				return CallWindowProc(oldProc,m_hWnd,WM_LBUTTONUP,wParam,lParam);
			}

			//return 0;
		}

		return 1;
	}

	BOOL SubclassWindow(HWND hWnd)
	{
		//BOOL bRet = CWindowImpl<CDropSettingsImpl>::SubclassWindow(hWnd);

		m_hWnd = hWnd;

		WNDPROC curProc = (WNDPROC) GetWindowLong(m_hWnd,GWL_WNDPROC);
		SetProp(m_hWnd,_T("_AveOldProc"),(HANDLE)curProc);
		SetProp(m_hWnd,_T("_AveObj"),(HANDLE)this);
		SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)&ProcessMessage);

		//if(bRet)
			AddRef();

		LPDROPTARGET lpDt = {0};
		QueryInterface(IID_IDropTarget,(void**)&lpDt);

		HRESULT hr = ::RegisterDragDrop(hWnd,lpDt);
		Release();

		//return bRet;
		return TRUE;
	}

	BOOL UnsubclassWindow(HWND hWnd)
	{
		//BOOL bRet = CWindowImpl<CDropSettingsImpl>::UnsubclassWindow(hWnd);
		//if(bRet)

		WNDPROC oldProc = (WNDPROC) GetProp(hWnd,_T("_AveOldProc"));
		SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)oldProc);

			Release();

		::RevokeDragDrop(hWnd);

		//return bRet;
		return TRUE;
	}

	static LRESULT __stdcall ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(uMsg == WM_LBUTTONDOWN && GetKeyState(VK_SHIFT) < 0)
		{
			CDropSettingsImpl<T>* t = static_cast<CDropSettingsImpl<T>*>(GetProp(hWnd,_T("_AveObj")));
		
			if(t && t->OnLeftButtonDown(uMsg,wParam,lParam) == 1)
				return 1;
		}
		else if(uMsg == WM_DESTROY)
		{
			CDropSettingsImpl<T>* t = static_cast<CDropSettingsImpl<T>*>(GetProp(hWnd,_T("_AveObj")));
			if(t)
				t->UnsubclassWindow(hWnd);
		}

		WNDPROC oldProc = (WNDPROC) GetProp(hWnd,_T("_AveOldProc"));
		if(oldProc)
			return CallWindowProc(oldProc,hWnd,uMsg,wParam,lParam);
		else
			return 0;
	}

	BEGIN_MSG_MAP(CDropSettingsImpl)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown);
		//CHAIN_MSG_MAP_ALT (CWindowImpl<CDropSettingsImpl>,0);
		//CHAIN_MSG_MAP(CWindowImpl<CDropSettingsImpl>)
	END_MSG_MAP()

};

#endif // !defined(AFX_DROPSETTINGSIMPL_H__06ECAF29_AADC_49EA_BEE6_00808E4F7DCA__INCLUDED_)
