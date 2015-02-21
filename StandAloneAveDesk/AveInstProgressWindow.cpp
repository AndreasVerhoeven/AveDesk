// AveInstProgressWindow.cpp: implementation of the CAveInstProgressWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstProgressWindow.h"
#include "AveInstCancelable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstProgressWindow::CAveInstProgressWindow()
{

}

CAveInstProgressWindow::~CAveInstProgressWindow()
{
	KillWindow();
}


void CAveInstProgressWindow::SetMessage(std::wstring msg)
{
	if(::IsWindow(m_hWnd))
	{
		SetDlgItemTextW(IDC_MESSAGE,msg.c_str());
		ShowWindow(SW_SHOW);
	}
}

void CAveInstProgressWindow::SetProgress(long progress, long max)
{
	if(::IsWindow(m_hWnd))
	{
		HWND pbar = GetDlgItem(IDC_PROGRESSBAR);
		if(0 == progress && 0 == max)
		{
			::ShowWindow(pbar, SW_HIDE);
		}
		else
		{
			
			progress = static_cast<long>(((float)progress  / (float)max) * 100.0f);
			max = 100;

			int prev = HIWORD(::SendMessage(pbar,PBM_SETRANGE ,0, MAKELPARAM(0,max)));
			::PostMessage(pbar,PBM_SETPOS,progress,0L); 
			::ShowWindow(pbar, SW_SHOW);
		}
	}
}

BOOL CAveInstProgressWindow::SetupWindow(HWND parent)
{
	CDialogImpl<CAveInstProgressWindow>::Create(parent);
	//HWND hwnd = CreateDialog(
	if(::IsWindow(m_hWnd))
		CenterWindow();
	return TRUE;
}

BOOL CAveInstProgressWindow::KillWindow()
{
	if(::IsWindow(m_hWnd))
	{
		SendMessage(WM_DESTROYME);
		//DestroyWindow();
	}

	return TRUE;
}

LRESULT CAveInstProgressWindow::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetDlgItemText(IDC_MESSAGE,L"");
	return 0;
}

LRESULT CAveInstProgressWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CAveInstProgressWindow::OnDestroyMe(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT CAveInstProgressWindow::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for(std::list<CAveInstCancelable*>::iterator iter = m_Cancelables.begin(); iter != m_Cancelables.end(); ++iter)
	{
		CAveInstCancelable* cur = *iter;
		if(cur)
			cur->Cancel();
	}

	ShowWindow(SW_HIDE);
	return 0;
}

bool CAveInstProgressWindow::RegisterCancelable(CAveInstCancelable* c)
{
	for(std::list<CAveInstCancelable*>::iterator iter = m_Cancelables.begin(); iter != m_Cancelables.end(); ++iter)
	{
		CAveInstCancelable* cur = *iter;
		if(cur == c)
			return false;
	}

	m_Cancelables.push_back(c);

	if(::IsWindow(m_hWnd))
	{
		HWND hwndCancel = GetDlgItem(IDCANCEL);
		::ShowWindow(hwndCancel,SW_SHOW);
	}

	return true;
}

void CAveInstProgressWindow::Hide()
{
	if(::IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);
}


bool CAveInstProgressWindow::UnregisterCancelable(CAveInstCancelable* c)
{
	for(std::list<CAveInstCancelable*>::iterator iter = m_Cancelables.begin(); iter != m_Cancelables.end(); ++iter)
	{
		CAveInstCancelable* cur = *iter;
		if(cur == c)
		{
			m_Cancelables.erase(iter);
			if(m_Cancelables.size() == 0)
			{
				if(::IsWindow(m_hWnd))
				{
					HWND hwndCancel = GetDlgItem(IDCANCEL);
					::ShowWindow(hwndCancel,SW_HIDE);
				}
			}

			return true;
		}
	}

	return false;
}