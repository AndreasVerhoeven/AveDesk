// TextShadowProperties.cpp: implementation of the CTextShadowProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShadowProperties.h"

#include <sstream>
#include <iomanip>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadowProperties::CShadowProperties(INT xOffset, INT yOffset, INT extraWidth, INT extraHeight, Color color, float blurRatio, UINT intensity)
	:
		m_XOffset(xOffset), m_YOffset(yOffset), m_ExtraWidth(extraWidth),
		m_ExtraHeight(extraHeight), m_Color(color), m_BlurRatio(blurRatio), 
		m_Intensity(intensity)
{
}

CShadowProperties::CShadowProperties()
	 :
		m_XOffset(1), m_YOffset(1),	m_ExtraWidth(0),
		m_ExtraHeight(0), m_Color(255,0,0,0), m_BlurRatio(1.0f),
		m_Intensity(20)
{

}

CShadowProperties::~CShadowProperties()
{

}

