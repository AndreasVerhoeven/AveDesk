

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

#define WM_SETPATH WM_USER + 202

#define FADER_TIMER 8860312

DWORD WINAPI WorkerThread(LPVOID lpData);

struct DirInfo
{
	int		numFiles;
	int		numFolders;
	float	byteSize;
};

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

void GetFileName(const char* filepath, char* buffer, int buffersize)
{
	// pre:  filepath and buffer are not 0, buffersize is the maximum size of chars copied
	// post: the filename in filepath is stored in buffer, this means that from filepath
	//			c:\blabla\yada.png buffer is filled with yada.png

	if(!filepath || !buffer)return;

	int i=strlen(filepath)-1;
	while( i >= 0 && buffersize-- >0 && filepath[i]!='\\' && filepath[i]!='/')i--;
	strcpy(buffer,filepath+i+1);
}

// This class will hold the settings for this desklet and wraps the recyclebin functions
class FolderDesklet
{
public:

	FolderDesklet(HWND hwndDesklet, HINSTANCE hInstanceDll) :
		hwndDesklet(hwndDesklet),
		hInstanceDll(hInstanceDll),
		numFiles(0),
		numFolders(0),
		sizeInBytes(0),
		isFileDrop(false),
		fadeCounter(0),
		fadingIn(false),
		itemId(0),
		notifyId(0),
		autoCaption(true),
		doDragDrop(true),
		hWorker(0),
		shouldStop(false)
		{
			// post: an FolderDesklet object has been created with the default settings
			strcpy(path,"");

			strcpy(label,"%f file(s) - %d folder(s)");

			DeskletGetRelativeFolder(hwndDesklet,imgOverlay);
			strcat(imgOverlay,"images\\open.png");
		}

		~FolderDesklet();

	// all the fields are public, because I was too lazy to encaps. them the right way ;-)
	char path[MAX_PATH];				// the path to which this folder points
	LPITEMIDLIST itemId;
	ULONG	notifyId;

	HWND hwndDesklet;					// the HWND that identifies this desklet		
	HINSTANCE hInstanceDll;				// the instance of this DLL

	int numFiles;						// the current number of items in the bin
	int numFolders;						// num of folders
	float sizeInBytes;					// the current total size of all the files in the bin

	char label[2000];					// the 'raw'-label as defined by the user

	//char imgEmpty[MAX_PATH];			// relative path for the image shown when there are NO imems in the bin
	char imgOverlay[MAX_PATH];			// relative path for the image shown when we hoover over an CD/DVD drive

	bool isFileDrop;							// a flag stating if the current drop includes a CD
	int  fadeCounter;					// a counter used for fading out layers
	bool fadingIn;						// a flag stating if we are fading in

	bool autoCaption;
	bool doDragDrop;

public:
	HANDLE hWorker;
	bool shouldStop;
	

	// read settings from ini file
	void ReadFromIni(char* ini, char* group);
	//save settings to ini file
	void SaveToIni(char* ini, char* group);

	// queries the recyclebin and update the data in the object. returns true if there are changes
	bool QueryFolder();
	// parses and updates the label
	void ParseLabel();

	void SetPath(char* path);

	void StartWorker();

	void FillDirInfo(char* path, DirInfo& info, bool doFileSize = false);
};

FolderDesklet::~FolderDesklet()
{
	IMalloc * imalloc = 0;
	if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
	{
		imalloc->Free ( itemId );
		imalloc->Release ( );
	}

	if(hWorker)
	{
		shouldStop = true;
		WaitForSingleObject(hWorker,INFINITE);
	}
}

void FolderDesklet::StartWorker()
{
	if(hWorker)
	{
		shouldStop = true;
		WaitForSingleObject(hWorker,INFINITE);
	}
	shouldStop = false;
	DWORD dummy(0);
	hWorker = CreateThread(0,0,WorkerThread,(LPVOID)this,0,&dummy);
}

void FolderDesklet::FillDirInfo(char* path, DirInfo& info, bool doFileSize)
{
	char tmp[MAX_PATH];
	strcpy(tmp,path);
	strcat(tmp,"\\*.*");

	WIN32_FIND_DATA data = {0};
	HANDLE h = FindFirstFile(tmp,&data);
	if(h)
	{
		do{
			if(strcmp(data.cFileName,".")!=0 && strcmp(data.cFileName,"..")!=0)
			{
				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					info.numFolders++;
					char npath[MAX_PATH];
					strcpy(npath,path);
					strcat(npath,"\\");
					strcat(npath,data.cFileName);
					FillDirInfo(npath,info,doFileSize);
				}
				else
				{
					info.numFiles++;
					char fpath[MAX_PATH];
					strcpy(fpath,path);
					strcat(fpath,"\\");
					strcat(fpath,data.cFileName);

					if(doFileSize)
					{
						HANDLE hFile = CreateFile(fpath,GENERIC_READ,FILE_SHARE_WRITE | FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
						if(hFile)
						{
							DWORD fhw = {0};
							DWORD f  = GetFileSize(hFile,&fhw);
							if(f != 0xFFFFFFFF)
							{
								info.byteSize += f + float(fhw)*(float(0xFFFFFFFF)+1);
							}
							CloseHandle(hFile);
						}
					}
				}
			}

			
		}while(FindNextFile(h,&data) && !shouldStop);

		FindClose(h);
	}
}

void FolderDesklet::SetPath(char* path)
{
	if(!path)return;

	if(stricmp(this->path,path)==0)return;

	strcpy(this->path,path);
	
	OLECHAR ocPath[MAX_PATH];
	MultiByteToWideChar(CP_OEMCP,0,this->path,-1,ocPath,sizeof(ocPath)/sizeof(ocPath[0]));

	if(autoCaption)
	{
		char caption[MAX_PATH] = {0};
		GetFileName(path,caption,sizeof(caption)/sizeof(caption[0]));
		DeskletSetLabel(hwndDesklet,caption);
	}

	if(itemId)
	{
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( itemId );
			imalloc->Release ( );
		}

	}

	if(notifyId)
		SHChangeNotifyDeregister(notifyId);

	if(GetItemIdListFromPath(ocPath,&itemId) )
	{
		SHChangeNotifyEntry shCNE;
		shCNE.pidl = itemId;
		shCNE.fRecursive = TRUE;

		notifyId = SHChangeNotifyRegister(hwndDesklet,0x0001 | 0x0002 | 0x1000, 
                SHCNE_ALLEVENTS, 
                WM_USER_FOLDERCHANGED,
				1,&shCNE);
	}
	//!old: QueryFolder();
	//!old: ParseLabel();
	StartWorker();
}

void FolderDesklet::ParseLabel()
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
			if(label[i]=='f')
			{
				lbl << numFiles;
			}
			else if(label[i] == 'd')
			{
				lbl << numFolders;
			}
			// s -> total filesize of the items in the bin
			//		automatically scale it
			else if(label[i]=='s')
			{
				float size = sizeInBytes;
				std::string postfix =" bytes";

				if(size >= 1024){size/=1024;postfix=" kB";};
				if(size >= 1024){size/=1024;postfix=" MB";};
				if(size >= 1024){size/=1024;postfix=" GB";};

				DWORD fsize = size * 100;

				//lbl.precision(1);
				lbl << float(fsize)/100 << postfix;

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

DWORD WINAPI WorkerThread(LPVOID lpData)
{
	FolderDesklet* fd = (FolderDesklet*)lpData;
	if(!fd)
	{
		ExitThread(TRUE);
		CloseHandle(fd->hWorker);
		fd->hWorker;
		return TRUE;
	}
	

	if(fd->QueryFolder())
		fd->ParseLabel();


	ExitThread(TRUE);
	CloseHandle(fd->hWorker);
	fd->hWorker = false;
	return TRUE;
}

bool FolderDesklet::QueryFolder()
{
	//return false;

	// post: true has been returned iff the recyclebin status has been changed
	//		 regarding the last query
	int oldNumFiles  = numFiles;
	int oldNumFolders= numFolders;
	float oldSizeInBytes = sizeInBytes;

	numFiles = 0;
	sizeInBytes = 0;

	DirInfo info = {0};

	if(strcmp(path,"")!=0 && (strstr(label,"d")!=0 || strstr(label,"f")!=0 || strstr(label,"s")!=0) )
		FillDirInfo(path,info,strstr(label,"%s")!=0);

	numFiles  = info.numFiles;
	numFolders = info.numFolders;
	sizeInBytes = info.byteSize;

	return oldNumFiles != numFiles || oldNumFolders != numFolders || oldSizeInBytes != sizeInBytes;
}

void FolderDesklet::ReadFromIni(char* ini, char* group)
{
	// pre:  ini and group are not 0
	// post: the settings has been retrieved from an ini file

	if(!ini || !group)return;

	GetPrivateProfileString(group,"LabelData",label,label,sizeof(label)/sizeof(label[0]),ini);
	//GetPrivateProfileString(group,"ImgEmpty",imgEmpty,imgEmpty,sizeof(imgEmpty)/sizeof(imgEmpty[0]),ini);
	GetPrivateProfileString(group,"ImgOverlay",imgOverlay,imgOverlay,sizeof(imgOverlay)/sizeof(imgOverlay[0]),ini);

	autoCaption = GetPrivateProfileInt(group,"AutoCaption",TRUE,ini) == TRUE;
	doDragDrop  = GetPrivateProfileInt(group,"DoDnD",TRUE,ini) == TRUE;

	char tmp[MAX_PATH] = {0};
	GetPrivateProfileString(group,"FolderPath",path,tmp,sizeof(tmp)/sizeof(tmp[0]),ini);
	SetPath(tmp);
}

void FolderDesklet::SaveToIni(char* ini, char* group)
{
	// pre:	 ini and group are not 0
	// post: the settings has been loaded from an ini file

	if(!ini || !group)return;

	WritePrivateProfileString(group,"LabelData",label,ini);
	//WritePrivateProfileString(group,"ImgEmpty",imgEmpty,ini);
	WritePrivateProfileString(group,"ImgOverlay",imgOverlay,ini);
	WritePrivateProfileString(group,"FolderPath",path,ini);
	WritePrivateProfileInt(group,"AutoCaption",autoCaption ? TRUE : FALSE,ini);
	WritePrivateProfileInt(group,"DoDnD",doDragDrop ? TRUE : FALSE,ini);
}

//	This function is called whenever AveDesk requests information about the desklet. This function
//	is not associated with a particular instance of this desklet and it therefore most of the times
//	looks exactly like the function below.
void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	// copy the name, author, notes and version number
	strcpy(szName,"Folder");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"A desklet that is optimized to be a shortcut to a folder.");
	*iVersion = 120;
}

//	The OnCreate function is called when a new instance of this desklet is created. If you use a class for
//	every instance - like in this FolderDesklet - you should create a new instance of that class and return
//	it. That pointer to the new instance of your class will be the first parameter of every other event function.
//	The hwndDesklet parameter identifies the desklet instance and is therefore needed in every callback function
//	like DeskletSetLabel.
//	The hInstance parameter is a handle to the instance of this desklets DLL which you need for resources etc...
//	The szIni and szIniGroup can be used to retrieve settings stored by the OnSave event. These parameters are 0
//	if the user adds this desklet for the first time.
FolderDesklet* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	// create a new instance of the FolderDesklet desklet class that holds
	// the settings for this desklet instance.
	FolderDesklet* lpData = new FolderDesklet(hwndDesklet,hInstance);

	// activate extended drag-'n-drop (OLE d&d) so we can accept
	// file and desklet drops
	DeskletActivateExtendedDragDrop(hwndDesklet);

	FORMATETC f;
	f.cfFormat = CF_HDROP;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	DeskletAddDragDropFormat(hwndDesklet,&f);


	if(szIni && szIniGroup)
	{
		// load the settings previously saved by a call to OnSave
		// here, the FolderDesklet class handles the loading of settings
		lpData->ReadFromIni(szIni,szIniGroup);
	}
	else
	{
		// here we could have set default settings for this desklet
		char path[MAX_PATH];

		DeskletGetRelativeFolder(hwndDesklet,path);
		strcat(path,"images\\folder.png");
		DeskletSetImageFile(hwndDesklet,path);
	}

	// update the desklet
	//!old: lpData->QueryFolder();
	lpData->StartWorker();
	lpData->ParseLabel();

	// set a timer that queries the recyclebin status every two seconds
	//SetTimer(hwndDesklet,QUERY_TIMER,2000,0);

	return lpData;
}

//	This function is called when a desklet instance is destroyed either by
//	the user manually removing it or by AveDesk closing.
//	You should release all memory, stop all threads etc... here. Also, do not
//	forget to release the memory that you allocated in the OnCreate.
void __stdcall OnDestroy(FolderDesklet *lpData, HWND hwndDesklet)
{
	// kill the timer that queries the recyclebin
	//KillTimer(lpData->hwndDesklet,QUERY_TIMER);

	// delete the instance of FolderDesklet created in the OnCreate function
	delete lpData;
	lpData = 0;
}


//	This function is called whenever the status of this desklet instance is saved to an INI-file.
//	szIni and szIniGroup can be used in calls to WritePrivateProfile*() if you want to save
//	settings.
//	bIsForExport is to be compatible with object dock docklets.
void __stdcall OnSave(FolderDesklet *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	// here, the FolderDesklet class handles the saving of settings
	lpData->SaveToIni(szIni,szIniGroup);
}

typedef struct {
    DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
    DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
} SHNOTIFYSTRUCT;

//	This function is called when a message has been sent to the desklets window. It's just like a regular WINPROC
//	(see WIN32 SDK for that), so you can use it to handle timer events etc...
void __stdcall OnProcessMessage(FolderDesklet* lpData,HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg==WM_SETPATH)
	{
		char* newPath = (char*)lParam;
		if(newPath)lpData->SetPath(newPath);
	}
	else if(msg==WM_USER_FOLDERCHANGED)
	{
		SHNOTIFYSTRUCT shns;
		char szBefore[MAX_PATH];
		char szAfter[MAX_PATH];
    
		memcpy((void *)&shns,(void *)wParam,sizeof(SHNOTIFYSTRUCT));

		SHGetPathFromIDList((struct _ITEMIDLIST *)shns.dwItem1, 
                        szBefore);
		SHGetPathFromIDList((struct _ITEMIDLIST *)shns.dwItem2,
                         szAfter);



		if (lParam == SHCNE_RENAMEFOLDER)
		{
			if(stricmp(szBefore,lpData->path)==0)
			{
				lpData->SetPath(szAfter);
			}
		}else if(lParam == SHCNE_RMDIR && stricmp(szBefore,lpData->path)==0)
		{
			DeskletSetSubLabel(lpData->hwndDesklet,"folder not found");
		}

		else if(lParam == SHCNE_CREATE || lParam == SHCNE_DELETE || lParam == SHCNE_MKDIR || lParam == SHCNE_RMDIR)
		{
			//!old: if(lpData->QueryFolder())
			//!old:		lpData->ParseLabel();
			lpData->StartWorker();
		}
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( (LPITEMIDLIST)shns.dwItem1 );
			imalloc->Free ( (LPITEMIDLIST)shns.dwItem2 );
			imalloc->Release ( );
		}
	}


	// here, we catch the OUR timer messages that tells us to query the recyclebin
	if(msg==WM_TIMER)
	{
		 if( wParam == FADER_TIMER)
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

				if(lpData->fadeCounter <= 0)
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
				if(lpData->fadeCounter >= 15)
				{
					KillTimer(lpData->hwndDesklet,FADER_TIMER);
					DeskletLayerSetAlpha(lpData->hwndDesklet,0,FALSE,0);
					DeskletLayerSetAlpha(lpData->hwndDesklet,id,true,255);	
				}
			}
		}
	}
}

//	This function is called when the user left clicks a desklet. Note that this can be either a single- or
//	double-click depending on the users settings.
//	p and s are obvious.
BOOL __stdcall OnLeftButtonClick(FolderDesklet *lpData, POINT* p, SIZE *s)
{
	// we open the recyclebin in explorer here.
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.hwnd = NULL;
	sei.lpVerb = NULL;
	sei.lpFile = lpData->path;
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
BOOL CALLBACK OnRightButtonClick(FolderDesklet *lpData, POINT* p, SIZE *s)
{
	// First, get the default menu. All default menu items will have an ID of at least 20,000
	HMENU menu = DeskletGetDefaultMenu(lpData->hwndDesklet,20000);

	// Add our own entries to the menu
	int cmd = 0;
	if(strcmp(lpData->path,"") != 0)
	{
		cmd = ShowContextMenu(lpData->hwndDesklet,menu,0,lpData->path);
	}
	else
	{
		//InsertMenu(menu,1,MF_BYPOSITION | MF_SEPARATOR,0,"-");
		
		// We show the menu at the position where the mouse was at the time the user right-clicked.
		// Also, we store the return value of TrackPopupMenu() to determine which action the user performed
		POINT pCur;
		GetCursorPos(&pCur);
		cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pCur.x, pCur.y, 0, lpData->hwndDesklet,0);
	}

	// We can now destroy the menu as it is no longer needed
	DestroyMenu(menu);

	if(cmd >= 20000)
	{
		// The user performed an action of the default menu (because the ID of the menu-item
		// is greater than 20,000 ), so we let AveDesk handle the action.
		DeskletDoDefaultMenuCommand(lpData->hwndDesklet,20000,cmd);
	}

	// return TRUE to indicate that we show our own menu and don't want the default menu to pop up
	return TRUE;
}

BOOL CALLBACK OnDragDropDataRequest(FolderDesklet* lpData,FORMATETC* format,STGMEDIUM* medium)
{
	if(format->cfFormat == CF_HDROP)
	{
		char m_szFiles[MAX_PATH];
		strcpy(m_szFiles,lpData->path);
		int m_nLen = strlen(m_szFiles)+1;
		HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, 
         sizeof(DROPFILES) + (m_nLen + 2) * sizeof(TCHAR)); 
		LPDROPFILES pDropFiles = (LPDROPFILES) ::GlobalLock(hglbCopy);
		pDropFiles->pFiles = sizeof(DROPFILES);
		pDropFiles->pt.x = pDropFiles->pt.y = 0;
		pDropFiles->fNC = TRUE;
		pDropFiles->fWide = FALSE; // ANSI charset
		LPTSTR lptstrCopy = (LPTSTR) pDropFiles;
		lptstrCopy += pDropFiles->pFiles;
		memcpy(lptstrCopy, m_szFiles, m_nLen * sizeof(TCHAR)); 
		lptstrCopy[m_nLen] =  '\0';    // null character 
		lptstrCopy[m_nLen+1] = '\0';    // null character 
	    ::GlobalUnlock(hglbCopy); 

		medium->hGlobal = hglbCopy;
	    medium->pUnkForRelease = NULL;
		medium->tymed = TYMED_HGLOBAL;

		return TRUE;
	}
	return FALSE;
}


//	This function is called when a user performs a drag over the desklet, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDragEnter(FolderDesklet* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	// for now, we accept all drops. We actually let the onDrop function handle this.
	// We return TRUE to indicate that we implemented this function.
	// If we want the drag to be not happening, we should set an internal flag, check for it in the OnDragOver()
	// and modify the effect flag to DROPEFFECT_NONE.

	if(IsDataObjectHDROP(obj))
	{
		if(lpData->isFileDrop = true)
		{
			if(lpData->doDragDrop)
			{
				lpData->fadingIn = false;
				SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
			}
		}
	}
	else
	{
		lpData->isFileDrop = false;
	}

	return TRUE;
}

//	This function is called when a user leaves a drag over the desklet, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDragLeave(FolderDesklet* lpData)
{
	if(lpData->isFileDrop)
	{
		if(lpData->doDragDrop)
		{
			lpData->fadingIn = true;
			SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
		}
	}
	return TRUE;
}

//	This function is called when a user performs a drag over the desklet, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDragOver(FolderDesklet* lpData,DWORD keys, POINTL pnt, DWORD* effect)
{
	// see OnDragEnter.
	return TRUE;
}

//	This function is called when a user drops something on this deskelt, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDrop(FolderDesklet* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{

	// the IsDataObjectHDROP helper function that is in the AD1.1 SDK, helps us to determine
	// if the user dropped a file
	if(IsDataObjectHDROP(obj))
	{
		// the user dropped a file, so we can try to delete it
	
		// the user wants really to remove the file
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
				pFromCur = pFromCur + DragQueryFile(ds->hDrop,i,0,0) +1;
				count++;
			}
	
			if(count > 0)
			{
				*pFromCur = 0;

				// let the shell do the removal
				SHFILEOPSTRUCT FileOp = {0};
				FileOp.hwnd = lpData->hwndDesklet;
				if(tolower(pFrom[0]) == tolower(lpData->path[0]))
					FileOp.wFunc = ( (keys & MK_SHIFT ) ? FO_COPY : FO_MOVE);
				else
					FileOp.wFunc = ( (keys & MK_SHIFT ) ? FO_MOVE : FO_COPY);
				FileOp.pFrom = pFrom;
				FileOp.pTo = lpData->path;
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

		if(lpData->isFileDrop)
		{
			if(lpData->doDragDrop)
			{
				lpData->fadingIn = true;
				SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
			}
		}

	}
	else
	{
		// there where no files dropped, so there was no effect from this drop
		*effect = DROPEFFECT_NONE;
	}

    return TRUE;
}

//	This is a DLGPROC for the Label propertysheetpage.
int __stdcall LabelFormatConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			// store the FolderDesklet-pointer of this desklet-instance in a WindowProperty, so
			// we can use it in other calls

			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			FolderDesklet* lpData = (FolderDesklet*)GetProp(hDlg,"lpData");

			if(!lpData)return TRUE;

			// set the edit-box for the label
			SetDlgItemText(hDlg,IDC_LABEL,lpData->label);

			CheckDlgButton(hDlg,IDC_AUTOCAPTION,lpData->autoCaption ? TRUE : FALSE);

		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_AUTOCAPTION:
					// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

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
					FolderDesklet* lpData = (FolderDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						// first get a pointer to the FolderDesklet object associated with this desklet instance.
						FolderDesklet* lpData = (FolderDesklet*)GetProp(hDlg,"lpData");

						// update the label-member and update the label.
						GetDlgItemText(hDlg,IDC_LABEL,lpData->label,sizeof(lpData->label)/sizeof(lpData->label[0]));
						//!old: lpData->QueryFolder();
						lpData->StartWorker();
						lpData->ParseLabel();

						lpData->autoCaption = IsDlgButtonChecked(hDlg,IDC_AUTOCAPTION) == BST_CHECKED;
						if(lpData->autoCaption)
						{
							char caption[MAX_PATH];
							GetFileName(lpData->path,caption,sizeof(caption)/sizeof(caption[0]));
							DeskletSetLabel(lpData->hwndDesklet,caption);
						}
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

// same except for images sheet
int __stdcall ImagesConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

//	static char imgEmpty[MAX_PATH];
	//static char imgFull[MAX_PATH];
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
			FolderDesklet* lpData = (FolderDesklet*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

//			SetProp(GetDlgItem(hDlg,IDC_EMPTYIMG),"img",(HANDLE)imgEmpty);
//			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_EMPTYIMG),GWL_WNDPROC,(LONG)FrameSubclassProc));

			SetProp(GetDlgItem(hDlg,IDC_EJECTIMG),"img",(HANDLE)imgOverlay);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_EJECTIMG),GWL_WNDPROC,(LONG)FrameSubclassProc));

		//	DragAcceptFiles(GetDlgItem(hDlg,IDC_EMPTYIMG),TRUE);
			DragAcceptFiles(GetDlgItem(hDlg,IDC_EJECTIMG),TRUE);

		//	strcpy(imgEmpty,lpData->imgEmpty);
			//strcpy(imgFull,lpData->imgFull);
			strcpy(imgOverlay,lpData->imgOverlay);

			CheckDlgButton(hDlg,IDC_DRAGDROP,lpData->doDragDrop ? TRUE : FALSE);

			init = false;
			InvalidateRect(hDlg,0,TRUE);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_DRAGDROP:
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
				/*case IDSETEMPTY:
					if(DeskletBrowseForImage(hDlg,imgEmpty))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
					
				break;*/

				/*case IDSETFULL:
					if(DeskletBrowseForImage(hDlg,imgFull))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				break;*/

				case IDSETEJECT:
					if(DeskletBrowseForImage(hDlg,imgOverlay))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
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
				{
					FolderDesklet* lpData = (FolderDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						strcpy(lpData->imgOverlay,imgOverlay);
						lpData->doDragDrop = IsDlgButtonChecked(hDlg,IDC_DRAGDROP) == BST_CHECKED;
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
		//	SetWindowLong(GetDlgItem(hDlg,IDC_EMPTYIMG),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_EMPTYIMG),"old_wndproc"));
			SetWindowLong(GetDlgItem(hDlg,IDC_EJECTIMG),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_EJECTIMG),"old_wndproc"));

			RemoveProp(hDlg,"lpData");
		break;
	}
	return FALSE;
}

// same except for images sheet
int __stdcall OtherConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;


	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			FolderDesklet* lpData = (FolderDesklet*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

			SetDlgItemText(hDlg,IDC_FOLDER,lpData->path);

			IAutoComplete *pac = {0};
			if ( SUCCEEDED( CoCreateInstance(CLSID_AutoComplete, 
                 NULL, 
                 CLSCTX_INPROC_SERVER,
                 IID_IAutoComplete, 
                 (LPVOID*)&pac) ))
			{

				IUnknown *punkSource = {0};
				if (SUCCEEDED( CoCreateInstance(CLSID_ACListISF, 
                 NULL, 
                 CLSCTX_INPROC_SERVER,
                 IID_IACList, 
                 (LPVOID*)&punkSource)) )
				{
					IACList2 *pal2  = {0};
					if (SUCCEEDED(punkSource->QueryInterface(IID_IACList2, (LPVOID*)&pal2)))
					{
						//pal2->SetOptions(ACLO_FILESSYSONLY);
						pal2->Release();
					}

					pac->Init(GetDlgItem(hDlg,IDC_FOLDER), punkSource, NULL, NULL);

					IAutoComplete2 *pac2 = {0};
					if (SUCCEEDED(pac->QueryInterface(IID_IAutoComplete2, (LPVOID*)&pac2)))
					{
						pac2->SetOptions(ACO_AUTOSUGGEST);
						pac2->Release();
					}
					punkSource->Release();
					
				}
				pac->Release();
			}

		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_FOLDER:
					// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDBROWSE:
					BROWSEINFO bi = { 0 };
					bi.lpszTitle = "Pick a Directory";
					LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
					if ( pidl != 0 )
					{
						// get the name of the folder
						char path[MAX_PATH];
						if ( SHGetPathFromIDList ( pidl, path ) )
						{
							SetDlgItemText(hDlg,IDC_FOLDER,path);
						}
					
						// free memory used
						IMalloc * imalloc = 0;
						if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
						{
								imalloc->Free ( pidl );
								imalloc->Release ( );
						}
					}
	
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
				{
					FolderDesklet* lpData = (FolderDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						char path[MAX_PATH];
						GetDlgItemText(hDlg,IDC_FOLDER,path,sizeof(path)/sizeof(path[0]));
						lpData->SetPath(path);
					}
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


//	This function is called before the configuration dialogue for this desklet is shown, allowing us
//	to add our own propertsheets to the dialogue (see WIN32 SDK for more on this).
//	We should return the number of propertysheets that we want to see present.
//	Here, we add 3 propertysheets.
INT __stdcall OnAddPropertySheets(FolderDesklet* lpData, int numPresent)
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
	psp.pszTitle = "Folder";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGOTHER);
	psp.pfnDlgProc = OtherConfigProc;
	psp.lParam = (LPARAM)lpData;

	res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	return res;
}

