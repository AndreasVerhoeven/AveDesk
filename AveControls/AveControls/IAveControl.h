// IAveControl.h: interface for the IAveControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVECONTROL_H__7EEACEBA_7377_4F04_AF1C_1176949103AA__INCLUDED_)
#define AFX_IAVECONTROL_H__7EEACEBA_7377_4F04_AF1C_1176949103AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveVisibilityControl.h"
#include "IAveLockable.h"
#include "IAveControlContainer.h"
#include <string>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

class IAveControl;
typedef BOOL (__stdcall *OnSetFocus)(void* callbackData, IAveControl* control);
typedef BOOL (__stdcall *OnKillFocus)(void* callbackData, IAveControl* control);

class IAveControl : 
	public IAveVisibilityControl, 
	public IAveLockableImpl
{
protected:
	std::string name;
	IAveControlContainer* container;
	RECT place;
	SIZE scale;
	BOOL hasFocus;
	BOOL isDisabled;
	INT autoSize;
	SIZE maxAutoSize;

	void* getFocusCallbackData;
	void* killFocusCallbackData;
	OnSetFocus onSetFocus;
	OnKillFocus onKillFocus;

	IAveResources* resources;

public:
	IAveControl(IAveControlContainer* container, std::string name);
	virtual ~IAveControl() = 0;

	virtual void SetOnSetFocus(void*  data, OnSetFocus func);
	virtual void SetOnKillFocus(void* data, OnKillFocus func);

	virtual std::string GetName()const;
	virtual IAveControlContainer* GetContainer();

	
	virtual BOOL GetRedraw(BOOL redraw)const;
	virtual void Hide();
	virtual void Show();

	virtual UINT GetLayerId()const;

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

	virtual RECT GetRect()const;
	virtual SIZE GetScale()const;
	virtual void SetRect(RECT* r, SIZE* scaleSize);
	virtual void SetRect(int x, int y, int width, int height, UINT xScale, UINT yScale);

	virtual BOOL SetFocus();
	virtual void KillFocus(const char* newLayer);

	virtual BOOL HasFocus()const;

	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

	virtual void Update() = 0;

	virtual IAveResources* GetResources();
	virtual void SetResources(IAveResources* res);

	virtual void SetIsDisabled(BOOL val);
	virtual BOOL GetIsDisabled()const;

};

#endif // !defined(AFX_IAVECONTROL_H__7EEACEBA_7377_4F04_AF1C_1176949103AA__INCLUDED_)
