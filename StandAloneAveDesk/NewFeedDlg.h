// NewFeedDlg.h: interface for the CNewFeedDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWFEEDDLG_H__FECF249B_7F0E_49F2_BF2D_37453C23BDE2__INCLUDED_)
#define AFX_NEWFEEDDLG_H__FECF249B_7F0E_49F2_BF2D_37453C23BDE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "resource.h"

#include "tooltipimpl.h"

class CNewFeedDlg  :
	public CDialogImpl<CNewFeedDlg>
{
public:
	CNewFeedDlg();
	~CNewFeedDlg();

	enum { IDD = IDD_NEWFEED };

	BEGIN_TOOLTIP_MAP()
		//DEF_TOOLTIP(IDC_ICONSLIDER, IDS_TOOLTIP_ICONSLIDER)
	END_TOOLTIP_MAP()

BEGIN_MSG_MAP(CNewFeedDlg)

	// dialog stuff
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	COMMAND_ID_HANDLER(IDOK, OnOkButton)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancelButton)

//	CHAIN_MSG_MAP(CDialogImpl<CNewFeedDlg>)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOkButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancelButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // !defined(AFX_NEWFEEDDLG_H__FECF249B_7F0E_49F2_BF2D_37453C23BDE2__INCLUDED_)
