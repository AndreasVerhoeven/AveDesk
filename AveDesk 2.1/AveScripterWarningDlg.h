#pragma once

#include "resource.h"
#include "ToolTipImpl.h"
#include "AsyncDownloader.h"

#define WM_USER_NOTIFY_ADD_DIR WM_USER + 498

class CAveScripterWarningDlg :
		public CDialogImpl<CAveScripterWarningDlg>
{
public:
	enum {IDD = IDD_AVESCRIPTER_WARNING};

	HWND hwndListBox;
	CHyperLink readMoreLink;
	CFont captionFont;
	CStatic captionStatic;
	CProgressBarCtrl progressBar;


	BEGIN_TOOLTIP_MAP()
		//DEF_TOOLTIP(IDC_FILTERTYPE, IDS_FILTERTYPE)
	END_TOOLTIP_MAP()

	BEGIN_MSG_MAP(CAveScripterWarningDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_USER_PROGRESS_CALLBACK, OnProgressCallback)

		COMMAND_ID_HANDLER(IDC_READMORE, OnReadMoreLink)
		COMMAND_ID_HANDLER(IDC_DOWNLOAD, OnDownloadButton)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnProgressCallback(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnReadMoreLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDownloadButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	CAveScripterWarningDlg(void);
	~CAveScripterWarningDlg(void);
};
