//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

#include "DeskletSDK.h"

#include <shellapi.h>
#include <shlobj.h>
#include <oleidl.h>

//--------------------------------------------------------------------------------------------
BOOL IsDataObjectHDROP(LPDATAOBJECT pDataObj)
{
	FORMATETC fmtetc; 

    fmtetc.cfFormat = CF_HDROP; 
    fmtetc.ptd      = 0; 
    fmtetc.dwAspect = DVASPECT_CONTENT; 
    fmtetc.lindex   = -1; 
    fmtetc.tymed    = TYMED_HGLOBAL; 

   return (pDataObj->QueryGetData(&fmtetc) == NOERROR);
}
//--------------------------------------------------------------------------------------------
DropStruct* LockHDROP(LPDATAOBJECT pDataObj)
{
	if(!pDataObj)return 0;

    FORMATETC fmtetc; 
    fmtetc.cfFormat = CF_HDROP; 
    fmtetc.ptd = 0; 
    fmtetc.dwAspect = DVASPECT_CONTENT; 
    fmtetc.lindex = -1; 
    fmtetc.tymed = TYMED_HGLOBAL; 

    STGMEDIUM medium; 
    HRESULT hr = pDataObj->GetData(&fmtetc, &medium); 

   if (!FAILED(hr)) 
    { 
        HGLOBAL HFiles = medium.hGlobal; 
        HDROP hDrop = (HDROP)GlobalLock(HFiles); 
		
		DropStruct* s = new DropStruct;
		s->hDrop = hDrop;
		s->medium = medium;
		return s;
    } 
   else 
    {  
       return 0;
    }
}
//--------------------------------------------------------------------------------------------
void UnlockHDROP(DropStruct* ds)
{
	if(!ds)return;

	HGLOBAL HFiles = ds->medium.hGlobal;
    GlobalUnlock(HFiles); 
    ReleaseStgMedium(&ds->medium); 

	delete ds;
}
//--------------------------------------------------------------------------------------------
void GetDLLPath(HMODULE h,char* path, DWORD nSize)
{
	GetModuleFileName(h,path,MAX_PATH);
	for(int i=strlen(path)-1;i>0;--i)
	{
		if(path[i]=='\\' || path[i]=='/')
		{
			path[i+1] = '\0';
			break;
		}
	}
}

void GetDLLPathW(HMODULE h,WCHAR* path, DWORD nSize)
{
	// pre:  h is not 0 and path is not 0, nSize determines the maximum number of
	//		 characters path can hold
	// post: the path (without the filename) where module h is located is stored
	//		  in path

	GetModuleFileNameW(h,path,MAX_PATH);

	for(int i=wcslen(path)-1;i>0;--i)
	{
		if(path[i]=='\\' || path[i]=='/')
		{
			path[i+1] = '\0';
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------
int WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int iValue, LPCTSTR lpFileName)
{
	char szNumber[100];
	strcpy(szNumber, "");
	itoa(iValue, szNumber, 10);
	return WritePrivateProfileString(lpAppName, lpKeyName, szNumber, lpFileName);
}
//--------------------------------------------------------------------------------------------
int ShowContextMenu(HWND owner, HMENU menu, POINT* p, const char* filename)
{
	if(!filename)return 0;

	char file[MAX_PATH] = {0};
	strcpy(file,filename);
	if(file[strlen(file)-1] == '\\')
	{
		file[strlen(file)-1] = '\0';
	}

	OleInitialize(0);

	char filepath[MAX_PATH];



	strcpy(filepath,file);
	for(int i=strlen(file)-1;i>0;--i)
	{
		if(file[i]=='\\' || file[i]=='/')
		{
			filepath[i] = '\0';
			strcpy(file,file+strlen(filepath)+1);
			break;
		}
	}

	if(strlen(filepath) < 3)
	{
		strcat(filepath,"\\");
	}
	if(strstr(file,":")!=0)
	{
		strcpy(file,"");
	}

	LPSHELLFOLDER DesktopFolder = {0};
	SHGetDesktopFolder(&DesktopFolder);
	if(!DesktopFolder)
	{
		return 0;
	}
	wchar_t Path[MAX_PATH] = {0};
	LPITEMIDLIST ParentPidl = {0};
	DWORD dummy = {0};
	MultiByteToWideChar(CP_ACP,0,filepath,-1,Path,MAX_PATH);
	DesktopFolder->ParseDisplayName(owner, 0, Path, &dummy, &ParentPidl, 0);
	LPSHELLFOLDER ParentFolder;
	DesktopFolder->BindToObject(ParentPidl, 0, IID_IShellFolder, (void**)&ParentFolder);
	if(!ParentFolder)
	{
		IMalloc * imalloc = {0};
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( ParentPidl );
			imalloc->Release ( );
		}

		DesktopFolder->Release();
		return 0;
	}

	LPITEMIDLIST Pidl = {0};
	MultiByteToWideChar(CP_ACP,0,file,-1,Path,MAX_PATH);
	ParentFolder->ParseDisplayName(owner, 0, Path, &dummy, &Pidl, 0);


	LPCONTEXTMENU CM = {0};
	if(strcmp(file,"")==0)
	{
		DesktopFolder->Release();
		ParentFolder->Release();

		SHGetDesktopFolder(&DesktopFolder);
		MultiByteToWideChar(CP_ACP,0,"",-1,Path,MAX_PATH);
		DesktopFolder->ParseDisplayName(owner,0,Path,&dummy,&ParentPidl,0);
		DesktopFolder->BindToObject(ParentPidl,0,IID_IShellFolder, (void**)&ParentFolder);

		MultiByteToWideChar(CP_ACP,0,filepath,-1,Path,MAX_PATH);
		ParentFolder->ParseDisplayName(owner,0,Path,&dummy,&Pidl,0);
		ParentFolder->GetUIObjectOf(owner, 1, (LPCITEMIDLIST*)&Pidl,IID_IContextMenu, 0, (void**)&CM);
	}
	else
		ParentFolder->GetUIObjectOf(owner, 1, (LPCITEMIDLIST*)&Pidl,IID_IContextMenu, 0, (void**)&CM);
	if(!CM)
	{
		IMalloc * imalloc = {0};
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( ParentPidl );
			imalloc->Free ( Pidl );
			imalloc->Release ( );
		}
		DesktopFolder->Release();
		ParentFolder->Release();
		return 0;
	}

	if(!menu)menu = CreatePopupMenu();


	DWORD Flags = CMF_EXPLORE;
	CM->QueryContextMenu(menu, 0, 1, 0x7FFF, Flags);

	POINT* pt = p;
	if(!p)
	{
		pt = new POINT;
		GetCursorPos(pt);
	}

	int Cmd = TrackPopupMenu(menu,
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
		pt->x, pt->y, 0, owner, 0);


	DestroyMenu(menu);

	if(!p)
	{
		delete pt;
		pt = 0;
	}

	if (Cmd!=0 && Cmd < 10000)
	{
		CMINVOKECOMMANDINFO CI ={0};

		ZeroMemory(&CI, sizeof(CI));
		CI.cbSize = sizeof(CMINVOKECOMMANDINFO);
		CI.hwnd = owner;
		CI.lpVerb = MAKEINTRESOURCE(Cmd - 1);
		CI.lpParameters = "";
		CI.lpDirectory = "";
		CI.nShow = SW_SHOWNORMAL;
		CM->InvokeCommand(&CI);

		IMalloc * imalloc = {0};
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( ParentPidl );
			imalloc->Free ( Pidl );
			imalloc->Release ( );
		}

		try
		{
			CM->Release();
		}
		catch(...)
		{

		}
		ParentFolder->Release();
		DesktopFolder->Release();

		return 0;
		
	}
	else
	{
		IMalloc * imalloc = {0};
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( ParentPidl );
			imalloc->Free ( Pidl );
			imalloc->Release ( );
		}

		try
		{
			CM->Release();
		}
		catch(...)
		{

		}
		ParentFolder->Release();
		DesktopFolder->Release();

		return Cmd;
	}

	OleUninitialize();
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletIsVisibleDUMMY)(HWND);
HDeskletIsVisibleDUMMY HDeskletIsVisible = (HDeskletIsVisibleDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletIsVisible");

BOOL CALLBACK DeskletIsVisible(HWND hwndDesklet)
{

	if(HDeskletIsVisible)return FALSE;

	return HDeskletIsVisible(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletGetRectDUMMY)(HWND,RECT*);
HDeskletGetRectDUMMY HDeskletGetRect = (HDeskletGetRectDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetRect");

BOOL CALLBACK DeskletGetRect(HWND hwndDesklet,RECT *rcDesklet)
{
	if(!HDeskletGetRect)return FALSE;

	return HDeskletGetRect(hwndDesklet,rcDesklet);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletGetLabelDUMMY)(HWND,char*);
HDeskletGetLabelDUMMY HDeskletGetLabel = (HDeskletGetLabelDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetLabel");

int CALLBACK DeskletGetLabel(HWND hwndDesklet,char* szLabel)
{

	if(!HDeskletGetLabel)return FALSE;

	return HDeskletGetLabel(hwndDesklet,szLabel);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletSetLabelDUMMY)(HWND,const char*);
HDeskletSetLabelDUMMY HDeskletSetLabel = (HDeskletSetLabelDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetLabel");

int CALLBACK DeskletSetLabel(HWND hwndDesklet, const char* szLabel)
{
	if(!HDeskletSetLabel)return FALSE;

	return HDeskletSetLabel(hwndDesklet,szLabel);
}
//--------------------------------------------------------------------------------------------
typedef Bitmap* (CALLBACK *HDeskletLoadGDIPlusImageDUMMY)(const char*);
HDeskletLoadGDIPlusImageDUMMY HDeskletLoadGDIPlusImage = (HDeskletLoadGDIPlusImageDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletLoadGDIPlusImage");

Bitmap* CALLBACK DeskletLoadGDIPlusImage(const char *szImage)
{

	if(!HDeskletLoadGDIPlusImage)return 0;

	return HDeskletLoadGDIPlusImage(szImage);
}
//-----------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetImageDUMMY)(HWND hwndDesklet, Image *lpImageNew,BOOL bAutomaticallyDeleteImage);
HDeskletSetImageDUMMY HDeskletSetImage = (HDeskletSetImageDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetImage");


void CALLBACK DeskletSetImage(HWND hwndDesklet, Image *lpImageNew,BOOL bAutomaticallyDeleteImage)
{
	if(!HDeskletSetImage)return;

	HDeskletSetImage(hwndDesklet,lpImageNew,bAutomaticallyDeleteImage);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetImageFileDUMMY)(HWND hwndDesklet,const char *szImage);
HDeskletSetImageFileDUMMY HDeskletSetImageFile = (HDeskletSetImageFileDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetImageFile");

void CALLBACK DeskletSetImageFile(HWND hwndDesklet,const char *szImage)
{

	if(!HDeskletSetImageFile)return;

	HDeskletSetImageFile(hwndDesklet,szImage);
}
//-----------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetImageOverlayDUMMY)(HWND hwndDesklet, Image *lpImageOverlayNew,BOOL bAutomaticallyDeleteImageOverlay);
HDeskletSetImageOverlayDUMMY HDeskletSetImageOverlay = (HDeskletSetImageOverlayDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetImageOverlay");

void CALLBACK DeskletSetImageOverlay(HWND hwndDesklet, Image *lpImageOverlayNew,BOOL bAutomaticallyDeleteImageOverlay)
{
	if(!HDeskletSetImageOverlay)return;

	HDeskletSetImageOverlay(hwndDesklet,lpImageOverlayNew,bAutomaticallyDeleteImageOverlay);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetImageOverlayFileDUMMY)(HWND hwndDesklet,const char *szImageOverlay);
HDeskletSetImageOverlayFileDUMMY HDeskletSetImageOverlayFile = (HDeskletSetImageOverlayFileDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetImageOverlayFile");

void CALLBACK DeskletSetImageOverlayFile(HWND hwndDesklet,const char *szImageOverlay)
{
	if(!HDeskletSetImageOverlayFile)return;

	HDeskletSetImageOverlayFile(hwndDesklet,szImageOverlay);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletBrowseForImageDUMMY)(HWND,char*);
HDeskletBrowseForImageDUMMY HDeskletBrowseForImage = (HDeskletBrowseForImageDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletBrowseForImage");

BOOL CALLBACK DeskletBrowseForImage(HWND hwndParent,char *szImage)
{

	if(!HDeskletBrowseForImage)return FALSE;

	return HDeskletBrowseForImage(hwndParent,szImage);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletDoAttentionAnimationDUMMY)(HWND,INT, UINT);
HDeskletDoAttentionAnimationDUMMY HDeskletDoAttentionAnimation = (HDeskletDoAttentionAnimationDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletDoAttentionAnimation");

void CALLBACK DeskletDoAttentionAnimation(HWND hwndDesklet, INT iAnimationType, UINT speed)
{
	if(!HDeskletDoAttentionAnimation)return;

	HDeskletDoAttentionAnimation(hwndDesklet,iAnimationType, speed);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletGetRelativeFolderDUMMY)(HWND,char*);
HDeskletGetRelativeFolderDUMMY HDeskletGetRelativeFolder = (HDeskletGetRelativeFolderDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetRelativeFolder");

void CALLBACK DeskletGetRelativeFolder(HWND hwndDesklet, char *szFolder)
{
	if(!HDeskletGetRelativeFolder)return;

	HDeskletGetRelativeFolder(hwndDesklet,szFolder);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletGetRelativeFolderDUMMYW)(HWND,WCHAR*);
HDeskletGetRelativeFolderDUMMYW HDeskletGetRelativeFolderW = (HDeskletGetRelativeFolderDUMMYW)GetProcAddress(GetModuleHandle(0),"DeskletGetRelativeFolderW");

void CALLBACK DeskletGetRelativeFolderW(HWND hwndDesklet, WCHAR *szFolder)
{
	if(!HDeskletGetRelativeFolderW)return;

	HDeskletGetRelativeFolderW(hwndDesklet,szFolder);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletGetRootFolderDUMMY)(HWND,char*);
HDeskletGetRootFolderDUMMY HDeskletGetRootFolder = (HDeskletGetRootFolderDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetRootFolder");

void CALLBACK DeskletGetRootFolder(HWND hwndDesklet,char *szFolder)
{
	if(!HDeskletGetRootFolder)return;

	HDeskletGetRootFolder(hwndDesklet,szFolder);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletGetRootFolderDUMMYW)(HWND,WCHAR*);
HDeskletGetRootFolderDUMMYW HDeskletGetRootFolderW = (HDeskletGetRootFolderDUMMYW)GetProcAddress(GetModuleHandle(0),"DeskletGetRootFolderW");

void CALLBACK DeskletGetRootFolderW(HWND hwndDesklet,WCHAR *szFolder)
{
	if(!HDeskletGetRootFolderW)return;

	HDeskletGetRootFolderW(hwndDesklet,szFolder);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletDefaultConfigDialogDUMMY)(HWND,PropertyDialogStruct*);
HDeskletDefaultConfigDialogDUMMY HDeskletDefaultConfigDialog = (HDeskletDefaultConfigDialogDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletDefaultConfigDialog");

void CALLBACK DeskletDefaultConfigDialog(HWND hwndDesklet,PropertyDialogStruct* pds)
{

	if(!HDeskletDefaultConfigDialog)return;

	HDeskletDefaultConfigDialog(hwndDesklet,pds);
}
//--------------------------------------------------------------------------------------------
// EXTENDED DESKLET API
typedef BOOL (CALLBACK *HDeskletRemoveDUMMY)(HWND,BOOL);
HDeskletRemoveDUMMY HDeskletRemove = (HDeskletRemoveDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletRemove");

BOOL CALLBACK DeskletRemove(HWND hwndDesklet, BOOL Ask)
{

	if(!HDeskletRemove)return FALSE;

	return HDeskletRemove(hwndDesklet,Ask);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletHideDUMMY)(HWND);
HDeskletHideDUMMY HDeskletHide = (HDeskletHideDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletHide");

void CALLBACK DeskletHide(HWND hwndDesklet)
{

	if(!HDeskletHide)return;

	HDeskletHide(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletShowDUMMY)(HWND);
HDeskletShowDUMMY HDeskletShow = (HDeskletShowDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletShow");

void CALLBACK DeskletShow(HWND hwndDesklet)
{

	if(!HDeskletShow)return;

	HDeskletShow(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetPositionDUMMY)(HWND,POINT*);
HDeskletSetPositionDUMMY HDeskletSetPosition = (HDeskletSetPositionDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetPosition");

void CALLBACK DeskletSetPosition(HWND hwndDesklet,POINT *p)
{
	if(!HDeskletSetPosition)return;

	HDeskletSetPosition(hwndDesklet,p);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetSizeDUMMY)(HWND,SIZE*);
HDeskletSetSizeDUMMY HDeskletSetSize = (HDeskletSetSizeDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetSize");

void CALLBACK DeskletSetSize(HWND hwndDesklet,SIZE *s)
{

	if(!HDeskletSetSize)return;

	HDeskletSetSize(hwndDesklet,s);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetAlphaDUMMY)(HWND,BYTE);
HDeskletSetAlphaDUMMY HDeskletSetAlpha = (HDeskletSetAlphaDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetAlpha");

void CALLBACK DeskletSetAlpha(HWND hwndDesklet,BYTE alpha)
{

	if(!HDeskletSetAlpha)return;

	HDeskletSetAlpha(hwndDesklet,alpha);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetRotationDUMMY)(HWND,INT);
HDeskletSetRotationDUMMY HDeskletSetRotation = (HDeskletSetRotationDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetRotation");

void CALLBACK DeskletSetRotation(HWND hwndDesklet,INT rotation)
{
	if(!HDeskletSetRotation)return;

	HDeskletSetRotation(hwndDesklet,rotation);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletGetPositionDUMMY)(HWND,POINT*);
HDeskletGetPositionDUMMY HDeskletGetPosition = (HDeskletGetPositionDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetPosition");

void CALLBACK DeskletGetPosition(HWND hwndDesklet,POINT *p)
{
	if(!HDeskletGetPosition)return;

	HDeskletGetPosition(hwndDesklet,p);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletGetSizeDUMMY)(HWND,SIZE*);
HDeskletGetSizeDUMMY HDeskletGetSize = (HDeskletGetSizeDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetSize");

void CALLBACK DeskletGetSize(HWND hwndDesklet,SIZE *s)
{

	if(!HDeskletGetSize)return;

	HDeskletGetSize(hwndDesklet,s);
}
//--------------------------------------------------------------------------------------------
typedef BYTE (CALLBACK *HDeskletGetAlphaDUMMY)(HWND);
HDeskletGetAlphaDUMMY HDeskletGetAlpha = (HDeskletGetAlphaDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetAlpha");

BYTE CALLBACK DeskletGetAlpha(HWND hwndDesklet)
{ 

	if(!HDeskletGetAlpha)return 0;

	return HDeskletGetAlpha(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef INT (CALLBACK *HDeskletGetRotationDUMMY)(HWND);
HDeskletGetRotationDUMMY HDeskletGetRotation = (HDeskletGetRotationDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetRotation"); 

INT CALLBACK DeskletGetRotation(HWND hwndDesklet)
{

	if(!HDeskletGetRotation)return 0;

	return HDeskletGetRotation(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef HWND (CALLBACK *HDeskletLoadDUMMY)(const char*,const char*,const char*,void**,BOOL);
HDeskletLoadDUMMY HDeskletLoad = (HDeskletLoadDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLoad");

HWND CALLBACK DeskletLoad(const char* filename,const char* ini, const char* iniGroup,void** p,BOOL makeVisible)
{
	
	if(!HDeskletLoad)return 0;

	return HDeskletLoad(filename,ini,iniGroup,p,makeVisible);
}
//--------------------------------------------------------------------------------------------
typedef UINT (CALLBACK *HDeskletGetParentOKDUMMY)();
HDeskletGetParentOKDUMMY HDeskletGetParentOK = (HDeskletGetParentOKDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetParentOK");

UINT CALLBACK DeskletGetParentOK()
{
	if(!HDeskletGetParentOK)return 0;

	return HDeskletGetParentOK();
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletManagerDUMMY)(UINT);
HDeskletManagerDUMMY HDeskletManager = (HDeskletManagerDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletManager");


void CALLBACK DeskletManager(UINT action)
{
	if(!HDeskletManager)return;

	HDeskletManager(action);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetStaticDUMMY)(HWND,BOOL);
HDeskletSetStaticDUMMY HDeskletSetStatic = (HDeskletSetStaticDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletSetStatic");

void CALLBACK DeskletSetStatic(HWND hwndDesklet, BOOL staticValue)
{

	if(!HDeskletSetStatic)return;

	HDeskletSetStatic(hwndDesklet,staticValue);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletActivateExtendedDragDropDUMMY)(HWND);
HDeskletActivateExtendedDragDropDUMMY HDeskletActivateExtendedDragDrop = (HDeskletActivateExtendedDragDropDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletActivateExtendedDragDrop");

void CALLBACK DeskletActivateExtendedDragDrop(HWND hwndDesklet)
{
	if(!HDeskletActivateExtendedDragDrop)return;

	HDeskletActivateExtendedDragDrop(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletDeactivateExtendedDragDropDUMMY)(HWND);
HDeskletDeactivateExtendedDragDropDUMMY HDeskletDeactivateExtendedDragDrop = (HDeskletDeactivateExtendedDragDropDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletDeactivateExtendedDragDrop");

void CALLBACK DeskletDeactivateExtendedDragDrop(HWND hwndDesklet)
{
	if(!HDeskletDeactivateExtendedDragDrop)return;

	HDeskletDeactivateExtendedDragDrop(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetPlacementDUMMY)(HWND,unsigned int,BOOL, BOOL, RECT*, int, int);
HDeskletLayerSetPlacementDUMMY HDeskletLayerSetPlacement = (HDeskletLayerSetPlacementDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetPlacement");

void CALLBACK DeskletLayerSetPlacement(HWND hwndDesklet, unsigned int id,BOOL redraw, BOOL usePlacement, RECT* s, int xScale, int yScale)
{

	if(!HDeskletLayerSetPlacement)return;

	HDeskletLayerSetPlacement(hwndDesklet,id,redraw,usePlacement,s,xScale,yScale);
}
//--------------------------------------------------------------------------------------------
typedef unsigned int (CALLBACK *HDeskletLayerGetLayerDUMMY)(HWND,const char*, BOOL*);
HDeskletLayerGetLayerDUMMY HDeskletLayerGetLayer = (HDeskletLayerGetLayerDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerGetLayer");

unsigned int CALLBACK DeskletLayerGetLayer(HWND hwndDesklet, const char* name, BOOL* success)
{
	if(!HDeskletLayerGetLayer)return 0;

	return HDeskletLayerGetLayer(hwndDesklet,name,success);
}
//--------------------------------------------------------------------------------------------
typedef unsigned int (CALLBACK *HDeskletLayerCountDUMMY)(HWND);
HDeskletLayerCountDUMMY HDeskletLayerCount = (HDeskletLayerCountDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerCount");

unsigned int CALLBACK DeskletLayerCount(HWND hwndDesklet)
{
	if(!HDeskletLayerCount)return 0;

	return HDeskletLayerCount(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef unsigned int (CALLBACK *HDeskletLayerAddDUMMY)(HWND,const char*);
HDeskletLayerAddDUMMY HDeskletLayerAdd = (HDeskletLayerAddDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerAdd");

unsigned int CALLBACK DeskletLayerAdd(HWND hwndDesklet, const char* name)
{
	if(!HDeskletLayerAdd)return 0;

	return HDeskletLayerAdd(hwndDesklet,name);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerRemoveDUMMY)(HWND,unsigned int, BOOL);
HDeskletLayerRemoveDUMMY HDeskletLayerRemove = (HDeskletLayerRemoveDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerRemove");

void  CALLBACK DeskletLayerRemove(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	if(!HDeskletLayerRemove)return;

	HDeskletLayerRemove(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMergeAllDUMMY)(HWND,BOOL);
HDeskletLayerMergeAllDUMMY HDeskletLayerMergeAll = (HDeskletLayerMergeAllDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMergeAll");

void  CALLBACK DeskletLayerMergeAll(HWND hwndDesklet, BOOL redraw)
{
	if(!HDeskletLayerMergeAll)return;

	HDeskletLayerMergeAll(hwndDesklet,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMergeVisibleDUMMY)(HWND, BOOL);
HDeskletLayerMergeVisibleDUMMY HDeskletLayerMergeVisible = (HDeskletLayerMergeVisibleDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMergeVisible");

void  CALLBACK DeskletLayerMergeVisible(HWND hwndDesklet,  BOOL redraw)
{
	if(!HDeskletLayerMergeVisible)return;

	HDeskletLayerMergeVisible(hwndDesklet,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveDownDUMMY)(HWND,unsigned int, BOOL);
HDeskletLayerMoveDownDUMMY HDeskletLayerMoveDown = (HDeskletLayerMoveDownDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveDown");

void  CALLBACK DeskletLayerMoveDown(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	if(!HDeskletLayerMoveDown)return;

	HDeskletLayerMoveDown(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveTopDUMMY)(HWND,unsigned int, BOOL);
HDeskletLayerMoveTopDUMMY HDeskletLayerMoveTop = (HDeskletLayerMoveTopDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveTop");

void  CALLBACK DeskletLayerMoveTop(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	if(!HDeskletLayerMoveTop)return;

	HDeskletLayerMoveTop(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveBottomDUMMY)(HWND,unsigned int, BOOL);
HDeskletLayerMoveBottomDUMMY HDeskletLayerMoveBottom = (HDeskletLayerMoveBottomDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveBottom");

void  CALLBACK DeskletLayerMoveBottom(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	if(!HDeskletLayerMoveBottom)return;

	HDeskletLayerMoveBottom(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveUpDUMMY)(HWND,unsigned int, BOOL);
HDeskletLayerMoveUpDUMMY HDeskletLayerMoveUp = (HDeskletLayerMoveUpDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveUp");

void  CALLBACK DeskletLayerMoveUp(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	if(!HDeskletLayerMoveUp)return;

	HDeskletLayerMoveUp(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerResetDUMMY)(HWND,unsigned int, BOOL);
HDeskletLayerResetDUMMY HDeskletLayerReset = (HDeskletLayerResetDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerReset");

void  CALLBACK DeskletLayerReset(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	if(!HDeskletLayerReset)return;

	HDeskletLayerReset(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetAlphaDUMMY)(HWND,unsigned int, BOOL, BYTE);
HDeskletLayerSetAlphaDUMMY HDeskletLayerSetAlpha = (HDeskletLayerSetAlphaDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetAlpha");

void  CALLBACK DeskletLayerSetAlpha(HWND hwndDesklet, unsigned int id, BOOL redraw, BYTE alpha)
{
	if(!HDeskletLayerSetAlpha)return;

	HDeskletLayerSetAlpha(hwndDesklet,id,redraw, alpha);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetColorMatrixDUMMY)(HWND,unsigned int, BOOL, ColorMatrix*);
HDeskletLayerSetColorMatrixDUMMY HDeskletLayerSetColorMatrix = (HDeskletLayerSetColorMatrixDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetColorMatrix");

void  CALLBACK DeskletLayerSetColorMatrix(HWND hwndDesklet, unsigned int id, BOOL redraw, ColorMatrix* matrix)
{
	if(!HDeskletLayerSetColorMatrix)return;

	HDeskletLayerSetColorMatrix(hwndDesklet,id,redraw, matrix);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetHueDUMMY)(HWND,unsigned int, BOOL, int);
HDeskletLayerSetHueDUMMY HDeskletLayerSetHue = (HDeskletLayerSetHueDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetHue");

void  CALLBACK DeskletLayerSetHue(HWND hwndDesklet, unsigned int id, BOOL redraw, int hue)
{

	if(!HDeskletLayerSetHue)
	{
		MessageBox(0,"error",0,0);
		return;
	}

	HDeskletLayerSetHue(hwndDesklet,id,redraw, hue);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetImageDUMMY)(HWND,unsigned int, BOOL, Gdiplus::Image*,BOOL);
HDeskletLayerSetImageDUMMY HDeskletLayerSetImage = (HDeskletLayerSetImageDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetImage");

void  CALLBACK DeskletLayerSetImage(HWND hwndDesklet, unsigned int id, BOOL redraw, Gdiplus::Image* img, BOOL AutoDel)
{
	if(!HDeskletLayerSetImage)return;

	HDeskletLayerSetImage(hwndDesklet,id,redraw,img,AutoDel);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetImageFileDUMMY)(HWND,unsigned int, BOOL, const char*);
HDeskletLayerSetImageFileDUMMY HDeskletLayerSetImageFile = (HDeskletLayerSetImageFileDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetImageFile");

void  CALLBACK DeskletLayerSetImageFile(HWND hwndDesklet, unsigned int id, BOOL redraw, const char* filename)
{

	if(!HDeskletLayerSetImageFile)return;

	HDeskletLayerSetImageFile(hwndDesklet,id,redraw,filename);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetRotationDUMMY)(HWND,unsigned int, BOOL, int);
HDeskletLayerSetRotationDUMMY HDeskletLayerSetRotation = (HDeskletLayerSetRotationDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetRotation");

void  CALLBACK DeskletLayerSetRotation(HWND hwndDesklet, unsigned int id, BOOL redraw, int rotation)
{
	if(!HDeskletLayerSetRotation)return;

	HDeskletLayerSetRotation(hwndDesklet,id,redraw, rotation);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetSaturationDUMMY)(HWND,unsigned int, BOOL, unsigned int);
HDeskletLayerSetSaturationDUMMY HDeskletLayerSetSaturation = (HDeskletLayerSetSaturationDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetSaturation");

void  CALLBACK DeskletLayerSetSaturation(HWND hwndDesklet, unsigned int id, BOOL redraw, unsigned int saturation)
{
	if(!HDeskletLayerSetSaturation)return;

	HDeskletLayerSetSaturation(hwndDesklet,id,redraw, saturation);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetVisibilityDUMMY)(HWND,unsigned int, BOOL, BOOL);
HDeskletLayerSetVisibilityDUMMY HDeskletLayerSetVisibility = (HDeskletLayerSetVisibilityDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetVisibility");

void  CALLBACK DeskletLayerSetVisibility(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL visibility)
{
	if(!HDeskletLayerSetVisibility)return;

	HDeskletLayerSetVisibility(hwndDesklet,id,redraw, visibility);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletAddDragDropFormatDUMMY)(HWND,FORMATETC*);
HDeskletAddDragDropFormatDUMMY HDeskletAddDragDropFormat = (HDeskletAddDragDropFormatDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletAddDragDropFormat");

void CALLBACK DeskletAddDragDropFormat(HWND hwndDesklet,FORMATETC* format)
{
	if(!HDeskletAddDragDropFormat)return;

	HDeskletAddDragDropFormat(hwndDesklet,format);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletRemoveDragDropFormatDUMMY)(HWND,FORMATETC*);
HDeskletRemoveDragDropFormatDUMMY HDeskletRemoveDragDropFormat = (HDeskletRemoveDragDropFormatDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletRemoveDragDropFormat");

void CALLBACK DeskletRemoveDragDropFormat(HWND hwndDesklet,FORMATETC* format)
{

	if(!HDeskletRemoveDragDropFormat)return;

	HDeskletRemoveDragDropFormat(hwndDesklet,format);
}
//--------------------------------------------------------------------------------------------
typedef HMENU (CALLBACK *HDeskletGetDefaultMenuDUMMY)(HWND,int);
HDeskletGetDefaultMenuDUMMY HDeskletGetDefaultMenu = (HDeskletGetDefaultMenuDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletGetDefaultMenu");

HMENU CALLBACK DeskletGetDefaultMenu(HWND hwndDesklet, int startId)
{
	if(!HDeskletGetDefaultMenu)return 0;

	return HDeskletGetDefaultMenu(hwndDesklet,startId);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletDoDefaultMenuCommandDUMMY)(HWND,int,int);
HDeskletDoDefaultMenuCommandDUMMY HDeskletDoDefaultMenuCommand = (HDeskletDoDefaultMenuCommandDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletDoDefaultMenuCommand");

void CALLBACK DeskletDoDefaultMenuCommand(HWND hwndDesklet,int startId,int cmd)
{
	if(!HDeskletDoDefaultMenuCommand)return;

	HDeskletDoDefaultMenuCommand(hwndDesklet,startId,cmd);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetIconStateDUMMY)(HWND,unsigned int,BOOL,BOOL);
HDeskletLayerSetIconStateDUMMY HDeskletLayerSetIconState = (HDeskletLayerSetIconStateDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetIconState");

void CALLBACK DeskletLayerSetIconState(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL canBeIcon)
{
	if(!HDeskletLayerSetIconState)return;

	HDeskletLayerSetIconState(hwndDesklet,id,redraw,canBeIcon);
}
//--------------------------------------------------------------------------------------------
typedef int (CALLBACK *HDeskletAddPropertySheetDUMMY)(HWND,HPROPSHEETPAGE);
HDeskletAddPropertySheetDUMMY HDeskletAddPropertySheet = (HDeskletAddPropertySheetDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletAddPropertySheet");
int CALLBACK DeskletAddPropertySheet(HWND hwndDesklet, HPROPSHEETPAGE hpg)
{
	if(!HDeskletAddPropertySheet)return 0;

	return HDeskletAddPropertySheet(hwndDesklet,hpg);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletSetSubLabelDUMMY)(HWND,const char*);
HDeskletSetSubLabelDUMMY HDeskletSetSubLabel = (HDeskletSetSubLabelDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletSetSubLabel");
void CALLBACK DeskletSetSubLabel(HWND hwndDesklet, const char* subLabel)
{
	if(!HDeskletSetSubLabel)return;

	HDeskletSetSubLabel(hwndDesklet,subLabel);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletMoveDUMMY)(HWND,RECT*,BOOL);
HDeskletMoveDUMMY HDeskletMove = (HDeskletMoveDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletMove");
void CALLBACK DeskletMove(HWND hwndDesklet,RECT* r, BOOL redraw)
{
	if(!HDeskletMove)return;

	HDeskletMove(hwndDesklet,r,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletFadeInDUMMY) (HWND, BOOL, DWORD);
HDeskletFadeInDUMMY HDeskletFadeIn = (HDeskletFadeInDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletFadeIn");
void CALLBACK DeskletFadeIn(HWND hwndDesklet,BOOL async, DWORD durationInMs)
{
	if(!HDeskletFadeIn)return;

	HDeskletFadeIn(hwndDesklet, async, durationInMs);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletFadeOutDUMMY) (HWND, BOOL, DWORD durationInMs);
HDeskletFadeOutDUMMY HDeskletFadeOut = (HDeskletFadeOutDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletFadeOut");
void CALLBACK DeskletFadeOut(HWND hwndDesklet, BOOL async, DWORD durationInMs)
{
	if(!HDeskletFadeOut)return;

	HDeskletFadeOut(hwndDesklet, async, durationInMs);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletSaveWDUMMY) (HWND,const WCHAR*);
HDeskletSaveWDUMMY HDeskletSaveW = (HDeskletSaveWDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletSaveW");
BOOL CALLBACK DeskletSaveW(HWND hwndDesklet, const WCHAR* fileName)
{
	if(!HDeskletSaveW)
		return FALSE;

	return HDeskletSaveW(hwndDesklet,fileName);
}
//--------------------------------------------------------------------------------------------
typedef HWND (CALLBACK *HDeskletLoadFromDLLWDUMMY) (const WCHAR*, const WCHAR*, void**,BOOL);
HDeskletLoadFromDLLWDUMMY HDeskletLoadFromDLLW = (HDeskletLoadFromDLLWDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLoadFromDLLW");
HWND CALLBACK DeskletLoadFromDLLW(const WCHAR* dllFileName, const WCHAR* configFileName, void** pointer,BOOL makeVisible)
{
	if(!HDeskletLoadFromDLLW)
		return NULL;

	return HDeskletLoadFromDLLW(dllFileName,configFileName,pointer,makeVisible);
}
//--------------------------------------------------------------------------------------------
typedef HWND (CALLBACK *HDeskletLoadFromConfigWDUMMY)  (const WCHAR*, void**, BOOL);
HDeskletLoadFromConfigWDUMMY HDeskletLoadFromConfigW = (HDeskletLoadFromConfigWDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLoadFromConfigW");
HWND CALLBACK DeskletLoadFromConfigW(const WCHAR* fileName, void** pointer, BOOL makeVisible)
{
	if(!HDeskletLoadFromConfigW)
		return NULL;

	return HDeskletLoadFromConfigW(fileName,pointer,makeVisible);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletEnumRunningDeskletsDUMMY) (DESKLETENUMPROC,LPARAM);
HDeskletEnumRunningDeskletsDUMMY HDeskletEnumRunningDesklets = (HDeskletEnumRunningDeskletsDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletEnumRunningDesklets");
BOOL CALLBACK DeskletEnumRunningDesklets(DESKLETENUMPROC enumProc, LPARAM lParam)
{
	if(!HDeskletEnumRunningDesklets)
		return FALSE;

	return HDeskletEnumRunningDesklets(enumProc,lParam);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletAsyncSetPointerDUMMY) (HWND, void*);
HDeskletAsyncSetPointerDUMMY HDeskletAsyncSetPointer = (HDeskletAsyncSetPointerDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletAsyncSetPointer");
BOOL CALLBACK DeskletAsyncSetPointer(HWND hwndDesklet, void* pointer)
{
	if(!HDeskletAsyncSetPointer)
		return FALSE;

	return HDeskletAsyncSetPointer(hwndDesklet,pointer);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletSetSkinInfoWDUMMY) (HWND,const SkinInfo*);
HDeskletSetSkinInfoWDUMMY HDeskletSetSkinInfoW = (HDeskletSetSkinInfoWDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletSetSkinInfoW");
BOOL CALLBACK DeskletSetSkinInfoW(HWND hwndDesklet, const SkinInfo* info)
{
	if(!HDeskletSetSkinInfoW)
		return FALSE;

	return HDeskletSetSkinInfoW(hwndDesklet,info);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletGetDirectoryWDUMMY) (DWORD,WCHAR*,UINT*);
HDeskletGetDirectoryWDUMMY HDeskletGetDirectoryW = (HDeskletGetDirectoryWDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletGetDirectoryW");

BOOL CALLBACK DeskletGetDirectoryW(DWORD directory, WCHAR* wBuf, UINT* maxBufSize)
{
	if(!HDeskletGetDirectoryW)
		return FALSE;

	return HDeskletGetDirectoryW(directory,wBuf,maxBufSize);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletSetImageSetStatusDUMMY) (HWND, BOOL);
HDeskletSetImageSetStatusDUMMY HDeskletSetImageSetStatus = (HDeskletSetImageSetStatusDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletSetImageSetStatus");
BOOL CALLBACK DeskletSetImageSetStatus(HWND hwndDesklet, BOOL imageCanBeSet)
{
	if(!HDeskletSetImageSetStatus)
		return FALSE;

	return HDeskletSetImageSetStatus(hwndDesklet, imageCanBeSet);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletFlipDUMMY)(HWND, const FlipParams*);
HDeskletFlipDUMMY HDeskletFlip = (HDeskletFlipDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletFlip");
BOOL CALLBACK DeskletFlip(HWND hwndDesklet, const FlipParams* params)
{
	if(!HDeskletFlip)
		return FALSE;

	return HDeskletFlip(hwndDesklet, params);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletSetClosePointDUMMY)(HWND, const POINT*, int, int);
HDeskletSetClosePointDUMMY HDeskletSetClosePoint = (HDeskletSetClosePointDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletSetClosePoint");
BOOL CALLBACK DeskletSetClosePoint(HWND hwndDesklet, const POINT* closePoint, int xScale, int yScale)
{
	if(!HDeskletSetClosePoint)
		return FALSE;

	return HDeskletSetClosePoint(hwndDesklet, closePoint, xScale, yScale);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetIsNotMovableDUMMY)(HWND, unsigned int, BOOL);
HDeskletLayerSetIsNotMovableDUMMY HDeskletLayerSetIsNotMovable = (HDeskletLayerSetIsNotMovableDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerSetIsNotMovable");
void CALLBACK DeskletLayerSetIsNotMovable(HWND hwndDesklet, unsigned int id, BOOL isNotMovable)
{
	if(!HDeskletLayerSetIsNotMovable)
		return;

	HDeskletLayerSetIsNotMovable(hwndDesklet, id, isNotMovable);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetIsClickThroughDUMMY)(HWND, unsigned int, BOOL);
HDeskletLayerSetIsClickThroughDUMMY HDeskletLayerSetIsClickThrough = (HDeskletLayerSetIsClickThroughDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerSetIsClickThrough");
void CALLBACK DeskletLayerSetIsClickThrough(HWND hwndDesklet, unsigned int id, BOOL isClickThrough)
{
	if(!HDeskletLayerSetIsClickThrough)
		return;
	HDeskletLayerSetIsClickThrough(hwndDesklet, id, isClickThrough);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetNoAlphaBlendedHitTestingDUMMY)(HWND, unsigned int, BOOL);
HDeskletLayerSetNoAlphaBlendedHitTestingDUMMY HDeskletLayerSetNoAlphaBlendedHitTesting = (HDeskletLayerSetNoAlphaBlendedHitTestingDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerSetNoAlphaBlendedHitTesting");
void CALLBACK DeskletLayerSetNoAlphaBlendedHitTesting(HWND hwndDesklet, unsigned int id, BOOL noAlphaBlendedHitTesting)
{
	if(!HDeskletLayerSetNoAlphaBlendedHitTesting)
		return;

	HDeskletLayerSetNoAlphaBlendedHitTesting(hwndDesklet, id, noAlphaBlendedHitTesting);
}
//--------------------------------------------------------------------------------------------
typedef unsigned int (CALLBACK *HDeskletHitTestDUMMY)(HWND, const POINT*, const SIZE*, BOOL);
HDeskletHitTestDUMMY HDeskletHitTest = (HDeskletHitTestDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletHitTest");
unsigned int CALLBACK DeskletHitTest(HWND hwndDesklet, const POINT* pt, const SIZE* s, BOOL onlyTestVisibleLayers)
{
	if(!HDeskletHitTest)
		return (unsigned int) -1;

	return HDeskletHitTest(hwndDesklet, pt, s, onlyTestVisibleLayers);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletLayerHitTestDUMMY)(HWND, unsigned int, const POINT*, const SIZE*);
HDeskletLayerHitTestDUMMY HDeskletLayerHitTest = (HDeskletLayerHitTestDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerHitTest");
BOOL CALLBACK DeskletLayerHitTest(HWND hwndDesklet, unsigned int id, const POINT* pt, const SIZE* s)
{
	if(!HDeskletLayerHitTest)
		return FALSE;

	return HDeskletLayerHitTest(hwndDesklet, id, pt, s);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletRedrawDUMMY)(HWND);
HDeskletRedrawDUMMY HDeskletRedraw = (HDeskletRedrawDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletRedraw");
void CALLBACK DeskletRedraw(HWND hwndDesklet)
{
	if(!HDeskletRedraw)
		return;

	HDeskletRedraw(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef UINT (CALLBACK *HDeskletLayerSetCaptureDUMMY)(HWND, unsigned int);
HDeskletLayerSetCaptureDUMMY HDeskletLayerSetCapture = (HDeskletLayerSetCaptureDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerSetCapture");
UINT CALLBACK DeskletLayerSetCapture(HWND hwndDesklet, unsigned int id)
{
	if(!HDeskletLayerSetCapture)
		return (UINT)-1;

	return HDeskletLayerSetCapture(hwndDesklet, id);
}
//--------------------------------------------------------------------------------------------
typedef UINT (CALLBACK *HDeskletLayerReleaseCaptureDUMMY)(HWND);
HDeskletLayerReleaseCaptureDUMMY HDeskletLayerReleaseCapture = (HDeskletLayerReleaseCaptureDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerReleaseCapture");
UINT CALLBACK DeskletLayerReleaseCapture(HWND hwndDesklet)
{
	if(!HDeskletLayerReleaseCapture)
		return (UINT)-1;

	return HDeskletLayerReleaseCapture(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef UINT (CALLBACK *HDeskletLayerGetCaptureDUMMY)(HWND);
HDeskletLayerGetCaptureDUMMY HDeskletLayerGetCapture = (HDeskletLayerGetCaptureDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerGetCapture");
UINT CALLBACK DeskletLayerGetCapture(HWND hwndDesklet)
{
	if(!HDeskletLayerGetCapture)
		return (UINT)-1;

	return HDeskletLayerGetCapture(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletLayerSetImageClipRectDUMMY)(HWND, unsigned int, BOOL, BOOL, RECT*, int, int);
HDeskletLayerSetImageClipRectDUMMY HDeskletLayerSetImageClipRect = (HDeskletLayerSetImageClipRectDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLayerSetImageClipRect");
BOOL CALLBACK DeskletLayerSetImageClipRect(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL useBounds, RECT* bounds, int xScale, int yScale)
{
	if(!HDeskletLayerSetImageClipRect)
		return FALSE;

	return HDeskletLayerSetImageClipRect(hwndDesklet, id, redraw, useBounds, bounds, xScale, yScale);
}
//--------------------------------------------------------------------------------------------
typedef INT (CALLBACK *HDeskletLockInvalidatingDUMMY)(HWND);
HDeskletLockInvalidatingDUMMY HDeskletLockInvalidating = (HDeskletLockInvalidatingDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletLockInvalidating");
INT CALLBACK DeskletLockInvalidating(HWND hwndDesklet)
{
	if(!HDeskletLockInvalidating)
		return 0;

	return HDeskletLockInvalidating(hwndDesklet);
}
//--------------------------------------------------------------------------------------------
typedef INT (CALLBACK *HDeskletUnlockInvalidatingDUMMY)(HWND,BOOL);
HDeskletUnlockInvalidatingDUMMY HDeskletUnlockInvalidating = (HDeskletUnlockInvalidatingDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletUnlockInvalidating");
INT CALLBACK DeskletUnlockInvalidating(HWND hwndDesklet, BOOL redraw)
{
	if(!HDeskletUnlockInvalidating)
		return 0;

	return HDeskletUnlockInvalidating(hwndDesklet, redraw);
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *HDeskletInvalidateRectDUMMY)(HWND, RECT*, SIZE*, BOOL);
HDeskletInvalidateRectDUMMY HDeskletInvalidateRect = (HDeskletInvalidateRectDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletInvalidateRect");
BOOL DeskletInvalidateRect(HWND hwndDesklet, RECT* rect, SIZE* scale, BOOL redraw)
{
	if(!HDeskletInvalidateRect)
		return FALSE;

	return HDeskletInvalidateRect(hwndDesklet, rect, scale, redraw);
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetImageFileWDUMMY)(HWND,unsigned int, BOOL, const WCHAR*);
HDeskletLayerSetImageFileWDUMMY HDeskletLayerSetImageFileW = (HDeskletLayerSetImageFileWDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetImageFileW");

void  CALLBACK DeskletLayerSetImageFileW(HWND hwndDesklet, unsigned int id, BOOL redraw, const WCHAR* filename)
{

	if(!HDeskletLayerSetImageFileW)
		return;

	HDeskletLayerSetImageFileW(hwndDesklet,id,redraw,filename);
}
//--------------------------------------------------------------------------------------------
typedef Bitmap* (CALLBACK *HDeskletLoadGDIPlusImageWDUMMY)(const WCHAR*);
HDeskletLoadGDIPlusImageWDUMMY HDeskletLoadGDIPlusImageW = (HDeskletLoadGDIPlusImageWDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletLoadGDIPlusImageW");

Bitmap* CALLBACK DeskletLoadGDIPlusImageW(const WCHAR *szImage)
{

	if(!HDeskletLoadGDIPlusImageW)
		return 0;

	return HDeskletLoadGDIPlusImageW(szImage);
}
//-----------------------------------------------------------------------------
typedef HCURSOR (CALLBACK *HDeskletSetCursorDUMMY)(HWND, HCURSOR);
HDeskletSetCursorDUMMY HDeskletSetCursor = (HDeskletSetCursorDUMMY) GetProcAddress(GetModuleHandle(0), "DeskletSetCursor");
HCURSOR CALLBACK DeskletSetCursor(HWND hwndDesklet, HCURSOR hCursor)
{
	if(!HDeskletSetCursor)
		return NULL;

	return HDeskletSetCursor(hwndDesklet, hCursor);
}
//-----------------------------------------------------------------------------
