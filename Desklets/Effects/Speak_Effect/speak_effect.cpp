#include <windows.h>					// trivial

#include <gdiplus.h>					// for GDI+ stuff
using namespace Gdiplus;				// idem
#pragma comment(lib, "gdiplus.lib")		// idem
	
#include "../deskletsdk.h"				// for the DeskletSDK
#include "../drawpluginsdk.h"			// for the DrawPluginSDK
#include "resource.h"					// trivial
#include <commctrl.h>					// trivial

#include <sapi.h>
#include <spuihelp.h>

const int reactToMouseOver	= 0;
const int reactToSelect		= 1;
const int reactToClick		= 2;

// This struct will hold the data for an instance of this drawplugin
class SpeakEffect
{
	public:
		HWND		hwndDesklet;		// The desklet that this effect is attached to
		HINSTANCE	hInstanceDll;		// The handle to this DLL file

		SpeakEffect(HWND hwndDesklet, HINSTANCE hInstanceDll);
		~SpeakEffect();

		CComPtr<ISpVoice> pVoice;
		ISpObjectToken* pToken;

		char voiceName[1024];
	
		int rate;
		int volume;
		int reactTo;

		bool useOwnText;

		char ownText[1024];

		void Talk();

		void ReadFromIni(const char* szIni,const char* szIniGroup);
		void SaveToIni(const char* szIni,const char* szIniGroup);
};

SpeakEffect::SpeakEffect(HWND hwndDesklet, HINSTANCE hInstanceDll)
	:
	hwndDesklet(hwndDesklet),
	hInstanceDll(hInstanceDll),
	pToken(0),
	rate(0),
	volume(100),
	reactTo(reactToMouseOver),
	useOwnText(false)
{
	strcpy(ownText,"");
	strcpy(voiceName,"");
	pVoice.CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL);
}

SpeakEffect::~SpeakEffect()
{
	if(pToken)
	{
		pToken->Release();
		pToken = 0;
	}
}

void SpeakEffect::ReadFromIni(const char* szIni,const char* szIniGroup)
{
	rate	= GetPrivateProfileInt(szIniGroup,"_ER",rate,szIni);
	volume	= GetPrivateProfileInt(szIniGroup,"_EV",volume,szIni);
	reactTo = GetPrivateProfileInt(szIniGroup,"_ERT",reactTo,szIni);

	useOwnText = GetPrivateProfileInt(szIniGroup,"_EUOT",FALSE,szIni) == TRUE;
	GetPrivateProfileString(szIniGroup,"_EOT",ownText,ownText,sizeof(ownText)/sizeof(ownText[0]),szIni);

	GetPrivateProfileString(szIniGroup,"_EVN",voiceName,voiceName,sizeof(voiceName)/sizeof(voiceName[0]),szIni);

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

void SpeakEffect::SaveToIni(const char* szIni,const char* szIniGroup)
{
	WritePrivateProfileInt(szIniGroup,"_EUOT",useOwnText ? TRUE : FALSE, szIni);
	WritePrivateProfileString(szIniGroup,"_EOT",ownText,szIni);
	WritePrivateProfileInt(szIniGroup,"ERT",reactTo,szIni);
	WritePrivateProfileString(szIniGroup,"_EVN",voiceName,szIni);
	WritePrivateProfileInt(szIniGroup,"_ER",rate,szIni);
	WritePrivateProfileInt(szIniGroup,"_EV",volume,szIni);
}

void SpeakEffect::Talk()
{
	pVoice->Speak( NULL, SPF_PURGEBEFORESPEAK, 0);
	OLECHAR mtext[10024];

	if(useOwnText)
	{
		MultiByteToWideChar(CP_OEMCP,0,ownText,-1,mtext,10024);
	}
	else
	{
		char lbl[10024];
		DeskletGetLabel(hwndDesklet,lbl);
		MultiByteToWideChar(CP_OEMCP,0,lbl,-1,mtext,10024);
	}
	pVoice->Speak(mtext, SPF_ASYNC, NULL);
}

void __stdcall OnDrawPluginInfo(char* name, char* author, char* notes, int* version)
{
	strcpy(name,"Speak");
	strcpy(author,"Andreas Verhoeven");
	strcpy(notes,"Speak Effect for AveDesk1.1");
	*version = 110;
}

SpeakEffect* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, char* iniFile, char* iniGroup)
{
	// create an new PLUGIN_DATA object for this instance.

	// set some default parameters
	SpeakEffect* lpData = new SpeakEffect(hwndDesklet,hInstanceDll);
	
	InitCommonControls();

	if(iniFile && iniGroup)
	{
		lpData->ReadFromIni(iniFile,iniGroup);
	}

	return lpData;
}

void __stdcall OnSave(SpeakEffect* lpData,char* iniFile, char* iniGroup)
{
	lpData->SaveToIni(iniFile,iniGroup);
}

//	This function is called when a DrawPlugin instance is destroyed. You should delete all allocated memory,
//	detroy all windows, exit all created threads etc... here.
void __stdcall OnDestroy(SpeakEffect* lpData)
{
	delete lpData;
}

void SaveFromDlg(SpeakEffect* lpData, HWND hDlg)
{
	lpData->reactTo = SendDlgItemMessage(hDlg,IDC_REACTTO,CB_GETCURSEL,0,0);
	lpData->useOwnText = IsDlgButtonChecked(hDlg,IDC_USEOWNTEXT) == BST_CHECKED;
	GetDlgItemText(hDlg,IDC_OWNTEXT,lpData->ownText,1024);


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

// same except for images sheet
int __stdcall ConfigDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
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
			SetProp(hDlg,"lpData",(HANDLE)lParam);
			SpeakEffect* lpData = (SpeakEffect*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

			
			SpInitTokenComboBox( GetDlgItem( hDlg, IDC_VOICE ), SPCAT_VOICES );

			if(strcmp(lpData->voiceName,"")!=0)
			{
				SendDlgItemMessage(hDlg,IDC_VOICE,CB_SELECTSTRING,0,(LPARAM)lpData->voiceName);
			}

			SendDlgItemMessage(hDlg,IDC_REACTTO,CB_ADDSTRING,0,(LPARAM)"Mouse-Over");
			SendDlgItemMessage(hDlg,IDC_REACTTO,CB_ADDSTRING,0,(LPARAM)"Single-Click (Select)");
			SendDlgItemMessage(hDlg,IDC_REACTTO,CB_ADDSTRING,0,(LPARAM)"Double-Click (Launch)");
			SendDlgItemMessage(hDlg,IDC_REACTTO,CB_SETCURSEL,lpData->reactTo,0);

			CheckDlgButton(hDlg,IDC_USEOWNTEXT,lpData->useOwnText ? TRUE : FALSE);
			SetDlgItemText(hDlg,IDC_OWNTEXT,lpData->ownText);

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
				case IDTEST:
				{
					SpeakEffect* lpData = (SpeakEffect*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						SaveFromDlg(lpData,hDlg);
						lpData->Talk();
					}
				}
				break;

				case IDOK:
				{
					SpeakEffect* lpData = (SpeakEffect*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						SaveFromDlg(lpData,hDlg);
					}
					EndDialog(hDlg,TRUE);
				}
				break;
			}
		break;

		case WM_CLOSE:
		{
			SpeakEffect* lpData = (SpeakEffect*)GetProp(hDlg,"lpData");
			if(lpData)
			{
					SaveFromDlg(lpData,hDlg);
			}
			EndDialog(hDlg,TRUE);
		}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
		break;
	}
	return FALSE;
}


//	This function is called when the user hits the "Configure" button for a particular instance of this
//	DrawPlugin. You can show your own dialog for modifying settings or simply don't implement/export this
//	routine to let AveDesk show the user that there is no configuration for this DrawPlugin.
void __stdcall OnConfigure(SpeakEffect* lpData, HWND owner)
{
	// Show our settings dialog.
	DialogBoxParam(lpData->hInstanceDll, MAKEINTRESOURCE(IDD_CONFIG), owner, ConfigDlgProc, (LPARAM)lpData);
}

//	This function is called when an event occurs in the desklet that this DrawPlugin instance is attached too
//	(for a list of events, see the DeskletSDK). This DrawPlugin instance can than take whatever action to modify,
//	the painting process for the desklet.
void __stdcall OnEvent(SpeakEffect* lpData, UINT event)
{
	if(lpData->reactTo == reactToMouseOver && event == EVENT_ONMOUSEON)
	{
		lpData->Talk();	
	}
	else if(lpData->reactTo == reactToSelect && event == EVENT_ONSELECT)
	{
		lpData->Talk();
	}
	else if(lpData->reactTo == reactToClick && event == EVENT_ONLEFTCLICK)
	{
		lpData->Talk();
	}
}

