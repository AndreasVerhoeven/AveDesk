#include "stdafx.h"
#include "HookApi.h"
#include "DeskletStyles.h"

HookApi::HookApi(void)
{
}

HookApi::~HookApi(void)
{
}


UINT CALLBACK HookApi::AveRegisterDeskletStyle(const DESKLET_STYLE_DATA* data)
{
	if(NULL == data)
		return 0;

	if(data->dwSize < sizeof(DESKLET_STYLE_DATA))
		return 0;

	if(NULL == data->friendlyName || NULL == data->uniqueName)
		return 0;

	CDeskletStyles* styles = CDeskletStyles::GetInstance();
	if(NULL == styles)
		return 0;

	return styles->AddStyle(data->friendlyName, data->uniqueName, data->callbackPointer, data->styleChangeCallback, 
				data->insertHwndCallback, data->messageCallback);
}

BOOL CALLBACK HookApi::AveUnregisterDeskletStyle(UINT styleID)
{
	CDeskletStyles* styles = CDeskletStyles::GetInstance();
	if(NULL == styles)
		return FALSE;

	return styles->RemoveStyle(styleID);
}


UINT CALLBACK HookApi::AveRegisterPositionHook(const POSITION_HOOK_DATA* data)
{
	if(NULL == data)
		return 0;

	if(data->dwSize < sizeof(POSITION_HOOK_DATA))
		return 0;

	CDeskletHooks* hooks = CDeskletHooks::GetInstance();
	if(hooks != NULL)
		return hooks->AddPositionHook(data->callbackPointer, data->onPositionChangeCallback);
	else
		return 0;
}

BOOL CALLBACK HookApi::AveUnregisterPositionHook(UINT id)
{
	CDeskletHooks* hooks = CDeskletHooks::GetInstance();
	if(hooks != NULL)
		return hooks->RemovePositionHook(id);
	else
		return 0;
}