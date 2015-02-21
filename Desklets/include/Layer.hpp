//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef LAYER_HPP_20C293AB93848F210EDD9119292C3938471A
#define LAYER_HPP_20C293AB93848F210EDD9119292C3938471A

#include "DeskletSDK.h"

#define AVE_DESKLET_USE_STD_STRING
#ifdef AVE_DESKLET_USE_STD_STRING
#include <string>
#endif//AVE_DESKLET_USE_STD_STRING

class CLayers;

class CLayer
{
private:
	#ifdef AVE_DESKLET_USE_STD_STRING
	std::string m_name;
	std::string m_imageFilePath;
	#endif//AVE_DESKLET_USE_STD_STRING

	CLayers* m_owner;

	ColorMatrix m_cm;
	BYTE m_alpha;
	INT m_rotation;
	INT m_hue;
	UINT m_saturation;
	BOOL m_isVisible;
	BOOL m_canBeIcon;

public:
	CLayer(CLayers* owner, std::string const& name);
	CLayer();

	const CLayers& GetOwner();

	UINT GetId()const;

	void MoveDown(BOOL bRedraw=FALSE);
	void MoveUp(BOOL bRedraw=FALSE);
	void MoveTop(BOOL bRedraw=FALSE);
	void MoveBottom(BOOL bRedraw=FALSE);

	void Reset(BOOL bRedraw=FALSE);
	void SetAlpha(BYTE btAlpha,BOOL bRedraw=FALSE);
	void SetColorMatrix(ColorMatrix& cm,BOOL bRedraw=FALSE);

	void SetHue(INT hue, BOOL bRedraw=FALSE);
	void SetImage(Image* img, BOOL bAutomaticallyDelete, BOOL bRedraw=FALSE);

	#ifdef AVE_DESKLET_USE_STD_STRING
	void SetImageFile(std::string& imageFilePath,BOOL bRedraw=FALSE);
	#endif//AVE_DESKLET_USE_STD_STRING

	void SetSaturation(UINT saturation, BOOL bRedraw=FALSE);
	void SetRotation(INT rotation, BOOL bRedraw=FALSE);
	void SetVisibility(BOOL bIsVisible, BOOL bRedraw=FALSE);
	void Show(BOOL bRedraw=FALSE);
	void Hide(BOOL bRedraw=FALSE);
	void SetCanBeIcon(BOOL bCanBeIcon, BOOL bRedraw=FALSE);

	void SetPlacement(BOOL usePlacement,BOOL bRedraw=FALSE, int left=0, int top=0, int width=0, int height=0, int xScale=0, int yScale=0);

	BYTE GetAlpha()const;
	ColorMatrix GetColorMatrix()const;
	void GetColorMatrix(ColorMatrix& cm)const;
	INT GetRotation()const;
	INT GetHue()const;
	UINT GetSaturation()const;
	BOOL IsVisible()const;
	BOOL GetCanBeIcon();

	#ifdef AVE_DESKLET_USE_STD_STRING
	std::string GetImageFile()const;
	void GetImageFile(std::string& s)const;
	#endif//AVE_DESKLET_USE_STD_STRING

};

#endif//LAYER_HPP_20C293AB93848F210EDD9119292C3938471A