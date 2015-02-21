// Sample Desklet
// Showes a drive image depending on the status of the drive

#include <windows.h>					// for common windows stuff
#include "../DeskletSDK.h"				// for common desklet stuff

#include <Gdiplus.h>					// for all GDI+ stuff
using namespace Gdiplus;				// use the namespace Gdiplus by default
#pragma comment(lib, "gdiplus.lib")		// link with gdiplus.lib

#include "resource.h"

class DOCKLET_DATA
{
	private:
		HINSTANCE	hInstanceDll;
		HWND		hwndDesklet;

	public:
		DOCKLET_DATA(HINSTANCE hInst,HWND hwnd) :
			hInstanceDll(hInst),
			hwndDesklet(hwnd),
			AcceptDrop(false)
		{
		}

		~DOCKLET_DATA()
		{
		}

		HINSTANCE	GetInstance()		{return hInstanceDll;};
		HWND		GetWindow()			{return hwndDesklet;};

		void ReadFromIni(char* group, char* ini)
		{
		}

		void WriteToIni(char* group, char* ini)
		{
		}

		bool AcceptDrop;

};

void SetSecondsHand(DOCKLET_DATA* lpData,char* path)
{
	Bitmap* bmp = DeskletLoadGDIPlusImage(path);

	Image* img = ((Image*)bmp)->Clone();
	delete bmp;
	bmp = (Bitmap*)img;
	
	// get the seconds layer or add it, if it isn't already added
	bool suc(false);
	unsigned int id = DeskletLayerGetLayer(lpData->GetWindow(),"AveClock.Seconds",&suc);
	if(!suc)
		id = DeskletLayerAdd(lpData->GetWindow(),"AveClock.Seconds");

	DeskletLayerSetImage(lpData->GetWindow(),id,false,bmp,true);
	
	// calculate it transformation
	int width	= bmp->GetWidth();
	int height	= bmp->GetHeight();
	RECT s;
	s.bottom = 64 - height;
	s.top = 64 ;
	s.left = 64 - width/2;
	s.right = 64 + width / 2;
	DeskletLayerSetPlacement(lpData->GetWindow(),id,true,&s,128,128);
}

void SetMinutesHand(DOCKLET_DATA* lpData,char* path)
{
	Bitmap* bmp = DeskletLoadGDIPlusImage(path);

	Image* img = ((Image*)bmp)->Clone();
	delete bmp;
	bmp = (Bitmap*)img;
	
	// get the seconds layer or add it, if it isn't already added
	bool suc(false);
	unsigned int id = DeskletLayerGetLayer(lpData->GetWindow(),"AveClock.Minutes",&suc);
	if(!suc)
		id = DeskletLayerAdd(lpData->GetWindow(),"AveClock.Minutes");

	DeskletLayerSetImage(lpData->GetWindow(),id,false,bmp,true);
	
	// calculate it transformation
	int width	= bmp->GetWidth();
	int height	= bmp->GetHeight();
	RECT s;
	s.bottom = 64 - height;
	s.top = 64 ;
	s.left = 64 - width/2;
	s.right = 64 + width / 2;
	DeskletLayerSetPlacement(lpData->GetWindow(),id,true,&s,128,128);
}

void SetHoursHand(DOCKLET_DATA* lpData,char* path)
{
	Bitmap* bmp = DeskletLoadGDIPlusImage(path);

	Image* img = ((Image*)bmp)->Clone();
	delete bmp;
	bmp = (Bitmap*)img;
	
	// get the seconds layer or add it, if it isn't already added
	bool suc(false);
	unsigned int id = DeskletLayerGetLayer(lpData->GetWindow(),"AveClock.Hours",&suc);
	if(!suc)
		id = DeskletLayerAdd(lpData->GetWindow(),"AveClock.Hours");

	DeskletLayerSetImage(lpData->GetWindow(),id,false,bmp,true);
	
	// calculate it transformation
	int width	= bmp->GetWidth();
	int height	= bmp->GetHeight();
	RECT s;
	s.bottom = 64 - height;
	s.top = 64 ;
	s.left = 64 - width/2;
	s.right = 64 + width / 2;
	DeskletLayerSetPlacement(lpData->GetWindow(),id,true,&s,128,128);
}

void UpdateHours(DOCKLET_DATA* lpData,SYSTEMTIME& t)
{
	UINT id = DeskletLayerGetLayer(lpData->GetWindow(),"AveClock.Hours",0);
	DeskletLayerSetRotation(lpData->GetWindow(),id,false, 30 * (t.wHour % 12));
}

void UpdateMinutes(DOCKLET_DATA* lpData,SYSTEMTIME& t)
{
	UINT id = DeskletLayerGetLayer(lpData->GetWindow(),"AveClock.Minutes",0);
	DeskletLayerSetRotation(lpData->GetWindow(),id,false, 6 * t.wMinute);
}

void UpdateSeconds(DOCKLET_DATA* lpData,SYSTEMTIME& t)
{
	UINT id = DeskletLayerGetLayer(lpData->GetWindow(),"AveClock.Seconds",0);
	DeskletLayerSetRotation(lpData->GetWindow(),id,true,6*t.wSecond); 
}


void UpdateClock(DOCKLET_DATA* lpData)
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	if(t.wMinute == 0)
	{
		UpdateHours(lpData,t);
	}

	if(t.wSecond  == 0)
	{
		UpdateMinutes(lpData,t);
	}

	UpdateSeconds(lpData,t);
}

BOOL CALLBACK OnDragDropDataRequest(DOCKLET_DATA* lpData,FORMATETC* format,STGMEDIUM* medium)
{
	if(format->cfFormat == CF_TEXT)
	{
		char tmp[MAX_PATH];
		GetTimeFormat(0,0,0,"HH:mm:ss dddd dd mmm yyyy",tmp,MAX_PATH);
		char date[MAX_PATH];
		strcpy(date,tmp);
		GetDateFormat(0,0,0,date,tmp,MAX_PATH);


		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,strlen(tmp)+1);
		char* memCpy = (char*)GlobalLock(hMem);

		memcpy((void*)memCpy,(void*)tmp,(strlen(tmp)+1));
		GlobalUnlock(hMem);

		medium->hGlobal = hMem;
		medium->pUnkForRelease = 0;
		medium->tymed = TYMED_HGLOBAL;
		return TRUE;
	}
	return FALSE;
}

DOCKLET_DATA *CALLBACK OnCreate(HWND hwndDesklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	DOCKLET_DATA* lpData = new DOCKLET_DATA(hInstance,hwndDesklet);

	if(!szIni || !szIniGroup)
	{
		char bg[MAX_PATH];
		DeskletGetRelativeFolder(hwndDesklet,bg);
		strcat(bg,"back.png");
		DeskletLayerSetImageFile(hwndDesklet,0,false,bg);
	}

	FORMATETC f;
	f.cfFormat = CF_TEXT;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	DeskletAddDragDropFormat(hwndDesklet,&f);
	
	// add the secondshand
	char path[MAX_PATH];
	DeskletGetRelativeFolder(hwndDesklet,path);
	strcat(path,"seconds.png");
	SetSecondsHand(lpData,path);

	// add the minuteshand
	DeskletGetRelativeFolder(hwndDesklet,path);
	strcat(path,"minutes.png");
	SetMinutesHand(lpData,path);

	// add the hourhand
	DeskletGetRelativeFolder(hwndDesklet,path);
	strcat(path,"hours.png");
	SetHoursHand(lpData,path);

	SYSTEMTIME t;
	GetLocalTime(&t);
	UpdateHours(lpData,t);
	UpdateMinutes(lpData,t);
	UpdateSeconds(lpData,t);


	DeskletActivateExtendedDragDrop(hwndDesklet);

	if(szIni && szIniGroup)
		lpData->ReadFromIni(szIniGroup,szIni);

	SetTimer(hwndDesklet,100,1000,0);

	return lpData;
}

void CALLBACK OnDestroy(DOCKLET_DATA *lpData, HWND hwndDesklet)
{
	delete lpData;
	lpData = 0;
}
void CALLBACK OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName, "AveClock");
	strcpy(szAuthor, "Andreas Verhoeven");
	(*iVersion) = 100;
	strcpy(szNotes,"A very simple clock desklet to test out the layers.");
}

BOOL CALLBACK OnDragEnter(DOCKLET_DATA* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(!IsDataObjectHDROP(obj))
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
	if(IsDataObjectHDROP(obj))
	{
		//bool isFolder = lpData->path[strlen(lpData->path)-1] == '\\';


		DropStruct* ds = LockHDROP(obj);

		//if(isFolder)
		{
			int num = DragQueryFile(ds->hDrop, -1, 0, 0);
	
			char file[MAX_PATH];

			SYSTEMTIME time;
			GetLocalTime(&time);
			FILETIME fileTime;
			SystemTimeToFileTime(&time,&fileTime);

			for(int i=0;i<num;i++)
			{
				DragQueryFile(ds->hDrop,i,file,sizeof(file)/sizeof(file[0]));
				HANDLE hFile = CreateFile(file,GENERIC_READ,FILE_SHARE_WRITE	 | FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
				if(hFile != 0)
				{
					SetFileTime(hFile,0,0,&fileTime);
					CloseHandle(hFile);
				}
				
			}
			
		}
		UnlockHDROP(ds);

		return TRUE;
	}
	else
	{
		*effect = DROPEFFECT_NONE;
		return FALSE;
	}
}

void CALLBACK OnSave(DOCKLET_DATA *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	lpData->WriteToIni(szIniGroup,szIni);
}

void CALLBACK OnProcessMessage(DOCKLET_DATA *lpData, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_TIMER && wParam == 100)
	{
		UpdateClock(lpData);
	}
}

/*
int CALLBACK DialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			SetProp(hDlg,"lpData",(HANDLE)lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
			if(!lpData)return TRUE;
			
			SetDlgItemText(hDlg,IDC_PATH,lpData->path);
			SetDlgItemText(hDlg,IDC_PARAMS,lpData->params);

			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_ADDSTRING,0,(LPARAM)"Normal Window");
			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_ADDSTRING,0,(LPARAM)"Minimized Window");
			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_ADDSTRING,0,(LPARAM)"Maximized Window");
			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_SETCURSEL,lpData->winState,0);

		break;

		case WM_COMMAND:
			if(LOWORD(wParam)==IDOK)
			{
				GetDlgItemText(hDlg,IDC_PATH,lpData->path,sizeof(lpData->path)/sizeof(lpData->path[0]));
				GetDlgItemText(hDlg,IDC_PARAMS,lpData->params,sizeof(lpData->params)/sizeof(lpData->params[0]));
				lpData->winState = SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_GETCURSEL,0,0);
			}
			else if(LOWORD(wParam)==IDBROWSE)
			{
				OPENFILENAME ofn;
			
				ZeroMemory(&ofn,sizeof(ofn));
			
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetParent(hDlg);
				ofn.hInstance = GetModuleHandle(0);
				ofn.lpstrFilter = "Executable files(*.exe;)\0*.exe;\0\0";
				ofn.lpstrFile = lpData->path; 
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrInitialDir = 0;
				ofn.nMaxFileTitle = MAX_PATH;
				if(GetOpenFileName(&ofn))
					SetDlgItemText(hDlg,IDC_PATH,lpData->path);
			}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
		break;
	}

	return FALSE;
}*/

BOOL CALLBACK OnLeftButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	/*UINT openMode = SW_NORMAL;
	if(lpData->winState == 1)openMode = SW_SHOWMINIMIZED;
	else if(lpData->winState == 2) openMode = SW_SHOWMAXIMIZED;
	ShellExecute(0,"open",lpData->path,lpData->params,lpData->path,openMode);*/
	return TRUE;
}

/*VOID CALLBACK OnConfigure(DOCKLET_DATA *lpData)
{
	TabPage t;
	t.dlgProc = DialogProc;
	t.title = "Shortcut";
	t.titleLength = 8;
	t.hInstance = lpData->GetInstance();
	t.resourceName = MAKEINTRESOURCE(IDD_SELECTDRIVE);

	PropertyDialogStruct pds;
	pds.lParam = 0;
	pds.pages = &t;
	pds.pagesCount = 1;

	DeskletDefaultConfigDialog(lpData->GetWindow(),&pds);	
}*/

BOOL CALLBACK OnRightButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	/*ReleaseCapture();
	HMENU menu = CreatePopupMenu();
	AppendMenu(menu,MF_SEPARATOR,0,0);
	AppendMenu(menu,MF_ENABLED | MF_STRING,10001,"Desklet Properties...");

	int cmd = ShowContextMenu(lpData->GetWindow(),menu,0,lpData->path);
	switch(cmd)
	{
		case 10001: OnConfigure(lpData);break;
	}
	return TRUE;*/
	return FALSE;
}

