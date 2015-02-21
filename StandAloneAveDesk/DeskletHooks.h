#pragma once


#include <list>

#include "PositionHookEntry.h"

class CDeskletHooks
{
protected:
	CDeskletHooks(void);
	~CDeskletHooks(void);

	std::list<CPositionHookEntry> positionHooks;

	static CDeskletHooks* m_Instance;

public:

	static CDeskletHooks* GetInstance() { return m_Instance;};

	BOOL onPositionChange(HWND hwndDesklet, RECT* r);

	UINT AddPositionHook(void* ptr, CPositionHookEntry::HOnPositionChange callback1);
	BOOL RemovePositionHook(UINT id);
};
