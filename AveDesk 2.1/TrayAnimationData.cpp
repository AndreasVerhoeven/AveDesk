// TrayAnimationData.cpp: implementation of the CTrayAnimationData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrayAnimationData.h"

#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayAnimationData::CTrayAnimationData() : 
	m_NumberOfRevolutionsNeededToRestore(0),
	m_InitialSpeedInMs(50),
	m_MaximumSpeedInMs(25)
{

}

CTrayAnimationData::~CTrayAnimationData()
{
	for(std::vector<HICON>::iterator iter = m_Icons.begin(); iter != m_Icons.end(); ++iter)
		DestroyIcon(*iter);

	m_Icons.clear();
}

HICON CTrayAnimationData::GetIcon(UINT index)
{
	if(index < m_Icons.size())
		return m_Icons[index];
	else
		return NULL;
}

const std::wstring& CTrayAnimationData::GetFolderName()const
{
	return m_FolderName;
}

CTrayAnimationData* CTrayAnimationData::LoadFromFolder(const WCHAR* folder, const WCHAR* folderName)
{
	if(folder == NULL || folderName == NULL)
		return NULL;

	WCHAR iniPath[MAX_PATH] = {0};
	wcscpy(iniPath,folder);
	PathAppendW(iniPath,L"animation.ini");

	UINT numberOfIcons = GetPrivateProfileIntW(L"INFO", L"NumberOfIcons", 0, iniPath);
	if(numberOfIcons == 0)
		return NULL; // no icons, no animation!


	CTrayAnimationData* data = new CTrayAnimationData;

	data->m_FolderName = folderName;

	data->m_InitialSpeedInMs = GetPrivateProfileIntW(L"INFO", L"InitialSpeed", data->m_InitialSpeedInMs, iniPath);
	data->m_MaximumSpeedInMs = GetPrivateProfileIntW(L"INFO", L"MaximumSpeedInMs", data->m_MaximumSpeedInMs, iniPath);
	data->m_NumberOfRevolutionsNeededToRestore  = GetPrivateProfileIntW(L"INFO", L"NumberOfRevolutionsNeededToRestore", data->m_NumberOfRevolutionsNeededToRestore, iniPath);


	WCHAR author[1024] = {0};
	GetPrivateProfileStringW(L"DESCRIPTION",L"Author",L"",author,sizeof(author) / sizeof(author[0]),iniPath);
	data->m_Author = author;

	WCHAR comments[1024] = {0};
	GetPrivateProfileStringW(L"DESCRIPTION",L"Comments",L"",comments,sizeof(comments) / sizeof(comments[0]),iniPath);
	data->m_Comments = comments;

	for(UINT i = 0; i < numberOfIcons; ++i)
	{
		WCHAR iconPath[MAX_PATH]  = {0};
		wcscpy(iconPath, folder);

		std::wstringstream ws;
		ws << (i+i) << L".ico";

		PathAppendW(iconPath, ws.str().c_str());

		// Tray icons are always 16 x 16
		HICON icon = reinterpret_cast<HICON>(::LoadImage(_Module.GetModuleInstance(),iconPath,IMAGE_ICON, 16, 16,LR_LOADFROMFILE | LR_DEFAULTCOLOR));

		if(icon != NULL)
			data->m_Icons.push_back(icon);
	}

	return data;

}

UINT CTrayAnimationData::GetNumberOfIcons()const
{
	return m_Icons.size();
}

const std::wstring& CTrayAnimationData::GetAuthor()const
{
	return m_Author;
}

const std::wstring& CTrayAnimationData::GetComments()const
{
	return m_Comments;
}

UINT CTrayAnimationData::GetInitialSpeedInMs()const
{
	return m_InitialSpeedInMs;
}

UINT CTrayAnimationData::GetMaximumSpeedInMs()const
{
	return m_MaximumSpeedInMs;
}

UINT CTrayAnimationData::GetNumberOfRevolutionsNeededToRestore()const
{
	return m_NumberOfRevolutionsNeededToRestore;
}