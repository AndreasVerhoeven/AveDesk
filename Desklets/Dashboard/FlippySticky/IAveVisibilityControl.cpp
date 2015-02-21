// IAveVisibilityControl.cpp: implementation of the IAveVisibilityControl class.
//
//////////////////////////////////////////////////////////////////////

#include "IAveVisibilityControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IAveVisibilityControl::IAveVisibilityControl() : 
	isVisible(TRUE)
{

}

IAveVisibilityControl::~IAveVisibilityControl()
{


}

void IAveVisibilityControl::SetVisibility(BOOL val)
{
	if(val)
		Show();
	else
		Hide();
}

BOOL IAveVisibilityControl::IsVisible()const
{
	return isVisible;
}
