//	This file and its contents is copyright © 2006 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.


#if !defined(AFX_AVAILABLEDESKLETSAPI_H__7B24644F_67DA_4FB8_A387_26D54C3A1DE0__INCLUDED_)
#define AFX_AVAILABLEDESKLETSAPI_H__7B24644F_67DA_4FB8_A387_26D54C3A1DE0__INCLUDED_

#ifdef __cplusplus
#define USE_AVAILABLEDESKLETSAPI AvailableDeskletsAPI::InitializeAvailableDeskletsAPI(NULL);
#else
#define USE_AVAILABLEDESKLETSAPI InitializeAvailableDeskletsAPI(NULL);
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#ifdef __cplusplus
class AvailableDeskletsAPI  
{
#endif//__cplusplus

public:

	
	static BOOL InitializeAvailableDeskletsAPI(HMODULE hMod = NULL)
	{
		if(NULL == hMod)
			hMod = GetModuleHandle(NULL);

		AveCreateADEnumeratorDUMMY = (HAveCreateADEnumerator) GetProcAddress(hMod, "AveCreateADEnumerator");
		AveFreeADEnumeratorDUMMY = (HAveFreeADEnumerator) GetProcAddress(hMod, "AveFreeADEnumerator");

		AveStartEnumeratingDUMMY = (HAveStartEnumerating) GetProcAddress(hMod, "AveStartEnumerating");
		AveStopEnumeratingDUMMY = (HAveStopEnumerating) GetProcAddress(hMod, "AveStopEnumerating");

		AveStartCreatingPreviewIconDUMMY = (HAveStartCreatingPreviewIcon) GetProcAddress(hMod, "AveStartCreatingPreviewIcon");
		AveInstantiateEntryDUMMY = (HAveInstantiateEntry) GetProcAddress(hMod, "AveInstantiateEntry");

		AveStartDragOperationDUMMY = (HAveStartDragOperation) GetProcAddress(hMod, "AveStartDragOperation");
		
		return AveCreateADEnumeratorDUMMY != NULL;
	}

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

	struct AveAD;

	typedef BOOL (CALLBACK *ONNEWDESKLET_PROC)(AveAD* ad, const AveADEntry* entry);
	typedef BOOL (CALLBACK *ONPREVIEWDONE_PROC)(AveAD* ad, const AveADEntry* entry, HBITMAP hBmp);

	struct AveAD
	{
		DWORD cbSize;				// must be set to sizeof(AveAD)
		HWND marshallingHwnd;		// a HWND internally used for marshalling - can be safely ignored.
		LONG_PTR* reserved1;		// a field used internally by the system. Do not change.
		LONG_PTR* reserved2;		// a field used internally by the system. Do not change.
		LONG_PTR* reserved3;		// a field used internally by the system. Do not change.
		LPARAM lParam;				// a field that can be set by the user, for callback purposes.
	};

#pragma pack(pop, 1)

	typedef AveAD* (CALLBACK *HAveCreateADEnumerator)(const WCHAR*, LPARAM);
	static HAveCreateADEnumerator AveCreateADEnumeratorDUMMY;

	typedef BOOL (CALLBACK *HAveFreeADEnumerator)(AveAD*);
	static HAveFreeADEnumerator AveFreeADEnumeratorDUMMY;

	typedef BOOL (CALLBACK *HAveStartEnumerating)(AveAD*, ONNEWDESKLET_PROC );
	static HAveStartEnumerating AveStartEnumeratingDUMMY;

	typedef BOOL (CALLBACK *HAveStopEnumerating)(AveAD*);
	static HAveStopEnumerating AveStopEnumeratingDUMMY;

	typedef BOOL (CALLBACK *HAveStartCreatingPreviewIcon)(AveAD*, const AveADEntry*, SIZE, ONPREVIEWDONE_PROC);
	static HAveStartCreatingPreviewIcon AveStartCreatingPreviewIconDUMMY;

	typedef HWND (CALLBACK *HAveInstantiateEntry)(const AveADEntry*, BOOL, BOOL);
	static HAveInstantiateEntry AveInstantiateEntryDUMMY;

	typedef  BOOL (CALLBACK *HAveStartDragOperation)(const AveADEntry*, SIZE, HWND);
	static HAveStartDragOperation AveStartDragOperationDUMMY;

	/**
	 * Creates an AD enumerator.
	 * @param folderName - the foldername to check the files from.
	 * @param lParam - a value thats kept inside the AveAD structure which can be used in callbacks. [optional]
	 * @return An AveAD structure
	 */
	static AveAD* AveCreateADEnumerator(const WCHAR* folderName, LPARAM lParam)
	{
		if(NULL == AveCreateADEnumeratorDUMMY && !InitializeAvailableDeskletsAPI(NULL))
			return FALSE;

		return AveCreateADEnumeratorDUMMY(folderName, lParam);
	}


	/**
	 * Frees an AD enumerator and its resources.
	 * @param ad - the enumerator to free.
	 * @return TRUE iff successfull.
	 */
	static BOOL CALLBACK AveFreeADEnumerator(AveAD* ad)
	{
		if(NULL == AveFreeADEnumeratorDUMMY && !InitializeAvailableDeskletsAPI(NULL))
			return FALSE;

		return AveFreeADEnumeratorDUMMY(ad);
	}

	/**
	 * Starts enumerating desklets
	 * @param ad - the enumerator that should start enumerating its desklets.
	 * @param proc - the function that will be called when a new desklet has been found.
	 * @return TRUE iff successfull.
	 */
	static BOOL CALLBACK AveStartEnumerating(AveAD* ad, ONNEWDESKLET_PROC proc)
	{
		if(NULL == AveStartEnumeratingDUMMY  && !InitializeAvailableDeskletsAPI(NULL))
			return FALSE;

		return AveStartEnumeratingDUMMY(ad, proc);
	}

	/**
	 * Stops enumerating desklets
	 * @param ad - the enumerator to stop.
	 * @return TRUE iff successfull.
	 */
	static BOOL CALLBACK AveStopEnumerating(AveAD* ad)
	{
		if(NULL == AveStopEnumeratingDUMMY  && !InitializeAvailableDeskletsAPI(NULL))
			return FALSE;

		return AveStopEnumeratingDUMMY(ad);
	}

	/**
	 * Start creating a preview icon from an entry.
	 * @param ad - the enumerator that fetched the entry.
	 * @param entry - the entry received thru a callback of AveStartEnumerating [internalId must be set].
	 * @param size - the requested size of the preview icon.
	 * @param proc - the function that will be called when the preview icon is ready.
	 * @return TRUE iff successfull and not already creating an icon for this entry,
	 */
	static BOOL CALLBACK AveStartCreatingPreviewIcon(AveAD* ad, const AveADEntry* entry, SIZE size, ONPREVIEWDONE_PROC proc)
	{
		if(NULL == AveStartCreatingPreviewIconDUMMY && !InitializeAvailableDeskletsAPI(NULL))
			return FALSE;

		return AveStartCreatingPreviewIconDUMMY(ad, entry, size, proc);
	}

	/**
	 * Instantiates a desklet from an entry.
	 * @param entry - the entry to instantiate in a desklet. [internalId must be set]
	 * @param deskletIsInitallyHidden - TRUE iff the desklet should not be made visible
	 * @param async - if TRUE, the function will not wait for the desklet being totally initialized. If FALSE,
	 *					the function will return when the desklet is totally initialized.
	 * @return the handle of the new desklet if successfull, otherwise NULL.
	 */
	static HWND AveInstantiateEntry(const AveADEntry* entry, BOOL deskletIsInitiallyHidden, BOOL async)
	{
		if(NULL == AveInstantiateEntryDUMMY && !InitializeAvailableDeskletsAPI(NULL))
			return NULL;

		return AveInstantiateEntryDUMMY(entry, deskletIsInitiallyHidden, async);
	}

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
	static BOOL AveStartDragOperation(AveADEntry* const entry, SIZE size, HWND dropWindow)
	{
		if(NULL == AveStartDragOperationDUMMY && !InitializeAvailableDeskletsAPI(NULL))
			return FALSE;

		return AveStartDragOperationDUMMY(entry, size, dropWindow);
	}

#ifdef __cplusplus
};

#endif//__cplusplus

#endif // !defined(AFX_AVAILABLEDESKLETSAPI_H__7B24644F_67DA_4FB8_A387_26D54C3A1DE0__INCLUDED_)
