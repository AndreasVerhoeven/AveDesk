// AveButton.h: interface for the CAveButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEBUTTON_H__BCEDA9C7_096A_41DA_B25C_1BECCC3EA158__INCLUDED_)
#define AFX_AVEBUTTON_H__BCEDA9C7_096A_41DA_B25C_1BECCC3EA158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"
#include "IAveCallback.h"

typedef BOOL (__stdcall *ButtonClickHandler)(void* ptr, UINT id, const char* name, const POINT* pt, const SIZE* s, DWORD keys);
class CAveButton : public IAveControl  
{
protected:
	BOOL mouseIsDown;
	BOOL mouseWentOut;
	ButtonClickHandler clickHandler;
	IAveCallback* callbackObject;

public:
	CAveButton(IAveControlContainer* container, std::string name);
	virtual ~CAveButton();

	virtual BOOL MouseDown(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseUp(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseClick(const POINT* pt, const SIZE* s, DWORD keys);

	virtual BOOL MouseOn(const POINT* pt, const SIZE* s);
	virtual BOOL MouseMove(const POINT* pt, const SIZE* s);
	virtual BOOL MouseOut();

	virtual void SetClickHandler(IAveCallback* callback, ButtonClickHandler handler);

};

#endif // !defined(AFX_AVEBUTTON_H__BCEDA9C7_096A_41DA_B25C_1BECCC3EA158__INCLUDED_)
