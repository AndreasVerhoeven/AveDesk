// Header file for the draw plugin SDK
// created by Andreas Verhoeven
// part of AveDesk 1.1

#include "DrawPluginSDK.h"

typedef void (__stdcall *HDeskletRedrawDUMMY) (HWND);
HDeskletRedrawDUMMY HDeskletRedraw = (HDeskletRedrawDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletRedraw");

void __stdcall DeskletRedraw(HWND hwndDesklet)
{
	if(!HDeskletRedraw)return;

	HDeskletRedraw(hwndDesklet);
}

typedef void (__stdcall *HDeskletMoveDUMMY)(HWND,RECT*,BOOL);
HDeskletMoveDUMMY HDeskletMove = (HDeskletMoveDUMMY) GetProcAddress(GetModuleHandle(0),"DeskletMove");

void __stdcall DeskletMove(HWND hwndDesklet, RECT* r, BOOL redraw)
{
	if(!HDeskletMove)return;

	HDeskletMove(hwndDesklet,r,redraw);
}