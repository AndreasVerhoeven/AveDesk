#ifndef AVE_IMGLIBPP_H_2220391
#define AVE_IMGLIBPP_H_2220391

#if defined(EXPORT) 
#undef EXPORT
#endif
#define EXPORT	extern "C" __declspec(dllexport)

#include <windows.h>

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


EXPORT BOOL __stdcall OpenIconFile(PICONFILE pIconFile);
EXPORT BOOL __stdcall WriteIconEntry(PICONFILE pIconFile, PCREATEICONENTRY pInfo);
EXPORT BOOL __stdcall CloseIconFile(PICONFILE pIconFile);


#endif//AVE_IMGLIBPP_H_2220391