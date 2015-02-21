// AvailableDeskletsAPI.cpp: implementation of the AvailableDeskletsAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AvailableDeskletsAPI.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

	/**
	 * Creates an AD enumerator.
	 * @param folderName - the foldername to check the files from.
	 * @param lParam - a value thats kept inside the AveAD structure which can be used in callbacks. [optional]
	 * @return An AveAD structure
	 */
AvailableDeskletsAPI::AveAD* CALLBACK AvailableDeskletsAPI::AveCreateADEnumerator(const WCHAR* folderName, LPARAM lParam)
	{
		if(NULL == folderName)
			return NULL;

		AveAD* ad = new AveAD;
		ad->cbSize = sizeof(AveAD);
		ad->callbacks = new AveADCallbacks;
		ad->lParam = lParam;
		ZeroMemory(ad->callbacks, sizeof(AveADCallbacks));
		ad->marshallingHwnd = CreateMarshallingWindow(ad);
		ad->finder = new CDeskletFinder(folderName);

		return ad;
	}


	/**
	 * Frees an AD enumerator and its resources.
	 * @param ad - the enumerator to free.
	 * @return TRUE iff successfull.
	 */
	BOOL CALLBACK AvailableDeskletsAPI::AveFreeADEnumerator(AveAD* ad)
	{
		if(NULL == ad)
			return FALSE;

		HWND hwnd = ad->marshallingHwnd;
		ad->finder->Stop(INFINITE);
		delete ad->finder;
		delete ad->callbacks;
		delete ad;
		::DestroyWindow(hwnd);

		return TRUE;
	}

	/**
	 * Starts enumerating desklets
	 * @param ad - the enumerator that should start enumerating its desklets.
	 * @param proc - the function that will be called when a new desklet has been found.
	 * @return TRUE iff successfull.
	 */
	BOOL CALLBACK AvailableDeskletsAPI::AveStartEnumerating(AveAD* ad, ONNEWDESKLET_PROC proc)
	{
		if(NULL == ad)
			return FALSE;

		ad->callbacks->OnNewDesklet = proc;

		ad->finder->Start(ad->marshallingHwnd, WM_FOUNDDESKLET_CALLBACK_MSG);
		return TRUE;
	}

	/**
	 * Stops enumerating desklets
	 * @param ad - the enumerator to stop.
	 * @return TRUE iff successfull.
	 */
	BOOL CALLBACK AvailableDeskletsAPI::AveStopEnumerating(AveAD* ad)
	{
		if(NULL == ad)
			return FALSE;

		ad->finder->Stop(INFINITE);

		return TRUE;
	}

	/**
	 * Start creating a preview icon from an entry.
	 * @param ad - the enumerator that fetched the entry.
	 * @param entry - the entry received thru a callback of AveStartEnumerating [internalId must be set].
	 * @param size - the requested size of the preview icon.
	 * @param proc - the function that will be called when the preview icon is ready.
	 * @return TRUE iff successfull and not already creating an icon for this entry,
	 */
	BOOL CALLBACK AvailableDeskletsAPI::AveStartCreatingPreviewIcon(AveAD* ad, AveADEntry* entry, SIZE size, ONPREVIEWDONE_PROC proc)
	{
		if(NULL == ad || NULL == entry)
			return FALSE;

		CFoundDeskletEntry* nativeEntry = (CFoundDeskletEntry*)entry->internalId;
		if(NULL == nativeEntry)
			return FALSE;

		if(nativeEntry->IsRunning())
			return FALSE;

		entry->reserved = (LONG_PTR*) proc;
		nativeEntry->SetLParam((LPARAM)proc);
		nativeEntry->StartCreatingPreviewBitmap(size, ad->marshallingHwnd, WM_PREVIEWDONE_CALLBACK_MSG);

		return TRUE;
	}

	/**
	 * Instantiates a desklet from an entry.
	 * @param entry - the entry to instantiate in a desklet. [internalId must be set]
	 * @param deskletIsInitallyHidden - TRUE iff the desklet should not be made visible
	 * @param async - if TRUE, the function will not wait for the desklet being totally initialized. If FALSE,
	 *					the function will return when the desklet is totally initialized.
	 * @return the handle of the new desklet if successfull, otherwise NULL.
	 */
	HWND CALLBACK AvailableDeskletsAPI::AveInstantiateEntry(AveADEntry* entry, BOOL deskletIsInitiallyHidden, BOOL async)
	{
		if(NULL == entry)
			return NULL;

		CFoundDeskletEntry* nativeEntry = (CFoundDeskletEntry*)entry->internalId;
		if(NULL == nativeEntry)
			return NULL;	

		Docklet* d = nativeEntry->Instantiate(FALSE, deskletIsInitiallyHidden);
		if(NULL == d)
			return NULL;
		
		if(!async)
			d->WaitForDockletBeingInitialized(INFINITE);

		return d->GetHWnd();
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
	BOOL CALLBACK AvailableDeskletsAPI::AveStartDragOperation(AveADEntry* entry, SIZE size, HWND dropWindow)
	{
		if(NULL == entry)
			return FALSE;

		CFoundDeskletEntry* nativeEntry = (CFoundDeskletEntry*)entry->internalId;
		if(NULL == nativeEntry)
			return FALSE;	

		CDragAnimationWindow* anim = new CDragAnimationWindow;
		anim->StartAnimation(nativeEntry, size, dropWindow);

		return TRUE;
	}
