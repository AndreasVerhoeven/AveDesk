// AveVerticalScrollBar.cpp: implementation of the CAveVerticalScrollBar class.
//
//////////////////////////////////////////////////////////////////////

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
		bgBmp(NULL)
{
	DeskletLayerSetNoAlphaBlendedHitTesting(GetContainer()->GetDeskletHandle(), GetLayerId(), 	TRUE);

	RECT r = GetRect();
	int height = r.bottom - r.top;
	int width = r.right - r.left;
	DWORD flags = SBS_VERT;
	scrollbarHwnd = ::CreateWindow(TEXT("SCROLLBAR"), "", flags, 0, 0, width, height, NULL, NULL, HINST_THISCOMPONENT, NULL);

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
	SetWindowPos(scrollbarHwnd, NULL, 0, 0, height, width,SWP_NOZORDER);	

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


void CAveVerticalScrollBar::SetPos(UINT pos)
{
	UINT curPos = GetPos();
	if(curPos != pos)
	{
		::SendMessage(scrollbarHwnd, SBM_SETPOS, (WPARAM)pos, (LPARAM)TRUE); 
		Update();
	}
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

void CAveVerticalScrollBar::Update()
{
	RECT r = GetRect();
	Rect layoutRect(0, 0, r.right - r.left, r.bottom - r.top);

	Bitmap* bmp = new Bitmap(layoutRect.Width, layoutRect.Height);
	Graphics g(bmp);

	if(NULL == bgBmp || bgBmp->GetHeight() != layoutRect.Width)
	{
		delete bgBmp;

		Bitmap* top = GetContainer()->GetResources()->GetBitmap("scroll_top.png");
		bgBmp = new Bitmap(top->GetWidth(), layoutRect.Height);
		Graphics g(bgBmp);
		g.DrawImage(top, Rect(0, 0, top->GetWidth(), top->GetHeight()), 0, 0, top->GetWidth(), top->GetHeight(), UnitPixel, NULL, NULL, NULL);

		Bitmap* bottom = GetContainer()->GetResources()->GetBitmap("scroll_bottom.png");
		g.DrawImage(bottom, Rect(0, layoutRect.Height - bottom->GetHeight(), bottom->GetWidth(), bottom->GetHeight()), 0, 0, bottom->GetWidth(), bottom->GetHeight(), UnitPixel, NULL, NULL, NULL);

		Bitmap* mid = GetContainer()->GetResources()->GetBitmap("scroll_mid.png");
		Rect midRect(0, top->GetHeight(), mid->GetWidth(), layoutRect.Height - top->GetHeight() - bottom->GetHeight()); 
		TextureBrush midBrush(mid);
		g.FillRectangle(&midBrush, midRect);

		delete mid;
		delete bottom;
		delete top;
	}

	UINT pos   = GetPos();
	UINT scale = GetScrollScale();
	UINT thumb = thumbSize;

	if(NULL == thumbBmp || thumbBmp->GetHeight() != thumb)
	{
		delete thumbBmp;

		Bitmap* top = GetContainer()->GetResources()->GetBitmap("thumb_top.png");
		thumbBmp = new Bitmap(top->GetWidth(), thumb);
		Graphics g(thumbBmp);
		g.DrawImage(top, Rect(0, 0, top->GetWidth(), top->GetHeight()), 0, 0, top->GetWidth(), top->GetHeight(), UnitPixel, NULL, NULL, NULL);

		Bitmap* bottom = GetContainer()->GetResources()->GetBitmap("thumb_bottom.png");
		g.DrawImage(bottom, Rect(0, thumb - bottom->GetHeight(), bottom->GetWidth(), bottom->GetHeight()), 0, 0, bottom->GetWidth(), bottom->GetHeight(), UnitPixel, NULL, NULL, NULL);

		Bitmap* mid = GetContainer()->GetResources()->GetBitmap("thumb_mid.png");
		Rect midRect(0, top->GetHeight(), mid->GetWidth(), thumb - top->GetHeight() - bottom->GetHeight()); 
		TextureBrush midBrush(mid);
		g.FillRectangle(&midBrush, midRect);

		delete mid;
		delete bottom;
		delete top;
	}

	float movingHeight = layoutRect.Height - thumbSize;

	float relPos = float(pos) / float(scale) * movingHeight;
	pos = relPos;

	if(bgBmp != NULL)
		g.DrawImage(bgBmp, Rect(0, 0, bgBmp->GetWidth(), bgBmp->GetHeight()), 0, 0, bgBmp->GetWidth(), bgBmp->GetHeight(), UnitPixel, NULL, NULL, NULL);

	if(thumbBmp != NULL)
		g.DrawImage(thumbBmp, Rect(0, pos, thumbBmp->GetWidth(), thumbBmp->GetHeight()), 0, 0, thumbBmp->GetWidth(), thumbBmp->GetHeight(), UnitPixel, NULL, NULL, NULL);

	DeskletLayerSetImage(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), bmp, TRUE);

}