// AveVerticalScrollBar.cpp: implementation of the CAveVerticalScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#include "aveconstants.h"
#include "AveVerticalScrollBar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

#define MIN_THUMBSIZE 20

CAveVerticalScrollBar::CAveVerticalScrollBar(IAveControlContainer* container, std::string name) :
	IAveControl(container, name),
		scrollbarHwnd(NULL),
		thumbSize(MIN_THUMBSIZE),
		projVisible(0),
		projTotal(0),
		thumbBmp(NULL),
		bgBmp(NULL),
		mouseIsDown(FALSE),
		callbackData(NULL),
		onScroll(NULL),
		downOnThumb(FALSE),
		thumbGrabPoint(0),
		prevDisabled(GetIsDisabled()),
		externalOnScroll(NULL)
{
	mouseDownPt.x = 0;
	mouseDownPt.y = 0;

	DeskletLayerSetNoAlphaBlendedHitTesting(GetContainer()->GetDeskletHandle(), GetLayerId(), 	TRUE);

	RECT r = GetRect();
	int height = r.bottom - r.top;
	int width = r.right - r.left;
	DWORD flags = SBS_VERT;
	scrollbarHwnd = ::CreateWindow(TEXT("SCROLLBAR"), "", flags, 0, 0, width, height, NULL, NULL, HINST_THISCOMPONENT, NULL);

}

DWORD_PTR CAveVerticalScrollBar::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	DWORD_PTR res = IAveControl::InternalMessage(msg, wParam, lParam, handled);
	if(handled)
		return res;

	switch(msg)
	{
		case AV_SCROLLBAR_SET_THUMBSIZE:
			handled = TRUE;
			SetThumbSize((UINT)wParam, (UINT)lParam);
			return 0;
		break;

		case AV_SCROLLBAR_SET_POS:
			handled = TRUE;
			SetPos((UINT)wParam);
			return 0;
		break;

		case AV_SCROLLBAR_GET_POS:
			handled = TRUE;
			return (DWORD_PTR)GetPos();
		break;

		case AV_SCROLLLBAR_SET_SCALE:
			handled = TRUE;
			SetScrollScale((UINT)wParam);
			return 0;
		break;

		case AV_SCROLLBAR_GET_SCALE:
			handled = TRUE;
			return (DWORD_PTR)GetScrollScale();
		break;

		case AV_SCROLLBAR_SET_ONSCROLL_CALLBACK:
			handled = TRUE;
			callbackData2 = (void*)wParam;
			externalOnScroll = (ExternalOnScroll)lParam;

			return 0;
		break;

		default:
			handled = FALSE;
			return 0;
	}
}

void CAveVerticalScrollBar::SetOnScroll(void* data, OnScroll func)
{
	callbackData = data;
	onScroll = func;
}

CAveVerticalScrollBar::~CAveVerticalScrollBar()
{
	if(::IsWindow(scrollbarHwnd))
		DestroyWindow(scrollbarHwnd);

	delete thumbBmp;
	delete bgBmp;
}

void CAveVerticalScrollBar::SetRect(RECT* r, SIZE* scaleSize)
{
	IAveControl::SetRect(r, scaleSize);

	RECT r2 = GetRect();
	int height = r2.bottom - r2.top;
	int width = r2.right - r2.left;
	SetWindowPos(scrollbarHwnd, NULL, 0, 0, height, width,SWP_NOZORDER | SWP_NOACTIVATE);	

	Update();
}

void CAveVerticalScrollBar::SetRect(int x, int y, int width, int height, UINT xScale, UINT yScale)
{
	IAveControl::SetRect(x, y, width, height, xScale, yScale);

	RECT r = GetRect();
	int h = r.bottom - r.top;
	int w = r.right - r.left;
	SetWindowPos(scrollbarHwnd, NULL, 0, 0, h, w,SWP_NOZORDER);

	Update();
}

UINT CAveVerticalScrollBar::GetPos()const
{
	return ::SendMessage(scrollbarHwnd, SBM_GETPOS, 0L, 0L);
}


void CAveVerticalScrollBar::InternalSetPos(UINT pos, bool notify)
{
	UINT curPos = GetPos();
	if(curPos != pos)
	{
		::SendMessage(scrollbarHwnd, SBM_SETPOS, (WPARAM)pos, (LPARAM)TRUE); 

		BOOL dontUpdate = FALSE;
		if(notify && onScroll!= NULL)
			dontUpdate = onScroll(callbackData, this);

		if(notify && externalOnScroll != NULL)
			dontUpdate |= externalOnScroll(callbackData2, this);

		Update();

	}
}

void CAveVerticalScrollBar::SetPos(UINT pos)
{
	InternalSetPos(pos, false);
}


UINT CAveVerticalScrollBar::GetScrollScale()const
{
	int min = 0;
	int max = 0;

	::SendMessage(scrollbarHwnd, SBM_GETRANGE, (WPARAM)&min, (LPARAM)&max);
	return max;
}


void CAveVerticalScrollBar::SetScrollScale(UINT scale)
{
	UINT curScale = GetScrollScale();
	if(curScale != scale)
	{
		::SendMessage(scrollbarHwnd, SBM_SETRANGE, (WPARAM)0L, (LPARAM)scale);
		Update();
	}
}

void CAveVerticalScrollBar::LockUpdate()
{
	Lock();
}

void CAveVerticalScrollBar::UnlockUpdate()
{
	Unlock();
}

void CAveVerticalScrollBar::SetThumbSize(UINT visible, UINT total)
{
	RECT r = GetRect();
	int height = r.bottom - r.top;

	UINT newThumbSize  = max((int)(float(visible) / float(total) * float(height)), MIN_THUMBSIZE);
	projVisible = visible;
	projTotal = total;
	if(newThumbSize != thumbSize)
	{
		thumbSize = newThumbSize;
		Update();
	}
}

BOOL CAveVerticalScrollBar::Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if((msg == WM_KEYUP || msg == WM_KEYDOWN || msg == WM_CHAR) && HasFocus())
	{
		UINT oldPos = GetPos();
		SendMessage(scrollbarHwnd, msg, wParam, lParam);

		if(oldPos != GetPos())
			Update();

		return TRUE;
	}

	return FALSE;
}

void CAveVerticalScrollBar::Update()
{
	RECT r = GetRect();
	Rect layoutRect(0, 0, r.right - r.left, r.bottom - r.top);

	Bitmap* bmp = new Bitmap(layoutRect.Width, layoutRect.Height);
	Graphics g(bmp);

	std::string dir;
	if(GetIsDisabled())
		dir = "disabled\\";

	if(NULL == bgBmp || bgBmp->GetHeight() != layoutRect.Height || GetIsDisabled() != prevDisabled)
	{
		delete bgBmp;

		Bitmap* bar = GetResources()->GetBitmap(std::string("scrollbar//" + dir + "scrollbar.png").c_str());
		if(bar != NULL && bar->GetLastStatus() == Ok && bar->GetHeight() == layoutRect.Height)
		{
			bgBmp = bar;
			bar = NULL;
		}
		else
		{
			Bitmap* top = GetResources()->GetBitmap(std::string("scrollbar//" + dir + "scroll_top.png").c_str());
			bgBmp = new Bitmap(top->GetWidth(), layoutRect.Height);
			Graphics g(bgBmp);
			g.DrawImage(top, Rect(0, 0, top->GetWidth(), top->GetHeight()), 0, 0, top->GetWidth(), top->GetHeight(), UnitPixel, NULL, NULL, NULL);

			Bitmap* bottom = GetResources()->GetBitmap(std::string("scrollbar//" + dir + "scroll_bottom.png").c_str());
			g.DrawImage(bottom, Rect(0, layoutRect.Height - bottom->GetHeight(), bottom->GetWidth(), bottom->GetHeight()), 0, 0, bottom->GetWidth(), bottom->GetHeight(), UnitPixel, NULL, NULL, NULL);

			Bitmap* mid = GetResources()->GetBitmap(std::string("scrollbar//" + dir + "scroll_mid.png").c_str());
			Rect midRect(0, top->GetHeight(), mid->GetWidth(), layoutRect.Height - top->GetHeight() - bottom->GetHeight()); 
			TextureBrush midBrush(mid);
			g.FillRectangle(&midBrush, midRect);

			delete mid;
			delete bottom;
			delete top;
		}
		delete bar;
	}

	UINT pos   = GetPos();
	UINT scale = GetScrollScale();
	UINT thumb = thumbSize;

	// resize
	UINT height = layoutRect.Height;
	float ratio = 0.0f;
	if(height > 0)
		ratio = float(height-4) / float(height);

	thumb = (UINT)(float(thumb) * ratio);

	if(NULL == thumbBmp || thumbBmp->GetHeight() != thumb ||  GetIsDisabled() != prevDisabled)
	{
		delete thumbBmp;

		Bitmap* top = GetResources()->GetBitmap(std::string("scrollbar//" + dir + "thumb_top.png").c_str());
		thumbBmp = new Bitmap(top->GetWidth(), thumb);
		Graphics g(thumbBmp);
		g.DrawImage(top, Rect(0, 0, top->GetWidth(), top->GetHeight()), 0, 0, top->GetWidth(), top->GetHeight(), UnitPixel, NULL, NULL, NULL);

		Bitmap* bottom = GetResources()->GetBitmap(std::string("scrollbar//" + dir + "thumb_bottom.png").c_str());
		g.DrawImage(bottom, Rect(0, thumb - bottom->GetHeight(), bottom->GetWidth(), bottom->GetHeight()), 0, 0, bottom->GetWidth(), bottom->GetHeight(), UnitPixel, NULL, NULL, NULL);

		Bitmap* mid = GetResources()->GetBitmap(std::string("scrollbar//" + dir + "thumb_mid.png").c_str());
		Rect midRect(0, top->GetHeight(), mid->GetWidth(), thumb - top->GetHeight() - bottom->GetHeight()); 
		TextureBrush midBrush(mid);
		g.FillRectangle(&midBrush, midRect);

		delete mid;
		delete bottom;
		delete top;
	}

	float movingHeight = (float)(layoutRect.Height - thumbSize-2);

	float relPos = float(pos) / float(scale) * movingHeight + 2;
	pos = static_cast<UINT>(relPos);

	if(bgBmp != NULL)
		g.DrawImage(bgBmp, Rect(0, 0, bgBmp->GetWidth(), bgBmp->GetHeight()), 0, 0, bgBmp->GetWidth(), bgBmp->GetHeight(), UnitPixel, NULL, NULL, NULL);

	if(thumbBmp != NULL)
		g.DrawImage(thumbBmp, Rect(layoutRect.Width - thumbBmp->GetWidth(), pos, thumbBmp->GetWidth(), thumbBmp->GetHeight()), 0, 0, thumbBmp->GetWidth(), thumbBmp->GetHeight(), UnitPixel, NULL, NULL, NULL);

	DeskletLayerSetImage(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), bmp, TRUE);

	prevDisabled = GetIsDisabled();

}

Rect CAveVerticalScrollBar::GetThumbRect()
{
	RECT r = GetRect();
	Rect layoutRect(0, 0, r.right - r.left, r.bottom - r.top);

	UINT pos   = GetPos();
	UINT scale = GetScrollScale();
	UINT thumb = thumbSize;

	float movingHeight = static_cast<float>(layoutRect.Height - thumbSize);

	float relPos = float(pos) / float(scale) * movingHeight;
	pos = static_cast<UINT>(relPos);

	int width = layoutRect.Width;
	if(thumbBmp != NULL)
		width = thumbBmp->GetWidth();

	return Rect(layoutRect.Width - width, pos, width, thumb);

}

inline bool _AvePointInRect(Rect& r,const POINT& p)
{
	return (p.x > r.X && p.x < (r.X+r.Width) && p.y > (r.Y) && p.y < (r.Y + r.Height));
}

BOOL CAveVerticalScrollBar::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseDownPt = *pt;

	Rect thumbRect = GetThumbRect();
	if(_AvePointInRect(thumbRect, mouseDownPt))
	{
		thumbGrabPoint = mouseDownPt.y - thumbRect.Y;
		downOnThumb = TRUE;
	}
	else
	{
		downOnThumb = FALSE;

		UINT scale = GetScrollScale();
		UINT thumb = thumbSize;

		RECT r = GetRect();
		int height = r.bottom - r.top - thumb;

		float relUnits = float(scale) / float(height);
		InternalSetPos((int)(relUnits * float(pt->y )), true);

	}

	mouseIsDown = TRUE;
	DeskletLayerSetCapture(GetContainer()->GetDeskletHandle(), GetLayerId());

	return TRUE;
}

BOOL CAveVerticalScrollBar::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = FALSE;
	downOnThumb = FALSE;
	DeskletLayerReleaseCapture(GetContainer()->GetDeskletHandle());

	return TRUE;
}

BOOL CAveVerticalScrollBar::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	return TRUE;
}

BOOL CAveVerticalScrollBar::MouseOn(const POINT* pt, const SIZE* s)
{
	return TRUE;
}

BOOL CAveVerticalScrollBar::MouseMove(const POINT* pt, const SIZE* s)
{
	if(downOnThumb)
	{
		UINT scale = GetScrollScale();
		UINT thumb = thumbSize;

		RECT r = GetRect();
		int height = r.bottom - r.top - thumb;

		float relUnits = float(scale) / float(height);
		InternalSetPos((int)(relUnits * float(pt->y - thumbGrabPoint)), true);
	}
	return TRUE;
}

BOOL CAveVerticalScrollBar::MouseOut()
{
	return TRUE;
}

BOOL CAveVerticalScrollBar::MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	return TRUE;
}

BOOL CAveVerticalScrollBar::MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	return TRUE;
}