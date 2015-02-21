// AveLabel.cpp: implementation of the CAveLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "AveLabel.h"
#include "AveControlHelper.h"
#include "AveConstants.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveLabel::CAveLabel(IAveControlContainer* container, std::string name) :
	IAveControl(container, name)
{

}

CAveLabel::~CAveLabel()
{

}

CAveFont& CAveLabel::GetFont()
{
	return font;
}

DWORD_PTR CAveLabel::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	DWORD_PTR res = IAveControl::InternalMessage(msg, wParam, lParam, handled);
	if(handled)
		return res;

	switch(msg)
	{
		case AV_SET_TEXT:
			handled = TRUE;
			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{
				SetText((WCHAR*)lParam);
				return TRUE;
			}
		break;

		case AV_GET_TEXT:
			handled = TRUE;
			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{	
				return CAveControlHelper::CopyWStringToWCHARPtr(GetText(), (WCHAR*)lParam, (size_t)wParam);
			}
		break;

		case AV_GET_TEXT_LEN:
			handled = TRUE;
			return GetText().length();
		break;

		case AV_SET_SHADOWPROPERTIES:
			handled = TRUE;

			if(lParam != NULL)
			{
				CShadowProperties sp;
				CAveControlHelper::ConvertToCShadowProperties((AVE_SHADOWPROPERTIES*)lParam, sp);
				font.SetShadowProperties(sp);
			}
			font.SetDrawShadow((BOOL)wParam);
			Update();

			return 0;
		break;

		case AV_GET_SHADOWPROPERTIES:
			handled = TRUE;

			if(wParam != NULL)
				*((BOOL*)wParam) = font.GetDrawShadow();

			if(lParam != NULL)
				CAveControlHelper::ConvertToShadowProperties(font.GetShadowProperties(), (AVE_SHADOWPROPERTIES*)lParam);

			return 0;
		break;

		case AV_SET_ALIGNMENT:
			handled = TRUE;
			font.SetAlignment((INT)wParam, (INT) lParam);
			Update();
			return 0;
		break;

		case AV_GET_HORALIGMENT:
			handled = TRUE;
			return (DWORD_PTR)font.GetHorizontalAligment();
		break;
	
		case AV_GET_VERTALIGNMENT:
			handled = TRUE;
			return (DWORD_PTR)font.GetVerticalAlignment();
		break;
			

		case AV_SET_TEXTCOLOR:
			handled = TRUE;
			font.SetFontColor(Color((ARGB)lParam));
			Update();
			return 0;
		break;

		case AV_GET_TEXTCOLOR:
			handled = TRUE;
			return (DWORD_PTR) font.GetFontColor().GetValue();
		break;

		case AV_SET_FONT2:
			handled = TRUE;

			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{
				AVE_FONT2* af = (AVE_FONT2*)lParam;
				font.SetFontName(af->faceName);
				font.SetFontSize(af->size);
				font.SetFontStyle(af->style);
				Update();

				return TRUE;
			}
		break;

		case AV_GET_FONT2:
			handled = TRUE;

			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{
				AVE_FONT2* af = (AVE_FONT2*)lParam;

				af->style = font.GetFontStyle();
				af->size = font.GetFontSize();
				wcscpy(af->faceName, font.GetFontName().c_str());

				return TRUE;
			}
		break;

		default:
			return 0;
	}
}

void CAveLabel::Update()
{
	// considering bitmap caching!
	RECT r = GetRect();
	Rect layoutRect(0, 0, r.right - r.left, r.bottom - r.top);
	Bitmap* bmp = new Bitmap(layoutRect.Width, layoutRect.Height);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	GetFont().Draw(g, layoutRect, GetText());

	DeskletLayerSetImage(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), bmp, TRUE);
}

void CAveLabel::SetText(const std::wstring ws)
{
	if(text != ws)
	{
		text = ws;
		Update();
	}
}

std::wstring CAveLabel::GetText()const
{
	return text;
}