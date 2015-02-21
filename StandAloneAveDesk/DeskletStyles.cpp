#include "stdafx.h"
#include "DeskletStyles.h"
#include "constants.h"

CDeskletStyles* CDeskletStyles::m_Instance = new CDeskletStyles;

CDeskletStyles* CDeskletStyles::GetInstance()
{
	return m_Instance;
}

CDeskletStyles::CDeskletStyles(void)
{
}

CDeskletStyles::~CDeskletStyles(void)
{
}


std::map<std::wstring, UINT> CDeskletStyles::GetStyles()
{
	std::map<std::wstring, UINT> styles;

	CString style;
	style.LoadString(IDS_STYLE0);

	styles[(const WCHAR*)style] = STYLE_NORMAL;
	
	style.LoadString(IDS_STYLE1);
	styles[(const WCHAR*)style] = STYLE_BOTTOM;

	style.LoadString(IDS_STYLE2);
	styles[(const WCHAR*)style] = STYLE_TOP;

	style.LoadString(IDS_STYLE3);
	styles[(const WCHAR*)style] = STYLE_ICON;

	style.LoadString(IDS_STYLE4);
	styles[(const WCHAR*)style] = STYLE_SHOWCASE;

	for(std::map<UINT, DeskletStyleData>::iterator iter = data.begin(); iter != data.end(); ++iter)
		styles[iter->second.friendlyName] = iter->first;

	return styles;
}

UINT CDeskletStyles::GetIDForUniqueName(const WCHAR* uniqueName)
{
	if(NULL == uniqueName)
		return 0;

	if(_wcsicmp(uniqueName, L"normal") == 0)
		return STYLE_NORMAL;
	else if(_wcsicmp(uniqueName, L"bottom") == 0)
		return STYLE_BOTTOM;
	else if(_wcsicmp(uniqueName, L"top") == 0)
		return STYLE_TOP;
	else if(_wcsicmp(uniqueName, L"icon") == 0)
		return STYLE_ICON;
	else if(_wcsicmp(uniqueName, L"showcase") == 0)
		return STYLE_SHOWCASE;
	else
	{
		for(std::map<UINT, DeskletStyleData>::iterator iter = data.begin(); iter != data.end(); ++iter)
		{
			if(_wcsicmp(iter->second.uniqueName.c_str(), uniqueName) == 0)
				return iter->first;
		}
	}


	return 0;
}


std::wstring CDeskletStyles::GetUniqueName(UINT id)
{
	switch(id)
	{
	// constants for desklets style
			case STYLE_NORMAL:
				return L"normal";
			case STYLE_BOTTOM:
				return L"bottom";
			case STYLE_TOP:
				return L"top";
			case STYLE_ICON:
				return L"icon";
			case STYLE_SHOWCASE:
				return L"showcase";

			default:
				if(data.find(id) != data.end())
					return data[id].uniqueName;
	}

	return L"";
}

UINT CDeskletStyles::AddStyle(const WCHAR* friendlyName, const WCHAR* uniqueName, void* ptr, HOnStyleChange callback,
							  HOnGetInsertHwnd callback2, HOnMessage callback3)
{
	if(NULL == friendlyName || NULL == uniqueName || NULL == callback)
		return 0;

	UINT newId = STYLE_SHOWCASE + 20; // reserve 20 entries for our usage
	for(std::map<UINT, DeskletStyleData>::iterator iter = data.begin(); iter != data.end(); ++iter)
	{
		const DeskletStyleData& dsd = iter->second;
		UINT id = iter->first;
		newId = id + 1;

		if(_wcsicmp(dsd.uniqueName.c_str(), uniqueName) == 0)
			return 0;
	}

	DeskletStyleData dsd;
	dsd.callbackPointer = ptr;
	dsd.friendlyName = friendlyName;
	dsd.uniqueName = uniqueName;
	dsd.callback = callback;
	dsd.callbackGetHwnd = callback2;
	dsd.callbackOnMessage = callback3;
	data[newId] = dsd;

	return newId;
}

BOOL CDeskletStyles::RemoveStyle(UINT id)
{
	if(data.find(id) != data.end())
	{
		data.erase(id);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDeskletStyles::OnStyleChange(const CDeskletStyles::DESKLET_STYLE_CHANGE_DATA* dscd)
{
	if(NULL == dscd)
		return FALSE;

	UINT oldId = dscd->oldStyle;
	UINT newId = dscd->newStyle;

	BOOL oldRes = FALSE;
	BOOL newRes = TRUE;

	if(data.find(oldId) != data.end())
		oldRes = data[oldId].callback(data[oldId].callbackPointer, dscd);

	if(data.find(newId) != data.end())
		newRes = data[newId].callback(data[newId].callbackPointer, dscd);

	return oldRes && newRes;
}

BOOL CDeskletStyles::OnGetInsertHwnd(DESKLET_STYLE_CHANGE_DATA* dscd)
{
	if(NULL == dscd)
		return FALSE;

	UINT newId = dscd->newStyle;

	UINT res = {0};
	if(data.find(newId) != data.end())
		res = data[newId].callbackGetHwnd(data[newId].callbackPointer, dscd);

	return res;
}

UINT_PTR CDeskletStyles::OnMessage(DESKLET_STYLE_CHANGE_DATA* dscd, MSG* msg)
{
	if(NULL == dscd)
		return FALSE;

	UINT newId = dscd->newStyle;

	UINT res = {0};
	if(data.find(newId) != data.end())
		res = data[newId].callbackOnMessage(data[newId].callbackPointer, dscd, msg);

	return res;
}