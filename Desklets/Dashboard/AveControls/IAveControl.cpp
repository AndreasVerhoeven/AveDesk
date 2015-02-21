// IAveControl.cpp: implementation of the IAveControl class.
//
//////////////////////////////////////////////////////////////////////

#include "aveconstants.h"
#include "IAveControl.h"
#include "IAveControlContainer.h"
#include "AveControlHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IAveControl::IAveControl(IAveControlContainer* container, std::string name) :
	container(container), name(name), hasFocus(FALSE),
	onSetFocus(NULL), onKillFocus(NULL), resources(NULL),
	isDisabled(FALSE)
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
	DeskletLayerRemove(container->GetDeskletHandle(), GetLayerId(), FALSE);
}

void IAveControl::SetIsDisabled(BOOL val)
{
	if(val != isDisabled)
	{
		isDisabled = val;
		Update();
	}
}

BOOL IAveControl::GetIsDisabled()const
{
	return isDisabled;
}

void IAveControl::SetOnSetFocus(void*  data, OnSetFocus func)
{
	getFocusCallbackData = data;
	onSetFocus = func;
}

void IAveControl::SetOnKillFocus(void* data, OnKillFocus func)
{
	killFocusCallbackData = data;
	onKillFocus = func;
}

DWORD_PTR IAveControl::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	switch(msg)
	{
		case AV_SHOW:
			handled = TRUE;
			Show();
			return 0;
		break;

		case AV_HIDE:
			handled = TRUE;
			Hide();
			return 0;
		break;

		case AV_ISVISIBLE:
			handled = TRUE;
			return IsVisible();
		break;

		case AV_SET_RECT:
			handled = TRUE;
			if(NULL == wParam || NULL == lParam)
			{
				return FALSE;
			}
			{
				SetRect((RECT*)wParam, (SIZE*)lParam);
				return TRUE;
			}
		break;

		case AV_GET_RECT:
			handled = TRUE;
			if(wParam != NULL)
				*((RECT*)wParam) = GetRect();

			if(lParam != NULL)
				*((SIZE*)lParam) = GetScale();

			return 0;
		break;

		case AV_GET_REDRAW:
			handled = TRUE;
			return (DWORD_PTR) GetRedraw((BOOL)wParam);
		break;

		case AV_UPDATE:
			handled = TRUE;
			Update();
			return 0;
		break;

		case AV_GET_NAME:
			handled = TRUE;
			if(lParam != NULL)
			{
				return CAveControlHelper::CopyStringToCHARPtr(GetName(), (char*)lParam, (size_t)wParam);
			}
			else
			{
				return (DWORD_PTR)0;
			}
		break;

		case AV_GET_NAMELENGTH:
			handled = TRUE;
			return (DWORD_PTR) GetName().length();
		break;

		case AV_SET_ONSETFOCUSCALLBACK:
			handled = TRUE;
			SetOnSetFocus((void*)wParam, (OnSetFocus)lParam);
			return 0;
		break;

		case AV_SET_ONKILLFOCUSCALLBACK:
			handled = TRUE;
			SetOnKillFocus((void*)wParam, (OnKillFocus)lParam);
			return 0;
		break;

		case AV_SET_RESOURCES:
			handled = TRUE;
			SetResources((IAveResources*)lParam);
			return 0;
		break;

		case AV_GET_LAYERID:
			handled = TRUE;
			return (DWORD_PTR) GetLayerId();
		break;

		case AV_SET_ISDISABLED:
			handled = TRUE;
			SetIsDisabled((BOOL)wParam);
			return 0;
		break;

		case AV_GET_ISDISABLED:
			handled = TRUE;
			return (DWORD_PTR)GetIsDisabled();
		break;

		default:
			handled = FALSE;
			return 0;
	}
}

IAveResources* IAveControl::GetResources()
{
	if(NULL == resources)
		return GetContainer()->GetResources();
	else
		return resources;
}

void IAveControl::SetResources(IAveResources* res)
{
	if(res != resources)
	{
		resources = res;
		Update();
	}
}

BOOL IAveControl::SetFocus()
{
	hasFocus = TRUE;

	if(onSetFocus != NULL)
		onSetFocus(getFocusCallbackData, this);

	return TRUE;
}
void IAveControl::KillFocus(const char* newLayer)
{
	hasFocus = FALSE;

	if(onKillFocus != NULL)
		onKillFocus(killFocusCallbackData, this);
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
BOOL IAveControl::MouseRightClick(const POINT* pt, const SIZE* s, DWORD keys)
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