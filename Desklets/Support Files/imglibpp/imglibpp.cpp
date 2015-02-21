#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

using namespace Gdiplus;

#include "imglibpp.h"

BOOL __stdcall GetBitmapSize(HBITMAP hBitmap, SIZE* size)
{
	if(NULL == hBitmap)
		return FALSE;

	if(NULL == size)
		return FALSE;

	BITMAP bitmap = {0};
	if(GetObject(hBitmap,sizeof(bitmap), &bitmap))
	{

		size->cx = bitmap.bmWidth;
		size->cy = bitmap.bmHeight;


		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

Bitmap* __stdcall HBitmapToCppGDIPlusBitmap(HBITMAP hBitmap)
{
	// pre:  hBitmap is not NULL.
	// post: A CPP Gdiplus::Bitmap object created  from the hBitmap has been 
	//		 returned.

	// note: caller is responsible for freeing the allocated memory.

	if(NULL == hBitmap)
		return NULL;

	SIZE s = {0};
	if ( GetBitmapSize(hBitmap,&s) )
	{
		Bitmap* retBmp = new Bitmap(s.cx, s.cy);

		Rect rc(0, 0, s.cx, s.cy);
		BitmapData data;
		retBmp->LockBits(&rc,ImageLockModeWrite,PixelFormat32bppARGB,&data);

		GetBitmapBits(hBitmap,s.cx * s.cy * 4,data.Scan0);
		retBmp->UnlockBits(&data);

		return retBmp;
	}
	else
	{
		return NULL;
	}
}


long BytesPerScanLine(long PixelsPerScanLine, long BitsPerPixel, long Alignment)
{
	Alignment--;
	long res  = ((PixelsPerScanLine * BitsPerPixel) + Alignment) & ~Alignment;
	res /= 8;
	return res;
}


void InitializeBitmapInfoHeader(HBITMAP bitmap,BITMAPINFOHEADER* BI, long Colors)
{
	DIBSECTION DS = {0};
	long Bytes = {0};

	DS.dsBmih.biSize = 0;
	Bytes = GetObject(bitmap,sizeof(DS),&DS);

	if( (Bytes >= (sizeof(DS.dsBm) + sizeof(DS.dsBmih)) ) && 
			(DS.dsBmih.biSize >= DWORD(sizeof(DS.dsBmih))) )
	{
		*BI = DS.dsBmih;
	}
	else
	{
		memset(BI,0,sizeof(BITMAPINFOHEADER));
		BI->biSize = sizeof(BITMAPINFOHEADER);
		BI->biWidth = DS.dsBm.bmWidth;
		BI->biHeight = DS.dsBm.bmHeight;
	}

	if(Colors==2)
		BI->biBitCount = 1;
	else if(Colors >= 3 && Colors <= 16)
	{
		BI->biBitCount = 4;
		BI->biClrUsed = Colors;
	}
	else if(Colors >= 17 && Colors <= 256)
	{
		BI->biBitCount = 8;
		BI->biClrUsed = Colors;
	}
	else
	{
		BI->biBitCount = DS.dsBm.bmBitsPixel * DS.dsBm.bmPlanes;
	}
	BI->biPlanes = 1;
	if(BI->biClrImportant > BI->biClrUsed)
		BI->biClrImportant = BI->biClrUsed;
	if(BI->biSizeImage == 0)
		BI->biSizeImage = BytesPerScanLine(BI->biWidth,BI->biBitCount,32) * abs(BI->biHeight);
}

bool InternalGetDIB(HBITMAP Bitmap,HPALETTE Palette,BITMAPINFO* BitmapInfo,BYTE* Bits,long Colors)
{
	bool res = false;

	InitializeBitmapInfoHeader(Bitmap,(BITMAPINFOHEADER*)BitmapInfo,Colors);

	HPALETTE OldPal = {0};
	HDC dc = CreateCompatibleDC(0);

	if(Palette)
	{
		OldPal = SelectPalette(dc,Palette,FALSE);
		RealizePalette(dc);
	}
	res = GetDIBits(dc,Bitmap,0,((BITMAPINFOHEADER*)BitmapInfo)->biHeight,Bits,BitmapInfo,DIB_RGB_COLORS) != 0;

	if(OldPal)
		SelectPalette(dc,OldPal,FALSE);

	DeleteDC(dc);

	return res;
}


void InternalGetDIBSizes(HBITMAP Bitmap,DWORD& InfoHeaderSize, DWORD& ImageSize, long Colors)
{
	BITMAPINFOHEADER BI = {0};
	InitializeBitmapInfoHeader(Bitmap,&BI,Colors);
	if(BI.biBitCount > 8)
	{
		InfoHeaderSize = sizeof(BITMAPINFOHEADER);
		if((BI.biCompression & BI_BITFIELDS) != 0 )
			InfoHeaderSize += 12;
	}
	else
	{
		if(BI.biClrUsed == 0)
			InfoHeaderSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << BI.biBitCount);
		else
			InfoHeaderSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * BI.biClrUsed;
	}
	ImageSize = BI.biSizeImage;
}

struct CursorOrIcon
{
	WORD Reserved, wType, Count;
};

struct IconRect
{
	BYTE Width, Height;
	WORD Colors, Reserved1, Reserved2;
	long DIBSize, DIBOffset;
};


DWORD WriteIcon(HANDLE f, HBITMAP hColor, HBITMAP hMask, long& Length)
{
	ICONINFO iconInfo = {0};
	DWORD MonoInfoSize=0, ColorInfoSize=0;
	DWORD MonoBitsSize=0, ColorBitsSize=0;
	BYTE *MonoInfo=0, *MonoBits=0, *ColorInfo=0, *ColorBits=0;

	CursorOrIcon CI = {0};
	IconRect List = {0};

	long x = 0;
	void* p = 0;
	BYTE b = 0;

	InternalGetDIBSizes(hMask,MonoInfoSize,MonoBitsSize,2);
	InternalGetDIBSizes(hColor,ColorInfoSize,ColorBitsSize,0xFFFFFFFF);
	
	MonoInfo = new BYTE[MonoInfoSize];
	MonoBits = new BYTE[MonoBitsSize];
	ColorInfo= new BYTE[ColorInfoSize];
	ColorBits= new BYTE[ColorBitsSize];

	InternalGetDIB(hMask,NULL,(BITMAPINFO*)MonoInfo,MonoBits,2);
	InternalGetDIB(hColor,NULL,(BITMAPINFO*)ColorInfo,ColorBits,0xFFFFFFFF);

	Length = ColorInfoSize + ColorBitsSize + MonoBitsSize;

	CI.wType = 1;
	CI.Count = 1;

	BITMAPINFOHEADER* pBih = (BITMAPINFOHEADER*)ColorInfo;
	List.Width = (BYTE)pBih->biWidth;
	List.Height = (BYTE)pBih->biHeight;
	List.Colors = pBih->biPlanes * pBih->biBitCount;
	List.DIBSize = ColorInfoSize + ColorBitsSize + (pBih->biWidth * pBih->biHeight);
	List.DIBOffset = sizeof(CI) + sizeof(List);

	pBih->biHeight += pBih->biHeight; // for icons and cursors, height include mono bits
	//fwrite(ColorInfo,ColorInfoSize,1,f);
	//fwrite(ColorBits,ColorBitsSize,1,f);
	//fwrite(MonoBits,MonoBitsSize,1,f);
	DWORD dwWritten(0);
	WriteFile(f,ColorInfo,ColorInfoSize,&dwWritten,0);
	WriteFile(f,ColorBits,ColorBitsSize,&dwWritten,0);
	WriteFile(f,MonoBits,MonoBitsSize,&dwWritten,0);

	delete[] ColorInfo;
	delete[] ColorBits;
	delete[] MonoInfo;
	delete[] MonoBits;

	DeleteObject(iconInfo.hbmColor);
	DeleteObject(iconInfo.hbmMask);

	return ColorBitsSize;
}

/*struct ICONFILE
{
	char   filename[MAX_PATH];
	UINT   numberOfEntries;
	LPVOID reserved1;
	LPVOID reserved2;
};*/
struct ICONHEADER
{
	WORD idReserved;
	WORD idType;
	WORD idCount;
};

struct ICONDIRENTRY
{
	BYTE bWidth;
	BYTE bHeight;
	BYTE bColorCount;
	BYTE bReserved;
	WORD wPlanes;
	WORD wBitCount;
	DWORD dwBytesInRes;
	DWORD dwImageOffset;
};

#define TYPE_ICON 1

BOOL __stdcall OpenIconFile(ICONFILE* pIconFile)
{
	if(!pIconFile)
		return FALSE;

	pIconFile->reserved1 = 0;

	//FILE* f = fopen(pIconFile->filename,"w+");
	HANDLE f = CreateFile(pIconFile->filename,GENERIC_WRITE | GENERIC_READ	,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE	| FILE_FLAG_RANDOM_ACCESS ,NULL);
	if(f==INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		return FALSE;
	}

	pIconFile->reserved1 = (LPVOID)f;

	ICONHEADER header = {0};
	header.idReserved = 0;
	header.idType = TYPE_ICON;
	header.idCount = (WORD)pIconFile->numberOfEntries;

	ICONDIRENTRY dummyEntry = {0};

	//fwrite(&header,sizeof(ICONHEADER),1,f);

	DWORD dwWritten(0);
	WriteFile(f,&header,sizeof(ICONHEADER),&dwWritten,0);

	for(int i=0;i<header.idCount;++i)
	{
		WriteFile(f,&dummyEntry,sizeof(ICONDIRENTRY),&dwWritten,0);
		//fwrite(&dummyEntry,sizeof(ICONDIRENTRY),1,f);
	}

	return TRUE;
}

/*struct CREATEICONENTRYINFO
{
	UINT entryNum;
	HBITMAP hColor;
	HBITMAP hMask;
	LONG lColors;
};*/

BOOL __stdcall WriteIconEntry(ICONFILE* pIconFile, CREATEICONENTRY* pInfo)
{
	if(!pInfo)
		return FALSE;

	if(!pIconFile)
		return FALSE;

	if(!pIconFile->reserved1)
		return FALSE;

	if(pInfo->entryNum >= pIconFile->numberOfEntries)
		return FALSE;

	if(!pInfo->hColor || !pInfo->hMask)
		return FALSE;


	HANDLE f = (HANDLE)pIconFile->reserved1;

	BITMAPINFOHEADER ih = {0};
	InitializeBitmapInfoHeader(pInfo->hColor,&ih,pInfo->lColors);

	LONG lHigh(0);
	DWORD dwNewFilePointer = SetFilePointer(f,0,&lHigh,FILE_END);

	ICONDIRENTRY entry = {0};
	entry.bColorCount = (BYTE) ih.biClrUsed;
	entry.bHeight = (BYTE) ih.biHeight;
	entry.bWidth = (BYTE) ih.biHeight;
	entry.wBitCount = ih.biBitCount;
	entry.wPlanes = ih.biPlanes;
	entry.dwImageOffset = dwNewFilePointer;

	long length(0);
	WriteIcon((HANDLE)pIconFile->reserved1,pInfo->hColor,pInfo->hMask,length);
	entry.dwBytesInRes = length;

	lHigh=0;
	SetFilePointer(f,sizeof(ICONHEADER)+pInfo->entryNum  * sizeof(ICONDIRENTRY),&lHigh,FILE_BEGIN);
	//fseek(f,sizeof(ICONHEADER)+pInfo->entryNum  * sizeof(ICONDIRENTRY),SEEK_SET);
	DWORD dwWritten(0);
	WriteFile(f,&entry,sizeof(ICONDIRENTRY),&dwWritten,0);

	return TRUE;
}

BOOL __stdcall CloseIconFile(ICONFILE* pIconFile)
{
	if(!pIconFile)
		return FALSE;

	if(!pIconFile->reserved1)
		return FALSE;

	//fclose((FILE*)pIconFile->reserved1);
	CloseHandle((HANDLE)pIconFile->reserved1);

	pIconFile->reserved1 = 0;

	return TRUE;
}
