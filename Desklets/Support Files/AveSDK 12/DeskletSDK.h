//Header file for the Desklets SDK
#pragma once

#include <windows.h>

#include <Gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include <oleidl.h>

#include <prsht.h>
#pragma comment(lib, "comctl32.lib")


// CONSTANTS
const ANIMATION_NONE	= 0;
const ANIMATION_FADEIN	= 1;
const ANIMATION_FADEOUT	= 2;
const ANIMATION_FADEINANDOUT = 3;
const ANIMATION_FADEOUTANDIN = 4;

const DM_CONTROLPANEL = 0;
const DM_SAVE = 1;
const DM_ABOUT = 2;
const DM_HELP = 3;
const DM_EXIT = 4;
const DM_GLOBALSETTINGS = 5;
const DM_DEFAULTDESKLETPROPERTIES = 6;
const DM_ADD = 7;

const AVEDESK_SETTING_SETTINGSPATH	= 1;
const AVEDESK_SETTING_THEMESPATH	= 2;

// STRUCTURES
struct TabPage
{
public:
	char*		title;			// title of the page
	int			titleLength;	// length of title
	char*		resourceName;	// resource name
	HINSTANCE	hInstance;		// instance where resource is located
	DLGPROC		dlgProc;		// callback dialog proc
};

typedef TabPage* LPTabPage;

struct PropertyDialogStruct
{
public:
	LPARAM		lParam;			// reserved
	UINT		pagesCount;		// number of pages
	TabPage*	pages;			// array of pages
};

typedef PropertyDialogStruct* LPPropertyDialogStruct;

// v1.1
struct DropStruct
{
public:
	HDROP hDrop;
	STGMEDIUM medium; 
};

//--------------------------------------------------------------------------------------------
void GetDLLPath(HMODULE h,char* path, DWORD nSize);
int WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int iValue, LPCTSTR lpFileName);
int ShowContextMenu(HWND owner, HMENU menu, POINT* p, const char* file);

BOOL CALLBACK DeskletIsVisible(HWND hwndDesklet);
BOOL CALLBACK DeskletGetRect(HWND hwndDesklet,RECT *rcDesklet);
int CALLBACK DeskletGetLabel(HWND hwndDesklet,char* szLabel);
int CALLBACK DeskletSetLabel(HWND hwndDesklet, const char* szLabel);
Bitmap* CALLBACK DeskletLoadGDIPlusImage(const char *szImage);
void CALLBACK DeskletSetImage(HWND hwndDesklet, Image *lpImageNew,BOOL bAutomaticallyDeleteImage=TRUE);
void CALLBACK DeskletSetImageFile(HWND hwndDesklet,const char *szImage);
void CALLBACK DeskletSetImageOverlay(HWND hwndDesklet, Image *lpImageNew,BOOL bAutomaticallyDeleteImage=TRUE);
void CALLBACK DeskletSetImageOverlayFile(HWND hwndDesklet,const char *szImage);
BOOL CALLBACK DeskletBrowseForImage(HWND hwndParent,char *szImage);
void CALLBACK DeskletDoAttentionAnimation(HWND hwndDesklet, INT iAnimationType, UINT speed);
void CALLBACK DeskletGetRelativeFolder(HWND hwndDesklet, char *szFolder);
void CALLBACK DeskletGetRootFolder(HWND hwndDesklet,char *szFolder);
void CALLBACK DeskletDefaultConfigDialog(HWND hwndDesklet,PropertyDialogStruct* pds);

// EXTENDED DESKLET API
BOOL CALLBACK DeskletRemove(HWND hwndDesklet, BOOL Ask);
void CALLBACK DeskletHide(HWND hwndDesklet);
void CALLBACK DeskletShow(HWND hwndDesklet);
void CALLBACK DeskletSetPosition(HWND hwndDesklet,POINT *p);
void CALLBACK DeskletSetSize(HWND hwndDesklet,SIZE *s);
void CALLBACK DeskletSetAlpha(HWND hwndDesklet,BYTE alpha);
void CALLBACK DeskletSetRotation(HWND hwndDesklet,INT rotation);
void CALLBACK DeskletGetPosition(HWND hwndDesklet,POINT *p);
void CALLBACK DeskletGetSize(HWND hwndDesklet,SIZE *s);
BYTE CALLBACK DeskletGetAlpha(HWND hwndDesklet);
INT  CALLBACK DeskletGetRotation(HWND hwndDesklet);
HWND CALLBACK DeskletLoad(const char* filename,const char* ini, const char* iniGroup,void** p,BOOL makeVisible=false);
UINT CALLBACK DeskletGetParentOK();
void CALLBACK DeskletManager(UINT action);
void CALLBACK DeskletSetStatic(HWND hwndDesklet, BOOL staticValue);

//  new v1.1
void CALLBACK DeskletActivateExtendedDragDrop(HWND hwndDesklet);
void CALLBACK DeskletDeactivateExtendedDragDrop(HWND hwndDesklet);
void CALLBACK DeskletAddDragDropFormat(HWND hwndDesklet,FORMATETC* format);
void CALLBACK DeskletRemoveDragDropFormat(HWND hwndDesklet,FORMATETC* format);
void CALLBACK DeskletMove(HWND hwndDesklet,RECT* r, BOOL redraw);

// SUPPORT FUNCTIONS
// new in v1.1
void UnlockHDROP(DropStruct* ds);
DropStruct* LockHDROP(LPDATAOBJECT pDataObj);
BOOL IsDataObjectHDROP(LPDATAOBJECT pDataObj);

// LAYER FUNCTIONS
// new in v1.1
void CALLBACK DeskletLayerSetPlacement(HWND hwndDesklet, unsigned int id,BOOL redraw, BOOL usePlacement, RECT* s, int xScale, int yScale);
unsigned int CALLBACK DeskletLayerGetLayer(HWND hwndDesklet, const char* name, BOOL* success);
unsigned int CALLBACK DeskletLayerCount(HWND hwndDesklet);
unsigned int CALLBACK DeskletLayerAdd(HWND hwndDesklet, const char* name);
void  CALLBACK DeskletLayerRemove(HWND hwndDesklet, unsigned int id, BOOL redraw);
void  CALLBACK DeskletLayerMergeAll(HWND hwndDesklet, BOOL redraw);


void  CALLBACK DeskletLayerMergeVisible(HWND hwndDesklet,  BOOL redraw);
void  CALLBACK DeskletLayerMoveDown(HWND hwndDesklet, unsigned int id, BOOL redraw);
void  CALLBACK DeskletLayerMoveTop(HWND hwndDesklet, unsigned int id, BOOL redraw);
void  CALLBACK DeskletLayerMoveBottom(HWND hwndDesklet, unsigned int id, BOOL redraw);
void  CALLBACK DeskletLayerMoveUp(HWND hwndDesklet, unsigned int id, BOOL redraw);
void  CALLBACK DeskletLayerReset(HWND hwndDesklet, unsigned int id, BOOL redraw);
void  CALLBACK DeskletLayerSetAlpha(HWND hwndDesklet, unsigned int id, BOOL redraw, BYTE alpha);
void  CALLBACK DeskletLayerSetColorMatrix(HWND hwndDesklet, unsigned int id, BOOL redraw, ColorMatrix* matrix);

void  CALLBACK DeskletLayerSetHue(HWND hwndDesklet, unsigned int id, BOOL redraw, int hue);
void  CALLBACK DeskletLayerSetImage(HWND hwndDesklet, unsigned int id, BOOL redraw, Gdiplus::Image* img, BOOL AutoDel);
void  CALLBACK DeskletLayerSetImageFile(HWND hwndDesklet, unsigned int id, BOOL redraw, const char* filename);
void  CALLBACK DeskletLayerSetRotation(HWND hwndDesklet, unsigned int id, BOOL redraw, int rotation);
void  CALLBACK DeskletLayerSetSaturation(HWND hwndDesklet, unsigned int id, BOOL redraw, unsigned int saturation);
void  CALLBACK DeskletLayerSetVisibility(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL visibility);
void  CALLBACK DeskletLayerSetIconState(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL canBeIcon);

HMENU CALLBACK DeskletGetDefaultMenu(HWND hwndDesklet, int startId);
void CALLBACK DeskletDoDefaultMenuCommand(HWND hwndDesklet,int startId,int cmd);

int CALLBACK DeskletAddPropertySheet(HWND hwndDesklet, HPROPSHEETPAGE hpg);


void CALLBACK DeskletSetSubLabel(HWND hwndDesklet, const char* subLabel);

// 1.2
const DWORD USE_DEFAULT_DURATION = -1;


const AVEDESK_NO_MORE_SKINS = FALSE;
const AVEDESK_MORE_SKINS	= TRUE;
const AVEDESK_BUF_TOO_SMALL	= -1;


#pragma pack(push, 1)
struct SkinInfo
{
	DWORD dwSize;			// sizeof(SkinInfo)

	WCHAR* wKey;			// key
	DWORD maxKeyLength;		// length of key buffer
	UINT num;				// skinNumber


	WCHAR* wName;			// skin name
	DWORD  maxNameLength;	// length of name buffer
	WCHAR* wAuthor;			// skin author
	DWORD maxAuthorLength;	// length of author buffer
	WCHAR* wNotes;			// skin notes
	DWORD maxNotesLength;	// length of notes buffer
	int version;			// skin version
};
#pragma pack(pop,1)

void CALLBACK DeskletFadeIn(HWND hwndDesklet, BOOL async, DWORD durationInMs);
void CALLBACK DeskletFadeOut(HWND hwndDesklet, BOOL async, DWORD durationInMs);

BOOL CALLBACK DeskletSaveW(HWND hwndDesklet, const WCHAR* fileName);
HWND CALLBACK DeskletLoadFromDLLW(const WCHAR* dllFileName, const WCHAR* configFileName, void** pointer,BOOL makeVisible);
HWND CALLBACK DeskletLoadFromConfigW(const WCHAR* fileName, void** pointer, BOOL makeVisible);
void GetDLLPathW(HMODULE h,WCHAR* path, DWORD nSize);

typedef BOOL (CALLBACK *DESKLETENUMPROC)(HWND,LPARAM);
BOOL CALLBACK DeskletEnumRunningDesklets(DESKLETENUMPROC enumProc, LPARAM lParam);
BOOL CALLBACK DeskletAsyncSetPointer(HWND hwndDesklet, void* pointer);
BOOL CALLBACK DeskletSetSkinInfoW(HWND hwndDesklet, const SkinInfo* info);
BOOL CALLBACK DeskletGetDirectoryW(DWORD directory, WCHAR* wBuf, UINT* maxBufSize);