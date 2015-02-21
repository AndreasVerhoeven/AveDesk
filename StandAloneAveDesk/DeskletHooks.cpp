#include "stdafx.h"
#include "DeskletHooks.h"

CDeskletHooks* CDeskletHooks::m_Instance = new CDeskletHooks;

CDeskletHooks::CDeskletHooks(void)
{
}

CDeskletHooks::~CDeskletHooks(void)
{
}


BOOL CDeskletHooks::onPositionChange(HWND hwndDesklet, RECT* r)
{
	std::list<CPositionHookEntry> hooks = positionHooks;

	BOOL res = FALSE;

	for(std::list<CPositionHookEntry>::iterator iter = hooks.begin(); iter != hooks.end(); ++iter)
	{
		res |= iter->onPositionChange(hwndDesklet, r);
	}

	return res;
}

UINT CDeskletHooks::AddPositionHook(void* ptr, CPositionHookEntry::HOnPositionChange callback1)
{
	UINT newID = 1;
	std::list<CPositionHookEntry> hooks = positionHooks;
	for(std::list<CPositionHookEntry>::iterator iter = hooks.begin(); iter != hooks.end(); ++iter)
	{
		UINT id = iter->GetID();
		if(id >= newID)
			newID = id + 1;
	}

	positionHooks.push_back(CPositionHookEntry(ptr, newID, callback1));

	return newID;
}


BOOL CDeskletHooks::RemovePositionHook(UINT id)
{
	for(std::list<CPositionHookEntry>::iterator iter = positionHooks.begin(); iter != positionHooks.end(); ++iter)
	{
		if(iter->GetID() == id)
		{
			positionHooks.erase(iter);
			return TRUE;
		}
	}

	return FALSE;
}