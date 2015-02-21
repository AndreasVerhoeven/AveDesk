// AveImageButton.cpp: implementation of the CAveImageButton class.
//
//////////////////////////////////////////////////////////////////////

#include "AveImageButton.h"
#include "AveConstants.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveImageButton::CAveImageButton(IAveControlContainer* container, std::string name) :
	IAveControl(container, name),
	isInPressedState(FALSE),
	mouseIsDown(FALSE),
	mouseWentOut(FALSE),
	clickHandler(NULL),
	retrieveImagesFromResources(FALSE)
{
	
}

CAveImageButton::~CAveImageButton()
{

}

DWORD_PTR CAveImageButton::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	DWORD_PTR res = IAveControl::InternalMessage(msg, wParam, lParam, handled);
	if(handled)
		return res;

	switch(msg)
	{
		case AV_IMAGEBUTTON_SET_ISPRESSED:
			SetIsInPressedState((BOOL)wParam);
			return 0;
		break;

		case AV_IMAGEBUTTON_GET_ISPRESSED:
			return (DWORD_PTR) GetIsInPressedState();
		break;

		case AV_IMAGEBUTTON_SET_CLICKHANDLER:
			SetClickHandler((void*)wParam, (ButtonClickHandler)lParam);
			return 0;
		break;

		case AV_SET_NORMALIMAGEFILE:

			if(NULL == lParam)
				return (DWORD_PTR)FALSE;

			SetNormalImageFile((WCHAR*)lParam, (BOOL)wParam);

			return (DWORD_PTR)TRUE;
		break;

		case AV_SET_DOWNIMAGEFILE:

			if(NULL == lParam)
				return (DWORD_PTR)FALSE;

			SetDownImageFile((WCHAR*)lParam, (BOOL)wParam);

			return (DWORD_PTR)TRUE;
		break;

		case AV_SET_PRESSEDIMAGEFILE:

			if(NULL == lParam)
				return (DWORD_PTR)FALSE;

			SetPressedImageFile((WCHAR*)lParam, (BOOL)wParam);

			return (DWORD_PTR)TRUE;
		break;

		case AV_SET_DISABLEDIMAGEFILE:

			if(NULL == lParam)
				return (DWORD_PTR)FALSE;

			SetDisabledImageFile((WCHAR*)lParam, (BOOL)wParam);

			return (DWORD_PTR)TRUE;
		break;

		case AV_SET_RETRIEVEIMAGESFROMRES:
			SetRetrieveImagesFromResources((BOOL)wParam);
			return 0;
		break;

		case AV_GET_RETRIEVEIMAGESFROMRES:
			return (DWORD_PTR) GetRetrieveImagesFromResources();
		break;

		default:
			handled = FALSE;
			return 0;
	}	
}

void CAveImageButton::SetIsInPressedState(BOOL state)
{
	if(isInPressedState != state)
	{
		isInPressedState = state;
		Update();
	}
}

void CAveImageButton::SetRetrieveImagesFromResources(BOOL val)
{
	if(retrieveImagesFromResources != val)
	{
		retrieveImagesFromResources = val;
		Update();
	}
}

BOOL CAveImageButton::GetRetrieveImagesFromResources()const
{
	return retrieveImagesFromResources;
}

BOOL CAveImageButton::GetIsInPressedState()const
{
	return isInPressedState;
}

std::wstring CAveImageButton::GetCurrentImageFile()
{
	std::wstring file(normalImgFile);


	if(GetIsDisabled())
		file = disabledImgFile;
	else if(mouseIsDown && !mouseWentOut)
		file = downImgFile;
	else if(GetIsInPressedState())
		file = pressedImgFile;

	if(GetRetrieveImagesFromResources())
		file = GetResources()->GetFilenameW(file.c_str());

	return file;
}

void CAveImageButton::Update()
{
	DeskletLayerSetImageFileW(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), GetCurrentImageFile().c_str());
}

void CAveImageButton::SetClickHandler(void* data, ButtonClickHandler handler)
{
	callbackData = data;
	clickHandler = handler;
}

BOOL CAveImageButton::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = TRUE;

	Update();

	return TRUE;
}

BOOL CAveImageButton::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = FALSE;
	Update();

	return FALSE;
}

BOOL CAveImageButton::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = FALSE;

	BOOL bRes = FALSE;
	if(clickHandler != NULL)
		bRes = clickHandler(callbackData, this, pt, s, keys);

	Update();

	return bRes;
}


void CAveImageButton::SetNormalImageFile(const std::wstring& file, BOOL update)
{
	normalImgFile = file;
	if(update)
		Update();
}

void CAveImageButton::SetDownImageFile(const std::wstring& file, BOOL update)
{
	downImgFile = file;
	if(update)
		Update();
}

void CAveImageButton::SetPressedImageFile(const std::wstring& file, BOOL update)
{
	pressedImgFile = file;
	if(update)
		Update();
}

void CAveImageButton::SetDisabledImageFile(const std::wstring& file, BOOL update)
{
	disabledImgFile = file;
	if(update)
		Update();
}

BOOL CAveImageButton::MouseOn(const POINT* pt, const SIZE* s)
{
	if(mouseIsDown && mouseWentOut)
	{
		mouseWentOut = FALSE;
		Update();
	}
	else
	{
		mouseWentOut = FALSE;
	}

	return TRUE;
}

BOOL CAveImageButton::MouseMove(const POINT* pt, const SIZE* s)
{
	return FALSE;
}

BOOL CAveImageButton::MouseOut()
{
	mouseWentOut = TRUE;
	if(mouseIsDown)
	{
		Update();
	}

	return TRUE;
}