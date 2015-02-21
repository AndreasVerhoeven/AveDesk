#include <windows.h>					// trivial

#include <gdiplus.h>					// for GDI+ stuff
using namespace Gdiplus;				// idem
#pragma comment(lib, "gdiplus.lib")		// idem
	
#include "../deskletsdk.h"				// for the DeskletSDK
#include "../drawpluginsdk.h"			// for the DrawPluginSDK
#include "resource.h"					// trivial
#include <commctrl.h>					// trivial

#define GLOW_TIMER 32032302				// a random timer ID for our timer


// This struct will hold the data for an instance of this drawplugin
struct PLUGIN_DATA
{
	public:
		HWND		hwndDesklet;		// The desklet that this effect is attached to
		HINSTANCE	hInstanceDll;		// The handle to this DLL file
		bool		isBackwardGlowing;	// The direction of glow => forward we are going from non-glow to glow
										//	backwards, we are going from glow to non-glow

		HWND		timerWindow;		// The windowshandle that will receive our timer message
		int			counter;			// A counter

		int			timerMs;			// The frame-duration in ms
		int			numFrames;			// The number of frames for each glow
		int			maxGlowLevel;		// The maximum level of glowiness

		bool		inPreview;			// A flag stating if we are in preview mode or not
		
};

//	This function is called whenever AveDesk wants to retrieve information about this DrawPlugin.
//	It is not associated with a particular instance of a plugin.
//
//	NOTE: The order of parameters is different than for the OnGetInformation()
void __stdcall OnDrawPluginInfo(char* name, char* author, char* notes, int* version)
{
	strcpy(name,"Glow");
	strcpy(author,"Andreas Verhoeven");
	strcpy(notes,"Sample Drawplugin for AveDesk1.1");
	*version = 100;
}


//	A trivial WINPROC that will handle our timer messages
LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_CREATE)
	{
		// oncreation we attach a property to this window so we can get the PLUGIN_DATA object
		// back. We pass this as a pointer through the lParam.
		SetProp(hwnd,"lpData",(HANDLE)((LPCREATESTRUCT) lParam)->lpCreateParams);
		PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hwnd,"lpData");
		if(!lpData)return 0;
	}
	else if(msg == WM_TIMER && wParam == GLOW_TIMER)
	{
		// A timer event occured, so we draw a new frame

		// First, check if we can access our PLUGIN_DATA object
		PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hwnd,"lpData");
		if(!lpData)return 0;

		if(lpData->isBackwardGlowing)
		{
			// we are glowing backwards, so decrement the counter
			lpData->counter--;

			// if we reached 0, we reverse the glowing process
			if(lpData->counter <= 0)
				lpData->isBackwardGlowing = false;
		}
		else
		{
			// we are glowing forward, so increment the counter
			lpData->counter++;

			// if we reached the maximum number of frames the user choosed, reverse the glowing process
			if(lpData->counter >= lpData->numFrames)
				lpData->isBackwardGlowing = true;
		}

		// let AveDesk redraw the desklet, so we can commit a glow in the BeforeDraw() event
		DeskletRedraw(lpData->hwndDesklet);

		return 0;
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
}

//	This function is called when an new instance of this DrawPlugin is created. iniFile and iniGroup are 
//	non-zero if the DrawPlugin had saved settings before.
//	hwndDesklet is the handle of the desklet this instance of the DrawPlugin will be attached to.
PLUGIN_DATA* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, char* iniFile, char* iniGroup)
{
	// create an new PLUGIN_DATA object for this instance.

	// set some default parameters
	PLUGIN_DATA* lpData = new PLUGIN_DATA;
	lpData->hwndDesklet = hwndDesklet;
	lpData->hInstanceDll = hInstanceDll;
	lpData->counter = 0;
	lpData->timerMs = 20;
	lpData->numFrames = 20;
	lpData->maxGlowLevel = 33;
	lpData->inPreview = false;

	if(iniFile && iniGroup)
	{
		// we can load previously stored settings
		// to avoid name-collising, we start each settings name with "_E"
		lpData->maxGlowLevel	= GetPrivateProfileInt(iniGroup,"_EMGL",33,iniFile);
		lpData->numFrames		= GetPrivateProfileInt(iniGroup,"_ENF",20,iniFile);
		lpData->timerMs			= GetPrivateProfileInt(iniGroup,"_ETMS",20,iniFile);
	}

	// now we set up a hidden window that will receive our timer messages
	WNDCLASS c;
	ZeroMemory(&c,sizeof(c));
	c.hInstance = hInstanceDll;
	c.lpfnWndProc = WinProc;
	c.lpszClassName = "AveGrayScalerTimerWindowClass";
	RegisterClass(&c);

	lpData->timerWindow = CreateWindow(c.lpszClassName,"HiddenAveGrayScalerTimerWindow",0,0,0,1,1,0,0,lpData->hInstanceDll,(LPVOID)lpData);

	// finally, return the object to AveDesk, so it will be included in each other event for this instance of this DrawPlugin.
	return lpData;
}

//	This function is called whenever a desklet is saved that has this DrawPlugin attached to it.
//	You can data in an INI-file by using the iniFile and iniGroup parameters combined with the WritePrivateProfile*() - 
//	WIN32 APIs.
//	To avoid name-collising, we let every key start with "_E".
void __stdcall OnSave(PLUGIN_DATA* lpData,char* iniFile, char* iniGroup)
{
	WritePrivateProfileInt(iniGroup,"_EMGL",lpData->maxGlowLevel,iniFile);
	WritePrivateProfileInt(iniGroup,"_ENF",lpData->numFrames,iniFile);
	WritePrivateProfileInt(iniGroup,"_ETMS",lpData->timerMs,iniFile);
}

//	This function is called when a DrawPlugin instance is destroyed. You should delete all allocated memory,
//	detroy all windows, exit all created threads etc... here.
void __stdcall OnDestroy(PLUGIN_DATA* lpData)
{
	// we destroy our hidden window created in the OnCreate
	DestroyWindow(lpData->timerWindow);
	delete lpData;
}

//	A trivial DLGPROC for our configuration dialog.
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
						lpData->isBackwardGlowing = false;
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

//	This function is called when the user hits the "Configure" button for a particular instance of this
//	DrawPlugin. You can show your own dialog for modifying settings or simply don't implement/export this
//	routine to let AveDesk show the user that there is no configuration for this DrawPlugin.
void __stdcall OnConfigure(PLUGIN_DATA* lpData, HWND owner)
{
	// Show our settings dialog.
	DialogBoxParam(lpData->hInstanceDll, MAKEINTRESOURCE(IDD_CONFIG), owner, ConfigDlgProc, (LPARAM)lpData);
}

//	This function is called when an event occurs in the desklet that this DrawPlugin instance is attached too
//	(for a list of events, see the DeskletSDK). This DrawPlugin instance can than take whatever action to modify,
//	the painting process for the desklet.
void __stdcall OnEvent(PLUGIN_DATA* lpData, UINT event)
{
	// Here, we want to show a glow-effect when the mouse is on the desklet and simply leave the desklet
	// in its original state if the mouse is out of the window. Therefore, we respond to the EVENT_ONMOUSEON and
	// EVENT_ONMOUSEOUT events.

	if(event == EVENT_ONMOUSEON)
	{
		// The mouse entered the desklet, so we start a timer that will draw frames of the glowing-process.
		lpData->isBackwardGlowing = false;
		SetTimer(lpData->timerWindow,GLOW_TIMER,lpData->timerMs,0);
	}
	else if(event == EVENT_ONMOUSEOUT)
	{
		// The mouse moved out of the desklet, so we immediately stop our glowing by stopping the timer that
		// would draw frames of the glowing-process.
		// Also, we reset the counter so we start from the glowing from the beginning the next time the mouse enters the
		// the desklet.
		// Finally, we redraw the desklet to make sure the glow is completely gone.
		KillTimer(lpData->timerWindow,GLOW_TIMER);
		lpData->counter = 0;
		DeskletRedraw(lpData->hwndDesklet);
	}
}

//	This function is the actuall 'workhorse'. It is called everytime when the desklet is redrawn, and to be exact,
//	right before all other layers are drawn for a desklet. You can drawn something behind the desklets layers,
//	modify the ColorMatrix and the matrix. If you modify something, please set modified to true.
//	left, top, width and height are passed as an INT rather than as a Gdiplus::Rect for two reasons:
//		- better performance due to the way AveDesk draws the layers
//		- they are not editable this way.
//	left, top, width and height determines the borders the layers are drawn. AveDesk can use a larger buffer than these
//	parameters say.
//	HWND hwnd is reserved for future usage.
//	isSelected and isMouseOn gives the current status of the desklets selection and mouse-on status.
void __stdcall BeforeDraw(PLUGIN_DATA* lpData, HWND hwnd,
	Gdiplus::Graphics* g,UINT left, UINT top,UINT width, UINT height, Gdiplus::ColorMatrix* m, Gdiplus::Matrix* matrix,BOOL* modified,
		BOOL isSelected, BOOL isMouseOn)
{
	// here, we only want to modify the colormatrix on mouseover OR when we are in preview mode.
	if(isMouseOn || lpData->inPreview)
	{
		// by modifying the 4th row of the colorMatrix, we can set a glow level based on the users settings.
		m->m[4][0] = m->m[4][1] = m->m[4][2] = (REAL)lpData->counter / (lpData->numFrames*(100.0f / (float)lpData->maxGlowLevel));
		
		// do NOT forget to set modified to true, otherwise our changes might not be used.
		*modified = true;
	}
}