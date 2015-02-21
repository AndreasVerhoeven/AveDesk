// AveInstProgressWindow.h: interface for the CAveInstProgressWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTPROGRESSWINDOW_H__840A82F4_6611_4F7E_B4AC_6F7B9D67176B__INCLUDED_)
#define AFX_AVEINSTPROGRESSWINDOW_H__840A82F4_6611_4F7E_B4AC_6F7B9D67176B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>

#include "resource.h"
#include "AveInstProgressIndicator.h"

#define WM_DESTROYME WM_USER + 666

class CAveInstProgressWindow  :
	//public CDialog,
	public CDialogImpl<CAveInstProgressWindow>,
	public CAveInstProgressIndicator
{
public:
	enum { IDD = IDD_INSTALLPROGRESS };

protected:
	std::wstring m_Message;
	long m_Progress;
	long m_Max;
	
	std::list<CAveInstCancelable*> m_Cancelables;

public:
	CAveInstProgressWindow();
	virtual ~CAveInstProgressWindow();

	BOOL SetupWindow(HWND parent);
	BOOL KillWindow();

	virtual void SetMessage(std::wstring msg);
	virtual void SetProgress(long progress, long max);

	virtual bool RegisterCancelable(CAveInstCancelable* c);
	virtual bool UnregisterCancelable(CAveInstCancelable* c);
	virtual void Hide();

BEGIN_MSG_MAP(CAveInstProgressWindow)
	// dialog stuff
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_DESTROYME,OnDestroyMe);
        
    // menu stuff
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	// other stuff
    REFLECT_NOTIFICATIONS()
	//CHAIN_MSG_MAP(CDialogImpl<CAveInstProgressWindow>)
END_MSG_MAP()
// Prototypen von Behandlungsroutinen:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	// dialog stuff
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroyMe(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

};

#endif // !defined(AFX_AVEINSTPROGRESSWINDOW_H__840A82F4_6611_4F7E_B4AC_6F7B9D67176B__INCLUDED_)
