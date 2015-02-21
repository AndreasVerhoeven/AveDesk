// SkinEnumerator.cpp: implementation of the CSkinEnumerator class.
//
//////////////////////////////////////////////////////////////////////

#include "SkinEnumerator.h"
#include "PidlShortcutDesklet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkinEnumerator::CSkinEnumerator(HINSTANCE hInstance, const WCHAR* deskletPath)
{
	CPidl pidl;
	pidl.SetFromPath(L"shell:recyclebinfolder");

	m_Skins[pidl.GetFriendlyName()] = L"shell:recyclebinfolder";
		
	pidl.SetFromPath(L"shell:drivefolder");
	m_Skins[pidl.GetFriendlyName()] = L"shell:drivefolder";
};

UINT CSkinEnumerator::size()const
{
	return m_Skins.size();
};

std::wstring CSkinEnumerator::getKey(UINT num)
{
	std::map<std::wstring,std::wstring>::iterator iter = m_Skins.begin();
	std::advance(iter,num);
	return iter->first;
};

std::wstring CSkinEnumerator::getValue(std::wstring key)
{
	return m_Skins[key];
};

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

	return AVEDESK_MORE_SKINS;
}

CPidlShortcutDesklet* __stdcall OnCreateFromSkinW(HWND hwnd,HINSTANCE hInstance, CSkinEnumerator* se, const SkinInfo* info)
{
	CPidlShortcutDesklet* desklet = OnCreate<CPidlShortcutDesklet>(hwnd,hInstance,NULL,NULL);
	if(se != NULL)
		desklet->UpdatePath( se->getValue(info->wKey) );
	else
		desklet->UpdatePath(info->wKey);

	desklet->SetAutoSetCaption(true);
	desklet->SetAutoSetCaption(false);

	return desklet;
}