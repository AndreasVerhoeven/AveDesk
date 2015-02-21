// IAveUsingFontImpl.cpp: implementation of the IAveUsingFontImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "IAveUsingFontImpl.h"
#include "AveConstants.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IAveUsingFontImpl::IAveUsingFontImpl() : 
	fontName(L"Lucida Sans Unicode"),
	fontSize(12.0f),
	fontStyle(FontStyleBold),
	fontColor(255,0,0,0)
{

}

IAveUsingFontImpl::~IAveUsingFontImpl()
{

}

void IAveUsingFontImpl::SetFontName(const std::wstring& name)
{
	fontName = name;
}

void IAveUsingFontImpl::SetFontSize(FLOAT size)
{
	fontSize = size;
}
void IAveUsingFontImpl::SetFontStyle(INT style)
{
	fontStyle = style;
}

void IAveUsingFontImpl::SetFontColor(Color c)
{
	fontColor = c;
}

std::wstring IAveUsingFontImpl::GetFontName()const
{
	return fontName;
}

FLOAT IAveUsingFontImpl::GetFontSize()const
{
	return fontSize;
}

INT IAveUsingFontImpl::GetFontStyle()const
{
	return fontStyle;
}

Color IAveUsingFontImpl::GetFontColor()const
{
	return fontColor;
}

DWORD_PTR IAveUsingFontImpl::InternalFontMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	switch(msg)
	{
		case AV_SET_TEXTCOLOR:
			handled = TRUE;
			SetFontColor(Color((ARGB)lParam));
			return 0;
		break;

		case AV_GET_TEXTCOLOR:
			handled = TRUE;
			return (DWORD_PTR) GetFontColor().GetValue();
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
				SetFontName(af->faceName);
				SetFontSize(af->size);
				SetFontStyle(af->style);

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

				af->style = GetFontStyle();
				af->size = GetFontSize();
				wcscpy_s(af->faceName, LF_FACESIZE, GetFontName().c_str());

				return TRUE;
			}
		break;
		
		default:
			handled = FALSE;
			return 0;
	}
}