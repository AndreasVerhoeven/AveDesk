#include "../DeskletSDK.h"

//#include <map>
#include <string>
//#include <iterator>
//#include <sstream>
#include <vector>
#include <math.h>


SIZE size = {360, 240};
SIZE logoSize = {46, 40};
RECT logoRect = {155, 73, 155 + 46, 73 + 40};
RECT logoRect2 = {154, 164, 154 + 46, 164 + 40};
RECT nextRect = {300, 200, 340, 220};
RECT flipRect = {150, 150, 210, 210};
POINT closePoint = {8,8};

template<typename T> T RemapPoint(const T& t, const SIZE& s, float xScale=100, float yScale=100)
{
	T nT;
	nT.x = nT.y = 0;
	
	if(s.cx == 0 || s.cy == 0)
		return nT;

	if(xScale == 0.0f || yScale == 0.0f)
		return nT;

	float xRatio = float(s.cx) / xScale;
	float yRatio = float(s.cy) / yScale;

	nT.x = static_cast<float>(t.x) * xRatio;
	nT.y = static_cast<float>(t.y) * yRatio;

	//nT.x = static_cast<long>(float(t.x) / float(s.cx) * xScale);
	//nT.y = static_cast<long>(float(t.y) / float(s.cy) * yScale);

	return nT;
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"AveDesk 1.3 Intro");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"An introduction to AveDesk 1.3!");
	*iVersion = 130;
}

ColorMatrix GetIdentityMatrix()
{
	// post: an indetity color-matrix has been returned

	ColorMatrix retMatrix;
	::ZeroMemory(retMatrix.m, 25 * sizeof(REAL));
	for (int i = 0; i < 5; i++)
		retMatrix.m[i][i] = 1.0f;	

	return retMatrix;
}

class Page;

class Intro
{
public:
	HWND hwnd;
	HINSTANCE dll;
	UINT cur;
	UINT flippedTo;
	std::vector<Page*> pages;
	HCURSOR oldCursor;
	BOOL cursorChanged;

	Intro(HWND hwnd, HINSTANCE dll);

	~Intro();

	static void __stdcall OnFlipDone(void* ptr, const FlipParams* params);
	void Flip(UINT numTo);
	void FlipBack(UINT numTo);
};

class Page
{
public:
	HWND hwnd;
	Intro* intro;
	Page(HWND hwnd, Intro* intro):hwnd(hwnd), intro(intro){}
	virtual ~Page(){};

	virtual std::string GetImageFile(std::string filename)
	{
		char path[MAX_PATH] = {0};
		DeskletGetRelativeFolder(hwnd, path);
		strcat(path, "images//");
		return path + filename;
	}


	virtual BOOL Message(UINT msg, WPARAM w, LPARAM l)
	{return FALSE;};
	virtual BOOL __stdcall LayerClick(UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
	{return FALSE;}
	virtual void OnShow(){}
	virtual void BeforeHide(){}

	virtual std::vector<UINT> GetLayers() = 0;
};

const page0StartTimerTimer = 2302;
const page0StartTimerInterval = 2000;
const page0Timer = 2303;
const page0TimerInterval = 10;
class Page0 : public Page
{
public:
	UINT page0;
	UINT logo;

	int step;

	Page0(HWND hwnd, Intro* intro):Page(hwnd, intro), step(20)
	{
		page0 = DeskletLayerAdd(hwnd, "page0");
		DeskletLayerSetVisibility(hwnd,page0, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, page0, FALSE, GetImageFile("page0.png").c_str());

		logo = DeskletLayerAdd(hwnd, "page0_logo");
		DeskletLayerSetAlpha(hwnd, logo, FALSE, 0);
		DeskletLayerSetVisibility(hwnd, logo, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, logo, FALSE, GetImageFile("logo.png").c_str());
		DeskletLayerSetPlacement(hwnd, logo, FALSE, TRUE, &logoRect, size.cx, size.cy);
	}

	virtual BOOL Message(UINT msg, WPARAM w, LPARAM l)
	{
		if(WM_TIMER == msg && page0StartTimerTimer == w)
		{
			KillTimer(hwnd, page0StartTimerTimer);
			SetTimer(hwnd, page0Timer, page0TimerInterval, NULL);
		}
		if(WM_TIMER == msg && page0Timer == w)
		{
			int div = 9;
			UINT w = step / div;
			if(div % 9 == 0)
			{
				UINT h = step / div;

				RECT r = logoRect;
				r.left   -= w;
				r.top    -= h;
				r.right  += w;
				r.bottom += h;

				DeskletLayerSetPlacement(hwnd, logo, FALSE, TRUE, &r, size.cx, size.cy);
				BOOL bAlpha = 255 - step;
				if(bAlpha > 200)
					bAlpha = 200;
				DeskletLayerSetAlpha(hwnd, logo, TRUE, bAlpha);

				int speed = 2;

				step += speed;
			}

			if(step > 255)
			{
				step = 0;
				KillTimer(hwnd, page0Timer);
				SetTimer(hwnd, page0StartTimerTimer, page0StartTimerInterval, NULL);
			}

			return TRUE;
		}

		return FALSE;
	}

	virtual void OnShow()
	{ 
		SetTimer(hwnd, page0Timer, page0TimerInterval, NULL);
	}

	virtual void BeforeHide()
	{
		KillTimer(hwnd, page0StartTimerTimer);
		KillTimer(hwnd, page0Timer);
	}

	virtual std::vector<UINT> GetLayers()
	{
		std::vector<UINT> v;
		v.push_back(page0);
		v.push_back(logo);

		return v;
	}
};

class Page1 : public Page
{
public:
	UINT page1;

	Page1(HWND hwnd, Intro* intro):Page(hwnd, intro)
	{
		page1 = DeskletLayerAdd(hwnd, "page1");
		DeskletLayerSetVisibility(hwnd,page1, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, page1, FALSE, GetImageFile("page1.png").c_str());
	}

	virtual std::vector<UINT> GetLayers()
	{
		std::vector<UINT> v;
		v.push_back(page1);

		return v;
	}

	virtual BOOL __stdcall LayerClick(UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
	{
		POINT p = *pt;

		if(PtInRect(&flipRect, p))
		{
			intro->FlipBack(intro->cur--);
			return TRUE;
		}
		return FALSE;
	}

};

class Page2 : public Page
{
public:
	UINT page2;

	Page2(HWND hwnd, Intro* intro):Page(hwnd, intro)
	{
		page2 = DeskletLayerAdd(hwnd, "page2");
		DeskletLayerSetVisibility(hwnd,page2, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, page2, FALSE, GetImageFile("page2.png").c_str());
	}

	virtual std::vector<UINT> GetLayers()
	{
		std::vector<UINT> v;
		v.push_back(page2);

		return v;
	}

	virtual void OnShow()
	{
		//DeskletManager(DM_CONTROLPANEL);
	}
};


const page3Timer = 729;
const page3Timer2 = 730;
const page3TimerInterval = 1000;
const page3TimerInterval2 = 5000;
const page3Timer3 = 731;
const page3TimerInterval3 = 8000;

class Page3: public Page
{
public:
	UINT page3;

	Page3(HWND hwnd, Intro* intro):Page(hwnd, intro)
	{
		page3 = DeskletLayerAdd(hwnd, "page3");
		DeskletLayerSetVisibility(hwnd,page3, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, page3, FALSE, GetImageFile("page3.png").c_str());
	}

	virtual std::vector<UINT> GetLayers()
	{
		std::vector<UINT> v;
		v.push_back(page3);

		return v;
	}

	virtual BOOL Message(UINT msg, WPARAM w, LPARAM l)
	{
		if(WM_TIMER == msg && page3Timer == w)
		{
			KillTimer(hwnd, page3Timer);

			WCHAR path[MAX_PATH] = {0};
			GetDLLPathW(intro->dll, path, MAX_PATH);
			wcscat(path, L"data\\translator.avedesklet");
			DeskletLoadFromConfigW(path, NULL, TRUE);
		}
		else if(WM_TIMER == msg && page3Timer2 == w)
		{
			KillTimer(hwnd, page3Timer2);

			WCHAR path[MAX_PATH] = {0};
			GetDLLPathW(intro->dll, path, MAX_PATH);
			wcscat(path, L"data\\flippy.avedesklet");
			DeskletLoadFromConfigW(path, NULL, TRUE);
		}
		else if(WM_TIMER == msg && page3Timer3 == w)
		{
			KillTimer(hwnd, page3Timer3);

			WCHAR path[MAX_PATH] = {0};
			GetDLLPathW(intro->dll, path, MAX_PATH);
			wcscat(path, L"data\\mypc.avedesklet");
			DeskletLoadFromConfigW(path, NULL, TRUE);

			GetDLLPathW(intro->dll, path, MAX_PATH);
			wcscat(path, L"data\\trash.avedesklet");
			DeskletLoadFromConfigW(path, NULL, TRUE);
		}

		return FALSE;
	}

	virtual void OnShow()
	{ 
		SetTimer(hwnd, page3Timer, page3TimerInterval, NULL);
		SetTimer(hwnd, page3Timer2, page3TimerInterval2, NULL);
		SetTimer(hwnd, page3Timer3, page3TimerInterval3, NULL);
	}
};

#define WM_USER_UPDATEPATH WM_USER + 1209

class Page4 : public Page
{
public:
	UINT page4;

	Page4(HWND hwnd, Intro* intro):Page(hwnd, intro)
	{
		page4 = DeskletLayerAdd(hwnd, "page4");
		DeskletLayerSetVisibility(hwnd,page4, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, page4, FALSE, GetImageFile("page4.png").c_str());
	}

	virtual std::vector<UINT> GetLayers()
	{
		std::vector<UINT> v;
		v.push_back(page4);

		return v;
	}

	virtual void OnShow()
	{
		WCHAR tut[MAX_PATH] = {0};
		GetDLLPathW(NULL, tut, MAX_PATH);
		wcscat(tut, L"tutorial\\index.html");

		WCHAR path[MAX_PATH] = {0};
		GetDLLPathW(intro->dll, path, MAX_PATH);
		wcscat(path, L"data\\tutorial.avedesklet");
		HWND desklet = DeskletLoadFromConfigW(path, NULL, TRUE);
		SendMessage(desklet, WM_USER_UPDATEPATH, (WPARAM)FALSE, (LPARAM)tut);
	}
};

const page5StartTimerTimer = 2353;
const page5StartTimerInterval = 2000;
const page5Timer = 2334;
const page5TimerInterval = 10;
class Page5 : public Page
{
public:
	UINT page5;
	UINT logo;
	int step;

	Page5(HWND hwnd, Intro* intro):Page(hwnd, intro), step(0)
	{
		page5 = DeskletLayerAdd(hwnd, "page5");
		DeskletLayerSetVisibility(hwnd,page5, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, page5, FALSE, GetImageFile("page5.png").c_str());

		logo = DeskletLayerAdd(hwnd, "page5_logo");
		DeskletLayerSetAlpha(hwnd, logo, FALSE, 0);
		DeskletLayerSetVisibility(hwnd, logo, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, logo, FALSE, GetImageFile("logo.png").c_str());
		DeskletLayerSetPlacement(hwnd, logo, FALSE, TRUE, &logoRect2, size.cx, size.cy);
	}

	virtual std::vector<UINT> GetLayers()
	{
		std::vector<UINT> v;
		v.push_back(page5);
		v.push_back(logo);
		return v;
	}

	virtual BOOL Message(UINT msg, WPARAM w, LPARAM l)
	{
		if(WM_TIMER == msg && page5StartTimerTimer == w)
		{
			KillTimer(hwnd, page5StartTimerTimer);
			SetTimer(hwnd, page5Timer, page5TimerInterval, NULL);
		}
		if(WM_TIMER == msg && page5Timer == w)
		{
			int div = 9;
			UINT w = step / div;
			UINT h = step / div;

			RECT r = logoRect2;
			r.left   -= w;
			r.top    -= h;
			r.right  += w;
			r.bottom += h;

			DeskletLayerSetPlacement(hwnd, logo, FALSE, TRUE, &r, size.cx, size.cy);
			BOOL bAlpha = 255 - step;
			if(bAlpha > 200)
				bAlpha = 200;
			DeskletLayerSetAlpha(hwnd, logo, TRUE, bAlpha);

			int speed = 2;

			step += speed;

			if(step > 255)
			{
				step = 0;
				KillTimer(hwnd, page5Timer);
				SetTimer(hwnd, page5StartTimerTimer, page5StartTimerInterval, NULL);
			}

			return TRUE;
		}

		return FALSE;
	}

	virtual void OnShow()
	{ 
		SetTimer(hwnd, page5Timer, page5TimerInterval, NULL);
	}

	virtual void BeforeHide()
	{
		KillTimer(hwnd, page5StartTimerTimer);
		KillTimer(hwnd, page5Timer);
	}
};

class Page6 : public Page
{
public:
	UINT page6;

	Page6(HWND hwnd, Intro* intro):Page(hwnd, intro)
	{
		page6 = DeskletLayerAdd(hwnd, "page6");
		DeskletLayerSetVisibility(hwnd,page6, FALSE, FALSE);
		DeskletLayerSetImageFile(hwnd, page6, FALSE, GetImageFile("page6.png").c_str());
	}

	virtual std::vector<UINT> GetLayers()
	{
		std::vector<UINT> v;
		v.push_back(page6);
		return v;
	}
};


Intro::Intro(HWND hwnd, HINSTANCE dll) : hwnd(hwnd), dll(dll), cur(0), flippedTo(0), cursorChanged(FALSE), oldCursor(NULL)
{
	RECT dtopRect = {0};
	GetWindowRect(GetDesktopWindow(), &dtopRect);
	POINT p = {0};
	p.x = dtopRect.right / 2 - size.cx / 2;
	p.y = dtopRect.bottom / 2 - size.cy / 2;
	DeskletSetPosition(hwnd, &p);
	DeskletSetSize(hwnd, &size);

	DeskletSetLabel(hwnd, "");
	DeskletSetSubLabel(hwnd, "");
	DeskletSetClosePoint(hwnd, &closePoint, size.cx, size.cy);

	while(DeskletLayerCount(hwnd) > 0)
		DeskletLayerRemove(hwnd, 0, FALSE);

	pages.push_back(new Page0(hwnd, this));
	pages.push_back(new Page1(hwnd, this));
	pages.push_back(new Page2(hwnd, this));
	pages.push_back(new Page3(hwnd, this));
	pages.push_back(new Page4(hwnd, this));
	pages.push_back(new Page5(hwnd, this));
	pages.push_back(new Page6(hwnd, this));

	std::vector<UINT> layers = pages[0]->GetLayers();
	for(UINT i = 0; i < layers.size(); ++i)
	{
		DeskletLayerSetVisibility(hwnd, layers[i], i == layers.size() - 1, TRUE);
	}

	pages[0]->OnShow();
}

Intro::~Intro()
{
	std::vector<Page*> oldPages = pages;
	pages.clear();
	for(std::vector<Page*>::iterator iter = oldPages.begin(); iter != oldPages.end(); ++iter)
	{
		delete *iter;
	}
}

void __stdcall Intro::OnFlipDone(void* ptr, const FlipParams* params)
{
	Intro* intro = (Intro*)ptr;
	intro->pages[intro->flippedTo]->OnShow();
}

void Intro::Flip(UINT numTo)
{
	if(numTo < pages.size())
	{
		flippedTo = numTo;

		std::vector<UINT> oldLayers = pages[numTo-1]->GetLayers();
		std::vector<UINT> newLayers = pages[numTo]->GetLayers();

		pages[numTo-1]->BeforeHide();

		FlipParams params = {0};
		params.flipToBack = TRUE;
		params.useLayers = TRUE;

		params.oldLayersVisible = &(*oldLayers.begin());
		params.oldLayersVisibleCount = oldLayers.size();

		params.newLayersVisible = &(*newLayers.begin());
		params.newLayersVisibleCount = newLayers.size();
		params.callback = OnFlipDone;
		params.callbackData = (void*)this;

		DeskletFlip(hwnd, &params);
	}
}

void Intro::FlipBack(UINT numTo)
{
	if(numTo < pages.size())
	{
		flippedTo = numTo-1;

		std::vector<UINT> newLayers = pages[numTo-1]->GetLayers();
		std::vector<UINT> oldLayers = pages[numTo]->GetLayers();

		pages[numTo]->BeforeHide();

		FlipParams params = {0};
		params.flipToBack = FALSE;
		params.useLayers = TRUE;

		params.oldLayersVisible = &(*oldLayers.begin());
		params.oldLayersVisibleCount = oldLayers.size();

		params.newLayersVisible = &(*newLayers.begin());
		params.newLayersVisibleCount = newLayers.size();
		params.callback = OnFlipDone;
		params.callbackData = (void*)this;

		DeskletFlip(hwnd, &params);
	}
}

Intro* __stdcall OnCreate(HWND hwnd, HINSTANCE dll, const char* group, const char* ini)
{
	Intro* intro = new Intro(hwnd, dll);

	return intro;
}

void __stdcall OnDestroy(Intro* intro, HWND hwndDesklet)
{
	delete intro;
}

void __stdcall OnProcessMessage(Intro* intro, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(NULL == intro)
		return;

	for(std::vector<Page*>::iterator iter = intro->pages.begin(); iter != intro->pages.end(); ++iter)
	{
		if((*iter)->Message(msg, wParam, lParam))
			return;
	}
}

BOOL __stdcall OnMouseWheelForward(Intro* intro, WORD fKeysDown, int numOfScrolls, int x, int y)
{
	POINT pos = {0};
	SIZE s = {0};
	DeskletGetPosition(intro->hwnd, &pos);
	DeskletGetSize(intro->hwnd, &s);
	RECT r = {0};
	r.left = pos.x - 1;
	r.top = pos.y - 1;
	r.right = r.left + s.cx + 2;
	r.bottom = r.top + s.cy + 2;

	DeskletMove(intro->hwnd, &r, TRUE);
	return TRUE;
}

BOOL __stdcall OnMouseWheelBackward(Intro* intro, WORD fKeysDown, int numOfScrolls, int x, int y)
{
	POINT pos = {0};
	SIZE s = {0};
	DeskletGetPosition(intro->hwnd, &pos);
	DeskletGetSize(intro->hwnd, &s);
	if(s.cx <= size.cx)
		return FALSE;

	RECT r = {0};
	r.left = pos.x + 1;
	r.top = pos.y + 1;
	r.right = r.left + s.cx - 2;
	r.bottom = r.top + s.cy - 2;

	DeskletMove(intro->hwnd, &r, TRUE);

	return TRUE;
}

BOOL __stdcall OnLayerMouseOut(Intro* intro, UINT layerId, const char* layerName)
{
	if(intro->cursorChanged)
	{
		intro->cursorChanged = FALSE;
		DeskletSetCursor(intro->hwnd, intro->oldCursor);
	}
	return FALSE;
}

BOOL __stdcall OnLayerMouseMove(Intro* intro, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	POINT rp = RemapPoint(*pt, *s, size.cx, size.cy);
	if(PtInRect(&nextRect, rp) && intro->cur < intro->pages.size())
	{
		if(!intro->cursorChanged)
		{
			intro->cursorChanged = TRUE;
			HCURSOR hand = LoadCursor(NULL, IDC_HAND);
			intro->oldCursor = DeskletSetCursor(intro->hwnd, hand);
		}
	}
	else
	{
		if(intro->cursorChanged)
		{
			intro->cursorChanged = FALSE;
			DeskletSetCursor(intro->hwnd, intro->oldCursor);
		}
	}

	return FALSE;
}


BOOL __stdcall OnLayerClick(Intro* intro, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	POINT rp = RemapPoint(*pt, *s, size.cx, size.cy);

	if(intro->cur < intro->pages.size())
	{
		if(intro->pages[intro->cur]->LayerClick(layerId, layerName, &rp, s, keys))
			return TRUE;
	}

	if(PtInRect(&nextRect, rp) && intro->cur < intro->pages.size())
		intro->Flip(++intro->cur);

	return FALSE;
}