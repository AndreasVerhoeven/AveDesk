// AveInstBitmapWindow.cpp: implementation of the CAveInstBitmapWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstBitmapWindow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define TIMER_HIDEBMPWINDOW 3048

CAveInstBitmapWindow::CAveInstBitmapWindow(const std::wstring& file) :
	m_Handle(NULL)
{
	Bitmap bmp(file.c_str());
	m_Bmp = new Bitmap(bmp.GetWidth(), bmp.GetHeight() );
	Graphics g(m_Bmp);
	g.DrawImage(&bmp,0,0,bmp.GetWidth(),bmp.GetHeight());
}

CAveInstBitmapWindow::~CAveInstBitmapWindow()
{

}

CAveInstBitmapWindow* CAveInstBitmapWindow::CreateBitmapWindow(const std::wstring& file)
{
	CAveInstBitmapWindow* bw = new CAveInstBitmapWindow(file);
	DWORD dummy(0);

	bw->m_Handle = CreateThread(NULL,0,ThisThreadFunc,reinterpret_cast<LPVOID>(bw),0,&dummy);

	return bw;
}

DWORD __stdcall CAveInstBitmapWindow::ThisThreadFunc(LPVOID lpData)
{
	CAveInstBitmapWindow* bw = reinterpret_cast<CAveInstBitmapWindow*>(lpData);

	HWND hiddenParentHwnd = CreateWindow(_T("AveDummyClass"),_T("AveDummyWindow"),0,0,1,1,0,0,0, _Module.GetModuleInstance(),0);

	RECT r = {0};
	bw->Create(hiddenParentHwnd,r,_T("AveDesklet"),WS_POPUP | WS_OVERLAPPED,WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	// we want the hand cursor to show up
	::SetClassLong(bw->m_hWnd,GCL_HCURSOR,reinterpret_cast<LONG>(LoadCursor(NULL,IDC_HAND)));

	// we dont want this window to show up in the taskbar
	bw->ModifyStyle(0,WS_POPUP | WS_OVERLAPPED);

	bw->ShowWindow(SW_HIDE);
	bw->ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW | WS_EX_TOPMOST,0);
	bw->SetParent(NULL);

	bw->EnsureWindowIsLayered();

	POINT p = {0,0};
	SIZE  s = {bw->m_Bmp->GetWidth(), bw->m_Bmp->GetHeight() };
	bw->SetBitmap(*bw->m_Bmp,0,&p,&s);
	bw->CenterWindow();
	bw->SetWindowPos(HWND_TOPMOST,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	const int maxFrameNum = 25;


	BOOL fFade=FALSE;
	SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &fFade, 0);
	if(fFade)
	{
		for(int i = 1; i < maxFrameNum; ++i)
		{
			float alphaPerFrame = 255.0f /  static_cast<float>(maxFrameNum);
			bw->SetAlpha(static_cast<BYTE>(alphaPerFrame * static_cast<float>(i)));
		}
	}
	bw->SetBitmap(bw->m_Bmp,255,NULL,&s);
	delete bw->m_Bmp;
	bw->m_Bmp = 0;


	bw->SetTimer(TIMER_HIDEBMPWINDOW,10000);

	MSG msg = {0};
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
        DispatchMessage(&msg);
	}

	SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &fFade, 0);
	if(fFade)
	{
		for(int i = 1; i < maxFrameNum; ++i)
		{
			float alphaPerFrame = 255.0f /  static_cast<float>(maxFrameNum);
			bw->SetAlpha(static_cast<BYTE>(255.0f - (alphaPerFrame * i)));
		}	
	}

	bw->DestroyWindow();
	::DestroyWindow(hiddenParentHwnd);

	HANDLE h = bw->m_Handle;

	delete bw;
	
	ExitThread(0);

	CloseHandle(h);
}

LRESULT CAveInstBitmapWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam == TIMER_HIDEBMPWINDOW)
	{
		KillTimer(TIMER_HIDEBMPWINDOW);
		PostQuitMessage(0);
	}
	return 0;
}

LRESULT CAveInstBitmapWindow::OnLeftButtonClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0);
	return 0;
}