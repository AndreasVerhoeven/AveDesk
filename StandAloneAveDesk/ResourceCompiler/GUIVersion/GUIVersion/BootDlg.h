// MainDlg.h : interface of the CBootDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "toolsettings.h"

class CBootDlg : public CPropertyPageImpl<CBootDlg>
{
public:
	enum { IDD = IDD_BOOTDLG };

	ToolSettings* ts;


	BEGIN_MSG_MAP(CBootDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BROWSEBOOTFILE, BN_CLICKED, OnBnClickedBrowsebootfile)
		COMMAND_HANDLER(IDC_BROWSESETTINGSFILE, BN_CLICKED, OnBnClickedBrowsesettingsfile)
		CHAIN_MSG_MAP(CPropertyPageImpl<CBootDlg>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BOOL OnSetActive();
	int OnWizardNext();
public:
	LRESULT OnBnClickedBrowsebootfile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnBnClickedBrowsesettingsfile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
