#ifndef AVE_Ave_POPUPMENU_484
#define AVE_Ave_POPUPMENU_484
#
struct MenuData
{
	WCHAR** items;
	DWORD  count;
	DWORD  selected;

};

BOOL DoAvePopupMenu(HWND parent, UINT notifyMessage, const MenuData* items, const char* resourcesPath, const POINT* pt);

#endif//AVE_Ave_POPUPMENU_484