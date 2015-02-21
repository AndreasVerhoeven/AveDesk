// Zipper.cpp: implementation of the CZipper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Zipper.h"

#include "zlib\zip.h"
#include "zlib\iowin32.h"
#include "..\shared\filemisc.h"

#pragma warning(disable:4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const UINT BUFFERSIZE = 2048;

CZipper::CZipper(const char* szFilePath, const char* szRootFolder, bool bAppend) : m_uzFile(0)
{
	CloseZip();

	if (szFilePath)
		OpenZip(szFilePath, szRootFolder, bAppend);
}

CZipper::~CZipper()
{
	CloseZip();
}

bool CZipper::CloseZip()
{
	int nRet = m_uzFile ? zipClose(m_uzFile, NULL) : ZIP_OK;

	m_uzFile = NULL;
	m_szRootFolder[0] = 0;
	ZeroMemory(&m_info, sizeof(m_info));

	return (nRet == ZIP_OK);
}

void CZipper::GetFileInfo(Z_FileInfo& info)
{
	info = m_info;
}

// simple interface
bool CZipper::ZipFile(const char* szFilePath)
{
	// make zip path
	char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
	_splitpath(szFilePath, szDrive, szFolder, szName, NULL);

	char szZipPath[MAX_PATH];
	_makepath(szZipPath, szDrive, szFolder, szName, "zip");

	CZipper zip;

	if (zip.OpenZip(szZipPath, false))
		return zip.AddFileToZip(szFilePath, false);

	return FALSE;
}
	
bool CZipper::ZipFolder(const char* szFilePath, bool bIgnoreFilePath)
{
	// make zip path
	char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
	_splitpath(szFilePath, szDrive, szFolder, szName, NULL);

	char szZipPath[MAX_PATH];
	_makepath(szZipPath, szDrive, szFolder, szName, "zip");

	CZipper zip;

	if (zip.OpenZip(szZipPath, FALSE))
		return zip.AddFolderToZip(szFilePath, bIgnoreFilePath);

	return FALSE;
}
	
// works with prior opened zip
bool CZipper::AddFileToZip(const char* szFilePath, bool bIgnoreFilePath)
{
	if (!m_uzFile)
		return FALSE;

	// we don't allow paths beginning with '..\' because this would be outside
	// the root folder
	if (!bIgnoreFilePath && strstr(szFilePath, "..\\") == szFilePath)
		return false;

	//TRACE ("CZipper::AddFileToZip(%s)\n", szFilePath);

	bool bFullPath = (strchr(szFilePath, ':') != NULL);

	// if the file is relative then we need to append the root before opening
	char szFullFilePath[MAX_PATH];
	
	lstrcpyA(szFullFilePath, szFilePath);
	PrepareSourcePath(szFullFilePath);

	// if the file is a fullpath then remove the root path bit
	char szFileName[MAX_PATH] = "";

	if (bIgnoreFilePath)
	{
		char szName[_MAX_FNAME], szExt[_MAX_EXT];
		_splitpath(szFilePath, NULL, NULL, szName, szExt);

		_makepath(szFileName, NULL, NULL, szName, szExt);
	}
	else if (bFullPath)
	{
		// check the root can be found
		if (0 != _strnicmp(szFilePath, m_szRootFolder, lstrlenA(m_szRootFolder)))
			return false;

		// else
		lstrcpyA(szFileName, szFilePath + lstrlenA(m_szRootFolder));
	}
	else // relative path
	{
		// if the path begins with '.\' then remove it
		if (strstr(szFilePath, ".\\") == szFilePath)
			lstrcpyA(szFileName, szFilePath + 2);
		else
			lstrcpyA(szFileName, szFilePath);
	}

	// save file attributes
	zip_fileinfo zfi;

	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributesA(szFilePath);
	
	// save file time
	SYSTEMTIME st;

	GetLastModified(szFullFilePath, st, TRUE);

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;
	
	// load input file
	HANDLE hInputFile = ::CreateFileA(szFullFilePath, 
									GENERIC_READ,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_READONLY,
									NULL);

	if (hInputFile == INVALID_HANDLE_VALUE)
		return FALSE;

	int nRet = zipOpenNewFileInZip(m_uzFile, 
									szFileName,
									&zfi, 
									NULL, 
									0,
									NULL,
									0, 
									NULL,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION);

	if (nRet == ZIP_OK)
	{
		m_info.nFileCount++;

		// read the file and output to zip
		char pBuffer[BUFFERSIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while (nRet == ZIP_OK && ::ReadFile(hInputFile, pBuffer, BUFFERSIZE, &dwBytesRead, NULL))
		{
			dwFileSize += dwBytesRead;

			if (dwBytesRead)
				nRet = zipWriteInFileInZip(m_uzFile, pBuffer, dwBytesRead);
			else
				break;
		}

		m_info.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip(m_uzFile);
	::CloseHandle(hInputFile);

	return (nRet == ZIP_OK);
}

bool CZipper::AddFileToZip(const char* szFilePath, const char* szRelFolderPath)
{
	if (!m_uzFile)
		return FALSE;

	// szRelFolderPath cannot contain drive info
	if (szRelFolderPath && strchr(szRelFolderPath, ':'))
		return FALSE;

	// if the file is relative then we need to append the root before opening
	char szFullFilePath[MAX_PATH];
	
	lstrcpyA(szFullFilePath, szFilePath);
	PrepareSourcePath(szFullFilePath);

	// save file attributes and time
	zip_fileinfo zfi;

	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributesA(szFilePath);
	
	// save file time
	SYSTEMTIME st;

	GetLastModified(szFullFilePath, st, TRUE);

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;

	// load input file
	HANDLE hInputFile = ::CreateFileA(szFullFilePath, 
									GENERIC_READ,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_READONLY,
									NULL);

	if (hInputFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// strip drive info off filepath
	char szName[_MAX_FNAME], szExt[_MAX_EXT];
	_splitpath(szFilePath, NULL, NULL, szName, szExt);

	// prepend new folder path 
	char szFileName[MAX_PATH];
	_makepath(szFileName, NULL, szRelFolderPath, szName, szExt);

	// open the file in the zip making sure we remove any leading '\'
	int nRet = zipOpenNewFileInZip(m_uzFile, 
									szFileName + (szFileName[0] == '\\' ? 1 : 0),
									&zfi, 
									NULL, 
									0,
									NULL,
									0, 
									NULL,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION);

	if (nRet == ZIP_OK)
	{
		m_info.nFileCount++;

		// read the file and output to zip
		char pBuffer[BUFFERSIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while (nRet == ZIP_OK && ::ReadFile(hInputFile, pBuffer, BUFFERSIZE, &dwBytesRead, NULL))
		{
			dwFileSize += dwBytesRead;

			if (dwBytesRead)
				nRet = zipWriteInFileInZip(m_uzFile, pBuffer, dwBytesRead);
			else
				break;
		}

		m_info.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip(m_uzFile);
	::CloseHandle(hInputFile);

	return (nRet == ZIP_OK);
}

bool CZipper::AddFolderToZip(const char* szFolderPath, bool bIgnoreFilePath, bool recursive)
{
	if (!m_uzFile)
		return FALSE;

	m_info.nFolderCount++;

	// if the path is relative then we need to append the root before opening
	char szFullPath[MAX_PATH];
	
	lstrcpyA(szFullPath, szFolderPath);
	PrepareSourcePath(szFullPath);

	// always add folder first
	// save file attributes
	zip_fileinfo zfi;
	
	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributesA(szFullPath);
	
	SYSTEMTIME st;
	
	GetLastModified(szFullPath, st, TRUE);
	
	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;
	
	// if the folder is a fullpath then remove the root path bit
	char szFolderName[MAX_PATH] = "";
	
	if (bIgnoreFilePath)
	{
		_splitpath(szFullPath, NULL, NULL, szFolderName, NULL);
	}
	else
	{
		// check the root can be found
		if (0 != _strnicmp(szFullPath, m_szRootFolder, lstrlenA(m_szRootFolder)))
			return false;
		
		// else
		lstrcpyA(szFolderName, szFullPath + lstrlenA(m_szRootFolder));
	}
	
	// folders are denoted by a trailing '\\'
	lstrcatA(szFolderName, "\\");
	
	// open the file in the zip making sure we remove any leading '\'
	int nRet = zipOpenNewFileInZip(m_uzFile, 
		szFolderName,
		&zfi, 
		NULL, 
		0,
		NULL,
		0, 
		NULL,
		Z_DEFLATED,
		Z_DEFAULT_COMPRESSION);
	
	zipCloseFileInZip(m_uzFile);

	if(!recursive)
		return TRUE;
	// build searchspec
	char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szName[_MAX_FNAME];
	_splitpath(szFullPath, szDrive, szFolder, szName, NULL);
	lstrcatA(szFolder, szName);

	char szSearchSpec[MAX_PATH];
	_makepath(szSearchSpec, szDrive, szFolder, "*", "*");

	WIN32_FIND_DATAA finfo;
	HANDLE hSearch = FindFirstFileA(szSearchSpec, &finfo);

	if (hSearch != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (finfo.cFileName[0] != '.') 
			{
				char szItem[MAX_PATH];
				_makepath(szItem, szDrive, szFolder, finfo.cFileName, NULL);
				
				if (finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					AddFolderToZip(szItem, bIgnoreFilePath);
				}
				else 
					AddFileToZip(szItem, bIgnoreFilePath);
			}
		} 
		while (FindNextFileA(hSearch, &finfo));
		
		FindClose(hSearch);
	}

	return TRUE;
}

// extended interface
bool CZipper::OpenZip(const char* szFilePath, const char* szRootFolder, bool bAppend)
{
	CloseZip();

	if (!szFilePath || !lstrlenA(szFilePath))
		return false;

	// convert szFilePath to fully qualified path 
	char szFullPath[MAX_PATH];

	if (!GetFullPathNameA(szFilePath, MAX_PATH, szFullPath, NULL))
		return false;

	// zipOpen will fail if bAppend is TRUE and zip does not exist
	if (bAppend && ::GetFileAttributesA(szFullPath) == 0xffffffff)
		bAppend = false;

	m_uzFile = zipOpen(szFullPath, bAppend ? 1 : 0);

	if (m_uzFile)
	{
		if (!szRootFolder)
		{
			char szDrive[_MAX_DRIVE], szFolder[MAX_PATH];
			_splitpath(szFullPath, szDrive, szFolder, NULL, NULL);

			_makepath(m_szRootFolder, szDrive, szFolder, NULL, NULL);
		}
		else if (lstrlenA(szRootFolder))
		{
			_makepath(m_szRootFolder, NULL, szRootFolder, "", NULL);
		}
	}

	return (m_uzFile != NULL);
}

void CZipper::PrepareSourcePath(char* szPath)
{
	bool bFullPath = (strchr(szPath, ':') != NULL);

	// if the file is relative then we need to append the root before opening
	if (!bFullPath)
	{
		char szTemp[MAX_PATH];
		lstrcpyA(szTemp, szPath);

		_makepath(szPath, NULL, m_szRootFolder, szTemp, NULL);
	}
}
