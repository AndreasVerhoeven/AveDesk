//Weather Docklet for ObjectDock

#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include "../DeskletSDK.h"
#include "resource.h"
#include <mmsystem.h>

#include <Gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")




  ///////////////////////////////////////////////////
  //Docklet 'Data' struct. On the OnCreate I create 
  // on of these structures to store all the plugin's
  // 'personal' data in. If mutliple of my own plugins
  // added to the dock, they will each therefore automatically
  // have their own personal data stores and thus will able
  // to easily keep track of all their individual options.
typedef struct
{
	HWND hwndDocklet;
	HINSTANCE hInstanceDll;

	char lzTime[200];
	char lzTimeFormat[200];
	char lzDateFormat[200];

	char lzTimeFormat2[200];
	char lzDateFormat2[200];

	int vAlignment;
	int hAlignment;

	char fontName[MAX_PATH];

	int textsize;
	int shadow;

	int red;
	int green;
	int blue;
	int alpha;

	int vAlignment2;
	int hAlignment2;

	char fontName2[MAX_PATH];

	int textsize2;
	int shadow2;

	int red2;
	int green2;
	int blue2;
	int alpha2;

	HANDLE hUpdateThread;

	WORD hour;
	WORD minute;
	WORD second;

	bool alarm;
	bool do_alarm;

	bool do_attention;
	bool do_blink;

	char sound[MAX_PATH];

	int blinknum;

} DOCKLET_DATA;
  //
  ///////////////////////////////////


#define UPDATE_TIME_TIMER	123211

//int CALLBACK ConfigureDocklet(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void UpdateTime(DOCKLET_DATA *lpData);
DWORD WINAPI UpdateTimeThread(LPVOID lpInput);
void ParseWeekNumber(const char* formatstring,char* string, int buffersize);
  //
  ///////////////////////////////////////


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}



//Return basic information about the plugin
void CALLBACK OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName, "Digital Alarm Clock");
	strcpy(szAuthor, "Andreas Verhoeven");
	(*iVersion) = 110;
	strcpy(szNotes,   "A digital clock that can also be used to set an alarm.");
}

//In the OnCreate, we will be passed the handle to the window which owns this docklet. In ObjectDock, each icon/image
// is its own window, thus the window passed is the actual window that the image is being displayed on. Thus, it is recommended
// that you not move or otherwise manually change the appearance of this window.
// However! You will need to record this window handle, as it is through that that you will identify yourself to ObjectDock in callbacks.
//
// An hInstance is passed giving this DLL's instance, as well as a location of an ini and the specific ini group to load from.
// If szIni and szIniGroup are NULL, this plugin is being created from scratch, and default options should be set/loaded. 
//   If they are not null, the plugin should load its settings from the ini given by szIni, from the ini group given by szIniGroup. See examples.
DOCKLET_DATA *CALLBACK OnCreate(HWND hwndDocklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	//Create a plugin personal-data structure and return it. This data will be maintained through the life
	// of the plugin, and will be passed with every function call.
	DOCKLET_DATA *lpData = new DOCKLET_DATA;
	ZeroMemory(lpData, sizeof(DOCKLET_DATA));


	//Remember to store the docklet's window handle in its personal data so we can interact with the host!
	lpData->hwndDocklet = hwndDocklet;
	lpData->hInstanceDll = hInstance;

	char image[MAX_PATH];
	strcpy(image,"");

	lpData->textsize = 32;
	lpData->red = 255;
	lpData->green = 255;
	lpData->blue = 255;
	lpData->alpha = 255;

	lpData->textsize2 = 32;
	lpData->red2 = 255;
	lpData->green2 = 0;
	lpData->blue2 = 0;
	lpData->alpha2 = 255;


	strcpy(lpData->fontName,"Arial");

	if(szIni && szIniGroup)
	{
		//If an Ini file and group were passed, load the plugin's data using them
		GetPrivateProfileString(szIniGroup, "TimeFormat", "HH:mm", lpData->lzTimeFormat, 200, szIni);
		GetPrivateProfileString(szIniGroup, "DateFormat", "MMM dd yyy", lpData->lzDateFormat, 200, szIni);
		GetPrivateProfileString(szIniGroup, "TimeFormat2", "HH:mm", lpData->lzTimeFormat2, 200, szIni);
		GetPrivateProfileString(szIniGroup, "DateFormat2", "MMM dd yyy", lpData->lzDateFormat2, 200, szIni);
		GetPrivateProfileString(szIniGroup,"Image",image,image,sizeof(image)/sizeof(image[0]),szIni);
		GetPrivateProfileString(szIniGroup,"Font","Arial",lpData->fontName,sizeof(lpData->fontName)/sizeof(lpData->fontName[0]),szIni);

		lpData->textsize = GetPrivateProfileInt(szIniGroup,"Textsize",32,szIni);
		lpData->red = GetPrivateProfileInt(szIniGroup,"Red",lpData->red,szIni);
		lpData->green = GetPrivateProfileInt(szIniGroup,"Green",lpData->green,szIni);
		lpData->blue = GetPrivateProfileInt(szIniGroup,"Blue",lpData->blue,szIni);
		lpData->alpha = GetPrivateProfileInt(szIniGroup,"Alpha",lpData->alpha,szIni);

		lpData->shadow = GetPrivateProfileInt(szIniGroup,"Shadow",0,szIni);
		lpData->vAlignment = GetPrivateProfileInt(szIniGroup,"VAlign",0,szIni);
		lpData->hAlignment = GetPrivateProfileInt(szIniGroup,"HAlign",0,szIni);

		GetPrivateProfileString(szIniGroup,"Font2","Arial",lpData->fontName2,sizeof(lpData->fontName2)/sizeof(lpData->fontName2[0]),szIni);

		lpData->textsize2 = GetPrivateProfileInt(szIniGroup,"Textsize2",32,szIni);
		lpData->red2 = GetPrivateProfileInt(szIniGroup,"Re2d",lpData->red2,szIni);
		lpData->green2 = GetPrivateProfileInt(szIniGroup,"Green2",lpData->green2,szIni);
		lpData->blue2 = GetPrivateProfileInt(szIniGroup,"Blue2",lpData->blue2,szIni);
		lpData->alpha2 = GetPrivateProfileInt(szIniGroup,"Alpha2",lpData->alpha2,szIni);

		lpData->shadow2 = GetPrivateProfileInt(szIniGroup,"Shadow2",0,szIni);
		lpData->vAlignment2 = GetPrivateProfileInt(szIniGroup,"VAlign2",0,szIni);
		lpData->hAlignment2 = GetPrivateProfileInt(szIniGroup,"HAlign2",0,szIni);

		lpData->hour = GetPrivateProfileInt(szIniGroup,"Hour",0,szIni);
		lpData->minute = GetPrivateProfileInt(szIniGroup,"Minute",0,szIni);
		lpData->second = GetPrivateProfileInt(szIniGroup,"Second",0,szIni);

		lpData->do_alarm = GetPrivateProfileInt(szIniGroup,"DoAlarm",0,szIni) == 1;
		lpData->do_attention = GetPrivateProfileInt(szIniGroup,"DoAttention",0,szIni) == 1;
		lpData->do_blink = GetPrivateProfileInt(szIniGroup,"DoBlink",0,szIni) == 1;

		GetPrivateProfileString(szIniGroup,"Sound","Sound",lpData->sound,sizeof(lpData->sound)/sizeof(lpData->sound[0]),szIni);

		UpdateTime(lpData);
	}
	else
	{
		strcpy(lpData->lzTimeFormat,"HH:mm");
		strcpy(lpData->lzDateFormat,"MMM dd yyy");
	}

	//DockletSetImageFile(lpData->hwndDocklet,image);

	DWORD dwNewThreadId = 0;
	lpData->hUpdateThread = CreateThread(NULL, 0, UpdateTimeThread, (VOID *) lpData, 0, &dwNewThreadId);

	// SetTimer(lpData->hwndDocklet, UPDATE_TIME_TIMER, 1000 , NULL);

	return lpData;
}




//When ObjectDock needs to save this plugin's individual properties, it will give you the plugin's data
// along with an szIni (file) and szIniGroup (ini section) to save the plugin's data to.
// REMEMBER! Multiple of your same plugin can be added to one dock at once, so if you want each of your
// plugins to be able to have unique options, follow the example given below.
//
// "bIsForExport" lets you know if this save is being done to be saved in a package to distribute.
//  While not important for most docklets, if for instance you have a mail checking plugin
//   you will not want to save any passwords/usernames in the event that this flag is set to TRUE.
void CALLBACK OnSave(DOCKLET_DATA *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	WritePrivateProfileString(szIniGroup, "TimeFormat", lpData->lzTimeFormat, szIni);
	WritePrivateProfileString(szIniGroup, "DateFormat", lpData->lzDateFormat, szIni);
	WritePrivateProfileString(szIniGroup, "TimeFormat2", lpData->lzTimeFormat2, szIni);
	WritePrivateProfileString(szIniGroup, "DateFormat2", lpData->lzDateFormat2, szIni);
	WritePrivateProfileString(szIniGroup,"Font", lpData->fontName, szIni);
	WritePrivateProfileInt(szIniGroup,"Textsize",lpData->textsize,szIni);
	WritePrivateProfileInt(szIniGroup,"Red",lpData->red,szIni);
	WritePrivateProfileInt(szIniGroup,"Green",lpData->green,szIni);
	WritePrivateProfileInt(szIniGroup,"Blue",lpData->blue,szIni);
	WritePrivateProfileInt(szIniGroup,"Alpha",lpData->alpha,szIni);
	WritePrivateProfileInt(szIniGroup,"Shadow",lpData->shadow,szIni);
	WritePrivateProfileInt(szIniGroup,"VAlign",lpData->vAlignment,szIni);
	WritePrivateProfileInt(szIniGroup,"HAlign",lpData->hAlignment,szIni);
	WritePrivateProfileString(szIniGroup,"Font2", lpData->fontName2, szIni);
	WritePrivateProfileInt(szIniGroup,"Textsize2",lpData->textsize2,szIni);
	WritePrivateProfileInt(szIniGroup,"Red2",lpData->red2,szIni);
	WritePrivateProfileInt(szIniGroup,"Green2",lpData->green2,szIni);
	WritePrivateProfileInt(szIniGroup,"Blue2",lpData->blue2,szIni);
	WritePrivateProfileInt(szIniGroup,"Alpha2",lpData->alpha2,szIni);
	WritePrivateProfileInt(szIniGroup,"Shadow2",lpData->shadow2,szIni);
	WritePrivateProfileInt(szIniGroup,"VAlign2",lpData->vAlignment2,szIni);
	WritePrivateProfileInt(szIniGroup,"HAlign2",lpData->hAlignment2,szIni);
	WritePrivateProfileInt(szIniGroup,"Hour",lpData->hour,szIni);
	WritePrivateProfileInt(szIniGroup,"Minute",lpData->minute,szIni);
	WritePrivateProfileInt(szIniGroup,"Second",lpData->second,szIni);

	WritePrivateProfileInt(szIniGroup,"DoAlarm",lpData->do_alarm,szIni);
	WritePrivateProfileInt(szIniGroup,"DoAttention",lpData->do_attention,szIni);
	WritePrivateProfileInt(szIniGroup,"DoBlink",lpData->do_blink,szIni);
	
	WritePrivateProfileString(szIniGroup, "Sound", lpData->sound, szIni);

}



//On destruction, top all timers, kill all threads left running and
// free the memory allocated for the plugin's personal data.
void CALLBACK OnDestroy(DOCKLET_DATA *lpData, HWND hwndDocklet)
{
	if(lpData->hUpdateThread)
		TerminateThread(lpData->hUpdateThread, 0);
	CloseHandle(lpData->hUpdateThread);
	delete lpData;
}




//Each icon in the dock is its own window. The OnProcessMessage function recieves every message that
// gets sent to that window (the HWND passed on OnCreate), including any timer messages, which we will
// make use of here
void CALLBACK OnProcessMessage(DOCKLET_DATA *lpData, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_TIMER:
		if(wParam == UPDATE_TIME_TIMER)
			UpdateTime(lpData);
		break;
	}
	return;
}


//Little helper function I wrote to easily draw nice-looking text with a shadow.
void DrawNiceText(Graphics *graphics,int r, int g, int b, int a, char *szText, WCHAR *fontName, int style, REAL size, Rect &rect,int drawShadow, int valign, int halign)
{
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


//Config dialog's proc
int CALLBACK ConfigureDocklet(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	DOCKLET_DATA *lpData = (DOCKLET_DATA *) GetProp(hDlg, "lpData");
	Graphics* gr;

	char tmp[100];
	char lzBuf[200];
	char lzBuf2[200];
	char msg[20024];
	static int r,g,b,a,r2,g2,b2;
	static char tempfont[MAX_PATH],tempfont2[MAX_PATH];
	SYSTEMTIME time;

	switch(iMsg)
	{
	case WM_PAINT:
		GetWindowText(GetDlgItem(hDlg,IDC_ALPHA),tmp,sizeof(tmp)/sizeof(tmp[0]));
		a = atoi(tmp);

		gr = new Graphics(GetDlgItem(hDlg,IDC_IMG));
		gr->SetInterpolationMode(InterpolationModeHighQuality);
		gr->SetSmoothingMode(SmoothingModeAntiAlias);
		gr->Clear(Color(255,255,255,255));
	
		GetTimeFormat(LOCALE_USER_DEFAULT,0,NULL,lpData->lzTimeFormat,lzBuf,sizeof(lzBuf));
		strcat(lzBuf," ");
		ParseWeekNumber(lpData->lzDateFormat,lzBuf2,sizeof(lzBuf2));
		GetDateFormat(LOCALE_USER_DEFAULT,0,NULL,lzBuf2,lzBuf2,sizeof(lzBuf2));
		strcat(lzBuf,lzBuf2);
	
		OLECHAR font[800];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,tempfont, -1, font, sizeof(font));

		DrawNiceText(gr,r,g,b,a,lzBuf, font, FontStyleBold, 20, Rect(0, 0,100,100),IsDlgButtonChecked(hDlg,IDC_SHADOW),SendDlgItemMessage(hDlg,IDC_VALIGN,CB_GETCURSEL,0,0),SendDlgItemMessage(hDlg,IDC_HALIGN,CB_GETCURSEL,0,0));

		delete gr;

		GetWindowText(GetDlgItem(hDlg,IDC_ALPHA2),tmp,sizeof(tmp)/sizeof(tmp[0]));
		a = atoi(tmp);

		gr = new Graphics(GetDlgItem(hDlg,IDC_IMG2));
		gr->SetInterpolationMode(InterpolationModeHighQuality);
		gr->SetSmoothingMode(SmoothingModeAntiAlias);
		gr->Clear(Color(255,255,255,255));
	
		GetTimeFormat(LOCALE_USER_DEFAULT,0,NULL,lpData->lzTimeFormat,lzBuf,sizeof(lzBuf));
		strcat(lzBuf," ");
		ParseWeekNumber(lpData->lzDateFormat,lzBuf2,sizeof(lzBuf2));
		GetDateFormat(LOCALE_USER_DEFAULT,0,NULL,lzBuf2,lzBuf2,sizeof(lzBuf2));
		strcat(lzBuf,lzBuf2);
	
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,tempfont2, -1, font, sizeof(font));

		DrawNiceText(gr,r2,g2,b2,a,lzBuf, font, FontStyleBold, 20, Rect(0, 0,100,100),IsDlgButtonChecked(hDlg,IDC_SHADOW2),SendDlgItemMessage(hDlg,IDC_VALIGN2,CB_GETCURSEL,0,0),SendDlgItemMessage(hDlg,IDC_HALIGN2,CB_GETCURSEL,0,0));

		delete gr;

		break;

	case WM_INITDIALOG:
		SetProp(hDlg, "lpData", (HANDLE) (char*) lParam);
		lpData = (DOCKLET_DATA *) GetProp(hDlg, "lpData");
		if(!lpData)
			return TRUE;

		InitCommonControls();

		SetDlgItemText(hDlg,IDC_TIME,lpData->lzTimeFormat);
		SetDlgItemText(hDlg,IDC_DATE,lpData->lzDateFormat);
		SetDlgItemText(hDlg,IDC_TIME2,lpData->lzTimeFormat2);
		SetDlgItemText(hDlg,IDC_DATE2,lpData->lzDateFormat2);
		SetDlgItemInt(hDlg,IDC_TEXTSIZE,lpData->textsize,FALSE);
		CheckDlgButton(hDlg,IDC_SHADOW,lpData->shadow);
		CheckDlgButton(hDlg,IDC_ATTENTION,lpData->do_attention);
		CheckDlgButton(hDlg,IDC_BLINK,lpData->do_blink);

		r = lpData->red;
		g = lpData->green;
		b = lpData->blue;

		SetDlgItemInt(hDlg,IDC_ALPHA,lpData->alpha,FALSE);

		SetDlgItemInt(hDlg,IDC_TEXTSIZE2,lpData->textsize2,FALSE);
		CheckDlgButton(hDlg,IDC_SHADOW2,lpData->shadow2);

		r2 = lpData->red2;
		g2 = lpData->green2;
		b2 = lpData->blue2;

		SetDlgItemInt(hDlg,IDC_ALPHA2,lpData->alpha2,FALSE);

		SendDlgItemMessage(hDlg,IDC_VALIGN,CB_ADDSTRING,0,(LPARAM)"Center");
		SendDlgItemMessage(hDlg,IDC_VALIGN,CB_ADDSTRING,0,(LPARAM)"Top");
		SendDlgItemMessage(hDlg,IDC_VALIGN,CB_ADDSTRING,0,(LPARAM)"Bottom");
		SendDlgItemMessage(hDlg,IDC_VALIGN,CB_SETCURSEL,lpData->vAlignment,0);

		SendDlgItemMessage(hDlg,IDC_HALIGN,CB_ADDSTRING,0,(LPARAM)"Center");
		SendDlgItemMessage(hDlg,IDC_HALIGN,CB_ADDSTRING,0,(LPARAM)"Left");
		SendDlgItemMessage(hDlg,IDC_HALIGN,CB_ADDSTRING,0,(LPARAM)"Right");
		SendDlgItemMessage(hDlg,IDC_HALIGN,CB_SETCURSEL,lpData->hAlignment,0);

		SendDlgItemMessage(hDlg,IDC_VALIGN2,CB_ADDSTRING,0,(LPARAM)"Center");
		SendDlgItemMessage(hDlg,IDC_VALIGN2,CB_ADDSTRING,0,(LPARAM)"Top");
		SendDlgItemMessage(hDlg,IDC_VALIGN2,CB_ADDSTRING,0,(LPARAM)"Bottom");
		SendDlgItemMessage(hDlg,IDC_VALIGN2,CB_SETCURSEL,lpData->vAlignment2,0);

		SendDlgItemMessage(hDlg,IDC_HALIGN2,CB_ADDSTRING,0,(LPARAM)"Center");
		SendDlgItemMessage(hDlg,IDC_HALIGN2,CB_ADDSTRING,0,(LPARAM)"Left");
		SendDlgItemMessage(hDlg,IDC_HALIGN2,CB_ADDSTRING,0,(LPARAM)"Right");
		SendDlgItemMessage(hDlg,IDC_HALIGN2,CB_SETCURSEL,lpData->hAlignment2,0);

		strcpy(tempfont,lpData->fontName);
		strcpy(tempfont2,lpData->fontName2);

		GetLocalTime(&time);
		time.wHour = lpData->hour;
		time.wMinute = lpData->minute;
		time.wSecond = lpData->second;
		SendDlgItemMessage(hDlg,IDC_ALARMTIME, DTM_SETSYSTEMTIME,(WPARAM)GDT_VALID,(LPARAM)&time);

		SetWindowText(GetDlgItem(hDlg,IDC_SOUND),lpData->sound);

		SendMessage(hDlg,WM_PAINT,0,0);	

		break;

	case WM_COMMAND:
		char szThis[2000];
		strcpy(szThis, "");
		switch(LOWORD(wParam))
		{
		case IDCHOOSEFONT2:
			LOGFONT lf;
			ZeroMemory(&lf,sizeof(lf));
			strcpy(lf.lfFaceName,tempfont2);
			CHOOSEFONT cf;
			ZeroMemory(&cf,sizeof(cf));
			cf.lStructSize = sizeof(cf);
			cf.lpLogFont = &lf;
			cf.hwndOwner = hDlg;
			cf.hInstance = lpData->hInstanceDll;
			cf.Flags = CF_NOSTYLESEL | CF_NOSIZESEL | CF_SCREENFONTS;
			cf.nFontType = SCREEN_FONTTYPE;

			if(!ChooseFont(&cf))return TRUE;

			strcpy(tempfont2,cf.lpLogFont->lfFaceName);

			SendMessage(hDlg,WM_PAINT,0,0);
			break;

		case IDCHOOSEFONT:
			ZeroMemory(&lf,sizeof(lf));
			strcpy(lf.lfFaceName,tempfont);
			ZeroMemory(&cf,sizeof(cf));
			cf.lStructSize = sizeof(cf);
			cf.lpLogFont = &lf;
			cf.hwndOwner = hDlg;
			cf.hInstance = lpData->hInstanceDll;
			cf.Flags = CF_NOSTYLESEL | CF_NOSIZESEL | CF_SCREENFONTS;
			cf.nFontType = SCREEN_FONTTYPE;

			if(!ChooseFont(&cf))return TRUE;

			strcpy(tempfont,cf.lpLogFont->lfFaceName);

			SendMessage(hDlg,WM_PAINT,0,0);
			break;

		case IDC_VALIGN: case IDC_HALIGN: case IDC_SHADOW:case IDC_ALPHA:
		case IDC_VALIGN2: case IDC_HALIGN2: case IDC_SHADOW2:case IDC_ALPHA2:
			SendMessage(hDlg,WM_PAINT,0,0);
			break;

		case IDCHOOSECOLOR:
			CHOOSECOLOR cc;
			COLORREF colors[16];
			ZeroMemory(colors,sizeof(colors));
			ZeroMemory(&cc,sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.lpCustColors = colors;
			cc.hwndOwner = hDlg;
			cc.rgbResult = RGB(r,g,b);
			cc.Flags = CC_RGBINIT | CC_FULLOPEN;
			if(ChooseColor(&cc))
			{
				r = GetRValue(cc.rgbResult);	
				g = GetGValue(cc.rgbResult);
				b = GetBValue(cc.rgbResult);
				SendMessage(hDlg,WM_PAINT,0,0);	
			}
			break;

		case IDCHOOSECOLOR2:
			ZeroMemory(colors,sizeof(colors));
			ZeroMemory(&cc,sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.lpCustColors = colors;
			cc.hwndOwner = hDlg;
			cc.rgbResult = RGB(r2,g2,b2);
			cc.Flags = CC_RGBINIT | CC_FULLOPEN;
			if(ChooseColor(&cc))
			{
				r2 = GetRValue(cc.rgbResult);	
				g2 = GetGValue(cc.rgbResult);
				b2 = GetBValue(cc.rgbResult);
				SendMessage(hDlg,WM_PAINT,0,0);	
			}
			break;

		case IDOK:
			if(!lpData)
				break;

			GetWindowText(GetDlgItem(hDlg, IDC_TIME), lpData->lzTimeFormat, sizeof(lpData->lzTimeFormat));
			GetWindowText(GetDlgItem(hDlg, IDC_DATE), lpData->lzDateFormat, sizeof(lpData->lzDateFormat));
			GetWindowText(GetDlgItem(hDlg, IDC_TIME2), lpData->lzTimeFormat2, sizeof(lpData->lzTimeFormat2));
			GetWindowText(GetDlgItem(hDlg, IDC_DATE2), lpData->lzDateFormat2, sizeof(lpData->lzDateFormat2));
			GetWindowText(GetDlgItem(hDlg,IDC_TEXTSIZE),tmp,sizeof(tmp)/sizeof(tmp[0]));
			lpData->textsize = atoi(tmp);

			GetWindowText(GetDlgItem(hDlg,IDC_ALPHA),tmp,sizeof(tmp)/sizeof(tmp[0]));
			lpData->alpha = atoi(tmp);

			lpData->red = r;
			lpData->green = g;
			lpData->blue = b;

			GetWindowText(GetDlgItem(hDlg,IDC_TEXTSIZE2),tmp,sizeof(tmp)/sizeof(tmp[0]));
			lpData->textsize2 = atoi(tmp);

			GetWindowText(GetDlgItem(hDlg,IDC_ALPHA2),tmp,sizeof(tmp)/sizeof(tmp[0]));
			lpData->alpha2 = atoi(tmp);

			GetWindowText(GetDlgItem(hDlg,IDC_SOUND),lpData->sound,sizeof(lpData->sound)/sizeof(lpData->sound[0]));

			lpData->red2 = r2;
			lpData->green2 = g2;
			lpData->blue2 = b2;

			lpData->vAlignment = SendDlgItemMessage(hDlg,IDC_VALIGN,CB_GETCURSEL,0,0);
			lpData->hAlignment = SendDlgItemMessage(hDlg,IDC_HALIGN,CB_GETCURSEL,0,0);
			lpData->vAlignment2 = SendDlgItemMessage(hDlg,IDC_VALIGN2,CB_GETCURSEL,0,0);
			lpData->hAlignment2 = SendDlgItemMessage(hDlg,IDC_HALIGN2,CB_GETCURSEL,0,0);

			lpData->shadow = IsDlgButtonChecked(hDlg,IDC_SHADOW);
			lpData->shadow2 = IsDlgButtonChecked(hDlg,IDC_SHADOW2);

			lpData->do_attention = IsDlgButtonChecked(hDlg,IDC_ATTENTION)==BST_CHECKED;
			lpData->do_blink = IsDlgButtonChecked(hDlg,IDC_BLINK)==BST_CHECKED;

			strcpy(lpData->fontName,tempfont);
			strcpy(lpData->fontName2,tempfont2);

			SendDlgItemMessage(hDlg,IDC_ALARMTIME, DTM_GETSYSTEMTIME,(WPARAM)GDT_VALID,(LPARAM)&time);
			lpData->hour = time.wHour;
			lpData->minute = time.wMinute;
			lpData->second = time.wSecond;

			strcpy(lpData->lzTime,"");
			UpdateTime(lpData);
			EndDialog(hDlg, 0);
			return TRUE;
			
		case IDC_DATEFORMAT: case IDC_DATEFORMAT2:

			strcpy(msg,"d\tDay of month as digits with no leading zero for single-digit days.\n");
			strcat(msg,"dd\tDay of month as digits with leading zero for single-digit days.\n");
			strcat(msg,"ddd\tDay of week as a three-letter abbreviation.\n");
			strcat(msg,"dddd\tDay of week as its full name.\n");
			strcat(msg,"M\tMonth as digits with no leading zero for single-digit months.\n");
			strcat(msg,"MM\tMonth as digits with leading zero for single-digit months.\n");
			strcat(msg,"MMM	Month as a three-letter abbreviation.\n");
			strcat(msg,"MMMM\tMonth as its full name.\n");
			strcat(msg,"yy\tYear as last two digits.\n");
			strcat(msg,"yyyy\tYear represented by full four digits.\n");
			strcat(msg,"gg\tPeriod/era string.\n");
			strcat(msg,"\nFor example, to get the date string \"Wed, Aug 31 94\"\n");
			strcat(msg,"use the following picture string: \"ddd\',\' MMM dd yy\"");

			MessageBox(hDlg,msg,"Date format",MB_ICONINFORMATION);

			return TRUE;

		case IDC_TIMEFORMAT: case IDC_TIMEFORMAT2:
			strcpy(msg,"h\tHours with no leading zero for single-digit hours; 12-hour clock\n");
			strcat(msg,"hh\tHours with leading zero for single-digit hours; 12-hour clock\n");
			strcat(msg,"H\tHours with no leading zero for single-digit hours; 24-hour clock\n");
			strcat(msg,"HH\tHours with leading zero for single-digit hours; 24-hour clock\n");
			strcat(msg,"m\tMinutes with no leading zero for single-digit minutes\n");
			strcat(msg,"mm\tMinutes with leading zero for single-digit minutes\n");
			strcat(msg,"s\tSeconds with no leading zero for single-digit seconds\n");
			strcat(msg,"ss\tSeconds with leading zero for single-digit seconds\n");
			strcat(msg,"t\tOne character time marker string, such as A or P\n");
			strcat(msg,"tt\tMulticharacter time marker string, such as AM or PM\n");
			strcat(msg,"\nFor example, to get the time string \"11:29 PM\"\n");
			strcat(msg,"use the following picture string: \"hh\':\'mm tt\"");

			MessageBox(hDlg,msg,"Time format",MB_ICONINFORMATION);

			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case IDBROWSE:
			OPENFILENAME ofn;
			ZeroMemory(&ofn,sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.hInstance = GetModuleHandle(NULL);
			ofn.lpstrFilter = "Wave Files(*.wav)\0*.wav\0All Files (*.*)\0*.*\0\0";
			char tmp[MAX_PATH];
			GetWindowText(GetDlgItem(hDlg, IDC_SOUND), tmp, sizeof(tmp));
			ofn.lpstrFile = tmp; 
			ofn.nMaxFile = MAX_PATH;
			
			if(GetOpenFileName(&ofn))
				SetDlgItemText(hDlg,IDC_SOUND,ofn.lpstrFile);
			break;
		}
		break;

	case WM_DESTROY:
		RemoveProp(hDlg, "lpData");
		break;
	}

	return FALSE;
}


//Called the dock item is called to be configured by ObjectDock, such as would happen if our plugin did not impliment
// OnRightMouseClick which would result in ObjectDock displaying a menu on right click with a menu option to configure.
void CALLBACK OnConfigure(DOCKLET_DATA *lpData)
{
	//Create dialog box 
	DialogBoxParam(lpData->hInstanceDll, MAKEINTRESOURCE(IDD_CONFIG), lpData->hwndDocklet, ConfigureDocklet, (LPARAM) lpData);
}



//Process a left mouse button click. Returning TRUE reports to ObjectDock that the event was acted upon.
BOOL CALLBACK OnLeftButtonClick(DOCKLET_DATA *lpData, POINT *ptCursor, SIZE *sizeDocklet)
{
	lpData->alarm = false;
	PlaySound(NULL,0,0);
	strcpy(lpData->lzTime,"");
	UpdateTime(lpData);
	return TRUE;
}


//By implimenting the OnRightButtonClick function, ObjectDock will not automatically show a right
// click menu for the item, and thus we must create one.
BOOL CALLBACK OnRightButtonClick(DOCKLET_DATA *lpData, POINT *ptCursor, SIZE *sizeDocklet)
{
	HMENU hMenu = DeskletGetDefaultMenu(lpData->hwndDocklet,20000);

	
	AppendMenu(hMenu, MF_STRING | MF_ENABLED, 1, "Update");
	AppendMenu(hMenu, MF_STRING | MF_ENABLED | (lpData->do_alarm)?MF_CHECKED:0, 4, "Use Alarm");


	POINT ptMenu;
	GetCursorPos(&ptMenu);
	//DockletLockMouseEffect(lpData->hwndDocklet, TRUE);	//Lock the dock's zooming while our menu is in use.
	int iCommand = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, ptMenu.x, ptMenu.y, 0, lpData->hwndDocklet, NULL);
	//DockletLockMouseEffect(lpData->hwndDocklet, FALSE);
	DestroyMenu(hMenu);
	if(iCommand <= 0)
		return TRUE;

	if(iCommand >= 20000)
	{
		DeskletDoDefaultMenuCommand(lpData->hwndDocklet,20000,iCommand);
		return TRUE;
	}


	switch(iCommand)
	{
	case 1:
		strcpy(lpData->lzTime,"");
		UpdateTime(lpData);
		break;
	case 4:
		lpData->do_alarm = !lpData->do_alarm;
		break;
	}
	
	return TRUE;
}


INT OnAddPropertySheets(DOCKLET_DATA* lpData, int numPresent)
{
	
	PROPSHEETPAGE psp = {0};
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Label Format";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGLABELFORMAT);
	psp.pfnDlgProc = LabelFormatConfigProc;
	psp.lParam = (LPARAM)lpData;

	// idem for the other propertypages.

	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Images";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGIMAGES);
	psp.pfnDlgProc = ImagesConfigProc;
	psp.lParam = (LPARAM)lpData;
	
	pages[1] = CreatePropertySheetPage(&psp);

	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Other Options";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGOTHER);
	psp.pfnDlgProc = OtherConfigProc;
	psp.lParam = (LPARAM)lpData;

	pages[2] = CreatePropertySheetPage(&psp);

	PROPSHEETHEADER hdr = {0};
	hdr.dwSize = sizeof(PROPSHEETHEADER);
	hdr.dwFlags = PSH_DEFAULT;
	hdr.nStartPage = 0;
	hdr.nPages = 3;
	hdr.phpage = pages;
	hdr.hwndParent = lpData->hwndDesklet;
	hdr.pszCaption = "Properties of RecycleBin Desklet";
	hdr.pfnCallback = 0;

}



/////////////////////////////////////////////////////////////////
DWORD WINAPI UpdateTimeThread(LPVOID lpInput)
{
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)lpInput;
	while(1)
	{
		if(lpData!=NULL)
			UpdateTime(lpData);
		SleepEx(500,FALSE);
	}
}

int GetWeekNumber(int year,int month,int day)
{
    //lets calc weeknumber the cruel and hard way :D
    //Find JulianDay 
    int a = int((14-(month))/12);
    int y = year+4800-a;
    int m = (month)+(12*a)-3;
    int jd = day + int(((153*m)+2)/5) + 
                 (365*y) + int(y/4) - int(y/100) + 
                 int(y/400) - 32045;      // (gregorian calendar)
    
    //now calc weeknumber according to JD
    int d4 = (jd+31741-(jd%7))%146097%36524%1461;
    int L = int(d4/1460);
    int d1 = ((d4-L)%365)+L;
    int NumberOfWeek = int(d1/7) + 1;
    return NumberOfWeek;        
}

int GetCurrentWeekNumber()
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	return GetWeekNumber(t.wYear,t.wMonth,t.wDay);
}

void ParseWeekNumber(const char* formatstring,char* string, int buffersize)
{
	strcpy(string,"");
	int count(0);
	unsigned int i=0;
	while(i<strlen(formatstring))
	{
		if(formatstring[i]=='W' && formatstring[i+1]=='W')
		{
			i+=2;
			int wn=GetCurrentWeekNumber();
			char wns[10];
			itoa(wn,wns,10);

			if(wn<10)*(string++) = '0';

			for(unsigned int j=0;j<strlen(wns) && j <10;j++)
				*(string++) = wns[j];
		}
		else if(formatstring[i]=='w' && formatstring[i+1]=='w')
		{
			i+=2;
			int wn=GetCurrentWeekNumber();
			char wns[10];
			itoa(wn,wns,10);

			for(unsigned int j=0;j<strlen(wns) && j <10;j++)
				*(string++) = wns[j];
		}
		else
		{
			if(count++<buffersize)
				*(string++) = formatstring[i++];
		}
	}
	*string = 0;
}

void UpdateTime(DOCKLET_DATA *lpData)
{
	// this function performs the actual updating, run every ~ 1/2 sec

	// buffers
	char lzOldTime[200];
	char lzBuf[1000];
	char lzBuf2[1000];

	// get the current label time (time2)
	char lbl[2003];
	GetTimeFormat(LOCALE_USER_DEFAULT,0,NULL,lpData->lzTimeFormat2,lzBuf,sizeof(lzBuf));
	strcpy(lbl,lzBuf);
	strcat(lbl," ");

	// parse the (european) weeknumber: ADDED for AM
	ParseWeekNumber(lpData->lzDateFormat2,lzBuf2,sizeof(lzBuf2));
	GetDateFormat(LOCALE_USER_DEFAULT,0,NULL,lzBuf2,lzBuf2,sizeof(lzBuf2));
	strcat(lbl,lzBuf2);

	// and set the label
	DeskletSetSubLabel(lpData->hwndDocklet,lbl);

	// retrieve the current system time
	SYSTEMTIME time;
	GetLocalTime(&time);
	if(lpData->do_alarm && lpData->hour == time.wHour && lpData->minute == time.wMinute && lpData->second == time.wSecond)
	{
		// if the system time is equal to the alarm time AND the alarm (do_alarm)  is set
		// do an alarm

		// if defined, play a sound
		if(strlen(lpData->sound)>0)
			PlaySound(lpData->sound,NULL,SND_ASYNC | SND_LOOP);
		lpData->alarm = true;
	}

	// save the previous time for the image
	strcpy(lzOldTime,lpData->lzTime);
	
	// retrieve the new time
	GetTimeFormat(LOCALE_USER_DEFAULT,0,NULL,lpData->lzTimeFormat,lzBuf,sizeof(lzBuf));
	strcat(lzBuf," ");
	strcpy(lpData->lzTime,lzBuf);

	// parse the ( european) weeknumber: ADDED for AM
	ParseWeekNumber(lpData->lzDateFormat,lzBuf2,sizeof(lzBuf2));
	GetDateFormat(LOCALE_USER_DEFAULT,0,NULL,lzBuf2,lzBuf2,sizeof(lzBuf2));

	// copy the new time string into the current time buffer
	strcat(lpData->lzTime,lzBuf2);
	
	if(strcmp(lzOldTime,lpData->lzTime)!=0 || lpData->alarm)
	{
		// if the old time displayed is different from the current time, update the icon

		// get a bitmap buffer
		Bitmap *image = new Bitmap(192, 192, PixelFormat32bppARGB);

		// we want smooth anti-aliasing :)
		Graphics graphics(image);
		graphics.SetInterpolationMode(InterpolationModeHighQuality);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	
		OLECHAR font[800];
		if(!lpData->alarm)
		{
			// if not in alarm-mode, draw the normal text
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpData->fontName, -1, font, sizeof(font));
			DrawNiceText(&graphics,lpData->red, lpData->green,lpData->blue,lpData->alpha, lpData->lzTime, font, FontStyleBold, lpData->textsize, Rect(0,0,192,192),lpData->shadow,lpData->vAlignment,lpData->hAlignment);
		}
		else
		{
			// otherwise, draw the alarm defined text
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpData->fontName2, -1, font, sizeof(font));
			if(!lpData->do_blink || lpData->blinknum==1) // no-blink
			{
				DrawNiceText(&graphics,lpData->red, lpData->green,lpData->blue,lpData->alpha, lpData->lzTime, font, FontStyleBold, lpData->textsize2, Rect(0,0,192,192),lpData->shadow2,lpData->vAlignment2,lpData->hAlignment2);
			}
			else // blink
			{
				DrawNiceText(&graphics,lpData->red2, lpData->green2,lpData->blue2,lpData->alpha2, lpData->lzTime, font, FontStyleBold, lpData->textsize2, Rect(0,0,192,192),lpData->shadow2,lpData->vAlignment2,lpData->hAlignment2);
			}
		}

		// set the icon
		DeskletSetImageOverlay(lpData->hwndDocklet,image);
	}

	if(lpData->alarm)
	{
		// if we are in alarm mode and do Attention animation is set on, do default attention animation
		//if(lpData->do_attention)
		//	DockletDoAttentionAnimation(lpData->hwndDocklet);

		// toggle the blink status
		lpData->blinknum = (lpData->blinknum==0)?1:0;
	}
}
