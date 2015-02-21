#define WINVER		0x0501
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0100

#define UNICODE

#include <atlbase.h>
#include <atlapp.h>

#include <vector>
#include <string>

#include "../pidl.h"

#include "../../DeskletSDK.h"

#include <algorithm>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#define WM_USER_UPDATEPATH WM_USER + 1209

#define WM_USER_NEWFILES WM_USER + 3094

LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);


std::vector<std::wstring> EnumFilesOnDesktop()
{
	std::vector<std::wstring> files;

	CPidl pidl;
	pidl.SetFromSpecialFolderLocation(CSIDL_DESKTOPDIRECTORY);

	std::wstring f = pidl.GetFullParseDisplayName() + L"\\*.*";

	WIN32_FIND_DATAW data = {0};

	HANDLE h = FindFirstFileW(f.c_str(),&data);
	if( h != INVALID_HANDLE_VALUE)
	{
		do
		{
			files.push_back(data.cFileName);

		}while(FindNextFileW(h,&data));

		FindClose(h);
	}

	return files;
}

std::vector<std::wstring> EnumDiff(std::vector<std::wstring>& old, std::vector<std::wstring>& cur)
{
	std::vector<std::wstring> dif;

	for(std::vector<std::wstring>::iterator iter = cur.begin(); iter != cur.end(); ++iter)
	{
		if( std::find(old.begin(),old.end(),*iter)  == old.end() )
		{
			dif.push_back(*iter);
		}
	}

	return dif;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

DWORD __stdcall ThreadFunc(LPVOID lpData);

class CDesktopModule
{
public:
	HANDLE h;
	std::vector<std::wstring> files;
	std::vector<std::wstring> newFiles;
	HANDLE cnh;
	HWND hwnd;
	bool stopped;

	CPidl pidl;

public:
	CDesktopModule() : cnh(NULL), hwnd(NULL),stopped(false)
	{
		pidl.SetFromSpecialFolderLocation(CSIDL_DESKTOPDIRECTORY);
		files = EnumFilesOnDesktop();

		WNDCLASS c;
		ZeroMemory(&c,sizeof(c));
		c.hInstance = HINST_THISCOMPONENT;
		c.lpfnWndProc = WinProc;
		c.lpszClassName = _T("AveDesktopModuleProxyWindowClass");
		RegisterClass(&c);

		hwnd = CreateWindow(c.lpszClassName,_T("HiddenAveDesktopModuleProxyWindow"),0,0,0,1,1,0,0,HINST_THISCOMPONENT,(LPVOID)this);


	
		DWORD dummy(0);
		CreateThread(NULL,NULL,ThreadFunc,(LPVOID)this,0,&dummy);
	}

	void OnNewFiles()
	{
		char path[MAX_PATH] = {0};
		GetDLLPath(HINST_THISCOMPONENT,path,MAX_PATH);
		strcat(path,"pidlshortcut.dll");

		for(std::vector<std::wstring>::iterator iter = newFiles.begin(); iter != newFiles.end(); ++iter)
		{
			void* p = NULL;
			
			std::wstring s = pidl.GetFullParseDisplayName() + L"\\" + *iter;

			//WCHAR drive[MAX_PATH]	= {0};
			//WCHAR folder[MAX_PATH]	= {0};
			//WCHAR file[MAX_PATH]		= {0};
			//WCHAR ext[MAX_PATH]		= {0};
			//_wsplitpath(s.c_str(),drive,folder,file,ext);
			//if(wcsicmp(ext,L".lnk") == 0)
			//{
				CComPtr<IShellLink> shellLink;
				if( SUCCEEDED(shellLink.CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER)) )
				{
					CComPtr<IPersistFile> persistFile;
					if( SUCCEEDED(shellLink.QueryInterface(&persistFile)) )
					{
						if( SUCCEEDED(persistFile->Load(s.c_str(),STGM_READ)) )
						{
							WCHAR wPath[MAX_PATH] = {0};
							WIN32_FIND_DATAW data = {0};
							if( SUCCEEDED(shellLink->GetPath(wPath,MAX_PATH,&data,SLGP_RAWPATH)) )
								s = wPath;
						}

					}
				}
			//}
	
			POINT pt = {0};
			GetCursorPos(&pt);

			HWND hwndDesklet = DeskletLoad(path, NULL, NULL, &p, FALSE);
			if(hwndDesklet)
			{
				SendMessage(hwndDesklet,WM_USER_UPDATEPATH,1,(LPARAM)s.c_str() );
				DeskletSetPosition(hwndDesklet,&pt);
				DeskletFadeIn(hwndDesklet,FALSE,USE_DEFAULT_DURATION);
			}
		}
		newFiles.clear();
	}

	~CDesktopModule()
	{
		stopped = true;
		//FindCloseChangeNotification(cnh);

		WaitForSingleObject(h,INFINITE);

		DestroyWindow(hwnd);

	}
};

LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_CREATE)
	{
		SetProp(hwnd,_T("lpData"),(HANDLE)((LPCREATESTRUCT) lParam)->lpCreateParams);
		CDesktopModule* dm = (CDesktopModule*) GetProp(hwnd,_T("lpData"));
		if(!dm)return 0;
	}
	else if(msg == WM_USER_NEWFILES)
	{
		CDesktopModule* dm = (CDesktopModule*) GetProp(hwnd,_T("lpData"));	
		dm->OnNewFiles();
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
}



DWORD __stdcall ThreadFunc(LPVOID lpData)
{
	CDesktopModule* dm = (CDesktopModule*)lpData;

	// request a handle to monitor c:\temp (only) for all changes
	dm->cnh = FindFirstChangeNotificationW(dm->pidl.GetFullParseDisplayName().c_str(), FALSE, 
               FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
	while(1 & !dm->stopped)
	{
		DWORD wr = WaitForSingleObject(dm->cnh, INFINITE);

		// get rid of all similar events that occur shortly after this
		DWORD dwMilSecs = 30; // arbitrary; enlarge for floppies
		do
		{
			FindNextChangeNotification(dm->cnh);
			wr = WaitForSingleObject(dm->cnh, dwMilSecs);
		} while(wr == WAIT_OBJECT_0);
		// now wr == WAIT_TIMEOUT, no more pending events
		
		std::vector<std::wstring> cur = EnumFilesOnDesktop();

		std::vector<std::wstring> createdFiles = EnumDiff(dm->files,cur);
		std::vector<std::wstring> deletedFiles = EnumDiff(cur,dm->files);

		std::copy(createdFiles.begin(),createdFiles.end(),std::back_inserter(dm->newFiles ));
		dm->files = cur;
		PostMessage(dm->hwnd,WM_USER_NEWFILES,0,0);

		/*
		for(std::vector<std::wstring>::iterator iter2 = deletedFiles.begin(); iter2 != deletedFiles.end(); ++iter2)
		{
			std::wstring s = L"DEL\t" + *iter2;
			dlg->lb.AddString(s.c_str() );
		}
		*/

		
	// thus, we have avoided unnecessary folder refreshes, see? :)

	// ...test some condition to break the infinite monitoring loop...
	}

	FindCloseChangeNotification(dm->cnh); // release notification handle

	ExitThread(0);

	CloseHandle(dm->h);

	return 0;
}

CDesktopModule* __stdcall OnCreate()
{
	CDesktopModule* dm = new CDesktopModule;


	return dm;
}

void __stdcall OnDestroy(CDesktopModule* dm)
{
	delete dm;
};