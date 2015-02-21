// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "toolsettings.h"

class CSelectDeskletFilesDlg : public CPropertyPageImpl<CSelectDeskletFilesDlg>
{
public:
	enum { IDD = IDD_SELECTFILESDLG };

	CShellTreeCtrl m_tree;

	ToolSettings* ts;

	std::vector<std::wstring> m_selectedFiles;

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(IDC_TREEFILES, TVN_CHECKEDSTATUS_CHANGED, OnSelChanged)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSelectDeskletFilesDlg>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnSelChanged(int /*idCtrl*/, 
                       LPNMHDR pnmh, 
                       BOOL& /*bHandled*/);
	
	void GetSelectedFiles(HTREEITEM hItem);
	BOOL HasSelectedFiles(HTREEITEM hItem);

	BOOL OnSetActive();
	int OnWizardNext();
};
