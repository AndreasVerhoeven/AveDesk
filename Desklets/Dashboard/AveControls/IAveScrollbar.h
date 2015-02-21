// IAveScrollbar.h: interface for the IAveScrollbar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVESCROLLBAR_H__864CA240_C3AC_4110_911F_3B4959E09A7E__INCLUDED_)
#define AFX_IAVESCROLLBAR_H__864CA240_C3AC_4110_911F_3B4959E09A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"

class IAveScrollbar;

typedef BOOL (__stdcall *OnScroll)(void * data, IAveScrollbar* scrollBar);

class IAveScrollbar  
{
public:
	IAveScrollbar();
	virtual ~IAveScrollbar();

	virtual void LockUpdate() = 0;
	virtual void UnlockUpdate() = 0;

	virtual void SetThumbSize(UINT visible, UINT total) = 0;

	virtual UINT GetPos()const = 0;
	virtual void SetPos(UINT pos) = 0;

	virtual UINT GetScrollScale()const = 0;
	virtual void SetScrollScale(UINT scale) = 0;

	virtual void SetOnScroll(void* data, OnScroll func) = 0;

};

#endif // !defined(AFX_IAVESCROLLBAR_H__864CA240_C3AC_4110_911F_3B4959E09A7E__INCLUDED_)
