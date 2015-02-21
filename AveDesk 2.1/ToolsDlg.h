// ToolsDlg.h : Deklaration von CToolsDlg

#ifndef __TOOLSDLG_H_
#define __TOOLSDLG_H_

#include "resource.h"       // Hauptsymbole
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CToolsDlg
class CToolsDlg : 
	public CAxDialogImpl<CToolsDlg>
{
public:
	CToolsDlg()
	{
	}

	~CToolsDlg()
	{
	}

	enum { IDD = IDD_TOOLSDLG };

BEGIN_MSG_MAP(CToolsDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Prototypen von Behandlungsroutinen:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;  // Lassen Sie das System den Fokus setzen
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

#endif //__TOOLSDLG_H_
