// InstallErrorDlg.cpp: implementation of the CInstallErrorDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InstallErrorDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInstallErrorDlg::CInstallErrorDlg(const CAveInstError& error) :
	m_Error(error)
{

}

CInstallErrorDlg::~CInstallErrorDlg()
{

}

LRESULT CInstallErrorDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_Icon = GetDlgItem(IDC_ERRORICON);
	m_Icon.SetIcon( LoadIcon(NULL,IDI_HAND));

	SetDlgItemText(IDC_ERRORTEXT,m_Error.GetMessage().c_str());

	m_ErrorLink.SubclassWindow(GetDlgItem(IDC_ERRORLINK));
	if(!m_Error.GetHomePage().empty())
	{
		m_ErrorLink.SetLabel(m_Error.GetHomePage().c_str());
		m_ErrorLink.SetHyperLink(m_Error.GetHomePage().c_str());
	}
	else
	{
		::ShowWindow(GetDlgItem(IDC_ERRORLINKTEXT),SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_ERRORLINK),SW_HIDE);
	}

	MessageBeep(MB_ICONHAND);

	return 0;

}

LRESULT CInstallErrorDlg::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TCHAR chCharCode = (TCHAR) wParam;

	if(chCharCode == _T('c') && GetKeyState(VK_CONTROL) < 0 ) // CTRL + C
	{
		const WCHAR sep[] = L"------------------------------------------------";
		std::wstring output;

		WCHAR windowTitle[100] = {0};
		GetWindowTextW(windowTitle,100);
		output = output + sep + L"\n" + windowTitle + sep + L"\n";
		output = output + m_Error.GetMessage().c_str() + L"\n" + sep;
		if(!m_Error.GetHomePage().empty())
			output = output + m_Error.GetHomePage().c_str() + L"\n" + sep;
		
		WCHAR butTitle[100] = {0};
		GetDlgItemText(IDOK,butTitle,100);
		output = output + butTitle + L"\n" + sep;

		OpenClipboard();

		HGLOBAL hGlob =  GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,(output.length()+1) * sizeof(WCHAR));
		WCHAR* buf = reinterpret_cast<WCHAR*>(GlobalLock(hGlob));
		if(buf != NULL)
		{
		
			wcscpy(buf,output.c_str());
			GlobalUnlock(hGlob);
			SetClipboardData(CF_TEXT,hGlob);
		}

		CloseClipboard();
	}

	return 0;
}

LRESULT CInstallErrorDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return 0;
}

LRESULT CInstallErrorDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyIcon(m_Icon.GetIcon());
	m_ErrorLink.UnsubclassWindow();
	return 0;
}


LRESULT CInstallErrorDlg::OnOKButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDOK);
	return 0;
}