#include "stdafx.h"
#include "AveScripterWarningDlg.h"
#include "Application.h"
#include "AveInstFileFetcher.h"
#include "AveInstaller.h"
#include "AsyncDownloader.h"

extern CComObject<CApplication>* _App;

CAveScripterWarningDlg::CAveScripterWarningDlg(void)
{
}

CAveScripterWarningDlg::~CAveScripterWarningDlg(void)
{
}


LRESULT CAveScripterWarningDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ToolTips_Init();

	progressBar = GetDlgItem(IDC_DOWNLOADPROGRESS);
	readMoreLink.SubclassWindow(GetDlgItem(IDC_READMORE));
	readMoreLink.SetHyperLink(L"http://www.avedesk.org");

	LOGFONTW lfCaption = {0};
	lfCaption.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfCaption.lfFaceName,L"MS Shell Dlg");
	lfCaption.lfWeight = FW_BOLD;
	lfCaption.lfHeight = -16;

	captionFont.CreateFontIndirectW(&lfCaption);

	captionStatic = GetDlgItem(IDC_WARNINGCAPTION);
	captionStatic.SetFont(captionFont, TRUE);


	WCHAR fname[MAX_PATH] = {0};
	wcscpy_s(fname, MAX_PATH, _App->GetAveDeskDataDir().c_str());
	PathAppendW(fname, L"images//icons//avedesk_bg.png");
	Bitmap bmp(fname);
	CBitmap hBmp;
	bmp.GetHBITMAP(NULL, &hBmp.m_hBitmap);

	CStatic bmpStatic = GetDlgItem(IDC_BMP);
	bmpStatic.SetBitmap(hBmp);


	return 0;
}

LRESULT CAveScripterWarningDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps = {0};
	HDC dc = ::BeginPaint(m_hWnd, &ps);
	
	//::SendMessage(hwndListBox, WM_PRINT, (WPARAM)dc, (LPARAM)PRF_ERASEBKGND | PRF_NONCLIENT | PRF_CLIENT);

	::EndPaint(m_hWnd, &ps);

	return 0;
}

LRESULT CAveScripterWarningDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

LRESULT CAveScripterWarningDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	readMoreLink.UnsubclassWindow();
	return 0;
}

LRESULT CAveScripterWarningDlg::OnProgressCallback(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	long progress = static_cast<long>(wParam);
	long max = static_cast<long>(lParam);
	if(progress != -1 && !::IsWindowVisible(progressBar))
		progressBar.ShowWindow(SW_SHOW);
	else if(-1 == progress && ::IsWindowVisible(progressBar))
	{
		progressBar.ShowWindow(SW_HIDE);
		GetParent().SendMessageW(WM_USER_NOTIFY_ADD_DIR, 0L, (LPARAM)L"Desklets\\AveScripter");
	}

	progressBar.SetRange(0, (int)max);
	progressBar.SetPos((int)progress);

	return 0;
}


LRESULT CAveScripterWarningDlg::OnReadMoreLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShellExecuteW(NULL, L"open", L"http://www.avedesk.org", NULL, NULL, SW_SHOW);
	return 0;
}

LRESULT CAveScripterWarningDlg::OnDownloadButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAsyncDownloader* d = new CAsyncDownloader(m_hWnd, L"http://avedesk.philc.ca/desklets/avescripterbeta.aveinst");
	d->Start();

	return 0;
}