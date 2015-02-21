// ShellFolder.h: interface for the CShellFolder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLFOLDER_H__34409689_1251_43AE_ABE8_02391266F2CA__INCLUDED_)
#define AFX_SHELLFOLDER_H__34409689_1251_43AE_ABE8_02391266F2CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER		0x0501
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0100



#pragma warning(disable: 4786)

#include <atlbase.h>

#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <shobjidl.h>
#include <string>
#pragma comment(lib,"shlwapi.lib")

class CShellFolder  
{
public:
	CComPtr<IShellFolder> m_ShellFolder;
	CComPtr<IShellFolder2> m_ShellFolder2;

	CShellFolder();
	virtual ~CShellFolder();

	HRESULT SetToDesktopFolder();
	HRESULT	SetFromShellFolder(LPSHELLFOLDER shellFolder);
	HRESULT SetFromPIDL(LPITEMIDLIST pidl);

	IShellFolder** operator&()
	{
		ATLASSERT(m_ShellFolder.p==NULL);
		return &m_ShellFolder;
	}


	HRESULT GetShellDetailString(LPCITEMIDLIST relativePidl, std::wstring& wStr, UINT num);

	bool GetPIDLFromRelativePath(const std::wstring& path, LPITEMIDLIST* pidl);
};

#endif // !defined(AFX_SHELLFOLDER_H__34409689_1251_43AE_ABE8_02391266F2CA__INCLUDED_)
