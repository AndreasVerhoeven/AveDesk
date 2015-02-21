// AveDeskFileAssociation.cpp: implementation of the CAveDeskFileAssociation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveDeskFileAssociation.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveDeskFileAssociation::CAveDeskFileAssociation(const std::wstring& ext, 
													const std::wstring& cls,
													const std::wstring& icon,
													long iconIndex)
													:
	m_Extension(ext),
	m_Class(cls),
	m_Icon(icon),
	m_IconIndex(iconIndex)
{

}

CAveDeskFileAssociation::~CAveDeskFileAssociation()
{

}
void CAveDeskFileAssociation::SetExtension(const std::wstring& ext)
{
	m_Extension = ext;
}

void CAveDeskFileAssociation::SetClass(const std::wstring& cls)
{
	m_Class = cls;
}

void CAveDeskFileAssociation::SetIcon(const std::wstring& fileName, long index)
{
	m_Icon = fileName;
	m_IconIndex = index;
}

bool CAveDeskFileAssociation::IsSet()const
{	
	WCHAR text[MAX_PATH+1] = {0};
	DWORD type(0);
	DWORD bufSize(MAX_PATH);

	HKEY hKey = {0};

	if(RegOpenKeyEx(HKEY_CLASSES_ROOT,m_Extension.c_str(),0,KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
					
	if(RegQueryValueExW(hKey,L"",0,&type,(BYTE*)text,&bufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return false;
	}

	RegCloseKey(hKey);

	return _wcsicmp(m_Class.c_str(), text) == 0;

}

bool CAveDeskFileAssociation::SetAssociation()const
{  
   	WCHAR data[MAX_PATH+100] = {0};
	wcscpy(data, m_Class.c_str());
	HKEY hKey = {0};
	DWORD dummy(0L);
	if(RegCreateKeyExW(HKEY_CLASSES_ROOT,m_Extension.c_str(),0,NULL, REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS, NULL, &hKey, &dummy) != ERROR_SUCCESS)
		return false;

	DWORD len = (wcslen(data)+1)*sizeof(WCHAR);

	if(RegSetValueExW(hKey,L"",0,REG_SZ,(BYTE*)data,len) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return false;
	}

	RegCloseKey(hKey);

	std::wstring keyPath = m_Class + L"\\DefaultIcon";
	if(RegCreateKeyEx(HKEY_CLASSES_ROOT,keyPath.c_str(),0,NULL, REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS, NULL, &hKey, &dummy) != ERROR_SUCCESS)
		return false;

	std::wstringstream ws;

	WCHAR shortPath[MAX_PATH] = {0};
	GetShortPathName(m_Icon.c_str(), shortPath, MAX_PATH);
	ws << shortPath << L"," << m_IconIndex;
	wcscpy(data, ws.str().c_str());
	len = (wcslen(data)+1)*sizeof(WCHAR);

	if(RegSetValueExW(hKey,L"",0,REG_SZ,(BYTE*)data,len) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return false;
	}

	RegCloseKey(hKey);

	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);


	return true;
}
