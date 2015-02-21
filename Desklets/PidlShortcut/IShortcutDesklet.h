// IShortcutDesklet.h: interface for the IShortcutDesklet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISHORTCUTDESKLET_H__B2AE3612_A14E_40EA_8752_53FFEB91F447__INCLUDED_)
#define AFX_ISHORTCUTDESKLET_H__B2AE3612_A14E_40EA_8752_53FFEB91F447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Pidl.h"

#include <windows.h>

#include <string>

class IShortcutDesklet  
{
public:
	IShortcutDesklet();
	~IShortcutDesklet();

	virtual void NotifySublabelChange() = 0;
	virtual void NotifyIconChange() = 0;

	virtual HWND GetHWnd() = 0;

	virtual CPidl& GetPidl() = 0;

	virtual void ImageFadeIn(std::wstring keyName) = 0;
	virtual void ImageFadeOut() = 0;

	virtual HINSTANCE GetInstanceHandle() = 0;
};

#endif // !defined(AFX_ISHORTCUTDESKLET_H__B2AE3612_A14E_40EA_8752_53FFEB91F447__INCLUDED_)
