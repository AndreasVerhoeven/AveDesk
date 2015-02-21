#pragma once

#include "resource.h"
#include "stdafx.h"
#include "TooltipImpl.h"
#include <string>
#include "DeskletFilter.h"

#define WM_USER_ADDFILTER    WM_USER + 344
#define WM_USER_REMOVEFILTER WM_USER + 345

class CFilterDlgControl : public CDialogImpl<CFilterDlgControl>
{
public:
	enum {IDD = IDD_FILTER};

	CComboBox box;

	int boxSel;
	std::wstring keyword;


	BEGIN_TOOLTIP_MAP()
		DEF_TOOLTIP(IDC_FILTERTYPE, IDS_FILTERTYPE)
		DEF_TOOLTIP(IDC_KEYWORD, IDS_KEYWORD)
		DEF_TOOLTIP(IDC_REMOVEFILTER, IDS_REMOVEFILTER)
		DEF_TOOLTIP(IDC_ADDFILTER, IDS_ADDFILTER)
	END_TOOLTIP_MAP()

	BEGIN_MSG_MAP(CFilterDlgControl)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		COMMAND_ID_HANDLER(IDC_ADDFILTER, OnAddFilter)
		COMMAND_ID_HANDLER(IDC_REMOVEFILTER, OnRemoveFilter)

		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnAddFilter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRemoveFilter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	int MapFilterToBox(CDeskletFilter::FilterType f);
	CDeskletFilter::FilterType MapBoxToFilter(int sel);

public:

	CFilterDlgControl(void);
	~CFilterDlgControl(void);
};
