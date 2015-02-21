// Notes.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include <Gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
#include "../DeskletSDK.h"
#include "resource.h"
#include <commctrl.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#define SCROLL_TIMER 1112222

class DOCKLET_DATA
{
	public:
		DOCKLET_DATA(HWND hwndDesklet,HINSTANCE hInstanceDll,char* t=0)
			: 
			hwndDesklet(hwndDesklet),
			hInstanceDll(hInstanceDll),
			fRotation(359),
			cA(225),cR(0),cG(0),cB(0),
			fStyle(0),fSize(14),
			AcceptDrop(false),
			fDropShadow(true),
			top(10), height(0),
			enableScrolling(true),
			timerMs(100),
			speed(2),
			br(10,10,176,163),
			scrollNeg(true),
			mouseIsOn(false),
			mouseOverPiece(0),
			goBackToStartOnMouseOut(true),
			wpOrigEditProc(0),
			scrollByKeys(false),
			boxHeight(0),
			boxWidth(0),
			h(0)
		{
			if(t)
			strcpy(text,t);
			strcpy(font,"Arial");
		}
	HWND hwndDesklet;
	HINSTANCE hInstanceDll;

	HANDLE h;

	char text[10024];
	char font[1024];

	int fRotation;
	int fStyle;
	int fSize;

	bool fDropShadow;

	BYTE cA;
	BYTE cR;
	BYTE cG;
	BYTE cB;

	int top;
	int height;

	bool enableScrolling;
	bool scrollByKeys;
	int  timerMs;
	int  speed;

	bool goBackToStartOnMouseOut;

	Rect br;

	bool scrollNeg;

	bool AcceptDrop;
	bool mouseIsOn;

	int mouseOverPiece;

	int boxHeight;
	int boxWidth;

	WNDPROC wpOrigEditProc;

	void DrawNotesText();
	void ReadFromIni(char* szIni,char* szIniGroup);
	void SaveToIni(char* szIni,char* szIniGroup);
};

void __stdcall OnProcessMessage(DOCKLET_DATA* lpData,HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_TIMER && wParam == SCROLL_TIMER)
	{
		if(lpData->mouseOverPiece != 0)
		{
			int speedT =(int)( (float)lpData->speed * lpData->mouseOverPiece/10);
			if(lpData->scrollNeg)
			{
				if(lpData->top*-1 < (lpData->height+lpData->br.GetTop() - lpData->br.GetBottom() ))
				{
	
					lpData->top-=speedT;
					lpData->DrawNotesText();
				}
				/*else
					lpData->top-=lpData->speed;*/
			}
			else
			{
				if(lpData->top < lpData->br.GetTop())
				{
					lpData->top+=speedT;
					lpData->DrawNotesText();
				}
			}
			//lpData->DrawNotesText();
		}
	}
	else if(msg == WM_KEYDOWN)
	{
		if(wParam == VK_UP)
		{
			lpData->scrollByKeys = true;
			lpData->scrollNeg = false;
			lpData->mouseOverPiece = 8;
			SetTimer(lpData->hwndDesklet,SCROLL_TIMER,lpData->timerMs,0);
		}
		else if(wParam == VK_DOWN)
		{
			lpData->scrollByKeys = true;
			lpData->scrollNeg = true;
			lpData->mouseOverPiece  = 8;
			SetTimer(lpData->hwndDesklet,SCROLL_TIMER,lpData->timerMs,0);
		}
	}
	else if(msg == WM_KEYUP)
	{
		if(lpData->scrollByKeys)
			KillTimer(lpData->hwndDesklet,SCROLL_TIMER);
		lpData->scrollByKeys = false;
	}
}

DWORD __stdcall MouseThread(LPVOID data)
{
	RECT r = {0};
	POINT p = {0};
	int height = 0;
	int middle = 0;
	int  ratio = 0;
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)data;
	while(lpData->mouseIsOn)
	{
		if(!lpData->scrollByKeys)
		{
			GetWindowRect(lpData->hwndDesklet,&r);
			height = r.bottom - r.top;
			middle = height/2;
	
			GetCursorPos(&p);
			ScreenToClient(lpData->hwndDesklet,&p);
	
			if(p.y > middle - 5 && p.y < middle +5 )
			{
				lpData->mouseOverPiece = 0;
			}
			else
			{
				ratio = height / 21;
				if(p.y > middle)
				{
					lpData->mouseOverPiece = (p.y - middle) / ratio;
				}
				else
				{
					lpData->mouseOverPiece = ((middle - p.y)/ ratio);
				}
			}
			lpData->scrollNeg = (p.y > (height/2));
		}	

		SleepEx(lpData->timerMs,TRUE);
	}
	CloseHandle(lpData->h );
	ExitThread(0);
	return FALSE;
}

BOOL __stdcall OnMouseOn(DOCKLET_DATA* lpData)
{
	if(lpData->enableScrolling && lpData->height > (lpData->br.GetBottom() - lpData->br.GetTop()))
	{
		lpData->mouseIsOn = true;
		DWORD dummy(0);
		lpData->h = CreateThread(0,0,MouseThread,(LPVOID)lpData,0,&dummy);
		SetTimer(lpData->hwndDesklet,SCROLL_TIMER,lpData->timerMs,0);
	}
	return TRUE;
}

BOOL __stdcall OnMouseOut(DOCKLET_DATA* lpData)
{
	lpData->mouseIsOn = false;
	KillTimer(lpData->hwndDesklet,SCROLL_TIMER);
	if(lpData->goBackToStartOnMouseOut)
		lpData->top = lpData->br.GetTop();
	lpData->DrawNotesText();
	return TRUE;
}

void DOCKLET_DATA::ReadFromIni(char* szIni,char* szIniGroup)
{
	GetPrivateProfileString(szIniGroup,"NotesText","Empty Note",text,sizeof(text)/sizeof(text[0]),szIni);
	int l=strlen(text);
	for(int i=0;i<l;i++)
	{
		if(text[i]==char(180))text[i]='\n';
		else if(text[i]==char(181))text[i]='\r';
		else if(text[i]==char(182))text[i]='=';
	}

	GetPrivateProfileString(szIniGroup,"NotesFont","Arial",font,sizeof(font)/sizeof(font[0]),szIni);
	fRotation = GetPrivateProfileInt(szIniGroup,"NotesRotation",359,szIni);
	fStyle	  = GetPrivateProfileInt(szIniGroup,"NotesStyle",0,szIni);
	fSize	  = GetPrivateProfileInt(szIniGroup,"NotesSize",22,szIni);

	cA = GetPrivateProfileInt(szIniGroup,"NotesCA",225,szIni);
	cR = GetPrivateProfileInt(szIniGroup,"NotesCR",0,szIni);
	cB = GetPrivateProfileInt(szIniGroup,"NotesCB",0,szIni);
	cG = GetPrivateProfileInt(szIniGroup,"NotesCG",0,szIni);

	fDropShadow = GetPrivateProfileInt(szIniGroup,"NotesShadow",TRUE,szIni) == TRUE;

	goBackToStartOnMouseOut = GetPrivateProfileInt(szIniGroup,"NotesGoBackTS",TRUE,szIni) == TRUE;

	enableScrolling = GetPrivateProfileInt(szIniGroup,"NotesEnableScroll",TRUE,szIni) == TRUE;

	timerMs	= GetPrivateProfileInt(szIniGroup,"NotesTMS",100,szIni);
	speed	= GetPrivateProfileInt(szIniGroup,"NotesSpeed",2,szIni);

	br = Rect(GetPrivateProfileInt(szIniGroup,"NotesBRL",10,szIni),
				GetPrivateProfileInt(szIniGroup,"NotesBRT",10,szIni),
				GetPrivateProfileInt(szIniGroup,"NotesBRR",176,szIni),
				GetPrivateProfileInt(szIniGroup,"NotesBRB",164,szIni)
		);

	boxHeight = GetPrivateProfileInt(szIniGroup,"BoxHeight",0,szIni);
	boxWidth = GetPrivateProfileInt(szIniGroup,"BoxWidth",0,szIni);
}

void DOCKLET_DATA::SaveToIni(char* szIni,char* szIniGroup)
{
	char tmp[10024];
	strcpy(tmp,text);
	int l=strlen(tmp);
	for(int i=0;i<l;i++)
	{
		if(tmp[i]=='\n')tmp[i]=char(180);
		else if(tmp[i]=='\r')tmp[i]=char(181);
		else if(tmp[i]== '=')tmp[i]=char(182);
	}

	WritePrivateProfileString(szIniGroup,"NotesText",tmp,szIni);

	WritePrivateProfileInt(szIniGroup,"NotesRotation",fRotation,szIni);
	WritePrivateProfileString(szIniGroup,"NotesFont",font,szIni);
	WritePrivateProfileInt(szIniGroup,"NotesStyle",fStyle,szIni);
	WritePrivateProfileInt(szIniGroup,"NotesSize",fSize,szIni);

	WritePrivateProfileInt(szIniGroup,"NotesCA",cA,szIni);
	WritePrivateProfileInt(szIniGroup,"NotesCR",cR,szIni);
	WritePrivateProfileInt(szIniGroup,"NotesCG",cG,szIni);
	WritePrivateProfileInt(szIniGroup,"NotesCB",cB,szIni);

	WritePrivateProfileInt(szIniGroup,"NotesGoBackTS",goBackToStartOnMouseOut ? TRUE : FALSE,szIni);

	WritePrivateProfileInt(szIniGroup,"NotesShadow",fDropShadow ? TRUE : FALSE,szIni);

	WritePrivateProfileInt(szIniGroup,"NotesEnableScroll",enableScrolling ? TRUE : FALSE,szIni);

	WritePrivateProfileInt(szIniGroup,"NotesTMS",timerMs,szIni);
	WritePrivateProfileInt(szIniGroup,"NotesSpeed",speed,szIni);

	WritePrivateProfileInt(szIniGroup,"NotesBRL",br.GetLeft(),szIni);
	WritePrivateProfileInt(szIniGroup,"NotesBRT",br.GetTop(),szIni);
	WritePrivateProfileInt(szIniGroup,"NotesBRR",br.GetRight()-br.GetLeft(),szIni);
	WritePrivateProfileInt(szIniGroup,"NotesBRB",br.GetBottom()-br.GetTop(),szIni);

	WritePrivateProfileInt(szIniGroup,"BoxHeight",boxHeight,szIni);
	WritePrivateProfileInt(szIniGroup,"BoxWidth",boxWidth,szIni);

}

void DrawShadow(Bitmap* image, Graphics* g,int left, int top,int width, int height, REAL grayshade, REAL alpha)
{
	// pre:  g and image are not 0
	// post: a shadow has been drawn

	const int ratio = 4;

	Bitmap bmp(image->GetWidth()/ratio,image->GetHeight()/ratio);
	Graphics tmp(&bmp);

	ImageAttributes ia;
	ColorMatrix cm;
	ZeroMemory(cm.m, 25 * sizeof(REAL));
	for (int i = 0; i < 5; i++) cm.m[i][i] = 1.0f;

	cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = grayshade;
	//cm.m[4][0] = cm.m[4][1] = cm.m[4][2] = 1;
	cm.m[3][3] = alpha;
	ia.SetColorMatrix(&cm);

	tmp.SetSmoothingMode(/*SmoothingModeAntiAlias8x8*/Gdiplus::SmoothingModeAntiAlias);
	tmp.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	tmp.DrawImage(image,Rect(0,0,image->GetWidth()/ratio,image->GetHeight()/ratio),0,0,image->GetWidth(),image->GetHeight(),
		UnitPixel,&ia,0,0);


	InterpolationMode im = g->GetInterpolationMode();
	SmoothingMode	  smode = g->GetSmoothingMode();
	g->SetSmoothingMode(/*SmoothingModeAntiAlias8x8*/Gdiplus::SmoothingModeAntiAlias);
	g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g->DrawImage(&bmp,Rect(left,top,width,height),0,0,bmp.GetWidth(),bmp.GetHeight(),
		UnitPixel,0,0,0);
	g->SetSmoothingMode(smode);
	g->SetInterpolationMode(im);

}

void DOCKLET_DATA::DrawNotesText()
{
	SIZE deskletSize = {0};
	DeskletGetSize(hwndDesklet,&deskletSize);

	BOOL layerFound = false;
	UINT id = DeskletLayerGetLayer(hwndDesklet,"Notes.Text",&layerFound);
	if(!layerFound)
		id = DeskletLayerAdd(hwndDesklet,"Notes.Text");

	Bitmap		bmp(deskletSize.cx,deskletSize.cy);
	Graphics	g(&bmp);
	g.SetSmoothingMode(/*SmoothingModeAntiAlias8x8*/SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
	OLECHAR wtext[10024];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, font , -1, wtext, sizeof(wtext));

	Gdiplus::FontFamily  fontFamily(wtext);
	SolidBrush  black(Gdiplus::Color(cA, 0, 0, 0));
	SolidBrush  color(Gdiplus::Color(cA, cR, cB, cG));
	Pen		 blackOutline(Gdiplus::Color(cA, 0, 0, 0), 1);

	GraphicsPath path(Gdiplus::FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);

	RectF rect((float)br.GetLeft(),(float)top,(float)br.GetRight()-br.GetLeft(),10000);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text , -1, wtext, sizeof(wtext));
	path.AddString(wtext, -1, &fontFamily, fStyle, (float)fSize, rect, &format);

	Rect rc;
	path.GetBounds(&rc);
	Gdiplus::Size s;
	rc.GetSize(&s);
	height = s.Height;

	g.FillPath(&color, &path);

	Bitmap* output = new Bitmap(deskletSize.cx,deskletSize.cy);
	Graphics outputG(output);
	outputG.SetClip(br,CombineModeReplace);
	if(fDropShadow)
	{
		DrawShadow(&bmp,&outputG,2,2,deskletSize.cx,deskletSize.cy,0.0f,(cA < 140) ? 50.0f/255 : 200.0f/255);
	}
	outputG.DrawImage(&bmp,0,0,deskletSize.cx,deskletSize.cy);

	DeskletLayerSetRotation(hwndDesklet,id,false,fRotation);
	DeskletLayerSetImage(hwndDesklet,id,true,output,true);
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"Notes Desklet");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"A desklet that will hold notes for you.");
	*iVersion = 110;
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

DOCKLET_DATA* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	DOCKLET_DATA* lpData = new DOCKLET_DATA(hwndDesklet,hInstance,"Empty Note");

	FORMATETC f;
	f.cfFormat = CF_TEXT;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	DeskletAddDragDropFormat(hwndDesklet,&f);
	DeskletActivateExtendedDragDrop(hwndDesklet);


	COLORREF color = RGB(200,0,0);
	ColorMatrix cm = GetIdentityMatrix();
	cm.m[4][0] = static_cast<float>GetRValue(color)	/ 255;
	cm.m[4][1] = static_cast<float>GetGValue(color)	/ 255;
	cm.m[4][2] = static_cast<float>GetBValue(color)	/ 255;
	//DeskletLayerSetColorMatrix(hwndDesklet,0,FALSE,&cm);

	if(szIni && szIniGroup)
	{
		lpData->ReadFromIni(szIni,szIniGroup);
	}
	else
	{
		char path[MAX_PATH];
		DeskletGetRelativeFolder(lpData->hwndDesklet,path);
		strcat(path,"bg.png");
		DeskletSetImageFile(lpData->hwndDesklet,path);
	}

	lpData->top = lpData->br.GetTop();
	lpData->DrawNotesText();

	return lpData;
}

void __stdcall OnDestroy(DOCKLET_DATA *lpData, HWND hwndDesklet)
{
	lpData->mouseIsOn = false;
	SleepEx(75,TRUE);
	delete lpData;
	lpData = 0;
}

void __stdcall OnSave(DOCKLET_DATA *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	lpData->SaveToIni(szIni,szIniGroup);
}

BOOL __stdcall OnDragDropDataRequest(DOCKLET_DATA* lpData,FORMATETC* format,STGMEDIUM* medium)
{
	if(format->cfFormat == CF_TEXT)
	{

		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,strlen(lpData->text)+1);
		char* memCpy = (char*)GlobalLock(hMem);

		memcpy((void*)memCpy,(void*)lpData->text,(strlen(lpData->text)+1));
		GlobalUnlock(hMem);

		medium->hGlobal = hMem;
		medium->pUnkForRelease = 0;
		medium->tymed = TYMED_HGLOBAL;
		return TRUE;
	}
	return FALSE;
}

int __stdcall FontConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

			if(!lpData)return TRUE;

			SetDlgItemText(hDlg,IDC_TEXT,lpData->text);
			SetDlgItemInt(hDlg,IDC_FONTROTATION,lpData->fRotation,TRUE);
			SetDlgItemInt(hDlg,IDC_COLORALPHA,lpData->cA,FALSE);
			CheckDlgButton(hDlg,IDC_SHADOW,lpData->fDropShadow ? TRUE : FALSE);
			init=false;
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_FONTROTATION: case IDC_COLORALPHA: case IDC_SHADOW:
					if(!init)
					{
						GetDlgItemText(hDlg,IDC_TEXT,lpData->text,sizeof(lpData->text)/sizeof(lpData->text[0]));
						lpData->fRotation = GetDlgItemInt(hDlg,IDC_FONTROTATION,0,TRUE);
						lpData->cA = GetDlgItemInt(hDlg,IDC_COLORALPHA,0,FALSE);
						lpData->fDropShadow = IsDlgButtonChecked(hDlg,IDC_SHADOW) == BST_CHECKED;
						lpData->DrawNotesText();
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				break;
				case IDSELECTFONT:
					LOGFONT lf;
					ZeroMemory(&lf,sizeof(lf));
					strcpy(lf.lfFaceName,lpData->font);
					lf.lfHeight = lpData->fSize * GetDeviceCaps(GetDC(0), LOGPIXELSY) / 72;
					lf.lfItalic = lpData->fStyle  & FontStyleItalic;
					lf.lfStrikeOut = lpData->fStyle  & FontStyleStrikeout;
					lf.lfUnderline = lpData->fStyle  & FontStyleUnderline;
					lf.lfWeight = (lpData->fStyle & FontStyleBold) ? FW_BOLD : FW_REGULAR;
					CHOOSEFONT cf;
					ZeroMemory(&cf,sizeof(cf));
					cf.lStructSize = sizeof(cf);
					cf.lpLogFont = &lf;
					cf.hwndOwner = hDlg;
					cf.hInstance = lpData->hInstanceDll;
					cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
					cf.nFontType = SCREEN_FONTTYPE;
					cf.iPointSize = lpData->fSize*10;
					if(!ChooseFont(&cf))return TRUE;

					lpData->fStyle  = FontStyleRegular;
					if(lf.lfItalic) lpData->fStyle |= FontStyleItalic;
					if(lf.lfStrikeOut) lpData->fStyle |= FontStyleStrikeout;
					if(lf.lfUnderline) lpData->fStyle  |= FontStyleUnderline;
					if(lf.lfWeight >= FW_BOLD) lpData->fStyle  |= FontStyleBold;

					lpData->fSize = cf.iPointSize/10;
					strcpy(lpData->font,lf.lfFaceName);

					GetDlgItemText(hDlg,IDC_TEXT,lpData->text,sizeof(lpData->text)/sizeof(lpData->text[0]));
					lpData->fRotation = GetDlgItemInt(hDlg,IDC_FONTROTATION,0,TRUE);
					lpData->cA = GetDlgItemInt(hDlg,IDC_COLORALPHA,0,FALSE);
					lpData->fDropShadow = IsDlgButtonChecked(hDlg,IDC_SHADOW) == BST_CHECKED;
					lpData->DrawNotesText();
					PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDSELECTCOLOR:
					CHOOSECOLOR cc;
					COLORREF colors[16];
					ZeroMemory(colors,sizeof(colors));
					ZeroMemory(&cc,sizeof(cc));
					cc.lStructSize = sizeof(cc);
					cc.lpCustColors = colors;
					cc.hwndOwner = hDlg;
					cc.rgbResult = RGB(lpData->cR,lpData->cG,lpData->cB);
					cc.Flags = CC_RGBINIT | CC_FULLOPEN;
					if(ChooseColor(&cc))
					{
						lpData->cR = GetRValue(cc.rgbResult);	
						lpData->cG = GetGValue(cc.rgbResult);
						lpData->cB = GetBValue(cc.rgbResult);	
					}
					GetDlgItemText(hDlg,IDC_TEXT,lpData->text,sizeof(lpData->text)/sizeof(lpData->text[0]));
					lpData->fRotation = GetDlgItemInt(hDlg,IDC_FONTROTATION,0,TRUE);
					lpData->cA = GetDlgItemInt(hDlg,IDC_COLORALPHA,0,FALSE);
					lpData->fDropShadow = IsDlgButtonChecked(hDlg,IDC_SHADOW) == BST_CHECKED;
					lpData->DrawNotesText();
					PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
					if(lpData)
					{
						GetDlgItemText(hDlg,IDC_TEXT,lpData->text,sizeof(lpData->text)/sizeof(lpData->text[0]));
						lpData->fRotation = GetDlgItemInt(hDlg,IDC_FONTROTATION,0,TRUE);
						lpData->cA = GetDlgItemInt(hDlg,IDC_COLORALPHA,0,FALSE);
						lpData->fDropShadow = IsDlgButtonChecked(hDlg,IDC_SHADOW) == BST_CHECKED;
						lpData->DrawNotesText();
					}
				break;
			}
		break;
	}

	return FALSE;
}

int __stdcall ScrollConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

			CheckDlgButton(hDlg,IDC_DOSCROLLING,lpData->enableScrolling ? TRUE : FALSE);

			SendDlgItemMessage(hDlg,IDC_SMOOTHNESS,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)10);
			SendDlgItemMessage(hDlg,IDC_SMOOTHNESS,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)200);
			SendDlgItemMessage(hDlg,IDC_SMOOTHNESS,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)200-lpData->timerMs);

			SendDlgItemMessage(hDlg,IDC_SPEED,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)1);
			SendDlgItemMessage(hDlg,IDC_SPEED,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)100);
			SendDlgItemMessage(hDlg,IDC_SPEED,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->speed);

			CheckDlgButton(hDlg,IDC_GOBACKTOTOP,lpData->goBackToStartOnMouseOut ? TRUE : FALSE);

			if(!lpData)return TRUE;
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_GOBACKTOTOP: case IDC_DOSCROLLING:
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case WM_HSCROLL:
					PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;
		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
					if(lpData)
					{
						lpData->enableScrolling = IsDlgButtonChecked(hDlg,IDC_DOSCROLLING) == BST_CHECKED;

						lpData->timerMs			= 200-SendDlgItemMessage(hDlg,IDC_SMOOTHNESS,TBM_GETPOS,0,0);
						lpData->speed			= SendDlgItemMessage(hDlg,IDC_SPEED,TBM_GETPOS,0,0);

						lpData->goBackToStartOnMouseOut = IsDlgButtonChecked(hDlg,IDC_GOBACKTOTOP) == BST_CHECKED;
					}
				break;
			}
		break;
	}
	return FALSE;
}

int __stdcall BRConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

			CheckDlgButton(hDlg,IDC_DOSCROLLING,lpData->enableScrolling ? TRUE : FALSE);

			
			SetDlgItemInt(hDlg,IDC_LEFT,lpData->br.GetLeft(),FALSE);
			SetDlgItemInt(hDlg,IDC_TOP,lpData->br.GetTop(),FALSE);
			SetDlgItemInt(hDlg,IDC_RIGHT,lpData->br.GetRight()-lpData->br.GetLeft(),FALSE);
			SetDlgItemInt(hDlg,IDC_BOTTOM,lpData->br.GetBottom()-lpData->br.GetTop(),FALSE);

			if(!lpData)return TRUE;
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_LEFT: case IDC_TOP: case IDC_RIGHT: case IDC_BOTTOM:
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
					if(lpData)
					{
						lpData->br = Rect(GetDlgItemInt(hDlg,IDC_LEFT,0,FALSE),
											GetDlgItemInt(hDlg,IDC_TOP,0,FALSE),
											GetDlgItemInt(hDlg,IDC_RIGHT,0,FALSE),
											GetDlgItemInt(hDlg,IDC_BOTTOM,0,FALSE)
										);
						lpData->DrawNotesText();
					}
				break;
			}
		break;
	}
	return FALSE;
}

INT __stdcall OnAddPropertySheets(DOCKLET_DATA* lpData, int numPresent)
{
	PROPSHEETPAGE psp = {0};
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Notes Text";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGTEXT);
	psp.pfnDlgProc = FontConfigProc;
	psp.lParam = (LPARAM)lpData;
	DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Scrolling";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGSCROLL);
	psp.pfnDlgProc = ScrollConfigProc;
	psp.lParam = (LPARAM)lpData;
	DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Bounding Rectangle";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGBR);
	psp.pfnDlgProc = BRConfigProc;
	psp.lParam = (LPARAM)lpData;

	return DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));
}

LRESULT APIENTRY EditSubclassProc(
    HWND hwnd, 
    UINT uMsg, 
    WPARAM wParam, 
    LPARAM lParam) 
{ 
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(GetParent(hwnd),"lpData");
    if (uMsg == WM_CONTEXTMENU) 
	{
		HMENU menu = CreatePopupMenu();//DeskletGetDefaultMenu(lpData->hwndDesklet,10);

		InsertMenu(menu,0,MF_BYPOSITION	| MF_ENABLED | MF_STRING,1,"Update");
		InsertMenu(menu,1,MF_BYPOSITION	| MF_SEPARATOR,1,"-");
		InsertMenu(menu,2,MF_BYPOSITION	| MF_ENABLED | MF_STRING,2,"Undo");
		InsertMenu(menu,3,MF_BYPOSITION	| MF_SEPARATOR,1,"-");
		InsertMenu(menu,4,MF_BYPOSITION	| MF_ENABLED | MF_STRING,3,"Cut");
		InsertMenu(menu,5,MF_BYPOSITION	| MF_ENABLED | MF_STRING,4,"Copy");
		InsertMenu(menu,6,MF_BYPOSITION	| MF_ENABLED | MF_STRING,5,"Paste");
		InsertMenu(menu,7,MF_BYPOSITION	| MF_SEPARATOR,1,"-");
		InsertMenu(menu,8,MF_BYPOSITION	| MF_ENABLED | MF_STRING,6,"Select All");
		//InsertMenu(menu,9,MF_BYPOSITION	| MF_SEPARATOR,1,"-");
		
		POINT p;
		GetCursorPos(&p);

		int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, p.x, p.y, 0, hwnd,0);
		DestroyMenu(menu);
		if(cmd > 10)
			DeskletDoDefaultMenuCommand(lpData->hwndDesklet,10,cmd);
		else
		{
			switch(cmd)
			{
				case 1: EndDialog(GetParent(hwnd),0); break;
				case 2:	SendMessage(hwnd,WM_UNDO,0,0); break;
				case 3:	SendMessage(hwnd,WM_CUT,0,0); break;
				case 4:	SendMessage(hwnd,WM_COPY,0,0); break;
				case 5:	SendMessage(hwnd,WM_PASTE,0,0); break;
				case 6:	SendMessage(hwnd,EM_SETSEL,0,-1); break;
			}
		}
		return TRUE;
	}
	else if(uMsg == WM_KILLFOCUS)
	{
		EndDialog(GetParent(hwnd),0);
		return TRUE;
	}
 
	if(lpData && lpData->wpOrigEditProc)
	{
		return CallWindowProc(lpData->wpOrigEditProc, hwnd, uMsg, 
        wParam, lParam); 
	}
	return FALSE;
} 


int __stdcall TextEditorProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_SIZING:
			RECT* r;
			r = (RECT*)lParam;
			MoveWindow(GetDlgItem(hDlg,IDC_TEXT),0,0,r->right-r->left,r->bottom-r->top,TRUE);
		break;

		case WM_SIZE:
			if(lpData)
			{
				lpData->boxHeight = HIWORD(lParam);
				lpData->boxWidth =  LOWORD(lParam);
			}
		break;

		case WM_INITDIALOG:
			SetProp(hDlg,"lpData",(HANDLE)lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
			if(!lpData)return TRUE;

			if(lpData)
			{
				RECT r;
				GetWindowRect(lpData->hwndDesklet,&r);

				if(lpData->boxHeight == 0 || lpData->boxWidth == 0)
				{
					SIZE s;
					DeskletGetSize(lpData->hwndDesklet,&s);
	
					MoveWindow(GetDlgItem(hDlg,IDC_TEXT),0,0,s.cx,s.cy,FALSE);
			
					MoveWindow(hDlg,r.left,r.top,s.cx,s.cy,TRUE);
				}
				else
				{
					MoveWindow(GetDlgItem(hDlg,IDC_TEXT),0,0,lpData->boxWidth,lpData->boxHeight,FALSE);
					MoveWindow(hDlg,r.left,r.top,lpData->boxWidth+2,lpData->boxHeight+2,TRUE);
				}

			}
			

			SetDlgItemText(hDlg,IDC_TEXT,lpData->text);
			SetFocus(GetDlgItem(hDlg,IDC_TEXT));

			lpData->wpOrigEditProc = (WNDPROC) SetWindowLong(GetDlgItem(hDlg,IDC_TEXT), 
                GWL_WNDPROC, (LONG) EditSubclassProc); 

		break;

		case WM_DESTROY:
			SetWindowLong(GetDlgItem(hDlg,IDC_TEXT), GWL_WNDPROC, 
                (LONG) lpData->wpOrigEditProc); 

			GetDlgItemText(hDlg,IDC_TEXT,lpData->text,sizeof(lpData->text)/sizeof(lpData->text[0]));
			lpData->DrawNotesText();

		break;
	}
	return FALSE;
}

BOOL __stdcall OnLeftButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	RECT r = {0};
	GetWindowRect(lpData->hwndDesklet,&r);
	DialogBoxParam(lpData->hInstanceDll, MAKEINTRESOURCE(IDD_TEXTEDITOR), lpData->hwndDesklet, TextEditorProc, (LPARAM)lpData);
	return TRUE;
}
BOOL CALLBACK OnDragEnter(DOCKLET_DATA* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	FORMATETC fmtetc; 

    fmtetc.cfFormat = CF_TEXT; 
    fmtetc.ptd      = 0; 
    fmtetc.dwAspect = DVASPECT_CONTENT; 
    fmtetc.lindex   = -1; 
    fmtetc.tymed    = TYMED_HGLOBAL;
	
	if(!SUCCEEDED(obj->QueryGetData(&fmtetc)))
	{
		lpData->AcceptDrop = false;
		*effect = DROPEFFECT_NONE;
	}
	else
	{
		lpData->AcceptDrop = true;
	}

	return TRUE;
}

BOOL CALLBACK OnDragOver(DOCKLET_DATA* lpData,DWORD keys, POINTL pnt, DWORD* effect)
{
	if(!lpData->AcceptDrop)
	{
		*effect = DROPEFFECT_NONE;
	}
	else
	{	
	}
	return TRUE;
}

BOOL CALLBACK OnDrop(DOCKLET_DATA* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	FORMATETC fmtetc; 

    fmtetc.cfFormat = CF_TEXT; 
    fmtetc.ptd      = 0; 
    fmtetc.dwAspect = DVASPECT_CONTENT; 
    fmtetc.lindex   = -1; 
    fmtetc.tymed    = TYMED_HGLOBAL; 

	if(SUCCEEDED(obj->QueryGetData(&fmtetc)))
	{
		    STGMEDIUM medium; 
			HRESULT hr = obj->GetData(&fmtetc, &medium); 

			if (!FAILED(obj->GetData(&fmtetc, &medium))) 
			{ 
				HGLOBAL hGlobal = medium.hGlobal;
				char* txt = (char*)GlobalLock(hGlobal);
				if(keys & MK_CONTROL)
					strncat(lpData->text,txt,10024-strlen(lpData->text));
				else
				strncpy(lpData->text,txt,10024);
				lpData->DrawNotesText();
				GlobalUnlock(hGlobal); 
				ReleaseStgMedium(&medium); 
				return TRUE;
			}
    }
	return FALSE;
}

BOOL __stdcall OnMouseWheelForward(DOCKLET_DATA* lpData, WORD fKeysDown, int numOfScrolls, int x, int y)
{
	if(lpData->top < lpData->br.GetTop())
	{
		KillTimer(lpData->hwndDesklet,SCROLL_TIMER);
		lpData->top += lpData->fSize;
		lpData->DrawNotesText();
	}
	return TRUE;
}

BOOL __stdcall OnMouseWheelBackward(DOCKLET_DATA* lpData, WORD fKeysDown, int numOfScrolls, int x, int y)
{
	//if(lpData->top < lpData->br.GetTop())
	{
		KillTimer(lpData->hwndDesklet,SCROLL_TIMER);
		lpData->top -= lpData->fSize;
		lpData->DrawNotesText();
	}
	return TRUE;
}

BOOL __stdcall OnMouseMove(DOCKLET_DATA* lpData, int x, int y)
{
	return TRUE;
}	
