// Pidl.h: interface for the CPidl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIDL_H__80618FE5_FD41_4C89_8838_C0857E6DF69D__INCLUDED_)
#define AFX_PIDL_H__80618FE5_FD41_4C89_8838_C0857E6DF69D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER		0x0500
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
private:
	CComPtr<IMalloc> m_Malloc;
	LPITEMIDLIST m_Pidl;

	CComPtr<IShellFolder>  m_ShellFolder;
	CComPtr<IShellFolder2> m_ShellFolder2;

	LPITEMIDLIST m_RelativePidl;

	LONG m_NotifyId;

	static LRESULT CALLBACK SubclassMenuWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT GetIContextMenu(IContextMenu** menu, HWND hwnd);

	void ClearCache();

public:
	std::wstring GetFriendlyName() const;
	std::wstring GetFullParseDisplayName()const;
	bool SetFromPath(const std::wstring& path);
	CPidl();
	virtual ~CPidl();

	operator LPITEMIDLIST() const
	{
		return m_Pidl;
	}

	operator bool()const
	{
		return IsValid();
	}

	bool IsValid()const
	{
		return m_Pidl != NULL;
	}

	bool operator == (const LPITEMIDLIST& pidl)const
	{
		CComPtr<IShellFolder> shellFolder;
		if( FAILED( SHGetDesktopFolder(&shellFolder)) )
			return false;

		return shellFolder->CompareIDs(SHCIDS_CANONICALONLY,m_Pidl, pidl) == 0;

	}

	bool operator == (const CPidl& that)const
	{
		return operator==(that.m_Pidl);
	}

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

	LPITEMIDLIST Detach()
	{
		LPITEMIDLIST prev = m_Pidl;
		m_Pidl = NULL;
		if(m_ShellFolder.p)m_ShellFolder.Release();
		if(m_ShellFolder2.p)m_ShellFolder2.Release();
		m_RelativePidl = NULL;

		return prev;
	}

	bool StartMonitoring(HWND hWnd, UINT msg, DWORD events = SHCNE_ALLEVENTS, BOOL recursive = TRUE);
	void StopMonitoring();

protected:
	void FreePidl(LPITEMIDLIST& pidl) const;
};

#endif // !defined(AFX_PIDL_H__80618FE5_FD41_4C89_8838_C0857E6DF69D__INCLUDED_)
