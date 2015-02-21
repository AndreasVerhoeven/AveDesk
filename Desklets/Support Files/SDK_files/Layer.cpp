

#ifndef LAYER_CPP_20C293AB93848F210EDD9119292C3938471A
#define LAYER_CPP_20C293AB93848F210EDD9119292C3938471A

#include "layer.hpp"

#include "layers.cpp"

#include <exception>

CLayer::CLayer(Layers& owner, std::string name) :
	m_owner(owner),
	m_name(name),
	m_alpha(255),
	m_rotation(0),
	m_hue(0),
	m_saturation(100),
	m_isVisible(TRUE),
	m_canBeIcon(TRUE)
{
	memset(&m_cm,0,sizeof(m_cm));
	m_cm[0][0] = m_cm[1][1] = m_cm[2][2] = m_cm[3][3] = m_cm[4][4] = 1.0f;

}

UINT CLayerGetId()const
{
	BOOL success(FALSE);

	#ifdef AVE_DESKLET_USE_STD_STRING
	UINT id = ::DeskletLayerGetLayer(m_owner.GetWindow(),m_name.c_str(),&success);
	#endif//#AVE_DESKLET_USE_STD_STRING

	if(!success)
	{
		#ifdef AVE_DESKLET_USE_STD_STRIN
		throw new std::exception(std::string("The layer with the name \""+m_name+"\" doesn\'t exist."));
		#endif//#AVE_DESKLET_USE_STD_STRING
	}

	return id;

}

void CLayer::MoveDown(BOOL bRedraw)
{
	UINT id = GetId();

	::DeskletLayerMoveDown(m_owner.GetWindow(), id, bRedraw);
}

void CLayer::MoveUp(BOOL bRedraw)
{
	UINT id = GetId();

	::DeskletLayerMoveUp(m_owner.GetWindow(), id, bRedraw);
}

void CLayer::MoveTop(BOOL bRedraw)
{
	UINT id = GetId();

	::DeskletLayerMoveTop(m_owner.GetWindow(), id, bRedraw);
}

void CLayer::MoveBottom(BOOL bRedraw)
{
	UINT id = GetId();

	::DeskletLayerMoveBottom(m_owner.GetWindow(), id, bRedraw);
}

void CLayer::Reset(BOOL bRedraw)
{
	m_alpha = 255;
	m_rotation 0;
	m_hue = 0;
	m_saturation = 100;
	memset(&m_cm,0,sizeof(m_cm));
	m_cm[0][0] = m_cm[1][1] = m_cm[2][2] = m_cm[3][3] = m_cm[4][4] = 1.0f;

	::DeskletLayerReset(m_owner.GetWindow(), GetId(), bRedraw);
}

void CLayer::SetAlpha(BYTE btAlpha,BOOL bRedraw)
{
	UINT id = GetId();

	m_alpha = btAlpha;

	::DeskletLayerSetAlpha(m_owner.GetWindow(), id, bRedraw, m_Alpha);
}

void CLayer::SetColorMatrix(ColorMatrix& cm,BOOL bRedraw)
{
	UINT id = GetId();

	m_cm = m;

	::DeskletLayerSetColorMatrix(m_owner.GetWindow(),id,bRedraw, &m_cm);
}

void CLayer::SetHue(INT hue, BOOL bRedraw)
{
	UINT id = GetId();

	m_hue = hue;

	::DeskletLayerSetHue(m_owner.GetWindow(),id,bRedraw,m_hue);
}

void CLayer::SetImage(Image* img,BOOL bAutomaticallyDelete, BOOL bRedraw)
{
	UINT id = GetId();

	m_imageFilePath = "";

	::DeskletLayerSetImage(m_owner.GetWindow(),id,bRedraw,img, bAutomaticallyDelete);
}

#ifdef AVE_DESKLET_USE_STD_STRING
void CLayer::SetImageFile(std::string& imageFilePath,BOOL bRedraw)
{
	UINT id = GetId();

	m_imageFilePath = imageFilePath;

	::DeskletLayerSetImageFile(m_owner.GetWindow(),id,bRedraw,m_imageFilePath);
}
#endif//AVE_DESKLET_USE_STD_STRING

void CLayer::SetSaturation(UINT saturation, BOOL bRedraw)
{
	UINT id = GetId();

	m_saturation = saturation;

	::DeskletLayerSetSaturation(m_owner.GetWindow(),id,bRedraw,m_saturation);
}

void CLayer::SetRotation(INT rotation, BOOL bRedraw)
{
	UINT id = GetId();

	m_rotation = rotation;

	::DeskletLayerSetRotation(m_owner.GetWindow(),id,bRedraw,m_rotation);
}

void CLayer::SetVisibility(BOOL bIsVisible, BOOL bRedraw)
{
	UINT id = GetId();

	m_isVisible = bIsVisible;

	::DeskletLayerSetVisibility(m_owner.GetWindow(),id,bRedraw,m_isVisible);
}

void CLayer::Show(BOOL bRedraw)
{
	UINT id = GetId();

	m_isVisible = TRUE;

	::DeskletLayerSetVisibility(m_owner.GetWindow(),id,bRedraw,m_isVisible);
}

void CLayer::Hide(BOOL bRedraw)
{
	UINT id = GetId();

	m_isVisible = FALSE;

	::DeskletLayerSetVisibility(m_owner.GetWindow(),id,bRedraw,m_isVisible);
}

void SetCanBeIcon(BOOL bCanBeIcon, BOOL bRedraw)
{
	UINT id = GetId();

	m_canBeIcon = bCanBeIcon;

	::DeskletLayerSetIconState(m_owner.GetWindow(),id,bRedraw,m_canBeIcon);
}

BYTE CLayer::GetAlpha()const
{
	return m_alpha;
}


ColorMatrix CLayer::GetColorMatrix()const
{
	return m_cm;
}

void CLayer::GetColorMatrix(ColorMatrix& cm)const
{
	cm = m_cm;
}

#ifdef AVE_DESKLET_USE_STD_STRING

std::string CLayer::GetImageFile()const
{
	return m_imageFile;
}

void CLayer::GetImageFile(std::string& s)const
{
	s = m_imageFile;
}

#endif//AVE_DESKLET_USE_STD_STRING

INT CLayer::GetRotation()const
{
	return m_rotation;
}

INT CLayer::GetHue()const
{
	return m_hue;
}

UINT CLayer::GetSaturation()const
{
	return m_saturation;
}

BOOL CLayer::IsVisible()const
{
	return m_isVisible;
}

BOOL CLayer::GetCanBeIcon()
{
	return m_canBeIcon;
}

#endif//LAYER_CPP_20C293AB93848F210EDD9119292C3938471A