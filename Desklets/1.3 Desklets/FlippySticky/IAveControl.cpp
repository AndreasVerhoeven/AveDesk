// IAveControl.cpp: implementation of the IAveControl class.
//
//////////////////////////////////////////////////////////////////////

#include "IAveControl.h"
#include "IAveControlContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IAveControl::IAveControl(IAveControlContainer* container, std::string name) :
	container(container), name(name), hasFocus(FALSE)
{
	place.left = 0;
	place.top  = 0;
	place.right = 0;
	place.bottom = 0;

	scale.cx = 0;
	scale.cy = 0;

	UINT id = DeskletLayerAdd(container->GetDeskletHandle(), name.c_str());
	DeskletLayerSetPlacement(container->GetDeskletHandle(), id, FALSE, TRUE, &place, scale.cx, scale.cy);
}

IAveControl::~IAveControl()
{

}

BOOL IAveControl::SetFocus()
{
	hasFocus = TRUE;

	return TRUE;
}
void IAveControl::KillFocus(const char* newLayer)
{
	hasFocus = FALSE;
}

BOOL IAveControl::HasFocus()const
{
	return hasFocus;
}

RECT IAveControl::GetRect()const
{
	return place;
}

SIZE IAveControl::GetScale()const
{
	return scale;
}

void IAveControl::SetRect(RECT* r, SIZE* scaleSize)
{
	if(r != NULL)
	{
		scale = *scaleSize;
		place = *r;
		DeskletLayerSetPlacement(container->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), TRUE, &place, scale.cx, scale.cy);
	}
}

void IAveControl::SetRect(int x, int y, int width, int height, UINT xScale, UINT yScale)
{
	RECT r = {x, y, x + width, y + height};
	SIZE s = {xScale, yScale};
	SetRect(&r, &s);
}

std::string IAveControl::GetName()const
{
	return name;
}

BOOL IAveControl::GetRedraw(BOOL redraw)const
{
	return redraw && !IsLocked() && container->GetRedraw(redraw);
}

IAveControlContainer* IAveControl::GetContainer()
{
	return container;
}

UINT IAveControl::GetLayerId()const
{
	BOOL success = FALSE;
	UINT id = DeskletLayerGetLayer(container->GetDeskletHandle(), name.c_str(), &success);
	if(!success)
		id = static_cast<UINT>(-1);

	return id;
}

void IAveControl::Hide()
{
	if(IsVisible())
	{
		isVisible = FALSE;
		DeskletLayerSetVisibility(container->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), FALSE);
	}
}

void IAveControl::Show()
{
	if(!IsVisible())
	{
		isVisible = TRUE;
		DeskletLayerSetVisibility(container->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), TRUE);
	}
}

BOOL IAveControl::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	return FALSE;
}

BOOL IAveControl::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	return FALSE;
}

BOOL IAveControl::MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	return FALSE;
}

BOOL IAveControl::MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	return FALSE;
}


BOOL IAveControl::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	return FALSE;
}

BOOL IAveControl::MouseOn(const POINT* pt, const SIZE* s)
{
	return FALSE;
}

BOOL IAveControl::MouseMove(const POINT* pt, const SIZE* s)
{
	return FALSE;
}

BOOL IAveControl::MouseOut()
{
	return FALSE;
}

BOOL IAveControl::Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}