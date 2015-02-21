#pragma once

class CPositionHookEntry
{
public:
	typedef BOOL (CALLBACK *HOnPositionChange)(void* ptr, HWND hwndDesklet, RECT* r);
protected:
	UINT id;
	HOnPositionChange callbackPositionChange;
	void* ptr;

public:
	CPositionHookEntry(void* ptr, UINT id, HOnPositionChange callbackPosChange);
	~CPositionHookEntry(void);

	BOOL onPositionChange(HWND hwndDesklet, RECT* r);

	UINT GetID()const{return id;};
};
