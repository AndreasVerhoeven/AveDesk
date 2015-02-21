// AveImageButton.h: interface for the CAveImageButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEIMAGEBUTTON_H__3C1F2348_782A_4225_AB10_CE33EB46B582__INCLUDED_)
#define AFX_AVEIMAGEBUTTON_H__3C1F2348_782A_4225_AB10_CE33EB46B582__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"

class CAveImageButton : public IAveControl  
{

public:
	
	typedef BOOL (__stdcall *ButtonClickHandler)(void* ptr,CAveImageButton* button, const POINT* pt, const SIZE* s, DWORD keys);

protected:
	BOOL mouseIsDown;
	BOOL mouseWentOut;
	ButtonClickHandler clickHandler;
	void* callbackData;

	BOOL isInPressedState;

	std::wstring normalImgFile;
	std::wstring downImgFile;
	std::wstring pressedImgFile;
	std::wstring disabledImgFile;
	BOOL retrieveImagesFromResources;

public:

	CAveImageButton(IAveControlContainer* container, std::string name);
	virtual ~CAveImageButton();

	virtual void SetIsInPressedState(BOOL state);
	virtual BOOL GetIsInPressedState()const;

	virtual BOOL MouseDown(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseUp(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseClick(const POINT* pt, const SIZE* s, DWORD keys);

	virtual BOOL MouseOn(const POINT* pt, const SIZE* s);
	virtual BOOL MouseMove(const POINT* pt, const SIZE* s);
	virtual BOOL MouseOut();

	virtual void SetClickHandler(void* data, ButtonClickHandler handler);

	virtual void Update();

	virtual void SetNormalImageFile(const std::wstring& file, BOOL update);
	virtual void SetDownImageFile(const std::wstring& file, BOOL update);
	virtual void SetPressedImageFile(const std::wstring& file, BOOL update);
	virtual void SetDisabledImageFile(const std::wstring& file, BOOL update);

	virtual std::wstring GetCurrentImageFile();

	virtual void SetRetrieveImagesFromResources(BOOL val);
	virtual BOOL GetRetrieveImagesFromResources()const;

	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
};

#endif // !defined(AFX_AVEIMAGEBUTTON_H__3C1F2348_782A_4225_AB10_CE33EB46B582__INCLUDED_)
