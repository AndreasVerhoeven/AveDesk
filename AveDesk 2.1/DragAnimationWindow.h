// DragAnimationWindow.h: interface for the CDragAnimationWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGANIMATIONWINDOW_H__8490C280_4BF4_4721_AAC6_D735DD1B025C__INCLUDED_)
#define AFX_DRAGANIMATIONWINDOW_H__8490C280_4BF4_4721_AAC6_D735DD1B025C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LayeredWindowImpl.hpp"
#include "FoundDeskletEntry.h"

class CDragAnimationWindow  :
	public CLayeredWindowImpl<CDragAnimationWindow>
{
protected:
	Bitmap* gdipBmp;
	Bitmap* gdipBmpLarge;
	int currentFrame;
	int currentLargeFrame;
	int size;
	CFoundDeskletEntry* entry;
	HWND forbiddenWindow;
	BOOL bDestroyed;

	BOOL wasInForbiddenWindowArea;

	SIZE wndSize;

	float xRatio, yRatio;

	int maxNumFrames;
	int maxNumFramesLarge;

	UINT CheckAnimSpeed();

	BOOL smallAndLargeAreTheSame;

public:
	CDragAnimationWindow();
	~CDragAnimationWindow();

	void StartAnimation(CFoundDeskletEntry* e, const SIZE& s, HWND parentHwnd);

	void UpdateLarge(SIZE s, SIZE s2, BYTE largeAlpha, BYTE smallAlpha, BOOL largeFirst);
	void Update(int size, int alpha);
	void SafeDestroy();
	virtual void OnFinalMessage(HWND);

	BOOL IsInsideForbiddenWindowArea();

	BEGIN_MSG_MAP(CDragAnimationWindow)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		CHAIN_MSG_MAP(CLayeredWindowImpl<CDragAnimationWindow>)
	END_MSG_MAP()

	LRESULT OnClose(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCaptureChanged(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);


};

#endif // !defined(AFX_DRAGANIMATIONWINDOW_H__8490C280_4BF4_4721_AAC6_D735DD1B025C__INCLUDED_)
