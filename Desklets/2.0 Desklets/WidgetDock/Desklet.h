#pragma once

#define _WIN32_WINNT 0x501
#define WINVER 0x0501
#pragma warning(disable:4503)
#pragma warning(disable:4786)
#include <windows.h>
#include <string>
#include <tchar.h>
#include <vector>
#include "AvailableDesklets.h"

class Desklet
{
	class _Item
	{
	public:
		HDC dc;
		HBITMAP hBmp;
		SIZE s;
		BOOL dockVisible;
		AvailableDeskletsAPI::AveADEntry entry;

		_Item() : dc(NULL), hBmp(NULL){}

		void AssignBitmap(HBITMAP bm, SIZE size)
		{
			s = size;
			hBmp = bm;
			HDC screenDC = ::GetDC(NULL);
			dc = ::CreateCompatibleDC(screenDC);
			SelectObject(dc, hBmp);

			::ReleaseDC(NULL, screenDC);
		}

		~_Item()
		{
		}

		void Destroy()
		{
			::DeleteDC(dc);
			// don't own it anymore
			//::DeleteObject(hBmp);
		}
	};
public:
	HWND hwnd;
	HWND dockHwnd;
	HINSTANCE hInstance;
	HBITMAP hBmp;
	HBITMAP hBmpDefault;
	HDC ownDC;
	HDC defaultDC;
	int bgWidth;
	int bgHeight;
	int itemTop;
	int windowWidth;
	int leftMargin;
	int totalItemWidth;
	int counter;
	int maxFrames;
	BOOL dockVisible;
	BOOL wasAttached;
	int prevMaxFrames;
	int scroll;
	int scrollWanted;
	BOOL scrollTimerRunning;
	int scrollCounter;
	int scrollTodo;
	int maxLeftFrames;
	int maxRightFrames;

	BOOL mouseIsOn;

	HCURSOR hCursorHand, hCursorNormal;

	int leftCounter, rightCounter;
	SIZE sLeftSmall, sLeftBig, sRightSmall, sRightBig;
	HBITMAP hLeftSmall, hLeftBig, hRightSmall, hRightBig;

	void LoadHBitmap(const std::wstring& fname, HBITMAP& hBitmap, SIZE* s);
	void LoadHBitmap(const std::wstring& fname, HBITMAP& hBitmap, int* w, int* h);

	AvailableDeskletsAPI::AveAD* aveAD;

	Desklet(HWND hwnd, HINSTANCE hInstance);
	~Desklet(void);

	std::vector<_Item> items;

	HWND SetupDockWindow();
	std::wstring GetImagePath(const std::wstring& base);

	void FreeItems();
	void FetchItems();
	void ToggleDock();

	int ItemFromPoint(POINT pt);

	static BOOL CALLBACK OnNewDeskletEntry(AvailableDeskletsAPI::AveAD* ad, const AvailableDeskletsAPI::AveADEntry* entry);
	static BOOL CALLBACK OnPreviewDone(AvailableDeskletsAPI::AveAD* ad, const AvailableDeskletsAPI::AveADEntry* entry, HBITMAP hBmp);

	static LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT OnMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
