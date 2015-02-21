// SkinEnumerator.h: interface for the CSkinEnumerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKINENUMERATOR_H__55AE6158_D065_4437_B548_8E30EB0A41B0__INCLUDED_)
#define AFX_SKINENUMERATOR_H__55AE6158_D065_4437_B548_8E30EB0A41B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <map>
#include <string>

class CSkinEnumerator
{
private:
	std::map<std::wstring,std::wstring> m_Skins;

public:
	CSkinEnumerator(HINSTANCE hInstance, const WCHAR* deskletPath);
	UINT size()const;
	std::wstring getKey(UINT num);
	std::wstring getValue(std::wstring key);
};

#endif // !defined(AFX_SKINENUMERATOR_H__55AE6158_D065_4437_B548_8E30EB0A41B0__INCLUDED_)
