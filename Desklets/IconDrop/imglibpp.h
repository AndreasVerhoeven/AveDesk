#ifndef AVE_IMGLIBPP_H_2220391
#define AVE_IMGLIBPP_H_2220391

#if defined(EXPORT) 
#undef EXPORT
#endif
#define EXPORT	extern "C" __declspec(dllexport)

#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

using namespace Gdiplus;


typedef struct
{
	char   filename[MAX_PATH];
	UINT   numberOfEntries;
	LPVOID reserved1;
	LPVOID reserved2;
} ICONFILE, *PICONFILE;

typedef struct
{
	UINT entryNum;
	HBITMAP hColor;
	HBITMAP hMask;
	LONG lColors;
} CREATEICONENTRY, *PCREATEICONENTRY;


//EXPORT Bitmap* __stdcall HBitmapToCppGDIPlusBitmap(HBITMAP hBitmap);

BOOL __stdcall OpenIconFile(PICONFILE pIconFile);
BOOL __stdcall WriteIconEntry(PICONFILE pIconFile, PCREATEICONENTRY pInfo);
BOOL __stdcall CloseIconFile(PICONFILE pIconFile);


#endif//AVE_IMGLIBPP_H_2220391