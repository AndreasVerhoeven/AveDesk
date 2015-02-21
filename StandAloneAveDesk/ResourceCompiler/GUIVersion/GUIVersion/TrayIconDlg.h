#pragma once

#include "toolsettings.h"


// CTrayIconDlg dialog

class CTrayIconDlg : public CPropertyPageImpl<CTrayIconDlg>
{
public:
	enum { IDD = IDD_TRAYICONDLG };

	ToolSettings* ts;

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_TRAYICON, EN_CHANGE, OnEnChangeTrayicon)
		COMMAND_HANDLER(IDC_BROWSETRAYICON, BN_CLICKED, OnBnClickedBrowsetrayicon)
		CHAIN_MSG_MAP(CPropertyPageImpl<CTrayIconDlg>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void UpdateIconPath();

	BOOL OnSetActive();
	int OnWizardNext();
public:
	LRESULT OnEnChangeTrayicon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnBnClickedBrowsetrayicon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
