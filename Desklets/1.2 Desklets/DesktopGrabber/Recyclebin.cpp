

#include <windows.h>				// trivial
#include "../DeskletSDK.h"			// for the deskletSDK

#include <Gdiplus.h>				// for gdiplus stuff
using namespace Gdiplus;			// we will use the gdiplus namespace so we don't have to do Gdiplus::Bitmap,
									// but just Bitmap
#pragma comment(lib, "gdiplus.lib")	// link the gdiplus lib

#include <shellapi.h>				// for the recyclebin API
#include <shlwapi.h>
#include <shlobj.h>
#include <winbase.h>
#include <winioctl.h>
#include <shobjidl.h>


#include <string>					// for std::string
#include <sstream>					// for std::stringstream

#include "resource.h"				// trivial

#define WM_USER_FOLDERCHANGED WM_USER+88


struct DesktopGrabber
{
	HWND hwnd;
	LONG notifyId;
};

#define WM_SETPATH WM_USER + 202

typedef struct {
    DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
    DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
} SHNOTIFYSTRUCT;

LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_CREATE)
	{
		SetProp(hwnd,"lpData",(HANDLE)((LPCREATESTRUCT) lParam)->lpCreateParams);
		DesktopGrabber* dg = (DesktopGrabber*) GetProp(hwnd,"lpData");
		if(!dg)return 0;
	}
	else if(msg == WM_USER_FOLDERCHANGED)
	{
		DesktopGrabber* dg = (DesktopGrabber*) GetProp(hwnd,"lpData");
		if(!dg)return 0;

		SHNOTIFYSTRUCT shns;
		char szBefore[MAX_PATH];
		char szAfter[MAX_PATH];
    
		memcpy((void *)&shns,(void *)wParam,sizeof(SHNOTIFYSTRUCT));

		SHGetPathFromIDList((struct _ITEMIDLIST *)shns.dwItem1, 
                        szBefore);
		SHGetPathFromIDList((struct _ITEMIDLIST *)shns.dwItem2,
                         szAfter);

		if(lParam == SHCNE_CREATE)
		{

			POINT pt = {0};
			GetCursorPos(&pt);
			char deskletPath[MAX_PATH] = {0};
			GetDLLPath(GetModuleHandle(0),deskletPath,sizeof(deskletPath)/sizeof(deskletPath[0]));


			char drive[MAX_PATH]	= {0};
			char folder[MAX_PATH]	= {0};
			char file[MAX_PATH]		= {0};
			char ext[MAX_PATH]		= {0};
			_splitpath(szBefore,drive,folder,file,ext);
			if(stricmp(ext,".lnk") == 0)
			{
				HRESULT hres = S_OK; 
				IShellLink* psl = {0};
				hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
                            IID_IShellLink, (LPVOID*)&psl); 
				if(SUCCEEDED(hres))
				{
					IPersistFile* ppf = {0};
					hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
					if (SUCCEEDED(hres)) 
					{ 
						WCHAR wsz[MAX_PATH] = {0};
						MultiByteToWideChar(CP_ACP, 0, szBefore, -1, wsz, MAX_PATH); 
						hres = ppf->Load(wsz, STGM_READ); 
						if (SUCCEEDED(hres)) 
						{
							WIN32_FIND_DATA wfd = {0};
							hres = psl->GetPath(szBefore, 
                                        MAX_PATH, 
                                        (WIN32_FIND_DATA*)&wfd, 
                                        SLGP_SHORTPATH); 

							strcat(deskletPath,"Desklets\\Shortcut\\shortcut.dll");
						}
						ppf->Release();
					}
					psl->Release();
				}
			}
			else
				strcat(deskletPath,"Desklets\\Shortcut\\shortcut.dll");

			void* pointer = {0};
			HWND deskletHwnd = DeskletLoad(deskletPath,0,0,&pointer,FALSE);
			//MessageBox(0,"1",0,0);
			if(deskletHwnd)
			{
				SendMessage(deskletHwnd,WM_SETPATH,0,(LPARAM)szBefore);	
				DeskletSetPosition(deskletHwnd,&pt);
				DeskletShow(deskletHwnd);
			}
			else
			{
				MessageBox(0,"error loading desklet",0,MB_ICONERROR);
			}

		}
		else if( lParam == SHCNE_MKDIR)
		{
			POINT pt = {0};
			GetCursorPos(&pt);
			char deskletPath[MAX_PATH] = {0};
			GetDLLPath(GetModuleHandle(0),deskletPath,sizeof(deskletPath)/sizeof(deskletPath[0]));
			strcat(deskletPath,"Desklets\\folder\\folder.dll");

			void* pointer = {0};
			HWND deskletHwnd = DeskletLoad(deskletPath,0,0,&pointer,FALSE);
			if(deskletHwnd)
			{
				SendMessage(deskletHwnd,WM_SETPATH,0,(LPARAM)szBefore);	
				DeskletSetPosition(deskletHwnd,&pt);
				DeskletShow(deskletHwnd);
			}
			else
			{
				MessageBox(0,"error loading desklet",0,MB_ICONERROR);
			}
		}
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( (LPITEMIDLIST)shns.dwItem1 );
			imalloc->Free ( (LPITEMIDLIST)shns.dwItem2 );
			imalloc->Release ( );
		}


		return 0;
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
}


DesktopGrabber* __stdcall OnCreate()
{
	DesktopGrabber* dg = new DesktopGrabber;

	WNDCLASS c;
	ZeroMemory(&c,sizeof(c));
	c.hInstance = GetModuleHandle(0);
	c.lpfnWndProc = WinProc;
	c.lpszClassName = "AveDesktopGrabberProxyWindowClass";
	RegisterClass(&c);

	dg->hwnd = CreateWindow(c.lpszClassName,"HiddenAveDesktopGrabberProxyWindow",0,0,0,1,1,0,0,GetModuleHandle(0),(LPVOID)dg);

	LPITEMIDLIST	pidlDesktopDir	= {0};

	SHGetSpecialFolderLocation (dg->hwnd, CSIDL_DESKTOPDIRECTORY	, &pidlDesktopDir);

	SHChangeNotifyEntry shCNE;
	shCNE.pidl = pidlDesktopDir;
	shCNE.fRecursive = FALSE;

	dg->notifyId = SHChangeNotifyRegister(dg->hwnd,0x0001 | 0x0002 | 0x1000, 
                SHCNE_ALLEVENTS, 
                WM_USER_FOLDERCHANGED,
				1,&shCNE);


	return dg;
}

void __stdcall OnDestroy(DesktopGrabber* dg)
{
	DestroyWindow(dg->hwnd);

	if(dg->notifyId)
		SHChangeNotifyDeregister(dg->notifyId);

	delete dg;
};