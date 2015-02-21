// Pidl.h: interface for the CPidl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIDL_H__80618FE5_FD41_4C89_8838_C0857E6DF69D__INCLUDED_)
#define AFX_PIDL_H__80618FE5_FD41_4C89_8838_C0857E6DF69D__INCLUDED_

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
#pragma comment(lib,"shlwapi.lib")

#include <string>
#include <memory>

	typedef struct {
		IContextMenu2*	pContextMenu2;
		IContextMenu3*	pContextMenu3;
		WNDPROC			pOldWndProc;
	} AVECMSUBCLASSDATA;

class CPidl  
{
public:
	CComPtr<IMalloc> m_Malloc;
	LPITEMIDLIST m_Pidl;

	CComPtr<IShellFolder>  m_ShellFolder;
	CComPtr<IShellFolder2> m_ShellFolder2;

	LPITEMIDLIST m_RelativePidl;

	LONG m_NotifyId;

protected:
	static std::wstring StringToSHITEMID(std::wstring s, SHITEMID* itemID);
	static std::wstring SHITEMIDToString(SHITEMID* itemID);

	static LRESULT CALLBACK SubclassMenuWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	std::wstring GetFriendlyName() const;
	std::wstring GetFullParseDisplayName()const;
	bool SetFromPath(const std::wstring& path);
	CPidl();
	~CPidl();

	operator LPITEMIDLIST() const;
	operator bool()const;
	bool IsValid()const;
	bool operator == (const LPITEMIDLIST& pidl)const;
	bool operator == (const CPidl& that)const;

	HRESULT GetAtrributesOf(ULONG* attr);

	HRESULT SetFromSpecialFolderLocation(int nFolder);

	void SetFromPIDL(LPITEMIDLIST pidl);

	HRESULT CacheParentShellFolder();
	HRESULT GetParentShellFolder(IShellFolder** folder,LPCITEMIDLIST* relativePidl=NULL, IShellFolder2** folder2=NULL)const;

	bool InvokeDefaultVerb(HWND hwnd, CMINVOKECOMMANDINFOEX& cmi);

	UINT TrackItemIDContextMenu(HMENU hAddMenu, UINT nFlags, const POINT& ptPoint, HWND hWnd, CMINVOKECOMMANDINFOEX& cmi);

	UINT GetItemIDCount() const;
	HRESULT CopyItemID(LPITEMIDLIST& pidl) const;
	UINT GetItemIDSize() const ;
	HRESULT GetRelativePidl(LPITEMIDLIST& pidl)const;

	HRESULT GetShellDetailString(std::wstring& str, UINT num);

	
	HICON ExtractIcon(UINT size, UINT flags = GIL_DEFAULTICON);
	HBITMAP ExtractThumb(const SIZE* prgSize, DWORD dwRecClrDepth);

	LPITEMIDLIST Detach();

	static LPITEMIDLIST Append(LPITEMIDLIST pidlBase, LPITEMIDLIST pidlAdd);


	void ClearCache();

	bool StartMonitoring(HWND hWnd, UINT msg, DWORD events = SHCNE_ALLEVENTS, BOOL recursive = TRUE);
	void StopMonitoring();

	HRESULT GetUIObject(HWND hWnd, REFIID riid, void** p);
	HRESULT GetIContextMenu(IContextMenu** menu, HWND hwnd);

	std::wstring ToString();
	HRESULT FromString(const std::wstring& str);

protected:
	void FreePidl(LPITEMIDLIST& pidl) const;
};

#endif // !defined(AFX_PIDL_H__80618FE5_FD41_4C89_8838_C0857E6DF69D__INCLUDED_)
