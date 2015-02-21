// RecycleBinBehaviour.cpp: implementation of the CRecycleBinBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#include "RecycleBinBehaviour.h"
#include "ShellHelper.h"
#include <sstream>

#include "resource.h"
#include "../DeskletSDK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define HIDA_GetPIDLFolder(pida) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[0])
#define HIDA_GetPIDLItem(pida, i) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[i+1])

CRecycleBinBehaviour::CRecycleBinBehaviour(IShortcutDesklet* desklet) :
	CShellItemBehaviour(desklet),
	m_IsInit(false),
	m_Desklet(desklet),
	m_CanDrop(false),
	m_DropIsDrive(false),
	m_InDeleteMode(false)
{
	m_ShellIdListClipFormat	= RegisterClipboardFormat(CFSTR_SHELLIDLIST);
	m_AveDeskDeskletFormat	= RegisterClipboardFormat("AveDeskDesklet");
}

CRecycleBinBehaviour::~CRecycleBinBehaviour()
{

}

BOOL CRecycleBinBehaviour::GetSpecialDropEffect(DWORD& effect)
{
	if(m_CanDrop)
		effect = (m_InDeleteMode ? PRIVATE_DROPEFFECT_DELETE  : PRIVATE_DROPEFFECT_REMOVE);
	else
		effect = DROPEFFECT_NONE;
	return TRUE;
}

bool CRecycleBinBehaviour::CanHandlePidl(CPidl& pidl, std::wstring)
{
	CPidl binPidl;
	binPidl.SetFromSpecialFolderLocation(CSIDL_BITBUCKET);

	return binPidl == pidl;
}

DWORD CRecycleBinBehaviour::OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter)
{
	ULARGE_INTEGER oldNumOfItems = m_NumItems;

	/*UpdateItems();

	if(oldNumOfItems.QuadPart == 0 || m_NumItems.QuadPart == NULL)
				return UPDATE_SUBLABEL | UPDATE_ICON;
			else
				return UPDATE_SUBLABEL;
		*/
	
	switch (event)
    {
		case SHCNE_CREATE              : //0x00000002L
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
		case SHCNE_FREESPACE           : //0x00040000L
			UpdateItems();
			
			if(oldNumOfItems.QuadPart == 0 || m_NumItems.QuadPart == NULL)
				m_Desklet->NotifyIconChange();

			m_Desklet->NotifySublabelChange();
		break;

		/*case SHCNE_UPDATEIMAGE         : //0x00008000L
			m_Desklet->NotifyIconChange();
        break;
		*/
    }

	return 0;
}

void CRecycleBinBehaviour::UpdateItems()
{
	m_NumItems.QuadPart = 0;
	m_SizeInBytes.QuadPart = 0;

	TCHAR drive[10];
	_tcscpy(drive,_T("A:\\"));

	// we now retrieve the information from all bins
	DWORD mask = {0};
	DWORD drives = GetLogicalDrives();
	for(int i=0; i < sizeof(drives) * 8; ++i)
	{
		mask <<= 1;
		if(true)///*(drives & mask) &&*/ GetDriveType(drive)==DRIVE_FIXED)
		{
			SHQUERYRBINFO binInfo = {0};
			binInfo.cbSize = sizeof(binInfo);
			SHQueryRecycleBin(drive,&binInfo);

			m_NumItems.QuadPart += binInfo.i64NumItems;
			m_SizeInBytes.QuadPart += binInfo.i64Size;
		}

		drive[0]++;
	}

	m_IsInit = true;
}

std::vector<std::wstring> CRecycleBinBehaviour::GetCustomSublabelItems()
{
	std::vector<std::wstring> items;
	items.push_back( L"%%\tpercentage sign");
	items.push_back( L"%#\tnewline");
	items.push_back( L"%u\tusername");
	items.push_back( L"%c\tcomputername");
	items.push_back( L"%s\ttotal size of all files");
	items.push_back( L"%n\tnumber of items");

	return items;
}

std::wstring CRecycleBinBehaviour::ParseSubLabel(CPidl& pidl,const std::wstring& format)
{
	std::wstringstream lbl;

	int i=0;
	int count( format.length() );
	while(i<count)
	{
		// the next character indicates that the user wants to have
		// some data inserted here
		if(format[i]==L'%')
		{
			i++;

			// n -> number of items
			if(format[i]==L'n')
			{
				if(!m_IsInit)UpdateItems();

				lbl << static_cast<DWORD>(m_NumItems.QuadPart);
			}

			// s -> total filesize of the items in the bin
			//		automatically scale it
			else if(format[i]=='s')
			{
				if(!m_IsInit)UpdateItems();

				ULARGE_INTEGER size;
				size.QuadPart = m_SizeInBytes.QuadPart;

				WCHAR conv[100] = {0};
				StrFormatByteSizeW(size.QuadPart,conv,sizeof(conv)/sizeof(conv[0]));
				lbl << conv;

				//std::wstring postfix =L" bytes";
				//if(size.QuadPart >= 1024){size.QuadPart/=1024;postfix=L" kB";};
				//if(size.QuadPart >= 1024){size.QuadPart/=1024;postfix=L" MB";};
				//if(size.QuadPart >= 1024){size.QuadPart/=1024;postfix=L" GB";};

				//lbl << static_cast<DWORD>(size.QuadPart) << postfix;
			}
			else
			{
				CShellItemBehaviour::ParseTokenAt(pidl,format,lbl,i);
			}
		}
		// this character just needs to be copied to the output label
		else
		{
			lbl << format[i];
		}
		i++;
	}

	return lbl.str();
}

std::wstring CRecycleBinBehaviour::GetCustomKeyName()
{
	if(!m_IsInit)
		UpdateItems();

	if(m_NumItems.QuadPart == 0)
		return L"Empty Bin";
	else
		return L"Full Bin";
}

std::map<std::wstring,std::wstring> CRecycleBinBehaviour::GetMapOfImageNames()
{
	std::map<std::wstring,std::wstring> map;
	map[L"Empty Bin"] = L"";
	map[L"Full Bin"] = L"";
	map[L"Eject"] = L"";

	return map;
}

BOOL CRecycleBinBehaviour::DragEnter(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect)
{
	int length(0);
	std::vector<std::wstring> filenames;

	m_InDeleteMode = (keys & MK_SHIFT) != 0;

	if(SUCCEEDED(CShellHelper::GetListOfFileNamesFromIDataObject(obj,filenames,length)) )
	{
		*effect = DROPEFFECT_MOVE;
		m_CanDrop = true;

		m_DropIsDrive = false;

		for(std::vector<std::wstring>::iterator iter = filenames.begin(); iter != filenames.end(); ++iter)
		{
			std::wstring fname = *iter;
			if(fname.length() < 4 )
			{
				UINT driveType = GetDriveTypeW(fname.c_str() );
				if(driveType == DRIVE_CDROM	 || driveType == DRIVE_REMOTE || driveType == DRIVE_REMOVABLE)
				{
					*effect = DROPEFFECT_LINK;
					m_DropIsDrive = true;
					m_Desklet->ImageFadeIn(L"Eject");
					break;
				}
				
			}
		}
	}
	else
	{
		FORMATETC etc = {0};
		etc.cfFormat = m_AveDeskDeskletFormat;
		etc.dwAspect = DVASPECT_CONTENT;
		etc.lindex = -1;
		etc.tymed = TYMED_HGLOBAL;

		if( SUCCEEDED(obj->QueryGetData(&etc)) )
		{
			*effect = DROPEFFECT_MOVE;
			m_CanDrop = true;
		}
		else
		{
			*effect = DROPEFFECT_NONE;
			m_CanDrop = false;
		}
	}

	return TRUE;
}

BOOL CRecycleBinBehaviour::DragOver(DWORD keys, POINTL pnt, DWORD *effect)
{
	m_InDeleteMode = (keys & MK_SHIFT) != 0;

	if(m_CanDrop)
	{
		if(m_DropIsDrive)
			*effect = DROPEFFECT_LINK;
		else
			*effect = DROPEFFECT_MOVE;
	}
	else
		*effect = DROPEFFECT_NONE;

	return TRUE;
}


void EjectDrive(const WCHAR* path)
{
	USES_CONVERSION;

    HANDLE hDisk = {0};
    DWORD dwRc = {0};
    TCHAR tsz[8] = {0};
    SECURITY_ATTRIBUTES sa = {0};

    wsprintf(tsz, TEXT("\\\\.\\%c:"), TEXT('@') + toupper(path[0]) - 'A' + 1);
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    hDisk =  CreateFile(tsz, GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,&sa,
                    OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
    if (hDisk != INVALID_HANDLE_VALUE)
    {
        FlushFileBuffers(hDisk);
        DeviceIoControl(hDisk,  IOCTL_DISK_EJECT_MEDIA	, NULL, 0, NULL, 0,&dwRc, NULL);
		CloseHandle(hDisk);
    }

    return;
}


INT __stdcall AskBinDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			CPidl pidl;
			pidl.SetFromSpecialFolderLocation(CSIDL_BITBUCKET);
			if(pidl.IsValid())
			{
				SetWindowTextW(hDlg, pidl.GetFriendlyName().c_str());
				HICON icon = pidl.ExtractIcon(48);
				SendDlgItemMessage(hDlg,IDC_BINICON,STM_SETIMAGE, static_cast<WPARAM>(IMAGE_ICON),reinterpret_cast<LPARAM>(icon));
				//DestroyIcon(icon);
			}
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
				case IDOK2:
				case IDOK3:
				case IDCANCEL:
					EndDialog(hDlg,LOWORD(wParam));
				break;
			}
		break;

		case WM_CLOSE:
			EndDialog(hDlg,IDCANCEL);
		break;

		case WM_DESTROY:
		{
			HICON icon = reinterpret_cast<HICON>(SendDlgItemMessage(hDlg,IDC_BINICON,STM_GETIMAGE,static_cast<WPARAM>(IMAGE_ICON),0L));
			if(icon != NULL)
				DestroyIcon(icon);
		}
		break;
	}

	return FALSE;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

BOOL CRecycleBinBehaviour::Drop(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect)
{
	std::vector<std::wstring> filenames;
	int length(0);
	HWND* deskletToRemove = NULL;
	STGMEDIUM medium = {0};

	FORMATETC etc = {0};
	etc.cfFormat = m_AveDeskDeskletFormat;
	etc.dwAspect = DVASPECT_CONTENT;
	etc.lindex = -1;
	etc.tymed = TYMED_HGLOBAL;

	bool removeFiles(true);
	bool removeDesklet(false);

	if( SUCCEEDED(obj->GetData(&etc,&medium)) )
	{
		removeDesklet = true;

		deskletToRemove = reinterpret_cast<HWND*>( GlobalLock(medium.hGlobal) );
		GlobalUnlock(medium.hGlobal);

		removeFiles = SUCCEEDED(CShellHelper::GetListOfFileNamesFromIDataObject(obj,filenames,length)) ? true : false;

		if(deskletToRemove && removeFiles)
		{
			int res = DialogBox(m_Desklet->GetInstanceHandle(),MAKEINTRESOURCE(IDD_ASKBIN),NULL,AskBinDlgProc);

			if(res == IDCANCEL)
			{
				*effect = DROPEFFECT_NONE;

				if(m_DropIsDrive)
					m_Desklet->ImageFadeOut();	

				return TRUE;
			}

			removeFiles		= (res == IDOK || res == IDOK2);
			removeDesklet	= (res == IDOK || res == IDOK3);
		}
	}
	else
	{
		if(filenames.size() == 0)
			removeFiles = SUCCEEDED(CShellHelper::GetListOfFileNamesFromIDataObject(obj,filenames,length)) ? true : false;
	}

	if(removeFiles)
	{
		bool ejectedDrive(false);
		for(std::vector<std::wstring>::iterator iter = filenames.begin(); iter != filenames.end(); ++iter)
		{
			UINT driveType = GetDriveTypeW(iter->c_str() );
			if(driveType == DRIVE_CDROM	 || driveType == DRIVE_REMOTE || driveType == DRIVE_REMOVABLE)
			{
				EjectDrive(iter->c_str());
				ejectedDrive = true;
			}
		}
		
		if(!ejectedDrive)
		{
			CPidl binPidl;
			binPidl.SetFromSpecialFolderLocation(CSIDL_BITBUCKET);

			CComPtr<IDropTarget> dropTarget;
			HRESULT hRes = binPidl.GetUIObject(m_Desklet->GetHWnd(),IID_IDropTarget,reinterpret_cast<void**>(&dropTarget));
			if(SUCCEEDED(hRes) && dropTarget.p != NULL)
				dropTarget->Drop(obj,keys,pnt,effect);
		}
		else
		{
			*effect = DROPEFFECT_MOVE;
		}
	}
	else
	{
		*effect = DROPEFFECT_NONE;
	}

	if(m_DropIsDrive)
		m_Desklet->ImageFadeOut();

	
	if(removeDesklet && deskletToRemove)
	{
		DeskletRemove(*deskletToRemove,false);	
	}

	return TRUE;
}

BOOL CRecycleBinBehaviour::DragLeave()
{
	if(m_DropIsDrive)
		m_Desklet->ImageFadeOut();

	return TRUE;
}