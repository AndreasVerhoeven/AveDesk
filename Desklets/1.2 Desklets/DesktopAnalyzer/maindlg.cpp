// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "maindlg.h"

typedef struct {
    DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
    DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
} SHNOTIFYSTRUCT;

#include <algorithm>

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

DWORD __stdcall ThreadFunc(LPVOID lpData)
{
	CMainDlg* dlg = (CMainDlg*)lpData;

	CPidl pidl;
	pidl.SetFromSpecialFolderLocation(CSIDL_DESKTOPDIRECTORY);

	// request a handle to monitor c:\temp (only) for all changes
	HANDLE cnh = FindFirstChangeNotificationW(pidl.GetFullParseDisplayName().c_str(), FALSE, 
               FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
	while(1)
	{
		DWORD wr = WaitForSingleObject(cnh, INFINITE);

		// get rid of all similar events that occur shortly after this
		DWORD dwMilSecs = 200; // arbitrary; enlarge for floppies
		do
		{
			FindNextChangeNotification(cnh);
			wr = WaitForSingleObject(cnh, dwMilSecs);
		} while(wr == WAIT_OBJECT_0);
		// now wr == WAIT_TIMEOUT, no more pending events
		
		std::vector<std::wstring> cur = EnumFilesOnDesktop();

		std::vector<std::wstring> createdFiles = EnumDiff(dlg->files,cur);
		std::vector<std::wstring> deletedFiles = EnumDiff(cur,dlg->files);

		for(std::vector<std::wstring>::iterator iter = createdFiles.begin(); iter != createdFiles.end(); ++iter)
		{
			std::wstring s = L"CREATE\t" + *iter;
			dlg->lb.AddString(s.c_str() );
		}

		for(std::vector<std::wstring>::iterator iter2 = deletedFiles.begin(); iter2 != deletedFiles.end(); ++iter2)
		{
			std::wstring s = L"DEL\t" + *iter2;
			dlg->lb.AddString(s.c_str() );
		}
	


		dlg->files = cur;

		
	// thus, we have avoided unnecessary folder refreshes, see? :)

	// ...test some condition to break the infinite monitoring loop...
	}

	FindCloseChangeNotification(cnh); // release notification handle
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	lb = GetDlgItem(IDC_LIST1);

	files = EnumFilesOnDesktop();

	DWORD dummy(0);
	CreateThread(NULL,NULL,ThreadFunc,(LPVOID)this,0,&dummy);

	return TRUE;
}

LRESULT CMainDlg::OnPidlChanged(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	USES_CONVERSION;

		SHNOTIFYSTRUCT shns = {0};
		memcpy((void *)&shns,(void *)wParam,sizeof(SHNOTIFYSTRUCT));

		CPidl pidlBefore, pidlAfter;
		pidlBefore.SetFromPIDL( (LPITEMIDLIST) shns.dwItem1);
		pidlAfter.SetFromPIDL( (LPITEMIDLIST) shns.dwItem2);

		if(lParam == SHCNE_CREATE)
		{
			std::wstring  str(L"CREATE:\t");
			str += pidlBefore.GetFriendlyName().c_str();
			lb.AddString(str.c_str());
		}

		if(lParam == SHCNE_MKDIR)
		{
			std::wstring str(L"MKDIR:\t");
			str += pidlBefore.GetFriendlyName().c_str();
			lb.AddString(str.c_str());
		}

		if(lParam == SHCNE_DELETE)
		{
			std::wstring str(L"DELETE:\t\t");
			str += pidlBefore.GetFriendlyName().c_str();
			lb.AddString(str.c_str());
		}
		
		if(lParam == SHCNE_RMDIR)
		{
			std::wstring str(L"RMDIR:\t");
			str += pidlBefore.GetFriendlyName().c_str();
			lb.AddString(str.c_str());
		}

	/*	case SHCNE_CREATE              : //0x00000002L
		case SHCNE_DELETE              : //0x00000004L
		case SHCNE_MKDIR               : //0x00000008L
		case SHCNE_RMDIR               : //0x00000010L
		case SHCNE_MEDIAINSERTED       : //0x00000020L
		case SHCNE_MEDIAREMOVED        : //0x00000040L
		case SHCNE_DRIVEREMOVED        : //0x00000080L
		case SHCNE_DRIVEADD            : //0x00000100L
		case SHCNE_NETSHARE            : //0x00000200L
		case SHCNE_NETUNSHARE          : //0x00000400L
		case SHCNE_ATTRIBUTES          : //0x00000800L
		case SHCNE_UPDATEDIR           : //0x00001000L
		case SHCNE_UPDATEITEM          : //0x00002000L
		case SHCNE_SERVERDISCONNECT    : //0x00004000L
		case SHCNE_UPDATEIMAGE         : //0x00008000L
		case SHCNE_DRIVEADDGUI         : //0x00010000L
		case SHCNE_RENAMEFOLDER        : //0x00020000L
		case SHCNE_FREESPACE           : //0x00040000L */


		pidlBefore.Detach();
		pidlAfter.Detach();

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
