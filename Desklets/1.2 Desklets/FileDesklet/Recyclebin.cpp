
#define _WIN32_WINNT 0x500
#define WINVER 0x500

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
#include <shobjidl.h>
#include <Ntquery.h>

#define WM_SETPATH WM_USER + 202

#define PSGUID_STORAGE \
    { 0xb725f130, 0x47ef, 0x101a, \
      { 0xa5, 0xf1, 0x02, 0x60, 0x8c, 0x9e, 0xeb, 0xac } }

#define PSGUID_SUMMARY \
    { 0xF29F85E0, 0x4FF9, 0x1068, \
		{ 0xAB, 0x91,0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9 } }

//--------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------
void GetFileExtension(const char* filepath, char* buffer, int buffersize)
{
	// pre:  filepath and buffer are not 0, buffersize is the maximum size of chars copied
	// post: the filename in filepath is stored in buffer, this means that from filepath
	//			c:\blabla\yada.png buffer is filled with yada.png

	if(!filepath || !buffer)return;

	int i=strlen(filepath)-1;
	while( i >= 0 && buffersize-- >0 && filepath[i]!='.')i--;
	if(i!=0)
		strcpy(buffer,filepath+i+1);
	else
		strcpy(buffer,"");
}
//--------------------------------------------------------------------------------------------
char* lowercase(char* str)
{
	// pre:  str is not 0
	// post: str has been converted to an all-lowercase-characters string and str
	//			has been returned again

	if(!str)return 0;

	for(unsigned int i=0;i<strlen(str);i++)
		str[i] = tolower(str[i]);

	return str;
}
//--------------------------------------------------------------------------------------------
bool FileExists(const char* file)
{
	// pre:  file is not 0 and file should be an absolute path
	// post: true has been returned if the file named by file does exist, otherwise false
	//			has been returned

	if(!file)return false;

	// get the filename of the file and store it in tmp
	char tmp[MAX_PATH];
	GetFileName(file,tmp,sizeof(tmp));

	// get WIN32_FIND_DATA for the file pointed to file
	WIN32_FIND_DATA d;
	ZeroMemory(&d,sizeof(d));
	HANDLE h=FindFirstFile(file,&d);

	// don't forget to close the handle, otherwise we have a mem-leak
	FindClose(h);

	// if the filename retrieved from file and the filename retrieved
	// by getting the WIN32_FIND_DATA are the same when compared (not case-sensitive)
	// then this file DOES exist, otherwise it DOESN'T exist
	return strcmp(lowercase(tmp),lowercase(d.cFileName))==0;
}


#include <string>					// for std::string
#include <sstream>					// for std::stringstream

#include "resource.h"				// trivial

#define WM_USER_FOLDERCHANGED WM_USER+88

#define FADER_TIMER 8860312

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
class FileDesklet
{
public:

	FileDesklet(HWND hwndDesklet, HINSTANCE hInstanceDll) :
		hwndDesklet(hwndDesklet),
		hInstanceDll(hInstanceDll),
		isFileDrop(false),
		fadeCounter(0),
		fadingIn(false),
		itemId(0),
		notifyId(0),
		autoCaption(true),
		autoLabel(true),
		execute(0)
		{
			// post: an FileDesklet object has been created with the default settings
			strcpy(path,"");
			strcpy(parameters,"");
			strcpy(startin,"");

			strcpy(label,"");

			DeskletGetRelativeFolder(hwndDesklet,imgOverlay);
			strcat(imgOverlay,"images\\dnd.png");
		}

		~FileDesklet();

	// all the fields are public, because I was too lazy to encaps. them the right way ;-)
	char path[MAX_PATH];				// the path to which this folder points
	char parameters[MAX_PATH*3];
	char startin[MAX_PATH];
	int execute;

	LPITEMIDLIST itemId;
	ULONG	notifyId;

	HWND hwndDesklet;					// the HWND that identifies this desklet		
	HINSTANCE hInstanceDll;				// the instance of this DLL

	char label[2000];					// the 'raw'-label as defined by the user

	char imgOverlay[MAX_PATH];			// relative path for the image shown when we hoover over an CD/DVD drive

	bool isFileDrop;							// a flag stating if the current drop includes a CD
	int  fadeCounter;					// a counter used for fading out layers
	bool fadingIn;						// a flag stating if we are fading in

	bool autoCaption;
	bool autoLabel;
	

	// read settings from ini file
	void ReadFromIni(char* ini, char* group);
	//save settings to ini file
	void SaveToIni(char* ini, char* group);

	// parses and updates the label
	void ParseLabel();

	void SetPath(char* path);
};

FileDesklet::~FileDesklet()
{
	IMalloc * imalloc = 0;
	if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
	{
		imalloc->Free ( itemId );
		imalloc->Release ( );
	}
}

void FileDesklet::SetPath(char* path)
{
	if(!path)return;

	if(stricmp(this->path,path)==0)return;

	strcpy(this->path,path);
	
	OLECHAR ocPath[MAX_PATH];
	MultiByteToWideChar(CP_OEMCP,0,this->path,-1,ocPath,sizeof(ocPath)/sizeof(ocPath[0]));

	if(autoCaption)
	{
		char caption[MAX_PATH];
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
	if(autoLabel)
	{
		char ext[MAX_PATH];
		GetFileExtension(path,ext,MAX_PATH);
		char file[MAX_PATH];
		GetDLLPath(hInstanceDll,file,MAX_PATH);
		strcat(file,"Presets\\");
		strcat(file,ext);
		strcat(file,".lbl");
		if(!FileExists(file))
		{
			GetDLLPath(hInstanceDll,file,MAX_PATH);
			strcat(file,"Presets\\default.lbl");
		}
		
		char lbl[2000];
		GetPrivateProfileString("Preset","Label","",lbl,sizeof(lbl)/sizeof(lbl[0]),file);
		strcpy(label,lbl);
	}

	char drive[MAX_PATH]	= {0};
	char folder[MAX_PATH]	= {0};
	char file[MAX_PATH]		= {0};
	char ext[MAX_PATH]		= {0};
	_splitpath(path,drive,folder,file,ext);
	lowercase(ext);

	bool isImageFile =	strcmp(ext,".png") == 0 ||
						strcmp(ext,".gif") == 0 ||
						strcmp(ext,".bmp") == 0 ||
						strcmp(ext,".jpg") == 0 ||
						strcmp(ext,".jpeg")== 0 ||
						strcmp(ext,".ico") == 0 ;

	if(isImageFile)
	{
		BOOL success(FALSE);
		UINT lId = DeskletLayerGetLayer(hwndDesklet,"Shortcut.ImageOverlay",&success);
		if(!success)
			lId = DeskletLayerAdd(hwndDesklet,"Shortcut.ImageOverlay");

		Bitmap* bmp = DeskletLoadGDIPlusImage(path);; 
		float w = bmp->GetWidth();
		float h = bmp->GetHeight();
		while(w > 90 || h > 90)
		{
			w /= 1.5;
			h /= 1.5;
		}
		Bitmap* overlay = new Bitmap((int)w,(int)h);
		Graphics g(overlay);
		g.DrawImage(bmp,0,0,(int)w,(int)h);
		delete bmp;
		RECT r = {128 - w, 128 - h,128,128};
		DeskletLayerSetPlacement(hwndDesklet,lId,FALSE,TRUE,&r,128,128);
		DeskletLayerSetAlpha(hwndDesklet,lId,FALSE,200);
		DeskletLayerSetImage(hwndDesklet,lId,TRUE,overlay,TRUE);
	}
	else
	{
		BOOL success(FALSE);
		UINT lId = DeskletLayerGetLayer(hwndDesklet,"Shortcut.ImageOverlay",&success);
		if(success)
			DeskletLayerRemove(hwndDesklet,lId,TRUE);
	}

	ParseLabel();
}

void FileDesklet::ParseLabel()
{
	// post: the desklets label has been updated
	//		 according to how the user did set up the label


	std::stringstream lbl;
	int i=0;
	int l=strlen(label);

	// we want to get a ShellFolder2 for this shortcut, together with a 
	// appropiate relative pidl.
	IShellFolder2 *folder2 = 0;
	LPITEMIDLIST Pidl = 0;

	if(strcmp(path,"")!=0)
	{

		char file[MAX_PATH];
		strcpy(file,path);
		if(file[strlen(file)-1] == '\\')
		{
			file[strlen(file)-1] = '\0';
		}
		
		char filepath[MAX_PATH];

		strcpy(filepath,file);
		for(int i=strlen(file)-1;i>0;--i)
		{
			if(file[i]=='\\' || file[i]=='/')
			{
				filepath[i] = '\0';
				strcpy(file,file+strlen(filepath)+1);
				break;
			}
		}

		if(strlen(filepath) < 3)
		{
			strcat(filepath,"\\");
		}

		if(strstr(file,":")!=0)
		{
			strcpy(file,"");
		}

		LPSHELLFOLDER DesktopFolder = 0;
		SHGetDesktopFolder(&DesktopFolder);
		if(DesktopFolder)
		{
			wchar_t Path[MAX_PATH];
			DWORD dummy;
			MultiByteToWideChar(CP_ACP,0,filepath,-1,Path,MAX_PATH);
			LPITEMIDLIST ParentPidl;
			DesktopFolder->ParseDisplayName(hwndDesklet, 0, Path, &dummy, &ParentPidl, 0);
			LPSHELLFOLDER ParentFolder;
			DesktopFolder->BindToObject(ParentPidl, 0, IID_IShellFolder, (void**)&ParentFolder);
			if(!ParentFolder)
			{
				DesktopFolder->Release();
			}
			else
			{
				MultiByteToWideChar(CP_ACP,0,file,-1,Path,MAX_PATH);
				if ( SUCCEEDED(ParentFolder->ParseDisplayName(hwndDesklet, 0, Path, &dummy, &Pidl, 0) ) )
				{
	
					if( SUCCEEDED(ParentFolder->QueryInterface(IID_IShellFolder2,(void**)&folder2) ) )
					{
						//folder2->Release();
					}
				}
				else
				{
					lbl << "file not found";
				}
				

				ParentFolder->Release();

			}
		}

	}

	// the maximum length for the label is 2000 characters
	while(i<l && i<2000 && Pidl!=0)
	{
		// the next character indicates that the user wants to have
		// some data inserted here
		if(label[i]=='%')
		{
			i++;

		
			// s -> total filesize of the items in the bin
			//		automatically scale it
			if(label[i]=='s')
			{
				float size(0);
				HANDLE hFile = CreateFile(path,GENERIC_READ,FILE_SHARE_WRITE | FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
				if(hFile)
				{
					LARGE_INTEGER s = {0};
					GetFileSizeEx(hFile,&s);
					size = (float) (signed __int64) s.QuadPart;
					CloseHandle(hFile);
				}
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
			// %number. Refers to the shelldetails id.
			else if(label[i] >= '0' && label[i] <= '9')
			{
				// get the number first
				char c = label[i];
				char c2 = 0;


				if(i<l-1 && label[i+1] >= '0' && label[i+1] <= '9')
					c2 = label[++i];

				int num(0);

				if(c2!=0)
					num = int(c - '0') * 10 + int(c2 - '0');
				else
					num = int(c - '0');

				// if the ShellFolder2 was loaded, we can actually parse this one,
				// otherwise it is ignored.
				if(folder2)
				{
					// let the ShellFolder2 fill the SHELLDETAILS.
					SHELLDETAILS d  = {0};
					if(SUCCEEDED(folder2->GetDetailsOf(Pidl,num,&d)))
					{
						// success! now stream the info into the lbl-stringstream.
						if(d.str.uType == STRRET_WSTR)
						{
							char text[1024];
							WideCharToMultiByte(CP_OEMCP,0,d.str.pOleStr,-1,text,1024,0,0);
							lbl << text;
							// if it's a WSTR, we got ownership of the string, so we need to free it.
							CoTaskMemFree(d.str.pOleStr);
						}
						else if(d.str.uType == STRRET_CSTR)
						{
							lbl << d.str.cStr;
						}
					}
				}
			}
		}
		// this character just needs to be copied to the output label
		else
		{
			lbl << label[i];
		}
		i++;
	}

	if(folder2)
	{
			folder2->Release();
			folder2 = 0;
	}

	// finally, set the label
	DeskletSetSubLabel(hwndDesklet,const_cast<char*>(lbl.str().c_str()));
}

void FileDesklet::ReadFromIni(char* ini, char* group)
{
	// pre:  ini and group are not 0
	// post: the settings has been retrieved from an ini file

	if(!ini || !group)return;

	GetPrivateProfileString(group,"LabelData",label,label,sizeof(label)/sizeof(label[0]),ini);
	GetPrivateProfileString(group,"ImgOverlay",imgOverlay,imgOverlay,sizeof(imgOverlay)/sizeof(imgOverlay[0]),ini);

	GetPrivateProfileString(group,"Params",parameters,parameters,sizeof(parameters)/sizeof(parameters[0]),ini);
	GetPrivateProfileString(group,"StartIn",startin,startin,sizeof(startin)/sizeof(startin[0]),ini);

	autoCaption = GetPrivateProfileInt(group,"AutoCaption",TRUE,ini) == TRUE;
	autoLabel = GetPrivateProfileInt(group,"AutoCaption",FALSE,ini) == TRUE;

	execute = GetPrivateProfileInt(group,"Execute",execute,ini);

	char tmp[MAX_PATH] = {0};
	GetPrivateProfileString(group,"FilePath",path,tmp,sizeof(tmp)/sizeof(tmp[0]),ini);
	SetPath(tmp);
}

void FileDesklet::SaveToIni(char* ini, char* group)
{
	// pre:	 ini and group are not 0
	// post: the settings has been loaded from an ini file

	if(!ini || !group)return;

	WritePrivateProfileString(group,"LabelData",label,ini);
	WritePrivateProfileString(group,"ImgOverlay",imgOverlay,ini);
	WritePrivateProfileString(group,"FilePath",path,ini);
	WritePrivateProfileInt(group,"AutoCaption",autoCaption ? TRUE : FALSE,ini);
	WritePrivateProfileInt(group,"AutoLabel",autoLabel ? TRUE : FALSE,ini);


	WritePrivateProfileString(group,"Params",parameters,ini);
	WritePrivateProfileString(group,"StartIn",startin,ini);
	WritePrivateProfileInt(group,"Execute",execute,ini);
}

//	This function is called whenever AveDesk requests information about the desklet. This function
//	is not associated with a particular instance of this desklet and it therefore most of the times
//	looks exactly like the function below.
void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	// copy the name, author, notes and version number
	strcpy(szName,"Shortcut");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"A desklet that is optimized to be a shortcut to a file or application.");
	*iVersion = 110;
}

//	The OnCreate function is called when a new instance of this desklet is created. If you use a class for
//	every instance - like in this FileDesklet - you should create a new instance of that class and return
//	it. That pointer to the new instance of your class will be the first parameter of every other event function.
//	The hwndDesklet parameter identifies the desklet instance and is therefore needed in every callback function
//	like DeskletSetLabel.
//	The hInstance parameter is a handle to the instance of this desklets DLL which you need for resources etc...
//	The szIni and szIniGroup can be used to retrieve settings stored by the OnSave event. These parameters are 0
//	if the user adds this desklet for the first time.
FileDesklet* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstance, char *szIni, char *szIniGroup)
{
	// create a new instance of the FileDesklet desklet class that holds
	// the settings for this desklet instance.
	FileDesklet* lpData = new FileDesklet(hwndDesklet,hInstance);

	// activate extended drag-'n-drop (OLE d&d) so we can accept
	// file and desklet drops
	DeskletActivateExtendedDragDrop(hwndDesklet);

	// register the dragformat this desklet supports.
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
		// here, the FileDesklet class handles the loading of settings
		lpData->ReadFromIni(szIni,szIniGroup);
	}
	else
	{
		char path[MAX_PATH];
		DeskletGetRelativeFolder(hwndDesklet,path);
		strcat(path,"images\\file.png");
		DeskletSetImageFile(lpData->hwndDesklet,path);
	}

	lpData->ParseLabel();

	return lpData;
}

//	This function is called when a desklet instance is destroyed either by
//	the user manually removing it or by AveDesk closing.
//	You should release all memory, stop all threads etc... here. Also, do not
//	forget to release the memory that you allocated in the OnCreate.
void __stdcall OnDestroy(FileDesklet *lpData, HWND hwndDesklet)
{
	// delete the instance of FileDesklet created in the OnCreate function
	delete lpData;
	lpData = 0;
}


//	This function is called whenever the status of this desklet instance is saved to an INI-file.
//	szIni and szIniGroup can be used in calls to WritePrivateProfile*() if you want to save
//	settings.
//	bIsForExport is to be compatible with object dock docklets.
void __stdcall OnSave(FileDesklet *lpData, char *szIni, char *szIniGroup, BOOL bIsForExport)
{
	// here, the FileDesklet class handles the saving of settings
	lpData->SaveToIni(szIni,szIniGroup);
}

typedef struct {
    DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
    DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
} SHNOTIFYSTRUCT;

//	This function is called when a message has been sent to the desklets window. It's just like a regular WINPROC
//	(see WIN32 SDK for that), so you can use it to handle timer events etc...
void __stdcall OnProcessMessage(FileDesklet* lpData,HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg==WM_SETPATH)
	{
		// a path has been set, update the desklet.
		char* newPath = (char*)lParam;
		if(newPath)lpData->SetPath(newPath);

	}
	else if(msg==WM_USER_FOLDERCHANGED)
	{
		// There was a change in the folder where the file is.
		// we need to handle this based on the type of operation (which is stored in the lParam).
		// First, we get retrieve the paths from the pdisl in the SHNOTIFYSTRUCT.
		SHNOTIFYSTRUCT shns;
		char szBefore[MAX_PATH];
		char szAfter[MAX_PATH];
    
		memcpy((void *)&shns,(void *)wParam,sizeof(SHNOTIFYSTRUCT));

		SHGetPathFromIDList((struct _ITEMIDLIST *)shns.dwItem1, 
                        szBefore);
		SHGetPathFromIDList((struct _ITEMIDLIST *)shns.dwItem2,
                         szAfter);



		if (lParam == SHCNE_RENAMEITEM)
		{
			// the item was renamed, so update the desklets path.
			if(stricmp(szBefore,lpData->path)==0)
			{
				lpData->SetPath(szAfter);
			}
		}
		
		// do not forget to free the memory which we got ownership of.
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( (LPITEMIDLIST)shns.dwItem1 );
			imalloc->Free ( (LPITEMIDLIST)shns.dwItem2 );
			imalloc->Release ( );
		}
		// we also just parse the label, in case the file has been changed.
		lpData->ParseLabel();
	}


	// here, we catch the OUR timer messages that tells us to query the recyclebin
	else if(msg==WM_TIMER)
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
			DeskletLayerSetAlpha(lpData->hwndDesklet,id,true,lpData->fadeCounter*13);

			// and the background layer, which always has an id of 0
			// we now set the redraw parameter to true, to redraw the desklet with its new
			// settings for the layers
			//DeskletLayerSetAlpha(lpData->hwndDesklet,0,true,255 - lpData->fadeCounter*13);

			if(lpData->fadingIn)
			{
				// we are fading in, so counter must be decreased
				--lpData->fadeCounter;

				if(lpData->fadeCounter == 0)
				{
					// counter reached 0, stop the timer and reset the layers

					KillTimer(lpData->hwndDesklet,FADER_TIMER);

					// we can remove the overlay-layer, cause we no longer need it
					DeskletLayerRemove(lpData->hwndDesklet,id,true);
				//	DeskletLayerSetAlpha(lpData->hwndDesklet,0,true,255);
				}
			}
			else
			{
				// etc...

				++lpData->fadeCounter;
				if(lpData->fadeCounter == 15)
				{
					KillTimer(lpData->hwndDesklet,FADER_TIMER);
				//	DeskletLayerSetAlpha(lpData->hwndDesklet,0,FALSE,0);
					DeskletLayerSetAlpha(lpData->hwndDesklet,id,true,255);	
				}
			}
		}
	}
}

//	This function is called when the user left clicks a desklet. Note that this can be either a single- or
//	double-click depending on the users settings.
//	p and s are obvious.
BOOL __stdcall OnLeftButtonClick(FileDesklet *lpData, POINT* p, SIZE *s)
{
	// if the user holds the ALT-key, open the folder where this file is placed in.
	if(GetKeyState(VK_MENU) & 0x8000)
	{
		char dir[MAX_PATH];
		strcpy(dir,lpData->path);
		for(int i=strlen(dir)-1;i>0;--i)
		{
			if(dir[i]=='\\' || dir[i]=='/')
			{
				dir[i] = '\0';
				break;
			}
		}

		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei, sizeof(sei));
		sei.cbSize = sizeof(sei);
		sei.hwnd = NULL;
		sei.lpVerb = NULL;
		sei.lpFile = dir;
		sei.lpParameters = "" ;
		sei.lpDirectory = "";
		sei.nShow = SW_SHOWNORMAL;
		if(lpData->execute == 1)
			sei.nShow = SW_SHOWMINIMIZED;
		else if(lpData->execute == 2)
			sei.nShow = SW_SHOWMAXIMIZED;
		ShellExecuteEx(&sei);

	}
	else
	{
		SHELLEXECUTEINFO sei;
		ZeroMemory(&sei, sizeof(sei));
		sei.cbSize = sizeof(sei);
		sei.hwnd = NULL;
		sei.lpVerb = NULL;
		sei.lpFile = lpData->path;
		sei.lpParameters = lpData->parameters;
		if(strcmp(lpData->startin,"")!=0)
			sei.lpDirectory = lpData->startin;
		else
			sei.lpDirectory = "";
		sei.nShow = SW_SHOWNORMAL;
		if(lpData->execute == 1)
			sei.nShow = SW_SHOWMINIMIZED;
		else if(lpData->execute == 2)
		sei.nShow = SW_SHOWMAXIMIZED;
		ShellExecuteEx(&sei);
	}
	// return TRUE to indicate that we implement this function.
	return TRUE;
}

//	This function is called when the user right-clicks a desklet. If we don't implement this or return FALSE
//	AveDesk will show a default menu.
//	If you want to show your own rightclick menu, please use DeskletGetDefaultMenu() to also include the
//	default menu to conform with the design guidelines.
BOOL CALLBACK OnRightButtonClick(FileDesklet *lpData, POINT* p, SIZE *s)
{
	if(strcmp(lpData->path,"")==0 || !FileExists(lpData->path))return FALSE;

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

BOOL CALLBACK OnDragDropDataRequest(FileDesklet* lpData,FORMATETC* format,STGMEDIUM* medium)
{
	// if a CF_HDROP is requested, we handle the request by setting up 
	// a HDROP structure in global memory and assigning it the the SRGMEDIUM.
	if(format->cfFormat == CF_HDROP)
	{
		char m_szFiles[MAX_PATH];
		strcpy(m_szFiles,lpData->path);
		int m_nLen = strlen(m_szFiles)+1;

		HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, 
         sizeof(DROPFILES) + (m_nLen + 2) * sizeof(TCHAR)); 
		LPDROPFILES pDropFiles = (LPDROPFILES) ::GlobalLock(hglbCopy);
		pDropFiles->pFiles = sizeof(DROPFILES);

		GetCursorPos(&pDropFiles->pt);
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
BOOL CALLBACK OnDragEnter(FileDesklet* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	// for now, we accept all drops. We actually let the onDrop function handle this.
	// We return TRUE to indicate that we implemented this function.
	// If we want the drag to be not happening, we should set an internal flag, check for it in the OnDragOver()
	// and modify the effect flag to DROPEFFECT_NONE.

	if(IsDataObjectHDROP(obj))
	{
		if(lpData->isFileDrop = true)
		{
			lpData->fadingIn = false;
			// animation disabled.
			//SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
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
BOOL CALLBACK OnDragLeave(FileDesklet* lpData)
{
	if(lpData->isFileDrop)
	{
		lpData->fadingIn = true;
		// animation disabled.
		//SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
	}
	return TRUE;
}

//	This function is called when a user performs a drag over the desklet, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDragOver(FileDesklet* lpData,DWORD keys, POINTL pnt, DWORD* effect)
{
	// see OnDragEnter.
	return TRUE;
}

//	This function is called when a user drops something on this deskelt, but only if you activated Extended
//	drag-'n-drop by using the ActivateExtendedDragDrop() callback function.
//	For more information, see the WIN32 SDK on OLE drag-'n-drop.
BOOL CALLBACK OnDrop(FileDesklet* lpData,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
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
				char* p = new char[strlen(pFrom)+1+MAX_PATH*3+1];

				SHELLEXECUTEINFO sei;
				ZeroMemory(&sei, sizeof(sei));
				sei.cbSize = sizeof(sei);
				sei.hwnd = NULL;
				sei.lpVerb = NULL;
				sei.lpFile = lpData->path;
				strcpy(p,lpData->parameters);
				strcat(p," ");
				strcat(p,pFrom);

				sei.lpParameters = p;
				if(strcmp(lpData->startin,"")!=0)
					sei.lpDirectory = lpData->startin;
				else
					sei.lpDirectory = "";
				sei.nShow = SW_SHOWNORMAL;
				if(lpData->execute == 1)
					sei.nShow = SW_SHOWMINIMIZED;
				else if(lpData->execute == 2)
					sei.nShow = SW_SHOWMAXIMIZED;
				ShellExecuteEx(&sei);
				delete[] p;
			}
	
			// release the memory we used for the string
			delete[] pFrom;

			// and finally, unlock the DropStruct, so the memory will be released
			UnlockHDROP(ds);

		}

		if(lpData->isFileDrop)
		{
			lpData->fadingIn = true;
			//SetTimer(lpData->hwndDesklet,FADER_TIMER,20,0);
		}

	}
	else
	{
		// there where no files dropped, so there was no effect from this drop
		*effect = DROPEFFECT_NONE;
	}

    return TRUE;
}

void FillLabelBox(FileDesklet* d, HWND hwndList)
{
	// pre:  d is not 0
	// post: the listbox with HWND hwndList has been filled with the strings
	//		 that can be parsed for the sublabel.

	if(!d)return;

	// we only need to query the desktopfolder, since we only want the names of the shelldetails
	// not the actuall data.
	LPSHELLFOLDER DesktopFolder = 0;
	SHGetDesktopFolder(&DesktopFolder);
	if(DesktopFolder)
	{

		IShellFolder2* folder2 = 0;
		if( SUCCEEDED(DesktopFolder->QueryInterface(IID_IShellFolder2,(void**)&folder2) ) )
		{
			SHELLDETAILS d  = {0};
			int i(0);
			while(SUCCEEDED(folder2->GetDetailsOf(0,i++,&d)))
			{
				std::stringstream s;
				s << "%" << i-1 << "\t";

				if(d.str.uType == STRRET_WSTR)
				{
					char text[1024];
					WideCharToMultiByte(CP_OEMCP,0,d.str.pOleStr,-1,text,1024,0,0);
					s << text;
					SendMessage(hwndList,LB_ADDSTRING,0,(LPARAM)s.str().c_str());
					CoTaskMemFree(d.str.pOleStr);
				}
				else if(d.str.uType == STRRET_CSTR)
				{
					s << d.str.cStr;
					SendMessage(hwndList,LB_ADDSTRING,0,(LPARAM)s.str().c_str());
				}
			}

			folder2->Release();
		}

		DesktopFolder->Release();
	}

}

//	This is a DLGPROC for the Label propertysheetpage.
int __stdcall LabelFormatConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			// store the FileDesklet-pointer of this desklet-instance in a WindowProperty, so
			// we can use it in other calls

			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");

			if(!lpData)return TRUE;

			// set the edit-box for the label
			SetDlgItemText(hDlg,IDC_LABEL,lpData->label);

			CheckDlgButton(hDlg,IDC_AUTOCAPTION,lpData->autoCaption ? TRUE : FALSE);
			CheckDlgButton(hDlg,IDC_AUTOLABEL,lpData->autoLabel ? TRUE : FALSE);

			FillLabelBox(lpData,GetDlgItem(hDlg,IDC_FORMATS));

		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDSAVEPRESET:
					{
						FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
						if(!lpData)
							return FALSE;

						char file[MAX_PATH];
						GetFileExtension(lpData->path,file,MAX_PATH);
						if(strcmp(file,"")==0)
							strcpy(file,"default.lbl");
						else
							strcat(file,".lbl");

						OPENFILENAME ofn = {0};
						ofn.hInstance = lpData->hInstanceDll;
						ofn.hwndOwner = GetParent(hDlg);
						ofn.lpstrFilter = "Label Presets(*.lbl)\0*.lbl\0\0";
						char initDir[MAX_PATH];
						GetDLLPath(lpData->hInstanceDll,initDir,MAX_PATH);
						strcat(initDir,"Presets\\");
						ofn.lpstrInitialDir = initDir;
						ofn.lStructSize = sizeof(ofn);
						ofn.nMaxFile = MAX_PATH;
						//ofn.nFileExtension = ".lbl";
						ofn.lpstrFile = file;
						ofn.lpstrTitle = "Save Label Preset";

						if(GetSaveFileName(&ofn))
						{
							char ext[MAX_PATH];
							GetFileExtension(file,ext,MAX_PATH);
							if(stricmp(ext,"lbl")!=0)
								strcat(file,".lbl");

							char lbl[2000];
							GetDlgItemText(hDlg,IDC_LABEL,lbl,sizeof(lbl)/sizeof(lbl[0]));
							WritePrivateProfileString("Preset","Label",lbl,file);
						}
					}
				break;

				case IDLOADPRESET:
					{
						FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
						if(!lpData)
							return FALSE;

						char file[MAX_PATH];
						GetFileExtension(lpData->path,file,MAX_PATH);
						if(strcmp(file,"")==0)
							strcpy(file,"default.lbl");
						else
							strcat(file,".lbl");

						OPENFILENAME ofn = {0};
						ofn.hInstance = lpData->hInstanceDll;
						ofn.hwndOwner = GetParent(hDlg);
						ofn.lpstrFilter = "Label Presets(*.lbl)\0*.lbl\0\0";
						char initDir[MAX_PATH];
						GetDLLPath(lpData->hInstanceDll,initDir,MAX_PATH);
						strcat(initDir,"Presets\\");
						ofn.lpstrInitialDir = initDir;
						ofn.lStructSize = sizeof(ofn);
						ofn.nMaxFile = MAX_PATH;
						//ofn.nFileExtension = ".lbl";
						ofn.lpstrFile = file;
						ofn.lpstrTitle = "Load Label Preset";

						if(GetOpenFileName(&ofn))
						{
							char lbl[2000];
							GetPrivateProfileString("Preset","Label","",lbl,sizeof(lbl)/sizeof(lbl[0]),file);
							SetDlgItemText(hDlg,IDC_LABEL,lbl);
						}
					}
				break;

				case IDSAVEDEFAULTPRESET:
					{
						FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
						if(!lpData)
							return FALSE;

						char file[MAX_PATH];
						GetDLLPath(lpData->hInstanceDll,file,MAX_PATH);
						strcat(file,"Presets\\default.lbl");

						char lbl[2000];
						GetDlgItemText(hDlg,IDC_LABEL,lbl,sizeof(lbl)/sizeof(lbl[0]));
						WritePrivateProfileString("Preset","Label",lbl,file);
					}
				break;

				case IDC_AUTOCAPTION: case IDC_AUTOLABEL:
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
					FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						// first get a pointer to the FileDesklet object associated with this desklet instance.
						FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");

						lpData->autoCaption = IsDlgButtonChecked(hDlg,IDC_AUTOCAPTION) == BST_CHECKED;

						bool oldAutoLabel = lpData->autoLabel;
						lpData->autoLabel = IsDlgButtonChecked(hDlg,IDC_AUTOLABEL) == BST_CHECKED;
												
						if(lpData->autoCaption)
						{
							char caption[MAX_PATH];
							GetFileName(lpData->path,caption,sizeof(caption)/sizeof(caption[0]));
							DeskletSetLabel(lpData->hwndDesklet,caption);
						}

						if(!oldAutoLabel && lpData->autoLabel)
						{
							char ext[MAX_PATH];
							GetFileExtension(lpData->path,ext,MAX_PATH);
							char file[MAX_PATH];
							GetDLLPath(lpData->hInstanceDll,file,MAX_PATH);
							strcat(file,"Presets\\");
							strcat(file,ext);
							strcat(file,".lbl");
							if(!FileExists(file))
							{
								GetDLLPath(lpData->hInstanceDll,file,MAX_PATH);
								strcat(file,"Presets\\default.lbl");
							}
							
							char lbl[2000];
							GetPrivateProfileString("Preset","Label","",lbl,sizeof(lbl)/sizeof(lbl[0]),file);
							SetDlgItemText(hDlg,IDC_LABEL,lbl);
						}

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
			FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

			SetProp(GetDlgItem(hDlg,IDC_EJECTIMG),"img",(HANDLE)imgOverlay);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_EJECTIMG),GWL_WNDPROC,(LONG)FrameSubclassProc));
			DragAcceptFiles(GetDlgItem(hDlg,IDC_EJECTIMG),TRUE);

			strcpy(imgOverlay,lpData->imgOverlay);

			init = false;
			InvalidateRect(hDlg,0,TRUE);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
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
					FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						strcpy(lpData->imgOverlay,imgOverlay);
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
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
			FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
		
			if(!lpData)return TRUE;

			SetDlgItemText(hDlg,IDC_FOLDER,lpData->path);

			SendDlgItemMessage(hDlg,IDC_WINDOW,CB_ADDSTRING,0,(LPARAM)"Normal Window");
			SendDlgItemMessage(hDlg,IDC_WINDOW,CB_ADDSTRING,0,(LPARAM)"Minimized Window");
			SendDlgItemMessage(hDlg,IDC_WINDOW,CB_ADDSTRING,0,(LPARAM)"Maximized Window");
			SendDlgItemMessage(hDlg,IDC_WINDOW,CB_SETCURSEL,lpData->execute,0);

			SetDlgItemText(hDlg,IDC_PARAMETERS,lpData->parameters);
			SetDlgItemText(hDlg,IDC_STARTIN,lpData->startin);

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
						pal2->SetOptions(ACLO_FILESYSONLY);
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
				case IDC_WINDOW:
				// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==CBN_SELCHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
				case IDC_FOLDER: case IDC_PARAMETERS: case IDC_STARTIN:
					// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDBROWSE:
				{
					FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
		
					if(!lpData)return TRUE;

					char file[MAX_PATH];
					GetDlgItemText(hDlg,IDC_FOLDER,file,sizeof(file)/sizeof(file[0]));

					OPENFILENAME ofn = {0};
					ofn.hInstance = lpData->hInstanceDll;
					ofn.hwndOwner = hDlg;
					ofn.lpstrFile = file;
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFilter = "All Files(*.*)\0*.*\0\0";
					ofn.lpstrTitle = "Select File";
					ofn.nMaxFile = MAX_PATH;


					if (GetOpenFileName(&ofn))
					{
						SetDlgItemText(hDlg,IDC_FOLDER,file);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				}
				break;

				case IDBROWSE2:
					BROWSEINFO bi = { 0 };
					bi.lpszTitle = "Pick a Directory";
					LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
					if ( pidl != 0 )
					{
						// get the name of the folder
						char path[MAX_PATH];
						if ( SHGetPathFromIDList ( pidl, path ) )
						{
							SetDlgItemText(hDlg,IDC_STARTIN,path);
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
					FileDesklet* lpData = (FileDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						char path[MAX_PATH];
						GetDlgItemText(hDlg,IDC_FOLDER,path,sizeof(path)/sizeof(path[0]));
						lpData->SetPath(path);

						GetDlgItemText(hDlg,IDC_PARAMETERS,lpData->parameters,sizeof(lpData->parameters)/sizeof(lpData->parameters[0]));
						GetDlgItemText(hDlg,IDC_STARTIN,lpData->startin,sizeof(lpData->startin)/sizeof(lpData->startin[0]));

						lpData->execute = SendDlgItemMessage(hDlg,IDC_WINDOW,CB_GETCURSEL,0,0);
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
INT __stdcall OnAddPropertySheets(FileDesklet* lpData, int numPresent)
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

	/*psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Images";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGIMAGES);
	psp.pfnDlgProc = ImagesConfigProc;
	psp.lParam = (LPARAM)lpData;
	res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));*/

	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = lpData->hInstanceDll;
	psp.pszTitle = "Shortcut";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CONFIGOTHER);
	psp.pfnDlgProc = OtherConfigProc;
	psp.lParam = (LPARAM)lpData;

	res = DeskletAddPropertySheet(lpData->hwndDesklet,CreatePropertySheetPage(&psp));

	return res;
}

