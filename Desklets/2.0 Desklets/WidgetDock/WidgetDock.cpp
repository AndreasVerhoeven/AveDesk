///
/// WidgetDock Example, by Andreas Verhoeven.
/// For AveDesk 2.0
///

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "AvailableDesklets.h"
#include "DeskletSDK.h"
#include "Desklet.h"

void CALLBACK OnGetInformation(char *szName,char *szAuthor,int *version,char *szNotes)
{
	strcpy(szName, "WidgetDock");
	strcpy(szAuthor, "Ave");
	strcpy(szNotes, "Needs AveDesk 2.0 to run");
	*version = 100;
}

Desklet* CALLBACK OnCreate(HWND hwndDesklet,HINSTANCE hInstance,const char *szIni,const char *szIniGroup)
{
	Desklet* d = new Desklet(hwndDesklet, hInstance);
	AvailableDeskletsAPI::InitializeAvailableDeskletsAPI(NULL);
	
	SIZE s = {36, 36};
	::DeskletSetSize(d->hwnd, &s);
	RECT placement = {0, 0, s.cx, s.cy};
	::DeskletLayerSetPlacement(hwndDesklet, 0, FALSE, TRUE, &placement, s.cx, s.cy);
	::DeskletLayerSetImageFileW(d->hwnd, 0, TRUE, d->GetImagePath(L"cross.png").c_str());
	::DeskletSetLabel(d->hwnd, "");
	::DeskletSetClosePoint(d->hwnd, NULL, 0, 0);

	d->SetupDockWindow();

	return d;
}

void CALLBACK OnDestroy(Desklet *d,HWND hwndDesklet)
{
	delete d;
	d = NULL;
}

BOOL CALLBACK OnLeftButtonClick(Desklet *d,POINT *ptCursor,SIZE *sizeDesklet)
{
	d->ToggleDock();
	return TRUE;
}

BOOL CALLBACK OnVisibilityNotify(Desklet *d, BOOL becomesVisible, DWORD flags)
{
	if(d->dockVisible && !becomesVisible)
		d->ToggleDock();

	return TRUE;
}