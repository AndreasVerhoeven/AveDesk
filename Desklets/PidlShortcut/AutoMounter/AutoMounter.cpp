#define WINVER		0x0501
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0100

#include <windows.h>
#include <wininet.h>
#include <shlobj.h>

#include "Pidl.h"


#include "../../DeskletSDK.h"

#include <string>
#include <algorithm>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#define WM_PIDLUPDATED WM_USER + 4931

#define WM_USER_UPDATEPATHFROMAUTOMOUNT WM_USER + 1210
#define WM_USER_GETPATH	WM_USER + 1211

LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)


struct TESTDATA
{
	LPITEMIDLIST pidl;
	BOOL flag;
};


BOOL CALLBACK EnumDeskletProc(HWND hWnd, LPARAM lParam)
{

	TESTDATA* data = reinterpret_cast<TESTDATA*>(lParam);
	if(NULL == data)
		return FALSE;

	WCHAR path[MAX_PATH] = {0};
	BOOL wasChanged(FALSE);
	SendMessage(hWnd,WM_USER_GETPATH,reinterpret_cast<WPARAM>(&wasChanged),reinterpret_cast<LPARAM>(path));
	if(TRUE == wasChanged)
	{
		CPidl tmpPidl;
		tmpPidl.SetFromPath(path);
		CPidl orgPidl;
		orgPidl.SetFromPIDL(data->pidl);

		data->flag = (orgPidl == tmpPidl) ? TRUE : FALSE;

		orgPidl.Detach();

		return !data->flag;
	}
	else
	{
		return TRUE;
	}

}


typedef struct {
    DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
    DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
} SHNOTIFYSTRUCT;

class AutoMounter
{
private:
	CPidl m_DrivesPidl;
	HWND m_hWnd;

public:
	AutoMounter()
	{
		m_DrivesPidl.SetFromSpecialFolderLocation(CSIDL_DRIVES);

		WNDCLASS c = {0};
		c.hInstance = HINST_THISCOMPONENT;
		c.lpfnWndProc = WinProc;
		c.lpszClassName = _T("AveAutoMounterModuleProxyWindowClass");
		RegisterClass(&c);

		m_hWnd = CreateWindow(c.lpszClassName,_T("HiddenAveAutoMounterModuleProxyWindow"),0,0,0,1,1,0,0,HINST_THISCOMPONENT,(LPVOID)this);
		SetProp(m_hWnd,TEXT("lpData"),reinterpret_cast<HANDLE>(this));

		m_DrivesPidl.StartMonitoring(m_hWnd,WM_PIDLUPDATED,SHCNE_GLOBALEVENTS);
	}

	~AutoMounter()
	{
		if(m_DrivesPidl.IsValid())
			m_DrivesPidl.StopMonitoring();

		if(IsWindow(m_hWnd))
			DestroyWindow(m_hWnd);
	}


	std::wstring GetConfigFilePath(CPidl& pidl)
	{
		WCHAR path[MAX_PATH * 2] = {0};
		//::GetDLLPathW(HINST_THISCOMPONENT,path,MAX_PATH);
		UINT bufSize(MAX_PATH+1);
		DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH,path,&bufSize);
		PathAppendW(path,L"\\AutoMounterDefaults\\");
		if(!PathFileExistsW(path))
			CreateDirectoryW(path,NULL);

		WCHAR add[3] = {0};

		std::wstring pdn = pidl.GetFullParseDisplayName();
		if(pdn.length() > 1 && pdn[1] != L':')
		{
			add[0] = pdn[0];
			add[1] = pdn[1];
			add[2] = L'\0';
		}
		else
		{
			if(pdn.length() > 0)
			{
				add[0] = pdn[0];
				add[1] = L'\0';
			}
		}

		PathAppendW(path,add);
		PathAddExtensionW(path,L".avedesklet");
		
		return path;
	}

	void OnPidlMessage(WPARAM wParam, LPARAM lParam)
	{
		SHNOTIFYSTRUCT shns = {0};
		memcpy((void *)&shns,(void *)wParam,sizeof(SHNOTIFYSTRUCT));

		CPidl pidlBefore, pidlAfter;
		pidlBefore.SetFromPIDL( (LPITEMIDLIST) shns.dwItem1);
		pidlAfter.SetFromPIDL( (LPITEMIDLIST) shns.dwItem2);

		if((lParam == SHCNE_MEDIAINSERTED || lParam == SHCNE_DRIVEADD || lParam == SHCNE_NETSHARE ))
		{
			// TODO: determine if pidl isn't already on the d-top.

			TESTDATA data = {0};
			data.flag = FALSE;
			data.pidl = pidlBefore;
			DeskletEnumRunningDesklets(EnumDeskletProc,reinterpret_cast<LPARAM>(&data));
			if(FALSE == data.flag)
			{
				
				WCHAR path[MAX_PATH] = {0};
				GetDLLPathW(HINST_THISCOMPONENT,path,MAX_PATH);
				wcscat(path,L"pidlshortcut.dll");

				std::wstring configFile = GetConfigFilePath(pidlBefore);
				//MessageBoxW(NULL,configFile.c_str(),0,0);
				if(PathFileExistsW(configFile.c_str()))
				{
					DeskletLoadFromConfigW(configFile.c_str(),NULL,TRUE);
				}
				else
				{
					HWND hwndDesklet = DeskletLoadFromDLLW(path,NULL,NULL,FALSE);
					DeskletHide(hwndDesklet);
					SendMessage(hwndDesklet,WM_USER_UPDATEPATHFROMAUTOMOUNT,0L,reinterpret_cast<LPARAM>(pidlBefore.GetFullParseDisplayName().c_str()) );
					DeskletFadeIn(hwndDesklet,FALSE,USE_DEFAULT_DURATION);
				}
			}
		}

		pidlBefore.Detach();
		pidlAfter.Detach();
	}

};

LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_CREATE)
	{
	}
	else if(msg == WM_PIDLUPDATED)
	{
		AutoMounter* am = (AutoMounter*) GetProp(hwnd,TEXT("lpData"));	
		if(NULL != am)
			am->OnPidlMessage(wParam, lParam);
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
}


AutoMounter* __stdcall OnCreate()
{
	AutoMounter* am = new AutoMounter;

	return am;
}

void __stdcall OnDestroy(AutoMounter* am)
{
	delete am;
};