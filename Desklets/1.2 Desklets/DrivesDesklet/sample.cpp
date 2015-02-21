// Sample Desklet
// Showes a drive image depending on the status of the drive

#include <windows.h>					// for common windows stuff
#include "../DeskletSDK.h"				// for common desklet stuff

#include <Gdiplus.h>					// for all GDI+ stuff
using namespace Gdiplus;				// use the namespace Gdiplus by default
#pragma comment(lib, "gdiplus.lib")		// link with gdiplus.lib
#include <shlobj.h>

#include "resource.h"

//--------------------------------------------------------------------------------------------
void GetFileName(const char* filepath, char* buffer, int buffersize)
{
	// pre:  filepath and buffer are not 0, buffersize is the maximum size of chars copied
	// post: the filename in filepath is stored in buffer, this means that from filepath
	//			c:\blabla\yada.png buffer is filled with yada.png

	if(!filepath || !buffer)return;

	int i=strlen(filepath)-1;
	while( i >= 0 && buffersize-- >0 && filepath[i]!='\\' && filepath[i]!='/')i--;
	strcpy(buffer,filepath+i+1);
}
//--------------------------------------------------------------------------------------------
char* lowercase(char* str)
{
	// pre:  str is not 0
	// post: str has been converted to an all-lowercase-characters string and str
	//			has been returned again

	if(!str)return 0;

	for(unsigned int i=0;i<strlen(str);i++)
		str[i] = tolower(str[i]);

	return str;
}
//--------------------------------------------------------------------------------------------
bool FileExists(const char* file)
{
	// pre:  file is not 0 and file should be an absolute path
	// post: true has been returned if the file named by file does exist, otherwise false
	//			has been returned

	if(!file)return false;

	// get the filename of the file and store it in tmp
	char tmp[MAX_PATH];
	GetFileName(file,tmp,sizeof(tmp));

	// get WIN32_FIND_DATA for the file pointed to file
	WIN32_FIND_DATA d;
	ZeroMemory(&d,sizeof(d));
	HANDLE h=FindFirstFile(file,&d);

	// don't forget to close the handle, otherwise we have a mem-leak
	FindClose(h);

	// if the filename retrieved from file and the filename retrieved
	// by getting the WIN32_FIND_DATA are the same when compared (not case-sensitive)
	// then this file DOES exist, otherwise it DOESN'T exist
	return strcmp(lowercase(tmp),lowercase(d.cFileName))==0;
}
//--------------------------------------------------------------------------------------------
bool strcmp_nocase(const char* str1, const char* str2)
{
	// pre:  str1 and str2 are not 0
	// post: if str1 and str2 are the same when compared not case-sensitive, then
	//		  true is returned, otherwise false is returned
	
	if(!str1 || !str2)return false;

	int i=0;
	while(str1[i]!=0 && str2[i]!=0 && tolower(str1[i]) == tolower(str2[i]))i++;
	return str1[i]==0 && str2[i]==0;
}
//--------------------------------------------------------------------------------------------

bool isImageFile(char* file)
{
	if(!file || strlen(file)<4)return false;

	int l = strlen(file);
	char ext[5];
	strcpy(ext,&file[l-4]);
	if(strcmp_nocase(ext,".jpg"))
		return true;
	else if(strcmp_nocase(ext,".png"))
		return true;
	else if(strcmp_nocase(ext,".tif"))
		return true;
	else if(strcmp_nocase(ext,".bmp"))
		return true;
	else
		return false;
}

#define IMAGE_TIMER 3430

class DOCKLET_DATA
{
	private:
		HINSTANCE	hInstanceDll;
		HWND		hwndDesklet;

	public:
		DOCKLET_DATA(HINSTANCE hInst,HWND hwnd) 
			: hInstanceDll(hInst), hwndDesklet(hwnd), winState(0), AcceptDrop(false)
		{
			strcpy(path,"");
			strcpy(params,"");
		}

		~DOCKLET_DATA()
		{
		}

		HINSTANCE	GetInstance()		{return hInstanceDll;};
		HWND		GetWindow()			{return hwndDesklet;};

		void ReadFromIni(char* group, char* ini)
		{
			GetPrivateProfileString(group,"Path",path,path,sizeof(path)/sizeof(path[0]),ini);
			GetPrivateProfileString(group,"Params",params,params,sizeof(params)/sizeof(params[0]),ini);
			if(isImageFile(path))
			{
				DeskletSetImageFile(hwndDesklet,path);
				HANDLE hFile = CreateFile(path,GENERIC_READ,FILE_SHARE_WRITE	 | FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
				if(hFile)
				{
					GetFileTime(hFile,0,0,&lastWrite);
					CloseHandle(hFile);
				}
			}
		}

		void WriteToIni(char* group, char* ini)
		{
			WritePrivateProfileString(group,"Path",path,ini);
			WritePrivateProfileString(group,"Params",params,ini);
		}

		char path[MAX_PATH];
		char params[MAX_PATH];
		int  winState;

		bool AcceptDrop;

		FILETIME lastWrite;

};


DOCKLET_DATA *CALLBACK OnCreate(HWND hwndDesklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	DOCKLET_DATA* lpData = new DOCKLET_DATA(hInstance,hwndDesklet);

	FORMATETC f;
	f.cfFormat = CF_HDROP;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	DeskletAddDragDropFormat(hwndDesklet,&f);

	DeskletActivateExtendedDragDrop(hwndDesklet);

	//DeskletSetStatic(hwndDesklet,true);
	UINT id = DeskletLayerAdd(hwndDesklet,"Shortcut.Bg");
	//DeskletLayerSetAlpha(hwndDesklet,id,false,255);
	DeskletLayerSetIconState(hwndDesklet,id,false,false);
	DeskletLayerSetVisibility(hwndDesklet,id,false,false);	

	char path[MAX_PATH];
	DeskletGetRelativeFolder(hwndDesklet,path);
	strcat(path,"bg.png");
	DeskletLayerSetImageFile(hwndDesklet,id,true,path);

	DeskletLayerMoveBottom(hwndDesklet,id,false);


	if(szIni && szIniGroup)
		lpData->ReadFromIni(szIniGroup,szIni);
	else
	{
		DeskletSetImageFile(hwndDesklet,"");
	}

	SetTimer(hwndDesklet,IMAGE_TIMER,10000,0);

	return lpData;
}

void CALLBACK OnDestroy(DOCKLET_DATA *lpData, HWND hwndDesklet)
{
	delete lpData;
	lpData = 0;
}
void CALLBACK OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName, "DeskletBin Desklet");
	strcpy(szAuthor, "Andreas Verhoeven");
	(*iVersion) = 100;
	strcpy(szNotes,"Ditch your desklets.");
}

BOOL CALLBACK OnDragEnter(DOCKLET_DATA* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	/*if(!IsDataObjectHDROP(obj))
	{
		lpData->AcceptDrop = false;
		*effect = DROPEFFECT_NONE;
	}
	else
	{
		lpData->AcceptDrop = true;
	}*/

	return TRUE;
}

BOOL CALLBACK OnDragOver(DOCKLET_DATA* lpData,DWORD keys, POINTL pnt, DWORD* effect)
{
	/*if(!lpData->AcceptDrop)
	{
		*effect = DROPEFFECT_NONE;
	}
	else
	{	
	}*/
	return TRUE;
}

BOOL CALLBACK OnDrop(DOCKLET_DATA* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	FORMATETC f;
	f.cfFormat = RegisterClipboardFormat("AveDeskDesklet");
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	
   STGMEDIUM medium; 
   HRESULT hr = obj->GetData(&f, &medium); 

   if (!FAILED(hr)) 
    { 
	    HGLOBAL hHwnd = medium.hGlobal;
		HWND* pHwnd = (HWND*) GlobalLock(hHwnd);

		DeskletRemove(*pHwnd,true);

		GlobalUnlock(hHwnd); 
		ReleaseStgMedium(&medium); 

		return TRUE;
    } 
   else 
    {  
	   *effect = DROPEFFECT_NONE;
       return FALSE;
    }

	/*if(IsDataObjectHDROP(obj))
	{
		bool isFolder = lpData->path[strlen(lpData->path)-1] == '\\';


		DropStruct* ds = LockHDROP(obj);

		if(isFolder)
		{
			int num = DragQueryFile(ds->hDrop, -1, 0, 0);

			char* pFrom = new char[num*MAX_PATH+1];
			char* pFromCur = pFrom;
	
			for(int i=0;i<num;i++)
			{
				DragQueryFile(ds->hDrop,i,pFromCur,MAX_PATH);
				pFromCur = pFromCur + DragQueryFile(ds->hDrop,i,0,0) +1;

			}
			*pFromCur = 0;

			SHFILEOPSTRUCT s;
			ZeroMemory(&s,sizeof(s));
			s.fAnyOperationsAborted = false;
			s.hwnd = 0;
			s.wFunc = FO_COPY;
			s.pFrom = pFrom;
			s.pTo = lpData->path;
			SHFileOperation(&s);

			delete[] pFrom;
			
		}
		UnlockHDROP(ds);

		return TRUE;
	}
	else
	{
		*effect = DROPEFFECT_NONE;
		return FALSE;
	}*/
}

void CALLBACK OnSave(DOCKLET_DATA *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	lpData->WriteToIni(szIniGroup,szIni);
}

BOOL CALLBACK OnSelect(DOCKLET_DATA *lpData)
{
	UINT id = DeskletLayerGetLayer(lpData->GetWindow(),"Shortcut.Bg",0);
	DeskletLayerSetVisibility(lpData->GetWindow(),id,true,true);
	return TRUE;
}

BOOL CALLBACK OnDeselect(DOCKLET_DATA *lpData)
{
	UINT id = DeskletLayerGetLayer(lpData->GetWindow(),"Shortcut.Bg",0);
	DeskletLayerSetVisibility(lpData->GetWindow(),id,true,false);	
	return TRUE;
}

void CALLBACK OnProcessMessage(DOCKLET_DATA *lpData, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*if(uMsg == WM_TIMER && wParam == IMAGE_TIMER)
	{
		if(strcmp(lpData->path,"")!=0)
		{
			if(!FileExists(lpData->path))
			{
				DeskletRemove(lpData->GetWindow(),false);
			}
		}
		if(isImageFile(lpData->path))
		{
			HANDLE hFile = CreateFile(lpData->path,GENERIC_READ,FILE_SHARE_WRITE	 | FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
			if(hFile)
			{
				FILETIME ft;
				GetFileTime(hFile,0,0,&ft);
				CloseHandle(hFile);
				if(CompareFileTime(&lpData->lastWrite,&ft)!=0)
				{
					lpData->lastWrite = ft;
					DeskletSetImageFile(lpData->GetWindow(),lpData->path);
					DeskletSetStatic(lpData->GetWindow(),false);
					DeskletSetStatic(lpData->GetWindow(),true);
				}
			}
		}
	}*/
	/*else if(uMsg == WM_SETFOCUS)
	{
		//MessageBox(0,"focus",0,0);
		//DeskletLayerSetAlpha(lpData->GetWindow(),0,true,100);
		UINT id = DeskletLayerGetLayer(lpData->GetWindow(),"Shortcut.Bg",0);
		//DeskletLayerSetAlpha(lpData->GetWindow(),id,true,255);
		DeskletLayerSetVisibility(lpData->GetWindow(),id,true,true);
	}
	else if(uMsg == WM_KILLFOCUS)
	{
		//DeskletLayerSetAlpha(lpData->GetWindow(),0,true,255);
		UINT id = DeskletLayerGetLayer(lpData->GetWindow(),"Shortcut.Bg",0);
		//DeskletLayerSetAlpha(lpData->GetWindow(),id,true,0);
		DeskletLayerSetVisibility(lpData->GetWindow(),id,true,false);
	}*/
}

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
				{
					SetDlgItemText(hDlg,IDC_PATH,lpData->path);

					if(isImageFile(lpData->path))
					{
						DeskletSetImageFile(lpData->GetWindow(),lpData->path);
						DeskletSetStatic(lpData->GetWindow(),false);
						DeskletSetStatic(lpData->GetWindow(),true);
						HANDLE hFile = CreateFile(lpData->path,GENERIC_READ,FILE_SHARE_WRITE	 | FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
						if(hFile)
						{
							GetFileTime(hFile,0,0,&lpData->lastWrite);
							CloseHandle(hFile);
						}
					}
				}
			}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
		break;
	}

	return FALSE;
}

BOOL CALLBACK OnLeftButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	UINT openMode = SW_NORMAL;
	if(lpData->winState == 1)openMode = SW_SHOWMINIMIZED;
	else if(lpData->winState == 2) openMode = SW_SHOWMAXIMIZED;
	ShellExecute(0,"open",lpData->path,lpData->params,lpData->path,openMode);
	return TRUE;
}

VOID CALLBACK OnConfigure(DOCKLET_DATA *lpData)
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
}

BOOL CALLBACK OnRightButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	if(strcmp(lpData->path,"")==0)
	{
		return FALSE;
	}
	else
	{
		HMENU menu = DeskletGetDefaultMenu(lpData->GetWindow(),20000);
	
		int cmd = ShowContextMenu(lpData->GetWindow(),menu,0,lpData->path);
		DestroyMenu(menu);
		if(cmd)DeskletDoDefaultMenuCommand(lpData->GetWindow(),20000,cmd);

		return TRUE;
	}
}

