// AveInstBitmapWindow.h: interface for the CAveInstBitmapWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTBITMAPWINDOW_H__84EACE70_69B7_4D6A_B7D6_BF8C1C7C4021__INCLUDED_)
#define AFX_AVEINSTBITMAPWINDOW_H__84EACE70_69B7_4D6A_B7D6_BF8C1C7C4021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LayeredWindowImpl.hpp"
#include <string>

class CAveInstBitmapWindow : public CLayeredWindowImpl<CAveInstBitmapWindow>  
{
protected:
	Bitmap* m_Bmp;
	HANDLE m_Handle;

	CAveInstBitmapWindow(const std::wstring& file);
	virtual ~CAveInstBitmapWindow();

	void SetupWindow();

public:
	static CAveInstBitmapWindow* CreateBitmapWindow(const std::wstring& file);
	static DWORD __stdcall ThisThreadFunc(LPVOID lpData);


	BEGIN_MSG_MAP(CAveInstBitmapWindow)

		// message handlers
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonClick)
		MESSAGE_HANDLER(WM_TIMER,OnTimer);
		//MESSAGE_HANDLER(WM_NCDESTROY,OnDestroy )
		// other stuff

		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CLayeredWindowImpl<CAveInstBitmapWindow>)
	END_MSG_MAP()

	LRESULT OnLeftButtonClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};

#endif // !defined(AFX_AVEINSTBITMAPWINDOW_H__84EACE70_69B7_4D6A_B7D6_BF8C1C7C4021__INCLUDED_)
