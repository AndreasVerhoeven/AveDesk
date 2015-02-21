// Sample Desklet
// Showes a drive image depending on the status of the drive

#define _WIN32_WINNT 0x500
#include <windows.h>					// for common windows stuff
#include "../DeskletSDK.h"				// for common desklet stuff
#include <dbt.h>						// drive checking stuff

#include <Gdiplus.h>					// for all GDI+ stuff
using namespace Gdiplus;				// use the namespace Gdiplus by default
#pragma comment(lib, "gdiplus.lib")		// link with gdiplus.lib

#include "resource.h"

class DOCKLET_DATA
{
	public:
		HINSTANCE	hInstanceDll;
		HWND		hwndDesklet;

		bool mouseover;
		char time[1000];
};

DOCKLET_DATA *CALLBACK OnCreate(HWND hwndDocklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	DOCKLET_DATA* lpData = new DOCKLET_DATA;
	ZeroMemory(lpData,sizeof(DOCKLET_DATA));

	lpData->hInstanceDll = hInstance;
	lpData->hwndDesklet = hwndDocklet;

	SIZE s = {200,32};
	DeskletSetSize(lpData->hwndDesklet,&s);

	char path[MAX_PATH];
	Bitmap* bmp = new Bitmap(200,32);
	DeskletGetRelativeFolder(lpData->hwndDesklet,path);
	strcat(path,"mouse_off.png");
	DeskletSetImageFile(lpData->hwndDesklet,path);
	DeskletSetImageOverlay(lpData->hwndDesklet,bmp,TRUE);

	SetTimer(lpData->hwndDesklet,30303,1000,0);

	return lpData;
}

void CALLBACK OnDestroy(DOCKLET_DATA *lpData, HWND hwndDocklet)
{
	KillTimer(lpData->hwndDesklet,30303);
	delete lpData;
	lpData = 0;
}
void CALLBACK OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName, "Time Desklet");
	strcpy(szAuthor, "Andreas Verhoeven");
	(*iVersion) = 100;
	strcpy(szNotes,   "Shows the time on mouse over. Sample desklet for the AveDesks Desklet SDK");
}

void DrawNiceText(Graphics *graphics,int r, int g, int b, int a, char *szText, WCHAR *fontName, int style, REAL size, Rect &rect,int drawShadow, int valign, int halign)
{
	// draw some nice text according to the params

	OLECHAR text[800];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, -1, text, sizeof(text));

	FontFamily  fontFamily(fontName);
	Font        font(&fontFamily, 18, FontStyleBold, UnitPoint);
	SolidBrush  black(Color(a, 0, 0, 0));
	SolidBrush  blue(Color(a, r, g, b));
	SolidBrush  white(Color(255, 255, 255, 255));
	SolidBrush  transparent(Color(0, 255, 255, 255));
	Pen			blackOutline(Color(255, 0, 0, 0), 1);



	GraphicsPath path(FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);

	if(halign == 0)format.SetAlignment(StringAlignmentCenter);
	if(halign == 1)format.SetAlignment(StringAlignmentNear);
	if(halign == 2)format.SetAlignment(StringAlignmentFar);
	if(valign == 0)format.SetLineAlignment(StringAlignmentCenter);
	if(valign == 1)format.SetLineAlignment(StringAlignmentNear);
	if(valign == 2)format.SetLineAlignment(StringAlignmentFar);

	path.AddString(text, -1, &fontFamily, style, size, rect, &format);

	SIZE sizeMovement = {2, 2};
	GraphicsPath pathShaddow(FillModeAlternate);

	if(drawShadow)
	{
		Rect shaddowRect(rect.GetLeft()+sizeMovement.cx, rect.GetTop()+sizeMovement.cy, (rect.GetRight()-rect.GetLeft()), (rect.GetBottom()-rect.GetTop()));
		pathShaddow.AddString(text, -1, &fontFamily, style, size, shaddowRect, &format);
		graphics->FillPath(&black, &pathShaddow);
	}


	graphics->FillPath(&blue, &path);
	graphics->DrawPath(&blackOutline, &path);
}

void CALLBACK OnProcessMessage(DOCKLET_DATA *lpData, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char path[MAX_PATH];

	switch(uMsg)
	{
	case WM_MOUSELEAVE:
		if(lpData->mouseover)
		{
			for(int i=32;i>=4;i--)
			{
				SIZE s = {200,i};
				DeskletSetSize(lpData->hwndDesklet,&s);
			}

			Bitmap* bmp = new Bitmap(200,32);
			DeskletGetRelativeFolder(lpData->hwndDesklet,path);
			strcat(path,"mouse_off.png");
			DeskletSetImageFile(lpData->hwndDesklet,path);
			DeskletSetImageOverlay(lpData->hwndDesklet,bmp,TRUE);

			SIZE s = {200,32};
			DeskletSetSize(lpData->hwndDesklet,&s);

			lpData->mouseover = false;

		}
		break;
	case WM_MOUSEMOVE:
		if(!lpData->mouseover)
		{
			SIZE s = {200,4};
			DeskletSetSize(lpData->hwndDesklet,&s);

			DeskletGetRelativeFolder(lpData->hwndDesklet,path);
			strcat(path,"mouse_on.png");
			DeskletSetImageFile(lpData->hwndDesklet,path);
			
			Bitmap* bmp = new Bitmap(200,32);
			Graphics g(bmp);
			g.SetSmoothingMode(SmoothingModeAntiAlias);
			GetDateFormat(LOCALE_USER_DEFAULT,0,0,"ddd',' MMM dd yy",lpData->time,sizeof(lpData->time)/sizeof(lpData->time[0]));
			DrawNiceText(&g,0,0,0,255,lpData->time,L"Arial",0,20,Rect(4,0,200,32),0,1,0);

			for(int i=4;i<=32;i++)
			{
				SIZE s = {200,i};
				DeskletSetSize(lpData->hwndDesklet,&s);
			}

			DeskletSetImageOverlay(lpData->hwndDesklet,bmp,TRUE);

			lpData->mouseover = true;

		}
		break;

	case WM_TIMER:
		if(wParam == 30303)
		{
			//if(lpData->mouseover)
			{
				char lbl[1000];
				GetTimeFormat(LOCALE_USER_DEFAULT,0,0,"HH:mm:ss",lbl,1000);
				DeskletSetLabel(lpData->hwndDesklet,lbl);
			}
		}
	break;
	}
	return;
}
