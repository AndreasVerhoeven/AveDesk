// NewFeedDlg.cpp: implementation of the CNewFeedDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewFeedDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewFeedDlg::CNewFeedDlg()
{

}

CNewFeedDlg::~CNewFeedDlg()
{

}

LRESULT CNewFeedDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    HICON hIco = LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE( IDI_ICON2 ));
    SetIcon( hIco, TRUE );
    SetIcon( hIco, FALSE );
    CenterWindow();

	CComboBox box = GetDlgItem(IDC_FEEDUPDATEINTERVAL);
	CString str;
	str.LoadString(IDS_UPDATE_INTERVAL0);
	box.AddString(str);
	str.LoadString(IDS_UPDATE_INTERVAL1);
	box.AddString(str);
	str.LoadString(IDS_UPDATE_INTERVAL2);
	box.AddString(str);
	str.LoadString(IDS_UPDATE_INTERVAL3);
	box.AddString(str);
	str.LoadString(IDS_UPDATE_INTERVAL4);
	box.AddString(str);
	str.LoadString(IDS_UPDATE_INTERVAL5);
	box.AddString(str);
	box.SetCurSel(4); // 4 == every day

	ToolTips_Init();
	return 0;
}

LRESULT CNewFeedDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CNewFeedDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog(FALSE);
	return 0;
}
LRESULT CNewFeedDlg::OnOkButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(TRUE);
	return 0;
}

LRESULT CNewFeedDlg::OnCancelButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(FALSE);
	return 0;
}