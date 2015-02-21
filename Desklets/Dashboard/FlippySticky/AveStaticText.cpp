// AveStaticText.cpp: implementation of the CAveStaticText class.
//
//////////////////////////////////////////////////////////////////////

#include "AveStaticText.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveStaticText::CAveStaticText(IAveControlContainer* container, std::string name, BOOL noMessages) :
	IAveControl(container, name),
	fontName(L"Lucida Sans Unicode"),
	fontSize(14),
	color(255, 255, 255, 255),
	fontStyle(FontStyleBold)

{
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentCenter);

	DeskletLayerSetNoAlphaBlendedHitTesting(container->GetDeskletHandle(), GetLayerId(), TRUE);

	if(noMessages)
		DeskletLayerSetIsClickThrough(container->GetDeskletHandle(), GetLayerId(), TRUE);
}

CAveStaticText::~CAveStaticText()
{

}

void CAveStaticText::SetText(const std::wstring& str)
{
	if(text != str)
	{
		text = str;
		Update();
	}
}

std::wstring CAveStaticText::GetText()const
{
	return text;
}

void CAveStaticText::Update()const
{
	RECT r = GetRect();
	Rect layoutRect(0,0, r.right - r.left, r.bottom - r.top);
	Bitmap* bmp = new Bitmap(layoutRect.Width, layoutRect.Height);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	SolidBrush  brush(color);
	FontFamily fontFamily(fontName.c_str());

	GraphicsPath path(Gdiplus::FillModeAlternate);

	path.AddString(text.c_str(), -1, &fontFamily, FontStyleBold, fontSize, layoutRect, &format);
	g.FillPath(&brush, &path);

	DeskletLayerSetImage(container->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), (Image*) bmp, TRUE);
}