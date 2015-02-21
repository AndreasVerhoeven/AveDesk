#pragma once

#include "DeskletItem.h"

class CDeskletItemDragAnimation : 
	public CLayeredWindowImpl<CDeskletItemDragAnimation>
{

protected:
	Bitmap* gdipBmp;
	Bitmap* gdipBmpLarge;
	int currentFrame;
	int currentLargeFrame;
	int size;
	CDeskletItem* item;
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
	CDeskletItemDragAnimation(void);
	virtual ~CDeskletItemDragAnimation(void);

	void StartAnimation(CDeskletItem* item, const SIZE& s, HWND parentHwnd);

	void UpdateLarge(SIZE s, SIZE s2, BYTE largeAlpha, BYTE smallAlpha, BOOL largeFirst);
	void Update(int size, int alpha);
	void SafeDestroy();
	virtual void OnFinalMessage(HWND);

	BOOL IsInsideForbiddenWindowArea();

	BEGIN_MSG_MAP(CDeskletItemDragAnimation)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		CHAIN_MSG_MAP(CLayeredWindowImpl<CDeskletItemDragAnimation>)
	END_MSG_MAP()

	LRESULT OnClose(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCaptureChanged(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);


};
