#pragma once

#include "DeskletStyles.h"
#include "DeskletHooks.h"

class HookApi
{
private:
	HookApi(void);
	~HookApi(void);

public:

#pragma pack(push, 1)
	struct DESKLET_STYLE_DATA
	{
		DWORD dwSize;				// sizeof(DESKLET_STYLE_DATA)
		void*	callbackPointer;// pointer used in callbacks
		const WCHAR* friendlyName;	// the friendly name (as displayed to the user) for this style
		const WCHAR* uniqueName;	// the unique name for this style
		DWORD flags;				// unused
		
		CDeskletStyles::HOnStyleChange	styleChangeCallback;	// style change callback function
		CDeskletStyles::HOnGetInsertHwnd insertHwndCallback;	// callback function for obtaining the insert hwnd
		CDeskletStyles::HOnMessage messageCallback;				// callback function for windows messages

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

		CPositionHookEntry::HOnPositionChange onPositionChangeCallback; // called when the position changes

		UINT_PTR* reserved1;//unused
		UINT_PTR* reserved2;//unused
		UINT_PTR* reserved3;//unused
		UINT_PTR* reserved4;//unused
		UINT_PTR* reserved5;//unused
		UINT_PTR* reserved6;//unused
	};

#pragma pack(pop)

	/**
	 * Registers a new desklet style.
	 *
	 * @param data: A pointer to a DESKLET_STYLE_DATA structure filled with information about the style.
	 * @return If successfull: the id of the style will be returned, otherwise 0 will be returned.
	 *
	 */
	static UINT CALLBACK AveRegisterDeskletStyle(const DESKLET_STYLE_DATA* data);

	/**
	 * Unregisters a previously registered desklet style.
	 *
	 * @param styleID: the id of the style to unregister, as previously has been obtained by
	 *					a call to AveRegisterDeskletStyle()
	 *
	 * @return: TRUE iff successfull.
	 *
	 */
	static BOOL CALLBACK AveUnregisterDeskletStyle(UINT styleID);


	/**
	 * Registers a new position hook.
	 *
	 * @param data: a pointer to a POSITION_HOOK_DATA* structure filled with information for the hook.
	 * @return: if successfull: the id of the hoook, otherwise 0.
	 *
	 */
	static UINT CALLBACK AveRegisterPositionHook(const POSITION_HOOK_DATA* data);

	/**
	 * Unregisters a previously registered position hook.
	 *
	 * @param id: the id of the hook to unregister, as previously has been obtained by
	 *					a call to AveRegisterPositionHook()
	 *
	 * @return: TRUE iff successfull.
	 *
	 */
	static BOOL CALLBACK AveUnregisterPositionHook(UINT id);
};
