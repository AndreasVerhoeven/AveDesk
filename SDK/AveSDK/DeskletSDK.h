//	This file and its contents is copyright � 2006 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

//Header file for the Desklets SDK
#pragma once

#include <windows.h>

#include <Gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include <oleidl.h>

#include <prsht.h>
#pragma comment(lib, "comctl32.lib")

// animation constants
const int ANIMATION_NONE	= 0;
const int ANIMATION_FADEIN	= 1;
const int ANIMATION_FADEOUT	= 2;
const int ANIMATION_FADEINANDOUT = 3;
const int ANIMATION_FADEOUTANDIN = 4;
const DWORD USE_DEFAULT_DURATION = -1;

// DeskletManager() constants
const int DM_CONTROLPANEL = 0;
const int DM_SAVE = 1;
const int DM_ABOUT = 2;
const int DM_HELP = 3;
const int DM_EXIT = 4;
const int DM_GLOBALSETTINGS = 5;
const int DM_DEFAULTDESKLETPROPERTIES = 6;
const int DM_ADD = 7;


// AveDeskGetDirectory() constant
const int AVEDESK_SETTING_SETTINGSPATH	= 1;
const int AVEDESK_SETTING_THEMESPATH	= 2;

// OnGetFlags() flags
const int DESKLET_NO_LABEL					= 0x01;
const int DESKLET_ONLY_INSTANTIATE_SKINS	= 0x02;
const int DESKLET_SUPPORTS_ASYNC_PAINTING	= 0x04;
const int DESKLET_NO_ROTATION				= 0x08;
const int DESKLET_NO_CHANGE_IMAGE			= 0x16;


// DeskletLayerAddLayerEx() flags
const int LAYER_IS_CONTAINER = 0x01;
const int LAYER_IS_BUFFERED =  0x02;


// OnGetSkinW() return constants
const int AVEDESK_NO_MORE_SKINS = FALSE;
const int AVEDESK_MORE_SKINS	= TRUE;
const int AVEDESK_BUF_TOO_SMALL	= -1;


//--------------------------------------------------------------------------------------------
// STRUCTURES

// v1.0 Tabpage structure - deprecated
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

// v1.0 PropertyDialogStruct structure - deprecated
struct PropertyDialogStruct
{
public:
	LPARAM		lParam;			// reserved
	UINT		pagesCount;		// number of pages
	TabPage*	pages;			// array of pages
};
typedef PropertyDialogStruct* LPPropertyDialogStruct;

// v1.1 DropStruct structure for DataObjectToHDROP()
struct DropStruct
{
public:
	HDROP hDrop;
	STGMEDIUM medium; 
};


// v1.2 SkinInfo structure
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


// forward declaration for callback function used in FlipParams structure
struct FlipParams;
typedef void (CALLBACK* DeskletFlipCallback)(void*, const FlipParams*);

// v1.3 FlipParams structure
struct FlipParams
{
	BOOL   useLayers;				// indicates if layers are used

	SIZE*  oldDeskletSize;			// optional
	UINT*  oldLayersVisible;		// optional
	DWORD  oldLayersVisibleCount;	// mandatory if oldLayersVisible != NULL

	SIZE*  newDeskletSize;			// optional
	UINT*  newLayersVisible;		// if useLayers, then mandatory
	DWORD  newLayersVisibleCount;	// mandatory if newLayersVisibile != NULL

	Bitmap* oldBitmap;				// optional
	Bitmap* newBitmap;				// if !useLayers, then mandatory

	BOOL    flipToBack;				// if true, then it flips to the back, otherwise to the front

	DeskletFlipCallback callback;	// can be used to be notified when the flip is done
	void*       callbackData;		// can be used for the callback as the first parameter
};
#pragma pack(pop)



// v2.0: skin preview bitmap generation constants
// cache data types
// cache data types
const DWORD CACHE_TYPE_LARGE_BITMAP		= 0x01;
const DWORD CACHE_TYPE_SMALL_BITMAP		= 0x02;

//  OnGetSkinPreviewBitmapCache() return values
const UINT AVEDESK_CACHE_ENTRY_FILLED	= 0x01;	// return when you filled in the cache entry
const UINT AVEDESK_CACHE_NOT_SUPPORTED	= 0x02; // return when your desklet does not support caching

// PreviewRequestData flags
const DWORD PREVIEW_REQUEST_WANT_LARGE_BITMAP		= 0x01;	// [in]
const DWORD PREVIEW_REQUEST_WANT_SMALL_BITMAP		= 0x02; // [in]
const DWORD PREVIEW_REQUEST_SMALL_AND_LARGE_SAME	= 0x04; // [out]


#pragma pack(push, 1)
const size_t MAX_CACHENAME = MAX_PATH + 100;
struct CacheEntry
{
	DWORD dwSize;				// sizeof(CacheData)

	DWORD	type;				// the type of this cache entry

	WCHAR name[MAX_CACHENAME];	// name of this cache entry
	FILETIME timestamp;			// time of this cache entry

};

struct PreviewRequestData
{
	DWORD dwSize;				// sizeof(PreviewData) [in]

	DWORD flags;				// the flags for this request [in, out]

	SIZE bmpSize;				// bitmap size of the request [in, out]
	HBITMAP hBmp;				// bitmap handle to be filled [in]

	BOOL bStop;					// TRUE iff the request should be canceled. [in]

	LPARAM lParam;				// token that can be set by the callee. [out] Can be used
								// in calls to OnStopSkinPreviewBitmap() to idenitify the
								// request to stop.
};

#pragma pack(pop)


//--------------------------------------------------------------------------------------------
// function prototypes

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

// AVEDESK 1.3
BOOL CALLBACK DeskletSetImageSetStatus(HWND hwndDesklet, BOOL imageCanBeSet);

BOOL CALLBACK DeskletFlip(HWND hwndDesklet, const FlipParams* params);
BOOL CALLBACK DeskletSetClosePoint(HWND hwndDesklet, const POINT* closePoint, int xScale, int yScale);
void CALLBACK DeskletLayerSetIsNotMovable(HWND hwndDesklet, unsigned int id, BOOL isNotMovable);
void CALLBACK DeskletLayerSetIsClickThrough(HWND hwndDesklet, unsigned int id, BOOL isClickThrough);
void CALLBACK DeskletLayerSetNoAlphaBlendedHitTesting(HWND hwndDesklet, unsigned int id, BOOL noAlphaBlendedHitTesting);
unsigned int CALLBACK DeskletHitTest(HWND hwndDesklet, const POINT* pt, const SIZE* s, BOOL onlyTestVisibleLayers);
BOOL CALLBACK DeskletLayerHitTest(HWND hwndDesklet, unsigned int id, const POINT* pt, const SIZE* s);

UINT CALLBACK DeskletLayerSetCapture(HWND hwndDesklet, unsigned int id);
UINT CALLBACK DeskletLayerReleaseCapture(HWND hwndDesklet);
UINT CALLBACK DeskletLayerGetCapture(HWND hwndDesklet);

BOOL CALLBACK DeskletLayerSetImageClipRect(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL useBounds, RECT* bounds, int xScale, int yScale);

INT CALLBACK DeskletLockInvalidating(HWND hwndDesklet);
INT CALLBACK DeskletUnlockInvalidating(HWND hwndDesklet, BOOL redraw);
BOOL DeskletInvalidateRect(HWND hwndDesklet, RECT* rect, SIZE* scale, BOOL redraw);
HCURSOR CALLBACK DeskletSetCursor(HWND hwndDesklet, HCURSOR hCursor);


void  CALLBACK DeskletLayerSetImageFileW(HWND hwndDesklet, unsigned int id, BOOL redraw, const WCHAR* filename);
Bitmap* CALLBACK DeskletLoadGDIPlusImageW(const WCHAR *szImage);

// UNICODE variants
void CALLBACK DeskletGetRelativeFolderW(HWND hwndDesklet, WCHAR *szFolder);
void CALLBACK DeskletGetRootFolderW(HWND hwndDesklet,WCHAR *szFolder);

// old 1.1 functions
void CALLBACK DeskletRedraw(HWND hwndDesklet);


// 2.0
unsigned int CALLBACK DeskletLayerAddEx(HWND hwndDesklet, const char* name, const char* nameOfParent, DWORD flags);


BOOL CALLBACK DeskletLayerGetImageClipRect(HWND hwndDesklet, unsigned int id, BOOL* useClipping, RECT* clipRect,
										int* xScale, int* yScale);
BOOL CALLBACK DeskletLayerGetPlacement(HWND hwndDesklet, unsigned int id, BOOL* usePlacement, RECT* placementRect, 
									int* xScale, int* yScale);
BOOL CALLBACK DeskletLayerGetIsNotMovable(HWND hwndDesklet, unsigned int id);
BOOL CALLBACK DeskletLayerGetIsClickThrough(HWND hwndDesklet, unsigned int id);
BOOL CALLBACK DeskletLayerGetNoAlphaBlendedHitTesting(HWND hwndDesklet, unsigned int id);
BOOL CALLBACK DeskletLayerRemoveAll(HWND hwndDesklet, BOOL redraw);
BYTE CALLBACK DeskletLayerGetAlpha(HWND hwndDesklet, unsigned int id);
BOOL  CALLBACK DeskletLayerGetGolorMatrix(HWND hwndDesklet, unsigned int id, ColorMatrix* matrix);
INT CALLBACK DeskletLayerGetHue(HWND hwndDesklet, unsigned int id);
int CALLBACK DeskletLayerGetRotation(HWND hwndDesklet, unsigned int id);
int CALLBACK DeskletLayerGetSaturation(HWND hwndDesklet, unsigned int id);
BOOL CALLBACK DeskletLayerGetVisibility(HWND hwndDesklet, unsigned int id);
BOOL CALLBACK DeskletLayerIsLayerVisible(HWND hwndDesklet, unsigned int id);
unsigned int CALLBACK DeskletLayerGetParent(HWND hwndDesklet, unsigned int id);


BOOL CALLBACK DeskletLayerSetToolTipText(HWND hwndDesklet, unsigned int id, const WCHAR* text);