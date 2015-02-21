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
#include "../ShellFolder.h"

#include "../../DeskletSDK.h"

#include <algorithm>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")


#define TIMERID 3039

#define WM_USER_UPDATEPIDL WM_USER + 1208
#define WM_USER_UPDATEPATH WM_USER + 1209
#define WM_USER_UPDATEPATHFROMAUTOMOUNT WM_USER + 1210
#define WM_USER_GETPATH	WM_USER + 1211


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


EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

DWORD __stdcall ThreadFunc(LPVOID lpData);

class CDesktopModule
{
public:
	HWND hwnd;

public:
	CDesktopModule() : hwnd(NULL)
	{
		bool launch = true;
		WCHAR path[MAX_PATH+2] = {0};
		GetDLLPathW(NULL,path,MAX_PATH);
		
		UINT maxBufSize(MAX_PATH+1);
		if(DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH,path,&maxBufSize))
		{
			PathAppend(path,TEXT("desktopinstaller.metadata"));

			TCHAR convMsg[MAX_PATH]  = {0};
			GetPrivateProfileString(TEXT("DesktopInstaller"),TEXT("Converted"),TEXT("MAYBE"),convMsg,MAX_PATH,path);

			if(_tcsicmp(convMsg,TEXT("YES")) == 0 || _tcsicmp(convMsg,TEXT("NO")) == 0)
				launch = false;
		}

		if(true == launch)
		{
			WNDCLASS c;
			ZeroMemory(&c,sizeof(c));
			c.hInstance = HINST_THISCOMPONENT;
			c.lpfnWndProc = WinProc;
			c.lpszClassName = _T("AveDesktopModuleProxyWindowClass");
			RegisterClass(&c);
	
			hwnd = CreateWindow(c.lpszClassName,_T("HiddenAveDesktopModuleProxyWindow"),0,0,0,1,1,0,0,HINST_THISCOMPONENT,(LPVOID)this);
			SetTimer(hwnd,TIMERID,10000,NULL);
		}
	}

	void OnTimer()
	{
		KillTimer(hwnd,TIMERID);

		int res = MessageBox(NULL,TEXT("Do you want to convert all icons on your desktop to desklets?"),TEXT("AveDesk Desktop"),MB_ICONQUESTION | MB_YESNOCANCEL);

		if(res == IDYES)
		{
			std::vector<std::wstring> files;

			TCHAR path[MAX_PATH+2] = {0};
			GetDLLPathW(NULL,path,MAX_PATH);

			ConvertFiles(/*EnumFilesOnDesktop()*/);

			UINT maxBufSize(MAX_PATH+1);
			if(DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH,path,&maxBufSize))
			{

				PathAppend(path,TEXT("desktopinstaller.metadata"));

				WritePrivateProfileString(TEXT("DesktopInstaller"),TEXT("Converted"),TEXT("YES"),path);
			}
		}
		else if(res == IDNO)
		{
			TCHAR path[MAX_PATH+2] = {0};
			GetDLLPathW(NULL,path,MAX_PATH);

			UINT maxBufSize(MAX_PATH+1);
			if(DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH,path,&maxBufSize))
			{

				PathAppend(path,TEXT("desktopinstaller.metadata"));

				WritePrivateProfileString(TEXT("DesktopInstaller"),TEXT("Converted"),TEXT("NO"),path);
			}

			DestroyWindow(hwnd);
			hwnd = NULL;
		}
	}

	void ConvertFiles(/*std::vector<std::wstring> files*/)
	{
		int xPos = 10;
		int yPos = 10;
		int maxWidth = 0;
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		WCHAR path[MAX_PATH] = {0};
		GetDLLPathW(HINST_THISCOMPONENT,path,MAX_PATH);
		wcscat(path,L"pidlshortcut.dll");

		CPidl dtopPidl;
		if( FAILED(dtopPidl.SetFromSpecialFolderLocation(CSIDL_DESKTOPDIRECTORY)) )
			return;

		CShellFolder dtopFolder;
		
		if( FAILED(dtopFolder.SetFromPIDL(dtopPidl)) )
			return;


		CComPtr<IEnumIDList> enumerator;
		if( FAILED(dtopFolder.m_ShellFolder->EnumObjects(NULL,SHCONTF_FOLDERS | SHCONTF_NONFOLDERS,&enumerator)) )
			return;

		LPITEMIDLIST p = {0};
		while( enumerator->Next(1,&p,NULL) == NOERROR && p != NULL )
		{	
			USES_CONVERSION;
	
			CPidl cp;
			cp.SetFromPIDL(p);


			CPidl curFilePidl;
			curFilePidl.SetFromPIDL(CPidl::Append(dtopPidl,cp));

			CComPtr<IShellLink> shellLink;
			if( SUCCEEDED(shellLink.CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER)) )
			{
				CComPtr<IPersistFile> persistFile;
				if( SUCCEEDED(shellLink.QueryInterface(&persistFile)) )
				{
					if( SUCCEEDED(persistFile->Load(curFilePidl.GetFullParseDisplayName().c_str(),STGM_READ)) )
					{
						WCHAR wPath[MAX_PATH] = {0};
						WIN32_FIND_DATAW data = {0};
						if( SUCCEEDED(shellLink->GetPath(wPath,MAX_PATH,&data,SLGP_RAWPATH)) )
						{
							curFilePidl.SetFromPath(wPath);
						}
					}
				}
			}


			HWND hwndDesklet = DeskletLoadFromDLLW(path,NULL,NULL,FALSE);
			if(hwndDesklet)
			{
				SIZE s = {0};
				DeskletGetSize(hwndDesklet,&s);

				if(yPos > screenHeight - s.cy)
				{
					yPos = 0;
					xPos += maxWidth;
					maxWidth = 0;
				}

				yPos = yPos + s.cy + s.cy / 2;
				int w = s.cx + 200;

				if(w > maxWidth)
					maxWidth = w;

				POINT pt = {xPos, yPos};
				DeskletSetPosition(hwndDesklet,&pt);

				SendMessage(hwndDesklet,WM_USER_UPDATEPIDL,1,(LPARAM)(LPITEMIDLIST)curFilePidl );
				DeskletFadeIn(hwndDesklet,FALSE,USE_DEFAULT_DURATION);
			}


		}


		/*

		for(std::vector<std::wstring>::iterator iter = files.begin(); iter != files.end(); ++iter)
		{
			void* p = NULL;
			
			CPidl pidl;
			pidl.SetFromSpecialFolderLocation(CSIDL_DESKTOPDIRECTORY);
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
	
			HWND hwndDesklet = DeskletLoadFromDLLW(path,NULL,NULL,FALSE);
			if(hwndDesklet)
			{
				SendMessage(hwndDesklet,WM_USER_UPDATEPATH,1,(LPARAM)s.c_str() );

				SIZE s = {0};
				DeskletGetSize(hwndDesklet,&s);

				if(yPos > screenHeight - s.cy)
				{
					yPos = 0;
					xPos += maxWidth;
					maxWidth = 0;
				}

				yPos = yPos + s.cy + s.cy / 2;
				int w = s.cx + 200;

				if(w > maxWidth)
					maxWidth = w;

				POINT pt = {xPos, yPos};
				DeskletSetPosition(hwndDesklet,&pt);
				DeskletFadeIn(hwndDesklet,FALSE);
			}
		}
		*/

		MessageBox(NULL,TEXT("All desktop files are now converted to AveDesk desklets.\nYou may now disable this module."),TEXT("AveDesk Desktop Converter - Done"),MB_ICONINFORMATION);
	}

	~CDesktopModule()
	{
		if(IsWindow(hwnd))
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
	else if(msg == WM_TIMER && wParam == TIMERID)
	{
		CDesktopModule* dm = (CDesktopModule*) GetProp(hwnd,_T("lpData"));	
		if(dm)
			dm->OnTimer();
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
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
