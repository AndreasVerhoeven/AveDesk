// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__2416BD2D_6A1E_4E3D_953C_BE9561A1F791__INCLUDED_)
#define AFX_STDAFX_H__2416BD2D_6A1E_4E3D_953C_BE9561A1F791__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UNICODE

#define STRICT
#define _WIN32_WINNT 0x501
#define WINVER 0x0501
#define _CRT_SECURE_NO_DEPRECATE
//#pragma warning(disable:4503)
//#pragma warning(disable:4786)
//#pragma warning(disable:4996)
//#pragma warning(disable:4482)
//#pragma warning(disable:4244)
//#pragma warning(disable:4018)
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#define _ATL_APARTMENT_THREADED

#include <windows.h>
#include <WININET.H>
#include <Shlobj.h>

#include <boost/smart_ptr.hpp>

#include <atlcrack.h>
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
#include <atlapp.h> 
class CExeModule : public CComModule
{
public:
	CExeModule() : m_MainLoop(NULL), dwThreadID(0), hEventShutdown(NULL), bActivity(false), CComModule()
	{
	}

	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	void ChangeResourceInstance(LPCTSTR file);
	bool StartMonitor();
	bool bActivity;

	CMessageLoop* m_MainLoop;
};

extern CExeModule _Module;

#include <atlcrack.h>
#include <atlwin.h>

#include <atlapp.h> 
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <atlgdi.h>

#include <msxml2.h>
#pragma comment(lib,"msxml2.lib")

#include <atlcom.h>
#include <atltheme.h>
#include <gdiplus.h>
#include "LayeredWindowImpl.hpp"
#include <ocidl.h>	// Added by ClassView
using namespace Gdiplus;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2416BD2D_6A1E_4E3D_953C_BE9561A1F791__INCLUDED)
