#include <windows.h>
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
//--------------------------------------------------------------------------------------------
int WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int iValue, LPCTSTR lpFileName)
{
	char szNumber[100];
	strcpy(szNumber, "");
	itoa(iValue, szNumber, 10);
	return WritePrivateProfileString(lpAppName, lpKeyName, szNumber, lpFileName);
}
//--------------------------------------------------------------------------------------------
int ShowContextMenu(HWND owner, HMENU menu, POINT* p, char* filename)
{
	if(!filename)return 0;

	char file[MAX_PATH];
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

	LPSHELLFOLDER DesktopFolder;
	SHGetDesktopFolder(&DesktopFolder);
	if(!DesktopFolder)
	{
		return 0;
	}
	wchar_t Path[MAX_PATH];
	LPITEMIDLIST ParentPidl;
	DWORD dummy;
	MultiByteToWideChar(CP_ACP,0,filepath,-1,Path,MAX_PATH);
	DesktopFolder->ParseDisplayName(owner, 0, Path, &dummy, &ParentPidl, 0);
	LPSHELLFOLDER ParentFolder;
	DesktopFolder->BindToObject(ParentPidl, 0, IID_IShellFolder, (void**)&ParentFolder);
	if(!ParentFolder)
	{
		DesktopFolder->Release();
		return 0;
	}

	LPITEMIDLIST Pidl;
	MultiByteToWideChar(CP_ACP,0,file,-1,Path,MAX_PATH);
	ParentFolder->ParseDisplayName(owner, 0, Path, &dummy, &Pidl, 0);


	LPCONTEXTMENU CM;
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
		CMINVOKECOMMANDINFO CI;

		ZeroMemory(&CI, sizeof(CI));
		CI.cbSize = sizeof(CMINVOKECOMMANDINFO);
		CI.hwnd = owner;
		CI.lpVerb = MAKEINTRESOURCE(Cmd - 1);
		CI.lpParameters = "";
		CI.lpDirectory = "";
		CI.nShow = SW_SHOWNORMAL;
		CM->InvokeCommand(&CI);

		CM->Release();
		ParentFolder->Release();
		DesktopFolder->Release();

		return 0;
		
	}
	else
	{
		CM->Release();
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
typedef BOOL (CALLBACK *HDeskletSetLabelDUMMY)(HWND,char*);
HDeskletSetLabelDUMMY HDeskletSetLabel = (HDeskletSetLabelDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetLabel");

int CALLBACK DeskletSetLabel(HWND hwndDesklet, char* szLabel)
{
	if(!HDeskletSetLabel)return FALSE;

	return HDeskletSetLabel(hwndDesklet,szLabel);
}
//--------------------------------------------------------------------------------------------
typedef Bitmap* (CALLBACK *HDeskletLoadGDIPlusImageDUMMY)(char*);
HDeskletLoadGDIPlusImageDUMMY HDeskletLoadGDIPlusImage = (HDeskletLoadGDIPlusImageDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletLoadGDIPlusImage");

Bitmap* CALLBACK DeskletLoadGDIPlusImage(char *szImage)
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
typedef void (CALLBACK *HDeskletSetImageFileDUMMY)(HWND hwndDesklet,char *szImage);
HDeskletSetImageFileDUMMY HDeskletSetImageFile = (HDeskletSetImageFileDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetImageFile");

void CALLBACK DeskletSetImageFile(HWND hwndDesklet,char *szImage)
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
typedef void (CALLBACK *HDeskletSetImageOverlayFileDUMMY)(HWND hwndDesklet,char *szImageOverlay);
HDeskletSetImageOverlayFileDUMMY HDeskletSetImageOverlayFile = (HDeskletSetImageOverlayFileDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletSetImageOverlayFile");

void CALLBACK DeskletSetImageOverlayFile(HWND hwndDesklet,char *szImageOverlay)
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
typedef void (CALLBACK *HDeskletGetRootFolderDUMMY)(HWND,char*);
HDeskletGetRootFolderDUMMY HDeskletGetRootFolder = (HDeskletGetRootFolderDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetRootFolder");

void CALLBACK DeskletGetRootFolder(HWND hwndDesklet,char *szFolder)
{
	if(!HDeskletGetRootFolder)return;

	HDeskletGetRootFolder(hwndDesklet,szFolder);
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
typedef BOOL (CALLBACK *HDeskletRemoveDUMMY)(HWND,bool);
HDeskletRemoveDUMMY HDeskletRemove = (HDeskletRemoveDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletRemove");

BOOL CALLBACK DeskletRemove(HWND hwndDesklet, bool Ask)
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
typedef HWND (CALLBACK *HDeskletLoadDUMMY)(char*,char*,char*,void**,bool);
HDeskletLoadDUMMY HDeskletLoad = (HDeskletLoadDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLoad");

HWND CALLBACK DeskletLoad(char* filename,char* ini, char* iniGroup,void** p,bool makeVisible)
{
	
	if(!HDeskletLoad)return 0;

	return DeskletLoad(filename,ini,iniGroup,p,makeVisible);
}
//--------------------------------------------------------------------------------------------
typedef UINT (CALLBACK *HDeskletGetParentOKDUMMY)();
HDeskletGetParentOKDUMMY HDeskletGetParentOK = (HDeskletGetParentOKDUMMY)GetProcAddress(GetModuleHandle(0),"DeskletGetParentOK");

UINT CALLBACK DeskletGetParentOK()
{
	if(!HDeskletGetParentOK)return 0;

	return DeskletGetParentOK();
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
typedef void (CALLBACK *HDeskletSetStaticDUMMY)(HWND,bool);
HDeskletSetStaticDUMMY HDeskletSetStatic = (HDeskletSetStaticDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletSetStatic");

void CALLBACK DeskletSetStatic(HWND hwndDesklet, bool staticValue)
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
typedef void (CALLBACK *HDeskletLayerSetPlacementDUMMY)(HWND,unsigned int, bool, RECT*, int, int);
HDeskletLayerSetPlacementDUMMY HDeskletLayerSetPlacement = (HDeskletLayerSetPlacementDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetPlacement");

void CALLBACK DeskletLayerSetPlacement(HWND hwndDesklet, unsigned int id, bool usePlacement, RECT* s, int xScale, int yScale)
{

	if(!HDeskletLayerSetPlacement)return;

	HDeskletLayerSetPlacement(hwndDesklet,id,usePlacement,s,xScale,yScale);
}
//--------------------------------------------------------------------------------------------
typedef unsigned int (CALLBACK *HDeskletLayerGetLayerDUMMY)(HWND,char*, bool*);
HDeskletLayerGetLayerDUMMY HDeskletLayerGetLayer = (HDeskletLayerGetLayerDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerGetLayer");

unsigned int CALLBACK DeskletLayerGetLayer(HWND hwndDesklet, char* name, bool* success)
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
typedef unsigned int (CALLBACK *HDeskletLayerAddDUMMY)(HWND,char*);
HDeskletLayerAddDUMMY HDeskletLayerAdd = (HDeskletLayerAddDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerAdd");

unsigned int CALLBACK DeskletLayerAdd(HWND hwndDesklet, char* name)
{
	if(!HDeskletLayerAdd)return 0;

	return HDeskletLayerAdd(hwndDesklet,name);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerRemoveDUMMY)(HWND,unsigned int, bool);
HDeskletLayerRemoveDUMMY HDeskletLayerRemove = (HDeskletLayerRemoveDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerRemove");

void  CALLBACK DeskletLayerRemove(HWND hwndDesklet, unsigned int id, bool redraw)
{
	if(!HDeskletLayerRemove)return;

	HDeskletLayerRemove(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMergeAllDUMMY)(HWND,bool);
HDeskletLayerMergeAllDUMMY HDeskletLayerMergeAll = (HDeskletLayerMergeAllDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMergeAll");

void  CALLBACK DeskletLayerMergeAll(HWND hwndDesklet, bool redraw)
{
	if(!HDeskletLayerMergeAll)return;

	HDeskletLayerMergeAll(hwndDesklet,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMergeVisibleDUMMY)(HWND, bool);
HDeskletLayerMergeVisibleDUMMY HDeskletLayerMergeVisible = (HDeskletLayerMergeVisibleDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMergeVisible");

void  CALLBACK DeskletLayerMergeVisible(HWND hwndDesklet,  bool redraw)
{
	if(!HDeskletLayerMergeVisible)return;

	HDeskletLayerMergeVisible(hwndDesklet,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveDownDUMMY)(HWND,unsigned int, bool);
HDeskletLayerMoveDownDUMMY HDeskletLayerMoveDown = (HDeskletLayerMoveDownDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveDown");

void  CALLBACK DeskletLayerMoveDown(HWND hwndDesklet, unsigned int id, bool redraw)
{
	if(!HDeskletLayerMoveDown)return;

	HDeskletLayerMoveDown(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveTopDUMMY)(HWND,unsigned int, bool);
HDeskletLayerMoveTopDUMMY HDeskletLayerMoveTop = (HDeskletLayerMoveTopDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveTop");

void  CALLBACK DeskletLayerMoveTop(HWND hwndDesklet, unsigned int id, bool redraw)
{
	if(!HDeskletLayerMoveTop)return;

	HDeskletLayerMoveTop(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveBottomDUMMY)(HWND,unsigned int, bool);
HDeskletLayerMoveBottomDUMMY HDeskletLayerMoveBottom = (HDeskletLayerMoveBottomDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveBottom");

void  CALLBACK DeskletLayerMoveBottom(HWND hwndDesklet, unsigned int id, bool redraw)
{
	if(!HDeskletLayerMoveBottom)return;

	HDeskletLayerMoveBottom(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerMoveUpDUMMY)(HWND,unsigned int, bool);
HDeskletLayerMoveUpDUMMY HDeskletLayerMoveUp = (HDeskletLayerMoveUpDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerMoveUp");

void  CALLBACK DeskletLayerMoveUp(HWND hwndDesklet, unsigned int id, bool redraw)
{
	if(!HDeskletLayerMoveUp)return;

	HDeskletLayerMoveUp(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerResetDUMMY)(HWND,unsigned int, bool);
HDeskletLayerResetDUMMY HDeskletLayerReset = (HDeskletLayerResetDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerReset");

void  CALLBACK DeskletLayerReset(HWND hwndDesklet, unsigned int id, bool redraw)
{
	if(!HDeskletLayerReset)return;

	HDeskletLayerReset(hwndDesklet,id,redraw);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetAlphaDUMMY)(HWND,unsigned int, bool, BYTE);
HDeskletLayerSetAlphaDUMMY HDeskletLayerSetAlpha = (HDeskletLayerSetAlphaDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetAlpha");

void CALLBACK DeskletLayerSetAlpha(HWND hwndDesklet, unsigned int id, bool redraw, BYTE alpha)
{
	if(!HDeskletLayerSetAlpha)return;

	HDeskletLayerSetAlpha(hwndDesklet,id,redraw, alpha);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetColorMatrixDUMMY)(HWND,unsigned int, bool, ColorMatrix*);
HDeskletLayerSetColorMatrixDUMMY HDeskletLayerSetColorMatrix = (HDeskletLayerSetColorMatrixDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetColorMatrix");

void CALLBACK DeskletLayerSetColorMatrix(HWND hwndDesklet, unsigned int id, bool redraw, ColorMatrix* matrix)
{
	if(!HDeskletLayerSetColorMatrix)return;

	HDeskletLayerSetColorMatrix(hwndDesklet,id,redraw, matrix);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetHueDUMMY)(HWND,unsigned int, bool, int);
HDeskletLayerSetHueDUMMY HDeskletLayerSetHue = (HDeskletLayerSetHueDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetHue");

void  CALLBACK DeskletLayerSetHue(HWND hwndDesklet, unsigned int id, bool redraw, int hue)
{

	if(!HDeskletLayerSetHue)
	{
		MessageBox(0,"error",0,0);
		return;
	}

	HDeskletLayerSetHue(hwndDesklet,id,redraw, hue);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetImageDUMMY)(HWND,unsigned int, bool, Gdiplus::Image*,bool);
HDeskletLayerSetImageDUMMY HDeskletLayerSetImage = (HDeskletLayerSetImageDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetImage");

void CALLBACK DeskletLayerSetImage(HWND hwndDesklet, unsigned int id, bool redraw, Gdiplus::Image* img, bool AutoDel)
{
	if(!HDeskletLayerSetImage)return;

	HDeskletLayerSetImage(hwndDesklet,id,redraw,img,AutoDel);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetImageFileDUMMY)(HWND,unsigned int, bool, char*);
HDeskletLayerSetImageFileDUMMY HDeskletLayerSetImageFile = (HDeskletLayerSetImageFileDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetImageFile");

void  CALLBACK DeskletLayerSetImageFile(HWND hwndDesklet, unsigned int id, bool redraw, char* filename)
{

	if(!HDeskletLayerSetImageFile)return;

	HDeskletLayerSetImageFile(hwndDesklet,id,redraw,filename);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetRotationDUMMY)(HWND,unsigned int, bool, int);
HDeskletLayerSetRotationDUMMY HDeskletLayerSetRotation = (HDeskletLayerSetRotationDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetRotation");

void  CALLBACK DeskletLayerSetRotation(HWND hwndDesklet, unsigned int id, bool redraw, int rotation)
{
	if(!HDeskletLayerSetRotation)return;

	HDeskletLayerSetRotation(hwndDesklet,id,redraw, rotation);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetSaturationDUMMY)(HWND,unsigned int, bool, unsigned int);
HDeskletLayerSetSaturationDUMMY HDeskletLayerSetSaturation = (HDeskletLayerSetSaturationDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetSaturation");

void  CALLBACK DeskletLayerSetSaturation(HWND hwndDesklet, unsigned int id, bool redraw, unsigned int saturation)
{
	if(!HDeskletLayerSetSaturation)return;

	HDeskletLayerSetSaturation(hwndDesklet,id,redraw, saturation);
}
//--------------------------------------------------------------------------------------------
typedef void (CALLBACK *HDeskletLayerSetVisibilityDUMMY)(HWND,unsigned int, bool, bool);
HDeskletLayerSetVisibilityDUMMY HDeskletLayerSetVisibility = (HDeskletLayerSetVisibilityDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetVisibility");

void  CALLBACK DeskletLayerSetVisibility(HWND hwndDesklet, unsigned int id, bool redraw, bool visibility)
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
typedef void (CALLBACK *HDeskletLayerSetIconStateDUMMY)(HWND,unsigned int,bool,bool);
HDeskletLayerSetIconStateDUMMY HDeskletLayerSetIconState = (HDeskletLayerSetIconStateDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletLayerSetIconState");

void CALLBACK DeskletLayerSetIconState(HWND hwndDesklet, unsigned int id, bool redraw, bool canBeIcon)
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
typedef void (CALLBACK *HDeskletSetSubLabelDUMMY)(HWND,char*);
HDeskletSetSubLabelDUMMY HDeskletSetSubLabel = (HDeskletSetSubLabelDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletSetSubLabel");
void CALLBACK DeskletSetSubLabel(HWND hwndDesklet, char* subLabel)
{
	if(!HDeskletSetSubLabel)return;

	HDeskletSetSubLabel(hwndDesklet,subLabel);
}
//--------------------------------------------------------------------------------------------