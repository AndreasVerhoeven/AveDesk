#include "stdafx.h"

#include "avestream.h"


#pragma pack(push,1) 
typedef struct tagFileGroupItem 
{ BYTE bWidth; 
  BYTE bHeight; 
  BYTE bColorCount; 
  BYTE bReserved; 
  WORD wPlanes; WORD wBitCount; 
  DWORD dwSize; // item size in bytes 
  DWORD dwOffset; // start of item in the file 


} TFileGroupItem; 


typedef struct tagFileGroupHeader 
{ WORD wReserved; 
  WORD wType; // 1 for icons, 2 for cursors 
  WORD wCount; 
 // TFileGroupItem Items[1]; 


} TFileGroupHeader; 


typedef struct tagGroupItem 
{ BYTE bWidth; 
  BYTE bHeight; 
  BYTE bColorCount; 
  BYTE bReserved; 
  WORD wPlanes; 
  WORD wBitCount; 
  DWORD dwBytesInRes; 
  WORD wNameOrdinal; 


} TGroupItem; 


typedef struct tagGroupHeader 
{ WORD wReserved; 
  WORD wType; // 1 for icons, 2 for cursors 
  WORD wCount; 
  //TGroupItem Items[1]; 

} TGroupHeader; 
#pragma pack(pop)

BOOL AddIconFileResource(HANDLE hUpdate, const WCHAR* fname, WORD groupID, WORD lang)
{
	HANDLE hFile = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL);
	if(NULL == hFile)
		return FALSE;

	DWORD numRead=0;
	SIZE_T startIconId = 1;


	tagGroupHeader header = {0};
	AveStream stream = AveCreateStream(0xFFFF);
	ReadFile(hFile, &header, sizeof(header), &numRead, NULL);
	DWORD startOffset = sizeof(header);

	size_t numIcons = (size_t)header.wCount;
	tagFileGroupItem* fgItems = new tagFileGroupItem[numIcons];

	AveWriteStream(&stream, &header, sizeof(header));
	for(size_t i = 0; i <numIcons; ++i)
	{
		WORD ordinalName = (WORD)(i + startIconId);
		ReadFile(hFile, &fgItems[i], sizeof(tagFileGroupItem), &numRead, NULL);
		tagGroupItem groupitem = {fgItems[i].bWidth,
									fgItems[i].bHeight,
									fgItems[i].bColorCount,
									fgItems[i].bReserved,
									fgItems[i].wPlanes,
									fgItems[i].wBitCount,
									fgItems[i].dwSize,
									ordinalName
									};
		AveWriteStream(&stream, &groupitem, sizeof(groupitem));
	}

	for(size_t i = 0; i <numIcons; ++i)
	{
		WORD ordinalName = (WORD)(i + startIconId);
		DWORD len = fgItems[i].dwSize;
		SetFilePointer(hFile, fgItems[i].dwOffset, 0, FILE_BEGIN);
		BYTE* buffer = new BYTE[len];
		ReadFile(hFile, (LPVOID)buffer, len, &numRead, 0);
		UpdateResource(hUpdate,RT_ICON, MAKEINTRESOURCE(ordinalName), lang, (LPVOID)buffer, len);

		delete[] buffer;
	}

	delete[] fgItems;
	CloseHandle(hFile);

	UpdateResource(hUpdate,RT_GROUP_ICON, MAKEINTRESOURCE(groupID), lang, (LPVOID)stream.hBuffer, (DWORD)stream.used);

	AveFreeStream(&stream);

	return TRUE;
}
