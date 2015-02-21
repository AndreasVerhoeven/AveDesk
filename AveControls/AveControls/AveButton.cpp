// AveButton.cpp: implementation of the CAveButton class.
//
//////////////////////////////////////////////////////////////////////

#include "AveButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveButton::CAveButton(IAveControlContainer* container, std::string name) :
	IAveControl(container, name), mouseIsDown(FALSE), mouseWentOut(FALSE),
	clickHandler(NULL), callbackObject(NULL)
{
	DeskletLayerSetImageFile(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), 
			GetResources()->GetFilename("done.png").c_str());
}

CAveButton::~CAveButton()
{

}

void CAveButton::SetClickHandler(IAveCallback* callback, ButtonClickHandler handler)
{
	callbackObject = callback;
	clickHandler = handler;
}

BOOL CAveButton::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = TRUE;

	DeskletLayerSetImageFile(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), 
			GetResources()->GetFilename("done_pressed.png").c_str());

	return TRUE;
}

BOOL CAveButton::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	DeskletLayerSetImageFile(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), 
			GetResources()->GetFilename("done.png").c_str());

	mouseIsDown = FALSE;

	return TRUE;
}

BOOL CAveButton::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	DeskletLayerSetImageFile(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), 
			GetResources()->GetFilename("done.png").c_str());

	if(clickHandler != NULL && callbackObject != NULL)
		return clickHandler((void*)callbackObject, GetLayerId(), GetName().c_str(), pt, s, keys);
	else
		return FALSE;
}

BOOL CAveButton::MouseOn(const POINT* pt, const SIZE* s)
{
	if(mouseIsDown && mouseWentOut)
	{
		DeskletLayerSetImageFile(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), 
			GetResources()->GetFilename("done_pressed.png").c_str());
	}

	mouseWentOut = FALSE;

	return TRUE;
}

BOOL CAveButton::MouseMove(const POINT* pt, const SIZE* s)
{
	return FALSE;
}

BOOL CAveButton::MouseOut()
{
	mouseWentOut = TRUE;
	if(mouseIsDown)
	{
		DeskletLayerSetImageFile(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), 
			GetResources()->GetFilename("done.png").c_str());
	}

	return TRUE;
}

void CAveButton::Update()
{
	// this body is intentionaly left blank
}