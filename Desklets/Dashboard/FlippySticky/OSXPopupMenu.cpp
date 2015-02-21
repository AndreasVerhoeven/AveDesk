
#define WINVER		0x0501
#define _WIN32_WINNT	0x0500
#include <windows.h>

#include <gdiplus.h>
using namespace Gdiplus;

#include <windowsx.h>

#include "osxpopupmenu.h"

struct MenuResources
{
	Bitmap* top_left;
	Bitmap* top_mid;
	Bitmap* top_right;

	Bitmap* mid_left;
	Bitmap* mid_mid;
	Bitmap* mid_right;

	Bitmap* sel_left;
	Bitmap* sel_mid;
	Bitmap* sel_right;

	Bitmap* bottom_left;
	Bitmap* bottom_mid;
	Bitmap* bottom_right;

	Bitmap* checkmark;
};

struct InternalMenuData
{
	HWND parent;
	UINT notifyMessage;
	HWND hwnd;

	DWORD mouseOverItem;
	DWORD menuWidth;
	DWORD menuHeight;

	MenuData data;
	MenuResources resources;

	BYTE alpha;
	POINT pt;
};

#define AVE_OSX_MENU_FONT           TEXT("Lucida Sans Unicode")
#define AVE_OSX_MENU_STYLES         FontStyleBold
#define AVE_OSX_MENU_FONTSIZE       12
#define AVE_OSX_MENU_FONTCOLOR      Color(255, 0, 0, 0)
#define AVE_OSX_MENU_SELFONTCOLOR   Color(255, 255, 255, 255)
#define AVE_OSX_BIGGEST_MENU_WIDTH 1024


BOOL SetBitmapToWindow(HWND hwnd, Bitmap* bmp, POINT pos, BYTE alpha)
{

	LONG exStyles = GetWindowLong(hwnd, GWL_EXSTYLE);
	exStyles |= WS_EX_LAYERED;
	SetWindowLong(hwnd, GWL_EXSTYLE, exStyles);

	HDC screenDC = GetDC(NULL);
	
	HDC dc = CreateCompatibleDC(screenDC);


	HBITMAP hBitmap;
	bmp->GetHBITMAP(NULL,&hBitmap);
	HBITMAP oldBitmapSelectedInDC = (HBITMAP) SelectObject(dc, hBitmap);

	SIZE newSize = {bmp->GetWidth(), bmp->GetHeight()};

	POINT ptNewPos = {pos.x, pos.y};
	POINT ptSourcePosition = {0, 0};

	BLENDFUNCTION blendFunction = {0};
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;

	blendFunction.SourceConstantAlpha = alpha;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;

	DWORD dwFlags = 2;

	COLORREF crKey = RGB(255,0,255); // dummy colourkey
	BOOL bRet = ::UpdateLayeredWindow(hwnd, screenDC, &ptNewPos,&newSize,dc,&ptSourcePosition, crKey,&blendFunction,dwFlags);
	DWORD lastError = GetLastError();

	SelectObject(dc, oldBitmapSelectedInDC);
	DeleteDC(dc);


	DeleteObject(hBitmap);


	ReleaseDC(NULL, screenDC);

	return bRet;
}

UINT GetStringLength(const char* str, UINT itemHeight)
{
	// 1024
	Rect rect(0,0,AVE_OSX_BIGGEST_MENU_WIDTH, itemHeight);
	
	OLECHAR wtext[10024] = {0};
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AVE_OSX_MENU_FONT , -1, wtext, sizeof(wtext));
#else
	wcscpy(wtext, AVE_OSX_MENU_FONT);
#endif

	Gdiplus::FontFamily  fontFamily(wtext);

	GraphicsPath path(Gdiplus::FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str , -1, wtext, sizeof(wtext));
	path.AddString(wtext, -1, &fontFamily, AVE_OSX_MENU_STYLES, (float)AVE_OSX_MENU_FONTSIZE, rect, &format);

	Rect rc;
	path.GetBounds(&rc);

	return rc.Width + 10;
}


UINT GetBiggestItemWidth(MenuData* data, int itemHeight)
{
	if(NULL == data)
		return 0;

	if(NULL == data->items)
		return 0;

	UINT biggest(0);
	for(UINT i = 0; i < data->count; ++i)
	{
		UINT len = GetStringLength(data->items[i], itemHeight);
		if(len > biggest)
			biggest = len;
	}

	return biggest;
}

void DrawRepeated(Bitmap* bmp, Graphics& g, const Rect& r)
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
	int drawsNeeded = r.Width / bmpWidth;
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

void DrawItem(const char* str, Graphics& g, Rect& r, MenuResources* res, BOOL selected, BOOL mouseOver)
{
	if(NULL == res)
		return;

	Bitmap* left  = res->mid_left;
	Bitmap* mid   = res->mid_mid;
	Bitmap* right = res->mid_right;

	if(mouseOver)
	{
		left  = res->sel_left;
		mid   = res->sel_mid;
		right = res->sel_right;
	}

	g.DrawImage(left, Rect(r.X, r.Y, left->GetWidth(), r.Height), 0, 0, left->GetWidth(), left->GetHeight(), UnitPixel, 0,0,0);
	Rect midRect(r.X + left->GetWidth(), r.Y, r.Width - left->GetWidth() - right->GetWidth(), r.Height);
	DrawRepeated(mid, g, midRect);
	g.DrawImage(right, Rect(r.Width - right->GetWidth(), r.Y, right->GetWidth(), r.Height), 0, 0, right->GetWidth(), right->GetHeight(), UnitPixel, 0,0,0);

	if(selected)
		g.DrawImage(res->checkmark, Rect(r.X + left->GetWidth(), r.Y, res->checkmark->GetWidth(), res->checkmark->GetHeight()), 0, 0, res->checkmark->GetWidth(), res->checkmark->GetHeight(), UnitPixel, 0,0,0);

	if(str != NULL)
	{
		OLECHAR wtext[10024] = {0};
#ifndef UNICODE
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AVE_OSX_MENU_FONT , -1, wtext, sizeof(wtext));
#else
		wcscpy(wtext, AVE_OSX_MENU_FONT);
#endif

		Gdiplus::FontFamily  fontFamily(wtext);
		SolidBrush  black(mouseOver ? AVE_OSX_MENU_SELFONTCOLOR : AVE_OSX_MENU_FONTCOLOR);

		GraphicsPath path(Gdiplus::FillModeAlternate);
		StringFormat format(0, LANG_NEUTRAL);
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);

		RectF rect((float)(r.X + left->GetWidth() + res->checkmark->GetWidth()), (float)r.Y, (float)(r.Width - res->checkmark->GetWidth() - left->GetWidth() - right->GetWidth()), (float)r.Height);

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str , -1, wtext, sizeof(wtext));
		path.AddString(wtext, -1, &fontFamily, AVE_OSX_MENU_STYLES, (float)AVE_OSX_MENU_FONTSIZE, rect, &format);

	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g.FillPath(&black, &path);
	}

}

BOOL CalculateMenuSize(InternalMenuData* data, SIZE* s)
{
	if(NULL == data)
		return FALSE;

	if(NULL == s)
		return FALSE;

	UINT height = 0;
	height += data->resources.top_mid->GetHeight();
	height += data->data.count * data->resources.mid_mid->GetHeight();
	height += data->resources.bottom_mid->GetHeight();

	UINT width = 0;
	width += data->resources.mid_left->GetWidth();
	width += data->resources.checkmark->GetWidth();
	width += data->menuWidth;
	width += data->resources.mid_right->GetWidth();

	s->cx = width;
	s->cy = height;
	
	return TRUE;
}

Bitmap* DrawMenu(InternalMenuData* data)
{
	if(NULL == data)
		return NULL;

	SIZE s  = {0};
	CalculateMenuSize(data, &s);
	UINT height = s.cy;
	UINT width = s.cx;

	Bitmap* bmp = new Bitmap(width, height);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	// draw top
	g.DrawImage(data->resources.top_left, Rect(0, 0, data->resources.top_left->GetWidth(), data->resources.top_left->GetHeight()), 0, 0, data->resources.top_left->GetWidth(), data->resources.top_left->GetHeight(), UnitPixel, 0,0,0);
	Rect r(data->resources.top_left->GetWidth(), 0, width - data->resources.top_left->GetWidth() - data->resources.top_right->GetWidth(), data->resources.top_right->GetHeight());
	DrawRepeated(data->resources.top_mid, g, r);
	Rect topRightRect(width - data->resources.top_right->GetWidth(),0, data->resources.top_right->GetWidth(), data->resources.top_right->GetHeight());
	g.DrawImage(data->resources.top_right, topRightRect, 0, 0, data->resources.top_right->GetWidth(), data->resources.top_right->GetHeight(), UnitPixel, 0,0,0);


	// draw mid
	Rect midR(0, data->resources.top_mid->GetHeight(), width, data->resources.mid_mid->GetHeight());
	for(UINT i = 0; i < data->data.count; ++i)
	{
		DrawItem(data->data.items[i], g, midR, &data->resources, i == data->data.selected, i == data->mouseOverItem);
		midR.Y += midR.Height;
	}

	// draw bottom
	int bottomHeight = data->resources.bottom_mid->GetHeight();
	int bottomDrawFrom = height - bottomHeight;
	g.DrawImage(data->resources.bottom_left, Rect(0, bottomDrawFrom, data->resources.bottom_left->GetWidth(), bottomHeight), 0, 0, data->resources.bottom_left->GetWidth(), data->resources.bottom_left->GetHeight(), UnitPixel, 0,0,0);
	Rect bottomR(data->resources.bottom_left->GetWidth(), bottomDrawFrom, width - data->resources.bottom_left->GetWidth() - data->resources.bottom_right->GetWidth(), bottomHeight);
	DrawRepeated(data->resources.bottom_mid, g, bottomR);
	g.DrawImage(data->resources.bottom_right,Rect(width - data->resources.bottom_right->GetWidth(), bottomDrawFrom, data->resources.bottom_right->GetWidth(), bottomHeight), 0, 0, data->resources.bottom_right->GetWidth(), data->resources.bottom_right->GetHeight(), UnitPixel, 0,0,0);


	return bmp;
}


extern Bitmap* __stdcall DeskletLoadGDIPlusImage(const char* szImage);
BOOL LoadMenuResource(Bitmap** bitmap, const char* resourcesPath, const char* file)
{
	if(NULL == bitmap)
		return FALSE;

	*bitmap = NULL;

	if(NULL == resourcesPath)
		return FALSE;

	if(NULL == file)
		return FALSE;

	char path[MAX_PATH] = {0};
	strcpy(path, resourcesPath);
	strcat(path, file);

	*bitmap = DeskletLoadGDIPlusImage(path);

	return *bitmap != NULL;
}


BOOL LoadMenuResources(MenuResources* res, const char* resourcesPath)
{
	if(NULL == res)
		return FALSE;

	LoadMenuResource(&res->bottom_left, resourcesPath, "bottom_left.png");
	LoadMenuResource(&res->bottom_mid, resourcesPath, "bottom_middle.png");
	LoadMenuResource(&res->bottom_right, resourcesPath, "bottom_right.png");

	LoadMenuResource(&res->mid_left, resourcesPath, "mid_left.png");
	LoadMenuResource(&res->mid_mid, resourcesPath, "mid_middle.png");
	LoadMenuResource(&res->mid_right, resourcesPath, "mid_right.png");


	LoadMenuResource(&res->sel_left, resourcesPath, "sel_left.png");
	LoadMenuResource(&res->sel_mid, resourcesPath, "sel_middle.png");
	LoadMenuResource(&res->sel_right, resourcesPath, "sel_right.png");

	LoadMenuResource(&res->top_left, resourcesPath, "top_left.png");
	LoadMenuResource(&res->top_mid, resourcesPath, "top_middle.png");
	LoadMenuResource(&res->top_right, resourcesPath, "top_right.png");

	LoadMenuResource(&res->checkmark, resourcesPath, "checkmark.png");


	return TRUE;

}

void FreeMenuResources(MenuResources* res)
{
	if(res != NULL)
	{
		delete res->bottom_left;
		delete res->bottom_mid;
		delete res->bottom_right;

		delete res->mid_left;
		delete res->mid_mid;
		delete res->mid_right;

		delete res->sel_left;
		delete res->sel_mid;
		delete res->sel_right;

		delete res->top_left;
		delete res->top_mid;
		delete res->top_right;

		delete res->checkmark;
	}
}

BOOL CopyMenuData(InternalMenuData* internal, const MenuData* data)
{
	if(NULL == internal || NULL == data)
		return FALSE;


	internal->data.count    = data->count;
	internal->data.selected = data->selected;
	internal->data.items    = NULL;

	internal->data.items = new char*[internal->data.count];
	if(NULL == internal->data.items)
		return FALSE;

	for(UINT i = 0; i < internal->data .count; ++i)
	{
		if(data->items[i] != NULL)
			internal->data .items[i] = strdup(data->items[i]);
		else
			internal->data.items = NULL;
	}

	return TRUE;
}

void FreeInternalMenuData(InternalMenuData* internal)
{
	if(internal != NULL)
	{
		for(UINT i = 0; i < internal->data.count; ++i)
			delete[] internal->data.items[i];
		delete internal->data.items;
	
		FreeMenuResources(&internal->resources);
		delete internal;
	}
}

BOOL MakeInternalMenuData(InternalMenuData** internal, HWND parent,
						  UINT notifyMessage, const MenuData* data,
						  const char* resourcesPath)
{
	if(NULL == internal)
		return FALSE;

	*internal = NULL;

	InternalMenuData* idm = new InternalMenuData;
	if(NULL == idm)
		return FALSE;

	ZeroMemory(idm, sizeof(InternalMenuData) );

	idm->hwnd = NULL;
	idm->parent = parent;
	idm->notifyMessage = notifyMessage;
	idm->mouseOverItem = -1;

	BOOL bRes = CopyMenuData(idm,data);
	if(!bRes)
	{
		FreeInternalMenuData(idm);
		return FALSE;
	}

	bRes = LoadMenuResources(&idm->resources, resourcesPath);
	if(!bRes)
	{
		FreeInternalMenuData(idm);
		return FALSE;
	}

	idm->menuWidth = GetBiggestItemWidth(&idm->data, idm->resources.mid_mid->GetWidth());
	
	*internal = idm;

	return TRUE;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

#define AVE_OSX_MENU_CLASS TEXT("AveOsxMenuClass")

LRESULT CALLBACK MenuWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

ATOM RegisterMenuClass()
{
	WNDCLASSEX cls = {0};
	cls.cbSize = sizeof(WNDCLASSEX);
	cls.cbClsExtra = NULL;
	cls.hInstance = HINST_THISCOMPONENT;
	cls.lpszClassName = AVE_OSX_MENU_CLASS;
	cls.style = CS_NOCLOSE;
	cls.lpfnWndProc = MenuWinProc;
	
	return RegisterClassEx(&cls);
}

BOOL InitializeMenu(HWND parent, UINT notifyMessage, const MenuData* items, const char* resourcesPath, const POINT* pt)
{
	InternalMenuData* data = NULL;
	BOOL bRes = MakeInternalMenuData(&data, parent, notifyMessage, items, resourcesPath);
	if(!bRes)
		return FALSE;

	if(pt != NULL)
		data->pt = *pt;
	else
		GetCursorPos(&data->pt);

	SIZE s = {0};
	CalculateMenuSize(data, &s);

	ATOM atom = RegisterMenuClass();
	
	DWORD exStyle = WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	DWORD style   = WS_VISIBLE | WS_POPUP;
	HWND hwnd = CreateWindowEx(exStyle, AVE_OSX_MENU_CLASS, TEXT(""), style, 0, 0, s.cx, s.cy, NULL, NULL, HINST_THISCOMPONENT, reinterpret_cast<LPVOID>(data));
	if(NULL == hwnd)
	{
		FreeInternalMenuData(data);
		return FALSE;
	}

	return TRUE;
}

BOOL DoOSXPopupMenu(HWND parent, UINT notifyMessage, const MenuData* items, const char* resourcesPath, const POINT* pt)
{
	return InitializeMenu(parent, notifyMessage, items, resourcesPath, pt);
}

#define AVE_OSX_INTERNAL_MENU_DATA_PROP_NAME TEXT("menu_data")

#define AVE_OSX_FADEIN_TIMER_ID  2949
#define AVE_OSX_FADEOUT_TIMER_ID 2950
#define AVE_OSX_ALPHA_DELTA      20
#define AVE_OSX_ALPHA_MAX        240
#define AVE_OSX_TIMER_INTERVAL   10

LRESULT OnMenuCreate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
	if(NULL == cs)
		return -1;

	InternalMenuData* data = reinterpret_cast<InternalMenuData*>(cs->lpCreateParams);
	if(NULL == data)
		return -1;

	BOOL bRes = SetProp(hwnd, AVE_OSX_INTERNAL_MENU_DATA_PROP_NAME, reinterpret_cast<void*>(data) );
	if(!bRes)
		return -1;

	Bitmap* bmp = DrawMenu(data);
	
	UINT itemHeight = data->resources.mid_mid->GetHeight();
	UINT checkMarkWidth = data->resources.checkmark->GetWidth();
	data->pt.x -= (data->resources.mid_left->GetWidth() + data->resources.checkmark->GetWidth());
	data->pt.y -= data->resources.top_mid->GetHeight();
	data->pt.y -= itemHeight * data->data.selected;

	int strStart = data->menuWidth / 2 - GetStringLength(data->data.items[data->data.selected], itemHeight) / 2;
	data->pt.x -= strStart;

	RECT desktopRect = {0};
	GetWindowRect(GetDesktopWindow(), &desktopRect);
	if(data->pt.x < desktopRect.left)
		data->pt.x = desktopRect.left;
	if(data->pt.y < desktopRect.top)
		data->pt.y = desktopRect.top;
	if(static_cast<int>(data->pt.x + bmp->GetWidth()) > desktopRect.right)
		data->pt.x = (desktopRect.right -  bmp->GetWidth());
	if(static_cast<int>(data->pt.y + bmp->GetHeight()) > desktopRect.bottom)
		data->pt.y = (desktopRect.bottom - bmp->GetHeight());

	data->alpha = AVE_OSX_ALPHA_DELTA;
	SetBitmapToWindow(hwnd, bmp, data->pt, data->alpha);
	delete bmp;

	::SetTimer(hwnd, AVE_OSX_FADEIN_TIMER_ID, AVE_OSX_TIMER_INTERVAL, NULL);

	return 0;
}

LRESULT OnMenuKillFocus(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	::KillTimer(hwnd, AVE_OSX_FADEIN_TIMER_ID);
	::SetTimer(hwnd, AVE_OSX_FADEOUT_TIMER_ID, AVE_OSX_TIMER_INTERVAL, NULL);
	return 0;
}

LRESULT OnMenuTimer(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(AVE_OSX_FADEIN_TIMER_ID == wParam)
	{
		InternalMenuData* data = reinterpret_cast<InternalMenuData*>(GetProp(hwnd, AVE_OSX_INTERNAL_MENU_DATA_PROP_NAME));
		if(data != NULL)
		{
			if(AVE_OSX_ALPHA_MAX - AVE_OSX_ALPHA_DELTA > data->alpha)
			{
				data->alpha += AVE_OSX_ALPHA_DELTA;

				BLENDFUNCTION blend;
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.AlphaFormat = 0;
				blend.SourceConstantAlpha = data->alpha;
 
				return ::UpdateLayeredWindow(hwnd, NULL, NULL, NULL, NULL, NULL,
                    NULL, &blend, ULW_ALPHA);
			}
			else
			{
				::KillTimer(hwnd, AVE_OSX_FADEIN_TIMER_ID);

				data->alpha = AVE_OSX_ALPHA_MAX;
				
				BLENDFUNCTION blend;
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.AlphaFormat = 0;
				blend.SourceConstantAlpha = data->alpha;
 
				::UpdateLayeredWindow(hwnd, NULL, NULL, NULL, NULL, NULL,
                    NULL, &blend, ULW_ALPHA);
			}
		}
	}
	else 
	if(AVE_OSX_FADEOUT_TIMER_ID == wParam)
	{
		InternalMenuData* data = reinterpret_cast<InternalMenuData*>(GetProp(hwnd, AVE_OSX_INTERNAL_MENU_DATA_PROP_NAME));
		if(data != NULL)
		{
			if(AVE_OSX_ALPHA_DELTA < data->alpha)
			{
				data->alpha -= AVE_OSX_ALPHA_DELTA;

				BLENDFUNCTION blend;
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.AlphaFormat = 0;
				blend.SourceConstantAlpha = data->alpha;
 
				return ::UpdateLayeredWindow(hwnd, NULL, NULL, NULL, NULL, NULL,
                    NULL, &blend, ULW_ALPHA);
			}
			else
			{
				::KillTimer(hwnd, AVE_OSX_FADEIN_TIMER_ID);

				DestroyWindow(hwnd);
			}
		}
	}

	return 0;
}

LRESULT OnMenuDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	InternalMenuData* data = reinterpret_cast<InternalMenuData*>(GetProp(hwnd, AVE_OSX_INTERNAL_MENU_DATA_PROP_NAME));
	if(data != NULL)
	{
		if(data->notifyMessage != 0 && data->parent != NULL)
			::SendMessage(data->parent, data->notifyMessage, static_cast<WPARAM>(data->data.selected), reinterpret_cast<LPARAM>(data->data.items[data->data.selected]));

		FreeInternalMenuData(data);
	}


	return 0;
}

LRESULT OnMenuMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	InternalMenuData* data = reinterpret_cast<InternalMenuData*>(GetProp(hwnd, AVE_OSX_INTERNAL_MENU_DATA_PROP_NAME));
	if(data != NULL)
	{
		int topCorrection = data->resources.top_mid->GetHeight();
		int itemHeight = data->resources.mid_mid->GetHeight();
		int leftCorrection = data->resources.mid_left->GetWidth();
		int checkMarkWidth = data->resources.checkmark->GetWidth();

		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);

		// BUGFIX! lParam gave wrong values
		POINT p = {0};
		p.x = GET_X_LPARAM(GetMessagePos());
		p.y = GET_Y_LPARAM(GetMessagePos());

		RECT r = {0};
		::GetWindowRect(hwnd, &r);
		p.x -= r.left;
		p.y -= r.top;
	
		xPos = p.x;
		yPos = p.y;

		xPos -= leftCorrection;
		yPos -= topCorrection;
		

		if(xPos >= 0 && xPos <= static_cast<int>(data->menuWidth + checkMarkWidth)  && yPos >= 0 && yPos <= static_cast<int>(itemHeight * data->data.count))
		{
			DWORD newMouseOverItem  = yPos / itemHeight;
			if(data->mouseOverItem != newMouseOverItem)
			{
				data->mouseOverItem = newMouseOverItem;	

				Bitmap* bmp = DrawMenu(data);
				SetBitmapToWindow(hwnd, bmp, data->pt, data->alpha);
				delete bmp;
			}
		}
		else
		{
			if(data->mouseOverItem != -1)
			{
				data->mouseOverItem = -1;

				Bitmap* bmp = DrawMenu(data);
				SetBitmapToWindow(hwnd, bmp, data->pt, data->alpha);
				delete bmp;
			}
		}

	}

	return 0;
}

LRESULT OnMenuLButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	InternalMenuData* data = reinterpret_cast<InternalMenuData*>(GetProp(hwnd, AVE_OSX_INTERNAL_MENU_DATA_PROP_NAME));
	if(data != NULL)
	{
		if(AVE_OSX_ALPHA_MAX == data->alpha) // means: animation is done
		{
			int topCorrection = data->resources.top_mid->GetHeight();
			int itemHeight = data->resources.mid_mid->GetHeight();
			int leftCorrection = data->resources.mid_left->GetWidth();
			int checkMarkWidth = data->resources.checkmark->GetWidth();

			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);

			POINT p = {0};
			p.x = GET_X_LPARAM(GetMessagePos());
			p.y = GET_Y_LPARAM(GetMessagePos());

			RECT r = {0};
			::GetWindowRect(hwnd, &r);
			p.x -= r.left;
			p.y -= r.top;
		
			xPos = p.x;
			yPos = p.y;

			xPos -= leftCorrection;
			yPos -= topCorrection;

			xPos -= leftCorrection;
			//yPos -= topCorrection;

			if(xPos >= 0 && xPos <= static_cast<int>(data->menuWidth + checkMarkWidth)  && yPos >= 0 && yPos <= static_cast<int>(itemHeight * data->data.count))
			{
				DWORD newSelected  = yPos / itemHeight;
				if(data->data.selected != newSelected)
				{
					data->data.selected= newSelected;	

					Bitmap* bmp = DrawMenu(data);
					SetBitmapToWindow(hwnd, bmp, data->pt, data->alpha);
					delete bmp;
				}
			}

			::KillTimer(hwnd, AVE_OSX_FADEIN_TIMER_ID);
			::SetTimer(hwnd, AVE_OSX_FADEOUT_TIMER_ID, AVE_OSX_TIMER_INTERVAL, NULL);
		}
	}

	return 0;
}

LRESULT CALLBACK MenuWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CREATE:
			return OnMenuCreate(hwnd, wParam, lParam);
			break;

		case WM_TIMER:
			return OnMenuTimer(hwnd, wParam, lParam);
			break;

		case WM_KILLFOCUS:
			return OnMenuKillFocus(hwnd, wParam, lParam);
			break;

		case WM_MOUSEMOVE:
			return OnMenuMouseMove(hwnd, wParam, lParam);
			break;

		case WM_DESTROY:
			return OnMenuDestroy(hwnd, wParam, lParam);
			break;

		case WM_LBUTTONUP:
			return OnMenuLButtonUp(hwnd, wParam, lParam);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}

}