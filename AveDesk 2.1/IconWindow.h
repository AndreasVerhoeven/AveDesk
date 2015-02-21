// IconWindow.h: interface for the CIconWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONWINDOW_H__18EF0EC2_4F72_497C_BE4A_76DA91C6174B__INCLUDED_)
#define AFX_ICONWINDOW_H__18EF0EC2_4F72_497C_BE4A_76DA91C6174B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <string>
#include <vector>
#include "constants.h"

const UINT WM_TASKBARCREATED = 
    ::RegisterWindowMessage(_T("TaskbarCreated"));

class CIconWindow : public CWindowImpl<CIconWindow>
{
protected:
	std::vector<std::wstring> m_filenames;
public:

	NOTIFYICONDATAW _ncd;

	CMenu popupMenu;
	bool themesAlreadyFilled;

	CIconWindow();
	~CIconWindow();

	DECLARE_WND_SUPERCLASS(_T("AveDeskMainWindowClass"), GetWndClassName()) 

	BEGIN_MSG_MAP(CIconWindow)

		MESSAGE_HANDLER(WM_TASKBARCREATED, OnTaskbarCreated)
		MESSAGE_HANDLER(WM_AVELOADMODULE, OnAveLoadModule)
		MESSAGE_HANDLER(WM_AVEREMOVEMODULE, OnAveRemoveModule)
		MESSAGE_HANDLER(WM_AVECLEARMODULES, OnAveClearModules)
		MESSAGE_HANDLER(WM_QUERYENDSESSION, OnQueryEndSession)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
		MESSAGE_HANDLER(ICON_CALLBACK, OnIconCallback)

	END_MSG_MAP()

	void CleanUpPhysicalMemUsage();


	LRESULT OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAveLoadModule(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAveRemoveModule(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAveClearModules(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnQueryEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnIconCallback(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnTrayIconLeftButtonDoubleClick();
	void OnTrayIconRightButtonClick();

	void MakeIcon();
	void KillIcon();
};

#endif // !defined(AFX_ICONWINDOW_H__18EF0EC2_4F72_497C_BE4A_76DA91C6174B__INCLUDED_)