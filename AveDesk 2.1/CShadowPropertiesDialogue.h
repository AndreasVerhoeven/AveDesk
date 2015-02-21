// CShadowPropertiesDialogue.h : Declaration of the CCShadowPropertiesDialogue

#ifndef __CSHADOWPROPERTIESDIALOGUE_H_
#define __CSHADOWPROPERTIESDIALOGUE_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CCShadowPropertiesDialogue
class CCShadowPropertiesDialogue : 
	public CAxDialogImpl<CCShadowPropertiesDialogue>
{
public:
	CCShadowPropertiesDialogue()
	{
	}

	~CCShadowPropertiesDialogue()
	{
	}

	enum { IDD = IDD_CSHADOWPROPERTIESDIALOGUE };

BEGIN_MSG_MAP(CCShadowPropertiesDialogue)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};

#endif //__CSHADOWPROPERTIESDIALOGUE_H_
