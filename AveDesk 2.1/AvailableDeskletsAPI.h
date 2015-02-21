// AvailableDeskletsAPI.h: interface for the AvailableDeskletsAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVAILABLEDESKLETSAPI_H__7B24644F_67DA_4FB8_A387_26D54C3A1DE0__INCLUDED_)
#define AFX_AVAILABLEDESKLETSAPI_H__7B24644F_67DA_4FB8_A387_26D54C3A1DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DeskletFinder.h"
#include "DragAnimationWindow.h"


#define WM_FOUNDDESKLET_CALLBACK_MSG WM_USER + 888
#define WM_PREVIEWDONE_CALLBACK_MSG  WM_USER + 889

class AvailableDeskletsAPI  
{
private:
	// protected c`tor, so this class cant be instantiated nor overriden
	AvailableDeskletsAPI(){}

public:

#pragma pack(push, 1)
	struct AveADEntry
	{
		DWORD cbSize;			// must be set to sizeof(AveADEntry)
		LONG_PTR* internalId;	// an internalId used... internally :-)
		LONG_PTR* reserved;		// reserved for the system
		HBITMAP hBmp;			// hBitmap of the preview icon
		WCHAR name[256];		// name of the entry
		WCHAR description[1024];// description of the entry
		WCHAR author[256];		// author of the entry
		WCHAR version[80];		// version of the entry
	};
#pragma pack(pop)

	struct AveAD;

	typedef BOOL (CALLBACK *ONNEWDESKLET_PROC)(AveAD* ad, const AveADEntry* entry);
	typedef BOOL (CALLBACK *ONPREVIEWDONE_PROC)(AveAD* ad, const AveADEntry* entry, HBITMAP hBmp);

#pragma pack(push,1)
	struct AveADCallbacks
	{
		ONNEWDESKLET_PROC OnNewDesklet;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct AveAD
	{
		DWORD cbSize;				// must be set to sizeof(AveAD)
		HWND marshallingHwnd;		// a HWND internally used for marshalling - can be safely ignored.
		CDeskletFinder* finder;		// a field used internally by the system. Do not change.
		AveADCallbacks* callbacks;	// a field used internally by the system. Do not change.
		LONG_PTR* reserved;			// a field used internally by the system. Do not change.
		LPARAM lParam;				// a field that can be set by the user, for callback purposes.
	};
#pragma pack(pop)


	/// START callback functions
	static ATOM RegisterMarshallingWindowClass()
	{
		// register window class
		WNDCLASSEX wc = {0};
		wc.cbSize = sizeof(wc);
		wc.hInstance = _Module.GetModuleInstance();
		wc.lpfnWndProc = MarshallingWndProc;
		wc.lpszClassName = _T("AveDeskThreadMarshalling");
		return ::RegisterClassEx(&wc);
	}

	static HWND CreateMarshallingWindow(AveAD* ad)
	{
		RegisterMarshallingWindowClass();

		HWND hwnd = ::CreateWindowEx(0, _T("AveDeskThreadMarshalling"), _T("AveDesk Marshalling"), 0,
						0,0, 1, 1,  HWND_MESSAGE, NULL, _Module.GetModuleInstance(), NULL);
		::SetProp(hwnd, _T("data"), (HANDLE)ad);
		return hwnd;
	}


	static AveADEntry GetEntry(CFoundDeskletEntry* entry)
	{
		AveADEntry adEntry = {0};
		adEntry.cbSize  = sizeof(adEntry);
		adEntry.internalId = (LONG_PTR*)entry;
		adEntry.reserved = NULL;
		adEntry.hBmp = NULL;
		std::wstring descr = entry->GetDescription();
		std::wstring name = entry->GetName();
		std::wstring author = entry->GetAuthor();
		std::wstring version = entry->GetVersion();
		
		wcsncpy(adEntry.description, descr.c_str(), sizeof(adEntry.description) / sizeof(adEntry.description[0]) - 1);
		wcsncpy(adEntry.name, name.c_str(),  sizeof(adEntry.name) / sizeof(adEntry.name[0]) - 1);
		wcsncpy(adEntry.author, author.c_str(), sizeof(adEntry.author) / sizeof(adEntry.author[0]) - 1);
		wcsncpy(adEntry.version, version.c_str(), sizeof(adEntry.version) / sizeof(adEntry.version[0]) - 1);

		return adEntry;
	}

	static LONG CALLBACK MarshallingWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
	{
		AveAD* ad;
		CFoundDeskletEntry* entry;

		switch(msg)
		{
		case WM_FOUNDDESKLET_CALLBACK_MSG:
			if( (ad = (AveAD*) GetProp(hwnd, _T("data"))) != NULL)
			{
				if(ad->callbacks->OnNewDesklet != NULL)
				{
					CFoundDeskletEntry* nativeEntry = ad->finder->GetEntry((UINT)wParam);
					if(nativeEntry != NULL)
					{
						AveADEntry entry = GetEntry(nativeEntry);
						BOOL bRes = ad->callbacks->OnNewDesklet(ad, &entry);
					}
				}
			}
			return 0;
			break;

		case WM_PREVIEWDONE_CALLBACK_MSG:
			if( (entry = (CFoundDeskletEntry*)lParam) != NULL)
			{
				if( (ad = (AveAD*) GetProp(hwnd, _T("data"))) != NULL)
				{
					ONPREVIEWDONE_PROC proc = (ONPREVIEWDONE_PROC) entry->GetLParam();
					if(proc != NULL)
					{
						HBITMAP hBmp = entry->GetPreviewBitmap();
						AveADEntry adEntry = GetEntry(entry);
						adEntry.hBmp = hBmp;
						BOOL bRes = proc(ad, &adEntry, hBmp);
					}
				}
			}

			return 0;
			break;

		default:
			break;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	/// END callback functions


	/**
	 * Creates an AD enumerator.
	 * @param folderName - the foldername to check the files from.
	 * @param lParam - a value thats kept inside the AveAD structure which can be used in callbacks. [optional]
	 * @return An AveAD structure
	 */
	static AveAD* CALLBACK AveCreateADEnumerator(const WCHAR* folderName, LPARAM lParam);

	/**
	 * Frees an AD enumerator and its resources.
	 * @param ad - the enumerator to free.
	 * @return TRUE iff successfull.
	 */
	static BOOL CALLBACK AveFreeADEnumerator(AveAD* ad);

	/**
	 * Starts enumerating desklets
	 * @param ad - the enumerator that should start enumerating its desklets.
	 * @param proc - the function that will be called when a new desklet has been found.
	 * @return TRUE iff successfull.
	 */
	static BOOL CALLBACK AveStartEnumerating(AveAD* ad, ONNEWDESKLET_PROC proc);

	/**
	 * Stops enumerating desklets
	 * @param ad - the enumerator to stop.
	 * @return TRUE iff successfull.
	 */
	static BOOL CALLBACK AveStopEnumerating(AveAD* ad);

	/**
	 * Start creating a preview icon from an entry.
	 * @param ad - the enumerator that fetched the entry.
	 * @param entry - the entry received thru a callback of AveStartEnumerating [internalId must be set].
	 * @param size - the requested size of the preview icon.
	 * @param proc - the function that will be called when the preview icon is ready.
	 * @return TRUE iff successfull and not already creating an icon for this entry,
	 */
	static BOOL CALLBACK AveStartCreatingPreviewIcon(AveAD* ad, AveADEntry* entry, SIZE size, ONPREVIEWDONE_PROC proc);

	/**
	 * Instantiates a desklet from an entry.
	 * @param entry - the entry to instantiate in a desklet. [internalId must be set]
	 * @param deskletIsInitallyHidden - TRUE iff the desklet should not be made visible
	 * @param async - if TRUE, the function will not wait for the desklet being totally initialized. If FALSE,
	 *					the function will return when the desklet is totally initialized.
	 * @return the handle of the new desklet if successfull, otherwise NULL.
	 */
	static HWND CALLBACK AveInstantiateEntry(AveADEntry* entry, BOOL deskletIsInitiallyHidden, BOOL async);

	/**
	 * Starts a drag-operation for an entry. A drag-operation allows the user to drag
	 * the preview icon to the desktop and drop it at the desired position, where the desklet
	 * will be instantiated. The operation will display several effects.
	 * @param entry - the entry to start a drag operation for. [internalId must be set]
	 * @param size - the preview icon size.
	 * @param dropWindow - if the "drag" is being dragged over the area occupied by this window,
	 *						it will notify the user graphically it will not instantiate at that position.
	 *						If the user drops the "drag" inside this windows boundaries, the desklet will not
	 *						instantiate.
	 *						Settings this property allows the user an escape - other than pressing ESCAPE - to the
	 *						drag-operation.
	 *						If set to NULL, this property will be ignored.
	 * @return TRUE iff successfull.
	 */
	static BOOL CALLBACK AveStartDragOperation(AveADEntry* entry, SIZE size, HWND dropWindow);
};

#endif // !defined(AFX_AVAILABLEDESKLETSAPI_H__7B24644F_67DA_4FB8_A387_26D54C3A1DE0__INCLUDED_)
