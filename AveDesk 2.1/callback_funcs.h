// filename:		callback_funcs.h
// Description:		Prototypes of callback functions not associated with a 
//					particular docklet instance
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!
//
//

#ifndef CALLBACK_FUNCS_H__303
#define CALLBACK_FUNCS_H__303

#include <gdiplus.h>
#include <windows.h>

struct TabPage
{
public:
	char*		title;			// title of the page
	int			titleLength;	// length of title
	char*		resourceName;	// resource name
	HINSTANCE	hInstance;		// instance where resource is located
	DLGPROC		dlgProc;		// callback dialog proc
};

struct PropertyDialogStruct
{
	LPARAM		lParam;		// reserved
	UINT		pagesCount;	// number of pages
	TabPage*	pages;		// array of pages
};

struct WrapperTabPage
{
public:
	DLGPROC	dlgProc;
	LPARAM	lParam;
};

Bitmap* CALLBACK DockletLoadGDIPlusImage(const char *szImage);
Bitmap* CALLBACK DeskletLoadGDIPlusImage(const char *szImage);
void CALLBACK DeskletSetImageFile(HWND hwndDesklet,const char *szImage);

void CALLBACK DeskletDefaultConfigDialog(HWND hwndDesklet,PropertyDialogStruct* pds);
BOOL CALLBACK DeskletRemove(HWND hwndDesklet, BOOL Ask);
void CALLBACK DuplicateDesklet(HWND hwndDesklet, BOOL Ask, HWND *phwndTarget = NULL);

BOOL CALLBACK ULW(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);

LONG CALLBACK DummyProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LONG CALLBACK LabelProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

#endif//CALLBACK_FUNCS_H__303