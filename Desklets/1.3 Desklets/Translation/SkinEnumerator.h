#ifndef SKIN_ENUMERATOR_H___
#define SKIN_ENUMERATOR_H___

#pragma once

#include <windows.h>
#include <map>
#include <string>
#include <shlwapi.h>

class CSkinEnumerator
{
private:
	WCHAR skinsPath[MAX_PATH];
	std::map<std::wstring,std::wstring> m_Skins;

public:
	CSkinEnumerator(HINSTANCE hInstance, const WCHAR* deskletPath)
	{
		WCHAR path[MAX_PATH] = {0};
		wcscpy_s(path, MAX_PATH, deskletPath);
		PathAppendW(path,L"skins\\");

		wcscpy_s(skinsPath, MAX_PATH, path);

		std::wstring s = path;
		s += L"*.*";

		WIN32_FIND_DATAW data = {0};
		HANDLE h = FindFirstFileW(s.c_str(),&data);
		if(h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(wcscmp(data.cFileName,L".")!=0 && wcscmp(data.cFileName,L"..")!=0 && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					WCHAR ini[MAX_PATH];
					wcscpy_s(ini, MAX_PATH, skinsPath);
					PathAppendW(ini,data.cFileName);
					PathAppendW(ini,L"skin.ini");

					WCHAR type[MAX_PATH] = {0};
					GetPrivateProfileStringW(L"About",L"Type",L"",type, MAX_PATH,ini);
					if(_wcsicmp(type, L"Translator") == 0)
						m_Skins[data.cFileName] = data.cFileName;
				}

			}while(FindNextFileW(h,&data));
			FindClose(h);
		}
	};

	UINT size()const { return m_Skins.size();};

	std::wstring getKey(UINT num)
	{
		std::map<std::wstring,std::wstring>::iterator iter = m_Skins.begin();
		std::advance(iter,num);
		//iter = num;
		return iter->first;
	};

	std::wstring getValue(std::wstring key)
	{
		return m_Skins[key];
	};

	std::wstring GetSkinsPath()const
	{
		return skinsPath;
	}
};

#endif//SKIN_ENUMERATOR_H___
