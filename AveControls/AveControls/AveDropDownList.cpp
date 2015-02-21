// AveDropDownList.cpp: implementation of the CAveDropDownList class.
//
//////////////////////////////////////////////////////////////////////

#include "AveDropDownList.h"
#include "AveConstants.h"
#include "OSXPopupMenu.h"
#include "AveControlHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define WM_APP_DROPDOWN_MENU_NOTIFIER WM_APP + 2304

CAveDropDownList::CAveDropDownList(IAveControlContainer* container, std::string name) :
	IAveControl(container, name),
	selItem(-1),
	isPressed(FALSE),
	isDroppedDown(FALSE),
	mouseIsDown(FALSE),
	onSelChange(NULL)
{
	font.SetAlignment(StringAlignmentCenter, StringAlignmentCenter);
	CShadowProperties sp(font.GetShadowProperties());
}

CAveDropDownList::~CAveDropDownList()
{

}

CAveFont& CAveDropDownList::GetFont()
{
	return font;
}

DWORD_PTR CAveDropDownList::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	DWORD res = IAveControl::InternalMessage(msg, wParam, lParam, handled);
	if(handled)
		return res;

//	res = IAveUsingFontImpl::InternalFontMessage(msg, wParam, lParam, handled);
//	if(handled)
//		return res;

	switch(msg)
	{
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
			if((INT)wParam < 9 && (INT)lParam < 9)
			{
				font.SetAlignment((INT)wParam, (INT) lParam);
				Update();
			}
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
				wcscpy_s(af->faceName,LF_FACESIZE, font.GetFontName().c_str());

				return TRUE;
			}
		break;

		case AV_DROPLIST_INSERTITEM:
			handled = TRUE;
			if(NULL == lParam)
				return (DWORD_PTR)DROPLIST_ERR;

			return (DWORD_PTR) InsertItem((int)wParam, (WCHAR*)lParam);
		break;

		case AV_DROPLIST_ADDITEM:
			handled = TRUE;
			if(NULL == lParam)
				return (DWORD_PTR)DROPLIST_ERR;

			return (DWORD_PTR) AddItem((WCHAR*)lParam);
		break;

		case AV_DROPLIST_REMOVEITEM:
			handled = TRUE;
			return (DWORD_PTR)RemoveItem((int)wParam);
		break;
		
		case AV_DROPLIST_FINDITEM:
			handled = TRUE;
			if(NULL == lParam)
				return (DWORD_PTR)DROPLIST_ERR;

			return (DWORD_PTR) FindItem((WCHAR*)lParam, (int)wParam);
		break;

		case AV_DROPLIST_CLEAR:
			handled = TRUE;
			Clear();
			return 0;
		break;

		case AV_DROPLIST_COUNT:
			handled = TRUE;
			return (DWORD_PTR)Count();
		break;

		case AV_DROPLIST_SET_SEL:
			handled = TRUE;
			SetSel((int)wParam);
			return 0;
		break;

		case AV_DROPLIST_GET_SEL:
			handled = TRUE;
			return (DWORD_PTR) GetSel();
		break;


		case AV_DROPLIST_DROPDOWN:
			handled = TRUE;
			DropDown();
			return 0;
		break;

		case AV_DROPLIST_ISDROPPEDDOWN:
			handled = TRUE;
			return (DWORD_PTR)isDroppedDown;
		break;

		case AV_DROPLIST_SET_ONSELCHANGECALLBACK:
			handled = TRUE;
			SetOnSelChange((void*)wParam, (OnSelChange)lParam);
			return 0;
		break;

		case AV_DROPLIST_GET_ITEMSTRING:

			handled = TRUE;

			if((size_t)wParam >= 0 && (size_t)wParam < items.size())
			{
				std::wstring& ws = items[(size_t)wParam];
				return CAveControlHelper::CopyWStringToWCHARPtr(ws, (WCHAR*)lParam, ws.length() + 1);
			}
			else
			{
				return 0;
			}
		break;

		case AV_DROPLIST_GET_ITEMSTRING_LENGTH:
			handled = TRUE;

			if((size_t)wParam >= 0 && (size_t)wParam < items.size())
			{
				std::wstring& ws = items[(size_t)wParam];
				return ws.length();
			}
			else
			{
				return 0;
			}
		break;

		default:
			handled = FALSE;
			return 0;
		break;
	}
}

void CAveDropDownList::SetOnSelChange(void* data, OnSelChange func)
{
	callbackData = data;
	onSelChange = func;
}

int  CAveDropDownList::InsertItem(int index, const std::wstring& item)
{
	if(index >= 0 && index < (int)items.size())
	{
		items.insert(items.begin() + index, item);
	
		if(selItem == index - 1)
			Update();

		return index;
	}
	else
	{
		return -1;
	}
}

int  CAveDropDownList::AddItem(const std::wstring& item)
{
	items.push_back(item);
	return items.size() - 1;
}

BOOL  CAveDropDownList::RemoveItem(int index)
{
	if(index >= 0 && index < (int)items.size())
	{
		items.erase(items.begin() + index);
		if(selItem == index)
		{
			selItem = -1;
			Update();
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CAveDropDownList::GetSel()const
{
	return selItem;
}


int CAveDropDownList::SetSel(int sel)
{
	if(selItem != sel)
	{
		selItem = sel;
		Update();
	}

	return selItem;
}

int  CAveDropDownList::FindItem(const std::wstring& item, int begin)
{
	if(begin >= 0 && begin < (int)items.size())
	{
		int index = begin;
		for(std::vector<std::wstring>::iterator iter = items.begin() + begin; iter != items.end(); ++iter)
		{
			std::wstring& ws = *iter;
			if(ws == item)
				return index;

			index += 1;
		}

		return -1;
	}
	else
	{
		return -1;
	}
}

void CAveDropDownList::Clear()
{
	if(items.size() > 0)
	{
		selItem = -1;
		items.clear();
		Update();
	}
}

int CAveDropDownList::Count()const
{
	return items.size();
}

void _AveDrawRepeated(Bitmap* bmp, Graphics& g, const Rect& r)
{
	Bitmap* bitmap = bmp;
	UINT bmpHeight = bmp->GetHeight();
	if(static_cast<UINT>(r.Height) != bmpHeight)
	{
		bitmap = new Bitmap(bmp->GetWidth(), r.Height);
		Graphics g2(bitmap);
		g2.SetSmoothingMode(SmoothingModeAntiAlias);
		g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		g2.DrawImage(bmp, Rect(0, 0, bmp->GetWidth(), r.Height), 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel, 0,0,0);
	}

	int bmpWidth = bitmap->GetWidth();
	int drawsNeeded = 0;
	if(bmpWidth > 0)
		drawsNeeded = r.Width / bmpWidth;
	int filled = 0;
	for(int i=0; i < drawsNeeded; ++i)
	{
		g.DrawImage(bitmap, Rect(r.X + filled, r.Y, bitmap->GetWidth(), r.Height), 0, 0, bitmap->GetWidth(), bitmap->GetHeight(), UnitPixel, 0, 0, 0);
		filled += bmpWidth;
	}
	g.DrawImage(bitmap, Rect(r.X + filled, r.Y, r.Width - filled, r.Height), 0, 0, r.Width - filled, bitmap->GetHeight(), UnitPixel, 0, 0, 0);


	if(bitmap != bmp)
		delete bitmap;
}

void CAveDropDownList::Update()
{
	RECT r = GetRect();
	Rect layoutRect(0, 0, r.right - r.left, r.bottom - r.top);


	Bitmap* left  = NULL;
	Bitmap* mid   = NULL;
	Bitmap* right = NULL;

	std::string dir;
	if(GetIsDisabled())
		dir = "disabled\\";

	if(isPressed)
	{
		left  = GetResources()->GetBitmap(std::string("combobox\\" + dir + "combo_left.png").c_str());
		mid   = GetResources()->GetBitmap(std::string("combobox\\" + dir + "combo_mid.png").c_str());
		right = GetResources()->GetBitmap(std::string("combobox\\" + dir + "combo_right_pressed.png").c_str());
	}
	else
	{
		left  = GetResources()->GetBitmap(std::string("combobox\\" + dir + "combo_left.png").c_str());
		mid   = GetResources()->GetBitmap(std::string("combobox\\" + dir + "combo_mid.png").c_str());
		right = GetResources()->GetBitmap(std::string("combobox\\" + dir + "combo_right.png").c_str());
	}


	Bitmap* bmp = new Bitmap(layoutRect.Width, layoutRect.Height);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	int width = layoutRect.Width;
	int height = layoutRect.Height;
	

	Rect midRect(left->GetWidth(), 0, layoutRect.Width - left->GetWidth() - right->GetWidth(), layoutRect.Height);
	g.DrawImage(left, Rect(0,0, left->GetWidth(), height), 0,0, left->GetWidth(), left->GetHeight(), UnitPixel, 0, 0, 0);
	g.DrawImage(right, Rect(width - right->GetWidth(),0 , right->GetWidth(), height), 0,0, right->GetWidth(), right->GetHeight(), UnitPixel, 0, 0, 0);


	_AveDrawRepeated(mid, g, midRect);

	if(selItem >= 0 && selItem < (int)items.size())
	{
		std::wstring& text = items[selItem];

		/*Gdiplus::FontFamily  fontFamily(GetFontName().c_str());
		SolidBrush  black(fontColor);

		font.Draw(g, rect, text);

		GraphicsPath path(Gdiplus::FillModeAlternate);
		StringFormat format(0, LANG_NEUTRAL);
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);

		RectF rect(midRect.X, midRect.Height / 2 - GetFontSize() / 2 + 1, midRect.Width, GetFontSize());
		path.AddString(text.c_str(), -1, &fontFamily, GetFontStyle(), GetFontSize(), rect, &format);

		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		g.FillPath(&black, &path);*/
		Rect rect(midRect.X, midRect.Height / 2 - (UINT)font.GetFontSize() / 2 + 1, midRect.Width, (int)font.GetFontSize());
		font.Draw(g, rect, text);
	}

	DeskletLayerSetImage(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), bmp, TRUE);

	delete left;
	delete mid;
	delete right;
}

BOOL CAveDropDownList::DropDown()
{
	if(items.size() == 0) 
		return FALSE;

	WCHAR** texts = new WCHAR*[items.size()];
	int index = 0;
	for(std::vector<std::wstring>::iterator iter = items.begin(); iter != items.end(); ++iter)
	{
		texts[index++] = (WCHAR*)iter->c_str();
	}
				
	MenuData data = {0};
	data.selected = 0;
	data.count = items.size();
	data.items = texts;
	data.selected = max(GetSel(), 0);

	POINT pt = {0};
	RECT r = {0};
	GetWindowRect(GetContainer()->GetDeskletHandle(), &r);
	pt.x = r.left + GetRect().left;
	pt.y = r.top  + GetRect().top;

	isDroppedDown = TRUE;

	DoOSXPopupMenu(GetContainer()->GetDeskletHandle(), WM_APP_DROPDOWN_MENU_NOTIFIER, &data, GetResources()->GetFilename("menu\\").c_str(), &pt);
	delete[] texts;

	return TRUE;
}

BOOL CAveDropDownList::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = TRUE;
	isPressed = TRUE;

	Update();
	DropDown();

	return TRUE;
}

BOOL CAveDropDownList::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = FALSE;
	isPressed = FALSE;

	return TRUE;
}

BOOL CAveDropDownList::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	return TRUE;
}

BOOL CAveDropDownList::MouseOn(const POINT* pt, const SIZE* s)
{
	return TRUE;
}

BOOL CAveDropDownList::MouseMove(const POINT* pt, const SIZE* s)
{
	return TRUE;
}

BOOL CAveDropDownList::MouseOut()
{
	return TRUE;
}

BOOL CAveDropDownList::MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(GetSel() < Count() - 1)
	{
		int newSel = GetSel() + 1;
		if(onSelChange != NULL)
				onSelChange(callbackData, this, &newSel);

		if(newSel != GetSel())
			SetSel(newSel);
	}

	return TRUE;
}

BOOL CAveDropDownList::MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(GetSel() >= 1)
	{
		int newSel = GetSel() - 1;
		if(onSelChange != NULL)
				onSelChange(callbackData, this, &newSel);

		if(newSel != GetSel())
			SetSel(newSel);
	}
	return TRUE;
}

BOOL CAveDropDownList::Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(isDroppedDown && msg == WM_APP_DROPDOWN_MENU_NOTIFIER)
	{
		isDroppedDown = FALSE;


		int item = (int)wParam;
		if(item != GetSel())
		{
			if(onSelChange != NULL)
				onSelChange(callbackData, this, &item);

			if(item != GetSel())
				SetSel(item);
			else
				Update();
		}
		else
		{
			Update();
		}

		return TRUE;
	}
	return FALSE;
}