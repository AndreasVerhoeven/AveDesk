// AveInstFileFetcher.cpp: implementation of the CAveInstFileFetcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstFileFetcher.h"

#include <wininet.h>
#pragma comment(lib,"wininet.lib")

#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstFileFetcher::CAveInstFileFetcher() :
	m_GoOn(true)
{

}

CAveInstFileFetcher::~CAveInstFileFetcher()
{

}

HRESULT CAveInstFileFetcher::FetchFileFromGUID(const std::wstring& guid,
	CAveInstProgressIndicator* indicator,
	const std::wstring& outputFile)
{
	HRESULT hRes = S_OK;

	if(guid.empty() || outputFile.empty())
		return (hRes = E_FAIL);

	// TODO: add code to fetch a file from a GUID

	return (hRes = E_FAIL);
}


HRESULT CAveInstFileFetcher::FetchFileFromURL(const std::wstring& url,
	CAveInstProgressIndicator* indicator,
	const std::wstring& outputFile)
{
	HRESULT hRes = S_OK;

	if(url.empty() || outputFile.empty())
		return (hRes = E_FAIL);

	if(indicator)
	{
		CString str;
		str.LoadString(IDS_CONNECTING);
		indicator->SetMessage(str.operator LPCTSTR());
	}

	HINTERNET hInternet = InternetOpen(L"AveDeskInstaller",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0L);
	if(!hInternet)
		return (hRes = E_FAIL);

	HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_EXISTING_CONNECT, 0);
	if(NULL == hUrl)
	{
		InternetCloseHandle(hInternet); // bleh, I want RAII classes, but too lazy to code them myself for wininet ;-)
		return (hRes = E_FAIL);
	}

	DeleteFile(outputFile.c_str() );

	if(indicator)
	{
		CString str;
		str.Format(IDS_DOWNLOADINGFILE,url.c_str(), L"?Kb", L"0Kb");
		indicator->SetMessage(str.operator LPCTSTR());
	}

	HANDLE hFile = CreateFile(outputFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,  FILE_ATTRIBUTE_NORMAL, NULL); 
	if(INVALID_HANDLE_VALUE == hFile)
	{
		InternetCloseHandle(hUrl);
		InternetCloseHandle(hInternet);
		return (hRes = E_FAIL);
	}

	const int bufferSize = 4096;
	DWORD bytesRead (0);
	BOOL bRes = TRUE;
	BYTE buffer[bufferSize];

	DWORD fileSize = InternetSetFilePointer(hUrl, 0L, NULL, FILE_END, 0);
	InternetSetFilePointer(hUrl, 0L, NULL, FILE_BEGIN, 0);

	DWORD totalBytesRead(0);

	while(m_GoOn && InternetReadFile(hUrl,reinterpret_cast<LPVOID>(buffer),bufferSize,&bytesRead) && bytesRead > 0)
	{
		DWORD bytesWritten(0);
		BOOL bRes = WriteFile(hFile,reinterpret_cast<LPCVOID>(buffer),bytesRead,&bytesWritten,NULL);
		if(!bRes || bytesWritten != bytesRead)
		{
			// THINK: WHAT TO DO ON LOCAL WRITE ERROR?
		}
		else
		{
			totalBytesRead += bytesRead;
			if(indicator)
			{
				const int maxBufSize = 100;
				WCHAR str1[maxBufSize] = {0};
				WCHAR str2[maxBufSize] = {0};
				StrFormatByteSizeW(totalBytesRead,str1,maxBufSize);
				StrFormatByteSizeW(fileSize,str2,maxBufSize);
				CString str;
				str.Format(IDS_DOWNLOADINGFILE,url.c_str(), str1, str2);
				indicator->SetMessage(str.operator LPCTSTR());
				indicator->SetProgress(totalBytesRead,fileSize);
			}
		}
	}
	if(indicator)
	{
		indicator->SetProgress(0,0);
		CString str;
		if(m_GoOn)
			str.LoadString(IDS_FILEDOWNLOADED);
		else
			str.LoadString(IDS_DOWNLOADCANCELED);
		indicator->SetMessage(str.operator LPCTSTR());
	}

	CloseHandle(hFile);
	InternetCloseHandle(hUrl);
 
	InternetCloseHandle(hInternet);

	return ((m_GoOn) ? S_OK : E_FAIL);
}

bool CAveInstFileFetcher::Cancel()
{
	m_GoOn = false;
	return true;
}

HRESULT CAveInstFileFetcher::FetchFile(CDownloadableFileDescription& descr,
	CAveInstProgressIndicator* indicator,
	const std::wstring& outputFile)
{
	HRESULT hRes = S_OK;

	if(descr.IsEmpty())
		return (hRes = E_FAIL);

	if(indicator)
		indicator->RegisterCancelable(this);

	std::wstring guid = descr.GetGUID();
	std::wstring url  = descr.GetURL();	

	if(!guid.empty())
	{
		hRes = FetchFileFromGUID(guid, indicator, outputFile);
	}
	
	if(!url.empty() && (guid.empty() || FAILED(hRes)))
		hRes = FetchFileFromURL(url, indicator, outputFile);

	if(indicator)
		indicator->UnregisterCancelable(this);

	return hRes;
}
