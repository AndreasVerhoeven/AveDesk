//	Name:		Od Docklet
//	Author:		Andreas Verhoeven
//	Date:		dec 07 2003, 16:10
//	Version:	1
//	Revision:	2
//	Comments:	<none>

#include "stdafx.h"
#include <windows.h>
#include <winbase.h>

#include "od_docklet.hpp"
#include "od_docklet_instance.hpp"

#include "Application.h"
extern CComObject<CApplication>* _App;

namespace docklets
{
	OdDocklet::OdDocklet(HMODULE module) : module(module),
		OnAcceptDropFiles(0), OnConfigure(0), OnDestroy(0),
		OnDoubleClick(0), OnDropFiles(0), OnExportFiles(0),
		OnGetInformation(0), OnLeftButtonClick(0),
		OnLeftButtonHeld(0), OnProcessMessage(0),
		OnRightButtonClick(0), OnSave(0), OnGetModel(0)
		
	{
		// pre:  module is not NULL
		// post: an OdDocklet object is created with all functions-pointers set to
		//			NULL if module is NULL, otherwise all functions-pointers will be
		//			assigned by trying to load them from module
		if(module)
		{
			WCHAR modulePath[MAX_PATH] = {0};
			GetModuleFileName(module, modulePath, MAX_PATH);
			const WCHAR* moduleFileName = PathFindFileName(modulePath);
			if(!IsAdmin() && _wcsicmp(moduleFileName, L"sysstats.dll") == 0)
			{
				// issue sysstats warning

				HWND iconHwnd = NULL;
				if(_App != NULL)
					iconHwnd = _App->GetIconWindow().m_hWnd;
				NOTIFYICONDATAW _ncd = {0};
				_ncd.hWnd = iconHwnd;
				if(iconHwnd != NULL)
				{
					_ncd.cbSize = NOTIFYICONDATA_V2_SIZE;
					_ncd.hIcon = NULL;
					_ncd.uID = 99;
					_ncd.uFlags = NIF_INFO;
					_ncd.dwInfoFlags = NIIF_WARNING | 0x00000020; //0x00000020 = NIIF_LARGE_ICON;
					wcscpy_s(_ncd.szInfo, 256, L"When using SysStats, run AveDesk as Admin User (Rightclick AveDesk.exe and pick 'run-as-admin' or modify in avedesk.exe's properties).");
					wcscpy_s(_ncd.szInfoTitle, 64, L"SysStats Problem");

					Shell_NotifyIconW(NIM_MODIFY,&_ncd);

				}

			}

			OnProcessMessage	= (void (CALLBACK *)(DOCKLET_DATA*,HWND,UINT,WPARAM,LPARAM))	GetProcAddress(module,"OnProcessMessage");
			OnCreate			= (DOCKLET_DATA* (CALLBACK *)(HWND,HINSTANCE,const char*,const char*))		GetProcAddress(module,"OnCreate");
			OnDestroy			= (void (CALLBACK *)(DOCKLET_DATA*,HWND))						GetProcAddress(module,"OnDestroy");
			OnSave				= (void (CALLBACK *)(DOCKLET_DATA*,const char*,const char *,BOOL))			GetProcAddress(module,"OnSave");
			OnLeftButtonClick	= (BOOL (CALLBACK *)(DOCKLET_DATA*,POINT*,SIZE*))				GetProcAddress(module,"OnLeftButtonClick");
			OnDoubleClick		= (BOOL (CALLBACK *)(DOCKLET_DATA*,POINT*,SIZE*))				GetProcAddress(module,"OnDoubleClick");
			OnLeftButtonHeld	= (BOOL (CALLBACK *)(DOCKLET_DATA*,POINT*,SIZE*))				GetProcAddress(module,"OnLeftButtonHeld");
			OnRightButtonClick	= (BOOL (CALLBACK *)(DOCKLET_DATA*,POINT*,SIZE*))				GetProcAddress(module,"OnRightButtonClick");
			OnConfigure			= (void (CALLBACK *)(DOCKLET_DATA*))							GetProcAddress(module,"OnConfigure");
			OnAcceptDropFiles	= (BOOL (CALLBACK *)(DOCKLET_DATA*))							GetProcAddress(module,"OnAcceptDropFiles");
			OnDropFiles			= (void (CALLBACK *)(DOCKLET_DATA*,HDROP))						GetProcAddress(module,"OnDropFiles");
			OnGetInformation	= (void (CALLBACK *)(char*,char*,int*,char*))					GetProcAddress(module,"OnGetInformation");
			OnExportFiles		= (BOOL (CALLBACK *)(DOCKLET_DATA*,char*,int))					GetProcAddress(module,"OnExportFiles");
			OnMouseOn			= (BOOL (CALLBACK *)(DOCKLET_DATA*))							GetProcAddress(module,"OnMouseOn");
			OnMouseOut			= (BOOL (CALLBACK *)(DOCKLET_DATA*))							GetProcAddress(module,"OnMouseOut");
			OnMouseMove			= (BOOL (CALLBACK *)(DOCKLET_DATA*,int,int))					GetProcAddress(module,"OnMouseMove");
			OnMouseWheelForward	= (BOOL (CALLBACK *)(DOCKLET_DATA*,WORD,int,int,int))			GetProcAddress(module,"OnMouseWheelForward");  
			OnMouseWheelBackward= (BOOL (CALLBACK *)(DOCKLET_DATA*,WORD,int,int,int))			GetProcAddress(module,"OnMouseWheelBackward");

			OnDragEnter			= (BOOL (CALLBACK *)(DOCKLET_DATA*,IDataObject*,DWORD,POINTL,DWORD*))	GetProcAddress(module,"OnDragEnter");			
			OnDragOver			= (BOOL (CALLBACK *)(DOCKLET_DATA*,DWORD,POINTL,DWORD*))				GetProcAddress(module,"OnDragOver");
			OnDragLeave			= (BOOL (CALLBACK *)(DOCKLET_DATA*))									GetProcAddress(module,"OnDragLeave");
			OnDrop				= (BOOL (CALLBACK *)(DOCKLET_DATA*,IDataObject*,DWORD,POINTL,DWORD*))	GetProcAddress(module,"OnDrop");
			OnDragDropDataRequest=(BOOL (CALLBACK *)(DOCKLET_DATA*,FORMATETC*,STGMEDIUM*))				GetProcAddress(module,"OnDragDropDataRequest");
			OnSelect			= (BOOL (CALLBACK *)(DOCKLET_DATA*)	)									GetProcAddress(module,"OnSelect");
			OnDeselect			= (BOOL (CALLBACK *)(DOCKLET_DATA*)	)									GetProcAddress(module,"OnDeselect");

			OnAddPropertySheets	= (INT (CALLBACK *)(DOCKLET_DATA*,int))							GetProcAddress(module,"OnAddPropertySheets");
			OnGetModel          = (BOOL (CALLBACK *)(DOCKLET_DATA*, IDispatch **))              GetProcAddress(module, "OnGetModel");
			OnProcessMessageEx	= (LRESULT (CALLBACK *)(DOCKLET_DATA*,HWND,UINT,WPARAM,LPARAM,BOOL*)) GetProcAddress(module, "OnProcessMessageEx");
			OnVisibilityNotify	= (BOOL (CALLBACK *)(DOCKLET_DATA*,BOOL,DWORD))					GetProcAddress(module,"OnVisibilityNotify");

			OnCreateSkinEnumeratorW = ( ENUMERATOR_DATA* (CALLBACK *)(HINSTANCE,const WCHAR*) )						GetProcAddress(module,"OnCreateSkinEnumeratorW");
			OnDestroySkinEnumeratorW= ( void (CALLBACK* )(ENUMERATOR_DATA*) )										GetProcAddress(module,"OnDestroySkinEnumeratorW");
			OnGetSkinW				= ( INT (CALLBACK* )(ENUMERATOR_DATA*,SkinInfo*) )								GetProcAddress(module,"OnGetSkinW");
			OnCreateFromSkinW		= (DOCKLET_DATA* (CALLBACK*)(HWND,HINSTANCE,ENUMERATOR_DATA*,const SkinInfo*))	GetProcAddress(module,"OnCreateFromSkinW");
			OnSetSkinW				= ( BOOL (CALLBACK* )(ENUMERATOR_DATA*, const SkinInfo*))						GetProcAddress(module,"OnSetSkinW");

			OnLayerMouseDown        = (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD))	GetProcAddress(module,"OnLayerMouseDown");
			OnLayerMouseUp          = (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD))	GetProcAddress(module,"OnLayerMouseUp");
			OnLayerClick            = (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD))	GetProcAddress(module,"OnLayerClick");
			OnLayerRightClick       = (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD))	GetProcAddress(module,"OnLayerRightClick");
			OnLayerMouseOn			= (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*))			GetProcAddress(module,"OnLayerMouseOn");
			OnLayerMouseMove		= (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*))			GetProcAddress(module,"OnLayerMouseMove");
			OnLayerMouseOut			= (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*))										GetProcAddress(module,"OnLayerMouseOut");	
			OnLayerMouseWheelForward= (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD, int))	GetProcAddress(module,"OnLayerMouseWheelForward");
			OnLayerMouseWheelBackward=(BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD, int))	GetProcAddress(module,"OnLayerMouseWheelBackward");
			OnBeforeRemoval         = (BOOL (CALLBACK *)(DOCKLET_DATA*)) GetProcAddress(module, "OnBeforeRemoval");

			OnGetFlags				= (BOOL (CALLBACK *)(DOCKLET_DATA*, DWORD*)) GetProcAddress(module, "OnGetFlags");
			OnGetSkinPreviewBitmapCacheEntry = (UINT (CALLBACK*)(ENUMERATOR_DATA*, const SkinInfo*, BitmapCacheEntry*)) GetProcAddress(module, "OnGetSkinPreviewBitmapCacheEntry");
			OnGetSkinPreviewBitmap	=	(BOOL (CALLBACK *)(ENUMERATOR_DATA*, const SkinInfo*,PreviewBitmapRequestData*)) GetProcAddress(module, "OnGetSkinPreviewBitmap");
			OnStopSkinPreviewBitmap = (BOOL (CALLBACK *)(ENUMERATOR_DATA*, PreviewBitmapRequestData*)) GetProcAddress(module, "OnStopSkinPreviewBitmap");

			OnLayerGetToolTip = (BOOL (CALLBACK *)(DOCKLET_DATA*, UINT, const char*, const WCHAR*)) GetProcAddress(module, "OnLayerGetToolTip");

		}
	};

	bool OdDocklet::GetFlags(DOCKLET_DATA* data, DWORD* flags)
	{
		if(!OnGetFlags)
			return false;

		return OnGetFlags(data, flags) != FALSE;
	}

	UINT  OdDocklet::GetSkinPreviewBitmapCacheEntry(ENUMERATOR_DATA* lpEnum, const SkinInfo* info, BitmapCacheEntry* entry)
	{
		if(!OnGetSkinPreviewBitmapCacheEntry)
			return AVEDESK_CACHE_NOT_SUPPORTED;

		return OnGetSkinPreviewBitmapCacheEntry(lpEnum, info, entry);
	}

	bool OdDocklet::GetSkinPreviewBitmap(ENUMERATOR_DATA* lpEnum, const SkinInfo* info,PreviewBitmapRequestData* data)
	{
		if(!OnGetSkinPreviewBitmap)
			return false;

		return OnGetSkinPreviewBitmap(lpEnum, info, data) != FALSE;
	}

	bool OdDocklet::StopSkinPreviewBitmap(ENUMERATOR_DATA* lpEnum, PreviewBitmapRequestData* data)
	{
		if(!OnStopSkinPreviewBitmap)
			return false;

		return OnStopSkinPreviewBitmap(lpEnum, data) != FALSE;
	}

	bool OdDocklet::BeforeRemoval(DOCKLET_DATA* lpData)
	{
		if(!OnBeforeRemoval)
			return false;

		return OnBeforeRemoval(lpData) != FALSE;
	}

	bool OdDocklet::LayerMouseWheelForward(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys, int numScrolls)
	{
		if(!OnLayerMouseWheelForward)
			return false;

		return OnLayerMouseWheelForward(data, id, name, pt, s, keys, numScrolls) != FALSE;
	}
	
	bool OdDocklet::LayerMouseWheelBackward(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys, int numScrolls)
	{
		if(!OnLayerMouseWheelBackward)
			return false;

		return OnLayerMouseWheelBackward(data, id, name, pt, s, keys, numScrolls) != FALSE;
	}


	bool OdDocklet::LayerMouseDown(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys)
	{
		if(!OnLayerMouseDown)
			return false;

		return OnLayerMouseDown(data, id, name, pt, s, keys) != FALSE;
	}

	bool OdDocklet::LayerMouseUp(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys)
	{
		if(!OnLayerMouseUp)
			return false;

		return OnLayerMouseUp(data, id, name, pt, s, keys) != FALSE;
	}

	bool OdDocklet::LayerMouseOn(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s)
	{
		if(!OnLayerMouseOn)
			return false;

		return OnLayerMouseOn(data, id, name, pt, s) != FALSE;
	}

	bool OdDocklet::LayerMouseMove(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s)
	{
		if(!OnLayerMouseMove)
			return false;

		return OnLayerMouseMove(data, id, name, pt, s) != FALSE;
	}

	bool OdDocklet::LayerMouseOut(DOCKLET_DATA* data, UINT id, const char* name)
	{
		if(!OnLayerMouseOut)
			return false;

		return OnLayerMouseOut(data, id, name) != FALSE;
	}

	bool OdDocklet::LayerClick(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys)
	{
		if(!OnLayerClick)
			return false;

		return OnLayerClick(data, id, name, pt, s, keys) != FALSE;
	}

	bool OdDocklet::LayerRightClick(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys)
	{
		if(!OnLayerRightClick)
			return false;

		return OnLayerRightClick(data, id, name, pt, s, keys) != FALSE;
	}

	OdDocklet* OdDocklet::LoadDockletFromDLL(HMODULE hMod)
	{
		// pre:  hMod is not NULL
		// post: if hMod is not NULL and hMod points to a valid OdDocklet-module,
		//			then a new Instance of OdDocklet is returned, otherwise NULL
		//			is returned

		// hMod is 0, so we can't load the dll, so return NULL immediately
		if(!hMod)return 0;

		// create a new OdDocklet which should try to load the module
		OdDocklet* docklet = new OdDocklet(hMod);

		// ave 9/06: - OnGetInformation must be present too.
		if(!(docklet->OnCreate && docklet->OnDestroy && docklet->OnGetInformation))
		{
			// this is not valid docklet delete it and set docklet to NULL
			delete docklet;
			docklet = 0;
		}

		return docklet;
	}

	OdDockletInstance* OdDocklet::CreateInstance(const char* iniFile, const char* iniGroup,
		WNDPROC wndProc,HINSTANCE hInstance)
	{
		// post: a new instance of this docklet has been returned if we were able to create
		//			a window for it, otherwise NULL has been returned

		// this is the OdDockletInstance that will be returned, it is NULL by default
		OdDockletInstance* dockletInstance=0;

		// create a new window for the docklet, use the module as the HINSTANCE when 
		// the hInstance argument is 0

		HWND hWnd = OdDockletInstance::CreateDockletWindow(wndProc,(hInstance)?hInstance:module);

		if(hWnd)
		{
			
			// ... and create a OdDockletInstance
			dockletInstance = new OdDockletInstance(this,0,hWnd);

			// the window is created, so call the Docklets OnCreate ...
			DOCKLET_DATA* pointer = OnCreate(hWnd,module,iniFile,iniGroup);
			dockletInstance->SetPointer(pointer);

			
		}
		
		return dockletInstance;
	}

	OdDockletInstance* OdDocklet::CreateNotActivatedInstance()
	{
		// post: a new instance of this docklet has been returned that is not activated yet
		//		 it should be activated with the Activate method of the odDockletInstance

		return new OdDockletInstance(this,0,0);
	}

	bool OdDocklet::SetSkinW(ENUMERATOR_DATA* ed, const SkinInfo* info)
	{
		if(!OnSetSkinW)
			return false;

		return OnSetSkinW(ed,info) == TRUE;
	}

	bool OdDocklet::SupportsSkinEnumerator()const
	{
		return OnCreateSkinEnumeratorW != NULL;
	}

	ENUMERATOR_DATA* OdDocklet::CreateSkinEnumeratorW()
	{
		if(!OnCreateSkinEnumeratorW)
			return NULL;

		WCHAR wPath[MAX_PATH+2] = {0};
		GetDLLPathW(module,wPath,MAX_PATH);
		return OnCreateSkinEnumeratorW(module,wPath);
	}

	void OdDocklet::DestroySkinEnumeratorW(ENUMERATOR_DATA* ed)
	{
		if(!OnDestroySkinEnumeratorW)
			return;

		OnDestroySkinEnumeratorW(ed);
	}
	
	INT	OdDocklet::GetSkinW(ENUMERATOR_DATA* ed, SkinInfo* skinInfo)
	{
		const int NO_MORE_SKINS = FALSE;
		
		if(!OnGetSkinW)
			return NO_MORE_SKINS;

		return OnGetSkinW(ed,skinInfo);
	}

	DOCKLET_DATA* OdDocklet::CreateFromSkinW(HWND hwnd, ENUMERATOR_DATA* ed, const SkinInfo* skinInfo)
	{
		if(!OnCreateFromSkinW)
			return NULL;

		return OnCreateFromSkinW(hwnd,module,ed,skinInfo);
	}


	int  OdDocklet::AddPropertySheets(DOCKLET_DATA* lpData,int numPresent)
	{
		// post: the result from OndAddPropertySheets has been returned if the function was
		//			called, otherwise 0 has been returned
		if(OnAddPropertySheets)
		{
			return OnAddPropertySheets(lpData,numPresent);
		}
		else
		{
			return 0;
		}
	}

	void OdDocklet::Destroy(DOCKLET_DATA *lpData,HWND hwndDocklet)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDestroy)
		{
			OnDestroy(lpData,hwndDocklet);
		}
	}
	
	bool OdDocklet::LayerGetToolTip(DOCKLET_DATA* lpData, UINT layerId, const char* layerName, const WCHAR* currentEntry)
	{
		if(OnLayerGetToolTip != NULL)
		{
			return OnLayerGetToolTip(lpData, layerId, layerName, currentEntry) != FALSE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::DragDropDataRequest(DOCKLET_DATA* lpData,FORMATETC* format, STGMEDIUM* medium)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDragDropDataRequest)
		{
			return OnDragDropDataRequest(lpData,format,medium) == TRUE;
		}
		else
		{
			return false;
		}
	}
	bool OdDocklet::Select(DOCKLET_DATA* lpData)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnSelect)
		{
			return OnSelect(lpData) == TRUE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::Deselect(DOCKLET_DATA* lpData)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDeselect)
		{
			return OnDeselect(lpData) == TRUE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::ProcessMessage(DOCKLET_DATA *lpData,HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnProcessMessage)
		{
			OnProcessMessage(lpData,hwnd,uMsg,wParam,lParam);
			return true;
		}
		else
		{
			return false;
		}
	}

	LRESULT OdDocklet::ProcessMessageEx(DOCKLET_DATA *lpData,HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam, BOOL* isHandled)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnProcessMessageEx)
		{
			return OnProcessMessageEx(lpData,hwnd,uMsg,wParam,lParam,isHandled);
		}
		else
		{
			(*isHandled) = FALSE;
			return 0;
		}
	}

	bool OdDocklet::Save(DOCKLET_DATA *lpData,const char *szIni,const char *szIniGroup,BOOL bIsForExport)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnSave)
		{
			OnSave(lpData,szIni,szIniGroup,bIsForExport);
			return true;
		}
		else
		{
			return false;
		}
	}
	bool OdDocklet::LeftButtonClick(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnLeftButtonClick)
		{
			return OnLeftButtonClick(lpData,ptCursor,sizeDocklet)==TRUE;
		}
		else
		{
			return false;
		}
	}
	bool OdDocklet::DoubleClick(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDoubleClick)
		{
			return OnDoubleClick(lpData,ptCursor,sizeDocklet)==TRUE;
		}
		else
		{
			return false;
		}	
	}
	bool OdDocklet::LeftButtonHeld(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnLeftButtonHeld)
		{
			return OnLeftButtonHeld(lpData,ptCursor,sizeDocklet)==TRUE;
		}
		else
		{
			return false;
		}	
	}
	bool OdDocklet::RightButtonClick(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnRightButtonClick)
		{
			return OnRightButtonClick(lpData,ptCursor,sizeDocklet)==TRUE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::HasOnConfigure()
	{
		// post: true has been returned if the docklet has a custom onconfigure function,
		//       otherwise false
		return OnConfigure != NULL;

	}

	bool OdDocklet::Configure(DOCKLET_DATA *lpData)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnConfigure)
		{
			OnConfigure(lpData);
			return true;
		}
		else
		{
			return false;
		}
	}
	bool OdDocklet::AcceptDropFiles(DOCKLET_DATA *lpData)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnAcceptDropFiles)
		{
			return OnAcceptDropFiles(lpData)==TRUE;
		}
		else
		{
			return false;
		}
	}
	bool OdDocklet::DropFiles(DOCKLET_DATA *lpData,HDROP hDrop)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDropFiles)
		{
			OnDropFiles(lpData,hDrop);
			return true;
		}
		else
		{
			return false;
		}
	}
	bool OdDocklet::ExportFiles(DOCKLET_DATA *lpData,char *szFileRelativeOut,int iteration)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnExportFiles)
		{
			OnExportFiles(lpData,szFileRelativeOut,iteration);
			return true;
		}
		else
		{
			return false;
		}
	}

	void OdDocklet::GetInformation(char *szName,char *szAuthor,int *lpiVersion,char *szNotes)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnGetInformation)
		{
			OnGetInformation(szName,szAuthor,lpiVersion,szNotes);
		}
	}

	bool OdDocklet::MouseOn(DOCKLET_DATA* lpData)
	{
		// post: true has been returend if the function is handled, otherwise false
		if(OnMouseOn)
		{
			return OnMouseOn(lpData) == TRUE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::MouseOut(DOCKLET_DATA* lpData)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnMouseOut)
		{
			return OnMouseOut(lpData) == TRUE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::MouseMove(DOCKLET_DATA* lpData, int x, int y)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnMouseMove)
		{
			return OnMouseMove(lpData, x ,y) == TRUE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::MouseWheelForward(DOCKLET_DATA* lpData, WORD fKeysDown, int numOfScrolls, int x, int y)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnMouseWheelForward)
		{
			return OnMouseWheelForward(lpData,fKeysDown,numOfScrolls,x,y) != FALSE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::MouseWheelBackward(DOCKLET_DATA* lpData, WORD fKeysDown, int numOfScrolls, int x, int y)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnMouseWheelBackward)
		{
			return OnMouseWheelBackward(lpData,fKeysDown,numOfScrolls,x,y) != FALSE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::DragEnter(DOCKLET_DATA *lpData,IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDragEnter)
		{
			return OnDragEnter(lpData,obj,keys,pnt,effect) != FALSE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::DragOver(DOCKLET_DATA *lpData,DWORD keys,POINTL pnt,DWORD* effect)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDragOver)
		{
			return OnDragOver(lpData,keys,pnt,effect) != FALSE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::DragLeave(DOCKLET_DATA *lpData)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDragLeave)
		{
			return OnDragLeave(lpData) == TRUE;
		}
		else
		{
			return false;
		}
	}

	bool OdDocklet::Drop(DOCKLET_DATA *lpData,IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect)
	{
		// post: true has been returned if the function is handled, otherwise false
		if(OnDrop)
		{
			return OnDrop(lpData,obj,keys,pnt,effect) == TRUE;
		}
		else
		{
			return false;
		}
	}

    bool OdDocklet::GetModel(DOCKLET_DATA *lpData, IDispatch**pVal)
    {
		// post: true has been returned if the function is handled, otherwise false
        if( OnGetModel)
        {
          return (OnGetModel(lpData, pVal))?true:false;
        }
        else
        {
            return false;
        }
    }

	bool OdDocklet::VisibilityNotify(DOCKLET_DATA *lpData, BOOL becomesVisible, DWORD flags)
	{
		// post: true has been returned iff the function has been handled
		if(OnVisibilityNotify)
		{
			return (OnVisibilityNotify(lpData,becomesVisible,flags))?true:false;
		}
		else
		{
			return false;
		}
	}

};