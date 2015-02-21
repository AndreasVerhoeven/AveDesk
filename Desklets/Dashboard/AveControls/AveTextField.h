// AveTextField.h: interface for the CAveTextField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVETEXTFIELD_H__A44CB55D_8816_4D63_B7D3_9A33BB14A0AB__INCLUDED_)
#define AFX_AVETEXTFIELD_H__A44CB55D_8816_4D63_B7D3_9A33BB14A0AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"
#include "AveVerticalScrollBar.h"

class CAveTextField;

typedef BOOL (__stdcall *OnScrollbarNotify)(void* callbackData, CAveTextField* textField, BOOL becomesVisible, DWORD* data);
typedef BOOL (__stdcall *OnEnterCallback)(void* callbackData, CAveTextField* textField);
typedef BOOL (__stdcall *OnTextFieldChangeCallback)(void* callbackData, CAveTextField* textField);

class CAveTextField : public IAveControl  
{
	BOOL mouseIsDown;
	BOOL mouseWentOut;
	BOOL caretVisible;
	BOOL movedCursor;
	BOOL onlyLoseFocusOnDeskletOut;
	BOOL noSmoothing;
	BOOL noReturns;
	BOOL scrollOnlyWholeLines;

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

	void* changeCallbackData;
	OnTextFieldChangeCallback onChangeCallback;

	CAveVerticalScrollBar* scrollBar;

	int scrollY;
	BOOL scrollTimerIsRunning;
	POINT mousePt;
	BOOL didScrollOnSide;
	BOOL scrollbarWasVisible;
	BOOL noScrollSet;

	Bitmap* bmp;

	BOOL isInRightClickMenu;

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
	virtual BOOL MouseRightClick(const POINT* pt, const SIZE* s, DWORD keys);

	virtual BOOL MouseOn(const POINT* pt, const SIZE* s);
	virtual BOOL MouseMove(const POINT* pt, const SIZE* s);
	virtual BOOL MouseOut();

	virtual BOOL MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	virtual BOOL MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	

	virtual BOOL Message(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual BOOL SetFocus();
	virtual void KillFocus(const char* newLayer);

	virtual void SetOnlyLoseFocusOnDeskletOut(BOOL val);
	virtual BOOL GetOnlyLoseFocusOnDeskletOut()const;

	virtual void SetNoSmoothing(BOOL v);
	virtual BOOL GetNoSmoothing()const;

	virtual void SetOnEnterCallback(void* data, OnEnterCallback func);
	virtual void SetOnScrollbarNotifyCallback(void* data, OnScrollbarNotify func);
	virtual void SetScrollbar(CAveVerticalScrollBar* scrollbar);

	virtual int GetScrollY()const;
	virtual BOOL SetScrollY(int val);

	virtual BOOL MakeSureCaretIsVisible();

	virtual BOOL NeedScrollBar();
	virtual BOOL UpdateScrollbar();

	static BOOL __stdcall OnScrollNotify(void * data, CAveVerticalScrollBar* scrollBar);

	virtual BOOL GetScrollOnlyWholeLines()const;
	virtual void SetScrollOnlyWholeLines(BOOL val);

	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

	virtual void SetReadOnly(BOOL val);
	virtual BOOL GetReadOnly()const;

	virtual void SetOnTextFieldChangeCallback(void* data, OnTextFieldChangeCallback func);

	virtual HandleCharMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // !defined(AFX_AVETEXTFIELD_H__A44CB55D_8816_4D63_B7D3_9A33BB14A0AB__INCLUDED_)
