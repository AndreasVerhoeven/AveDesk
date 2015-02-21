#define _WIN32_WINNT 0x501
#define WINVER 0x501

#include <windows.h>
#include <Gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include "../DeskletSDK.h"
#include "../DeskletSDK.cpp"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

//#pragma comment(lib,"imglibpp~.lib")
#include "imglibpp.h"
#include "../imglibpp/imglibpp.cpp"

#include <vector>
#include <string>
#include <sstream>

#include <winbase.h>
#include <math.h>

#include "resource.h"


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

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
bool IsDirectory(const char* filename)
{
	// pre:  filename is not 0
	// post: true has been returned iff filename is a directory
	
	if(!filename)return false;

	WIN32_FIND_DATA data = {0};
	HANDLE h = FindFirstFile(filename,&data);
	if(h && h!=INVALID_HANDLE_VALUE)
	{
		FindClose(h);
		return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0;
	}
	else
	{
		return false;
	}

}

#define ICN16	1
#define ICN24	2
#define ICN32	4
#define ICN48	8
#define ICN64	16
#define ICN72	32
#define ICN96	64
#define ICN128	128

Rect GetPrivateProfileRect(const char* section, const char* ini)
{
	return Rect(	GetPrivateProfileInt(section,"Left",0,ini),
					GetPrivateProfileInt(section,"Top",0,ini),
					GetPrivateProfileInt(section,"Width",0,ini),
					GetPrivateProfileInt(section,"Height",0,ini)
				);
}

class Skin
{
private:
	char name[1024];
	char author[1024];
	char info[1024];



	int width, height;
	char normal[MAX_PATH];
	char canAccept[MAX_PATH];
	char refuse[MAX_PATH];

	int busyNumOfFrames;
	int busyInterval;
	std::vector<std::string> busyFrames;
	char busySubLabel[1024];

	Rect iconOverlayRect;
	char iconOverlayBgFileName[MAX_PATH];

	char dirImageFileName[MAX_PATH];

public:
	Skin(const char* ini)
	{
		GetPrivateProfileString("Info","Name","Unknown",name,1024,ini);
		GetPrivateProfileString("Info","Author","Unknown",author,1024,ini);
		GetPrivateProfileString("Info","Info","",info,1024,ini);

		width = GetPrivateProfileInt("General","Width",0,ini);
		height= GetPrivateProfileInt("General","Height",0,ini);

		memset(normal,0,MAX_PATH);
		GetPrivateProfileString("Backgrounds","FileNameNormal",normal,normal,MAX_PATH,ini);
		memset(canAccept,0,MAX_PATH);
		GetPrivateProfileString("Backgrounds","FileNameCanDrop",canAccept,canAccept,MAX_PATH,ini);
		memset(refuse,0,MAX_PATH);
		GetPrivateProfileString("Backgrounds","FileNameRefuseDrop",refuse,refuse,MAX_PATH,ini);


		busyNumOfFrames = GetPrivateProfileInt("BusyAnim","NumberOfFrames",0,ini);
		for(int i=0;i<busyNumOfFrames;++i)
		{
			std::stringstream ss;
			ss << "FileName";
			ss << i+1;
			char tmp[MAX_PATH] = {0};
			GetPrivateProfileString("BusyAnim",ss.str().c_str(),tmp,tmp,MAX_PATH,ini);
			busyFrames.push_back(tmp);
		}
		busyInterval = GetPrivateProfileInt("BusyAnim","IntervalBetweenFramesInMilliSeconds",100,ini);

		memset(busySubLabel,0,1024);
		GetPrivateProfileString("BusyAnim","SubLabel",busySubLabel,busySubLabel,1024,ini);

		iconOverlayRect = GetPrivateProfileRect("IconOverlay",ini);

		memset(iconOverlayBgFileName,0,MAX_PATH);
		GetPrivateProfileString("IconOverlay","BgFileName",iconOverlayBgFileName,iconOverlayBgFileName,MAX_PATH,ini);

		memset(dirImageFileName,0,MAX_PATH);
		GetPrivateProfileString("Other","DirImageFileName",dirImageFileName,dirImageFileName,MAX_PATH,ini);
	}

	const char* GetName()const{return name;};
	const char* GetAuthor()const{return author;};
	const char* GetInfo()const{return info;};

	int GetWidth()const{return width;};
	int GetHeight()const{return height;};

	const char* GetNormalFileName()const{return normal;};
	const char* GetCanAcceptFileName()const{return canAccept;};
	const char* GetRefuseFileName()const{return refuse;};

	int GetBusyNumberOfFrames()const{return busyNumOfFrames;};
	int GetBusyInterval()const{return busyInterval;};
	const char* GetBusyFileName(int i)const{return busyFrames[i].c_str();};
	const char* GetBusySubLabel()const{return busySubLabel;};

	Rect GetIconOverlayRect()const{return iconOverlayRect;};

	const char* GetIconOverlayBgFileName()const{return iconOverlayBgFileName;};

	const char* GetDirImageFileName()const{return dirImageFileName;};
};

class IconDrop
{
private:
	HWND hwnd;
	HINSTANCE dll;
	bool canAccept;

	UINT formats;

	Skin* skin;

	char skinDir[MAX_PATH];

	bool isConverting;

public:
	IconDrop(HWND hwnd, HINSTANCE dll) : hwnd(hwnd), dll(dll), canAccept(false),
		formats(ICN32 | ICN48 | ICN128), skin(0),h(0), isConverting(false)
	{
		memset(skinDir,0,MAX_PATH);
		strcpy(skinDir,"Default");
	}

	HANDLE h;

	~IconDrop()
	{
		if(h)
		{
			SetIsConverting(false);
			SignalObjectAndWait(h,h,INFINITE,TRUE);
		}

		delete skin;
		skin = 0;
	}

	std::vector<std::string> inputFiles;
	std::vector<std::string> outputFiles;

	HWND GetWindow()const{return hwnd;};
	HINSTANCE GetDll()const{return dll;};

	void SetSkin(const char* dir)
	{
		strcpy(skinDir,dir);
		char path[MAX_PATH] = {0};
		GetDLLPath(GetDll(),path,MAX_PATH);
		PathAppend(path,"skins\\");
		PathAppend(path,skinDir);
		PathAppend(path,"\\");

		char ini[MAX_PATH] = {0};
		strcpy(ini,path);
		PathAppend(ini,"\\skin.ini");

		CRITICAL_SECTION cs = {0};
		InitializeCriticalSection(&cs);
		EnterCriticalSection(&cs);
		delete skin;
		skin = 0;
		skin = new Skin(ini);
		LeaveCriticalSection(&cs);

		Rect rc = GetSkin()->GetIconOverlayRect();

		RECT r = {0};
		r.left = rc.X;
		r.right = r.left + rc.Width;
		r.top = rc.Y;
		r.bottom = r.top + rc.Height;
		DeskletLayerSetPlacement(GetWindow(),GetOverlayLayerID(),FALSE,TRUE,&r,GetSkin()->GetWidth(),GetSkin()->GetHeight() );
		if(outputFiles.size() > 0)
			CreateIconOverlay();

		if(!GetIsConverting() )
		{
			std::string file = GetAbsoluteSkinFolder();
			file += skin->GetNormalFileName();
			DeskletSetImageFile(GetWindow(),file.c_str());
		}
	}

	std::string GetAbsoluteSkinFolder()
	{
		char bgPath[MAX_PATH];
		DeskletGetRelativeFolder(GetWindow(),bgPath);
		PathAppend(bgPath,"\\skins\\");
		PathAppend(bgPath,skinDir);		
		strcat(bgPath,"\\");

		return std::string(bgPath);
	}


	void ClearDirectory(const char* dir)
	{
		char search[MAX_PATH+4] = {0};
		strcpy(search,dir);
		strcat(search,"\\*.*");
		WIN32_FIND_DATA data ={0};
		HANDLE h = FindFirstFile(search,&data);
		if(h && h!=INVALID_HANDLE_VALUE)
		{
			do
			{
				if(strcmp(data.cFileName,".")!=0 && strcmp(data.cFileName,"..")!=0)
				{
					if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						char dir2[MAX_PATH] = {0};
						strcpy(dir2,dir);
						strcat(dir2,"\\");
						ClearDirectory(dir2);
					}
					else
					{
						char f[MAX_PATH+MAX_PATH]  ={0};
						strcpy(f,dir);
						strcat(f,"\\");
						strcat(f,data.cFileName);
						DeleteFile(f);
					}
				}
			}while(FindNextFile(h,&data));
			FindClose(h);
		}
		RemoveDirectory(dir);
	}

	void ClearFiles()
	{
		//inputFiles.clear();
		//outputFiles.clear();
		for(std::vector<std::string>::iterator iter = outputFiles.begin();iter != outputFiles.end(); ++iter)
		{
			std::string&  s = *iter;
			if( IsDirectory(s.c_str()) )
			{
				ClearDirectory(s.c_str() );
			}
			else
			{
				DeleteFile(s.c_str() );
			}
		}

	}



	void MakeIconSize(PICONFILE iconFile,int entryNum, Bitmap* src, int size)
	{
		Bitmap bmp(size,size);
		Graphics g(&bmp);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        //g.SetSmoothingMode(SmoothingModeAntialias);
		g.DrawImage(src,0,0,size,size);

		HBITMAP hColor = {0};
		HBITMAP hMask = {0};

		bmp.GetHBITMAP(0,&hColor);

        hMask = CreateBitmap(size,size,1,1,0);
        HDC dc = CreateCompatibleDC(0);
        HBITMAP oldHBMP = (HBITMAP) SelectObject(dc,hMask);
		for(int x=0;x<size;++x)
		{
			for(int y=0;y<size;++y)
			{
				Color c;
				bmp.GetPixel(x,y,&c);
				if(c.GetAlpha() == 0)
					SetPixel(dc,x,y,RGB(255,255,255));
				else
					SetPixel(dc,x,y,RGB(0,0,0));
			}
		}

        SelectObject(dc,oldHBMP);
        DeleteDC(dc);

		CREATEICONENTRY entry = {0};
		entry.entryNum = entryNum;
		entry.hColor = hColor;
		entry.hMask = hMask;
		entry.lColors = 0xFFFFFFFF;

		WriteIconEntry(iconFile,&entry);

		DeleteObject((HGDIOBJ)hColor);
		DeleteObject((HGDIOBJ)hMask);

	}

	void AddDirectory(const char* dir)
	{
		char path[MAX_PATH] = {0};
		GetDLLPath(dll,path,MAX_PATH);
		strcat(path,"temp\\");

		char dirName[MAX_PATH] = {0};
		GetFileName(dir,dirName,MAX_PATH);
		strcat(path,dirName);

		CreateDirectory(path,0);

		inputFiles.push_back(dir);
		outputFiles.push_back(path);


		char search[MAX_PATH+4] = {0};
		strcpy(search,dir);
		strcat(search,"\\*.*");
		WIN32_FIND_DATA data ={0};
		HANDLE h = FindFirstFile(search,&data);
		if(h && h!=INVALID_HANDLE_VALUE)
		{
			do
			{
				if(strcmp(data.cFileName,".")!=0 && strcmp(data.cFileName,"..")!=0)
				{
					if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						char dir[MAX_PATH] = {0};
						strcpy(dir,path);
						strcat(dir,"\\");
						strcat(dir,data.cFileName);
						AddDirectory(dir);
					}
					else
					{
						char ext[MAX_PATH] = {0};
						GetFileExtension(data.cFileName,ext,MAX_PATH);
						lowercase(ext);
						if(strcmp(ext,"ico") ==0 ||
							strcmp(ext,"png")==0 ||
							strcmp(ext,"bmp")==0 ||
							strcmp(ext,"tif")==0 ||
							strcmp(ext,"jpg")==0 ||
							strcmp(ext,"jpeg")==0||
							strcmp(ext,"tiff")==0
						)
						{
							char file[MAX_PATH] = {0};
							strcpy(file,dir);
							strcat(file,"\\");
							strcat(file,data.cFileName);
							AddFile(file,dirName,false);
						}
					}
				}
			}while(FindNextFile(h,&data));
			FindClose(h);
		}

	}

	void AddFile(const char* file, const char* dir = NULL, bool AddToLists=true)
	{
		char ext[MAX_PATH] = {0};
		GetFileExtension(file,ext,MAX_PATH);
		lowercase(ext);
		if(strcmp(ext,"ico")==0 )
		{
			char path[MAX_PATH] = {0};
			GetDLLPath(dll,path,MAX_PATH);
			strcat(path,"temp\\");
			char filename[MAX_PATH] = {0};
			GetFileName(file,filename,MAX_PATH);
			if(dir)
			{
				strcat(path,dir);
				strcat(path,"\\");
			}
			int i = strlen(filename)-1;
			while(i>0)
			{
				if(filename[i] == '.')
				{
					filename[i]=0;
					break;
				}
				i--;
			}
			strcat(path,filename);

			strcat(path,".png");

			Bitmap* bmp = DeskletLoadGDIPlusImage(const_cast<char*>(file));

			CLSID pngClsid = {0};
			GetEncoderClsid(L"image/png",&pngClsid);
			WCHAR wPath[MAX_PATH] = {0};
			MultiByteToWideChar(CP_OEMCP,0,path,-1,wPath,MAX_PATH);
			bmp->Save(wPath,&pngClsid,NULL);

			delete bmp;

			if(AddToLists)
			{
				inputFiles.push_back(file);
				outputFiles.push_back(path);
			}
		}
		else
		{
			char path[MAX_PATH] = {0};
			GetDLLPath(dll,path,MAX_PATH);
			strcat(path,"temp\\");
			char filename[MAX_PATH] = {0};
			GetFileName(file,filename,MAX_PATH);
			if(dir)
			{
				strcat(path,dir);
				strcat(path,"\\");
			}
			int i = strlen(filename)-1;
			while(i>0)
			{
				if(filename[i] == '.')
				{
					filename[i]=0;
					break;
				}
				i--;
			}
			strcat(path,filename);
			strcat(path,".ico");

			Bitmap* bmp = DeskletLoadGDIPlusImage(const_cast<char*>(file));

			ICONFILE iconFile = {0};
			strcpy(iconFile.filename,path);



			iconFile.numberOfEntries = 0;
			if(formats & ICN16)iconFile.numberOfEntries++;
			if(formats & ICN24)iconFile.numberOfEntries++;
			if(formats & ICN32)iconFile.numberOfEntries++;
			if(formats & ICN48)iconFile.numberOfEntries++;
			if(formats & ICN64)iconFile.numberOfEntries++;
			if(formats & ICN72)iconFile.numberOfEntries++;
			if(formats & ICN96)iconFile.numberOfEntries++;
			if(formats & ICN128)iconFile.numberOfEntries++;

			if( OpenIconFile(&iconFile) )
			{
				UINT count(0);
				if(formats & ICN16)MakeIconSize(&iconFile,count++,bmp,16);
				if(formats & ICN24)MakeIconSize(&iconFile,count++,bmp,24);
				if(formats & ICN32)MakeIconSize(&iconFile,count++,bmp,32);
				if(formats & ICN48)MakeIconSize(&iconFile,count++,bmp,48);
				if(formats & ICN64)MakeIconSize(&iconFile,count++,bmp,64);
				if(formats & ICN72)MakeIconSize(&iconFile,count++,bmp,72);
				if(formats & ICN96)MakeIconSize(&iconFile,count++,bmp,96);
				if(formats & ICN128)MakeIconSize(&iconFile,count++,bmp,128);

				CloseIconFile(&iconFile);
			}

			delete bmp;

			if(AddToLists)
			{
				inputFiles.push_back(file);
				outputFiles.push_back(path);
			}

		}
	}

	void CreateIconOverlay()
	{
		std::string dirFile = GetAbsoluteSkinFolder();
		dirFile += GetSkin()->GetDirImageFileName();
		Bitmap* dir = DeskletLoadGDIPlusImage(dirFile.c_str() );

		Rect r = GetSkin()->GetIconOverlayRect();
		Bitmap* bmp = new Bitmap(r.Width,r.Height);
		Graphics g(bmp);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		std::string bgFile = GetAbsoluteSkinFolder();
		bgFile += GetSkin()->GetIconOverlayBgFileName();
		Bitmap* bg = DeskletLoadGDIPlusImage(bgFile.c_str() );
		g.DrawImage(bg,0,0,r.Width,r.Height);
		delete bg;
		bg = 0;

		float nc = sqrt(outputFiles.size() );
		int ratio = (int)nc;
		if(nc - int(nc)  > 0.1)
			ratio++;

		// we wont have a div by 0 zero  exception
		if(ratio == 0)
			ratio++;

		float w = float(r.Width) / float(ratio);
		float h = float(r.Height) / float(ratio);

		int i(0);
		for(std::vector<std::string>::iterator iter = outputFiles.begin();iter != outputFiles.end(); ++iter)
		{
			Bitmap* print = NULL;

			std::string&  s = *iter;
			if( IsDirectory(s.c_str()) )
				print = dir;
			else
				print = DeskletLoadGDIPlusImage(s.c_str() );

			g.DrawImage(print,i%ratio * w,i/ratio * h,w,h);

			if(print != dir)
				delete print;

			i++;
		}

		delete dir;
		
		DeskletLayerSetImage(GetWindow(),GetOverlayLayerID(),FALSE,bmp,TRUE);

	}

	UINT GetOverlayLayerID()
	{
		BOOL success(FALSE);
		UINT layerId = DeskletLayerGetLayer(GetWindow(),"IconDrop.Overlay",&success);
		if(!success)
			layerId = DeskletLayerAdd(GetWindow(),"IconDrop.Overlay");

		return layerId; 
	}

	void ReadFromIni(const char* ini, const char* group)
	{
		formats = GetPrivateProfileInt(group,"icon_formats",formats,ini);
		GetPrivateProfileString(group,"SkinDir",skinDir,skinDir,MAX_PATH,ini);
	}

	void SaveToIni(const char* ini, const char* group)
	{
		WritePrivateProfileInt(group,"icon_formats",formats,ini);
		WritePrivateProfileString(group,"SkinDir",skinDir,ini);
	}

	bool GetCanAccept()const{return canAccept;};
	void SetCanAccept(bool val){canAccept = val;};

	UINT GetFormats()const{return formats;};
	void SetFormats(UINT i){formats=i;};

	Skin* GetSkin()const{return skin;};
	const char* GetSkinDir()const{return skinDir;};

	bool GetIsConverting()const{return isConverting;};
	void SetIsConverting(bool v){isConverting=v;};
};


void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"IconDrop");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Allows you to convert ICONS to PNGs and PNGs to ICONS quickly.");
	*iVersion = 110;
}

IconDrop* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	IconDrop* id = new IconDrop(hwndDesklet,hInstanceDll);

	DeskletActivateExtendedDragDrop(id->GetWindow() );

	FORMATETC f;
	f.cfFormat = CF_HDROP;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	DeskletAddDragDropFormat(id->GetWindow(),&f);

	if(ini && group)
		id->ReadFromIni(ini,group);

	id->SetSkin(id->GetSkinDir());
	DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,FALSE);

	return id;
}

void __stdcall OnDestroy(IconDrop* id, HWND hwndDesklet)
{
	id->ClearFiles();
	delete id;
}

void __stdcall OnSave(IconDrop* id, const char* ini, const char* group, BOOL bIsForExport)
{
	id->SaveToIni(ini,group);
}

BOOL __stdcall OnDragEnter(IconDrop* id,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(IsDataObjectHDROP(obj))
	{
		id->SetCanAccept(true);
		DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,FALSE);
		std::string file = id->GetAbsoluteSkinFolder();
		file += id->GetSkin()->GetCanAcceptFileName();
		DeskletSetImageFile(id->GetWindow(),file.c_str());
		*effect = DROPEFFECT_NONE;
	}
	else
	{
		id->SetCanAccept(false);
		DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,FALSE);
		std::string file = id->GetAbsoluteSkinFolder();
		file += id->GetSkin()->GetRefuseFileName();
		DeskletSetImageFile(id->GetWindow(),file.c_str());
		*effect = DROPEFFECT_COPY;
	}
	return TRUE;
}

BOOL __stdcall OnDragLeave(IconDrop* id)
{
	id->SetCanAccept(false);
	if(id->outputFiles.size() > 0)
		DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,TRUE);
	std::string file = id->GetAbsoluteSkinFolder();
	file += id->GetSkin()->GetNormalFileName();
	DeskletSetImageFile(id->GetWindow(),file.c_str());
	return TRUE;
}

BOOL __stdcall OnDragOver(IconDrop* id,DWORD keys, POINTL pnt, DWORD* effect)
{
	if(!id->GetCanAccept())
	{
		*effect = DROPEFFECT_NONE;
	}
	else
	{
		*effect = DROPEFFECT_COPY;
	}
	return TRUE;
}

DWORD CALLBACK AnimThread(LPVOID lpData)
{
	IconDrop* id = (IconDrop*)lpData;
	if(!lpData)
	{
		ExitThread(0);
		return TRUE;
	}

	int cur(0);
	while(id->GetIsConverting() )
	{
		std::string file = id->GetAbsoluteSkinFolder();
		file += id->GetSkin()->GetBusyFileName(cur);
		DeskletSetImageFile(id->GetWindow(),file.c_str() );
		SleepEx(id->GetSkin()->GetBusyInterval(),TRUE);
		cur++;
		if(cur>=id->GetSkin()->GetBusyNumberOfFrames())
			cur = 0;
	}

	CloseHandle(id->h);
	id->h = 0;
	ExitThread(0);
	return TRUE;
}

BOOL __stdcall OnDrop(IconDrop* id,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(IsDataObjectHDROP(obj) )
	{
		DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,FALSE);

		id->SetIsConverting(true);

		if(id->GetSkin()->GetBusySubLabel()[0] != 0)
			DeskletSetSubLabel(id->GetWindow(),id->GetSkin()->GetBusySubLabel());

		if(id->GetSkin()->GetBusyNumberOfFrames() > 0 && !id->h)
		{
			DWORD dummy(0);
			id->h = CreateThread(0,0,AnimThread,(LPVOID)id,0,&dummy);
		}

		id->ClearFiles();
		id->inputFiles.clear();
		id->outputFiles.clear();

		DropStruct* ds = LockHDROP(obj);
	
		int num = DragQueryFile(ds->hDrop, -1, 0, 0);

		char buf[MAX_PATH+1] = {0};

		for(int i=0;i<num;++i)
		{
			DragQueryFile(ds->hDrop,i,buf,MAX_PATH);
			if(IsDirectory(buf))
				id->AddDirectory(buf);
			else
				id->AddFile(buf);
		}

		UnlockHDROP(ds);
		*effect = DROPEFFECT_COPY;

		id->SetIsConverting(false);
		if(id->h)
		{
			SignalObjectAndWait(id->h,id->h,INFINITE,TRUE);
			CloseHandle(id->h);
			id->h = 0;
		}
		if(id->outputFiles.size() > 0)
		{
			id->CreateIconOverlay();
			DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,TRUE);
		}

		std::string f = id->GetAbsoluteSkinFolder();
		f += id->GetSkin()->GetNormalFileName();
		DeskletSetImageFile(id->GetWindow(),f.c_str());

		DeskletSetSubLabel(id->GetWindow(),"");
	}
	else
	{
		*effect = DROPEFFECT_NONE;
	}
	return TRUE;
}

BOOL __stdcall OnDragDropDataRequest(IconDrop* id,FORMATETC* format,STGMEDIUM* medium)
{
	// if a CF_HDROP is requested, we handle the request by setting up 
	// a HDROP structure in global memory and assigning it the the SRGMEDIUM.
	if(format->cfFormat == CF_HDROP)
	{
		char* m_szFiles = new char[(MAX_PATH+1) * id->outputFiles.size() ];
		m_szFiles[0] = 0;

		int m_nLen = 0;

		char* p = m_szFiles;
		for(int i =0; i<id->outputFiles.size(); ++i)
		{
			std::string& s = id->outputFiles[i];
			strcpy(p,s.c_str());
			p += s.size()+1;
			m_nLen += s.size() + 1;
		}

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
		
		delete[] m_szFiles;

		return TRUE;
	}
	return FALSE;
}


int __stdcall FormatsConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	IconDrop* id = (IconDrop*)GetProp(hDlg,"lpData");

	static UINT oldFormats;

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			id = (IconDrop*)GetProp(hDlg,"lpData");
			if(!id)return TRUE;

			{
			UINT formats = id->GetFormats();
			if(formats & ICN16)CheckDlgButton(hDlg,IDC_16,TRUE);
			if(formats & ICN24)CheckDlgButton(hDlg,IDC_24,TRUE);
			if(formats & ICN32)CheckDlgButton(hDlg,IDC_32,TRUE);
			if(formats & ICN48)CheckDlgButton(hDlg,IDC_48,TRUE);
			if(formats & ICN64)CheckDlgButton(hDlg,IDC_64,TRUE);
			if(formats & ICN72)CheckDlgButton(hDlg,IDC_72,TRUE);
			if(formats & ICN96)CheckDlgButton(hDlg,IDC_96,TRUE);
			if(formats & ICN128)CheckDlgButton(hDlg,IDC_128,TRUE);
			oldFormats = formats;
			}

			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{

				case IDC_16:case IDC_24:case IDC_32:case IDC_48:
					case IDC_64:case IDC_72:case IDC_96:case IDC_128:
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
					if(id)
					{
						UINT formats(0);
						if(IsDlgButtonChecked(hDlg,IDC_16))formats |= ICN16;
						if(IsDlgButtonChecked(hDlg,IDC_24))formats |= ICN24;
						if(IsDlgButtonChecked(hDlg,IDC_32))formats |= ICN32;
						if(IsDlgButtonChecked(hDlg,IDC_48))formats |= ICN48;
						if(IsDlgButtonChecked(hDlg,IDC_64))formats |= ICN64;
						if(IsDlgButtonChecked(hDlg,IDC_72))formats |= ICN72;
						if(IsDlgButtonChecked(hDlg,IDC_96))formats |= ICN96;
						if(IsDlgButtonChecked(hDlg,IDC_128))formats |= ICN128;

						id->SetFormats(formats);
						if(id->inputFiles.size() > 0 && formats != oldFormats)
						{
							HCURSOR hCursorIssLoading = LoadCursor(0,IDC_WAIT);

							SetCursor(hCursorIssLoading);

							if(MessageBox(hDlg,"You have changed the ICON output format.\nDo you want to reconvert the current dropped files?","IconDrop - Changed Format",MB_ICONQUESTION | MB_YESNO)==IDYES)
							{
								DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,FALSE);

								id->SetIsConverting(true);

								if(id->GetSkin()->GetBusySubLabel()[0] != 0)
									DeskletSetSubLabel(id->GetWindow(),id->GetSkin()->GetBusySubLabel());

								if(id->GetSkin()->GetBusyNumberOfFrames() > 0 && !id->h)
								{
									DWORD dummy(0);
									id->h = CreateThread(0,0,AnimThread,(LPVOID)id,0,&dummy);
								}

								id->ClearFiles();
								id->outputFiles.clear();

								std::vector<std::string> oldInputFiles = id->inputFiles;
								id->inputFiles.clear();
								for(std::vector<std::string>::iterator iter = oldInputFiles.begin(); iter != oldInputFiles.end(); ++iter)
								{
									std::string& s = *iter;
									if( IsDirectory(s.c_str()) )
										id->AddDirectory(s.c_str());
									else
										id->AddFile(s.c_str() );

								}

								id->SetIsConverting(false);
								if(id->h)
								{
									SignalObjectAndWait(id->h,id->h,INFINITE,TRUE);
									CloseHandle(id->h);
									id->h = 0;
								}

								if(id->outputFiles.size() > 0)
								{
									id->CreateIconOverlay();
									DeskletLayerSetVisibility(id->GetWindow(),id->GetOverlayLayerID(),FALSE,TRUE);
								}

								std::string f = id->GetAbsoluteSkinFolder();
								f += id->GetSkin()->GetNormalFileName();
								DeskletSetImageFile(id->GetWindow(),f.c_str());

								DeskletSetSubLabel(id->GetWindow(),"");
								
								SetCursor(NULL);
							}
							oldFormats = formats;
						}
					}
				break;
			}
		break;
	}
	return FALSE;
}

int __stdcall SkinConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	IconDrop* id = (IconDrop*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			id = (IconDrop*)GetProp(hDlg,"lpData");
			if(!id)return TRUE;
			
			{
				char path[MAX_PATH] = {0};
				GetDLLPath(id->GetDll(),path,MAX_PATH);
				PathAppend(path,"skins\\");
				std::string s = path;
				s += "*.*";
				WIN32_FIND_DATA data = {0};
				HANDLE h = FindFirstFile(s.c_str(),&data);
				if(h != INVALID_HANDLE_VALUE)
				{
					do
					{
						if(strcmp(data.cFileName,".")!=0 && strcmp(data.cFileName,"..")!=0 && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
							SendDlgItemMessage(hDlg,IDC_SKIN,CB_ADDSTRING,0,(LPARAM)data.cFileName);

					}while(FindNextFile(h,&data));
					FindClose(h);
				}
				SendDlgItemMessage(hDlg,IDC_SKIN,CB_SELECTSTRING,0,(LPARAM)id->GetSkinDir());

				SetDlgItemText(hDlg,IDC_NAME,id->GetSkin()->GetName());
				SetDlgItemText(hDlg,IDC_AUTHOR,id->GetSkin()->GetAuthor());
				SetDlgItemText(hDlg,IDC_INFO,id->GetSkin()->GetInfo());
			}

			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_SKIN:
					if(HIWORD(wParam)==CBN_SELCHANGE)
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
					if(id)
					{
						char path[MAX_PATH] = {0};
						GetDlgItemText(hDlg,IDC_SKIN,path,MAX_PATH);
						id->SetSkin(path);

						SetDlgItemText(hDlg,IDC_NAME,id->GetSkin()->GetName());
						SetDlgItemText(hDlg,IDC_AUTHOR,id->GetSkin()->GetAuthor());
						SetDlgItemText(hDlg,IDC_INFO,id->GetSkin()->GetInfo());
					}
				break;
			}
		break;
	}
	return FALSE;
}

INT __stdcall OnAddPropertySheets(IconDrop* id, int numPresent)
{
	PROPSHEETPAGE psp = {0};

	
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = id->GetDll();
	psp.pszTitle = "ICON Formats";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_FORMATS);
	psp.pfnDlgProc = FormatsConfigProc;
	psp.lParam = (LPARAM)id;

	DeskletAddPropertySheet(id->GetWindow(),CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = id->GetDll();
	psp.pszTitle = "Skin";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_SKIN);
	psp.pfnDlgProc = SkinConfigProc;
	psp.lParam = (LPARAM)id;

	return DeskletAddPropertySheet(id->GetWindow(),CreatePropertySheetPage(&psp));
}

#include <map>

class CSkinEnumerator
{
private:
	std::map<std::wstring,std::wstring> m_Skins;
	WCHAR basePath[MAX_PATH];

public:
	CSkinEnumerator(HINSTANCE hInstance, const WCHAR* deskletPath)
	{
		WCHAR path[MAX_PATH] = {0};
		wcscpy(path,deskletPath);
		PathAppendW(path,L"skins\\");

		wcscpy(basePath,path);

		std::wstring s = path;
		s += L"*.*";

		WIN32_FIND_DATAW data = {0};
		HANDLE h = FindFirstFileW(s.c_str(),&data);
		if(h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(wcscmp(data.cFileName,L".")!=0 && wcscmp(data.cFileName,L"..")!=0 && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					m_Skins[data.cFileName] = data.cFileName;
				}while(FindNextFileW(h,&data));
			FindClose(h);
		}
	};

	UINT size()const { return m_Skins.size();};

	std::wstring getKey(UINT num)
	{
		std::map<std::wstring,std::wstring>::iterator iter = m_Skins.begin();
		std::advance(iter,num);
		//iter = num;
		return iter->first;
	};

	std::wstring getValue(std::wstring key)
	{
		return m_Skins[key];
	};

	std::wstring GetBasePath()const
	{
		return basePath;
	}
};

CSkinEnumerator* __stdcall OnCreateSkinEnumeratorW(HINSTANCE hInstance, const WCHAR* deskletPath)
{
	return new CSkinEnumerator(hInstance, deskletPath);
}

void __stdcall OnDestroySkinEnumeratorW(CSkinEnumerator* se)
{
	delete se;
}

INT __stdcall OnGetSkinW(CSkinEnumerator* se, SkinInfo* info)
{
	if(info->num >= se->size() )
		return AVEDESK_NO_MORE_SKINS;

	std::wstring skin = se->getKey(info->num);

	if(info->maxKeyLength < skin.length()+1 || info->maxNameLength < skin.length() + 1)
	{
		info->maxKeyLength = skin.length() + 1;	
		info->maxNameLength = skin.length() + 1;
		return AVEDESK_BUF_TOO_SMALL;
	}

	WCHAR ini[MAX_PATH] = {0};
	wcscpy(ini,se->GetBasePath().c_str());
	PathAppendW(ini,skin.c_str());
	PathAppendW(ini,L"skin.ini");

	GetPrivateProfileStringW(L"Info",L"Name",skin.c_str(),info->wName,info->maxNameLength,ini);
	GetPrivateProfileStringW(L"Info",L"Author",L"",info->wAuthor,info->maxAuthorLength,ini);
	GetPrivateProfileStringW(L"Info",L"Comments",L"",info->wNotes,info->maxNotesLength,ini);
	info->version = GetPrivateProfileIntW(L"Info",L"Version",200,ini);

	wcscpy(info->wKey,skin.c_str());

	return AVEDESK_MORE_SKINS;
}

IconDrop* __stdcall OnCreateFromSkinW(HWND hwnd,HINSTANCE hInstance, CSkinEnumerator* se, const SkinInfo* info)
{

	IconDrop* desklet = OnCreate(hwnd,hInstance,NULL,NULL);


	if(se != NULL)
	{
		char skinPath[MAX_PATH] = {0};
		WideCharToMultiByte(GetACP(),0,se->getValue(info->wKey).c_str(),-1,skinPath,MAX_PATH,NULL,NULL);
	
		desklet->SetSkin( skinPath );
	}
	else
	{
		char skinPath[MAX_PATH] = {0};
		WideCharToMultiByte(GetACP(),0,info->wKey,-1,skinPath,MAX_PATH,NULL,NULL);

		desklet->SetSkin(skinPath);
	}
	return desklet;
}