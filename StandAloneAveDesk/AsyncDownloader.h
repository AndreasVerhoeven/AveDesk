#pragma once

#include "AveInstProgressIndicator.h"

#include <string>

#define WM_USER_PROGRESS_CALLBACK WM_USER + 547
class CAveInstCancelable;

class CAsyncDownloader : 
	public CAveInstProgressIndicator
{
protected:
	HWND callbackHwnd;
	CAveInstCancelable* canceler;
	std::wstring url;
	HANDLE threadHandle;
public:
	CAsyncDownloader(HWND callbackHwnd, const std::wstring& url);
	virtual ~CAsyncDownloader(void);

	virtual void SetMessage(std::wstring msg);
	virtual void SetProgress(long progress, long max);
	virtual bool RegisterCancelable(CAveInstCancelable* c);
	virtual bool UnregisterCancelable(CAveInstCancelable* c);
	virtual void Hide();

	void Cancel();
	void Start();

	static DWORD CALLBACK ThisThreadFunc(LPVOID lpData);
};
