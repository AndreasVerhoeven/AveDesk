// LabelProperties.cpp: implementation of the CLabelProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LabelProperties.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define INITPROP(n,v) m_##n = v;

CLabelProperties::CLabelProperties()
{
	INITPROP(HasShadow,true);
	INITPROP(Font,L"Arial");
	//INITPROP(Color,Color::White);
	m_Color.SetFromCOLORREF((COLORREF)Color::White);
	INITPROP(Style,0);
	INITPROP(Size,14);
}

CLabelProperties::~CLabelProperties()
{

}
