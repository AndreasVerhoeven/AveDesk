// AveVerticalScrollBar.h: interface for the CAveVerticalScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEVERTICALSCROLLBAR_H__44123683_2411_496E_843E_A5D6B8FBE90D__INCLUDED_)
#define AFX_AVEVERTICALSCROLLBAR_H__44123683_2411_496E_843E_A5D6B8FBE90D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"
//#include "IAveScrollbar.h"

class CAveVerticalScrollBar;
typedef BOOL (__stdcall *ExternalOnScroll)(void* data, CAveVerticalScrollBar* bar);
typedef BOOL (__stdcall *OnScroll)(void * data, CAveVerticalScrollBar* scrollBar);


class CAveVerticalScrollBar : public IAveControl
//	,public IAveScrollbar 
{
private:
	HWND scrollbarHwnd;
	UINT thumbSize;
	UINT projVisible;
	UINT projTotal;

	Bitmap* thumbBmp;
	Bitmap* bgBmp;

	BOOL mouseIsDown;
	BOOL downOnThumb;

	int thumbGrabPoint;

	void* callbackData, *callbackData2;
	OnScroll onScroll;
	ExternalOnScroll externalOnScroll;

	POINT mouseDownPt;

	BOOL prevDisabled;

	virtual void InternalSetPos(UINT pos, bool notify);

public:
	CAveVerticalScrollBar(IAveControlContainer* container, std::string name);
	virtual ~CAveVerticalScrollBar();

	virtual void SetRect(RECT* r, SIZE* scaleSize);
	virtual void SetRect(int x, int y, int width, int height, UINT xScale, UINT yScale);

	virtual UINT GetPos()const;
	virtual void SetPos(UINT pos);

	virtual UINT GetScrollScale()const;
	virtual void SetScrollScale(UINT scale);

	virtual void LockUpdate();
	virtual void UnlockUpdate();

	virtual void SetThumbSize(UINT visible, UINT total);
	
	virtual void Update();

	virtual BOOL MouseDown(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseUp(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseClick(const POINT* pt, const SIZE* s, DWORD keys);

	virtual BOOL MouseOn(const POINT* pt, const SIZE* s);
	virtual BOOL MouseMove(const POINT* pt, const SIZE* s);
	virtual BOOL MouseOut();

	virtual BOOL MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	virtual BOOL MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	

	virtual BOOL Message(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void SetOnScroll(void* data, OnScroll func);

	virtual Rect GetThumbRect();

	
	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

};

#endif // !defined(AFX_AVEVERTICALSCROLLBAR_H__44123683_2411_496E_843E_A5D6B8FBE90D__INCLUDED_)
