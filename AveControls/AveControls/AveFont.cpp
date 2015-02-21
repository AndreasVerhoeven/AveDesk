// AveFont.cpp: implementation of the CAveFont class.
//
//////////////////////////////////////////////////////////////////////

#include "AveFont.h"
#include "aveconstants.h"

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
	INT a = 0;
	if(vert & ALIGNMENT_NEAR)a = 0;
	if(vert & ALIGNMENT_CENTRE)a = 1;
	if(vert & ALIGNMENT_FAR)a=2;

	INT b = 0;
	if(hor & ALIGNMENT_NEAR)b = 0;
	if(hor & ALIGNMENT_CENTRE)b = 1;
	if(hor & ALIGNMENT_FAR)b=2;
	strFormat.SetAlignment((StringAlignment)a);
	strFormat.SetLineAlignment((StringAlignment)b);	
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

SIZE CAveFont::GetSize(SIZE maxSize, const std::wstring& ws)
{
	FontFamily family(GetFontName().c_str());

	Rect rc(0,0, maxSize.cx, maxSize.cy);
	GraphicsPath path(FillModeAlternate);

	path.AddString(ws.c_str(),-1,&family,(FontStyle)GetFontStyle(),GetFontSize(),rc,&strFormat);

	Rect rcBounds;
	path.GetBounds(&rcBounds, NULL, NULL);

	SIZE s = {rcBounds.Width, rcBounds.Height};
	if(drawShadow)
	{
		s.cx += shadowProperties.GetXOffset() + 5;
		s.cy += shadowProperties.GetYOffset() + 5;
	}

	return s;
}

void CAveFont::Draw(Graphics& g, Rect& rc, const std::wstring& ws)
{
	SolidBrush brush(GetFontColor());
	FontFamily family(GetFontName().c_str());

	AveDrawString(&g,  &brush, ws.c_str(), &family, (FontStyle)GetFontStyle(), GetFontSize(), rc, &strFormat, drawShadow != FALSE, shadowProperties);
}