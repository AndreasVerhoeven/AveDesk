// AveEditBox.h: interface for the CAveEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEEDITBOX_H__A26F85F8_8427_4FFF_813E_89FD316F35E2__INCLUDED_)
#define AFX_AVEEDITBOX_H__A26F85F8_8427_4FFF_813E_89FD316F35E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"

class CAveEditBox;

typedef BOOL (__stdcall *OnEditBoxEnterCallback)(void* callbackData, CAveEditBox* editBox);
typedef BOOL (__stdcall *OnEditBoxChangeCallback)(void* callbackData, CAveEditBox* editBox);

class CAveEditBox : public IAveControl  
{
protected:

	BOOL mouseIsDown;
	BOOL mouseWentOut;
	BOOL caretVisible;
	BOOL movedCursor;
	BOOL onlyLoseFocusOnDeskletOut;
	BOOL noSmoothing;
	POINT mouseDownPoint;
	POINT mousePt;
	BOOL isPassword;
	BOOL dontHandleMouse;

	HWND editHwnd;

	Font* font;
	Color textColor;
	Color selBgColor;

	HCURSOR oldCursor;
	LONG    oldLong;

	Bitmap* bmp;
	Bitmap dummyBmp;

	void* callbackData;
	OnEditBoxEnterCallback onEnterCallback;

	void* changeCallbackData;
	OnEditBoxChangeCallback onChangeCallback;

	static const int CARET_TIMER_ID;
	static const int CARET_TIMER_INTERVAL;

	static const int SCROLL_TIMER_ID;
	static const int SCROLL_TIMER_INTERVAL;

	int scrollX;
	BOOL scrollTimerIsRunning;
	BOOL isInRightClickMenu;

public:
	CAveEditBox(IAveControlContainer* container, std::string name);
	virtual ~CAveEditBox();

	virtual void SetOnEnterCallback(void* data, OnEditBoxEnterCallback func);

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


	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

	virtual void Update();

	virtual void SetNoSmoothing(BOOL v);
	virtual BOOL GetNoSmoothing()const;

	virtual BOOL SetScrollX(int val);
	virtual int  GetScrollX()const;

	virtual BOOL MakeSureCaretIsVisible();

	virtual void SetReadOnly(BOOL val);
	virtual BOOL GetReadOnly()const;

	virtual void SetIsPassword(BOOL val);
	virtual BOOL GetIsPassword()const;
	virtual std::wstring GetDrawingString();

	virtual void SetOnlyLoseFocusOnDeskletOut(BOOL val);
	virtual BOOL GetOnlyLoseFocusOnDeskletOut()const;

	virtual void SetOnEditBoxChangeCallback(void* data, OnEditBoxChangeCallback func);

	virtual BOOL HandleCharMessage(UINT msg, WPARAM wParam, LPARAM lParam);

};

#endif // !defined(AFX_AVEEDITBOX_H__A26F85F8_8427_4FFF_813E_89FD316F35E2__INCLUDED_)
