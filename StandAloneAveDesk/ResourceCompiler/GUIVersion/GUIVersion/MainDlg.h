// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "toolsettings.h"

class CMainDlg : public CPropertyPageImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	ToolSettings* ts;


	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BROWSEPROJECTDIR, BN_CLICKED, OnBnClickedBrowseprojectdir)
		COMMAND_HANDLER(IDC_NEWTOOLNAME, EN_CHANGE, OnEnChangeNewtoolname)
		COMMAND_HANDLER(IDC_PROJECTDIR, EN_CHANGE, OnEnChangeProjectdir)
		CHAIN_MSG_MAP(CPropertyPageImpl<CMainDlg>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void UpdateOutputDir();

	BOOL OnSetActive();
	int OnWizardNext();
public:
	LRESULT OnBnClickedBrowseprojectdir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnEnChangeNewtoolname(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnEnChangeProjectdir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
