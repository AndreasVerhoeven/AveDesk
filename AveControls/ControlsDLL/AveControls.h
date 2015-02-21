//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

#ifndef AVE_CONTROLS_HEADER_GUARD_23939
#define AVE_CONTROLS_HEADER_GUARD_23939

#define _UNICODE
#define UNICODE
#include <windows.h>

typedef DWORD_PTR* AVEHANDLE;

#pragma pack(push, 1)

struct AVE_VERSION
{
	DWORD major;
	DWORD minor;
	DWORD revision;

	DWORD intendedAveDeskVersion;
};

#pragma pack(pop)

// general functions
BOOL __stdcall AveGetVersion(AVE_VERSION* version);


// resources functions
AVEHANDLE __stdcall AveCreateResources(const CHAR* baseFolder);
BOOL __stdcall AveFreeResources(AVEHANDLE handle);
AVEHANDLE __stdcall AveCreateResourcesW(const WCHAR* baseFolder);
BOOL __stdcall AveSetResourcesPath(AVEHANDLE resourcesHandle, const CHAR* baseFolder);
BOOL __stdcall AveSetResourcesPathW(AVEHANDLE resourcesHandle, const WCHAR* baseFolder);

// control container functions
AVEHANDLE __stdcall AveCreateControlContainer(HWND deskletHwnd, AVEHANDLE resources);
BOOL __stdcall AveFreeControlContainer(AVEHANDLE container);

BOOL __stdcall AveAddControlToContainer(AVEHANDLE containerHandle, AVEHANDLE controlHandle);
BOOL __stdcall AveRemoveControlFromContainer(AVEHANDLE containerHandle, const CHAR* name);
AVEHANDLE __stdcall AveGetControlFromContainer(AVEHANDLE containerHandle, const CHAR* name);
BOOL __stdcall AveEmptyContainer(AVEHANDLE containerHandle);

BOOL __stdcall AveSetFocusToControl(AVEHANDLE containerHandle, const CHAR* name);

INT __stdcall AveLockContainer(AVEHANDLE containerHandle);
INT __stdcall AveUnlockContainer(AVEHANDLE containerHandle);
BOOL __stdcall AveIsContainerLocked(AVEHANDLE containerHandle);

// control functions
AVEHANDLE __stdcall AveCreateControl(AVEHANDLE container, const CHAR* name, const CHAR* type, DWORD flags, DWORD_PTR* reserved);
BOOL __stdcall AveFreeControl(AVEHANDLE control);


INT __stdcall AveLockControl(AVEHANDLE controlHandle);
INT __stdcall AveUnlockControl(AVEHANDLE controlHandle);
BOOL __stdcall AveIsControlLocked(AVEHANDLE controlHandle);

// container event forwarders
BOOL __stdcall AveForwardMessage(AVEHANDLE containerHandle, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL __stdcall AveForwardMouseWheelForward(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys, int scrolls);
BOOL __stdcall AveForwardMouseWheelBackward(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys, int scrolls);
BOOL __stdcall AveForwardMouseDown(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);
BOOL __stdcall AveForwardMouseUp(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);
BOOL __stdcall AveForwardMouseOn(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s);
BOOL __stdcall AveForwardMouseMove(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s);
BOOL __stdcall AveForwardMouseClick(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);
BOOL __stdcall AveForwardMouseRightClick(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);
BOOL __stdcall AveForwardMouseOut(AVEHANDLE containerHandle, UINT id, const CHAR* layerName);


// control set/get function
DWORD_PTR __stdcall AveSendMessage(AVEHANDLE controlHandle, UINT msg, WPARAM wParam, LPARAM lParam);


#endif//AVE_CONTROLS_HEADER_GUARD_23939