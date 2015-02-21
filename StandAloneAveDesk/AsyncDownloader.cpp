#include "stdafx.h"
#include "AsyncDownloader.h"
#include "AveInstCancelable.h"
#include "AveInstFileFetcher.h"
#include "AveInstaller.h"

CAsyncDownloader::CAsyncDownloader(HWND callbackHwnd, const std::wstring& url) : 
	callbackHwnd(callbackHwnd),
	url(url),
	threadHandle(NULL)
{
}

CAsyncDownloader::~CAsyncDownloader(void)
{
}

void CAsyncDownloader::SetMessage(std::wstring msg)
{
	// not supported currently, do nothing.
}


void CAsyncDownloader::SetProgress(long progress, long max)
{
	if(callbackHwnd != NULL)
		::PostMessage(callbackHwnd, WM_USER_PROGRESS_CALLBACK, (WPARAM)progress, (LPARAM)max);
}

bool CAsyncDownloader::RegisterCancelable(CAveInstCancelable* c)
{
	canceler = c;
	return true;
}

bool CAsyncDownloader::UnregisterCancelable(CAveInstCancelable* c)
{
	canceler = NULL;
	return true;
}

void CAsyncDownloader::Hide()
{
	// not supported currently, do nothing.
}

void CAsyncDownloader::Cancel()
{
	if(canceler != NULL)
		canceler->Cancel();
}

void CAsyncDownloader::Start()
{
	DWORD id(0);
	threadHandle = ::CreateThread(NULL, 0, ThisThreadFunc, reinterpret_cast<LPVOID>(this), 0, &id);
}

DWORD CALLBACK CAsyncDownloader::ThisThreadFunc(LPVOID lpData)
{
	CoInitialize(NULL);
	CAsyncDownloader* d = reinterpret_cast<CAsyncDownloader*>(lpData);
	if(d != NULL)
	{
		WCHAR tmpPath[MAX_PATH] = {0};
		GetTempPath(MAX_PATH, tmpPath);
		WCHAR tmpFile[MAX_PATH] = {0};
		GetTempFileName(tmpPath, _T("ave"), 0, tmpFile);
		CAveInstFileFetcher fetcher;
		HRESULT hRes = fetcher.FetchFileFromURL(d->url, d, tmpFile);
		if(SUCCEEDED(hRes))
		{
			CAveInstaller inst;
			HRESULT hRes = inst.Install(tmpFile, d->callbackHwnd, TRUE, d);
			DeleteFile(tmpFile);
		}

	}

	if(d->callbackHwnd != NULL)
		::PostMessage(d->callbackHwnd, WM_USER_PROGRESS_CALLBACK, (WPARAM)-1L, (LPARAM)-1L);

	CloseHandle(d->threadHandle);

	delete d;

	CoUninitialize();

	ExitThread(0);
	return 0;
}