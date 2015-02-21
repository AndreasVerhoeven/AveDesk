#define WINVER		0x0501
#define _WIN32_WINNT	0x0500
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0100
#include <windows.h>

#include "avehookapi.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
using namespace Gdiplus;

#include <map>
#include <vector>

#define COMPILE_MULTIMON_STUBS 
#include <multimon.h> 

struct SideBarredDesklet
{
	std::vector<HWND> hwnds;
};

struct Module
{
public:
	UINT styleID;
	UINT posHookID;
	HWND sidebarHwnd;

	BOOL onTop;

	ATOM atom;

	std::map<HWND, SideBarredDesklet> desklets;
};


 void __stdcall OnGetInformation(char* name, char* author, int* version, char* description)
{
	strcpy_s(name, 100, "Sidebar");
	strcpy_s(author, 100, "Ave");
	*version = 100;
	strcpy_s(description, 100, "Emulates the Vista Sidebar");
}

UINT CALLBACK onStyleChange(void* ptr, const DESKLET_STYLE_CHANGE_DATA* dscd)
{
	Module* m = (Module*)ptr;

	if(m->styleID == dscd->newStyle)
	{
		SetPropW(dscd->deskletHwnd, L"is_sidebarred", (HANDLE)1);
		SideBarredDesklet d;
		d.hwnds.push_back(dscd->deskletHwnd);
		d.hwnds.push_back(dscd->labelHwnd);
		for(UINT i = 0; i < dscd->numExtraHwnds; ++i)
			d.hwnds.push_back(dscd->extraHwnds[i]);

		for(std::vector<HWND>::iterator iter = d.hwnds.begin(); iter != d.hwnds.end(); ++iter)
		{
			if(GetWindowLong(*iter, 0xFFFFFFF8) != (LONG)m->sidebarHwnd)
			{
				SetPropW(*iter, L"prevowner", (HANDLE)(HWND)GetWindowLong(*iter, 0xFFFFFFF8));
				SetWindowLong(*iter, 0xFFFFFFF8, (LONG)m->sidebarHwnd);
			}
		}

		m->desklets[dscd->deskletHwnd] = d;
	}
	else if(m->styleID == dscd->oldStyle || dscd->newStyle == (UINT)-1)
	{
		SetPropW(dscd->deskletHwnd, L"is_sidebarred", (HANDLE)0);
		SideBarredDesklet& d = m->desklets[dscd->deskletHwnd];
		for(std::vector<HWND>::iterator iter = d.hwnds.begin(); iter != d.hwnds.end(); ++iter)
		{
			SetWindowLong(*iter, 0xFFFFFFF8, (LONG)GetPropW(*iter,L"prevowner"));
		}

		m->desklets.erase(dscd->deskletHwnd);
	}

	return 0;
}

UINT CALLBACK onGetInsertHwnd(void* ptr, DESKLET_STYLE_CHANGE_DATA* dscd)
{
	Module* m = (Module*)ptr;

	//dscd->deskletHwnd = m->sidebarHwnd;
	//dscd->labelHwnd = m->sidebarHwnd;
	SetWindowPos(m->sidebarHwnd, dscd->deskletHwnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	dscd->deskletHwnd = HWND_BOTTOM;

	return 1;
}

UINT_PTR CALLBACK onMessage(void* ptr, const DESKLET_STYLE_CHANGE_DATA* dscd, MSG* msg)
{
	Module* m = (Module*)ptr;

	if(msg->message == WM_WINDOWPOSCHANGING)
	{
		WINDOWPOS* w = (WINDOWPOS*)msg->lParam;
		if((w->flags & SWP_NOZORDER) != 0)
			SetWindowPos(m->sidebarHwnd, w->hwnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	}

	return 0;
}


BOOL CALLBACK onPositionChange(void* ptr, HWND hwndDesklet, RECT* r)
{
	Module* m = (Module*)ptr;

	if(GetPropW(hwndDesklet, L"is_sidebarred") == (HANDLE)1)
	{
		RECT sr = {0};
		GetWindowRect(m->sidebarHwnd, &sr);

		int offset = 5;
		int width = r->right - r->left;
		r->left = sr.right - width - offset;
		r->right = r->left + width;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

HWND CreateSideBar(Module* m);
void RenderSideBar(Module* m, BYTE alpha);

Module* __stdcall OnCreate()
{
	Module* m = new Module;
	m->onTop = FALSE;

	POSITION_HOOK_DATA phd = {0};
	phd.dwSize = sizeof(phd);
	phd.onPositionChangeCallback = &onPositionChange;
	phd.callbackPointer = (void*)m;
	m->posHookID = AveRegisterPositionHook(&phd);

	DESKLET_STYLE_DATA data = {0};
	data.dwSize = sizeof(data);
	data.callbackPointer = (void*)m;
	data.friendlyName = L"Sidebar";
	data.uniqueName = L"ave_sidebar";
	data.flags = 0L;
	data.styleChangeCallback = &onStyleChange;
	data.insertHwndCallback = &onGetInsertHwnd;
	data.messageCallback = &onMessage;

	m->styleID = AveRegisterDeskletStyle(&data);

	m->sidebarHwnd = CreateSideBar(m);
	m->atom = GlobalAddAtom(L"avedesk_sidebar_to_top");
	RegisterHotKey(m->sidebarHwnd, m->atom, 0x8, 'S');
	RenderSideBar(m,0);
	SetWindowPos(m->sidebarHwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	for(int i=10; i < 255; i+=10)
	{
		BLENDFUNCTION blend;
        blend.BlendOp = AC_SRC_OVER;
        blend.BlendFlags = 0;
        blend.AlphaFormat = 0;
        blend.SourceConstantAlpha = (BYTE)i;

	    ::UpdateLayeredWindow(m->sidebarHwnd, NULL, NULL, NULL, NULL, NULL,
                    NULL, &blend, ULW_ALPHA);
	}
	RenderSideBar(m,255);

	return m;
}

void __stdcall OnDestroy(Module* m)
{
	UnregisterHotKey(m->sidebarHwnd, m->atom);
	GlobalDeleteAtom(m->atom);

	AveUnregisterPositionHook(m->posHookID);

	std::map<HWND, SideBarredDesklet> ds = m->desklets;
	for(std::map<HWND, SideBarredDesklet>::iterator iter = ds.begin(); iter != ds.end(); ++iter)
	{
		for(std::vector<HWND>::iterator iter2 = iter->second.hwnds.begin(); iter2 != iter->second.hwnds.end(); ++iter2)
		{
			SetWindowLong(*iter2, 0xFFFFFFF8, (LONG)GetPropW(*iter2,L"prevowner"));
		}
	}
	m->desklets.clear();

	for(int i=250; i >= 0; i-=10)
	{
		BLENDFUNCTION blend;
        blend.BlendOp = AC_SRC_OVER;
        blend.BlendFlags = 0;
        blend.AlphaFormat = 0;
        blend.SourceConstantAlpha = (BYTE)i;

	    ::UpdateLayeredWindow(m->sidebarHwnd, NULL, NULL, NULL, NULL, NULL,
                    NULL, &blend, ULW_ALPHA);
	}
	::DestroyWindow(m->sidebarHwnd);

	AveUnregisterDeskletStyle(m->styleID);

	delete m;
};

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch(msg)
	{
		case WM_HOTKEY:
			{
				Module* m = NULL;
				m = (Module*)GetProp(hwnd, L"m_ptr");
				if(m != NULL)
				{
					m->onTop =! m->onTop;
					SetWindowPos(m->sidebarHwnd, m->onTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW	| SWP_NOZORDER);
				}
			}
		break;

	}

	return ::DefWindowProc(hwnd, msg, w, l);
}

HWND CreateSideBar(Module* m)
{
	WNDCLASSW c = {0};
	c.hInstance = HINST_THISCOMPONENT;
	c.lpfnWndProc = WinProc;
	c.lpszClassName = L"AveDeskSideBarAddOnClass";
	c.hCursor = LoadCursor(NULL, IDC_ARROW	);
	RegisterClass(&c);

	HWND hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		c.lpszClassName,L"AveDeskSideBar",WS_POPUP,0,0,1,1,0,0,HINST_THISCOMPONENT,(LPVOID)m);

	SetProp(hwnd, L"m_ptr", (HANDLE)m);

	return hwnd;
}

void GetDLLPathW(HMODULE h,WCHAR* path, DWORD nSize)
{
	GetModuleFileNameW(h,path,MAX_PATH);

	for(size_t i=wcslen(path)-1;i>0;--i)
	{
		if(path[i]=='\\' || path[i]=='/')
		{
			path[i+1] = '\0';
			break;
		}
	}
}

void RenderSideBar(Module* m, BYTE alpha)
{
	WCHAR fname[MAX_PATH] = {0};
	GetDLLPathW(HINST_THISCOMPONENT, fname, MAX_PATH);
	PathAppendW(fname, L"bg.png");
	Bitmap* img = new Bitmap(fname);

	HBITMAP hBmp = {0};

	RECT r = {0};

	r.left = GetSystemMetrics( SM_XVIRTUALSCREEN );
	r.top = GetSystemMetrics( SM_YVIRTUALSCREEN ); 
	r.right = r.left + GetSystemMetrics( SM_CXVIRTUALSCREEN );
	r.bottom = r.top + GetSystemMetrics( SM_CYVIRTUALSCREEN ); 

	SIZE size = {img->GetWidth(), r.bottom - r.top};

	Bitmap* bmp = new Bitmap(size.cx, size.cy);
	Graphics g(bmp);

	int imgHeight = img->GetHeight();
	int cycles = 0;
	if(img->GetHeight() > 0)
	{
		cycles = (int)(size.cy / imgHeight);
		if(size.cy % imgHeight != 0)
			cycles += 1;
	}

	for(int i = 0; i < cycles; ++i)
	{
		Rect rc(0, (imgHeight * i), img->GetWidth(), imgHeight);
		g.DrawImage(img, rc, 0, 0, img->GetWidth(), imgHeight, UnitPixel, 0, 0, 0);
	}

	bmp->GetHBITMAP(NULL,&hBmp);

	POINT pos = {r.right - size.cx, 0};

	HDC screenDC = ::GetDC(NULL);

	HDC dc = ::CreateCompatibleDC(screenDC);
	HBITMAP old = (HBITMAP)::SelectObject(dc, hBmp);

	POINT ptSourcePosition = {0,0};
	BLENDFUNCTION blendFunction = {0};

	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = alpha;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;

	COLORREF crKey = RGB(255,0,255); // dummy colourkey
	DWORD dwFlags = 2;

	::UpdateLayeredWindow(m->sidebarHwnd,screenDC,&pos,&size,dc,&ptSourcePosition, crKey,&blendFunction,dwFlags);

	::SelectObject(dc, old);
	ReleaseDC(NULL, screenDC);

	::DeleteObject(hBmp);

	delete bmp;
	delete img;
}