// ResourceCompiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

HANDLE _consoleHandle = NULL;

void print(const WCHAR* txt)
{
	if(NULL == txt)
		return;

	if(NULL == _consoleHandle)
		_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if(_consoleHandle != NULL)
	{
		DWORD len = static_cast<DWORD>(wcslen(txt));
		DWORD written = 0;
		WriteConsoleW(_consoleHandle, reinterpret_cast<CONST VOID*>(txt), len, &written, NULL);
	}
}

void println(const WCHAR* txt)
{
	if(NULL == txt)
		return;

	print(txt);
	print(L"\n");
}

void pause()
{
	HANDLE consoleInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if(consoleInputHandle != NULL)
	{
		println(L"press any key to to continue...");
		SetConsoleMode(consoleInputHandle, ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		FlushConsoleInputBuffer(consoleInputHandle);
		WaitForSingleObject(consoleInputHandle, INFINITE);
	}
}

void error(const WCHAR* msg)
{
	println(msg);
	pause();
	exit(0);
}

void InsertResourceDataFromFile(HANDLE hUpdate, const WCHAR* fname, const WCHAR* type, const WCHAR* name, WORD lang)
{
	HANDLE hFile = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
		error(L"could not open file.");

	DWORD fileSize = GetFileSize(hFile, NULL);
	HLOCAL hBuffer = LocalAlloc(LMEM_FIXED, fileSize);
	if(NULL == hBuffer)
		error(L"could not allocate enough memory");

	DWORD numRead = 0;
	BOOL bRes = ReadFile(hFile, reinterpret_cast<LPVOID>(hBuffer), fileSize, &numRead, NULL);
	if(!bRes || numRead != fileSize)
		error(L"could not read data from file");

	CloseHandle(hFile);

	bRes = UpdateResource(hUpdate, type, name, lang, reinterpret_cast<LPVOID>(hBuffer), fileSize);
	if(!bRes)
		error(L"FATAL: updating a resource failed.");

	hBuffer = LocalFree(hBuffer);

}

WCHAR** ReadLinesAndSplit(const WCHAR* fname, DWORD* numEntries, DWORD* numEntriesAllocated)
{
	HANDLE hFile = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
		error(L"could not open file.");

	DWORD fileSize = GetFileSize(hFile, NULL);
	HLOCAL hBuffer = LocalAlloc(LMEM_FIXED, fileSize+2);

	DWORD numRead = 0;
	BOOL bRes = ReadFile(hFile, reinterpret_cast<LPVOID>(hBuffer), fileSize, &numRead, NULL);
	if(!bRes || numRead != fileSize)
		error(L"could not read data from file");

	CloseHandle(hFile);

	((BYTE*)hBuffer)[fileSize] = '\0';
	((BYTE*)hBuffer)[fileSize+1] = '\0';

	DWORD numLines = 0;
	BYTE* buf = (BYTE*)hBuffer;
	for(DWORD i = 0; i < fileSize; ++i)
	{
		if(buf[i] == '\n')
			numLines++;
	}

	WCHAR* wText = new WCHAR[fileSize];
	MultiByteToWideChar(GetACP(), 0, (const CHAR*)hBuffer, -1, wText,fileSize);
	WCHAR* txtPtr = wText;

	LocalFree(hBuffer);

	DWORD realNumEntries=0;
	WCHAR** entries = new WCHAR*[numLines*2];
	for(DWORD i = 0; i < numLines*2;++i)
		entries[i] = NULL;

	BOOL okay=TRUE;
	while(okay)
	{
		WCHAR* nextEntry = wcsstr(txtPtr,L"\n");
		WCHAR* line = txtPtr;


		if(nextEntry != NULL)
		{
			txtPtr = nextEntry+1;
			*nextEntry = L'\0';
		}
		else
			okay = FALSE;

		WCHAR* sign =  wcsstr(line, L"=");
		if(sign != NULL)
		{
			*sign = 0;
			sign++;
			size_t len = (size_t)wcslen(line)+1;
			entries[realNumEntries*2] = new WCHAR[len];
			wcscpy_s(entries[realNumEntries*2],len, line);

			WCHAR* breakch = wcsstr(sign, L"\r");
			if(breakch != NULL)
				*breakch = L'\0';
			len = (size_t)wcslen(sign)+1;
			entries[realNumEntries*2+1] = new WCHAR[len];
			wcscpy_s(entries[realNumEntries*2+1],len, sign);

			realNumEntries++;
		}
	}
	

	delete[] wText;

	if(numEntriesAllocated != NULL)
		*numEntriesAllocated = numLines*2;

	if(numEntries != NULL)
		*numEntries = realNumEntries;

	return entries;
	
}

extern BOOL AddIconFileResource(HANDLE hUpdate, const WCHAR* fname, WORD groupID, WORD lang);
extern BOOL WriteInfoResource(HANDLE hUpdate, WCHAR** entries, SIZE_T numEntries, WORD lang);

int _tmain(int argc, _TCHAR* argv[])
{
	

	// print a fancy header
	println(L"AveTool Resource Compiler");
	println(L"(c) Andreas Verhoeven, 2007");
	println(L"");

	if(argc < 7)
	{
		println(L"Usage: AveResC <template> <output> <package> <policies> <icon> <version>");
		return 0;
	}

	WCHAR* inputFile	= argv[1];//L"avedesk.template";
	WCHAR* outputFile	= argv[2];//L"desklet.exe";
	WCHAR* packageFile	= argv[3];//L"package.zip";
	WCHAR* policiesFile	= argv[4];//L"policies.xml";
	WCHAR* iconFile		= argv[5];//L"icon.ico";
	WCHAR* versionFile	= argv[6];//L"version.txt";
	print(L"output:\t\t"); println(outputFile);
	print(L"package:\t");println(packageFile);
	print(L"policies:\t");println(policiesFile);
	print(L"icon:\t\t");println(iconFile);
	print(L"version:\t\t");println(versionFile);

	// create new .exe from template avedesk.exe
	BOOL bRes = CopyFile(inputFile, outputFile, FALSE);
	if(!bRes)
	{
		bRes = DeleteFile(outputFile);
		if(!bRes)
			error(L"FATAL: output file is locked or still in use.");

		bRes = CopyFile(inputFile, outputFile, FALSE);
	}
	if(!bRes)
		error(L"FATAL: could not create output file.");

	// open new .exe for resource modification
	HANDLE hUpdate = BeginUpdateResourceW(outputFile, FALSE);
	if(hUpdate == NULL)
		error(L"FATAL: could not load avedesk.exe template.");

	
	println(L"\nstarting build process:\n");

	WORD lang = 1043; //MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)

	// update packageData resource
	InsertResourceDataFromFile(hUpdate, packageFile, L"PACKAGEDATA",MAKEINTRESOURCE(181), lang);
	println(L"successfully inserted package resource!");

	// update policies resource
	InsertResourceDataFromFile(hUpdate, policiesFile, L"POLICIES",MAKEINTRESOURCE(188), lang);
	println(L"successfully inserted policy file!");

	// update main icon resource
	AddIconFileResource(hUpdate, iconFile, 139, lang);
	println(L"successfully inserted icon!");

	// updating version resource
	DWORD numEntries = 0;
	DWORD numEntriesAllocated = 0;
	WCHAR** entries = ReadLinesAndSplit(versionFile, &numEntries, &numEntriesAllocated);
	WriteInfoResource(hUpdate, entries, numEntries, lang);
	println(L"successfully updated version resources!");

	//for(DWORD i=0; i < numEntriesAllocated; ++i)
	//	delete[] entries[i];

	//delete[] entries;


	// and free the .exe
	EndUpdateResource(hUpdate, FALSE);

	println(L"\ndone!");
	//pause();

	return 0;
}

