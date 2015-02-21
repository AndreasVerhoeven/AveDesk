// AveFont.cpp: implementation of the CAveFont class.
//
//////////////////////////////////////////////////////////////////////

#include "AveFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveFont::CAveFont() :
	drawShadow(FALSE),
	strFormat(0, LANG_NEUTRAL)
{
	strFormat.SetAlignment(StringAlignmentNear);
	strFormat.SetLineAlignment(StringAlignmentCenter);
}

CAveFont::~CAveFont()
{

}

void CAveFont::SetFromFont(const CAveFont& font)
{
	SetDrawShadow(font.GetDrawShadow());
	SetAlignment(font.GetVerticalAlignment(), font.GetHorizontalAligment());
	SetShadowProperties(font.shadowProperties);
	SetFontColor(font.GetFontColor());
	SetFontName(font.GetFontName());
	SetFontSize(font.GetFontSize());
	SetFontStyle(font.GetFontStyle());

}

INT CAveFont::GetVerticalAlignment()const
{
	return (INT)strFormat.GetAlignment();
}

INT CAveFont::GetHorizontalAligment()const
{
	return (INT)strFormat.GetLineAlignment();
}

void CAveFont::SetAlignment(INT vert, INT hor)
{
	strFormat.SetAlignment((StringAlignment)vert);
	strFormat.SetLineAlignment((StringAlignment)hor);	
}

BOOL CAveFont::GetDrawShadow()const
{
	return drawShadow;
}

void CAveFont::SetDrawShadow(BOOL val)
{
	if(drawShadow != val)
	{
		drawShadow = val;
	}
}

void CAveFont::SetShadowProperties(const CShadowProperties& props)
{
	shadowProperties = props;
}

CShadowProperties CAveFont::GetShadowProperties()
{
	return shadowProperties;
}

void CAveFont::Draw(Graphics& g, Rect& rc, const std::wstring& ws)
{
	SolidBrush brush(GetFontColor());
	FontFamily family(GetFontName().c_str());

	AveDrawString(&g,  &brush, ws.c_str(), &family, (FontStyle)GetFontStyle(), GetFontSize(), rc, &strFormat, drawShadow != FALSE, shadowProperties);
}