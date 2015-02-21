#pragma once

#include <string>
#include <map>

class CDeskletStyles
{
public:

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

#pragma pack(pop)

protected:

	struct DeskletStyleData
	{
		std::wstring friendlyName;
		std::wstring uniqueName;
		
		HOnStyleChange callback;
		HOnGetInsertHwnd callbackGetHwnd;
		HOnMessage		callbackOnMessage;

		void* callbackPointer;
	};

	std::map<UINT, DeskletStyleData> data;

	static CDeskletStyles* m_Instance;

public:

	CDeskletStyles(void);
	~CDeskletStyles(void);


	std::map<std::wstring, UINT> GetStyles();
	std::wstring GetUniqueName(UINT id);
	UINT GetIDForUniqueName(const WCHAR* uniqueName);

	UINT AddStyle(const WCHAR* friendlyName, const WCHAR* uniqueName, void* ptr, HOnStyleChange callback, HOnGetInsertHwnd callback2, HOnMessage callback3);
	BOOL RemoveStyle(UINT id);

	BOOL OnStyleChange(const DESKLET_STYLE_CHANGE_DATA* dscd);
	BOOL OnGetInsertHwnd(DESKLET_STYLE_CHANGE_DATA* dscd);
	UINT_PTR OnMessage(DESKLET_STYLE_CHANGE_DATA* dscd, MSG* msg);

	static CDeskletStyles* GetInstance();

};
