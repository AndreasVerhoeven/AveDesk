// AveHyperLink.cpp: implementation of the CAveHyperLink class.
//
//////////////////////////////////////////////////////////////////////

#include "AveHyperLink.h"
#include "AveConstants.h"
#include "AveControlHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveHyperLink::CAveHyperLink(IAveControlContainer* container, std::string name)
	: CAveLabel(container, name),
	oldCursor(NULL),
	mouseIsOn(FALSE)
{
	DeskletLayerSetNoAlphaBlendedHitTesting(GetContainer()->GetDeskletHandle(), GetLayerId(), TRUE);

	CAveFont& font = CAveLabel::GetFont();
	font.SetFontColor(Color(255,0,0,255));
	font.SetFontStyle( font.GetFontStyle() | FontStyleUnderline);

	mouseOverFont.SetFromFont(font);
	mouseOverFont.SetFontColor(Color(255,255,0,0));
}

CAveHyperLink::~CAveHyperLink()
{

}

CAveFont& CAveHyperLink::GetFont()
{
	if(mouseIsOn)
		return mouseOverFont;
	else
		return CAveLabel::GetFont();
}

BOOL CAveHyperLink::MouseOn(const POINT* pt, const SIZE* s)
{
	HCURSOR hand = LoadCursor(NULL, IDC_HAND);
	oldCursor = DeskletSetCursor(GetContainer()->GetDeskletHandle(), hand);
	mouseIsOn = TRUE;
	Update();

	return FALSE;
}

BOOL CAveHyperLink::MouseOut()
{
	DeskletSetCursor(GetContainer()->GetDeskletHandle(), oldCursor);

	mouseIsOn = FALSE;
	Update();

	return FALSE;
}

BOOL CAveHyperLink::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	if(onLinkClick != NULL)
		onLinkClick(callbackData, this, pt, s, keys);

	return TRUE;
}

void CAveHyperLink::SetOnLinkClickCallback(void* data, OnLinkClick func)
{
	callbackData = data;
	onLinkClick = func;
}

DWORD_PTR CAveHyperLink::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	DWORD_PTR res = CAveLabel::InternalMessage(msg, wParam, lParam, handled);
	if(handled)
		return res;

	switch(msg)
	{
		case AV_HYPERLINK_SET_CLICK_CALLBACK:
			SetOnLinkClickCallback((void*)wParam, (OnLinkClick)lParam);
			return 0;
		break;

		case AV_SET_MOUSEOVER_SHADOWPROPERTIES:
			handled = TRUE;

			if(lParam != NULL)
			{
				CShadowProperties sp;
				CAveControlHelper::ConvertToCShadowProperties((AVE_SHADOWPROPERTIES*)lParam, sp);
				mouseOverFont.SetShadowProperties(sp);
			}
			mouseOverFont.SetDrawShadow((BOOL)wParam);
			Update();

			return 0;
		break;

		case AV_GET_MOUSEOVER_SHADOWPROPERTIES:
			handled = TRUE;

			if(wParam != NULL)
				*((BOOL*)wParam) = mouseOverFont.GetDrawShadow();

			if(lParam != NULL)
				CAveControlHelper::ConvertToShadowProperties(mouseOverFont.GetShadowProperties(), (AVE_SHADOWPROPERTIES*)lParam);

			return 0;
		break;

		case AV_SET_MOUSEOVER_ALIGNMENT:
			handled = TRUE;
			mouseOverFont.SetAlignment((INT)wParam, (INT) lParam);
			Update();
			return 0;
		break;

		case AV_GET_MOUSEOVER_HORALIGMENT:
			handled = TRUE;
			return (DWORD_PTR)mouseOverFont.GetHorizontalAligment();
		break;
	
		case AV_GET_MOUSEOVER_VERTALIGNMENT:
			handled = TRUE;
			return (DWORD_PTR)mouseOverFont.GetVerticalAlignment();
		break;
			

		case AV_SET_MOUSEOVER_TEXTCOLOR:
			handled = TRUE;
			mouseOverFont.SetFontColor(Color((ARGB)lParam));
			Update();
			return 0;
		break;

		case AV_GET_MOUSEOVER_TEXTCOLOR:
			handled = TRUE;
			return (DWORD_PTR) mouseOverFont.GetFontColor().GetValue();
		break;

		case AV_SET_MOUSEOVER_FONT2:
			handled = TRUE;

			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{
				AVE_FONT2* af = (AVE_FONT2*)lParam;
				mouseOverFont.SetFontName(af->faceName);
				mouseOverFont.SetFontSize(af->size);
				mouseOverFont.SetFontStyle(af->style);
				Update();

				return TRUE;
			}
		break;

		case AV_GET_MOUSEOVER_FONT2:
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
				wcscpy_s(af->faceName, LF_FACESIZE, font.GetFontName().c_str());

				return TRUE;
			}
		break;

		default:
			handled = FALSE;
		return 0;
	}
}

BOOL CAveHyperLink::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	return TRUE;
}

BOOL CAveHyperLink::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	return FALSE;
}

BOOL CAveHyperLink::Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(HasFocus() && WM_KEYDOWN == msg)
	{
		int nVirtKey = (int) wParam;
		if(VK_RETURN == nVirtKey)
		{
			if(onLinkClick != NULL)
			{
				POINT pt = {0};
				SIZE s = GetScale();
				DWORD keys = 0;
				onLinkClick(callbackData, this, &pt, &s, keys);
			}

			return TRUE;
		}

	}

	return FALSE;
}