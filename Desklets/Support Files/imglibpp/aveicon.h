///////////////////////////////////////////////////////////////////////////////
//
//		AveIcon library version 1
//
//		21 August 2004
//
//		(c) copyright Andreas Verhoeven 2004
//		
//		This library may be used freely, without a charge. The author is not
//		responsible for any possible damage that might occur by using this
//		library.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AVEICON_H_202039193111
#define AVEICON_H_202039193111

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

#endif//AVEICON_H_202039193111