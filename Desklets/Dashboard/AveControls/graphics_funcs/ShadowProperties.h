// TextShadowProperties.h: interface for the CTextShadowProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADOWPROPERTIES_H__9E67589C_5397_4D70_9DD2_F985F9079C57__INCLUDED_)
#define AFX_SHADOWPROPERTIES_H__9E67589C_5397_4D70_9DD2_F985F9079C57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

// getting dirty (changing access-modifiers in this macro, beware!), the herd style.
#define CSHADOWPROPERTY(t,n) private: ##t m_##n; \
public:	void Set##n (##t const & v) { m_##n = v; }; \
		##t Get##n ()const {return m_##n;}

class CShadowProperties
{
	CSHADOWPROPERTY(INT, XOffset);
	CSHADOWPROPERTY(INT, YOffset);
	CSHADOWPROPERTY(INT, ExtraWidth);
	CSHADOWPROPERTY(INT, ExtraHeight);
	CSHADOWPROPERTY(Color, Color);
	CSHADOWPROPERTY(float, BlurRatio);
	CSHADOWPROPERTY(UINT, Intensity);

public:
	CShadowProperties();

	CShadowProperties(INT xOffset,INT yOffset, INT extraWidth, INT extraHeight, Color color, float blurRatio, UINT intensity);

	~CShadowProperties();
};

#endif // !defined(AFX_SHADOWPROPERTIES_H__9E67589C_5397_4D70_9DD2_F985F9079C57__INCLUDED_)
