

#include <windows.h>				// trivial
#include "../DeskletSDK.h"			// for the deskletSDK
#include "../DrawPluginSDK.h"

#include <Gdiplus.h>				// for gdiplus stuff
using namespace Gdiplus;			// we will use the gdiplus namespace so we don't have to do Gdiplus::Bitmap,
									// but just Bitmap
#pragma comment(lib, "gdiplus.lib")	// link the gdiplus lib

#include <shellapi.h>				// for the recyclebin API
#include <shlwapi.h>
#include <shlobj.h>
#include <winbase.h>
#include <winioctl.h>


#include <string>					// for std::string
#include <sstream>					// for std::stringstream

#include "resource.h"				// trivial

// a randon number for our timer
#define QUERY_TIMER 2000101
#define FADER_TIMER 8860312

// the GUID of the recyclebin
#define RECYCLEBIN_GUID	"::{645FF040-5081-101B-9F08-00AA002F954E}"


BOOL GetItemIdListFromPath (LPWSTR lpszPath, LPITEMIDLIST *lpItemIdList)
{
   LPSHELLFOLDER pShellFolder = NULL;
   HRESULT         hr;
   ULONG         chUsed;

   // Get desktop IShellFolder interface
   if (SHGetDesktopFolder (&pShellFolder) != NOERROR)
      return FALSE;     // failed



   // convert the path to an ITEMIDLIST
   hr = pShellFolder->ParseDisplayName (
                  NULL,           // owner window
                  NULL,           // reserved (must be NULL)
                  lpszPath,       // folder name
                  &chUsed,    // number of chars parsed
                  lpItemIdList,   // ITEMIDLIST
                  NULL            // attributes (can be NULL)
               );
      
   if (FAILED(hr))
   {
      pShellFolder->Release();
      *lpItemIdList = NULL;
      return FALSE;
   }
   
   pShellFolder->Release();
   return TRUE;
}

// This class will hold the settings for this desklet and wraps the recyclebin functions
class RecycleBinDesklet
{
public:

	RecycleBinDesklet(HWND hwndDesklet, HINSTANCE hInstanceDll) :
		hwndDesklet(hwndDesklet),
		hInstanceDll(hInstanceDll),
		numItems(0),
		sizeInBytes(0),
		noRecycle(false),
		ask(true),
		isEmptying(false),
		isCD(false),
		fadeCounter(0),
		fadingIn(false),
		notifyId(0)
		{
			// post: an RecycleBinDesklet object has been created with the default settings

			strcpy(label,"%n file(s) - %s");

			DeskletGetRelativeFolder(hwndDesklet,imgEmpty);
			strcat(imgEmpty,"empty.png");

			DeskletGetRelativeFolder(hwndDesklet,imgFull);
			strcat(imgFull,"full.png");

			DeskletGetRelativeFolder(hwndDesklet,imgOverlay);
			strcat(imgOverlay,"eject.png");
		}

	// all the fields are public, because I was too lazy to encaps. them the right way ;-)

	HWND hwndDesklet;					// the HWND that identifies this desklet		
	HINSTANCE hInstanceDll;				// the instance of this DLL

	int numItems;						// the current number of items in the bin
	__int64 sizeInBytes;					// the current total size of all the files in the bin

	char label[2000];					// the 'raw'-label as defined by the user

	char imgEmpty[MAX_PATH];			// relative path for the image shown when there are NO imems in the bin
	char imgFull[MAX_PATH];				// relative path for the image shown when there are items in the bin
	char imgOverlay[MAX_PATH];			// relative path for the image shown when we hoover over an CD/DVD drive

	bool noRecycle;						// if true, files are not moved to the bin, but deleted
	bool ask;							// if true, we ask the user for confirmation if he drops files / empty the bin

	bool isEmptying;					// a flag stating if we are emptying the bin

	bool isCD;							// a flag stating if the current drop includes a CD
	int  fadeCounter;					// a counter used for fading out layers
	bool fadingIn;						// a flag stating if we are fading in

	LONG notifyId;

	// read settings from ini file
	void ReadFromIni(char* ini, char* group);
	//save settings to ini file
	void SaveToIni(char* ini, char* group);

	// queries the recyclebin and update the data in the object. returns true if there are changes
	bool QueryRecycleBin();
	// parses and updates the label
	void ParseLabel();
	//updates the image
	void UpdateImage();
	// empties the recyclebin
	void EmptyRecycleBin();
};

void RecycleBinDesklet::UpdateImage()
{
	// post: the desklets image has been updated to reflect the number of items
	//		 in the recyclebin (empty vs. not-empty)
	if(numItems==0)
		DeskletSetImageFile(hwndDesklet,imgEmpty);
	else
		DeskletSetImageFile(hwndDesklet,imgFull);
}

void RecycleBinDesklet::ParseLabel()
{
	// post: the desklets label has been updated
	//		 according to how the user did set up the label


	std::stringstream lbl;
	int i=0;
	int l=strlen(label);

	// the maximum length for the label is 2000 characters
	while(i<l && i<2000)
	{
		// the next character indicates that the user wants to have
		// some data inserted here
		if(label[i]=='%')
		{
			i++;

			// n -> number of items
			if(label[i]=='n')
			{
				lbl << numItems;
			}
			// s -> total filesize of the items in the bin
			//		automatically scale it
			else if(label[i]=='s')
			{
				__int64 size = sizeInBytes;
				std::string postfix =" bytes";
				if(size >= 1024){size/=1024;postfix=" kB";};
				if(size >= 1024){size/=1024;postfix=" MB";};
				if(size >= 1024){size/=1024;postfix=" GB";};

				lbl << static_cast<int>(size) << postfix;
			}
			// % -> percentage sign
			else if(label[i]=='%')
			{
				lbl << "%";
			}
			// # -> newline character
			else if(label[i]=='#')
			{
				lbl << "\n"; 
			}
		}
		// this character just needs to be copied to the output label
		else
		{
			lbl << label[i];
		}
		i++;
	}
	// finally, set the label
	DeskletSetSubLabel(hwndDesklet,const_cast<char*>(lbl.str().c_str()));
}

bool RecycleBinDesklet::QueryRecycleBin()
{
	// post: true has been returned iff the recyclebin status has been changed
	//		 regarding the last query

	// if we are emptying the bin, do not retrieve any information, otherwise the 
	// desklet might crash
	if(isEmptying)return false;

	__int64 oldNumItems = numItems;
	__int64 oldSizeInBytes = sizeInBytes;

	numItems = 0;
	sizeInBytes = 0;

	char drive[10];
	strcpy(drive,"c:\\");

	// we now retrieve the information from all bins
	while(GetDriveType(drive)==DRIVE_FIXED)
	{
		SHQUERYRBINFO binInfo = {0};
		binInfo.cbSize = sizeof(binInfo);
		SHQueryRecycleBin(drive,&binInfo);

		numItems += (signed __int64) binInfo.i64NumItems;
		sizeInBytes += binInfo.i64Size;

		drive[0]++;
	}

	return oldNumItems != numItems || oldSizeInBytes != sizeInBytes;
}

void EjectDrive(char *path)
{
    HANDLE hDisk;
    DWORD dwRc;
    TCHAR tsz[8];
    SECURITY_ATTRIBUTES sa;

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
    }

    return;
}

void UnmountDrive(char *path)
{
    HANDLE hDisk;
    DWORD dwRc;
    TCHAR tsz[8];
    SECURITY_ATTRIBUTES sa;

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
		DeviceIoControl(hDisk,  IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0,&dwRc, NULL);
        DeviceIoControl(hDisk,  FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0,&dwRc, NULL);
    }

    return;
}

void RecycleBinDesklet::EmptyRecycleBin()
{
	// post: the recyclebin has been emptied

	if(ask && 
		MessageBox(hwndDesklet,"Are you sure you want to empty the recycle bin?","Emtpy Recycle Bin",
		MB_ICONQUESTION |MB_YESNO)==IDNO)
	{
		// if the user cancels the action, exit the method
		return;
	}

	// set a flag that states we are emptying the recyclebin to
	// avoid us from retrieving data from the bin while emptying
	isEmptying = true;

	numItems = 0;
	sizeInBytes = 0;

	// we will now empty each bin on every FIXED drive.
	char drive[10];
	strcpy(drive,"c:\\");

	while(GetDriveType(drive)==DRIVE_FIXED)
	{
		SHQUERYRBINFO binInfo = {0};
		binInfo.cbSize = sizeof(binInfo);
		SHQueryRecycleBin(drive,&binInfo);

		if(binInfo.i64NumItems != 0)
		{
			// let window empty the bins folder for this drive IF there are items in the bin
			SHEmptyRecycleBin(hwndDesklet,drive,SHERB_NOSOUND | SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI);
		}
		
		// go to the next drive
		drive[0]++;
	}

	// update the desklet
	ParseLabel();
	UpdateImage();

	// unset the flag we previously did set
	isEmptying = false;
}

void RecycleBinDesklet::ReadFromIni(char* ini, char* group)
{
	// pre:  ini and group are not 0
	// post: the settings has been retrieved from an ini file

	if(!ini || !group)return;

	GetPrivateProfileString(group,"LabelData",label,label,sizeof(label)/sizeof(label[0]),ini);
	GetPrivateProfileString(group,"ImgEmpty",imgEmpty,imgEmpty,sizeof(imgEmpty)/sizeof(imgEmpty[0]),ini);
	GetPrivateProfileString(group,"ImgFull",imgFull,imgFull,sizeof(imgFull)/sizeof(imgFull[0]),ini);
	GetPrivateProfileString(group,"ImgOverlay",imgOverlay,imgOverlay,sizeof(imgOverlay)/sizeof(imgOverlay[0]),ini);

	noRecycle	= GetPrivateProfileInt(group,"NoRecycle",FALSE,ini) == TRUE;
	ask			= GetPrivateProfileInt(group,"AskRemove",TRUE,ini) == TRUE;
}

void RecycleBinDesklet::SaveToIni(char* ini, char* group)
{
	// pre:	 ini and group are not 0
	// post: the settings has been loaded from an ini file

	if(!ini || !group)return;

	WritePrivateProfileString(group,"LabelData",label,ini);
	WritePrivateProfileString(group,"ImgEmpty",imgEmpty,ini);
	WritePrivateProfileString(group,"ImgFull",imgFull,ini);
	WritePrivateProfileString(group,"ImgOverlay",imgOverlay,ini);

	WritePrivateProfileInt(group,"NoRecycle",noRecycle ? TRUE : FALSE,ini);
	WritePrivateProfileInt(group,"AskRemove",ask ? TRUE : FALSE,ini);
}

//	This function is called whenever AveDesk requests information about the desklet. This function
//	is not associated with a particular instance of this desklet and it therefore most of the times
//	looks exactly like the function below.
void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	// copy the name, author, notes and version number
	strcpy(szName,"Recyclebin Desklet");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"A desklet that will act as the Recycle Bin. The label can be configured. Thanks to Albeik for the drive-ejection code!");
	*iVersion = 110;
}

//	The OnCreate function is called when a new instance of this desklet is created. If you use a class for
//	every instance - like in this RecycleBinDesklet - you should create a new instance of that class and return
//	it. That pointer to the new instance of your class will be the first parameter of every other event function.
//	The hwndDesklet parameter identifies the desklet instance and is therefore needed in every callback function
//	like DeskletSetLabel.
//	The hInstance parameter is a handle to the instance of this desklets DLL which you need for resources etc...
//	The szIni and szIniGroup can be used to retrieve settings stored by the OnSave event. These parameters are 0
//	if the user adds this desklet for the first time.
RecycleBinDesklet* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	// create a new instance of the RecycleBinDesklet desklet class that holds
	// the settings for this desklet instance.
	RecycleBinDesklet* lpData = new RecycleBinDesklet(hwndDesklet,hInstance);

	// activate extended drag-'n-drop (OLE d&d) so we can accept
	// file and desklet drops
	DeskletActivateExtendedDragDrop(hwndDesklet);


	if(szIni && szIniGroup)
	{
		// load the settings previously saved by a call to OnSave
		// here, the RecycleBinDesklet class handles the loading of settings
		lpData->ReadFromIni(szIni,szIniGroup);
	}
	else
	{
		// here we could have set default settings for this desklet
	}

	// update the desklet
	lpData->QueryRecycleBin();
	lpData->ParseLabel();
	lpData->UpdateImage();

	// set a timer that queries the recyclebin status every two seconds
	SetTimer(hwndDesklet,QUERY_TIMER,2000,0);

	return lpData;
}

//	This function is called when a desklet instance is destroyed either by
//	the user manually removing it or by AveDesk closing.
//	You should release all memory, stop all threads etc... here. Also, do not
//	forget to release the memory that you allocated in the OnCreate.
void __stdcall OnDestroy(RecycleBinDesklet *lpData, HWND hwndDesklet)
{
	// kill the timer that queries the recyclebin
	KillTimer(lpData->hwndDesklet,QUERY_TIMER);

	// delete the instance of RecycleBinDesklet created in the OnCreate function
	delete lpData;
	lpData = 0;
}


//	This function is called whenever the status of this desklet instance is saved to an INI-file.
//	szIni and szIniGroup can be used in calls to WritePrivateProfile*() if you want to save
//	settings.
//	bIsForExport is to be compatible with object dock docklets.
void __stdcall OnSave(RecycleBinDesklet *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	// here, the RecycleBinDesklet class handles the saving of settings
	lpData->SaveToIni(szIni,szIniGroup);
}

//	This function is called when a message has been sent to the desklets window. It's just like a regular WINPROC
//	(see WIN32 SDK for that), so you can use it to handle timer events etc...
void __stdcall OnProcessMessage(RecycleBinDesklet* lpData,HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// here, we catch the OUR timer messages that tells us to query the recyclebin
	if(msg==WM_TIMER)
	{
		if(wParam == QUERY_TIMER)
		{
			if(lpData->QueryRecycleBin())
			{
				// only if the bin was updated, update the label and image
				lpData->ParseLabel();
				lpData->UpdateImage();
			}
		}
		else if( wParam == FADER_TIMER)
		{

			// first we search for the overlay layer named RecycleBin.EjectOverlay
			BOOL success(FALSE);
			unsigned int id = DeskletLayerGetLayer(lpData->hwndDesklet,"RecycleBin.EjectOverlay",&success);


			if(!success)
			{
				// sucess is false, so the layer was not found. In that case we need to create a layer

				// add a layer and store it's id
				id = DeskletLayerAdd(lpData->hwndDesklet,"RecycleBin.EjectOverlay");

				// we load the eject.png image into this layer also
				// note that we don't let the desklet redraw here yet by setting
				// the redraw parameter to false
				DeskletLayerSetImageFile(lpData->hwndDesklet,id,false,lpData->imgOverlay);
			}

			// next we set the alpha level for the eject-overlay layer ( id)
			// note that we don't let the desklet redraw here yet by setting
			// the redraw parameter to false
			DeskletLayerSetAlpha(lpData->hwndDesklet,id,false,lpData->fadeCounter*13);

			// and the background layer, which always has an id of 0
			// we now set the redraw parameter to true, to redraw the desklet with its new
			// settings for the layers
			DeskletLayerSetAlpha(lpData->hwndDesklet,0,true,255 - lpData->fadeCounter*13);

			if(lpData->fadingIn)
			{
				// we are fading in, so counter must be decreased
				--lpData->fadeCounter;

				if(lpData->fadeCounter == 0)
				{
					// counter reached 0, stop the timer and reset the layers

					KillTimer(lpData->hwndDesklet,FADER_TIMER);

					// we can remove the overlay-layer, cause we no longer need it
					DeskletLayerRemove(lpData->hwndDesklet,id,false);
					DeskletLayerSetAlpha(lpData->hwndDesklet,0,true,255);
				}
			}
			else
			{
				// etc...

				++lpData->fadeCounter;
				if(lpData->fadeCounter == 15)
				{
					KillTimer(lpData->hwndDesklet,FADER_TIMER);
					DeskletLayerSetAlpha(lpData->hwndDesklet,id,true,255);	
				}
			}
		}
	}
}

//	This function is called when the user left clicks a desklet. Note that this can be either a single- or
//	double-click depending on the users settings.
//	p and s are obvious.
BOOL __stdcall OnLeftButtonClick(RecycleBinDesklet *lpData, POINT* p, SIZE *s)
{
	// we open the recyclebin in explorer here.
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.hwnd = NULL;
	sei.lpVerb = NULL;
	sei.lpFile = RECYCLEBIN_GUID;
	sei.lpParameters = "";
	sei.lpDirectory = "";
	sei.nShow = SW_SHOWNORMAL;
	ShellExecuteEx(&sei);

	// return TRUE to indicate that we implement this function.
	return TRUE;
}

//	This function is called when the user right-clicks a desklet. If we don't implement this or return FALSE
//	AveDesk will show a default menu.
//	If you want to show your own rightclick menu, please use DeskletGetDefaultMenu() to also include the
//	default menu to conform with the design guidelines.
BOOL CALLBACK OnRightButtonClick(RecycleBinDesklet *lpData, POINT* p, SIZE *s)
{
	// First, get the default menu. All default menu items will have an ID of at least 20,000
	HMENU menu = DeskletGetDefaultMenu(lpData->hwndDesklet,20000);

	// Add our own entries to the menu
	InsertMenu(menu,0,MF_BYPOSITION | (lpData->numItems!=0 ? MF_ENABLED : MF_DISABLED ) | MF_STRING,1,"Empty Recycle Bin");
	InsertMenu(menu,1,MF_BYPOSITION | MF_SEPARATOR,0,"-");
	
	// We show the menu at the position where the mouse was at the time the user right-clicked.
	// Also, we store the return value of TrackPopupMenu() to determine which action the user performed
	POINT pCur;
	GetCursorPos(&pCur);
	int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pCur.x, pCur.y, 0, lpData->hwndDesklet,0);

	// We can now destroy the menu as it is no longer needed
	DestroyMenu(menu);

	if(cmd >= 20000)
	{
		// The user performed an action of the default menu (because the ID of the menu-item
		// is greater than 20,000 ), so we let AveDesk handle the action.
		DeskletDoDefaultMenuCommand(lpData->hwndDesklet,20000,cmd);
	}
	else
	{
		// the user performed one of our own actions. Check which action he performed, and than execute it.
		switch(cmd)
		{
			case 1:
				lpData->EmptyRecycleBin();
			break;
		}
	}

		// return TRUE to indicate that we show our own menu and don't want the default menu to pop up
		return TRUE;
}

//	This function is called when a user performs a drag over the desklet, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDragEnter(RecycleBinDesklet* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	// for now, we accept all drops. We actually let the onDrop function handle this.
	// We return TRUE to indicate that we implemented this function.
	// If we want the drag to be not happening, we should set an internal flag, check for it in the OnDragOver()
	// and modify the effect flag to DROPEFFECT_NONE.

	if(IsDataObjectHDROP(obj))
	{
		bool hasCd(false);
		DropStruct* ds = LockHDROP(obj);
		
		int num = DragQueryFile(ds->hDrop, -1, 0, 0);

		char file[MAX_PATH];

		int i=0;
		while(i< num && !hasCd)
		{
			DragQueryFile(ds->hDrop,i,file,MAX_PATH);
			hasCd = GetDriveType(file) == DRIVE_CDROM || GetDriveType(file) == DRIVE_REMOVABLE	&& strlen(file) < 4;
			i++;
		}

		UnlockHDROP(ds);



		if(lpData->isCD = hasCd )
		{
			lpData->fadingIn = false;
			SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
		}
	}

	return TRUE;
}

//	This function is called when a user leaves a drag over the desklet, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDragLeave(RecycleBinDesklet* lpData)
{
	if(lpData->isCD)
	{
		lpData->fadingIn = true;
		SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
	}
	return TRUE;
}

//	This function is called when a user performs a drag over the desklet, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDragOver(RecycleBinDesklet* lpData,DWORD keys, POINTL pnt, DWORD* effect)
{
	// see OnDragEnter.
	return TRUE;
}

//	This function is called when a user drops something on this deskelt, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDrop(RecycleBinDesklet* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{

	// the IsDataObjectHDROP helper function that is in the AD1.1 SDK, helps us to determine
	// if the user dropped a file
	if(IsDataObjectHDROP(obj))
	{
		// the user dropped a file, so we can try to delete it
	
		// first, ask the user for confirmation if he checked the "Ask" option.
		bool doDel = true;
		if(lpData->ask)
		{
			if(lpData->noRecycle)
				doDel = MessageBox(lpData->hwndDesklet,"Are you sure you want to delete these file(s)?","Recyclebin",MB_ICONQUESTION | MB_YESNO | MB_SYSTEMMODAL) == IDYES;
			else
				doDel = MessageBox(lpData->hwndDesklet,"Are you sure you want to move these file(s) to the recyclebin?","Recyclebin",MB_ICONQUESTION | MB_YESNO | MB_SYSTEMMODAL) == IDYES;
		}

		// the user wants really to remove the file
		if(doDel)
		{
			// get a DropStruct object by using the LockHDROP() helper function from the AD1.1 SDK
			// These helper functions makes it easier for the programmer to retrieve an HDROP from an IDataObject.
			DropStruct* ds = LockHDROP(obj);
	
			// get the number of files in the HDROP
			int num = DragQueryFile(ds->hDrop, -1, 0, 0);
	
			// now create a string where all files are in, seperated by a 0-character
			char* pFrom = new char[num*MAX_PATH+1];
			char* pFromCur = pFrom;
	
			int count = 0;

			for(int i=0;i<num;i++)
			{
				DragQueryFile(ds->hDrop,i,pFromCur,MAX_PATH);

				if (GetDriveType(pFromCur) == DRIVE_CDROM)
				{					
					EjectDrive(pFromCur);
				}
				else if(GetDriveType(pFromCur)==DRIVE_REMOVABLE && strlen(pFromCur) < 4)
				{
					UnmountDrive(pFromCur);
				}
				else
				{
					pFromCur = pFromCur + DragQueryFile(ds->hDrop,i,0,0) +1;
					count++;
				}
			}
	
			if(count > 0)
			{
				*pFromCur = 0;

				// let the shell do the removal
				SHFILEOPSTRUCT FileOp = {0};
				FileOp.hwnd = lpData->hwndDesklet;
				FileOp.wFunc = FO_DELETE;
				FileOp.pFrom = pFrom;
				FileOp.pTo = NULL;
				FileOp.fFlags = ((lpData->noRecycle) ? 0 : FOF_ALLOWUNDO) | FOF_NOCONFIRMATION;
				FileOp.fAnyOperationsAborted = FALSE;
				FileOp.hNameMappings = NULL;
				FileOp.lpszProgressTitle = NULL;
				SHFileOperation(&FileOp);
			}
	
			// release the memory we used for the string
			delete[] pFrom;

			// and finally, unlock the DropStruct, so the memory will be released
			UnlockHDROP(ds);

		}

		if(lpData->isCD)
		{
			lpData->fadingIn = true;
			SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
		}

	}
	else
	{
		// there where no files dropped, so there was no effect from this drop
		*effect = DROPEFFECT_NONE;
	}

	// We will also handle desklets being dropped.

	// First get an appropriate format for the desklet; every desklet can be dragged by default
	// and can handle the AveDeskDesklet-format. All this is done by AveDesk.
	FORMATETC f = {0};
	f.cfFormat = RegisterClipboardFormat("AveDeskDesklet");
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	

	// try get the data for the AveDeskDesklet format
    STGMEDIUM medium; 
    HRESULT hr = obj->GetData(&f, &medium); 

    if (!FAILED(hr)) 
    { 
		// the IDataObject was capable of giving us data for the AveDeskDesklet format, so
		// now we can try to remove the desklet.

		// first, we get a HANDLE to the memory holding the data for this desklet-drop.
		// The data is actually just a HWND that identifies the desklet, therefore we can use
		// the callback functions with this HWND;
	    HGLOBAL hHwnd = medium.hGlobal;
		HWND* pHwnd = (HWND*) GlobalLock(hHwnd);

		// We now use the DeskletRemove callback function for the desklet that was dropped.
		// Note that the use of this function also allows use to drop the recyclebin in itself,
		// because DeskletRemove does have a little delay before the actuall removal is done.

		if(lpData->ask && MessageBox(lpData->hwndDesklet,"Remove this Desklet?","Trash",MB_ICONQUESTION | MB_YESNO | MB_SYSTEMMODAL)==IDYES)
		{
			int numFrames = 30;

			SIZE s = {0};
			SIZE ourSize = {0};
			DeskletGetSize(lpData->hwndDesklet,&s);
			DeskletGetSize(*pHwnd,&s);
			float cxRatio = s.cx / numFrames;
			float cyRatio = s.cy / numFrames;

			POINT p = {0};
			POINT ourPos = {0};
			DeskletGetPosition(lpData->hwndDesklet,&ourPos);
			DeskletGetPosition(*pHwnd,&p);
			float xRatio = (p.x - ourPos.x-s.cx/2) / numFrames; 
			float yRatio = (p.y - ourPos.y-s.cy /2) / numFrames;

			float x = p.x;
			float y = p.y;
			float w = s.cx;
			float h = s.cy;

			BYTE iA = DeskletGetAlpha(*pHwnd);

			float a = iA;
			float aRatio = iA / numFrames;

			RECT r = {0};

			for(int i=0;i<numFrames;i++)
			{
				x -= xRatio;
				y -= yRatio;
				w -= cxRatio;
				h -= cyRatio;
				r.left = x;
				r.top = y;
				r.right = x+w;
				r.bottom = y+h;
				a -= aRatio;
				if(a<30)a=30;
				DeskletMove(*pHwnd,&r,FALSE);
				DeskletSetAlpha(*pHwnd,(BYTE)a);
				SleepEx(20,TRUE);
			}
			DeskletRemove(*pHwnd,false);
		}

		// release the memory used
		GlobalUnlock(hHwnd); 
		ReleaseStgMedium(&medium); 

		return TRUE;
    } 
    else 
    {  
		// again, no effect
	   *effect = DROPEFFECT_NONE;
       return FALSE;
    }
}

LRESULT __stdcall FrameSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_DROPFILES)
	{
		HDROP drop = (HDROP)wParam;
		int numFiles = DragQueryFile(drop,-1,0,0);
		if(numFiles > 0)
		{
			char* img = (char*) GetProp(hwnd,"img");

			char path[MAX_PATH];
			DragQueryFile(drop,0,path,sizeof(path)/sizeof(path[0]));

			char drive[MAX_PATH];
			char folder[MAX_PATH];
			char file[MAX_PATH];
			char ext[MAX_PATH];
			_splitpath(path,drive,folder,file,ext);

			if(stricmp(ext,".png") == 0 || stricmp(ext,".ico") == 0)
			{
				strcpy(img,path);
	
				InvalidateRect(hwnd,0,TRUE);
				PropSheet_Changed(GetParent(GetParent(hwnd)),GetParent(hwnd));
			}
		}
		DragFinish(drop);
		return TRUE;
	}
	else if( msg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if ( msg == WM_PAINT)
	{
		WNDPROC oldProc = (WNDPROC) GetProp(hwnd,"old_wndproc");
		CallWindowProc(oldProc,hwnd,msg,wParam,lParam);

		char* imgEmpty = (char*) GetProp(hwnd,"img");

		PAINTSTRUCT ps;
		HDC dc = BeginPaint(hwnd,&ps);

		RECT r = {0};

			Pen p(Color(255,0,0,0),1);

			GetClientRect(hwnd,&r);
			Bitmap  bufEmpty(r.right - r.left, r.bottom - r.top);
			Bitmap* bmpEmpty = DeskletLoadGDIPlusImage(imgEmpty);
			Graphics emptyG(&bufEmpty);
			emptyG.Clear(Color(255,255,255,255));
			emptyG.DrawRectangle(&p,0,0,bufEmpty.GetWidth() -4 ,bufEmpty.GetHeight()-4);
			emptyG.DrawImage(bmpEmpty,0,0,bufEmpty.GetWidth()-4,bufEmpty.GetWidth()-4);
			delete bmpEmpty;
			Graphics screenEmpty(hwnd);
			screenEmpty.DrawImage(&bufEmpty,2,2);

			ReleaseDC(hwnd,dc);
			EndPaint(hwnd,&ps);

			return TRUE;
	}

	else
	{
		WNDPROC oldProc = (WNDPROC) GetProp(hwnd,"old_wndproc");
		return CallWindowProc(oldProc,hwnd,msg,wParam,lParam);
	}
}


//	This is a DLGPROC for the Label propertysheetpage.
int __stdcall LabelFormatConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			// store the RecycleBinDesklet-pointer of this desklet-instance in a WindowProperty, so
			// we can use it in other calls

			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			RecycleBinDesklet* lpData = (RecycleBinDesklet*)GetProp(hDlg,"lpData");
			
			// set the edit-box for the label
			SetDlgItemText(hDlg,IDC_LABEL,lpData->label);

			if(!lpData)return TRUE;
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_LABEL:
					// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				// Apply is pressed, so commit the changes.
				case PSN_APPLY:
				{
					RecycleBinDesklet* lpData = (RecycleBinDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						// first get a pointer to the RecycleBinDesklet object associated with this desklet instance.
						RecycleBinDesklet* lpData = (RecycleBinDesklet*)GetProp(hDlg,"lpData");

						// update the label-member and update the label.
						GetDlgItemText(hDlg,IDC_LABEL,lpData->label,sizeof(lpData->label)/sizeof(lpData->label[0]));
						lpData->ParseLabel();
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
			// remove the property from the window
			RemoveProp(hDlg,"lpData");
		break;
	}
	return FALSE;
}

// same except for Other sheet
int __stdcall OtherConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			RecycleBinDesklet* lpData = (RecycleBinDesklet*)GetProp(hDlg,"lpData");
			
			CheckDlgButton(hDlg,IDC_ASK,lpData->ask ? TRUE : FALSE);
			CheckDlgButton(hDlg,IDC_NORECYCLE,lpData->noRecycle ? TRUE : FALSE);

			if(!lpData)return TRUE;
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_ASK: case IDC_NORECYCLE:
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
					RecycleBinDesklet* lpData = (RecycleBinDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						lpData->ask = IsDlgButtonChecked(hDlg,IDC_ASK) == BST_CHECKED;
						lpData->noRecycle = IsDlgButtonChecked(hDlg,IDC_NORECYCLE) ==BST_CHECKED;
					}
				break;
			}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
		break;
	}
	return FALSE;
}

// same except for images sheet
int __stdcall ImagesConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	static char imgEmpty[MAX_PATH];
	static char imgFull[MAX_PATH];
	static char imgOverlay[MAX_PATH];

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			RecycleBinDesklet* lpData = (RecycleBinDesklet*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

			strcpy(imgEmpty,lpData->imgEmpty);
			strcpy(imgFull,lpData->imgFull);
			strcpy(imgOverlay,lpData->imgOverlay);

			SetProp(GetDlgItem(hDlg,IDC_EMPTYIMG),"img",(HANDLE)imgEmpty);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_EMPTYIMG),GWL_WNDPROC,(LONG)FrameSubclassProc));

			SetProp(GetDlgItem(hDlg,IDC_FULLIMG),"img",(HANDLE)imgFull);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_FULLIMG),GWL_WNDPROC,(LONG)FrameSubclassProc));

			SetProp(GetDlgItem(hDlg,IDC_EJECTIMG),"img",(HANDLE)imgOverlay);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_EJECTIMG),GWL_WNDPROC,(LONG)FrameSubclassProc));

			DragAcceptFiles(GetDlgItem(hDlg,IDC_EMPTYIMG),TRUE);
			DragAcceptFiles(GetDlgItem(hDlg,IDC_EJECTIMG),TRUE);
			DragAcceptFiles(GetDlgItem(hDlg,IDC_FULLIMG),TRUE);

			init = false;
			InvalidateRect(hDlg,0,TRUE);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDSETEMPTY:
					if(DeskletBrowseForImage(hDlg,imgEmpty))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
					
				break;

				case IDSETFULL:
					if(DeskletBrowseForImage(hDlg,imgFull))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				break;

				case IDSETEJECT:
					if(DeskletBrowseForImage(hDlg,imgOverlay))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				break;
			}
		break;
/*
		case WM_PAINT:
			if(!init)
			{
				PAINTSTRUCT ps;
				HDC dc = BeginPaint(hDlg,&ps);

				HWND hwndEmpty = GetDlgItem(hDlg,IDC_EMPTYIMG);
				HWND hwndFull = GetDlgItem(hDlg,IDC_FULLIMG);
				HWND hwndOverlay = GetDlgItem(hDlg,IDC_EJECTIMG);
				RECT r = {0};

				GetClientRect(hwndEmpty,&r);
				Bitmap  bufEmpty(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpEmpty = DeskletLoadGDIPlusImage(imgEmpty);
				Graphics emptyG(&bufEmpty);
				emptyG.Clear(Color(255,255,255,255));
				emptyG.DrawImage(bmpEmpty,0,0,bufEmpty.GetWidth()-4,bufEmpty.GetWidth()-4);
				delete bmpEmpty;
				Graphics screenEmpty(hwndEmpty);
				screenEmpty.DrawImage(&bufEmpty,2,2);

				GetClientRect(hwndFull,&r);
				Bitmap  bufFull(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpFull = DeskletLoadGDIPlusImage(imgFull);
				Graphics FullG(&bufFull);
				FullG.Clear(Color(255,255,255,255));
				FullG.DrawImage(bmpFull,0,0,bufFull.GetWidth()-4,bufFull.GetWidth()-4);
				delete bmpFull;
				Graphics screenFull(hwndFull);
				screenFull.DrawImage(&bufFull,2,2);

				GetClientRect(hwndOverlay,&r);
				Bitmap  bufOverlay(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpOverlay = DeskletLoadGDIPlusImage(imgOverlay);
				Graphics OverlayG(&bufOverlay);
				OverlayG.Clear(Color(255,255,255,255));
				OverlayG.DrawImage(bmpOverlay,0,0,bufOverlay.GetWidth()-4,bufOverlay.GetWidth()-4);
				delete bmpOverlay;
				Graphics screenOverlay(hwndOverlay);
				screenOverlay.DrawImage(&bufOverlay,2,2);

				ReleaseDC(hDlg,dc);
				EndPaint(hDlg,&ps);
			}
		break;*/

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
				{
					RecycleBinDesklet* lpData = (RecycleBinDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						strcpy(lpData->imgEmpty,imgEmpty);
						strcpy(lpData->imgFull,imgFull);
						strcpy(lpData->imgOverlay,imgOverlay);
						lpData->UpdateImage();
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
			SetWindowLong(GetDlgItem(hDlg,IDC_EMPTYIMG),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_EMPTYIMG),"old_wndproc"));
			SetWindowLong(GetDlgItem(hDlg,IDC_FULLIMG),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_FULLIMG),"old_wndproc"));
			SetWindowLong(GetDlgItem(hDlg,IDC_EJECTIMG),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_EMPTYIMG),"old_wndproc"));
			RemoveProp(hDlg,"lpData");

		break;
	}
	return FALSE;
}


//	This function is called before the configuration dialogue for this desklet is shown, allowing us
//	to add our own propertsheets to the dialogue (see WIN32 SDK for more on this).
//	We should return the number of propertysheets that we want to see present.
//	Here, we add 3 propertysheets.
INT __stdcall OnAddPropertySheets(RecycleBinDesklet* lpData, int numPresent)
{
	// first, set all the members for this PropertySheetPage.
	PROPSHEETPAGE psp = {0};
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Label Format";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGLABELFORMAT);
	psp.pfnDlgProc = LabelFormatConfigProc;
	psp.lParam = (LPARAM)lpData;

	// Adding the propertysheet is done by using the DeskletAddPropertySheet () callback function
	// with the result of the WIN32 CreatePropertySheetPage() API as the parameter.
	// DeskletAddPropertySheet() returns the number of propertysheets that will be shown.
	int res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	// idem for the other propertypages.

	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Images";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGIMAGES);
	psp.pfnDlgProc = ImagesConfigProc;
	psp.lParam = (LPARAM)lpData;
	res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Other Options";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGOTHER);
	psp.pfnDlgProc = OtherConfigProc;
	psp.lParam = (LPARAM)lpData;

	res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	return res;
}

