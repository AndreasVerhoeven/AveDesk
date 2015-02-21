// InstallErrorDlg.h: interface for the CInstallErrorDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTALLERRORDLG_H__33C69218_0458_4EB6_B7B6_FA4378C6E0AA__INCLUDED_)
#define AFX_INSTALLERRORDLG_H__33C69218_0458_4EB6_B7B6_FA4378C6E0AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstError.h"
#include  "resource.h"

class CInstallErrorDlg :
	public CDialogImpl<CInstallErrorDlg> 
{
protected:
	CAveInstError m_Error;
public:
	CInstallErrorDlg(const CAveInstError& error);
	virtual ~CInstallErrorDlg();

	enum { IDD = IDD_INSTALLERROR };

	CHyperLink m_ErrorLink;
	CStatic m_Icon;

	BEGIN_MSG_MAP(CInstallErrorDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_KEYDOWN, OnChar)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		COMMAND_ID_HANDLER(IDOK, OnOKButton)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOKButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

};

#endif // !defined(AFX_INSTALLERRORDLG_H__33C69218_0458_4EB6_B7B6_FA4378C6E0AA__INCLUDED_)
