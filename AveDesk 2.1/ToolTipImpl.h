// ToolTipImpl.h: interface for the CToolTipImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPIMPL_H__B180C413_D3FB_480B_856C_493350E7578C__INCLUDED_)
#define AFX_TOOLTIPIMPL_H__B180C413_D3FB_480B_856C_493350E7578C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"


#define BEGIN_TOOLTIP_MAP()								\
	CToolTip m_tt;										\
	void ToolTips_Init()								\
	{													\
		if(!m_tt.IsToolTip())							\
			m_tt.Create(m_hWnd);						\
		CString str;
							


#define BEGIN_TOOLTIP_MAP_NOVAR()						\
	void ToolTips_Init()								\
	{													\
		if(!m_tt.IsToolTip())							\
			m_tt.Create(m_hWnd);						\
		CString str;		


#define DEF_TOOLTIP(ctrl, ids)		\
			str.LoadString(ids);	\
			m_tt.AddTip(GetDlgItem(ctrl), const_cast<LPTSTR>((LPCTSTR)str));

#define DEF_TOOLTIP_STR(ctrl, strng)	\
			m_tt.AddTip(GetDlgItem(ctrl), strng);


#define END_TOOLTIP_MAP() }


#define USE_TOOLTIPS_INIT() ToolTips_Init()
#define USE_TOOLTIPS_DESTROY() m_tt.Destroy();

template<DWORD styles = WS_POPUP | TTS_NOPREFIX, DWORD flags = TTF_SUBCLASS | TTF_IDISHWND> class CToolTipImpl  
{
public:
	HWND m_hWnd;
	HWND m_parenthWnd;
	CToolTipImpl() : m_hWnd(NULL), m_parenthWnd(NULL)
	{

	}


	~CToolTipImpl()
	{
		Destroy();
	}

	BOOL IsToolTip()const
	{
		return ::IsWindow(m_hWnd);
	}

	void Create(HWND hwndParent)
	{
		if(IsToolTip())
			Destroy();

		ATLASSERT(::IsWindow(hwndParent));
		m_parenthWnd = hwndParent;

		m_hWnd = ::CreateWindowEx(WS_EX_TOPMOST,
        TOOLTIPS_CLASS,
        NULL,
        styles,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        hwndParent,
        NULL,
        _Module.GetModuleInstance(),
        NULL
        );


	::SetWindowPos(m_hWnd,
        HWND_TOPMOST,
        0,
        0,
        0,
        0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	void Destroy()
	{
		if(::IsWindow(m_hWnd))
			::DestroyWindow(m_hWnd);

		m_hWnd = NULL;
		m_parenthWnd = NULL;
	}


	BOOL AddTip(HWND hwndControl, LPTSTR str)
	{
		ATLASSERT(IsToolTip());
		ATLASSERT(::IsWindow(hwndControl));

		TOOLINFO ti = {0};
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = flags;
		ti.hwnd = m_parenthWnd;
		ti.hinst = _Module.GetModuleInstance();
		ti.uId =  (UINT_PTR)hwndControl; // uid
		ti.lpszText = str;
		::GetWindowRect(hwndControl, &ti.rect);

		LRESULT res = ::SendMessage(m_hWnd, TTM_ADDTOOL, 0L, (LPARAM) (LPTOOLINFO) &ti);
		return res != FALSE;
	}

	BOOL DeleteTip(HWND hwndControl)
	{
		ATLASSERT(IsToolTip());
		ATLASSERT(::IsWindow(hwndControl));

		TOOLINFO ti = {0};
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = flags;
		ti.hwnd = m_parenthWnd;
		ti.hinst = _Module.GetModuleInstance();
		ti.uId =  (UINT_PTR)hwndControl; // uid

		LRESULT res = ::SendMessage(m_hWnd, TTM_DELTOOL, 0L, (LPARAM) (LPTOOLINFO) &ti);
		return res != FALSE;

	}

};

typedef CToolTipImpl<> CToolTip;

#endif // !defined(AFX_TOOLTIPIMPL_H__B180C413_D3FB_480B_856C_493350E7578C__INCLUDED_)
