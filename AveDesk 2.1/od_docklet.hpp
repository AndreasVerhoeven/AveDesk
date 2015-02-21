//	Name:		Od Docklet
//	Author:		Andreas Verhoeven
//	Date:		dec 07 2003, 18:50
//	Version:	1
//	Revision:	2
//	Comments:	<none>

#include <windows.h>

#ifndef INCLUDE_GUARD_203919929191929119191_OD_DOCKLET_HPP
#define INCLUDE_GUARD_203919929191929119191_OD_DOCKLET_HPP

#include "DeskletSkinInfo.h"
#include "DeskletBitmapCacheEntry.h"
#include "DeskletPreviewBitmapRequestData.h"

namespace docklets
{
	// some typedefs
	typedef void DOCKLET_DATA;
	typedef void ENUMERATOR_DATA;

	// forward declaration of odDockletInstance
	class OdDockletInstance;

	class OdDocklet
	{
		template<typename Ip> friend void Activate(OdDockletInstance* d,Ip* persister,HWND hwnd, HINSTANCE hInstance);

		private:

			// odDockletInstance would like to use the OnCreate function
			// and it's very important that others don't call this, because
			// the docklet otherwise will create a new instance for it that
			// has no matching odDockletInstance
			friend OdDockletInstance;

			HMODULE module;			// module from the docklets DLL

			// private constructor
			OdDocklet(HMODULE mod);

			// function pointers to actual functions in the DLL
			void (CALLBACK *OnProcessMessage)(DOCKLET_DATA *lpData,HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
			DOCKLET_DATA *(CALLBACK* OnCreate)(HWND hwndDocklet,HINSTANCE hInstance,const char *szIni,const char *szIniGroup);
			void (CALLBACK *OnSave)(DOCKLET_DATA *lpData,const char *szIni,const char *szIniGroup,BOOL bIsForExport);
			void (CALLBACK *OnDestroy)(DOCKLET_DATA *lpData,HWND hwndDocklet);
			BOOL (CALLBACK *OnLeftButtonClick)(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			BOOL (CALLBACK *OnDoubleClick)(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			BOOL (CALLBACK *OnLeftButtonHeld)(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			BOOL (CALLBACK *OnRightButtonClick)(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			void (CALLBACK *OnConfigure)(DOCKLET_DATA *lpData);
			BOOL (CALLBACK *OnAcceptDropFiles)(DOCKLET_DATA *lpData);
			void (CALLBACK *OnDropFiles)(DOCKLET_DATA *lpData,HDROP hDrop);
			void (CALLBACK *OnGetInformation)(char *szName,char *szAuthor,int *lpiVersion,char *szNotes);
			BOOL (CALLBACK *OnExportFiles)(DOCKLET_DATA *lpData,char *szFileRelativeOut,int iteration);

			// v1.1 only
			BOOL (CALLBACK *OnMouseOn)(DOCKLET_DATA* lpData);
			BOOL (CALLBACK *OnMouseOut)(DOCKLET_DATA* lpData);
			BOOL (CALLBACK *OnMouseMove)(DOCKLET_DATA* lpData, int x, int y);
			BOOL (CALLBACK *OnMouseWheelForward)(DOCKLET_DATA*,WORD,int,int,int);  
			BOOL (CALLBACK *OnMouseWheelBackward)(DOCKLET_DATA*,WORD,int,int,int);
			BOOL (CALLBACK *OnDragEnter)(DOCKLET_DATA*,IDataObject*,DWORD,POINTL,DWORD*);
			BOOL (CALLBACK *OnDragOver)(DOCKLET_DATA*,DWORD,POINTL,DWORD*);
			BOOL (CALLBACK *OnDragLeave)(DOCKLET_DATA*);
			BOOL (CALLBACK *OnDrop)(DOCKLET_DATA*,IDataObject*,DWORD,POINTL,DWORD*);
			BOOL (CALLBACK *OnDragDropDataRequest)(DOCKLET_DATA*,FORMATETC*,STGMEDIUM*);

			BOOL (CALLBACK *OnSelect)(DOCKLET_DATA*);
			BOOL (CALLBACK *OnDeselect)(DOCKLET_DATA*);

			int (CALLBACK *OnAddPropertySheets)(DOCKLET_DATA*,int);

			BOOL (CALLBACK *OnGetModel)(DOCKLET_DATA* lpData, IDispatch **pVal);
			LRESULT (CALLBACK *OnProcessMessageEx)(DOCKLET_DATA* lpData, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL* isHandled);
			BOOL (CALLBACK *OnVisibilityNotify)(DOCKLET_DATA *lpData, BOOL becomesVisible, DWORD flags);


			ENUMERATOR_DATA* (CALLBACK *OnCreateSkinEnumeratorW)(HINSTANCE,const WCHAR*);
			void (CALLBACK* OnDestroySkinEnumeratorW)(ENUMERATOR_DATA*);
			INT (CALLBACK* OnGetSkinW)(ENUMERATOR_DATA*, SkinInfo*);
			DOCKLET_DATA* (CALLBACK* OnCreateFromSkinW)(HWND,HINSTANCE,ENUMERATOR_DATA*,const SkinInfo* info);
			BOOL (CALLBACK* OnSetSkinW)(ENUMERATOR_DATA*, const SkinInfo*);

			BOOL (CALLBACK *OnLayerMouseDown)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD);
			BOOL (CALLBACK *OnLayerMouseUp)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD);
			BOOL (CALLBACK *OnLayerClick)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD);
			BOOL (CALLBACK *OnLayerRightClick)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD);
			BOOL (CALLBACK *OnLayerMouseOn)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*);
			BOOL (CALLBACK *OnLayerMouseMove)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*);
			BOOL (CALLBACK *OnLayerMouseOut)(DOCKLET_DATA*, UINT, const char*);
			BOOL (CALLBACK *OnLayerMouseWheelForward)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD, int);
			BOOL (CALLBACK *OnLayerMouseWheelBackward)(DOCKLET_DATA*, UINT, const char*, const POINT*, const SIZE*, DWORD, int);

			BOOL (CALLBACK *OnBeforeRemoval)(DOCKLET_DATA*);

			// 1.4?
			const SkinInfo* (CALLBACK *OnGetCurrentSkin)(DOCKLET_DATA*);
			BOOL (CALLBACK *OnSetCurrentSkin)(DOCKLET_DATA*, const SkinInfo*);


			// 2.0
			BOOL (CALLBACK *OnGetFlags) (DOCKLET_DATA*, DWORD*);
			UINT (CALLBACK *OnGetSkinPreviewBitmapCacheEntry)(ENUMERATOR_DATA*, const SkinInfo*, BitmapCacheEntry*);
			BOOL (CALLBACK *OnGetSkinPreviewBitmap)(ENUMERATOR_DATA*, const SkinInfo*,PreviewBitmapRequestData*);
			BOOL (CALLBACK *OnStopSkinPreviewBitmap)(ENUMERATOR_DATA*, PreviewBitmapRequestData*);

			BOOL (CALLBACK *OnLayerGetToolTip)(DOCKLET_DATA*, UINT, const char*, const WCHAR*);

				
			// wrapper functions that will execute the function pointer with the same
			// name if this is defined by the DLL
			bool ProcessMessage(DOCKLET_DATA *lpData,HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
			bool Save(DOCKLET_DATA *lpData,const char *szIni,const char *szIniGroup,BOOL bIsForExport);
			bool LeftButtonClick(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			bool DoubleClick(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			bool LeftButtonHeld(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			bool RightButtonClick(DOCKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDocklet);
			bool Configure(DOCKLET_DATA *lpData);
			bool AcceptDropFiles(DOCKLET_DATA *lpData);
			bool DropFiles(DOCKLET_DATA *lpData,HDROP hDrop);
			bool ExportFiles(DOCKLET_DATA *lpData,char *szFileRelativeOut,int iteration);
			void Destroy(DOCKLET_DATA *lpData,HWND hwndDocklet);

			// v1.1 only
			bool MouseOn(DOCKLET_DATA* lpData);
			bool MouseOut(DOCKLET_DATA* lpData);
			bool MouseMove(DOCKLET_DATA* lpData, int x, int y);
			bool MouseWheelForward(DOCKLET_DATA* lpData, WORD fKeysDown, int numOfScrolls, int x, int y);  
			bool MouseWheelBackward(DOCKLET_DATA* lpData, WORD fKeysDown, int numOfScrolls, int x, int y); 

			bool DragEnter(DOCKLET_DATA *lpData,IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect);
			bool DragOver(DOCKLET_DATA *lpData,DWORD keys,POINTL pnt,DWORD* effect);
			bool DragLeave(DOCKLET_DATA *lpData);
			bool Drop(DOCKLET_DATA *lpData,IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect);
			bool DragDropDataRequest(DOCKLET_DATA* lpData,FORMATETC* format, STGMEDIUM* medium);

			bool Select(DOCKLET_DATA* lpData);
			bool Deselect(DOCKLET_DATA* lpData);

			int  AddPropertySheets(DOCKLET_DATA* lpData,int numPresent);
			LRESULT ProcessMessageEx(DOCKLET_DATA *lpData,HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam, BOOL* isHandled);
			bool VisibilityNotify(DOCKLET_DATA *lpData, BOOL isVisible, DWORD flags);
			bool BeforeRemoval(DOCKLET_DATA* lpData);

			// tooltip request
			bool LayerGetToolTip(DOCKLET_DATA* lpData, UINT layerId, const char* layerName, const WCHAR* currentEntry);

		public:

			ENUMERATOR_DATA*	CreateSkinEnumeratorW();
			void				DestroySkinEnumeratorW(ENUMERATOR_DATA* ed);
			INT					GetSkinW(ENUMERATOR_DATA* ed,SkinInfo* skinInfo);
			DOCKLET_DATA*		CreateFromSkinW(HWND hwnd, ENUMERATOR_DATA* ed, const SkinInfo* wrapper);
			bool				SetSkinW(ENUMERATOR_DATA* ed, const SkinInfo* info);
			bool				SupportsSkinEnumerator()const;
			
			// to create an instance from this docklet
			OdDockletInstance* CreateInstance(const char* iniFile,const  char* iniGroup, WNDPROC wndProc, HINSTANCE hInstance);
			OdDockletInstance* CreateNotActivatedInstance();

			void GetInformation(char *szName,char *szAuthor,int *lpiVersion,char *szNotes);

			// accessible from COM wrapper
			bool GetModel(DOCKLET_DATA* lpData, IDispatch**pVal);

			// v2.0 only
			bool GetFlags(DOCKLET_DATA *lpData, DWORD* flags);
			UINT  GetSkinPreviewBitmapCacheEntry(ENUMERATOR_DATA*, const SkinInfo*, BitmapCacheEntry*);
			bool GetSkinPreviewBitmap(ENUMERATOR_DATA*, const SkinInfo*,PreviewBitmapRequestData*);
			bool StopSkinPreviewBitmap(ENUMERATOR_DATA*, PreviewBitmapRequestData*);
			bool SupportBitmapPreviews(){return OnGetSkinPreviewBitmap != NULL;};

      // static functions
			static OdDocklet* LoadDockletFromDLL(HMODULE hMod);

			// FIX: helpfunction for OnConfigure() hanging the control panel
			bool HasOnConfigure();

			bool LayerMouseDown(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys);
			bool LayerMouseUp(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys);
			bool LayerClick(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys);
			bool LayerRightClick(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys);
			bool LayerMouseOn(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s);
			bool LayerMouseMove(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s);
			bool LayerMouseOut(DOCKLET_DATA* data, UINT id, const char* name);
			bool LayerMouseWheelForward(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys, int numScrolls);
			bool LayerMouseWheelBackward(DOCKLET_DATA* data, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys, int numScrolls);
	};

};

#endif//INCLUDE_GUARD_203919929191929119191_OD_DOCKLET_HPP