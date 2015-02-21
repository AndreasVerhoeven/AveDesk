#pragma pack(push, 1)

struct DESKLET_STYLE_CHANGE_DATA
{
	DWORD dwSize;		// sizeof(DESKLET_STYLE_CHANGE_DATA)

	UINT oldStyle;		// oldStyle
	UINT newStyle;		// newStyle
	DWORD flags;		// unused

	HWND deskletHwnd;	// the hwnd of the desklet
	HWND labelHwnd;		// the hwnd of the desklets label

	UINT numExtraHwnds;	// the number of extra hwnds
	HWND* extraHwnds;	// pointer to an array containing the extra hwnds
};

typedef UINT (CALLBACK *HOnStyleChange)(void* ptr, const DESKLET_STYLE_CHANGE_DATA* dscd);
typedef UINT (CALLBACK *HOnGetInsertHwnd)(void* ptr, DESKLET_STYLE_CHANGE_DATA* dscd);
typedef UINT_PTR (CALLBACK *HOnMessage)(void* ptr, const DESKLET_STYLE_CHANGE_DATA* dscd, MSG* msg);

typedef BOOL (CALLBACK *HOnPositionChange)(void* ptr, HWND hwndDesklet, RECT* r);

struct DESKLET_STYLE_DATA
{
	DWORD dwSize;				// sizeof(DESKLET_STYLE_DATA)
	void*	callbackPointer;// pointer used in callbacks
	const WCHAR* friendlyName;	// the friendly name (as displayed to the user) for this style
	const WCHAR* uniqueName;	// the unique name for this style
	DWORD flags;				// unused
		
	HOnStyleChange	styleChangeCallback;	// style change callback function
	HOnGetInsertHwnd insertHwndCallback;	// callback function for obtaining the insert hwnd
	HOnMessage		 messageCallback;		// callback function for passing windows messages

	UINT_PTR* reserved1;//unused
	UINT_PTR* reserved2;//unused
	UINT_PTR* reserved3;//unused
	UINT_PTR* reserved4;//unused
	UINT_PTR* reserved5;//unused
	UINT_PTR* reserved6;//unused
};

struct POSITION_HOOK_DATA
{
	DWORD dwSize;				// sizeof(POSITION_HOOK_DATA)
	void* callbackPointer;		// pointer used in callbacks

	HOnPositionChange onPositionChangeCallback; // called when the position changes

	UINT_PTR* reserved1;//unused
	UINT_PTR* reserved2;//unused
	UINT_PTR* reserved3;//unused
	UINT_PTR* reserved4;//unused
	UINT_PTR* reserved5;//unused
	UINT_PTR* reserved6;//unused
};

#pragma pack(pop)

UINT CALLBACK AveRegisterDeskletStyle(const DESKLET_STYLE_DATA* data)
{
	typedef UINT (CALLBACK *HAveRegisterDeskletStyle)(const DESKLET_STYLE_DATA*);
	HAveRegisterDeskletStyle ptr = (HAveRegisterDeskletStyle)GetProcAddress(GetModuleHandle(0), "AveRegisterDeskletStyle");

	if(!ptr)
		return 0;

	return ptr(data);
}

BOOL CALLBACK AveUnregisterDeskletStyle(UINT styleID)
{
	typedef BOOL (CALLBACK *HAveUnregisterDeskletStyle)(UINT);
	HAveUnregisterDeskletStyle ptr = (HAveUnregisterDeskletStyle)GetProcAddress(GetModuleHandle(0), "AveUnregisterDeskletStyle");

	if(!ptr)
		return 0;

	return ptr(styleID);
}

UINT CALLBACK AveRegisterPositionHook(const POSITION_HOOK_DATA* data)
{
	typedef BOOL (CALLBACK *HAveRegisterPositionHook)(const POSITION_HOOK_DATA*);
	HAveRegisterPositionHook ptr = (HAveRegisterPositionHook)GetProcAddress(GetModuleHandle(0), "AveRegisterPositionHook");

	if(!ptr)
		return 0;

	return ptr(data);
}

BOOL CALLBACK AveUnregisterPositionHook(UINT id)
{
	typedef BOOL (CALLBACK *HAveUnregisterPositionHook)(UINT);
	HAveUnregisterPositionHook ptr = (HAveUnregisterPositionHook)GetProcAddress(GetModuleHandle(0), "AveUnregisterPositionHook");

	if(!ptr)
		return 0;

	return ptr(id);
}