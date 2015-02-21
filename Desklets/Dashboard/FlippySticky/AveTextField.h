// AveTextField.h: interface for the CAveTextField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVETEXTFIELD_H__A44CB55D_8816_4D63_B7D3_9A33BB14A0AB__INCLUDED_)
#define AFX_AVETEXTFIELD_H__A44CB55D_8816_4D63_B7D3_9A33BB14A0AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"
#include "IAveScrollBar.h"

class CAveTextField;

typedef BOOL (__stdcall *OnScrollbarNotify)(void* callbackData, CAveTextField* textField, BOOL becomesVisible, DWORD* data);

typedef BOOL (__stdcall *OnEnterCallback)(void* callbackData, CAveTextField* textField);

class CAveTextField : public IAveControl  
{
	BOOL mouseIsDown;
	BOOL mouseWentOut;
	BOOL caretVisible;
	BOOL movedCursor;
	BOOL onlyLoseFocusOnDeskletOut;
	BOOL noSmoothing;
	BOOL noReturns;

	std::wstring text;
	Color textColor;
	Color selBgColor;
	Font* font;

	Bitmap dummyBmp;

	POINT mouseDownPoint;
	
	HCURSOR oldCursor;
	LONG   oldLong;

	HWND editHwnd;

	void* callbackData;
	OnEnterCallback onEnterCallback;

	void* callbackDataScrollbar;
	OnScrollbarNotify onScrollbarNotify;

	IAveScrollbar* scrollBar;

	int scrollY;
	BOOL scrollTimerIsRunning;
	POINT mousePt;
	BOOL didScrollOnSide;
	BOOL scrollbarWasVisible;

	static const UINT CARET_TIMER_ID;
	static const UINT CARET_TIMER_INTERVAL;
	static const UINT AVE_TEXTBOXMAXHEIGHT;
	static const UINT SCROLL_TIMER_ID;
	static const UINT SCROLL_TIMER_INTERVAL;

public:

	virtual UINT GetSelIndexFromPoint(const POINT* pt, bool noSideCorrections = false);
	virtual Rect GetRectForCharacterIndex(int index);

	virtual void SetSelStart(int index);
	virtual void SetSelEnd(int index);
	virtual void SetSelRange(int start, int end);

	virtual int GetSelStart()const;
	virtual int GetSelEnd()const;
	virtual int GetCaretPos()const;

	virtual bool HasSelection()const;
	virtual void NoSelection();

	virtual Color GetTextColor()const;
	virtual void  SetTextColor(const Color& c);

	virtual Color GetSelBgColor()const;
	virtual void  SetSelBgColor(const Color& c);

	virtual Font* GetFont();
	virtual void SetFont(Font* f);

	virtual std::wstring GetText();
	virtual void SetText(const std::wstring& str);

	virtual std::string GetTextA();
	virtual void SetTextA(const std::string& str);

	virtual void DeleteSelection();

	virtual void Update();

	CAveTextField(IAveControlContainer* container, std::string name,BOOL noReturns = FALSE);
	virtual ~CAveTextField();

	
	virtual BOOL MouseDown(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseUp(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseClick(const POINT* pt, const SIZE* s, DWORD keys);

	virtual BOOL MouseOn(const POINT* pt, const SIZE* s);
	virtual BOOL MouseMove(const POINT* pt, const SIZE* s);
	virtual BOOL MouseOut();

	virtual BOOL MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	virtual BOOL MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	

	virtual BOOL Message(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual BOOL SetFocus();
	virtual void KillFocus(const char* newLayer);

	void SetOnlyLoseFocusOnDeskletOut(BOOL val);
	BOOL GetOnlyLoseFocusOnDeskletOut()const;

	virtual void SetNoSmoothing(BOOL v);
	virtual BOOL GetNoSmoothing()const;

	virtual void SetOnEnterCallback(void* data, OnEnterCallback func);
	virtual void SetOnScrollbarNotifyCallback(void* data, OnScrollbarNotify func);
	virtual void SetScrollbar(IAveScrollbar* scrollbar);

	virtual int GetScrollY()const;
	virtual BOOL SetScrollY(int val);

	virtual BOOL MakeSureCaretIsVisible();

	virtual BOOL NeedScrollBar();
	virtual BOOL UpdateScrollbar();
};

#endif // !defined(AFX_AVETEXTFIELD_H__A44CB55D_8816_4D63_B7D3_9A33BB14A0AB__INCLUDED_)
