// MainDlg.h : interface of the CVersionDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "toolsettings.h"
#include "PropertyList.h"

class CVersionDlg : public CPropertyPageImpl<CVersionDlg>
{
public:
	enum { IDD = IDD_VERSIONDLG };

	ToolSettings* ts;

	CPropertyListCtrl m_list;


	BEGIN_MSG_MAP(CVersionDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CPropertyPageImpl<CVersionDlg>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BOOL OnSetActive();
	int OnWizardNext();
};
