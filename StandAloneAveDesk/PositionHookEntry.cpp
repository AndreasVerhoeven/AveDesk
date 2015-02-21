#include "stdafx.h"
#include "PositionHookEntry.h"

CPositionHookEntry::CPositionHookEntry(void* ptr, UINT id, HOnPositionChange callbackPosChange) : 
	ptr(ptr),
	id(id),
	callbackPositionChange(callbackPosChange)
{
}

CPositionHookEntry::~CPositionHookEntry(void)
{
}

BOOL CPositionHookEntry::onPositionChange(HWND hwndDesklet, RECT* r)
{
	if(NULL == callbackPositionChange)
		return FALSE;

	return callbackPositionChange(ptr, hwndDesklet, r);
}