// TextShadowProperties.h: interface for the CTextShadowProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADOWPROPERTIES_H__9E67589C_5397_4D70_9DD2_F985F9079C57__INCLUDED_)
#define AFX_SHADOWPROPERTIES_H__9E67589C_5397_4D70_9DD2_F985F9079C57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;

#include "IPersistable.h"

// getting dirty (changing access-modifiers in this macro, beware!), the herd style.
#define CSHADOWPROPERTY(t,n) private: ##t m_##n; \
public:	void Set##n (##t const & v) { m_##n = v; }; \
		##t Get##n () {return m_##n;}

class CShadowProperties : public IPersistable
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

	DECLARE_PERSISTANCYNAME(ShadowProperties);

	BEGIN_LOADING_MAP()
		LOADABLE_PROP_PRIMITIVE(XOffset);
		LOADABLE_PROP_PRIMITIVE(YOffset);
		LOADABLE_PROP_PRIMITIVE(ExtraWidth);
		LOADABLE_PROP_PRIMITIVE(ExtraHeight);
		LOADABLE_PROP_PRIMITIVE(BlurRatio);
		LOADABLE_PROP_PRIMITIVE(Intensity);
		LOADABLE_PROP_PRIMITIVE(Color);
	END_LOADING_MAP()

	BEGIN_SAVING_MAP()
		SAVABLE_PROP_PRIMITIVE(XOffset);
		SAVABLE_PROP_PRIMITIVE(YOffset);
		SAVABLE_PROP_PRIMITIVE(ExtraWidth);
		SAVABLE_PROP_PRIMITIVE(ExtraHeight);
		SAVABLE_PROP_PRIMITIVE(BlurRatio);
		SAVABLE_PROP_PRIMITIVE(Intensity);
		SAVABLE_PROP_PRIMITIVE(Color);
	END_SAVING_MAP()
};

#endif // !defined(AFX_SHADOWPROPERTIES_H__9E67589C_5397_4D70_9DD2_F985F9079C57__INCLUDED_)
