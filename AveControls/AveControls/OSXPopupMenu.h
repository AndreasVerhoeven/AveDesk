#ifndef AVE_OSX_POPUPMENU_484
#define AVE_OSX_POPUPMENU_484
#
struct MenuData
{
	WCHAR** items;
	DWORD  count;
	DWORD  selected;

};

BOOL DoOSXPopupMenu(HWND parent, UINT notifyMessage, const MenuData* items, const char* resourcesPath, const POINT* pt);

#endif//AVE_OSX_POPUPMENU_484