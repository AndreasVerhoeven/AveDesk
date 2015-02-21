#include "SkinEnumerator.h"
#include "../../../DeskletSDK.h"
#include <shlwapi.h>

CSkinEnumerator* __stdcall OnCreateSkinEnumeratorW(HINSTANCE hInstance, const WCHAR* deskletPath)
{
	return new CSkinEnumerator(hInstance, deskletPath);
}

void __stdcall OnDestroySkinEnumeratorW(CSkinEnumerator* se)
{
	delete se;
}

INT __stdcall OnGetSkinW(CSkinEnumerator* se,SkinInfo* info)
{
	if(info->num >= se->size() )
		return AVEDESK_NO_MORE_SKINS;

	std::wstring skin = se->getKey(info->num);

	if(info->maxKeyLength < skin.length()+1 || info->maxNameLength < skin.length()+1)
	{
		info->maxKeyLength = skin.length() + 1;	
		info->maxNameLength = skin.length() + 1;
		return AVEDESK_BUF_TOO_SMALL;
	}

	wcscpy(info->wKey,skin.c_str());
	wcscpy(info->wName,skin.c_str());


	WCHAR ini[MAX_PATH];
	wcscpy(ini,se->GetSkinsPath().c_str());
	PathAppendW(ini,se->getValue(skin).c_str());
	PathAppendW(ini,L"skin.ini");

	GetPrivateProfileStringW(L"About",L"Name",skin.c_str(),info->wName,info->maxNameLength,ini);
	GetPrivateProfileStringW(L"About",L"Author",L"Unknown",info->wAuthor,info->maxAuthorLength,ini);
	GetPrivateProfileStringW(L"About",L"Comment",L"",info->wNotes,info->maxNotesLength,ini);
	info->version = GetPrivateProfileIntW(L"Info",L"Version",130,ini);

	return AVEDESK_MORE_SKINS;
}
