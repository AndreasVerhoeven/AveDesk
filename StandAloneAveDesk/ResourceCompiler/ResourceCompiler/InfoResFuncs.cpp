#include "stdafx.h"

#pragma pack(push, 1)
struct VS_VERSION_INFO_HEADER 
{  
    WORD  wLength; 
    WORD  wValueLength; 
    WORD  wType; 
}; 
#pragma pack(pop)

#include "avestream.h"


DWORD PadStream(AveStream* stream)
{
	if(NULL == stream)
		return 0;

	BYTE padding[4] = {0};

	DWORD numPadded = (DWORD)stream->used % 4;
	AveWriteStream(stream,(LPVOID)padding,numPadded);
	return numPadded;
}

BOOL WriteInfoResource(HANDLE hUpdate, WCHAR** entries, SIZE_T numEntries, WORD lang)
{
	DWORD fileVersionMajor = MAKELONG(0,2);
	DWORD fileVersionMinor = MAKELONG(0,0);
	if(NULL == entries && numEntries > 0)
		return FALSE;

	DWORD offset = 0;
	DWORD numPadded = 0;
	AveStream stream = AveCreateStream(0xFFFF);

	VS_VERSION_INFO_HEADER hdr = {0,sizeof(VS_FIXEDFILEINFO),0};

	AveWriteStream(&stream, (LPVOID)&hdr, sizeof(hdr));
	AveWriteStream(&stream,(LPVOID)L"VS_VERSION_INFO", ((DWORD)wcslen(L"VS_VERSION_INFO")+1)*sizeof(WCHAR));
	numPadded += PadStream(&stream);
	
	VS_FIXEDFILEINFO fixedInfo = {0xFEEF04BD,
									MAKELONG(0,1),
									fileVersionMajor, fileVersionMinor,
									fileVersionMajor, fileVersionMinor,
									0,0,
									VOS_NT_WINDOWS32,
									VFT_APP, 0,
									0,0
								};
	
	AveWriteStream(&stream, (LPVOID)&fixedInfo, sizeof(fixedInfo));
	numPadded += PadStream(&stream);

	DWORD strOffset = (DWORD)stream.used;
	DWORD numStrDataPadded = 0;
	

	VS_VERSION_INFO_HEADER strhdr = {0,0,0};
	AveWriteStream(&stream, (LPVOID)&strhdr, sizeof(strhdr));
	AveWriteStream(&stream,(LPVOID)L"StringFileInfo", ((DWORD)wcslen(L"StringFileInfo")+1)*sizeof(WCHAR));
	numStrDataPadded += PadStream(&stream);

	DWORD langOffset = (DWORD)stream.used;
	DWORD numLangPadded = 0;
	VS_VERSION_INFO_HEADER langhdr = {0,0,1};
	AveWriteStream(&stream, (LPVOID)&langhdr, sizeof(langhdr));
	AveWriteStream(&stream,(LPVOID)L"000004B0", ((DWORD)wcslen(L"000004B0")+1)*sizeof(WCHAR));
	numLangPadded += PadStream(&stream);

	for(SIZE_T i = 0; i < numEntries; ++i)
	{
		DWORD offset = (DWORD)stream.used;
		DWORD padded = 0;
		VS_VERSION_INFO_HEADER hdr = {0,0,1};
		AveWriteStream(&stream, (LPVOID)&hdr, sizeof(hdr));

		const WCHAR* key = entries[i*2];
		const WCHAR* val = entries[i*2+1];
		
		DWORD keylen = ((DWORD)wcslen(key)+1)*sizeof(WCHAR);
		DWORD vallen = ((DWORD)wcslen(val)+1)*sizeof(WCHAR);
		AveWriteStream(&stream, (LPVOID)key, keylen);
		padded += PadStream(&stream);
		AveWriteStream(&stream, (LPVOID)val, vallen);
		DWORD datalen = vallen;

		padded += PadStream(&stream);

		BYTE* hdrptr = ((BYTE*)stream.hBuffer) + offset;
		hdr.wLength = (WORD)((DWORD)stream.used - offset);
		hdr.wValueLength = (WORD)datalen;

		memcpy((void*)hdrptr, (void*)&hdr, sizeof(hdr));

		numLangPadded += padded;
	}

	BYTE* hdrptr = ((BYTE*)stream.hBuffer) + langOffset;
	langhdr.wLength = (WORD)((DWORD)stream.used - langOffset);
	memcpy((void*)hdrptr, (void*)&langhdr, sizeof(langhdr));

	numStrDataPadded += numLangPadded;

	hdrptr = ((BYTE*)stream.hBuffer) + strOffset;
	strhdr.wLength = (WORD)((DWORD)stream.used - strOffset);
	memcpy((void*)hdrptr, (void*)&strhdr, sizeof(strhdr));

	numPadded += numStrDataPadded;

	DWORD varFileOffset = (DWORD)stream.used;
	DWORD varFileNumPadded = 0;
	VS_VERSION_INFO_HEADER varfilehdr = {0,0,1};
	AveWriteStream(&stream, (LPVOID)&varfilehdr, sizeof(varfilehdr));
	AveWriteStream(&stream,(LPVOID)L"VarFileInfo", ((DWORD)wcslen(L"VarFileInfo")+1)*sizeof(WCHAR));
	varFileNumPadded += PadStream(&stream);

	
	VS_VERSION_INFO_HEADER translationhdr = {0x24,0x04,0};
	AveWriteStream(&stream, (LPVOID)&translationhdr, sizeof(translationhdr));
	AveWriteStream(&stream,(LPVOID)L"Translation", ((DWORD)wcslen(L"Translation")+1)*sizeof(WCHAR));
	varFileNumPadded += PadStream(&stream);

	BYTE translationData[4] = {0, 0, 0xB0, 0x04};
	AveWriteStream(&stream,(LPVOID)translationData, 4);
	varFileNumPadded += PadStream(&stream);

	hdrptr = ((BYTE*)stream.hBuffer) + varFileOffset;
	varfilehdr.wLength = (WORD)((DWORD)stream.used - varFileOffset);
	memcpy((void*)hdrptr, (void*)&varfilehdr, sizeof(varfilehdr));

	numPadded += varFileNumPadded;
	
	hdrptr = ((BYTE*)stream.hBuffer) + offset;
	hdr.wLength = (WORD)((DWORD)stream.used - offset);
	memcpy((void*)hdrptr, (void*)&hdr, sizeof(hdr));

	UpdateResource(hUpdate, RT_VERSION, MAKEINTRESOURCE(1),lang, (LPVOID)stream.hBuffer, (DWORD)stream.used);

	AveFreeStream(&stream);

	return TRUE;

}