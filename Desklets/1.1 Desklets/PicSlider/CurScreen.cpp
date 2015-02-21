#include <windows.h>				// trivial

#include <Gdiplus.h>				// for gdiplus stuff
using namespace Gdiplus;			// we will use the gdiplus namespace so we don't have to do Gdiplus::Bitmap,
									// but just Bitmap
#pragma comment(lib, "gdiplus.lib")	// link the gdiplus lib

#include "resource.h"

#include <string>

#include <shlobj.h>

#include "QGaussFilter.h"

#include "SearchFolder.hpp"

#include "../DeskletSDK.h"				// for the deskletSDK

class DOCKLET_DATA
{
	public:
		RECT r,r2;
		int update;
		char onclick[MAX_PATH];

		bool useBuf1;

		int count;

		bool changeFlag;

		int l1,l2,l3;

		char maskPath[MAX_PATH];
		char overlayPath[MAX_PATH];

		Bitmap* mask;

		DOCKLET_DATA()
			: search(""),
			useBuf1(true),
			count(0),
			mask(0),
			changeFlag(true)

		{
		}

		~DOCKLET_DATA()
		{
			delete mask;
		}

		std::string folder;

		Search search;
		std::string curFile;

		HWND hwndDesklet;
		HINSTANCE hInstanceDll;
};

#define TIMER_ID 101329
#define BLEND_ID 154339

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	// copy the name, author, notes and version number
	strcpy(szName,"PicSlider Desklet");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"A desklet that will show an overlay of the current screen.");
	*iVersion = 100;
}


#ifndef CAPTUREBLT
#define CAPTUREBLT 0x40000000
#endif

void UpdateScreen(DOCKLET_DATA* lpData)
{

	SIZE s= { 0 };
	DeskletGetSize(lpData->hwndDesklet,&s);
	Bitmap* bmp = new Bitmap(s.cx,s.cy);
	Bitmap* overlay = 0;

	if(lpData->mask)
	{
		if(lpData->mask->GetWidth()!=s.cx || lpData->mask->GetHeight()!=s.cy)
			lpData->changeFlag = true;
	}

	if(lpData->mask == 0 || lpData->mask->GetWidth()!=s.cx || lpData->mask->GetHeight()!=s.cy)
	{
		delete lpData->mask;
		if(strcmp(lpData->maskPath,"")!=0)
		{
			lpData->mask = new Bitmap(s.cx,s.cy);
			char path[MAX_PATH];
			DeskletGetRelativeFolder(lpData->hwndDesklet,path);
			strcat(path,"masks\\");
			strcat(path,lpData->maskPath);
			Bitmap* tmpMask = DeskletLoadGDIPlusImage(path);
			Graphics g(lpData->mask);
			g.DrawImage(tmpMask,0,0,s.cx,s.cy);
			delete tmpMask;
		}
	}

	Bitmap* curImg = DeskletLoadGDIPlusImage(const_cast<char*>(lpData->curFile.c_str()) );
	Bitmap* curOverlay = 0;

	if(lpData->changeFlag && strcmp(lpData->overlayPath,"")!=0)
	{
		overlay = new Bitmap(s.cx,s.cy);
		curOverlay = DeskletLoadGDIPlusImage(lpData->overlayPath);
	}

	int l =s.cx*lpData->r.left / 192;
	int t =s.cy * lpData->r.top / 192 ;
	int w=(s.cx*(lpData->r.right - lpData->r.left)/192);
	int h =(s.cy*(lpData->r.bottom - lpData->r.top)/192);

	float scaleX=1;
	float scaleY=1;
	
	if(curImg->GetWidth() > curImg->GetHeight())
	{
		scaleY = curImg->GetWidth() / w;
		int z = curImg->GetHeight() / scaleY;
		t = h/2 - z /2;
		h = z;

	}
	else
	{
		scaleX = curImg->GetHeight() / h;
		int z = curImg->GetWidth() / scaleX;
		l = w/2 - z /2;
		w = z;
	}


	Graphics g2(bmp);
	g2.SetSmoothingMode(SmoothingModeAntiAlias);
	g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	//g2.DrawImage(curImg,s.cx*lpData->r.left / 192, s.cy * lpData->r.top / 192,s.cx*(lpData->r.right - lpData->r.left)/192,s.cy*(lpData->r.bottom - lpData->r.top)/192 );
	g2.DrawImage(curImg,l,t,w,h);

	if(overlay)
	{
		Graphics g3(overlay);
		g3.SetSmoothingMode(SmoothingModeAntiAlias);
		g3.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		g3.DrawImage(curOverlay,float(s.cx*lpData->r2.left / 192 + l), float(s.cy * lpData->r2.top / 192 + t),float((s.cx*(lpData->r2.right - lpData->r2.left)/192)/scaleX + w),float((s.cy*(lpData->r2.bottom - lpData->r2.top)/192)/scaleY + h) );
		delete curOverlay;
	}

	delete curImg;
	QGaussFilter f;
	if(bmp)
	{
		Bitmap* tmp = f.GetUnsharpMask(bmp, 1.0f, 1.0f,0);
		delete bmp;
		bmp = tmp;
	}

	if(lpData->mask)
	{
		for(int x=0;x<s.cx;++x)
		{
			for(int y=0;y<s.cy;++y)
			{
				Color color;
				Color maskColor;
				lpData->mask->GetPixel(x,y,&maskColor);
				bmp->GetPixel(x,y,&color);
				BYTE alpha = int(maskColor.GetRed() * color.GetAlpha()) / 255;
				color = Color::MakeARGB(alpha,color.GetRed(),color.GetGreen(),color.GetBlue() );
				bmp->SetPixel(x,y,color);

				if(overlay)
				{
					overlay->GetPixel(x,y,&color);
					alpha = int(maskColor.GetRed() * color.GetAlpha() ) / 255;
					color = Color::MakeARGB(alpha,color.GetRed(),color.GetGreen(),color.GetBlue() );
					overlay->SetPixel(x,y,color); 
				}
			}
		}
	}

	if(lpData->changeFlag)
		DeskletLayerSetImage(lpData->hwndDesklet,DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Overlay",0),FALSE,overlay,TRUE);
	else
	{
		if(strcmp(lpData->overlayPath,"")==0)
			DeskletLayerSetImage(lpData->hwndDesklet,DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Overlay",0),FALSE,0,TRUE);
	}


	int id;
	if(lpData->useBuf1)
		id = DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf1",0);
	else
		id = DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf2",0);

	DeskletLayerSetImage(lpData->hwndDesklet,id,true,bmp,true);

	lpData->changeFlag = false;
}

DOCKLET_DATA* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	DOCKLET_DATA* lpData = new DOCKLET_DATA;
	lpData->hwndDesklet = hwndDesklet;
	lpData->hInstanceDll = hInstance;

	lpData->l1 = DeskletLayerAdd(lpData->hwndDesklet,"PicSlider.Buf1");
	lpData->l2 = DeskletLayerAdd(lpData->hwndDesklet,"PicSlider.Buf2");
	lpData->l3 = DeskletLayerAdd(lpData->hwndDesklet,"PicSlider.Overlay");
	
	if(szIni && szIniGroup)
	{
		GetPrivateProfileString(szIniGroup,"OnClick","shell:DriveFolder",lpData->onclick,MAX_PATH,szIni);

		GetPrivateProfileString(szIniGroup,"Mask","",lpData->maskPath,MAX_PATH,szIni);
		GetPrivateProfileString(szIniGroup,"Overlay","",lpData->overlayPath,MAX_PATH,szIni);

		lpData->r.left = GetPrivateProfileInt(szIniGroup,"Left",0,szIni);
		lpData->r.top = GetPrivateProfileInt(szIniGroup,"Top",0,szIni);
		lpData->r.right = GetPrivateProfileInt(szIniGroup,"Right",192,szIni);
		lpData->r.bottom = GetPrivateProfileInt(szIniGroup,"Bottom",192,szIni);

		
		lpData->r2.left = GetPrivateProfileInt(szIniGroup,"OLeft",0,szIni);
		lpData->r2.top = GetPrivateProfileInt(szIniGroup,"OTop",0,szIni);
		lpData->r2.right = GetPrivateProfileInt(szIniGroup,"ORight",192,szIni);
		lpData->r2.bottom = GetPrivateProfileInt(szIniGroup,"OBottom",192,szIni);

		lpData->update = GetPrivateProfileInt(szIniGroup,"UpdateIntervalInMS",10000,szIni);

		char tmp[MAX_PATH];
		GetPrivateProfileString(szIniGroup,"Folder","",tmp,sizeof(tmp)/sizeof(tmp[0]),szIni);
		lpData->folder = tmp;
		lpData->search.SetBaseFolder(lpData->folder);
	}
	else
	{
		char path[MAX_PATH];
		GetDLLPath(lpData->hInstanceDll,path,MAX_PATH);
		strcat(path,"defaults.ini");

		strcpy(lpData->overlayPath,"");

		GetPrivateProfileString("DEFAULTVALUES","OnClick","shell:DriveFolder",lpData->onclick,MAX_PATH,path);

		lpData->r.left = GetPrivateProfileInt("DEFAULTVALUES","Left",0,path);
		lpData->r.top = GetPrivateProfileInt("DEFAULTVALUES","Top",0,path);
		lpData->r.right = GetPrivateProfileInt("DEFAULTVALUES","Right",192,path);
		lpData->r.bottom = GetPrivateProfileInt("DEFAULTVALUES","Bottom",192,path);

		lpData->update = GetPrivateProfileInt("DEFAULTVALUES","UpdateIntervalInMS",10000,path);

		char img[MAX_PATH];
		DeskletGetRelativeFolder(lpData->hwndDesklet,img);
		strcat(img,"icon.png");
		DeskletSetImageFile(lpData->hwndDesklet,img);
	}

	lpData->curFile = lpData->search.NextFile();
	UpdateScreen(lpData);

	SetTimer(lpData->hwndDesklet,TIMER_ID,lpData->update,0);


	return lpData;
}

void __stdcall OnDestroy(DOCKLET_DATA *lpData, HWND hwndDesklet)
{
	KillTimer(lpData->hwndDesklet,TIMER_ID);

	delete lpData;
	lpData = 0;
}


void __stdcall OnSave(DOCKLET_DATA *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	WritePrivateProfileString(szIniGroup,"OnClick",lpData->onclick,szIni);

	WritePrivateProfileString(szIniGroup,"Overlay",lpData->overlayPath,szIni);

	WritePrivateProfileInt(szIniGroup,"Left",lpData->r.left,szIni);
	WritePrivateProfileInt(szIniGroup,"Top",lpData->r.top,szIni);
	WritePrivateProfileInt(szIniGroup,"Bottom",lpData->r.bottom,szIni);
	WritePrivateProfileInt(szIniGroup,"Right",lpData->r.right,szIni);

	WritePrivateProfileInt(szIniGroup,"OLeft",lpData->r2.left,szIni);
	WritePrivateProfileInt(szIniGroup,"OTop",lpData->r2.top,szIni);
	WritePrivateProfileInt(szIniGroup,"OBottom",lpData->r2.bottom,szIni);
	WritePrivateProfileInt(szIniGroup,"ORight",lpData->r2.right,szIni);

	WritePrivateProfileInt(szIniGroup,"UpdateIntervalInMS",lpData->update,szIni);

	WritePrivateProfileString(szIniGroup,"Mask",lpData->maskPath,szIni);

	WritePrivateProfileString(szIniGroup,"Folder",const_cast<char*>(lpData->folder.c_str()),szIni);
}

void __stdcall OnProcessMessage(DOCKLET_DATA* lpData,HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg==WM_TIMER)
	{
		if(wParam == TIMER_ID)
		{
			lpData->useBuf1 = !lpData->useBuf1;

			int id;
			if(lpData->useBuf1)
				id = DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf1",0);
			else
				id = DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf2",0);

			DeskletLayerSetAlpha(lpData->hwndDesklet,id,false,0);

			DeskletLayerMoveTop(lpData->hwndDesklet,id,false);
			DeskletLayerMoveTop(lpData->hwndDesklet,DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Overlay",0),false);

			lpData->curFile = lpData->search.NextFile();
			if(lpData->curFile == "")
			{
				lpData->search.Reset();
				lpData->curFile = lpData->search.NextFile();
			}
			UpdateScreen(lpData);
			
			SetTimer(lpData->hwndDesklet,BLEND_ID,50,0);
		}
		else if(wParam == BLEND_ID)
		{
			int id;
			int id2;
			if(lpData->useBuf1)
			{
				id = DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf1",0);
				id2= DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf2",0);
			}
			else
			{
				id = DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf2",0);
				id2= DeskletLayerGetLayer(lpData->hwndDesklet,"PicSlider.Buf1",0);
			}
			DeskletLayerSetAlpha(lpData->hwndDesklet,id,false,(int)(lpData->count * 25));
			DeskletLayerSetAlpha(lpData->hwndDesklet,id2,true,255-(int)(lpData->count * 25));

			if(lpData->count == 10)
			{
				KillTimer(lpData->hwndDesklet,BLEND_ID);
				lpData->count = 0;
				DeskletLayerSetImage(lpData->hwndDesklet,id2,false,0,true);
			}
			else
			{
				lpData->count++;
			}
		}
	}
}

BOOL __stdcall OnLeftButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	ShellExecute(0,"open",lpData->onclick,0,0,SW_SHOW);
	return TRUE;
}

int __stdcall SettingsConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

			if(!lpData)return TRUE;
			
		
			SetDlgItemInt(hDlg,IDC_LEFT,lpData->r.left,FALSE);
			SetDlgItemInt(hDlg,IDC_TOP,lpData->r.top,FALSE);
			SetDlgItemInt(hDlg,IDC_RIGHT,lpData->r.right,FALSE);
			SetDlgItemInt(hDlg,IDC_BOTTOM,lpData->r.bottom,FALSE);
			SetDlgItemInt(hDlg,IDC_UPDATE,lpData->update/1000,FALSE);

			SetDlgItemText(hDlg,IDC_FOLDER,lpData->folder.c_str() );

			SendDlgItemMessage(hDlg,IDC_MASKS,CB_ADDSTRING,0,(LPARAM)"");

			char path[MAX_PATH];
			GetDLLPath(lpData->hInstanceDll,path,MAX_PATH);
			strcat(path,"masks\\");
			strcat(path,"*.png");
			WIN32_FIND_DATA data = {0};
			HANDLE h = FindFirstFile(path,&data);
			while ( h)
			{
				SendDlgItemMessage(hDlg,IDC_MASKS,CB_ADDSTRING,0,(LPARAM)data.cFileName);

				if(!FindNextFile(h,&data))
				{
					FindClose(h);
					h = 0;
				}
			}
			SendDlgItemMessage(hDlg,IDC_MASKS,CB_SELECTSTRING,0,(LPARAM)lpData->maskPath);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDBROWSE:
				{
					char displayName[MAX_PATH];

					BROWSEINFO b = {0};
					b.hwndOwner = hDlg;
					b.pszDisplayName = displayName;
					b.lpszTitle = "Select Folder";
					LPITEMIDLIST pidl = SHBrowseForFolder(&b);
					if(pidl!=0)
					{
						char path[MAX_PATH];
						SHGetPathFromIDList(pidl,path);
						if(path[strlen(path)-1]!='\\' && path[strlen(path)-1]!='/')
							strcat(path,"\\");

						SetDlgItemText(hDlg,IDC_FOLDER,path);

						IMalloc * imalloc = 0;
						if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
						{
						imalloc->Free ( pidl );
						imalloc->Release ( );
				        }

					}


				}
				break;

				case IDC_FOLDER: case IDC_LEFT: case IDC_TOP: case IDC_RIGHT: case IDC_BOTTOM: case IDC_UPDATE:
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDC_MASKS:
					if(HIWORD(wParam)==CBN_SELCHANGE)
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
					DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						char tmp[MAX_PATH];
						GetDlgItemText(hDlg,IDC_FOLDER,tmp,sizeof(tmp)/sizeof(tmp[0]));
						if(std::string(tmp) != lpData->folder)
						{
							lpData->folder = std::string(tmp);
							lpData->search.SetBaseFolder(lpData->folder);
						}

						lpData->r.left = GetDlgItemInt(hDlg,IDC_LEFT,0,FALSE);
						lpData->r.right = GetDlgItemInt(hDlg,IDC_RIGHT,0,FALSE);
						lpData->r.top = GetDlgItemInt(hDlg,IDC_TOP,0,FALSE);
						lpData->r.bottom = GetDlgItemInt(hDlg,IDC_BOTTOM,0,FALSE);

						char path[MAX_PATH];
						GetDlgItemText(hDlg,IDC_MASKS,path,MAX_PATH);
						if(strcmp(path,lpData->maskPath)!=0)
						{
							lpData->changeFlag = true;
							delete lpData->mask;
							lpData->mask = 0;
							strcpy(lpData->maskPath,path);
						}

						lpData->update  = GetDlgItemInt(hDlg,IDC_UPDATE,0,FALSE) * 1000;

						KillTimer(lpData->hwndDesklet,TIMER_ID);

						UpdateScreen(lpData);

						SetTimer(lpData->hwndDesklet,TIMER_ID,lpData->update,0);
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


int __stdcall OverlayConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

			if(!lpData)return TRUE;
			
		
			SetDlgItemInt(hDlg,IDC_LEFT,lpData->r2.left,FALSE);
			SetDlgItemInt(hDlg,IDC_TOP,lpData->r2.top,FALSE);
			SetDlgItemInt(hDlg,IDC_RIGHT,lpData->r2.right,FALSE);
			SetDlgItemInt(hDlg,IDC_BOTTOM,lpData->r2.bottom,FALSE);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{

				case IDBROWSEOVERLAY:
				{
					//MessageBox(hDlg,"1",0,0);
					DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
					//MessageBox(hDlg,"2",0,0);
					if(lpData && DeskletBrowseForImage(hDlg,lpData->overlayPath))
					{
						//MessageBox(hDlg,"3",0,0);
						lpData->changeFlag = true;
						UpdateScreen(lpData);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				}
				break;

				case IDREMOVEOVERLAY:
				{
					DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						lpData->changeFlag = true;
						strcpy(lpData->overlayPath,"");
						UpdateScreen(lpData);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				}
				break;

					case IDC_LEFT: case IDC_TOP: case IDC_RIGHT: case IDC_BOTTOM: case IDC_UPDATE:
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
					DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						lpData->changeFlag = true;
						lpData->r2.left = GetDlgItemInt(hDlg,IDC_LEFT,0,FALSE);
						lpData->r2.right = GetDlgItemInt(hDlg,IDC_RIGHT,0,FALSE);
						lpData->r2.top = GetDlgItemInt(hDlg,IDC_TOP,0,FALSE);
						lpData->r2.bottom = GetDlgItemInt(hDlg,IDC_BOTTOM,0,FALSE);

						UpdateScreen(lpData);
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


int __stdcall MaskConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

			if(!lpData)return TRUE;
			
			SendDlgItemMessage(hDlg,IDC_MASKS,CB_ADDSTRING,0,(LPARAM)"");

			char path[MAX_PATH];
			GetDLLPath(lpData->hInstanceDll,path,MAX_PATH);
			strcat(path,"masks\\");
			strcat(path,"*.png");
			WIN32_FIND_DATA data = {0};
			HANDLE h = FindFirstFile(path,&data);
			while ( h)
			{
				SendDlgItemMessage(hDlg,IDC_MASKS,CB_ADDSTRING,0,(LPARAM)data.cFileName);

				if(!FindNextFile(h,&data))
				{
					FindClose(h);
					h = 0;
				}
			}
			SendDlgItemMessage(hDlg,IDC_MASKS,CB_SELECTSTRING,0,(LPARAM)lpData->maskPath);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_MASKS:
					if(HIWORD(wParam)==CBN_SELCHANGE)
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
					DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						char path[MAX_PATH];
						GetDlgItemText(hDlg,IDC_MASKS,path,MAX_PATH);
						if(strcmp(path,lpData->maskPath)!=0)
						{
							lpData->changeFlag = true;
							delete lpData->mask;
							lpData->mask = 0;
							strcpy(lpData->maskPath,path);
						}
						UpdateScreen(lpData);
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

INT __stdcall OnAddPropertySheets(DOCKLET_DATA* lpData, int numPresent)
{
	// first, set all the members for this PropertySheetPage.
	PROPSHEETPAGE psp = {0};
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Slide";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_SETTINGS);
	psp.pfnDlgProc = SettingsConfigProc;
	psp.lParam = (LPARAM)lpData;

	int res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Overlay";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_OVERLAY);
	psp.pfnDlgProc = OverlayConfigProc;
	psp.lParam = (LPARAM)lpData;


	res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Mask";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_MASK);
	psp.pfnDlgProc = MaskConfigProc;
	psp.lParam = (LPARAM)lpData;


	res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	return res;
}
