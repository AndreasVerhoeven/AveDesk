// SubLayersTest.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

struct Desklet
{
	HWND hwnd;
	HINSTANCE dll;
	int alpha;
};

void __stdcall OnGetInformation(char *szName,char *szAuthor,int *version,char *szNotes)
{
	strcpy(szName, "SublayersTest");
	strcpy(szAuthor, "AveDesk 2.0");
	strcpy(szNotes, "A test desklet");
	*version = 200;
}

Desklet* __stdcall OnCreate(HWND hwnd, HINSTANCE hInst, const char* ini, const char* group)
{
	Desklet* d = new Desklet;
	d->hwnd = hwnd;
	d->dll = hInst;
	d->alpha = 0;

	UINT contLayer = DeskletLayerAddEx(hwnd, "BigMain", NULL, LAYER_IS_CONTAINER);
	UINT childA = DeskletLayerAddEx(hwnd, "BigMain.ChildA", "BigMain", 0);
	UINT contLayerB = DeskletLayerAddEx(hwnd, "SmallMain", "BigMain", LAYER_IS_CONTAINER);
	UINT childB = DeskletLayerAddEx(hwnd, "SmallMain.ChildB", "SmallMain", 0);
	UINT childC = DeskletLayerAddEx(hwnd, "SmallMain.ChildC", "SmallMain", 0);

	Bitmap* bmpA = new Bitmap(100, 50);
	Bitmap* bmpB = new Bitmap(100, 50);
	Bitmap* bmpC = new Bitmap(50, 25);
	Graphics gA(bmpA);
	Graphics gB(bmpB);
	Graphics gC(bmpC);
	SolidBrush red(Color(255,255,0,0));
	SolidBrush green(Color(255,0,255,0));
	SolidBrush blue(Color(255, 0, 0, 255));
	gA.FillRectangle(&red, 0, 0, 100, 100);
	gB.FillRectangle(&blue, 0, 0, 100, 100);
	gC.FillRectangle(&green, 0, 0, 100, 100);

	DeskletLayerSetImage(hwnd, childA, FALSE, bmpA, TRUE);
	DeskletLayerSetImage(hwnd, childB, FALSE, bmpB, TRUE);
	DeskletLayerSetImage(hwnd, childC, FALSE, bmpC, TRUE);

	RECT rcA = {0,0, 100, 50};
	RECT rcContB = {0,50, 100, 100};
	RECT rcB = {0,0, 100, 100};
	RECT rcC = {50,50, 100, 100};

	DeskletLayerSetPlacement(hwnd, childA, FALSE, TRUE, &rcA, 100,100);

	DeskletLayerSetPlacement(hwnd, contLayerB, FALSE, TRUE, &rcContB, 100, 100);
	DeskletLayerSetPlacement(hwnd, childB, FALSE, TRUE, &rcB, 100,100);
	DeskletLayerSetPlacement(hwnd, childC, FALSE, TRUE, &rcC, 100,100);


	RECT rcClip = {0, 40, 100, 100};
	::DeskletLayerSetImageClipRect(hwnd, contLayerB, FALSE, TRUE, &rcClip, 100, 100);
	DeskletLayerSetAlpha(hwnd, childC, FALSE, 150);

	//DeskletLayerSetRotation(hwnd, contLayerB, TRUE, 15);

	return d;
}

void __stdcall OnDestroy(Desklet* d, HWND)
{
	delete d;
	d = NULL;
}

BOOL __stdcall OnLeftButtonClick(Desklet* d,POINT *ptCursor,SIZE *sizeDesklet)
{
	d->alpha += 5;
	UINT id = DeskletLayerGetLayer(d->hwnd, "SmallMain", NULL);
	DeskletLayerSetAlpha(d->hwnd, id, TRUE, d->alpha);
	return TRUE;
}

BOOL __stdcall OnLayerMouseMove(Desklet* d, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize)
{
	DeskletSetLabel(d->hwnd, layerName);
	return TRUE;
}
