#include <windows.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include "../deskletsdk.h"
#include "../drawpluginsdk.h"
#include "resource.h"
#include <commctrl.h>

#define GLOW_TIMER 32032302

struct PLUGIN_DATA
{
	public:
		HWND		hwndDesklet;
		HINSTANCE	hInstanceDll;
		bool		isGrayScaling;
		HWND		timerWindow;
		int			counter;

		int			timerMs;
		int			numFrames;
		int			maxGlowLevel;

		bool		inPreview;

		POINT		oldPoint;
		SIZE		oldSize;

		bool		isZoomingIn;
		
};


void __stdcall OnDrawPluginInfo(char* name, char* author, char* notes, int* version)
{
	// pre:  
	// post: 

	strcpy(name,"Smooth Magnification");
	strcpy(author,"Alilm");
	strcpy(notes,"If you like this, please donate some cash:)");
	*version = 110;
}

LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hwnd,"lpData");
	if(msg == WM_CREATE)
	{
		SetProp(hwnd,"lpData",(HANDLE)((LPCREATESTRUCT) lParam)->lpCreateParams);
		lpData = (PLUGIN_DATA*) GetProp(hwnd,"lpData");
		if(!lpData)return 0;
	}
	else if(msg == WM_TIMER && wParam == GLOW_TIMER && lpData)
	{
		POINT p = {0};
		SIZE  s = {0};

		DeskletGetPosition(lpData->hwndDesklet,&p);
		DeskletGetSize(lpData->hwndDesklet,&s);

		if(lpData->isZoomingIn)
		{
			lpData->counter++;
			{
				p.x -= 4;
				p.y -=4;

				s.cx += 8;
				s.cy += 8;

				RECT r = {0};
				r.left = p.x;
				r.top = p.y;
				r.right = p.x + s.cx;
				r.bottom = p.y + s.cy;
				DeskletMove(lpData->hwndDesklet,&r,TRUE);

				//DeskletSetSizeAndPosition(lpData->hwndDesklet,&p,&s,true);
			}
			if(lpData->counter >= 10)
				KillTimer(lpData->timerWindow,GLOW_TIMER);
		}
		else
		{
			lpData->counter--;
			p.x += 4;
			p.y += 4;

			s.cx -= 8;
			s.cy -= 8;

			RECT r = {0};
			r.left = p.x;
			r.top = p.y;
			r.right = p.x + s.cx;
			r.bottom = p.y + s.cy;
			DeskletMove(lpData->hwndDesklet,&r,TRUE);
			//DeskletSetSizeAndPosition(lpData->hwndDesklet,&p,&s,true);

			if(lpData->counter <= 0)
				KillTimer(lpData->timerWindow,GLOW_TIMER);
		}
		DeskletRedraw(lpData->hwndDesklet);
		return 0;
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
}

PLUGIN_DATA* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, char* iniFile, char* iniGroup)
{
	// pre:  
	// post: 

	PLUGIN_DATA* lpData = new PLUGIN_DATA;
	lpData->hwndDesklet = hwndDesklet;
	lpData->hInstanceDll = hInstanceDll;
	lpData->counter = 0;
	lpData->timerMs = 20;
	lpData->numFrames = 20;
	lpData->maxGlowLevel = 33;
	lpData->inPreview = false;
	lpData->isZoomingIn = false;

	if(iniFile && iniGroup)
	{
		lpData->maxGlowLevel	= GetPrivateProfileInt(iniGroup,"_EMGL",33,iniFile);
		lpData->numFrames		= GetPrivateProfileInt(iniGroup,"_ENF",20,iniFile);
		lpData->timerMs			= GetPrivateProfileInt(iniGroup,"_ETMS",20,iniFile);
	}

	WNDCLASS c;
	ZeroMemory(&c,sizeof(c));
	c.hInstance = hInstanceDll;
	c.lpfnWndProc = WinProc;
	c.lpszClassName = "AveGrayScalerTimerWindowClass";
	RegisterClass(&c);

	lpData->timerWindow = CreateWindow(c.lpszClassName,"HiddenAveGrayScalerTimerWindow",0,0,0,1,1,0,0,lpData->hInstanceDll,(LPVOID)lpData);

	return lpData;
}

void __stdcall OnSave(PLUGIN_DATA* lpData,char* iniFile, char* iniGroup)
{
	WritePrivateProfileInt(iniGroup,"_EMGL",lpData->maxGlowLevel,iniFile);
	WritePrivateProfileInt(iniGroup,"_ENF",lpData->numFrames,iniFile);
	WritePrivateProfileInt(iniGroup,"_ETMS",lpData->timerMs,iniFile);
}

void __stdcall OnDestroy(PLUGIN_DATA* lpData)
{
	// pre:  
	// post:
	DestroyWindow(lpData->timerWindow);
	delete lpData;
}

BOOL CALLBACK ConfigDlgProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");

	static int mgl, fd, nof;

	switch(msg)
	{
		case WM_INITDIALOG:
			SetProp(hDlg,"lpData",(HANDLE)lParam);
			lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");
			if(!lpData)return TRUE;

			mgl = lpData->maxGlowLevel;
			fd  = lpData->timerMs;
			nof = lpData->numFrames;
			SendDlgItemMessage(hDlg,IDC_FRAMEDURATION,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)10);
			SendDlgItemMessage(hDlg,IDC_FRAMEDURATION,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)200);
			SendDlgItemMessage(hDlg,IDC_FRAMEDURATION,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)200-lpData->timerMs);

			SendDlgItemMessage(hDlg,IDC_NUMBEROFFRAMES,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)1);
			SendDlgItemMessage(hDlg,IDC_NUMBEROFFRAMES,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)200);
			SendDlgItemMessage(hDlg,IDC_NUMBEROFFRAMES,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->numFrames);

			SendDlgItemMessage(hDlg,IDC_MAXGLOWLEVEL,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)1);
			SendDlgItemMessage(hDlg,IDC_MAXGLOWLEVEL,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)100);
			SendDlgItemMessage(hDlg,IDC_MAXGLOWLEVEL,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->maxGlowLevel);

		break;

		case WM_HSCROLL:
					if(IsDlgButtonChecked(hDlg,IDC_PREVIEW))
					{
						lpData->timerMs			= 200-SendDlgItemMessage(hDlg,IDC_FRAMEDURATION,TBM_GETPOS,0,0);
						lpData->numFrames		= SendDlgItemMessage(hDlg,IDC_NUMBEROFFRAMES,TBM_GETPOS,0,0);
						lpData->maxGlowLevel	= SendDlgItemMessage(hDlg,IDC_MAXGLOWLEVEL,TBM_GETPOS,0,0);

						if(lpData->counter != 0)
							KillTimer(lpData->timerWindow,GLOW_TIMER);

						lpData->inPreview = true;
						lpData->isGrayScaling = false;
						SetTimer(lpData->timerWindow,GLOW_TIMER,lpData->timerMs,0);	
					}
					else
					{
						lpData->inPreview = false;
						KillTimer(lpData->timerWindow,GLOW_TIMER);
						lpData->counter = 0;
						DeskletRedraw(lpData->hwndDesklet);
					}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_PREVIEW: case IDC_FRAMEDURATION: case IDC_NUMBEROFFRAMES: case IDC_MAXGLOWLEVEL:
					ConfigDlgProc(hDlg,WM_HSCROLL,0,0);
				break;

				case IDDEFAULT:
					SendDlgItemMessage(hDlg,IDC_FRAMEDURATION,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)200-20);
					SendDlgItemMessage(hDlg,IDC_NUMBEROFFRAMES,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)20);
					SendDlgItemMessage(hDlg,IDC_MAXGLOWLEVEL,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)33);
					ConfigDlgProc(hDlg,WM_HSCROLL,0,0);
				break;

				case IDRESET:
					SendDlgItemMessage(hDlg,IDC_FRAMEDURATION,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)200-fd);
					SendDlgItemMessage(hDlg,IDC_NUMBEROFFRAMES,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)nof);
					SendDlgItemMessage(hDlg,IDC_MAXGLOWLEVEL,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)mgl);
					ConfigDlgProc(hDlg,WM_HSCROLL,0,0);
				break;

				case IDOK:
					lpData->timerMs			= 200-SendDlgItemMessage(hDlg,IDC_FRAMEDURATION,TBM_GETPOS,0,0);
					lpData->numFrames		= SendDlgItemMessage(hDlg,IDC_NUMBEROFFRAMES,TBM_GETPOS,0,0);
					lpData->maxGlowLevel	= SendDlgItemMessage(hDlg,IDC_MAXGLOWLEVEL,TBM_GETPOS,0,0);

					EndDialog(hDlg,TRUE);
				break;
			}
		break;

		case WM_CLOSE:
			EndDialog(hDlg,0);
		break;

		case WM_DESTROY:
			lpData->inPreview = false;
			KillTimer(lpData->timerWindow,GLOW_TIMER);
			lpData->counter = 0;
			DeskletRedraw(lpData->hwndDesklet);
		break;

	}
	return FALSE;
}


void __stdcall OnConfigure(PLUGIN_DATA* lpData, HWND owner)
{
	// pre:  
	// post: 
	DialogBoxParam(lpData->hInstanceDll, MAKEINTRESOURCE(IDD_CONFIG), owner, ConfigDlgProc, (LPARAM)lpData);
}

void __stdcall OnEvent(PLUGIN_DATA* lpData, UINT event)
{
	// pre:  
	// post: 
	if(event == EVENT_ONMOUSEON)
	{
		POINT p = {0};
		SIZE  s = {0};

		DeskletGetPosition(lpData->hwndDesklet,&p);
		DeskletGetSize(lpData->hwndDesklet,&s);

		lpData->oldPoint = p;

		lpData->oldSize = s;

		lpData->isZoomingIn = true;
		SetTimer(lpData->timerWindow,GLOW_TIMER,lpData->timerMs,0);
	}
	else if(event == EVENT_ONMOUSEOUT)
	{
		if(lpData->isZoomingIn == true)
		{
			lpData->isZoomingIn = false;
			SetTimer(lpData->timerWindow,GLOW_TIMER,lpData->timerMs,0);
		}
	}
	else if(event == EVENT_ONCONFIGURE || event == EVENT_ONSTARTMOVE || event == EVENT_ONRIGHTCLICK )
	{
		if(lpData->isZoomingIn == true)
		{
			lpData->counter = 0;
			lpData->isZoomingIn = false;
			KillTimer(lpData->timerWindow,GLOW_TIMER);
			SleepEx(5,TRUE);
			RECT r = {0};
			r.left	= lpData->oldPoint.x;
			r.top	= lpData->oldPoint.y;
			r.right = r.left + lpData->oldSize.cx;
			r.bottom= r.top + lpData->oldSize.cy;
			DeskletMove(lpData->hwndDesklet,&r,TRUE);
		}
	}
}
