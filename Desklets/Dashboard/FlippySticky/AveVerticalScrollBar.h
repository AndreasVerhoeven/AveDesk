// AveVerticalScrollBar.h: interface for the CAveVerticalScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEVERTICALSCROLLBAR_H__44123683_2411_496E_843E_A5D6B8FBE90D__INCLUDED_)
#define AFX_AVEVERTICALSCROLLBAR_H__44123683_2411_496E_843E_A5D6B8FBE90D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"
#include "IAveScrollbar.h"

class CAveVerticalScrollBar : public IAveControl,
	public IAveScrollbar 
{
private:
	HWND scrollbarHwnd;
	UINT thumbSize;
	UINT projVisible;
	UINT projTotal;

	Bitmap* thumbBmp;
	Bitmap* bgBmp;

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

};

#endif // !defined(AFX_AVEVERTICALSCROLLBAR_H__44123683_2411_496E_843E_A5D6B8FBE90D__INCLUDED_)
