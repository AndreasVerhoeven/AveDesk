#include <windows.h>

#include <sapi.h>
#include <spuihelp.h>

const  WM_TTSAPPCUSTOMEVENT = WM_APP;

#include "resource.h"

#include "../DeskletSdk.h"

#include <commctrl.h>		// trivial

class Speak
{
public:
	HWND hwndDesklet;
	HINSTANCE hInstanceDll;
	//ISpVoice* pVoice;
	CComPtr<ISpVoice> pVoice;
	ISpObjectToken* pToken;

public:
	HWND GetWindow(){return hwndDesklet;};
	HINSTANCE GetDLL() { return hInstanceDll;};

	Speak(HWND hwndDesklet, HINSTANCE hInstanceDll);
	~Speak();

	char text[100024];


	char font[1024];
	BYTE a, r, g, b;
	int l,t,w,h;
	int fSize;
	int fStyle;

	int rate, volume;

	char voiceName[1024];

	void TalkIt();

	void ReadFromIni(const char* szIni,const char* szIniGroup);
	void SaveToIni(const char* szIni,const char* szIniGroup);
};

Speak::Speak(HWND hwndDesklet, HINSTANCE hInstanceDll) :
	hwndDesklet		(hwndDesklet),
	hInstanceDll	(hInstanceDll),
	pToken(0),
	a(220),r(255),g(255),b(255),
	fSize(32),
	fStyle(0),
	l(20),t(10),w(150),h(152),
	rate(0),
	volume(100)
{
	strcpy(font,"Arial");
	strcpy(text,"");
	strcpy(voiceName,"");

	//pVoice = 0;
	pVoice.CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL);
	if(pVoice)
	{
		//pVoice->SetNotifyCallbackFunction(NotifyCallBack,0,(LPARAM)this);
		pVoice->SetNotifyWindowMessage(hwndDesklet,WM_TTSAPPCUSTOMEVENT,0,0);
		pVoice->SetInterest( SPFEI_ALL_TTS_EVENTS, SPFEI_ALL_TTS_EVENTS );
	}
}

Speak::~Speak()
{
	if(pToken)
	{
		pToken->Release();
		pToken = 0;
	}
}

void Speak::ReadFromIni(const char* szIni,const char* szIniGroup)
{
	GetPrivateProfileString(szIniGroup,"SpeakFont","Arial",font,sizeof(font)/sizeof(font[0]),szIni);
	fStyle	  = GetPrivateProfileInt(szIniGroup,"SpeakStyle",0,szIni);
	fSize	  = GetPrivateProfileInt(szIniGroup,"SpeakSize",22,szIni);

	a = GetPrivateProfileInt(szIniGroup,"SpeakA",220,szIni);
	r = GetPrivateProfileInt(szIniGroup,"SpeakR",255,szIni);
	g = GetPrivateProfileInt(szIniGroup,"SpeakG",255,szIni);
	b = GetPrivateProfileInt(szIniGroup,"SpeakB",255,szIni);

	l = GetPrivateProfileInt(szIniGroup,"SpeakL",l,szIni);
	t = GetPrivateProfileInt(szIniGroup,"SpeakT",t,szIni);
	w = GetPrivateProfileInt(szIniGroup,"SpeakW",w,szIni);
	h = GetPrivateProfileInt(szIniGroup,"SpeakH",h,szIni);

	rate	= GetPrivateProfileInt(szIniGroup,"Rate",rate,szIni);
	volume	= GetPrivateProfileInt(szIniGroup,"Volume",volume,szIni);

	GetPrivateProfileString(szIniGroup,"VoiceName",voiceName,voiceName,sizeof(voiceName)/sizeof(voiceName[0]),szIni);

	char prop[2000]; 
	strcpy(prop,"name=");
	strcat(prop,voiceName);

	OLECHAR ocProp[2000];
	MultiByteToWideChar(CP_OEMCP,0,prop,-1,ocProp,sizeof(ocProp)/sizeof(ocProp[0]));


	SpFindBestToken(SPCAT_VOICES,ocProp, 0, &pToken);
	if(pVoice)
	{
		pVoice->SetVoice(pToken);
		pVoice->SetRate(rate);
		pVoice->SetVolume(volume);
	}



}

void Speak::SaveToIni(const char* szIni,const char* szIniGroup)
{
	WritePrivateProfileString(szIniGroup,"SpeakFont",font,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakStyle",fStyle,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakSize",fSize,szIni);

	WritePrivateProfileInt(szIniGroup,"SpeakA",a,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakR",r,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakG",g,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakB",b,szIni);

	WritePrivateProfileInt(szIniGroup,"SpeakL",l,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakT",t,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakW",w,szIni);
	WritePrivateProfileInt(szIniGroup,"SpeakH",h,szIni);

	WritePrivateProfileString(szIniGroup,"VoiceName",voiceName,szIni);
	WritePrivateProfileInt(szIniGroup,"Rate",rate,szIni);
	WritePrivateProfileInt(szIniGroup,"Volume",volume,szIni);
}

void Speak::TalkIt()
{
	pVoice->Speak( NULL, SPF_PURGEBEFORESPEAK, 0);
	OLECHAR mtext[10024];
	MultiByteToWideChar(CP_OEMCP,0,text,-1,mtext,10024);
	pVoice->Speak(mtext, SPF_ASYNC, NULL);
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"Speak");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Drop text on it and the magic will start.");
	*iVersion = 110;
}

Speak* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	Speak* speak = new Speak(hwndDesklet,hInstanceDll);
	DeskletActivateExtendedDragDrop(hwndDesklet);

	InitCommonControls();

	if(ini && group)
	{
		speak->ReadFromIni(ini,group);
	}
	else
	{
		char path[MAX_PATH];
		DeskletGetRelativeFolder(hwndDesklet,path);
		strcat(path,"bubble.png");
		DeskletSetImageFile(hwndDesklet,path);
	}

	return speak;
}

void __stdcall OnDestroy(Speak* speak, HWND hwndDesklet)
{
	delete speak;
}

void __stdcall OnSave(Speak* speak, const char* ini, const char* group, BOOL bIsForExport)
{
	speak->SaveToIni(ini,group);
}

void __stdcall OnProcessMessage(Speak* speak, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg==WM_TTSAPPCUSTOMEVENT && speak->pVoice)
	{
		CSpEvent event;

		while(event.GetFrom(speak->pVoice) == S_OK)
		{
			if(event.eEventId == SPEI_END_INPUT_STREAM)
			{
				//Bitmap* bmp = new Bitmap(192,192);
				DeskletSetImageOverlay(speak->GetWindow(),0,TRUE);
				DeskletLayerSetVisibility(speak->GetWindow(),0,TRUE,TRUE);
			}

			else if(event.eEventId==SPEI_WORD_BOUNDARY)
			{
				SPVOICESTATUS   Stat;
				HRESULT hr = speak->pVoice->GetStatus( &Stat, NULL );
				if( SUCCEEDED(hr) )
				{
					int nStart = (LPARAM)( Stat.ulInputWordPos / sizeof(char) );
					int nLen = Stat.ulInputWordLen;
					if(nLen > 0 && nLen < 10024)
					{
						char* curText = new char[nLen+1];
						strncpy(curText,&speak->text[nStart],nLen);
						curText[nLen]=0;
						OLECHAR* mCurText = new OLECHAR[nLen+1];
						MultiByteToWideChar(CP_OEMCP,0,curText,-1,mCurText,nLen+1);

						
						Bitmap* bmp = new Bitmap(192,192);
						Graphics g(bmp);


						OLECHAR font[1024];
						MultiByteToWideChar(CP_OEMCP,0,speak->font,-1,font,1024);
						FontFamily  fontFamily(font);
						SolidBrush  color(Gdiplus::Color(speak->a,speak->r,speak->g,speak->b));
	
						GraphicsPath path(Gdiplus::FillModeAlternate);
						GraphicsPath path2(Gdiplus::FillModeAlternate);
						StringFormat format(0, LANG_NEUTRAL);
						
						format.SetAlignment(Gdiplus::StringAlignmentCenter);
						format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

						Rect r(speak->l,speak->t,speak->w,speak->h);

						path.AddString(mCurText, -1, &fontFamily, speak->fStyle, static_cast<float>(speak->fSize), r, &format);

						g.SetSmoothingMode(SmoothingModeAntiAlias);
						g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

						g.FillPath(&color, &path);



						DeskletSetImageOverlay(speak->GetWindow(),bmp,TRUE);

						delete[] curText;
						delete[] mCurText;
					}
				}
			}
		}
		
	}
}

BOOL CALLBACK OnDrop(Speak* speak,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
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
				strncpy(speak->text,txt,10024);
				speak->TalkIt();
				GlobalUnlock(hGlobal); 
				ReleaseStgMedium(&medium); 
				return TRUE;
			}
    }
	return FALSE;
}

// same except for images sheet
int __stdcall TextConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	switch(msg)
	{		case WM_INITDIALOG:
		{
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			Speak* lpData = (Speak*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

			SetDlgItemInt(hDlg,IDC_ALPHA,lpData->a,FALSE);

			SetDlgItemInt(hDlg,IDC_LEFT,lpData->l,TRUE);
			SetDlgItemInt(hDlg,IDC_TOP,lpData->t,TRUE);
			SetDlgItemInt(hDlg,IDC_WIDTH,lpData->w,TRUE);
			SetDlgItemInt(hDlg,IDC_HEIGHT,lpData->h,TRUE);

			init = false;
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
					case IDC_LEFT: case IDC_TOP: case IDC_WIDTH: case IDC_HEIGHT: case IDC_ALPHA:
						// the user changed something, so notify the propertydialogue that there was a change.
						if(HIWORD(wParam)==EN_CHANGE)
							PropSheet_Changed(GetParent(hDlg),hDlg);
					break;

				case IDSETFONT:
				{
					Speak* lpData = (Speak*)GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

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

					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				}
				break;

				case IDSETCOLOR:
				{
					Speak* lpData = (Speak*)GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

					CHOOSECOLOR cc;
					COLORREF colors[16];
					ZeroMemory(colors,sizeof(colors));
					ZeroMemory(&cc,sizeof(cc));
					cc.lStructSize = sizeof(cc);
					cc.lpCustColors = colors;
					cc.hwndOwner = hDlg;
					cc.rgbResult = RGB(lpData->r,lpData->g,lpData->b);
					cc.Flags = CC_RGBINIT | CC_FULLOPEN;
					if(ChooseColor(&cc))
					{
						lpData->r = GetRValue(cc.rgbResult);	
						lpData->g = GetGValue(cc.rgbResult);
						lpData->b = GetBValue(cc.rgbResult);	
					}
					lpData->a = GetDlgItemInt(hDlg,IDC_ALPHA,0,FALSE);

					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				}
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
				{
					Speak* lpData = (Speak*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						lpData->a = GetDlgItemInt(hDlg,IDC_ALPHA,0,FALSE);

						lpData->l = GetDlgItemInt(hDlg,IDC_LEFT,0,TRUE);
						lpData->t = GetDlgItemInt(hDlg,IDC_TOP,0,TRUE);
						lpData->w = GetDlgItemInt(hDlg,IDC_WIDTH,0,TRUE);
						lpData->h = GetDlgItemInt(hDlg,IDC_HEIGHT,0,TRUE);
					}
				}
				break;
			}
		break;

		case WM_DESTROY:

			RemoveProp(hDlg,"lpData");
		break;
	}
	return FALSE;
}
			
// same except for images sheet
int __stdcall TTSConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	switch(msg)
	{	
		case WM_HSCROLL:
			if(!init)
				PropSheet_Changed(GetParent(hDlg),hDlg);	
		break;

		case WM_INITDIALOG:
		{
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			Speak* lpData = (Speak*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

			
			SpInitTokenComboBox( GetDlgItem( hDlg, IDC_VOICE ), SPCAT_VOICES );

			if(strcmp(lpData->voiceName,"")!=0)
			{
				SendDlgItemMessage(hDlg,IDC_VOICE,CB_SELECTSTRING,0,(LPARAM)lpData->voiceName);
			}

			SendDlgItemMessage(hDlg,IDC_RATE,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)-10);
			SendDlgItemMessage(hDlg,IDC_RATE,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)10);
			SendDlgItemMessage(hDlg,IDC_RATE,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->rate);

			SendDlgItemMessage(hDlg,IDC_VOLUME,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)0);
			SendDlgItemMessage(hDlg,IDC_VOLUME,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)100);
			SendDlgItemMessage(hDlg,IDC_VOLUME,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->volume);


			init = false;
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_VOICE:
						// the user changed something, so notify the propertydialogue that there was a change.
						if(!init && HIWORD(wParam)==CBN_SELCHANGE)
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
				{
					Speak* lpData = (Speak*)GetProp(hDlg,"lpData");
					if(lpData)
					{	
						char oldVoiceName[1024];
						strcpy(oldVoiceName,lpData->voiceName);

						GetDlgItemText(hDlg,IDC_VOICE,lpData->voiceName,sizeof(lpData->voiceName)/sizeof(lpData->voiceName[0]));

						if(strcmp(oldVoiceName,lpData->voiceName)!=0)
						{
							if(lpData->pToken)
							{
								lpData->pToken->Release();
								lpData->pToken = 0;
							}
	
							int cursel = SendDlgItemMessage(hDlg,IDC_VOICE,CB_GETCURSEL,0,0);
							if(cursel >=0 && cursel < SendDlgItemMessage(hDlg,IDC_VOICE,CB_GETCOUNT,0,0))
							{
								lpData->pToken = SpGetComboBoxToken(GetDlgItem(hDlg,IDC_VOICE),cursel);
								if(lpData->pVoice)lpData->pVoice->SetVoice(lpData->pToken);
							}
						}

						if(lpData->rate != SendDlgItemMessage(hDlg,IDC_RATE,TBM_GETPOS,0,0) ||
							lpData->volume != SendDlgItemMessage(hDlg,IDC_VOLUME,TBM_GETPOS,0,0))
						{
	
							lpData->rate	= SendDlgItemMessage(hDlg,IDC_RATE,TBM_GETPOS,0,0);
							lpData->volume	= SendDlgItemMessage(hDlg,IDC_VOLUME,TBM_GETPOS,0,0);
	
							if(lpData->pVoice)
							{
								lpData->pVoice->SetRate(lpData->rate);
								lpData->pVoice->SetVolume(lpData->volume);
							}
						}
					}
				}
				break;
			}
		break;

		case WM_DESTROY:

			RemoveProp(hDlg,"lpData");
		break;
	}
	return FALSE;
}

INT __stdcall OnAddPropertySheets(Speak* lpData, int numPresent)
{
	PROPSHEETPAGE psp = {0};
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Text Options";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_SETTINGS);
	psp.pfnDlgProc = TextConfigProc;
	psp.lParam = (LPARAM)lpData;

	DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Text-To-Speech Options";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_TTS);
	psp.pfnDlgProc = TTSConfigProc;
	psp.lParam = (LPARAM)lpData;
	/*DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Bounding Rectangle";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGBR);
	psp.pfnDlgProc = BRConfigProc;
	psp.lParam = (LPARAM)lpData;
	*/

	return DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));
}