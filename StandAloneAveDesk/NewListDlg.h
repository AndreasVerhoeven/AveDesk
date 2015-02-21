#pragma once


#include "resource.h"
#include "stdafx.h"
#include <vector>
#include "ToolTipImpl.h"
#include "DeskletFilter.h"
#include "FilterDlgControl.h"

class CFilterDlgControl;

class CNewListDlg : 
	public CDialogImpl<CNewListDlg>
{
public:
	enum {IDD = IDD_NEWLIST};

	static const UINT WM_USER_ICONFOUND = WM_USER + 405;

	HANDLE threadHandle;

	struct IconFindData
	{
		HWND callbackHwnd;
		UINT callbackMsg;
		WCHAR path[MAX_PATH];
		HANDLE threadHandle;
	};

	static DWORD CALLBACK EnumIconsThreadFunc(LPVOID lPData);
	void StartIconEnum();
	void StopIconEnum();


	BEGIN_TOOLTIP_MAP()
		//DEF_TOOLTIP(IDC_FILTERTYPE, IDS_FILTERTYPE)
	END_TOOLTIP_MAP()

	BEGIN_MSG_MAP(CNewListDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_USER_ADDFILTER, OnAddFilter)
		MESSAGE_HANDLER(WM_USER_REMOVEFILTER, OnRemoveFilter)
		MESSAGE_HANDLER(WM_USER_ICONFOUND, OnIconFound)
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)

		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAddFilter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRemoveFilter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnIconFound(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	std::vector<CFilterDlgControl*> filters;
	std::wstring name;
	std::wstring iconFileName;
	std::vector<CDeskletFilter> filterData;

	CComboBoxEx iconBox;
	CImageList imgList;

	void AddFilter(int index=-1, int type=0, const std::wstring& keyword=L"");
	void RemoveFilter(int index);

	CNewListDlg(void);
	~CNewListDlg(void);
};
