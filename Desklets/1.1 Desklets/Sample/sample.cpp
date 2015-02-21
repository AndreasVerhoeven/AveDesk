// Sample Desklet
// Showes a drive image depending on the status of the drive

#include <windows.h>					// for common windows stuff
#include "../DeskletSDK.h"				// for common desklet stuff
#include <dbt.h>						// drive checking stuff

#include <Gdiplus.h>					// for all GDI+ stuff
using namespace Gdiplus;				// use the namespace Gdiplus by default
#pragma comment(lib, "gdiplus.lib")		// link with gdiplus.lib

#include "resource.h"

class DOCKLET_DATA
{
	public:
		HINSTANCE	hInstanceDll;
		HWND		hwndDesklet;
		bool doLeftClick;
		UINT windowState;
		bool doAnimation;

		char drive[MAX_PATH];

	void OnDeviceChange (HWND hwnd, WPARAM wParam, LPARAM lParam);

	void SetLabel()
	{
		char lbl[1024];
		lbl[0] = drive[0];
		lbl[1] = ' ';
		lbl[2] = '(';
		lbl[3] = '\0';
		char drivename[100];
		GetVolumeInformation(drive,drivename,100,0,0,0,0,0);
		strcat(lbl,drivename);
		strcat(lbl,")");
		DeskletSetLabel(hwndDesklet,lbl);
	}
};

void CheckDrive(DOCKLET_DATA* lpData)
{
	if(strcmp(lpData->drive,"")==0)
	{
		DeskletShow(lpData->hwndDesklet);
		return;
	}

	ULARGE_INTEGER dummy0, dummy1, dummy2;
	if(GetDiskFreeSpaceEx(lpData->drive,&dummy0,&dummy1,&dummy2))
	{
		DeskletShow(lpData->hwndDesklet);
	}
	else
	{
		DeskletHide(lpData->hwndDesklet);
	}
	
}

DOCKLET_DATA *CALLBACK OnCreate(HWND hwndDocklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	DOCKLET_DATA* lpData = new DOCKLET_DATA;
	ZeroMemory(lpData,sizeof(DOCKLET_DATA));

	lpData->hInstanceDll = hInstance;
	lpData->hwndDesklet = hwndDocklet;

	if(szIni && szIniGroup)
	{
		GetPrivateProfileString(szIniGroup,"Drive","",lpData->drive,sizeof(lpData->drive)/sizeof(lpData->drive[0]),szIni);
		lpData->SetLabel();

		lpData->doLeftClick = GetPrivateProfileInt(szIniGroup,"DoLC",TRUE,szIni)==TRUE;
		lpData->doAnimation = GetPrivateProfileInt(szIniGroup,"DoAnim",FALSE,szIni)==TRUE;
		lpData->windowState = GetPrivateProfileInt(szIniGroup,"WinState",TRUE,szIni);


		ULARGE_INTEGER dummy0, dummy1, dummy2;
		if(!GetDiskFreeSpaceEx(lpData->drive,&dummy0,&dummy1,&dummy2))
			DeskletHide(lpData->hwndDesklet);
	}
	else
	{
		lpData->doLeftClick = true;
		lpData->windowState = 0;
		lpData->doAnimation = false;
		strcpy(lpData->drive,"c:\\");
		DeskletSetLabel(lpData->hwndDesklet,"Right-click to setup!");
	}

	DeskletSetAlpha(lpData->hwndDesklet,255);
	CheckDrive(lpData);

	return lpData;
}

void CALLBACK OnDestroy(DOCKLET_DATA *lpData, HWND hwndDocklet)
{
	delete lpData;
	lpData = 0;
}
void CALLBACK OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName, "Mountchecker Desklet");
	strcpy(szAuthor, "Andreas Verhoeven");
	(*iVersion) = 100;
	strcpy(szNotes,   "The Desklet is only visible when the drive is mounted.");
}

void CALLBACK OnSave(DOCKLET_DATA *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	WritePrivateProfileString(szIniGroup,"Drive",lpData->drive,szIni);
	WritePrivateProfileInt(szIniGroup,"DoLC",lpData->doLeftClick ? TRUE : FALSE,szIni);
	WritePrivateProfileInt(szIniGroup,"DoAnim",lpData->doAnimation ? TRUE : FALSE,szIni);
	WritePrivateProfileInt(szIniGroup,"WinState",lpData->windowState,szIni);
}

char FirstDriveFromMask (ULONG unitmask)
{
	char i;

	for (i = 0; i < 26; ++i)
    {
		if (unitmask & 0x1)
			break;
		unitmask = unitmask >> 1;
    }

    return (i + 'a');
}

void DOCKLET_DATA::OnDeviceChange (HWND hwnd, WPARAM wParam, LPARAM lParam)
   {

      PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

      switch(wParam)
      {
      case DBT_DEVICEARRIVAL:
         // See if a CD-ROM or DVD was inserted into a drive.
         if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
         {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

            if (lpdbv -> dbcv_flags & DBTF_MEDIA)
            {
               if(FirstDriveFromMask(lpdbv ->dbcv_unitmask)==tolower(drive[0]))
			   {
					SetLabel();
					if(doAnimation)
						DeskletDoAttentionAnimation(hwndDesklet,ANIMATION_FADEIN,1);	
					else
						DeskletShow(hwndDesklet);
			   }
            }
         }
        break;


     case DBT_DEVICEREMOVECOMPLETE:
        // See if a CD-ROM was removed from a drive.
        if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
        {
           PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

           if (lpdbv -> dbcv_flags & DBTF_MEDIA)
           {
              if(FirstDriveFromMask(lpdbv ->dbcv_unitmask)==tolower(drive[0]))
			  {
				if(doAnimation)
					DeskletDoAttentionAnimation(hwndDesklet,ANIMATION_FADEOUT,1);
				else
					DeskletHide(hwndDesklet);
			  }
           }
        }	
        break;
      }
   }

void CALLBACK OnProcessMessage(DOCKLET_DATA *lpData, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DEV_BROADCAST_HDR* hdr= (DEV_BROADCAST_HDR*) lParam;
	switch(uMsg)
	{
		case WM_DEVICECHANGE:
			lpData->OnDeviceChange(hwnd,wParam,lParam);
		break;
	}
}

int CALLBACK DialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			SetProp(hDlg,"lpData",(HANDLE)lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
			if(!lpData)return TRUE;
			
			DWORD drives;
			drives= GetLogicalDrives();
			DWORD d;
			d = 0x01;
			int i;
			i = 0;
			while(i < 32)
			{
				if(drives & d)
				{
					char tmp[100];
					tmp[0] = 'a'+i;
					tmp[1] = ':';
					tmp[2] = '\\';
					tmp[3] = '\0';
					SendDlgItemMessage(hDlg,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)tmp);

				}
				d  *= 2;
				i++;

			}
			SetDlgItemText(hDlg,IDC_COMBO1,lpData->drive);
			

		break;

		case WM_COMMAND:
			if(LOWORD(wParam)==IDOK)
			{
				GetDlgItemText(hDlg,IDC_COMBO1,lpData->drive,sizeof(lpData->drive)/sizeof(lpData->drive[0]));
				lpData->drive[0] = tolower(lpData->drive[0]);
				ULARGE_INTEGER dummy0, dummy1, dummy2;
				lpData->SetLabel();
				if(!GetDiskFreeSpaceEx(lpData->drive,&dummy0,&dummy1,&dummy2))
					DeskletHide(lpData->hwndDesklet);
				else
					DeskletShow(lpData->hwndDesklet);
			}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
		break;
	}

	return FALSE;
}

int CALLBACK LeftClickDialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			SetProp(hDlg,"lpData",(HANDLE)lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
			if(!lpData)return TRUE;
			
			CheckDlgButton(hDlg,IDC_LEFTCLICK,lpData->doLeftClick);

			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_ADDSTRING,0,(LPARAM)"Normal Window");
			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_ADDSTRING,0,(LPARAM)"Minimized Window");
			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_ADDSTRING,0,(LPARAM)"Maximized Window");
			SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_SETCURSEL,lpData->windowState,0);

		break;

		case WM_COMMAND:
			if(LOWORD(wParam)==IDOK)
			{
				lpData->doLeftClick = IsDlgButtonChecked(hDlg,IDC_LEFTCLICK) == BST_CHECKED;
				lpData->windowState = SendDlgItemMessage(hDlg,IDC_WINSTATE,CB_GETCURSEL,0,0);
			}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
		break;
	}

	return FALSE;
}

int CALLBACK AnimationDialogProc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	DOCKLET_DATA* lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			SetProp(hDlg,"lpData",(HANDLE)lParam);
			lpData = (DOCKLET_DATA*)GetProp(hDlg,"lpData");
			if(!lpData)return TRUE;
			
			CheckDlgButton(hDlg,IDC_ANIM,lpData->doAnimation);


		break;

		case WM_COMMAND:
			if(LOWORD(wParam)==IDOK)
			{
				lpData->doAnimation = IsDlgButtonChecked(hDlg,IDC_ANIM) == BST_CHECKED;
			}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
		break;
	}

	return FALSE;
}

void CALLBACK OnConfigure(DOCKLET_DATA* lpData)
{
	TabPage t[3];
	t[0].dlgProc = DialogProc;
	t[0].title = "Drive";
	t[0].titleLength = 5;
	t[0].hInstance = lpData->hInstanceDll;
	t[0].resourceName = MAKEINTRESOURCE(IDD_SELECTDRIVE);

	t[1].dlgProc = LeftClickDialogProc;
	t[1].title = "Left Click Behaviour";
	t[1].titleLength = strlen("Left Click Behaviour");
	t[1].hInstance = lpData->hInstanceDll;
	t[1].resourceName = MAKEINTRESOURCE(IDD_LEFTCLICK);

	t[2].dlgProc = AnimationDialogProc;
	t[2].title = "Animation";
	t[2].titleLength = strlen("Animation");
	t[2].hInstance = lpData->hInstanceDll;
	t[2].resourceName = MAKEINTRESOURCE(IDD_ANIMATION);


	PropertyDialogStruct pds;
	pds.lParam = 0;
	pds.pages = &t[0];
	pds.pagesCount = 3;

	DeskletDefaultConfigDialog(lpData->hwndDesklet,&pds);
}

BOOL CALLBACK OnRightButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	HMENU menu = CreatePopupMenu();
	AppendMenu(menu,MF_SEPARATOR,0,0);
	AppendMenu(menu,MF_ENABLED | MF_STRING,10001,"Desklet Properties...");

	int cmd = ShowContextMenu(lpData->hwndDesklet,menu,0,lpData->drive);
	switch(cmd)
	{
		case 10001: OnConfigure(lpData);break;
	}
	return TRUE;
}

BOOL CALLBACK OnLeftButtonClick(DOCKLET_DATA *lpData, POINT* p, SIZE *s)
{
	if(lpData->doLeftClick)
	{
		UINT openMode = SW_NORMAL;
		if(lpData->windowState == 1)openMode = SW_SHOWMINIMIZED;
		else if(lpData->windowState == 2) openMode = SW_SHOWMAXIMIZED;

		ShellExecute(0,"open",lpData->drive,0,0,openMode);
	}

	return TRUE;
}
