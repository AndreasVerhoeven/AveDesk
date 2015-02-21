#include ".\desklet.h"
#include "DeskletSDK.h"
#include <cassert>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "msimg32.lib")
#include <windowsx.h>
#include <cmath>
#include <winuser.h>
#include <sstream>
#include <gdiplus.h>

const double PI25DT = 3.141592653589793238462643f;

Desklet::Desklet(HWND hwnd, HINSTANCE hInstance) :
	hwnd(hwnd), hInstance(hInstance), dockHwnd(NULL),
		hBmp(NULL), ownDC(NULL), bgWidth(0), itemTop(14),
		aveAD(NULL), windowWidth(0), leftMargin(26), totalItemWidth(120),
		dockVisible(FALSE),
		counter(0), maxFrames(40), bgHeight(118), wasAttached(FALSE),
		prevMaxFrames(40), scroll(0), scrollWanted(0), scrollTimerRunning(FALSE),
		scrollCounter(0), scrollTodo(0),
		leftCounter(0), rightCounter(0),
		maxLeftFrames(40), maxRightFrames(40),
		mouseIsOn(FALSE),
		hCursorHand(NULL), hCursorNormal(NULL),
		hBmpDefault(NULL), defaultDC(NULL)
{

	hCursorHand   = ::LoadCursor(NULL,IDC_HAND);
	hCursorNormal = ::LoadCursor(NULL,IDC_ARROW);

	LoadHBitmap(L"images//body.png", hBmp, &bgWidth, &bgHeight);
	LoadHBitmap(L"images//left.png", hLeftSmall, &sLeftSmall);
	LoadHBitmap(L"images//left_big.png", hLeftBig, &sLeftBig);

	LoadHBitmap(L"images//right.png", hRightSmall, &sRightSmall);
	LoadHBitmap(L"images//right_big.png", hRightBig, &sRightBig);

	LoadHBitmap(L"images//default.png", hBmpDefault, NULL);

	HDC screenDC = ::GetDC(NULL);
	ownDC = ::CreateCompatibleDC(screenDC);
	::SelectObject(ownDC, hBmp);

	defaultDC = ::CreateCompatibleDC(screenDC);
	::SelectObject(defaultDC, hBmpDefault);

	::ReleaseDC(NULL, screenDC);

	items.reserve(100);
}

void Desklet::LoadHBitmap(const std::wstring& fname, HBITMAP& hBitmap, SIZE* s)
{
	if(s != NULL)
	{
		int t1=0, t2=0;
		LoadHBitmap(fname, hBitmap, &t1, &t2);
		s->cx = (LONG)t1;
		s->cy = (LONG)t2;
	}
	else
		LoadHBitmap(fname, hBitmap, NULL, NULL);
}

void Desklet::LoadHBitmap(const std::wstring& fname, HBITMAP& hBitmap, int* w, int* h)
{
	WCHAR path[MAX_PATH] = {0};
	::GetDLLPathW(hInstance, path, MAX_PATH);
	::PathAppendW(path, fname.c_str());

	Bitmap bmp(path);
	bmp.GetHBITMAP(NULL, &hBitmap);
	if(w != NULL)
		*w = bmp.GetWidth();
	if(h != NULL)
		*h = bmp.GetHeight();

}

void Desklet::ToggleDock()
{
	prevMaxFrames = maxFrames;
	maxFrames = 20;
	if(GetKeyState(VK_SHIFT) < 0)
		maxFrames = 50;

	float ratio = float(maxFrames) / float(prevMaxFrames);
	counter = (int)(float(counter) * ratio);

	if(dockVisible)
	{
		dockVisible = FALSE;
		::KillTimer(dockHwnd, 1);
		::SetTimer(dockHwnd, 2, 10, NULL);
	}
	else
	{
		if(NULL == aveAD)
		{
			scroll = 0;
			FetchItems();
		}
		SetWindowPos(dockHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
		::KillTimer(dockHwnd, 2);
		::SetTimer(dockHwnd, 1, 10, NULL);
		dockVisible = TRUE;
	}
}


Desklet::~Desklet(void)
{
	DestroyWindow(dockHwnd);
	::DeleteDC(ownDC);
	::DeleteDC(defaultDC);
	::DeleteObject(hBmp);
	::DeleteObject(hLeftSmall);
	::DeleteObject(hLeftBig);
	::DeleteObject(hRightSmall);
	::DeleteObject(hRightBig);
	::DeleteObject(hBmpDefault);
	FreeItems();
}

void Desklet::FreeItems()
{
	for(std::vector<_Item>::iterator iter = items.begin(); iter != items.end(); ++iter)
	{
		_Item& item = *iter;
		item.Destroy();
	}
	items.clear();
}

void Desklet::FetchItems()
{
	aveAD = AvailableDeskletsAPI::AveCreateADEnumerator(L"Desklets", (LPARAM)this);
	AvailableDeskletsAPI::AveStartEnumerating(aveAD, OnNewDeskletEntry);
}

BOOL CALLBACK Desklet::OnNewDeskletEntry(AvailableDeskletsAPI::AveAD* ad, const AvailableDeskletsAPI::AveADEntry* entry)
{
	SIZE size = {80, 80};
	Desklet* d = (Desklet*)ad->lParam;
	int left = ((int)d->items.size()) * d->totalItemWidth + d->leftMargin - d->scroll;
	RECT itemRect = {left, d->itemTop, left + d->totalItemWidth, d->bgHeight};
	_Item item;
	item.entry = *entry;
	d->items.push_back(item);
	InvalidateRect(d->dockHwnd, &itemRect, FALSE);

	AvailableDeskletsAPI::AveStartCreatingPreviewIcon(ad, entry, size, OnPreviewDone);
	return TRUE;
}

BOOL CALLBACK Desklet::OnPreviewDone(AvailableDeskletsAPI::AveAD* ad, const AvailableDeskletsAPI::AveADEntry* entry, HBITMAP hBmp)
{
	if(NULL == hBmp)
		return TRUE;

	SIZE size = {80, 80};
	Desklet* d = (Desklet*)ad->lParam;
	int left = ((int)d->items.size()) * d->totalItemWidth + d->leftMargin - d->scroll;
	RECT itemRect = {left, d->itemTop, left + d->totalItemWidth, d->bgHeight};
	
	_Item* pItem = NULL;
	int counter(0);
	for(std::vector<_Item>::iterator iter = d->items.begin(); iter != d->items.end(); ++iter)
	{
		_Item& item = *iter;
		if(item.entry.internalId == entry->internalId)
		{
			item.entry = *entry;
			item.AssignBitmap(hBmp, size);
			pItem = &item;

			left = counter * d->totalItemWidth + d->leftMargin - d->scroll;
			::SetRect(&itemRect,left, d->itemTop, left + d->totalItemWidth, d->bgHeight);

			break;
		}
		++counter;
	}

	if(NULL == pItem)
	{
		_Item item;
		item.entry = *entry;
		item.AssignBitmap(hBmp, size);
		d->items.push_back(item);
	}

	InvalidateRect(d->dockHwnd, &itemRect, FALSE);
	return TRUE;
}

int Desklet::ItemFromPoint(POINT pt)
{
	if(pt.y > itemTop && pt.y <= 80 + itemTop)
	{
		int l = pt.x - leftMargin + scroll;
		l /= totalItemWidth;
		if(l >=0 && l <(int)items.size())
			return (int)l;
	}

	return -1;
}

HWND Desklet::SetupDockWindow()
{
	// register window class
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	//wc.style = CS_DBLCLKS;
	wc.lpszClassName = _T("DockWindow");
	::RegisterClassEx(&wc);

	HWND desktopHwnd = ::GetDesktopWindow();
	RECT r = {0};
	::GetWindowRect(desktopHwnd, &r);
	r.top = r.bottom;
	r.bottom = r.top + bgHeight;
	windowWidth = r.right - r.left;

	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	DWORD dwStyle = WS_POPUP;
	dockHwnd = ::CreateWindowEx(dwExStyle, _T("DockWindow"), _T("AveDesk"), dwStyle, r.left, r.top, 
		r.right - r.left, r.bottom - r.top, NULL, NULL, hInstance, NULL);
	::SetProp(dockHwnd, _T("data"), (HANDLE) this);

	return dockHwnd;
}

LRESULT CALLBACK Desklet::WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	Desklet* d = (Desklet*)GetProp(hwnd, _T("data"));
	if(d != NULL)
		return d->OnMessage(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Desklet::OnMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(WM_SETCURSOR == msg)
	{
		DWORD msgPos = ::GetMessagePos();
		POINT screenPt = {GET_X_LPARAM(msgPos), GET_Y_LPARAM(msgPos)};
		POINT pt = screenPt;
		::ScreenToClient(dockHwnd, &pt);	

		SIZE* smallSize = (leftCounter  > 0) ? &sLeftBig  : &sLeftSmall;
		SIZE* bigSize   = (rightCounter > 0) ? &sRightBig : &sRightSmall;

		if(pt.x <= smallSize->cx || pt.x >= windowWidth - bigSize->cx)
			::SetCursor(hCursorHand);
		else
			::SetCursor(hCursorNormal);
	}
	else if(WM_MOUSELEAVE == msg)
	{
		mouseIsOn = FALSE;

		KillTimer(hwnd, 4);
		SetTimer(hwnd, 5, GetKeyState(VK_SHIFT) < 0 ? 100 : 15, NULL);

		KillTimer(hwnd, 6);
		SetTimer(hwnd, 7, GetKeyState(VK_SHIFT) < 0 ? 100 : 15, NULL);
	}
	else if(WM_MOUSEMOVE == msg)
	{
		if(!mouseIsOn)
		{
			TRACKMOUSEEVENT tme = {0};
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hwnd;
			TrackMouseEvent(&tme);
			mouseIsOn = TRUE;
		}

		DWORD msgPos = ::GetMessagePos();
		POINT screenPt = {GET_X_LPARAM(msgPos), GET_Y_LPARAM(msgPos)};
		POINT pt = screenPt;
		::ScreenToClient(dockHwnd, &pt);

		SIZE* smallSize = (leftCounter  > 0) ? &sLeftBig  : &sLeftSmall;
		SIZE* bigSize   = (rightCounter > 0) ? &sRightBig : &sRightSmall;

		if(pt.x <= smallSize->cx)
		{
			KillTimer(hwnd, 5);
			SetTimer(hwnd, 4, GetKeyState(VK_SHIFT) < 0 ? 100 : 15, NULL);
		}
		else
		{
			KillTimer(hwnd, 4);
			SetTimer(hwnd, 5, GetKeyState(VK_SHIFT) < 0 ? 100 : 15, NULL);
		}

		if(pt.x >= windowWidth - bigSize->cx)
		{
			KillTimer(hwnd, 7);
			SetTimer(hwnd, 6, GetKeyState(VK_SHIFT) < 0 ? 100 : 15, NULL);
		}
		else
		{
			KillTimer(hwnd, 6);
			SetTimer(hwnd, 7, GetKeyState(VK_SHIFT) < 0 ? 100 : 15, NULL);
		}
	}
	else if(WM_MOUSEWHEEL == msg)
	{
		RECT winRect = {0};
		::GetWindowRect(hwnd, &winRect);
		int windowWidth = winRect.right - winRect.left;
		int oldScroll = scroll;

		DWORD fwKeys = LOWORD(wParam);
		int zDelta = (short) HIWORD(wParam);

		int rotates = zDelta / WHEEL_DELTA;
		scrollWanted += 100 * rotates;

		if(scrollWanted < 0)
			scrollWanted = 0;

		if(scrollWanted > (int)items.size() * totalItemWidth - leftMargin*2)
			scrollWanted = (int)items.size() * totalItemWidth - leftMargin*2 - windowWidth;

		if(oldScroll != scrollWanted && !scrollTimerRunning)
		{
			scrollTodo = abs(scrollWanted - scroll);
			scrollTimerRunning = TRUE;
			::SetTimer(dockHwnd, 3, GetKeyState(VK_SHIFT) < 0 ? 50 : 25, NULL);
		}
	}
	else if(WM_TIMER == msg)
	{
		if(6 == wParam || 7 == wParam)
		{
			RECT winRect = {0};
			::GetWindowRect(hwnd, &winRect);
			int windowWidth = winRect.right - winRect.left;

			if(6 == wParam)
			{
				if(rightCounter >= maxRightFrames)
				{
					rightCounter = maxRightFrames;
					KillTimer(hwnd,6);
					return 0;
				}
				else
					rightCounter++;
			}
			else if(7 == wParam)
			{
				if(rightCounter <= 0)
				{
					rightCounter = 0;
					KillTimer(hwnd, 7);
					return 0;
				}
				else
					rightCounter--;
			}

			RECT rightRect = {windowWidth - sRightBig.cx, 0, windowWidth, bgHeight};
			InvalidateRect(hwnd, &rightRect, FALSE);
		}
		else if(4 == wParam || 5 == wParam)
		{
			if(4 == wParam)
			{
				if(leftCounter >= maxLeftFrames)
				{
					leftCounter = maxLeftFrames;
					KillTimer(hwnd,4);
					return 0;
				}
				else
					leftCounter++;
			}
			else if(5 == wParam)
			{
				if(leftCounter <= 0)
				{
					leftCounter = 0;
					KillTimer(hwnd, 5);
					return 0;
				}
				else
					leftCounter--;
			}

			RECT leftRect = {0, 0, sLeftBig.cx, bgHeight};
			InvalidateRect(hwnd, &leftRect, FALSE);

			return 0;
		}
		else if(3 == wParam)
		{
			int oldScroll = scroll;

			if(scrollWanted == scroll)
			{
				scrollCounter = 0;
				scrollTimerRunning = FALSE;
				::KillTimer(dockHwnd, 3);
				return 1;
			}

			const maxScrollFrames = 500;
			float ratio = (float)sin(float(scrollCounter) / float(maxScrollFrames) * PI25DT / 2.0f);
			if(scrollWanted > scroll)
				scroll = min(scrollWanted, scroll + (int)(ratio * float(scrollTodo)));
			else
				scroll = max(scrollWanted, scroll - (int)(ratio * float(scrollTodo)));

			if(scroll != oldScroll)
				InvalidateRect(dockHwnd, NULL, FALSE);

			scrollCounter++;
			if(scrollCounter >= maxScrollFrames)
				scrollCounter = maxScrollFrames - 2;

			return 0;
		}

		float ratio = (float)sin(float(counter) / float(maxFrames) * PI25DT / 2.0f);
		int newHeight = (int)(ratio * float(bgHeight));
		HWND desktopHwnd = ::GetDesktopWindow();
		RECT r = {0};
		::GetWindowRect(desktopHwnd, &r);
		int newTop = r.bottom - newHeight;
		::SetWindowPos(dockHwnd, 0, 0, newTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		RECT deskletRect = {0};
		::DeskletGetRect(this->hwnd, &deskletRect);
		if((deskletRect.bottom + 4) > newTop || wasAttached)
		{
			wasAttached = TRUE;
			int height = deskletRect.bottom - deskletRect.top;
			deskletRect.bottom = newTop - 4;
			deskletRect.top = deskletRect.bottom - height;
			::DeskletMove(this->hwnd, &deskletRect, FALSE);
		}

		::DeskletLayerSetRotation(this->hwnd, 0, TRUE, (int)(ratio * 360.0f));

		if(1 == wParam)
		{
			if(counter >= maxFrames)
			{
				::KillTimer(hwnd, 1);
				::InvalidateRect(hwnd, NULL, FALSE);
			}
			else
				counter++;
		}
		else if(2 == wParam)
		{
			if(counter == 0)
			{
				::KillTimer(hwnd, 2);
				FreeItems();

				if(aveAD != NULL)
				{
					AvailableDeskletsAPI::AveFreeADEnumerator(aveAD);
					aveAD = NULL;
				}
				ShowWindow(dockHwnd, SW_HIDE);
				wasAttached = FALSE;
			}
			else
				counter--;	
		}
	}
	else if(WM_LBUTTONDBLCLK == msg)
	{
		DWORD msgPos = ::GetMessagePos();
		POINT screenPt = {GET_X_LPARAM(msgPos), GET_Y_LPARAM(msgPos)};
		POINT pt = screenPt;
		::ScreenToClient(hwnd, &pt);

			int index = ItemFromPoint(pt);
			if(index != -1)
			{
				_Item& item = items[index];
				AvailableDeskletsAPI::AveInstantiateEntry(&item.entry, FALSE, TRUE);
			}
	}
	else if(WM_LBUTTONDOWN == msg)
	{
		DWORD msgPos = ::GetMessagePos();
		POINT screenPt = {GET_X_LPARAM(msgPos), GET_Y_LPARAM(msgPos)};
		POINT pt = screenPt;
		::ScreenToClient(hwnd, &pt);
		SIZE* smallSize = (leftCounter  > 0) ? &sLeftBig  : &sLeftSmall;
		SIZE* bigSize   = (rightCounter > 0) ? &sRightBig : &sRightSmall;

		if(pt.x <= smallSize->cx || pt.x >= windowWidth - bigSize->cx)
		{
			int oldScroll = scroll;
			RECT winRect = {0};
			::GetWindowRect(hwnd, &winRect);
			int windowWidth = winRect.right - winRect.left;
			int pageSpace = windowWidth - leftMargin*2;
			int itemsPerPage = pageSpace / totalItemWidth;
			int numPages = (int)items.size() / itemsPerPage + 1;
			int curPage = scroll / pageSpace;
			int prevPage = curPage - 1;
			if(prevPage < 0)prevPage = numPages-1;
			int nextPage = curPage + 1;
			if(curPage >= numPages - 1)nextPage = 0;

			int toPage = prevPage;
			if(pt.x >= windowWidth - bigSize->cx)
				toPage = nextPage;

			scrollWanted = toPage * pageSpace + leftMargin;

			if(oldScroll != scrollWanted && !scrollTimerRunning)
			{
				scrollTodo = abs(scrollWanted - scroll);
				scrollTimerRunning = TRUE;
				::SetTimer(dockHwnd, 3, GetKeyState(VK_SHIFT) < 0 ? 100 : 10, NULL);			
			}
		}
		else
		{
			int index = ItemFromPoint(pt);
			if(index != -1)
			{
				if(::DragDetect(dockHwnd, screenPt))
				{
					_Item& item = items[index];
					AvailableDeskletsAPI::AveStartDragOperation(&item.entry, item.s, dockHwnd);
				}
				else
				{
					::SendMessage(hwnd, WM_LBUTTONUP, wParam, lParam);
				}
			}
		}

	}else if(WM_ERASEBKGND == msg)
	{
		return 1;
	}
	else if(WM_PAINT == msg)
	{
		RECT winRect = {0};
		::GetWindowRect(hwnd, &winRect);
		int windowWidth = winRect.right - winRect.left;
		RECT updateRect = {0};
		::GetUpdateRect(hwnd, &updateRect, FALSE);
		if(!::IsRectEmpty(&updateRect))
		{
			int pageSpace = windowWidth - leftMargin*2;
			int itemsPerPage = pageSpace / totalItemWidth;
			int numPages = (int)items.size() / itemsPerPage + 1;
			int curPage = scroll / pageSpace;
			int prevPage = curPage - 1;
			if(prevPage < 0)prevPage = numPages-1;
			int nextPage = curPage + 1;
			if(curPage >= numPages - 1)nextPage = 0;

			PAINTSTRUCT ps = {0};
			HDC dc = ::BeginPaint(hwnd, &ps);
	
			HDC screenDC = ::GetDC(NULL);
			int count(0);
			int lastLeft = updateRect.left;
			int firstLeft = leftMargin - scroll;

			int lfHeight = 16;
			HFONT hFont = ::CreateFont(lfHeight, 0, 0, 0, FW_BOLD, FALSE, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, FF_DONTCARE, _T("Arial"));


			HDC tempDC = ::CreateCompatibleDC(screenDC);

			HDC rightDC = NULL;
			HBITMAP rightBmp = NULL;
			SIZE* rightSize = &sRightSmall;
			if(rightCounter > 0)
				rightSize = &sRightBig;

			HDC leftDC = NULL;
			HBITMAP leftBmp = NULL;
			SIZE* leftSize = &sLeftSmall;
			if(leftCounter > 0)
				leftSize = &sLeftBig;

			if(updateRect.left <= leftSize->cx)
			{
				leftDC = ::CreateCompatibleDC(screenDC);
				leftBmp = ::CreateCompatibleBitmap(screenDC, leftSize->cx, leftSize->cy);
				::SelectObject(leftDC, leftBmp);

				::SelectObject(tempDC, hLeftSmall);
				BYTE leftAlpha = (BYTE) ( (float)sin(float(leftCounter) / float(maxLeftFrames) * PI25DT / 2.0f + PI25DT / 2.0f) * 255.0f);
				//if(leftCounter == 0)
				//	leftAlpha = 255;
				BLENDFUNCTION bf = {AC_SRC_OVER, 0, leftAlpha, AC_SRC_ALPHA};
				::AlphaBlend(leftDC, 0, 0, sLeftSmall.cx, sLeftSmall.cy, tempDC, 0, 0, sLeftSmall.cx, sLeftSmall.cy, bf);

				if(leftCounter > 0)
				{
					HDC bufferDC = ::CreateCompatibleDC(screenDC);
					HBITMAP bufferBmp = ::CreateCompatibleBitmap(screenDC, sLeftBig.cx, sLeftBig.cy);
					::SelectObject(bufferDC, bufferBmp);
					::SelectObject(tempDC, hLeftBig);
					
					bf.SourceConstantAlpha = 255;
					::AlphaBlend(bufferDC, 0, 0, sLeftBig.cx, sLeftBig.cy, tempDC, 0, 0, sLeftBig.cx, sLeftBig.cy, bf);

					{
					std::wstringstream ws;
					ws << prevPage+1 << L" of " << numPages;
					Gdiplus::Graphics g(bufferDC);
					Font f(L"Arial", 10.0f, FontStyleBold);
					RectF rcText((float)sLeftSmall.cx-6,0, (float)(sLeftBig.cx - sLeftSmall.cx+6), (float)sLeftBig.cy);
					StringFormat sf;
					sf.SetLineAlignment(StringAlignmentCenter);
			
					SolidBrush brush(Color(254, 255, 255, 255));
					SolidBrush brush2(Color(254, 0x2A,0x2B,0x2F));
					g.DrawString(ws.str().c_str(), -1, &f, rcText, &sf, &brush);
					rcText.Y--;
					g.DrawString(ws.str().c_str(), -1, &f, rcText, &sf, &brush2);
					}

					bf.SourceConstantAlpha = (int) ( (float)sin(float(leftCounter) / float(maxLeftFrames) * PI25DT / 2.0f) * 255.0f);
					::AlphaBlend(leftDC, 0, 0, sLeftBig.cx, sLeftBig.cy, bufferDC, 0, 0, sLeftBig.cx, sLeftBig.cy, bf);

					::DeleteDC(bufferDC);
					::DeleteBitmap(bufferBmp);
				}
			}

			if(updateRect.right >= windowWidth - rightSize->cx)
			{
				rightDC = ::CreateCompatibleDC(screenDC);
				rightBmp = ::CreateCompatibleBitmap(screenDC, rightSize->cx, rightSize->cy);
				::SelectObject(rightDC, rightBmp);

				::SelectObject(tempDC, hRightSmall);
				BYTE rightAlpha = (BYTE) ( (float)sin(float(rightCounter) / float(maxRightFrames) * PI25DT / 2.0f + PI25DT / 2.0f) * 255.0f);
				//if(leftCounter == 0)
				//	leftAlpha = 255;
				BLENDFUNCTION bf = {AC_SRC_OVER, 0, rightAlpha, AC_SRC_ALPHA};
				::AlphaBlend(rightDC, rightSize->cx - sRightSmall.cx, 0, sRightSmall.cx, sRightSmall.cy, tempDC, 0, 0, sRightSmall.cx, sRightSmall.cy, bf);

				if(rightCounter > 0)
				{
					HDC bufferDC = ::CreateCompatibleDC(screenDC);
					HBITMAP bufferBmp = ::CreateCompatibleBitmap(screenDC, sRightBig.cx, sRightBig.cy);
					::SelectObject(bufferDC, bufferBmp);
					::SelectObject(tempDC, hRightBig);
					
					bf.SourceConstantAlpha = 255;
					::AlphaBlend(bufferDC, 0, 0, sRightBig.cx, sRightBig.cy, tempDC, 0, 0, sRightBig.cx, sRightBig.cy, bf);

					{
					std::wstringstream ws;
					ws << nextPage+1 << L" of " << numPages;
					Gdiplus::Graphics g(bufferDC);
					Font f(L"Arial", 10.0f, FontStyleBold);
					RectF rcText((float)0,0, (float)(sRightBig.cx - sRightSmall.cx)+8.0f, (float)sRightBig.cy);
					StringFormat sf;
					sf.SetAlignment(StringAlignmentFar);
					sf.SetLineAlignment(StringAlignmentCenter);
			
					SolidBrush brush(Color(254, 255, 255, 255));
					SolidBrush brush2(Color(254, 0x2A,0x2B,0x2F));
					g.DrawString(ws.str().c_str(), -1, &f, rcText, &sf, &brush);
					rcText.Y--;
					g.DrawString(ws.str().c_str(), -1, &f, rcText, &sf, &brush2);
					}

					bf.SourceConstantAlpha = (int) ( (float)sin(float(rightCounter) / float(maxRightFrames) * PI25DT / 2.0f) * 255.0f);
					::AlphaBlend(rightDC, 0, 0, sRightBig.cx, sRightBig.cy, bufferDC, 0, 0, sRightBig.cx, sRightBig.cy, bf);

					::DeleteDC(bufferDC);
					::DeleteBitmap(bufferBmp);
				}
			}

			for(std::vector<_Item>::iterator iter = items.begin(); iter != items.end(); ++iter, ++count)
			{
				int left = count * totalItemWidth + leftMargin - scroll;
				_Item& item = *iter;

				RECT intersection = {0};
				RECT itemRect = {left, itemTop, left + totalItemWidth, itemTop + 80};
				if(IntersectRect(&intersection, &updateRect, &itemRect))// && itemRect.right < windowWidth )
				{
					int w = intersection.right - intersection.left;
					int h = intersection.bottom - intersection.top;
					int l = intersection.left;
					int t = intersection.top;

					HDC bufferDC = ::CreateCompatibleDC(screenDC);
					HBITMAP hBmp = ::CreateCompatibleBitmap(screenDC, totalItemWidth, bgHeight);
					::SelectObject(bufferDC, hBmp);
					::SelectObject(bufferDC, hFont);


					int diff = l - left;
					int tempL = l;
					while(tempL < l + totalItemWidth)
					{
						int bgL = tempL % bgWidth;
						int curW = bgWidth - bgL;
						::BitBlt(bufferDC, tempL - l + diff, 0, totalItemWidth, bgHeight, ownDC, bgL, 0, SRCCOPY);
						tempL += curW;
					}

					//::SetTextAlign(bufferDC, TA_BOTTOM | TA_CENTER);
					DWORD flags = DT_CENTER	| DT_BOTTOM | DT_SINGLELINE | DT_END_ELLIPSIS;
					int len = (int)wcslen(item.entry.name);
					::SetTextColor(bufferDC, RGB(255,255,255));
					::SetBkMode(bufferDC, TRANSPARENT);
					RECT textRect = {5, itemTop + 80 + 10, totalItemWidth - 5, bgHeight+1};
					DrawTextW(bufferDC, item.entry.name, len, &textRect, flags);
					textRect.top--;
					//textRect.left++;
					//textRect.right++;
					textRect.bottom--;
					::SetTextColor(bufferDC, RGB(0x2A,0x2B,0x2F));
					DrawTextW(bufferDC, item.entry.name, len, &textRect, flags);

					int outL = totalItemWidth/2 - 80/2;
                    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
					HDC iconDC = item.dc;
					if(NULL == iconDC)
						iconDC = defaultDC;
					BOOL bRes = ::AlphaBlend(bufferDC, outL, t, 80, h, iconDC, 0, t - itemRect.top, 80, h, bf);
//					BOOL bRes = ::AlphaBlend(bufferDC, outL, t, w, h, item.dc, l  - itemRect.left, t - itemRect.top, w, h, bf);


					if(leftDC != NULL && l <= leftSize->cx)
					{
						int w = leftSize->cx- l;
						//int ll = 
						BLENDFUNCTION bfLeft = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
						BOOL bRes = ::AlphaBlend(bufferDC, diff, bgHeight/2 - leftSize->cy/2, w, leftSize->cy, leftDC, l, 0, w, leftSize->cy, bfLeft);	
					}

					if(rightDC != NULL && (l+w >= windowWidth - rightSize->cx))
					{
						int start = windowWidth - rightSize->cx;
						int l2 = max(0,start - left);
						int w2 = max(0, left - start);
						BLENDFUNCTION bfRight = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
						BOOL bRes = ::AlphaBlend(bufferDC, l2, bgHeight/2 - rightSize->cy/2, rightSize->cx - w2, rightSize->cy, rightDC, w2, 0, rightSize->cx - w2, rightSize->cy, bfRight);
						//HBRUSH brsh = ::CreateSolidBrush(RGB(255,255,255));
						//RECT rc2={l2,0,rightSize->cx + l2, 40};
						//::FillRect(bufferDC, &rc2,  brsh);
						//::DeleteObject(brsh);
					}

					BitBlt(dc, left, 0, totalItemWidth, bgHeight, bufferDC, 0, 0, SRCCOPY);
					::DeleteDC(bufferDC);
					::DeleteObject(hBmp);
					//ExcludeClipRect(dc, l, t, w, h);
					//BOOL bRes = ::AlphaBlend(dc, l, t, w, h, item.dc, l  - itemRect.left, t - itemRect.top, w, h, bf);

					lastLeft = l + totalItemWidth;
				}
			}



			RECT r = updateRect;
			r.left = lastLeft;
			int origRight = r.right;

			int start = windowWidth - rightSize->cx;
			if(rightDC != NULL && lastLeft <= windowWidth && origRight >= start)
				r.right = start;

			while(r.left < r.right)
			{
				int w = r.right - r.left;
				int h = r.bottom - r.top;
				int l = r.left % bgWidth;
				int curW = min(bgWidth - l, r.right - r.left);
				::BitBlt(dc, r.left, r.top, curW, h, ownDC, l, r.top, SRCCOPY);
				r.left += curW;
			}

			if(rightDC != NULL && lastLeft <= windowWidth && origRight >= start)
			{
				int w = origRight - start;
				HDC bufferDC = ::CreateCompatibleDC(screenDC);
				HBITMAP bufferBmp = ::CreateCompatibleBitmap(screenDC, w, bgHeight);
				::SelectObject(bufferDC, bufferBmp);

				r.right = origRight;
				while(r.left < r.right)
				{
					int w = r.right - r.left;
					int h = r.bottom - r.top;
					int l = r.left % bgWidth;
					int curW = min(bgWidth - l, r.right - r.left);
					::BitBlt(bufferDC, r.left - start, r.top, curW, h, ownDC, l, r.top, SRCCOPY);
					r.left += curW;
				}

				int l2 = 0;
				int w2 = 0;
				BLENDFUNCTION bfRight = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
				BOOL bRes = ::AlphaBlend(bufferDC, l2, bgHeight/2 - rightSize->cy/2, rightSize->cx - w2, rightSize->cy, rightDC, w2, 0, rightSize->cx - w2, rightSize->cy, bfRight);

				BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
				bRes = ::AlphaBlend(dc, start, r.top, w, r.bottom - r.top, bufferDC, 0, r.top, w, r.bottom - r.top, bf);	

				::DeleteDC(bufferDC);
				::DeleteObject(bufferBmp);
			}

			int leftStillToPaintWidth = leftMargin - scroll;
			if(leftStillToPaintWidth > 0)
			{
				HDC bufferDC = NULL;
				HBITMAP hBufferBmp = NULL;
				HDC paintDC = dc;
				if(leftDC != NULL)
				{
					bufferDC = ::CreateCompatibleDC(screenDC);
					hBufferBmp = ::CreateCompatibleBitmap(screenDC, leftStillToPaintWidth, bgHeight);
					SelectObject(bufferDC, hBufferBmp);
					paintDC = bufferDC;
				}

				::BitBlt(paintDC, 0, r.top, leftStillToPaintWidth, r.bottom - r.top, ownDC, 0, r.top, SRCCOPY);

				if(bufferDC != NULL)
				{
					if(leftDC != NULL)
					{
						BLENDFUNCTION bfLeft = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
						BOOL bRes = ::AlphaBlend(bufferDC, 0, bgHeight/2 - leftSize->cy/2, min(leftStillToPaintWidth, leftSize->cx), leftSize->cy, leftDC, 0, 0, min(leftStillToPaintWidth, leftSize->cx), leftSize->cy, bfLeft);	
					}

					::BitBlt(dc, 0, r.top, leftStillToPaintWidth, r.bottom - r.top, bufferDC, 0, r.top, SRCCOPY);
					::DeleteDC(bufferDC);
					::DeleteObject(hBufferBmp);
				}
			}

			::DeleteObject(hFont);
			::DeleteDC(tempDC);

			::ReleaseDC(NULL, screenDC);

			if(leftDC != NULL)
				::DeleteDC(leftDC);
			if(leftBmp != NULL)
				::DeleteObject(leftBmp);

			if(rightDC != NULL)
				::DeleteDC(rightDC);
			if(rightBmp != NULL)
				::DeleteObject(rightBmp);

			::EndPaint(hwnd, &ps);
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

std::wstring Desklet::GetImagePath(const std::wstring& base)
{
	assert(hwnd != NULL);
	WCHAR path[MAX_PATH] = {0};
	::DeskletGetRelativeFolderW(hwnd, path);
	::PathAppendW(path, L"images");
	::PathAppendW(path, base.c_str());

	return std::wstring(path);
}