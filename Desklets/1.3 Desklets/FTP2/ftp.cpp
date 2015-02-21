#include <windows.h>
#include <Wininet.h>
#pragma comment(lib,"Wininet.lib")

#include "../DeskletSDK.h"

#include <queue>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")

#include "zipunzip/zipper.h"
#pragma comment(lib,"zipunzip/zlib/zlibstat.lib")

#include "resource.h"

bool IsDirectory(const char* filename)
{
	WIN32_FIND_DATA data = {0};
	HANDLE h = FindFirstFile(filename,&data);
	bool res = h != 0 && data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY;
	FindClose(h);
	return res;
}

std::string WStringToString(const std::wstring& wstr)
{
	// post: a unicode wstring has been converted to a multibyte wtring equivalent
	std::string ret;

	char* tmp = new char[ wstr.length() + 2];
	memset(static_cast<void*>(tmp),0,wstr.length()+2);

	WideCharToMultiByte(GetACP(),0,wstr.c_str(),-1,tmp,wstr.length()+1,NULL,NULL );

	ret = tmp;

	delete[] tmp;

	return ret;
}

const TYPE_PNG = 0;
const TYPE_BMP = 1;
const TYPE_JPG = 2;

struct IMAGEUPLOADSTRUCT
{
	HBITMAP hBitmap;
	std::string fileName;
	BOOL generateThumbNail;
	int type;
	int quality;
};

const TYPE_ZIP_ALL = 0;
const TYPE_ZIP_OWN = 1;
const TYPE_ZIP_NONE = 2;

struct ZIPUPLOADSTRUCT
{
	std::string fileName;
	int type;
};

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
  unsigned int  num = 0;    // number of image encoders
  unsigned int  size = 0;   // size of the image encoder array in bytes
  
  Gdiplus::GetImageEncodersSize(&num, &size);
  if(size == 0)return -1;
  
  Gdiplus::ImageCodecInfo* imageCodecInfo = new Gdiplus::ImageCodecInfo[size];
  Gdiplus::GetImageEncoders(num, size, imageCodecInfo);
  
  for(unsigned int i = 0; i < num; ++i)
  {
    if( wcscmp(imageCodecInfo[i].MimeType, format) == 0 )
    {
       *pClsid = imageCodecInfo[i].Clsid;
       delete[] imageCodecInfo;
       return i;
    }    
  }
  delete[] imageCodecInfo;
  return -1;
}

int __stdcall UploadImageDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL OpenFileNameMultipleFiles(std::vector<std::string>& files, HWND hwnd, HINSTANCE hInstance);

std::string GetDirectoryName(const char* dirName)
{
	WIN32_FIND_DATA data = {0};
	HANDLE h = FindFirstFile(dirName,&data);
	FindClose(h);
	return std::string(data.cFileName);
}

int __stdcall SkinConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);

class FTP
{
	private:
		HWND		hwndDesklet;
		HINSTANCE	hInstanceDll;

		std::queue<std::string> files;
		std::vector<std::string> dirs;

		std::vector<Bitmap*> bmps;


		std::string name;
		std::string host;
		unsigned int	port;
		std::string username;
		std::string password;
		bool isPassive;

		std::string imgNormal;
		std::string imgTransfer;
		std::string imgUploading;
		std::string imgProgress;
		std::string imgError;
		RECT progressRect;
		UINT progressDir;

		std::string linkFormat;

		std::string links;

		bool canAccept;
		bool readyFlag;
		bool showErrMsg;

		std::string dir;

		HINTERNET hInet;
		HINTERNET hFtp;

		HANDLE hThread;

		char skinDir[MAX_PATH];
		char skinName[1024];
		char skinAuthor[1024];
		char skinInfo[1024];

		bool skinIsChanged;

		bool stoppedUploading;

		IMAGEUPLOADSTRUCT defaultImageUploadStruct;
		ZIPUPLOADSTRUCT	  defaultZipUploadStruct;

		bool shouldZip;

	public:
		FTP(HWND hwndDesklet, HINSTANCE hInstanceDll);
		~FTP();

		void ReadFromIni(const char* ini, const char* group);
		void SaveToIni(const char* ini, const char* group);

		void AddFile(const std::string& file);

		void StartUploading();
		bool IsUploading();
		void AddDirectory(const std::string& dir);
		bool UploadFile();

		bool Connect();
		void Disconnect();

		void UpdateProgressBar(Bitmap* bmp, UINT id,float percentage);

		void SetNormalImage(const std::string& s);
		std::string GetNormalImage() const;

		void SetTransferImage(const std::string& s);
		std::string GetTransferImage() const;

		void SetUploadingImage(const std::string& s);
		std::string GetUploadingImage() const;

		void SetErrorImage(const std::string& s);
		std::string GetErrorImage() const;

		void SetProgressImage(const std::string& s);
		std::string GetProgressImage() const;

		void SetHost(const std::string& h);
		std::string GetHost()const;

		void SetPort(unsigned int p);
		unsigned int GetPort()const;

		void SetUsername(const std::string& u);
		std::string GetUsername()const;

		void SetPassword(const std::string& pw);
		std::string GetPassword()const;

		void SetIsPassive(bool p);
		bool GetIsPassive()const;

		void SetDir(const std::string& d);
		std::string GetDir()const;

		void SetLinkFormat(const std::string& f){linkFormat = f;};
		std::string GetLinkFormat()const { return linkFormat;};

		std::string FormatLink(const std::string& file)const;

		int GetNumberOfFiles()const;

		HINSTANCE GetDLL(){return hInstanceDll;};

		HWND GetWindow() { return hwndDesklet; };

		void SetCanAccept(bool b){canAccept = b;};
		bool GetCanAccept(){return canAccept;};

		void SetReadyFlag(bool b){readyFlag = b;};
		bool GetReadyFlag(){return readyFlag;};

		void SetProgressRect(const RECT& r);
		RECT GetProgressRect()const;

		void SetLinks(const std::string& l){links  = l;};
		std::string GetLinks()const{return links;};

		void SetName(const std::string& n){name = n;};
		std::string GetName()const{return name;};

		void SetShowErrMsg(const bool b){showErrMsg=b;};
		bool GetShowErrMsg()const{return showErrMsg;};

		UINT GetProgressDir()const{return progressDir;};
		void SetProgressDir(UINT v){progressDir=v;};

		void SetSkin(const char* dir, bool noResize=false);

		const char* GetSkinName()const{return skinName;};
		const char* GetSkinAuthor()const {return skinAuthor;};
		const char* GetSkinInfo()const{return skinInfo;};

		const char* GetSkinDir()const{return skinDir;};

		void SetSkinIsChanged(bool v){skinIsChanged=v;};
		bool GetSkinIsChanged()const{return skinIsChanged;};

		HPROPSHEETPAGE hPageSkin;
		HPROPSHEETPAGE hPageImage;
		HPROPSHEETPAGE hPageProgess;

		void AddBitmap(Bitmap* bmp)
		{
			bmps.push_back(bmp);
		}

		void StopUploading();
		bool GetStoppedUploading();
		void ClearFiles();

		IMAGEUPLOADSTRUCT GetImageUploadStruct()const
		{
			return defaultImageUploadStruct;
		}

		void SetImageUploadStruct(const IMAGEUPLOADSTRUCT& ius)
		{
			defaultImageUploadStruct = ius;
		}

		ZIPUPLOADSTRUCT GetDefaultZipUploadStruct()const
		{
			return defaultZipUploadStruct;
		}

		void SetZipUploadSturct(const ZIPUPLOADSTRUCT& zus)
		{
			defaultZipUploadStruct = zus;
		}

		bool GetShouldZip()const
		{
			return shouldZip;
		}

		void SetShouldZip(bool val)
		{
			shouldZip = val;
		}

		bool UploadMultipleFiles(std::vector<std::string>& files, bool alwaysZip = false);

};

int __stdcall UploadZipDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

bool FTP::UploadMultipleFiles(std::vector<std::string>& files, bool alwaysZip)
{
	if(shouldZip || alwaysZip)
	{
		if(files.size() > 1)
		{
			if(GetKeyState(VK_SHIFT) < 0 || DialogBoxParam(GetDLL(),MAKEINTRESOURCE(IDD_ZIP),GetWindow(),UploadZipDlgProc,(LPARAM)&defaultZipUploadStruct) == TRUE)
			{
				if(TYPE_ZIP_ALL == defaultZipUploadStruct.type)
				{
					std::stringstream ss;
					ss << "zipping " << files.size() << " files(s)";

					if(!IsUploading())
						DeskletSetSubLabel(GetWindow(),const_cast<char*>(ss.str().c_str() ));

					std::string tmp = defaultZipUploadStruct.fileName;
					if(tmp.find(".") == std::string::npos)
						tmp += ".zip";

					char fileName[MAX_PATH] = {0};
					GetTempPath(MAX_PATH,fileName);
					PathAppend(fileName,tmp.c_str());

					CZipper zipper;
					zipper.OpenZip(fileName);
					
					for(std::vector<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter)
					{
						std::string& file = *iter;

						if(IsDirectory(file.c_str()))
							zipper.AddFolderToZip(file.c_str());
						else
							zipper.AddFileToZip(file.c_str(),TRUE);
					}

					zipper.CloseZip();

					std::string upFileName("*");
					upFileName += fileName;
					AddFile(upFileName);
				}
				else if(TYPE_ZIP_OWN == defaultZipUploadStruct.type)
				{
					std::stringstream ss;
					ss << "zipping " << files.size() << " files(s)";

					if(!IsUploading())
						DeskletSetSubLabel(GetWindow(),const_cast<char*>(ss.str().c_str() ));

					for(std::vector<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter)
					{
						char singleFile[MAX_PATH+5] = {0};
						strcpy(singleFile,iter->c_str());
						char* ext = PathFindExtension(singleFile);
						if(ext != NULL)
							*ext = '\0';

						strcat(singleFile,".zip");

						char fileName[MAX_PATH] = {0};
						GetTempPath(MAX_PATH,fileName);
						PathAppend(fileName,singleFile);

							
						CZipper zipper;
						zipper.OpenZip(fileName);

						std::string& file = *iter;

						if(IsDirectory(file.c_str()))
							zipper.AddFolderToZip(file.c_str());
						else
							zipper.AddFileToZip(file.c_str());

						zipper.CloseZip();

						std::string upFileName("*");
						upFileName += fileName;
						AddFile(upFileName);
					}	
				}
				else if(TYPE_ZIP_NONE == defaultZipUploadStruct.type)
				{
					for(std::vector<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter)
						AddFile(*iter);
				}
			}
			else
			{
				return false;
			}
		}
		else if(files.size() == 1)
		{
			std::stringstream ss;
			ss << "zipping file";

			if(!IsUploading())
				DeskletSetSubLabel(GetWindow(),const_cast<char*>(ss.str().c_str() ));

			char singleFile[MAX_PATH+5] = {0};
			strcpy(singleFile,files[0].c_str());
			char* ext = PathFindExtension(singleFile);
			if(ext != NULL)
				*ext = '\0';

			strcat(singleFile,".zip");

			char fileName[MAX_PATH] = {0};
			GetTempPath(MAX_PATH,fileName);
			PathAppend(fileName,singleFile);

			CZipper zipper;
			zipper.OpenZip(fileName);

			std::string& file = files[0];

			if(IsDirectory(file.c_str()))
				zipper.AddFolderToZip(file.c_str());
			else
				zipper.AddFileToZip(file.c_str());

			zipper.CloseZip();

			std::string upFileName("*");
			upFileName += fileName;
			AddFile(upFileName);
		}
	}
	else
	{
		for(std::vector<std::string>::iterator iter = files.begin(); iter != files.end(); ++iter)
		{
			AddFile(*iter);
		}
	}

	if(!IsUploading())
		StartUploading();
	else
	{
		std::stringstream ss;
		ss << GetNumberOfFiles()+1 <<" files left";
		DeskletSetSubLabel(GetWindow(),const_cast<char*>(ss.str().c_str() ));
	}

	return true;
}

const DIR_LEFT_RIGHT = 0;
const DIR_RIGHT_LEFT = 1;
const DIR_TOP_BOTTOM = 2;
const DIR_BOTTOM_TOP = 3;

FTP::FTP(HWND hwndDesklet, HINSTANCE hInstanceDll) :
	hwndDesklet	(hwndDesklet),
	hInstanceDll(hInstanceDll),
	hInet		(0),
	hFtp		(0),
	hThread		(0),
	port		(21),
	canAccept	(false),
	readyFlag	(false),
	showErrMsg	(false),
	progressDir (DIR_LEFT_RIGHT),
	skinIsChanged(false),
	hPageSkin	(0),
	hPageImage	(0),
	hPageProgess(0),
	shouldZip(false)
{
	progressRect.bottom = 177;
	progressRect.left = 12;
	progressRect.right = 181;
	progressRect.top = 150;

	memset(skinAuthor,0,1024);
	memset(skinName,0,1024);
	memset(skinInfo,0,1024);

	memset(skinDir,0,MAX_PATH);
	strcpy(skinDir,"Default");

	defaultImageUploadStruct.fileName = "NewFile";
	defaultImageUploadStruct.generateThumbNail = FALSE;
	defaultImageUploadStruct.hBitmap = FALSE;
	defaultImageUploadStruct.quality = 100;
	defaultImageUploadStruct.type = TYPE_PNG;

	defaultZipUploadStruct.fileName = "ZippedFile";

	hInet = InternetOpen("AveFTP",INTERNET_OPEN_TYPE_PRECONFIG,0,0,0);
	if(!hInet)
		MessageBox(0,"Could not initialize wininet.dll, so this desklet won\'t be of any use for you.","FTP Desklet",MB_ICONERROR);
}

FTP::~FTP()
{
	InternetCloseHandle(hInet);

	if(hThread)
	{
		TerminateThread(hThread,0);
		CloseHandle(hThread);
	}

	if(hFtp)
		InternetCloseHandle(hFtp);

	for(std::vector<Bitmap*>::iterator iter = bmps.begin(); iter != bmps.end(); ++iter)
	{
		Bitmap* bmp = *iter;
		delete bmp;
	}
	bmps.clear();

	ClearFiles();
}

Rect GetPrivateProfileRect(const char* section, const char* ini)
{
	return Rect(	GetPrivateProfileInt(section,"Left",0,ini),
					GetPrivateProfileInt(section,"Top",0,ini),
					GetPrivateProfileInt(section,"Width",0,ini),
					GetPrivateProfileInt(section,"Height",0,ini)
				);
}

void FTP::SetSkin(const char* dir, bool noResize)
{
	strcpy(skinDir,dir);
	char path[MAX_PATH] = {0};
	GetDLLPath(GetDLL(),path,MAX_PATH);
	PathAppend(path,"skins\\");
	PathAppend(path,dir);
	PathAppend(path,"\\");

	char ini[MAX_PATH] = {0};
	strcpy(ini,path);
	PathAppend(ini,"\\skin.ini");

	strcpy(skinDir,dir);

	char type[1024] = {0};
	GetPrivateProfileString("Info","Type",type,type,1024,ini);
	if(stricmp(type,"FTP")!=0)
	{
		std::string msg = "This does not appear to be a skin for the FTP desklet.\n";
		if(type[0] != 0)
		{
			msg += "It looks like it\'s a skin for the ";
			msg += type;
			msg += "desklet.\n";
			msg += "Therefore, the FTP desklet cannnot use this skin.";
			MessageBox(GetActiveWindow(),msg.c_str(),"FTP Desklet - Set Skin",MB_ICONERROR);
			return;
		}
		else
		{
			msg += "No specific desklet type is specified at all. Do you want to try it anyway?";
			if(MessageBox(GetActiveWindow(),msg.c_str(),"FTP Desklet - Set Skin",MB_ICONERROR | MB_YESNO)!=IDYES)
				return;
		}
	}
		
		GetPrivateProfileString("Info","Name","",skinName,1024,ini);
		GetPrivateProfileString("Info","Author","",skinAuthor,1024,ini);
		GetPrivateProfileString("Info","Comments","",skinInfo,1024,ini);

		if(!noResize)
		{
			SIZE s = {0};
			DeskletGetSize(GetWindow(),&s);
			s.cx  = GetPrivateProfileInt("BG","Width",s.cx,ini);
			s.cy  = GetPrivateProfileInt("BG","Height",s.cy,ini);

			
			POINT closePoint = {0};
			closePoint.x = GetPrivateProfileInt("CloseButton","X",s.cx,ini);
			closePoint.y = GetPrivateProfileInt("CloseButton","Y",s.cx,ini);
			DeskletSetClosePoint(GetWindow(), &closePoint, s.cx, s.cy);

			POINT p ={0};
			DeskletGetPosition(GetWindow(),&p);
			RECT r = {0};
			r.left = p.x;
			r.top = p.y;
			r.right = r.left + s.cx;
			r.bottom = r.top + s.cy;
			DeskletMove(GetWindow(),&r,FALSE);
		}


		char tmp[MAX_PATH] = {0};

		memset(tmp,0,MAX_PATH);
		GetPrivateProfileString("Images","Normal","normal.png",tmp,MAX_PATH,ini);
		char ni[MAX_PATH] = {0};
		strcpy(ni,path);
		PathAppend(ni,tmp);
		SetNormalImage(ni);

		memset(tmp,0,MAX_PATH);
		GetPrivateProfileString("Images","Transfer","tranfer.png",tmp,MAX_PATH,ini);
		char ti[MAX_PATH] = {0};
		strcpy(ti,path);
		PathAppend(ti,tmp);
		SetTransferImage(ti);

		memset(tmp,0,MAX_PATH);
		GetPrivateProfileString("Images","Uploading","uploading.png",tmp,MAX_PATH,ini);
		char ui[MAX_PATH] = {0};
		strcpy(ui,path);
		PathAppend(ui,tmp);
		SetUploadingImage(ui);

		memset(tmp,0,MAX_PATH);
		GetPrivateProfileString("Images","Error","error.png",tmp,MAX_PATH,ini);
		char ei[MAX_PATH] = {0};
		strcpy(ei,path);
		PathAppend(ei,tmp);
		SetErrorImage(ei);

		memset(tmp,0,MAX_PATH);
		GetPrivateProfileString("ProgressBar","Image","progress.png",tmp,MAX_PATH,ini);
		char pi[MAX_PATH] = {0};
		strcpy(pi,path);
		PathAppend(pi,tmp);
		SetProgressImage(pi);

		Rect tmpRect = GetPrivateProfileRect("ProgressBar",ini);
		RECT r2 = {0};
		r2.left = tmpRect.X;
		r2.top = tmpRect.Y;
		r2.right = r2.left + tmpRect.Width;
		r2.bottom = r2.top + tmpRect.Height;
		SetProgressRect(r2);

		SetProgressDir( GetPrivateProfileInt("ProgressBar","Direction",DIR_LEFT_RIGHT,ini) );

		if(IsUploading())
			DeskletSetImageFile(GetWindow(),GetUploadingImage().c_str());
		else
			DeskletSetImageFile(GetWindow(),GetNormalImage().c_str());
}

int FTP::GetNumberOfFiles()const
{
	return files.size();
}

void FTP::SetNormalImage(const std::string& s)
{
	imgNormal = s;
	if(!IsUploading())
		DeskletSetImageFile(hwndDesklet,const_cast<char*>(imgNormal.c_str()));
}

std::string FTP::GetNormalImage() const
{
	return imgNormal;	
}

void FTP::SetTransferImage(const std::string& s)
{
	imgTransfer = s;
}

std::string FTP::GetTransferImage() const
{
	return imgTransfer;
}

void FTP::SetUploadingImage(const std::string& s)
{
	imgUploading = s;
	if(IsUploading())
		DeskletSetImageFile(hwndDesklet,const_cast<char*>(imgUploading.c_str()));
}

std::string FTP::GetUploadingImage() const
{
	return imgUploading;
}

void FTP::SetProgressImage(const std::string& s)
{
	imgProgress = s;
	BOOL success(FALSE);
	UINT id = DeskletLayerGetLayer(hwndDesklet,"FTP.ProgressBar",&success);
	if(!success)
	{
		id = DeskletLayerAdd(hwndDesklet,"FTP.ProgressBar");
		DeskletLayerSetVisibility(hwndDesklet,id,FALSE,IsUploading() ? TRUE : FALSE);
	}

	//DeskletLayerSetImageFile(hwndDesklet,id,TRUE,const_cast<char*>(imgProgress.c_str()));
}

std::string FTP::GetProgressImage() const
{
	return imgProgress;
}

void FTP::SetErrorImage(const std::string& s)
{
	imgError = s;
}

std::string FTP::GetErrorImage() const
{
	return imgError;
}


void FTP::SetProgressRect(const RECT& r)
{
	progressRect = r;

	BOOL success(FALSE);
	UINT id = DeskletLayerGetLayer(hwndDesklet,"FTP.ProgressBar",&success);
	if(!success)
	{
		id = DeskletLayerAdd(hwndDesklet,"FTP.ProgressBar");
		DeskletLayerSetVisibility(hwndDesklet,id,FALSE,IsUploading() ? TRUE : FALSE);
	}

	//DeskletLayerSetPlacement(hwndDesklet,id,TRUE,TRUE,&progressRect,192,192);
}

RECT FTP::GetProgressRect()const
{
	return progressRect;
}

void findandreplace(std::string& source, const std::string& find, const std::string& replace)
{
	size_t j;
	for(; (j = source.find(find)) != std::string::npos;)
	{
		source.replace(j,find.length(),replace);
	}
}

std::string FTP::FormatLink(const std::string& file)const
{
	std::string ret = linkFormat;
	findandreplace(ret,"{server}",host);
	findandreplace(ret,"%s",host);
	findandreplace(ret,"{username}",username);
	findandreplace(ret,"%u",username);
	findandreplace(ret,"{password}",password);
	findandreplace(ret,"%pw",password);
	std::stringstream ss;
	ss << port;
	findandreplace(ret,"{port}",ss.str());
	findandreplace(ret,"%p",ss.str());
	findandreplace(ret,"{directory}",dir);
	findandreplace(ret,"%d",dir);
	findandreplace(ret,"{file}",file);
	findandreplace(ret,"%f",file);

	char buf[INTERNET_MAX_URL_LENGTH+2] = {0};
	DWORD bufSize = INTERNET_MAX_URL_LENGTH+1;
	UrlEscape(ret.c_str(),buf,&bufSize, URL_ESCAPE_SPACES_ONLY | URL_DONT_ESCAPE_EXTRA_INFO);
	ret = buf;

	return ret;
}

void FTP::UpdateProgressBar(Bitmap* bmp, UINT id, float percentage)
{
	RECT r = GetProgressRect();

	SIZE s = {0};
	DeskletGetSize(hwndDesklet,&s);
	Bitmap* tmp = new Bitmap(s.cx,s.cy);

	Graphics g(tmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	Rect clipR;

	UINT pd = GetProgressDir();
	if(pd == DIR_LEFT_RIGHT)
	{
		int w = r.right - r.left;
		w = (int)(percentage/100.0f * w);

		clipR.Width = (int)(float(w+r.left)/192 * s.cx);
		clipR.Height = s.cy;
	}
	else if(pd == DIR_RIGHT_LEFT)
	{
		int w = r.right - r.left;
		w = (int)(percentage/100.0f * w);

		clipR.Width = (int)(float(w)/192 * s.cx);
		clipR.X = (int)((float)(r.right)/192 * s.cx) - clipR.Width;
		clipR.Height = s.cy;
	}
	else if(pd == DIR_TOP_BOTTOM)
	{
		int h = r.bottom - r.top;
		h = (int)(percentage/100.0f * h);

		clipR.Height = (int)(float(h+r.top)/192 * s.cy);
		clipR.Width = s.cx;
	}
		else if(pd == DIR_BOTTOM_TOP)
	{
		int h = r.bottom - r.top;
		h = (int)(percentage/100.0f * h);

		clipR.Height = (int)(float(h)/192 * s.cy);
		clipR.Y = (int)((float)(r.bottom)/192 * s.cy) - clipR.Height;
		clipR.Width = s.cx;
	}

	g.SetClip(clipR);
	g.DrawImage(bmp,(int)((float)r.left/192 *s.cx),(int)((float)r.top/192 * s.cy),(int)((float)(r.right - r.left)/192 * s.cx),(int)((float)(r.bottom - r.top)/192 * s.cy));

	DeskletLayerSetVisibility(hwndDesklet,id,FALSE,TRUE);
	DeskletLayerSetImage(hwndDesklet,id,TRUE,tmp,TRUE);
}

void FTP::StopUploading()
{
	stoppedUploading = true;
}

bool FTP::UploadFile()
{
	if(GetNumberOfFiles() < 1)
	{
		DeskletSetSubLabel(hwndDesklet,"");
		return false;
	}


	bool deleteFile(false);

	std::string file(files.front());

	files.pop();

	std::stringstream ss;
	ss << GetNumberOfFiles()+1;
	if(GetNumberOfFiles() > 0)
		ss <<" files left";
	else
		ss << " file left";
	DeskletSetSubLabel(hwndDesklet,const_cast<char*>(ss.str().c_str() ));

	if(file[0]=='?')
	{
		file.erase(file.begin() );
		FtpCreateDirectory(hFtp,file.c_str());
		FtpSetCurrentDirectory(hFtp,file.c_str());

		if(file == "..")
		{
			if(dirs.size() > 0)
				dirs.erase(dirs.end()-1 );
		}
		else
			dirs.push_back(file);

		return true;
	}
	else if(file[0] == '*')
	{
		deleteFile = true;
		file = file.substr(1,file.size()-1);
	}
	
	std::string filename("");
		
	std::string::reverse_iterator dotPosition = std::find(file.rbegin(),file.rend(),'\\');
	
	
	if(dotPosition == file.rend() )
		dotPosition = std::find(file.rbegin(),file.rend(),'/');

	std::reverse_copy(file.rbegin(),dotPosition,std::insert_iterator<std::string>(filename,filename.begin()) );


	char fbuf[MAX_PATH];
	strcpy(fbuf,file.c_str());

	char fnbuf[MAX_PATH];
	strcpy(fnbuf,filename.c_str());

	BOOL success(FALSE);
	UINT id = DeskletLayerGetLayer(hwndDesklet,"FTP.ProgressBar",&success);
	if(!success)
	{
		id = DeskletLayerAdd(hwndDesklet,"FTP.ProgressBar");
	}

	//return FtpPutFile(hFtp,fbuf,fnbuf,FTP_TRANSFER_TYPE_BINARY,0) == TRUE;
	HANDLE h = CreateFile(fbuf,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if(h)
	{
		//FtpGetFileSize
		HINTERNET hFile = FtpOpenFile(hFtp,fnbuf,GENERIC_WRITE,FTP_TRANSFER_TYPE_BINARY,0);
		//InternetSetStatusCallback(hFile,InternetStatusCallback);

		DWORD fileSize = GetFileSize(h,0);

		const maxbuf = 8192;
		char buf[maxbuf];

		DWORD numRead = maxbuf;
		DWORD numWritten = 0;

		DWORD i = 0;

		DWORD numWrittenTotal=0;
		DWORD numShouldBeWritten=0;

		Bitmap* bmp = DeskletLoadGDIPlusImage(const_cast<char*>(GetProgressImage().c_str()));

		while(numRead==maxbuf && !stoppedUploading)
		{
			ReadFile(h,buf,maxbuf,&numRead,0);
			numShouldBeWritten+=numRead;

			if(!InternetWriteFile(hFile,buf,numRead,&numWritten))
				break;

			i += numRead;
			numWrittenTotal += numWritten;

			UpdateProgressBar(bmp,id,((float)i)/((float)fileSize) * 100.0f);
		/*	RECT r = GetProgressRect();
			int w = r.right - r.left;
			w = (int)((float)i/fileSize * w);
			SIZE s = {0};
			DeskletGetSize(hwndDesklet,&s);
			Bitmap* tmp = new Bitmap(s.cx,s.cy);
			Graphics g(tmp);
			Rect clipR;
			clipR.Width = (int)(float(w+r.left)/192 * s.cx);
			clipR.Height = s.cy;
			g.SetClip(clipR);
			g.SetSmoothingMode(SmoothingModeAntiAlias);
			g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			g.DrawImage(bmp,(int)((float)r.left/192 *s.cx),(int)((float)r.top/192 * s.cy),(int)((float)(r.right - r.left)/192 * s.cx),(int)((float)(r.bottom - r.top)/192 * s.cy));
			DeskletLayerSetVisibility(hwndDesklet,id,FALSE,TRUE);
			DeskletLayerSetImage(hwndDesklet,id,TRUE,tmp,TRUE);
		*/
		}
		if(numWrittenTotal != numShouldBeWritten && !stoppedUploading)
		{
			if(GetShowErrMsg() )
			{
				DWORD error(0);
				DWORD errorSize(10024);
				char  errorMsg[100024] = {0};
				InternetGetLastResponseInfo(&error,errorMsg,&errorSize);
				if(errorMsg[0] == 0)
				{
					//error = GetLastError();
					//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,(LPVOID)errorMsg,error,0,errorMsg,sizeof(errorMsg)/sizeof(errorMsg[0]),0);
					strcpy(errorMsg,"The address supplied for the FTP server does not seem to exist.\nMake sure you did not add ftp:// in front of the address.");
				}
				MessageBox(GetWindow(),errorMsg,"FTP Error",MB_ICONERROR);
			}
		}

		delete bmp;
		DeskletLayerSetVisibility(hwndDesklet,id,TRUE,FALSE);
		
		//InternetSetStatusCallback(hFile,0);
		InternetCloseHandle(hFile);
		CloseHandle(h);

		if(numWrittenTotal == numShouldBeWritten)
			links +=FormatLink(filename);

		if (GetNumberOfFiles() > 0)
			links += std::string("\r\n");

		if(deleteFile)
			DeleteFile(file.c_str());

		return true;
	}
	else
	{
		return false;
	}
}

void FTP::ReadFromIni(const char* ini, const char* group)
{
	char buf[1024];

	GetPrivateProfileString(group,"Name","",buf,1024,ini);
	SetName(buf);

	GetPrivateProfileString(group,"Host","",buf,1024,ini);
	SetHost(buf);

	SetPort( GetPrivateProfileInt(group,"Port",21,ini) );

	GetPrivateProfileString(group,"Username","",buf,1024,ini);
	SetUsername(buf);

	GetPrivateProfileString(group,"PW","",buf,1024,ini);
	SetPassword(buf);

	SetIsPassive( GetPrivateProfileInt(group,"Passive",TRUE,ini) == TRUE);

	GetPrivateProfileString(group,"Dir","",buf,1024,ini);
	SetDir(buf);

	GetPrivateProfileString(group,"NormalImg","",buf,1024,ini);
	SetNormalImage(buf);

	GetPrivateProfileString(group,"TransferImg","",buf,1024,ini);
	SetTransferImage(buf);

	GetPrivateProfileString(group,"UploadingImg","",buf,1024,ini);
	SetUploadingImage(buf);

	GetPrivateProfileString(group,"ProgressImg","",buf,1024,ini);
	SetProgressImage(buf);

	GetPrivateProfileString(group,"ErrorImg","",buf,1024,ini);
	SetErrorImage(buf);

	SetProgressDir(GetPrivateProfileInt(group,"ProgressDir",GetProgressDir(),ini));


	char fileName[MAX_PATH] = {0};
	GetPrivateProfileString(group,"IUSFileName",defaultImageUploadStruct.fileName.c_str(),fileName,MAX_PATH,ini);
	defaultImageUploadStruct.fileName = fileName;
	defaultImageUploadStruct.generateThumbNail = GetPrivateProfileInt(group,"IUSGenerateThumbNail",defaultImageUploadStruct.generateThumbNail,ini);
	defaultImageUploadStruct.quality = GetPrivateProfileInt(group,"IUSQuality",defaultImageUploadStruct.quality,ini);
	defaultImageUploadStruct.type = GetPrivateProfileInt(group,"IUSType",defaultImageUploadStruct.type,ini);



	GetPrivateProfileString(group,"ZUSFileName",defaultZipUploadStruct.fileName.c_str(),fileName,MAX_PATH,ini);
	defaultZipUploadStruct.fileName = fileName;
	defaultZipUploadStruct.type = GetPrivateProfileInt(group,"ZUSType",defaultZipUploadStruct.type,ini);


	GetPrivateProfileString(group,"LinkFormat","",buf,1024,ini);
	SetLinkFormat(buf);

	RECT r = {0};
	r.bottom	= GetPrivateProfileInt(group,"PR_B",0,ini);
	r.left		= GetPrivateProfileInt(group,"PR_L",0,ini);
	r.right		= GetPrivateProfileInt(group,"PR_R",0,ini);
	r.top		= GetPrivateProfileInt(group,"PR_T",0,ini);
	SetProgressRect(r);

	SetShowErrMsg( GetPrivateProfileInt(group,"ShowErrMsg",GetShowErrMsg() ? TRUE : FALSE,ini) == TRUE );

	SetSkinIsChanged( GetPrivateProfileInt(group,"SkinIsChanged",FALSE,ini)==TRUE);
	GetPrivateProfileString(group,"SkinDir",skinDir,skinDir,MAX_PATH,ini);

	shouldZip	= GetPrivateProfileInt(group,"ShouldZip",FALSE,ini) == TRUE;
}

void FTP::SaveToIni(const char* ini, const char* group)
{
	WritePrivateProfileString(group,"Name",GetName().c_str(),ini);
	WritePrivateProfileString(group,"Host",GetHost().c_str(),ini);
	WritePrivateProfileInt(group,"Port",GetPort(),ini);
	WritePrivateProfileString(group,"Username",GetUsername().c_str(),ini);
	WritePrivateProfileString(group,"PW",GetPassword().c_str(),ini);
	WritePrivateProfileInt(group,"Passive", GetIsPassive() ? TRUE : FALSE,ini);
	WritePrivateProfileString(group,"Dir", GetDir().c_str(),ini);

	WritePrivateProfileString(group,"NormalImg", GetNormalImage().c_str(),ini);
	WritePrivateProfileString(group,"TransferImg", GetTransferImage().c_str(),ini);
	WritePrivateProfileString(group,"UploadingImg", GetUploadingImage().c_str(),ini);
	WritePrivateProfileString(group,"ProgressImg", GetProgressImage().c_str(),ini);
	WritePrivateProfileString(group,"ErrorImg", GetErrorImage().c_str(),ini);

	WritePrivateProfileInt(group,"ProgressDir",GetProgressDir(),ini);

	WritePrivateProfileInt(group,"ShouldZip",shouldZip ? TRUE : FALSE,ini);

	WritePrivateProfileString(group,"LinkFormat", GetLinkFormat().c_str(),ini);

	RECT& r = GetProgressRect();
	WritePrivateProfileInt(group,"PR_B",r.bottom,ini);
	WritePrivateProfileInt(group,"PR_L",r.left,ini);
	WritePrivateProfileInt(group,"PR_R",r.right,ini);
	WritePrivateProfileInt(group,"PR_T",r.top,ini);

	WritePrivateProfileInt(group,"ShowErrMsg",GetShowErrMsg() ? TRUE : FALSE,ini);

	WritePrivateProfileInt(group,"SkinIsChanged",GetSkinIsChanged(),ini);
	WritePrivateProfileString(group,"SkinDir",skinDir,ini);

	WritePrivateProfileString(group,"IUSFileName",defaultImageUploadStruct.fileName.c_str(),ini);
	WritePrivateProfileInt(group,"IUSGenerateThumbNail",defaultImageUploadStruct.generateThumbNail,ini);
	WritePrivateProfileInt(group,"IUSQuality",defaultImageUploadStruct.quality,ini);
	WritePrivateProfileInt(group,"IUSType",defaultImageUploadStruct.type,ini);


	WritePrivateProfileString(group,"ZUSFileName",defaultZipUploadStruct.fileName.c_str(),ini);
	WritePrivateProfileInt(group,"ZUSType",defaultZipUploadStruct.type,ini);
}

void FTP::AddDirectory(const std::string& dir)
{
	std::string dirName = GetDirectoryName(dir.c_str());
	files.push("?"+dirName);
	WIN32_FIND_DATA data = {0};
	HANDLE h = FindFirstFile(std::string(dir+"\\*.*").c_str(),&data);
	if(h)
	{
		do
		{
			if(strcmp(data.cFileName,".")!=0 && strcmp(data.cFileName,"..")!=0)
			{
				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					AddDirectory(std::string(dir+"\\"+data.cFileName));
				else
					files.push(std::string(dir+"\\"+data.cFileName));
			}
		}while(FindNextFile(h,&data));
	}

	files.push("?..");
}

void FTP::AddFile(const std::string& file)
{
	if(IsDirectory(file.c_str()) )
		AddDirectory(file);
	else
		files.push(file);
}


bool FTP::Connect()
{
	char hbuf[1024] = {0};
	char ubuf[1024] = {0};
	char pwbuf[1024] = {0};
	strcpy(hbuf,GetHost().c_str() );
	strcpy(ubuf,GetUsername().c_str());
	strcpy(pwbuf,GetPassword().c_str() );

	hFtp = InternetConnect(hInet,hbuf,
	INTERNET_DEFAULT_FTP_PORT,
	ubuf,
	pwbuf,
	INTERNET_SERVICE_FTP,
	GetIsPassive() ? INTERNET_FLAG_PASSIVE : 0,0);

	if(!hFtp)
	{
		if(GetShowErrMsg() )
		{
			DWORD error(0);
			DWORD errorSize(10024);
			char  errorMsg[100024] = {0};
			InternetGetLastResponseInfo(&error,errorMsg,&errorSize);
			if(errorMsg[0] == 0)
			{
				//error = GetLastError();
				//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,(LPVOID)errorMsg,error,0,errorMsg,sizeof(errorMsg)/sizeof(errorMsg[0]),0);
				strcpy(errorMsg,"The address supplied for the FTP server does not seem to excist.\nMake sure you did not add ftp:// in front of the address.");
			}
			MessageBox(GetWindow(),errorMsg,"FTP Error",MB_ICONERROR);
		}
	}

	if(GetDir()!="")
	{
		char dbuf[1024] = {0};
		strcpy(dbuf,GetDir().c_str() );
		FtpSetCurrentDirectory(hFtp,dbuf);
	}

	return hFtp != 0;
}

void FTP::Disconnect()
{
	if(hFtp)
		InternetCloseHandle(hFtp);
	hFtp = 0;
	CloseHandle(hThread);
	hThread = 0;

}

int __stdcall LinksProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			FTP* ftp = (FTP*)lParam;
			if(ftp)
				SetDlgItemText(hDlg,IDC_EDIT,ftp->GetLinks().c_str());
		}
		break;

		case WM_CLOSE:
			DestroyWindow(hDlg);
		break;
	}
	return FALSE;
}

bool FTP::GetStoppedUploading()
{
	return stoppedUploading;
}

void FTP::ClearFiles()
{
	while(!files.empty())
	{
		std::string f = files.front();
		if(f[0] == '*')
		{
			f = f.substr(1,f.length()-1);
			DeleteFile(f.c_str());
		}
		files.pop();
	}

	dirs.clear();
}

DWORD WINAPI UploadFunc( LPVOID lpVoid)
{
	FTP* ftp = (FTP*) lpVoid;
	
	KillTimer(ftp->GetWindow(),100);

	if(ftp->Connect() )
	{
		ftp->SetLinks("");

		DeskletSetImageFile(ftp->GetWindow(),const_cast<char*>(ftp->GetUploadingImage().c_str()));

		while(ftp->GetNumberOfFiles() > 0 && !ftp->GetStoppedUploading())
		{
			ftp->UploadFile();
		}
		DeskletSetImageFile(ftp->GetWindow(),const_cast<char*>(ftp->GetNormalImage().c_str()));
		DeskletSetSubLabel(ftp->GetWindow(),"");

		if(ftp->GetStoppedUploading())
		{
			ftp->ClearFiles();
		}


		//DialogBoxParam(ftp->GetDLL(),MAKEINTRESOURCE(IDD_LINKS),0,LinksProc,(LPARAM)ftp);
	}
	else
	{
		DeskletSetSubLabel(ftp->GetWindow(),"connecting error");
		DeskletSetImageFile(ftp->GetWindow(),const_cast<char*>(ftp->GetErrorImage().c_str()));
		SetTimer(ftp->GetWindow(),100,5000,0);
	}

	ftp->Disconnect();
	ExitThread(0);
	return 0;
}

void __stdcall OnProcessMessage(FTP* ftp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_TIMER && wParam == 100)
	{
		DeskletSetImageFile(ftp->GetWindow(),const_cast<char*>(ftp->GetNormalImage().c_str()));
		DeskletSetSubLabel(ftp->GetWindow(),"");
		KillTimer(hwnd,wParam);
	}
}

void FTP::StartUploading()
{
	stoppedUploading = false;
	DWORD dummy(0);
	hThread = CreateThread(0,0,UploadFunc,(LPVOID)this,0,&dummy);
}

bool FTP::IsUploading()
{
	return hThread != 0;
}

void FTP::SetHost(const std::string& h)
{
	host = h;
}

std::string FTP::GetHost()const
{
	return host;
}

void FTP::SetPort(unsigned int p)
{
	port = p;
}

unsigned int FTP::GetPort()const
{
	return port;
}

void FTP::SetUsername(const std::string& u)
{
	username = u;
}

std::string FTP::GetUsername()const
{
	return username;
}

void FTP::SetPassword(const std::string& pw)
{
	password = pw;
}

std::string FTP::GetPassword()const
{
	return password;
}

void FTP::SetIsPassive(bool p)
{
	isPassive = p;
}

bool FTP::GetIsPassive()const
{
	return isPassive;
}

void FTP::SetDir(const std::string& d)
{
	dir = d;
}

std::string FTP::GetDir()const
{
	return dir;
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"FTP");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Allows you to upload files to a FTP-server by dropping them onto the desklet.");
	*iVersion = 210;
}

FTP* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	FTP* ftp = new FTP(hwndDesklet,hInstanceDll);
	if(ini && group)
		ftp->ReadFromIni(ini,group);

	if(!ftp->GetSkinIsChanged() )
	{
		ftp->SetSkin(ftp->GetSkinDir(), ini && group );
	}

	FORMATETC f;
	f.cfFormat = CF_TEXT;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;
	DeskletAddDragDropFormat(hwndDesklet,&f);

	DeskletActivateExtendedDragDrop(hwndDesklet);

	return ftp;
}

void __stdcall OnDestroy(FTP* ftp, HWND hwndDesklet)
{
	delete ftp;
}

void __stdcall OnSave(FTP* ftp, const char* ini, const char* group, BOOL bIsForExport)
{
	ftp->SaveToIni(ini,group);
}


// same except for Other sheet
int __stdcall FTPConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool static init;

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init = true;

			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			FTP* ftp = (FTP*)GetProp(hDlg,"lpData");

			if(!ftp)return TRUE;

			SendDlgItemMessage(hDlg,IDC_PASSWORD,EM_SETPASSWORDCHAR,(WPARAM)'*',0);
			SetDlgItemText(hDlg,IDC_NAME,ftp->GetName().c_str() );
			SetDlgItemText(hDlg,IDC_HOST,ftp->GetHost().c_str() );
			SetDlgItemText(hDlg,IDC_USERNAME,ftp->GetUsername().c_str() );
			SetDlgItemText(hDlg,IDC_PASSWORD,ftp->GetPassword().c_str() );			
			SetDlgItemInt(hDlg,IDC_PORT,ftp->GetPort(),FALSE);
			SetDlgItemText(hDlg,IDC_LINKFORMAT,ftp->GetLinkFormat().c_str());
			SetDlgItemText(hDlg,IDC_DIR,ftp->GetDir().c_str() );
			CheckDlgButton(hDlg,IDC_PASSIVE,ftp->GetIsPassive() ? TRUE : FALSE);

			init = false;
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDSAVE:
				{
					FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
					if(ftp)
					{
						char ini[MAX_PATH];
						//GetDLLPath(ftp->GetDLL(),ini,sizeof(ini)/sizeof(ini[0]));
						WCHAR folder[MAX_PATH+1] = {0};
						UINT bufSize=MAX_PATH+1;
						DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH, folder, &bufSize);
						strcpy(ini, WStringToString(folder).c_str());
						strcat(ini,"\\FTPPresets\\");
						CreateDirectory(ini, NULL);
						char group[]="Preset";

						char buf[1024];

						GetDlgItemText(hDlg,IDC_NAME,buf,sizeof(buf)/sizeof(buf[0]) );

						strcat(ini,buf);
						strcat(ini,".ini");

						WritePrivateProfileString(group,"Name",buf,ini);

						GetDlgItemText(hDlg,IDC_HOST,buf,sizeof(buf)/sizeof(buf[0]) );
						char test[1024] = {0};
						strcpy(test,buf);
						test[6]=0;
						if(stricmp(test,"ftp:\\\\")==0 || stricmp(test,"ftp://")==0)
						{
							if(MessageBox(GetActiveWindow(),"You have included the prefix \'ftp:\\\\\' in the hosts address. This is not required and might stop the FTP desklet from working correctly.\nDo you want to remove this prefix?","FTP Desklet - Set Properties",MB_ICONEXCLAMATION | MB_YESNO)==IDYES)
							{
								init = true;

								memset(test,0,1024);
								strcpy(test,buf+6);
								SetDlgItemText(hDlg,IDC_HOST,test);
								WritePrivateProfileString(group,"Host",test,ini);
								

								init = false;
							}
							else
							{
								WritePrivateProfileString(group,"Host",buf,ini);
							}
						}
						else
							WritePrivateProfileString(group,"Host",buf,ini);

						GetDlgItemText(hDlg,IDC_USERNAME,buf,sizeof(buf)/sizeof(buf[0]) );
						WritePrivateProfileString(group,"Username",buf,ini);

						GetDlgItemText(hDlg,IDC_PASSWORD,buf,sizeof(buf)/sizeof(buf[0]) );
						WritePrivateProfileString(group,"PW",buf,ini);

						WritePrivateProfileInt(group,"Port",GetDlgItemInt(hDlg,IDC_PORT,0,FALSE) ,ini);

						WritePrivateProfileInt(group,"Passive",IsDlgButtonChecked(hDlg,IDC_PASSIVE) ? TRUE : FALSE ,ini);

						GetDlgItemText(hDlg,IDC_DIR,buf,sizeof(buf)/sizeof(buf[0]) );
						WritePrivateProfileString(group,"Dir",buf,ini);


						GetDlgItemText(hDlg,IDC_LINKFORMAT,buf,sizeof(buf)/sizeof(buf[0]) );
						WritePrivateProfileString(group,"LinkFormat",buf,ini);

					}
				}
				break;
				case IDC_PASSIVE:
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDC_NAME: case IDC_HOST: case IDC_USERNAME: case IDC_PASSWORD: case IDC_PORT: case IDC_DIR: case IDC_LINKFORMAT:
					if(HIWORD(wParam)==EN_UPDATE && !init)
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
					FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
					if(ftp)
					{
						char buf[1024];

						GetDlgItemText(hDlg,IDC_NAME,buf,sizeof(buf)/sizeof(buf[0]) );
						ftp->SetName(buf);

						GetDlgItemText(hDlg,IDC_HOST,buf,sizeof(buf)/sizeof(buf[0]) );
						char test[1024] = {0};
						strcpy(test,buf);
						test[6]=0;
						if(stricmp(test,"ftp:\\\\")==0 || stricmp(test,"ftp://")==0)
						{
							if(MessageBox(GetActiveWindow(),"You have included the prefix \'ftp:\\\\\' in the hosts address. This is not required and might stop the FTP desklet from working correctly.\nDo you want to remove this prefix?","FTP Desklet - Set Properties",MB_ICONEXCLAMATION | MB_YESNO)==IDYES)
							{
								init = true;

								memset(test,0,1024);
								strcpy(test,buf+6);
								SetDlgItemText(hDlg,IDC_HOST,test);
								ftp->SetHost(test);

								init = false;
							}
							else
							{
								ftp->SetHost(buf);
							}
						}
						else
							ftp->SetHost(buf);

						GetDlgItemText(hDlg,IDC_USERNAME,buf,sizeof(buf)/sizeof(buf[0]) );
						ftp->SetUsername(buf);

						GetDlgItemText(hDlg,IDC_PASSWORD,buf,sizeof(buf)/sizeof(buf[0]) );
						ftp->SetPassword(buf);

						ftp->SetPort(GetDlgItemInt(hDlg,IDC_PORT,0,FALSE) );

						ftp->SetIsPassive(IsDlgButtonChecked(hDlg,IDC_PASSIVE) == TRUE );

						GetDlgItemText(hDlg,IDC_DIR,buf,sizeof(buf)/sizeof(buf[0]) );
						ftp->SetDir(buf);

						GetDlgItemText(hDlg,IDC_LINKFORMAT,buf,sizeof(buf)/sizeof(buf[0]) );
						ftp->SetLinkFormat(buf);


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
			FTP* ftp = (FTP*)GetProp(hDlg,"lpData");

			if(!ftp)return TRUE;

			CheckDlgButton(hDlg,IDC_SHOWERROR,ftp->GetShowErrMsg() ? TRUE : FALSE);
			CheckDlgButton(hDlg,IDC_ZIP,ftp->GetShouldZip() ? TRUE : FALSE);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_SHOWERROR: case IDC_ZIP:
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;;
			}
		break;

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
					FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
					if(ftp)
					{
						ftp->SetShowErrMsg( IsDlgButtonChecked(hDlg,IDC_SHOWERROR) == BST_CHECKED );
						ftp->SetShouldZip( IsDlgButtonChecked(hDlg,IDC_ZIP) == BST_CHECKED );
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

	static char imgNormal[MAX_PATH];
	static char imgTransfer[MAX_PATH];
	static char imgUploading[MAX_PATH];
	static char imgError[MAX_PATH];

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
		
			if(!ftp)return TRUE;

			strcpy(imgNormal,ftp->GetNormalImage().c_str());
			strcpy(imgTransfer,ftp->GetTransferImage().c_str());
			strcpy(imgUploading,ftp->GetUploadingImage().c_str());
			strcpy(imgError,ftp->GetErrorImage().c_str());

			SetProp(GetDlgItem(hDlg,IDC_IMGNORMAL),"img",(HANDLE)imgNormal);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_IMGNORMAL),GWL_WNDPROC,(LONG)FrameSubclassProc));
			DragAcceptFiles(GetDlgItem(hDlg,IDC_IMGNORMAL),TRUE);

			SetProp(GetDlgItem(hDlg,IDC_IMGTRANSFER),"img",(HANDLE)imgTransfer);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_IMGTRANSFER),GWL_WNDPROC,(LONG)FrameSubclassProc));
			DragAcceptFiles(GetDlgItem(hDlg,IDC_IMGTRANSFER),TRUE);

			SetProp(GetDlgItem(hDlg,IDC_IMGUPLOADING),"img",(HANDLE)imgUploading);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_IMGUPLOADING),GWL_WNDPROC,(LONG)FrameSubclassProc));
			DragAcceptFiles(GetDlgItem(hDlg,IDC_IMGUPLOADING),TRUE);

			SetProp(GetDlgItem(hDlg,IDC_IMGERROR),"img",(HANDLE)imgError);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_IMGERROR),GWL_WNDPROC,(LONG)FrameSubclassProc));
			DragAcceptFiles(GetDlgItem(hDlg,IDC_IMGERROR),TRUE);

			init = false;
			InvalidateRect(hDlg,0,TRUE);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDSKINPICKERMODE:
				{
				FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
				if(ftp)
				{
					PROPSHEETPAGE psp = {0};
					psp.dwSize = sizeof(psp);
					psp.dwFlags = PSP_USETITLE;
					psp.hInstance = ftp->GetDLL();
					psp.pszTitle = "Skin";
					psp.pszTemplate = MAKEINTRESOURCE(IDD_SKIN);
					psp.pfnDlgProc = SkinConfigProc;
					psp.lParam = (LPARAM)ftp;

					ftp->hPageSkin = CreatePropertySheetPage(&psp);
					PropSheet_AddPage(GetParent(hDlg),ftp->hPageSkin);

					PropSheet_RemovePage(GetParent(hDlg),0,ftp->hPageImage);
					PropSheet_RemovePage(GetParent(hDlg),0,ftp->hPageProgess);
					PropSheet_SetCurSel(GetParent(hDlg),ftp->hPageSkin,0);

				}
				}
				break;

				case IDSETNORMAL:
				
					if(DeskletBrowseForImage(hDlg,imgNormal))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
				}
					
				break;

				case IDSETTRANSFER:
					if(DeskletBrowseForImage(hDlg,imgTransfer))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				break;

				case IDSETUPLOADING:
					if(DeskletBrowseForImage(hDlg,imgUploading))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				break;
				

				case IDSETERRORIMG:
					if(DeskletBrowseForImage(hDlg,imgError))
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

				HWND hwndEmpty = GetDlgItem(hDlg,IDC_IMGNORMAL);
				HWND hwndFull = GetDlgItem(hDlg,IDC_IMGTRANSFER);
				HWND hwndOverlay = GetDlgItem(hDlg,IDC_IMGUPLOADING);
				HWND hwndError = GetDlgItem(hDlg,IDC_IMGERROR);
				RECT r = {0};

				GetClientRect(hwndEmpty,&r);
				Bitmap  bufEmpty(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpEmpty = DeskletLoadGDIPlusImage(imgNormal);
				Graphics emptyG(&bufEmpty);
				emptyG.Clear(Color(255,255,255,255));
				emptyG.DrawImage(bmpEmpty,0,0,bufEmpty.GetWidth()-4,bufEmpty.GetWidth()-4);
				delete bmpEmpty;
				Graphics screenEmpty(hwndEmpty);
				screenEmpty.DrawImage(&bufEmpty,2,2);

				GetClientRect(hwndFull,&r);
				Bitmap  bufFull(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpFull = DeskletLoadGDIPlusImage(imgTransfer);
				Graphics FullG(&bufFull);
				FullG.Clear(Color(255,255,255,255));
				FullG.DrawImage(bmpFull,0,0,bufFull.GetWidth()-4,bufFull.GetWidth()-4);
				delete bmpFull;
				Graphics screenFull(hwndFull);
				screenFull.DrawImage(&bufFull,2,2);

				GetClientRect(hwndOverlay,&r);
				Bitmap  bufOverlay(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpOverlay = DeskletLoadGDIPlusImage(imgUploading);
				Graphics OverlayG(&bufOverlay);
				OverlayG.Clear(Color(255,255,255,255));
				OverlayG.DrawImage(bmpOverlay,0,0,bufOverlay.GetWidth()-4,bufOverlay.GetWidth()-4);
				delete bmpOverlay;
				Graphics screenOverlay(hwndOverlay);
				screenOverlay.DrawImage(&bufOverlay,2,2);

				GetClientRect(hwndError,&r);
				Bitmap  bufError(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpError = DeskletLoadGDIPlusImage(imgError);
				Graphics errorG(&bufError);
				errorG.Clear(Color(255,255,255,255));
				errorG.DrawImage(bmpError,0,0,bufError.GetWidth()-4,bufError.GetWidth()-4);
				delete bmpError;
				Graphics screenError(hwndError);
				screenError.DrawImage(&bufError,2,2);

				ReleaseDC(hDlg,dc);
				EndPaint(hDlg,&ps);
			}
		break;
	*/

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
				{
					FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
					if(ftp)
					{
						ftp->SetNormalImage(imgNormal);
						ftp->SetTransferImage(imgTransfer);
						ftp->SetUploadingImage(imgUploading);
						ftp->SetErrorImage(imgError);
						ftp->SetSkinIsChanged(true);
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
			RemoveProp(hDlg,"lpData");
			SetWindowLong(GetDlgItem(hDlg,IDC_IMGNORMAL),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_IMGNORMAL),"old_wndproc"));
			SetWindowLong(GetDlgItem(hDlg,IDC_IMGTRANSFER),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_IMGTRANSFER),"old_wndproc"));
			SetWindowLong(GetDlgItem(hDlg,IDC_IMGUPLOADING),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_IMGUPLOADING),"old_wndproc"));
			SetWindowLong(GetDlgItem(hDlg,IDC_IMGERROR),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_IMGERROR),"old_wndproc"));
		break;
	}
	return FALSE;
}


int __stdcall UploadZipDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init = true;

			InitCommonControls();

			ZIPUPLOADSTRUCT* zus = (ZIPUPLOADSTRUCT*)lParam;
			if(!zus)
				return FALSE;

			SetProp(hDlg,"lpData",(HANDLE)zus);

			int id = IDC_ZIPALL;
			if(TYPE_ZIP_OWN == zus->type)
				id = IDC_ZIPOWN;
			else if(TYPE_ZIP_NONE == zus->type)
				id = IDC_ZIPNONE;

			CheckRadioButton(hDlg,IDC_ZIPALL,IDC_ZIPNONE,id);

			SetDlgItemText(hDlg,IDC_FILENAME,zus->fileName.c_str());

			
			init = false;

			SetFocus(GetDlgItem(hDlg,IDOK));
		}
		break;


		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
				{
					ZIPUPLOADSTRUCT* zus = (ZIPUPLOADSTRUCT*)GetProp(hDlg,"lpData");
					if(zus)
					{
						char fileName[MAX_PATH] = {0};
						GetDlgItemText(hDlg,IDC_FILENAME,fileName,MAX_PATH);
						zus->fileName = fileName;

						if(IsDlgButtonChecked(hDlg,IDC_ZIPALL))
							zus->type = TYPE_ZIP_ALL;
						else if(IsDlgButtonChecked(hDlg,IDC_ZIPOWN))
							zus->type = TYPE_ZIP_OWN;
						else if(IsDlgButtonChecked(hDlg,IDC_ZIPNONE))
							zus->type = TYPE_ZIP_NONE;

						EnableWindow(GetDlgItem(hDlg,IDC_FILENAME),IsDlgButtonChecked(hDlg,IDC_ZIPALL));
					}
					EndDialog(hDlg,TRUE);
				}
				break;

				case IDCANCEL:
					EndDialog(hDlg,FALSE);
				break;

				case IDC_JPG: case IDC_PNG: case IDC_BMP:
					EnableWindow(GetDlgItem(hDlg,IDC_FILENAME),IsDlgButtonChecked(hDlg,IDC_ZIPALL));
				break;
			}
			break;
		break;

		case WM_CLOSE:
			EndDialog(hDlg,FALSE);
		break;
	}

	return 0;
}


int __stdcall UploadImageDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init = true;

			InitCommonControls();

			IMAGEUPLOADSTRUCT* ius = (IMAGEUPLOADSTRUCT*)lParam;
			if(!ius)
				return FALSE;

			SetProp(hDlg,"lpData",(HANDLE)ius);

			int id = IDC_PNG;
			if(TYPE_BMP == ius->type)
				id = IDC_BMP;
			else if(TYPE_JPG == ius->type)
				id = IDC_JPG;

			CheckRadioButton(hDlg,IDC_PNG,IDC_JPG,id);

			EnableWindow(GetDlgItem(hDlg,IDC_QUALITY),TYPE_JPG == ius->type);

			SetDlgItemText(hDlg,IDC_FILENAME,ius->fileName.c_str());

			std::stringstream ss;
			ss << ius->quality << "%";
			SetDlgItemText(hDlg,IDC_QUALITYTEXT,ss.str().c_str());
			SendDlgItemMessage(hDlg,IDC_QUALITY,TBM_SETRANGE,(WPARAM)FALSE,MAKELONG(0,100));
			SendDlgItemMessage(hDlg,IDC_QUALITY,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)ius->quality);

			CheckDlgButton(hDlg,IDC_THUMBNAIL,ius->generateThumbNail);

			if(ius->hBitmap)
			{
				Bitmap* bmp = Bitmap::FromHBITMAP(ius->hBitmap,NULL);
				if(bmp)
				{
					std::stringstream ss;
					ss << "Width: " << bmp->GetWidth() << " px\nHeight: " << bmp->GetHeight() << " px";
					SetDlgItemText(hDlg,IDC_SIZES,ss.str().c_str());
					
					RECT r = {0};
					GetWindowRect(GetDlgItem(hDlg,IDC_IMG),&r);
					int w = r.right - r.left;
					int h = r.bottom - r.top;

					Bitmap* bmpSmall = new Bitmap(w,h);
					Graphics g(bmpSmall);
					g.SetSmoothingMode(SmoothingModeAntiAlias);
					g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
					g.DrawImage(bmp,0,0,w,h);

					HBITMAP hBmp = {0};
					bmpSmall->GetHBITMAP(NULL,&hBmp);
					
					SendDlgItemMessage(hDlg,IDC_IMG,STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
					DeleteObject(hBmp);
					delete bmpSmall;

				}
				delete bmp;
			}


			init = false;

			SetFocus(GetDlgItem(hDlg,IDOK));
		}
		break;

		case WM_HSCROLL:
			if(!init)
			{
				std::stringstream ss;
				ss << SendDlgItemMessage(hDlg,IDC_QUALITY,TBM_GETPOS,0L,0L) << "%";
				SetDlgItemText(hDlg,IDC_QUALITYTEXT,ss.str().c_str());
			}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
				{
					IMAGEUPLOADSTRUCT* ius = (IMAGEUPLOADSTRUCT*)GetProp(hDlg,"lpData");
					if(ius)
					{
						char fileName[MAX_PATH] = {0};
						GetDlgItemText(hDlg,IDC_FILENAME,fileName,MAX_PATH);
						ius->fileName = fileName;
						ius->generateThumbNail = IsDlgButtonChecked(hDlg,IDC_THUMBNAIL);
						ius->quality = SendDlgItemMessage(hDlg,IDC_QUALITY,TBM_GETPOS,0L,0L);

						if(IsDlgButtonChecked(hDlg,IDC_PNG))
							ius->type = TYPE_PNG;
						else if(IsDlgButtonChecked(hDlg,IDC_BMP))
							ius->type = TYPE_BMP;
						else if(IsDlgButtonChecked(hDlg,IDC_JPG))
							ius->type = TYPE_JPG;
					}
					EndDialog(hDlg,TRUE);
				}
				break;

				case IDCANCEL:
					EndDialog(hDlg,FALSE);
				break;

				case IDC_JPG: case IDC_PNG: case IDC_BMP:
					EnableWindow(GetDlgItem(hDlg,IDC_QUALITY),IsDlgButtonChecked(hDlg,IDC_JPG));
				break;
			}
			break;
		break;

		case WM_CLOSE:
			EndDialog(hDlg,FALSE);
		break;

		case WM_DESTROY:
		{
			HBITMAP hBmp = (HBITMAP) SendDlgItemMessage(hDlg,IDC_IMG,STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, 0L);
			if(hBmp)
				DeleteObject(hBmp);
		}
		break;
	}

	return 0;
}

// same except for images sheet
int __stdcall ProgressConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	static char imgProgress[MAX_PATH];

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
		
			if(!ftp)return TRUE;

			{
				char tmp[1024] = {0};
				LoadString(ftp->GetDLL(),IDS_DIR1,tmp,sizeof(tmp)/sizeof(tmp[0]) );
				SendDlgItemMessage(hDlg,IDC_DIR,CB_ADDSTRING,0,(LPARAM)tmp);

				LoadString(ftp->GetDLL(),IDS_DIR2,tmp,sizeof(tmp)/sizeof(tmp[0]) );
				SendDlgItemMessage(hDlg,IDC_DIR,CB_ADDSTRING,0,(LPARAM)tmp);

				LoadString(ftp->GetDLL(),IDS_DIR3,tmp,sizeof(tmp)/sizeof(tmp[0]) );
				SendDlgItemMessage(hDlg,IDC_DIR,CB_ADDSTRING,0,(LPARAM)tmp);

				LoadString(ftp->GetDLL(),IDS_DIR4,tmp,sizeof(tmp)/sizeof(tmp[0]) );
				SendDlgItemMessage(hDlg,IDC_DIR,CB_ADDSTRING,0,(LPARAM)tmp);

				SendDlgItemMessage(hDlg,IDC_DIR,CB_SETCURSEL,ftp->GetProgressDir(),0);

			}

			strcpy(imgProgress,ftp->GetProgressImage().c_str());

			SetProp(GetDlgItem(hDlg,IDC_IMGPROGRESS),"img",(HANDLE)imgProgress);
			SetProp(hDlg,"old_wndproc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_IMGPROGRESS),GWL_WNDPROC,(LONG)FrameSubclassProc));
			DragAcceptFiles(GetDlgItem(hDlg,IDC_IMGPROGRESS),TRUE);


			RECT& r = ftp->GetProgressRect();
			SetDlgItemInt(hDlg,IDC_LEFT,r.left,TRUE);
			SetDlgItemInt(hDlg,IDC_RIGHT,r.right,TRUE);
			SetDlgItemInt(hDlg,IDC_TOP,r.top,TRUE);
			SetDlgItemInt(hDlg,IDC_BOTTOM,r.bottom,TRUE);

			init = false;
			InvalidateRect(hDlg,0,TRUE);
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDSKINPICKINGMODE2:
				{
				FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
				if(ftp)
				{
					PROPSHEETPAGE psp = {0};
					psp.dwSize = sizeof(psp);
					psp.dwFlags = PSP_USETITLE;
					psp.hInstance = ftp->GetDLL();
					psp.pszTitle = "Skin";
					psp.pszTemplate = MAKEINTRESOURCE(IDD_SKIN);
					psp.pfnDlgProc = SkinConfigProc;
					psp.lParam = (LPARAM)ftp;

					ftp->hPageSkin = CreatePropertySheetPage(&psp);
					PropSheet_AddPage(GetParent(hDlg),ftp->hPageSkin);

					PropSheet_RemovePage(GetParent(hDlg),0,ftp->hPageImage);
					PropSheet_RemovePage(GetParent(hDlg),0,ftp->hPageProgess);
					PropSheet_SetCurSel(GetParent(hDlg),ftp->hPageSkin,0);

				}

				}
				break;

				case IDC_LEFT: case IDC_RIGHT: case IDC_BOTTOM: case IDC_TOP:
					if(HIWORD(wParam)==EN_UPDATE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDC_DIR:
					if(HIWORD(wParam)==CBN_SELCHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;


				case IDSETPROGRESS:
				
					if(DeskletBrowseForImage(hDlg,imgProgress))
					{
						InvalidateRect(hDlg,0,TRUE);
						PropSheet_Changed(GetParent(hDlg),hDlg);
				}	
				break;

				case IDPREVIEW:
				{
					FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
					if(ftp)
					{
						if(ftp->IsUploading())
							break;

						BOOL success(FALSE);
						UINT id = DeskletLayerGetLayer(ftp->GetWindow(),"FTP.ProgressBar",&success);
						if(!success)
						{
							id = DeskletLayerAdd(ftp->GetWindow(),"FTP.ProgressBar");
						}

						ftp->SetProgressDir(SendDlgItemMessage(hDlg,IDC_DIR,CB_GETCURSEL,0,0));

						Bitmap* bmp = DeskletLoadGDIPlusImage(imgProgress);
						SIZE s = {0};
						DeskletGetSize(ftp->GetWindow(),&s);
						Bitmap* tmp = new Bitmap(s.cx,s.cy);
						Graphics g(tmp);
						g.SetSmoothingMode(/*SmoothingModeAntiAlias8x8*/SmoothingModeAntiAlias);
						g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

						RECT r = {0};
						r.bottom	= GetDlgItemInt(hDlg,IDC_BOTTOM,0,TRUE);
						r.left		= GetDlgItemInt(hDlg,IDC_LEFT,0,TRUE);
						r.right		= GetDlgItemInt(hDlg,IDC_RIGHT,0,TRUE);
						r.top		= GetDlgItemInt(hDlg,IDC_TOP,0,TRUE);

						g.DrawImage(bmp,(int)((float)r.left/192 *s.cx),(int)((float)r.top/192 * s.cy),(int)((float)(r.right - r.left)/192 * s.cx),(int)((float)(r.bottom - r.top)/192 * s.cy));

						delete bmp;
						DeskletLayerSetVisibility(ftp->GetWindow(),id,FALSE,TRUE);
						DeskletLayerSetImage(ftp->GetWindow(),id,TRUE,tmp,TRUE);
					}
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

				HWND hwndEmpty = GetDlgItem(hDlg,IDC_IMGPROGRESS);
				RECT r = {0};

				GetClientRect(hwndEmpty,&r);
				Bitmap  bufEmpty(r.right - r.left-4, r.bottom - r.top-4);
				Bitmap* bmpEmpty = DeskletLoadGDIPlusImage(imgProgress);
				Graphics emptyG(&bufEmpty);
				emptyG.Clear(Color(255,255,255,255));
				emptyG.DrawImage(bmpEmpty,0,0,bufEmpty.GetWidth()-4,bufEmpty.GetWidth()-4);
				delete bmpEmpty;
				Graphics screenEmpty(hwndEmpty);
				screenEmpty.DrawImage(&bufEmpty,2,2);

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
					FTP* ftp = (FTP*)GetProp(hDlg,"lpData");
					if(ftp)
					{
						ftp->SetProgressDir(SendDlgItemMessage(hDlg,IDC_DIR,CB_GETCURSEL,0,0));
						ftp->SetProgressImage(imgProgress);
						RECT r = {0};
						r.bottom	= GetDlgItemInt(hDlg,IDC_BOTTOM,0,TRUE);
						r.left		= GetDlgItemInt(hDlg,IDC_LEFT,0,TRUE);
						r.right		= GetDlgItemInt(hDlg,IDC_RIGHT,0,TRUE);
						r.top		= GetDlgItemInt(hDlg,IDC_TOP,0,TRUE);
						ftp->SetProgressRect(r);
						ftp->SetSkinIsChanged(true);
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
			FTP* ftp = (FTP*) RemoveProp(hDlg,"lpData");
			if(ftp)
			{
				if(!ftp->IsUploading())
				{
					BOOL success(FALSE);
					int id = DeskletLayerGetLayer(ftp->GetWindow(),"FTP.ProgressBar",&success);
					if(success)
						DeskletLayerSetVisibility(ftp->GetWindow(),id,TRUE,FALSE);
				}
			}
			SetWindowLong(GetDlgItem(hDlg,IDC_IMGPROGRESS),GWL_WNDPROC,(LONG)RemoveProp(GetDlgItem(hDlg,IDC_IMGPROGRESS),"old_wndproc"));
		break;
	}
	return FALSE;
}

int __stdcall SkinConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	FTP* id = (FTP*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			id = (FTP*)GetProp(hDlg,"lpData");
			if(!id)return TRUE;
			
			{
				char path[MAX_PATH] = {0};
				GetDLLPath(id->GetDLL(),path,MAX_PATH);
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

				SetDlgItemText(hDlg,IDC_NAME2,id->GetSkinName());
				SetDlgItemText(hDlg,IDC_AUTHOR,id->GetSkinAuthor());
				SetDlgItemText(hDlg,IDC_INFO,id->GetSkinInfo());
			}

			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_SKIN:
					if(HIWORD(wParam)==CBN_SELCHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDADVANCEDMODE:
				if(id)
				{
					PROPSHEETPAGE psp = {0};
					psp.dwSize = sizeof(psp);
					psp.dwFlags = PSP_USETITLE;
					psp.hInstance = id->GetDLL();
					psp.pszTitle = "Images";
					psp.pszTemplate = MAKEINTRESOURCE(IDD_IMAGES);
					psp.pfnDlgProc = ImagesConfigProc;
					psp.lParam = (LPARAM)id;
					id->hPageImage = CreatePropertySheetPage(&psp);
					PropSheet_AddPage(GetParent(hDlg),id->hPageImage);

					psp.dwSize = sizeof(psp);
					psp.dwFlags = PSP_USETITLE;
					psp.hInstance = id->GetDLL();
					psp.pszTitle = "Progress";
					psp.pszTemplate = MAKEINTRESOURCE(IDD_PROGRESS);
					psp.pfnDlgProc = ProgressConfigProc;
					psp.lParam = (LPARAM)id;
					id->hPageProgess = CreatePropertySheetPage(&psp);
					PropSheet_AddPage(GetParent(hDlg),id->hPageProgess);

					PropSheet_RemovePage(GetParent(hDlg),0,id->hPageSkin);
					PropSheet_SetCurSel(GetParent(hDlg),id->hPageImage,0);
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
					if(id)
					{
						char path[MAX_PATH] = {0};
						GetDlgItemText(hDlg,IDC_SKIN,path,MAX_PATH);
						id->SetSkin(path);

						SetDlgItemText(hDlg,IDC_NAME2,id->GetSkinName());
						SetDlgItemText(hDlg,IDC_AUTHOR,id->GetSkinAuthor());
						SetDlgItemText(hDlg,IDC_INFO,id->GetSkinInfo());

						id->SetSkinIsChanged(false);
					}
				break;
			}
		break;
	}
	return FALSE;
}

INT __stdcall OnAddPropertySheets(FTP* ftp, int numPresent)
{
	PROPSHEETPAGE psp = {0};
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = ftp->GetDLL();
	psp.pszTitle = "FTP Settings";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_FTP);
	psp.pfnDlgProc = FTPConfigProc;
	psp.lParam = (LPARAM)ftp;

	int res = DeskletAddPropertySheet(ftp->GetWindow(),CreatePropertySheetPage(&psp));

	if(ftp->GetSkinIsChanged() )
	{
		psp.dwSize = sizeof(psp);
		psp.dwFlags = PSP_USETITLE;
		psp.hInstance = ftp->GetDLL();
		psp.pszTitle = "Images";
		psp.pszTemplate = MAKEINTRESOURCE(IDD_IMAGES);
		psp.pfnDlgProc = ImagesConfigProc;
		psp.lParam = (LPARAM)ftp;

		res	= DeskletAddPropertySheet(ftp->GetWindow(),ftp->hPageImage = CreatePropertySheetPage(&psp));

		psp.dwSize = sizeof(psp);
		psp.dwFlags = PSP_USETITLE;
		psp.hInstance = ftp->GetDLL();
		psp.pszTitle = "Progress";
		psp.pszTemplate = MAKEINTRESOURCE(IDD_PROGRESS);
		psp.pfnDlgProc = ProgressConfigProc;
		psp.lParam = (LPARAM)ftp;

		res = DeskletAddPropertySheet(ftp->GetWindow(),ftp->hPageProgess = CreatePropertySheetPage(&psp));
	}
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = ftp->GetDLL();
	psp.pszTitle = "Other Options";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_OTHER);
	psp.pfnDlgProc = OtherConfigProc;
	psp.lParam = (LPARAM)ftp;

	
	res = DeskletAddPropertySheet(ftp->GetWindow(),CreatePropertySheetPage(&psp));

	if(!ftp->GetSkinIsChanged() )
	{
		psp.dwSize = sizeof(psp);
		psp.dwFlags = PSP_USETITLE;
		psp.hInstance = ftp->GetDLL();
		psp.pszTitle = "Skin";
		psp.pszTemplate = MAKEINTRESOURCE(IDD_SKIN);
		psp.pfnDlgProc = SkinConfigProc;
		psp.lParam = (LPARAM)ftp;

		res =  DeskletAddPropertySheet(ftp->GetWindow(),ftp->hPageSkin=CreatePropertySheetPage(&psp));
	}

	return res;
}

BOOL CALLBACK OnDragEnter(FTP* ftp,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(IsDataObjectHDROP(obj))
	{
		ftp->SetCanAccept(true);
		DeskletSetImageFile(ftp->GetWindow(),const_cast<char*>(ftp->GetTransferImage().c_str() ) );
	}
	else
	{
		ftp->SetCanAccept(false);	
	}
	return TRUE;
}

BOOL CALLBACK OnDragLeave(FTP* ftp)
{		
	ftp->SetCanAccept(false);
	if(!ftp->IsUploading())
		DeskletSetImageFile(ftp->GetWindow(),const_cast<char*>(ftp->GetNormalImage().c_str() ) );
	else
		DeskletSetImageFile(ftp->GetWindow(),const_cast<char*>(ftp->GetUploadingImage().c_str() ) );
	return TRUE;
}

BOOL CALLBACK OnDrop(FTP* ftp,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	/*
	FORMATETC f;
	f.cfFormat = CF_TEXT;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;

	if(SUCCEEDED(obj->QueryGetData(&etc)) )
	{
		if(MessageBox(ftp->GetWindow(),"You have dropped some text. Do you want to upload this too?","Ftp drop",MB_SYSTEMMODAL | MB_YESNO ) == IDYES)
		{
			STGMEDIUM medium = {0};
			if( SUCCEEDED( obj->GetData(&etc,&medium) )
			{
				ftp->AddFile("s"
			}
		}
	}*/

	if(IsDataObjectHDROP(obj) )
	{
			DropStruct* ds = LockHDROP(obj);
	
			int num = DragQueryFile(ds->hDrop, -1, 0, 0);

			/*
			char buf[MAX_PATH];

			for(int i=0;i<num;++i)
			{
				DragQueryFile(ds->hDrop,i,buf,MAX_PATH);
		
				char fileName[MAX_PATH] = {0};
				GetTempPath(MAX_PATH,fileName);
				PathAppend(fileName,"test.zip");

				CZipper zipper;
				zipper.OpenZip(fileName);

				if(IsDirectory(buf))
					zipper.AddFolderToZip(buf,TRUE);
				else
					zipper.AddFileToZip(buf,TRUE);

				zipper.CloseZip();

				std::string upFileName("*");
				upFileName += fileName;
				ftp->AddFile(upFileName);
				//ftp->AddFile(buf);
			}
			*/

			std::vector<std::string> files;
			for(int i=0;i<num;++i)
			{
				char buf[MAX_PATH] = {0};
				DragQueryFile(ds->hDrop,i,buf,MAX_PATH);
				files.push_back(buf);
			}

			UnlockHDROP(ds);

			ftp->UploadMultipleFiles(files);

	}
	return TRUE;
}

BOOL __stdcall OnDragDropDataRequest(FTP* ftp,FORMATETC* format,STGMEDIUM* medium)
{
	if(format->cfFormat == CF_TEXT)
	{

		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,ftp->GetLinks().size()+1);
		char* memCpy = (char*)GlobalLock(hMem);

		memcpy((void*)memCpy,(void*)ftp->GetLinks().c_str(),ftp->GetLinks().size()+1);
		GlobalUnlock(hMem);

		medium->hGlobal = hMem;
		medium->pUnkForRelease = 0;
		medium->tymed = TYMED_HGLOBAL;
		return TRUE;
	}
	return FALSE;
}

void SaveFileFromImageUploadStruct(FTP* ftp, IMAGEUPLOADSTRUCT* ius, Bitmap* bmp)
{
	char fileName[MAX_PATH] = {0};
	GetTempPath(MAX_PATH,fileName);
	PathAppend(fileName,ius->fileName.c_str());

	if(strstr(ius->fileName.c_str(),".") == 0)
	{
		const char* ext = NULL;

		if(ius->type == TYPE_PNG)
			ext = ".png";
		else if(ius->type == TYPE_BMP)
			ext = ".bmp";
		else if(ius->type == TYPE_JPG)
			ext =".jpg";

		if(ext != NULL)
		strcat(fileName,ext);
	}

	WCHAR wFileName[MAX_PATH] = {0};
	MultiByteToWideChar(GetACP(),0,fileName,-1,wFileName,MAX_PATH);

	if(bmp)
	{
		if(ius->type == TYPE_PNG)
		{
			CLSID encoderClsid;
			GetEncoderClsid(L"image/png", &encoderClsid);
			bmp->Save(wFileName,&encoderClsid,NULL);
		}
		else if(ius->type == TYPE_BMP)
		{
			CLSID encoderClsid;
			GetEncoderClsid(L"image/bmp", &encoderClsid);
			bmp->Save(wFileName,&encoderClsid,NULL);
		}
		else
		{
			EncoderParameters encoderParameters;
			CLSID encoderClsid;
			GetEncoderClsid(L"image/jpeg", &encoderClsid);

			encoderParameters.Count = 1;
			encoderParameters.Parameter[0].Guid = EncoderQuality;
			encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
			encoderParameters.Parameter[0].NumberOfValues = 1;
			encoderParameters.Parameter[0].Value = &ius->quality;

			bmp->Save(wFileName, &encoderClsid, &encoderParameters);
		}

		std::string upFileName("*");
		upFileName += fileName;
		ftp->AddFile(upFileName);
	}
}

BOOL __stdcall OnRightButtonClick(FTP* ftp, POINT* p, SIZE *s)
{
	std::vector<std::string> links;
	std::string l = ftp->GetLinks();

	if(l.size() > 0)
	{
		std::string::iterator iter;
		do
		{
			std::string tmp;
			iter = std::find(l.begin(),l.end(),'\r');
			std::copy(l.begin(),iter,std::insert_iterator<std::string>(tmp,tmp.begin() ));
			links.push_back(tmp);
			if(iter!=l.end())
				l = l.substr(tmp.size()+2,l.size()-tmp.size()-1);
			else
				l="";
		}while(l.size() > 0);
	}

	HMENU menu = DeskletGetDefaultMenu(ftp->GetWindow(),10000);


	const UPLOAD_IMAGE_MENU_ITEM = 9999;
	const UPLOAD_FILES_FROM_CLIPBOARD = 9998;
	const UPLOAD_FILES_FROM_CLIPBOARD_ZIPPED = 9997;
	const STOP_UPLOADING = 9996;
	const UPLOAD_FILES = 9995;
	const UPLOAD_FILES_ZIPPED = 9994;


	int menuPos(0);

	if(ftp->IsUploading())
		InsertMenu(menu,menuPos++,MF_ENABLED | MF_STRING | MF_BYPOSITION,STOP_UPLOADING,"Stop Uploading");


	OpenClipboard(ftp->GetWindow() );

	if(IsClipboardFormatAvailable(CF_HDROP))
	{
		InsertMenu(menu,menuPos++,MF_ENABLED | MF_STRING | MF_BYPOSITION,UPLOAD_FILES_FROM_CLIPBOARD,"Upload Files In Clipboard");
		if(!ftp->GetShouldZip())
			InsertMenu(menu,menuPos++,MF_ENABLED | MF_STRING | MF_BYPOSITION,UPLOAD_FILES_FROM_CLIPBOARD_ZIPPED,"Zip-and-Upload Files In Clipboard");
	}
	else
	{
		InsertMenu(menu,menuPos++,MF_ENABLED | MF_STRING | MF_BYPOSITION,UPLOAD_FILES,"Upload Files...");
		if(!ftp->GetShouldZip())
			InsertMenu(menu,menuPos++,MF_ENABLED | MF_STRING | MF_BYPOSITION,UPLOAD_FILES_ZIPPED,"Zip-and-Upload Files...");
	}


	if( IsClipboardFormatAvailable(CF_BITMAP) )
	{
		InsertMenu(menu,menuPos++,MF_STRING | MF_BYPOSITION | MF_ENABLED,UPLOAD_IMAGE_MENU_ITEM,"Upload Image on Clipboard...");	
	}

	CloseClipboard();


	//AppendMenu(menu,MF_STRING | MF_SEPARATOR,0,0);

	HMENU subMenu = CreatePopupMenu();

	for(int i=0;i<links.size();i++)
		AppendMenu(subMenu,MF_STRING | MF_ENABLED,i+1,links[i].c_str() );

	if(links.size() == 0)
		AppendMenu(subMenu,MF_STRING | MF_DISABLED,0,"");

	//if(links.size() > 0)
		InsertMenu(menu,menuPos++,MF_POPUP | MF_BYPOSITION,(UINT)subMenu,"Links");
	//AppendMenu(menu,MF_POPUP | MF_BYPOSITION,(UINT)subMenu,"Links");

	//DestroyMenu(subMenu);

	char path[MAX_PATH] = {0};
	//GetDLLPath(ftp->GetDLL(),path,MAX_PATH);
	
	WCHAR folder[MAX_PATH+1] = {0};
						UINT bufSize=MAX_PATH+1;
	DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH, folder, &bufSize);
	strcpy(path, WStringToString(folder).c_str());
	strcat(path,"\\ftppresets\\*.ini");

	WIN32_FIND_DATA data = {0};

	std::vector<std::string>presets;

	HANDLE h = FindFirstFile(path,&data);
	if(h)
	{
		HMENU hSubMenu = CreatePopupMenu();

		int num = 30000;

		do
		{
			std::string s = data.cFileName;
			AppendMenu(hSubMenu,MF_STRING | MF_ENABLED,num,data.cFileName);
			presets.push_back(s);
			num++;

		}while(FindNextFile(h,&data));

		if(presets.size() > 0)
		{
			//AppendMenu(menu,MF_POPUP,(UINT)hSubMenu,"Presets");
			InsertMenu(menu,menuPos++,MF_POPUP | MF_BYPOSITION	,(UINT)hSubMenu,"Presets");
		}

		DestroyMenu(hSubMenu);

		FindClose(h);
	}


	if(menuPos>0)
		InsertMenu(menu,menuPos++,MF_SEPARATOR | MF_BYPOSITION,0,0);


	POINT pCur;
	GetCursorPos(&pCur);
	int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pCur.x, pCur.y, 0, ftp->GetWindow(),0);

	// We can now destroy the menu as it is no longer needed
	DestroyMenu(menu);

	if(STOP_UPLOADING == cmd)
	{
		ftp->StopUploading();
	}
	if(cmd == UPLOAD_IMAGE_MENU_ITEM)
	{
		OpenClipboard(ftp->GetWindow());

		HBITMAP clipBmp = static_cast<HBITMAP> (GetClipboardData(CF_BITMAP));
		HBITMAP hBmp = {0};
		if(clipBmp)
		hBmp = reinterpret_cast<HBITMAP>(CopyImage(reinterpret_cast<HANDLE>(clipBmp),IMAGE_BITMAP,0,0,LR_COPYRETURNORG));
		CloseClipboard();

		if(hBmp)
		{	
			IMAGEUPLOADSTRUCT ius = ftp->GetImageUploadStruct();
			ius.hBitmap = hBmp;
			if(GetKeyState(VK_SHIFT) < 0 || DialogBoxParam(ftp->GetDLL(),MAKEINTRESOURCE(IDD_UPLOADBITMAP),ftp->GetWindow(),UploadImageDlgProc,(LPARAM)&ius) == TRUE)
			{	
				ftp->SetImageUploadStruct(ius);
				Bitmap* bmp = Bitmap::FromHBITMAP(ius.hBitmap, NULL);
				if(bmp)
				{
					SaveFileFromImageUploadStruct(ftp,&ius,bmp);


					if(ius.generateThumbNail)
					{
						ius.fileName += "_thumbnail";

						int w = float(bmp->GetWidth())*0.25f;
						int h = float(bmp->GetHeight())*0.25f;
	
						Bitmap* bmpSmall = new Bitmap(w,h);
						Graphics g(bmpSmall);
						g.SetSmoothingMode(SmoothingModeAntiAlias);
						g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
						g.DrawImage(bmp,0,0,w,h);
						SaveFileFromImageUploadStruct(ftp,&ius,bmpSmall);

						delete bmpSmall;
					}
				}
				delete bmp;

				if(!ftp->IsUploading())
					ftp->StartUploading();
			}	
		}


		DeleteObject(hBmp);

	}
	else if(UPLOAD_FILES == cmd)
	{
			std::vector<std::string> files;
			if(OpenFileNameMultipleFiles(files,ftp->GetWindow(),ftp->GetDLL()))
			{
				ftp->UploadMultipleFiles(files);
			}
	}
	else if(UPLOAD_FILES_ZIPPED == cmd)
	{
			std::vector<std::string> files;
			if(OpenFileNameMultipleFiles(files,ftp->GetWindow(), ftp->GetDLL()))
			{
				ftp->UploadMultipleFiles(files, true);
			}
	}
	else if(UPLOAD_FILES_FROM_CLIPBOARD == cmd)
	{
		 if(OpenClipboard(ftp->GetWindow()))
		 {
			 HANDLE h = GetClipboardData(CF_HDROP);
			 if(h != NULL)
			 {
				 std::vector<std::string> files;
				HDROP hDrop = reinterpret_cast<HDROP>(h);
				int count = DragQueryFile(hDrop,(UINT)-1, NULL, 0);
				for(int i=0; i< count;++i)
				{
					char path[MAX_PATH] = {0};
					DragQueryFile(hDrop,i,path,MAX_PATH);
					files.push_back(path);
				}

				ftp->UploadMultipleFiles(files);
				//DragFinish(hDrop);
			 }
		 }
		

		 CloseClipboard();
	}
	else if(UPLOAD_FILES_FROM_CLIPBOARD_ZIPPED == cmd)
	{
		 if(OpenClipboard(ftp->GetWindow()))
		 {
			 HANDLE h = GetClipboardData(CF_HDROP);
			 if(h != NULL)
			 {
				 std::vector<std::string> files;
				HDROP hDrop = reinterpret_cast<HDROP>(h);
				int count = DragQueryFile(hDrop,(UINT)-1, NULL, 0);
				for(int i=0; i< count;++i)
				{
					char path[MAX_PATH] = {0};
					DragQueryFile(hDrop,i,path,MAX_PATH);
					files.push_back(path);
				}
				ftp->UploadMultipleFiles(files,true);
				//DragFinish(hDrop);
			 }
		 }
		

		 CloseClipboard();
	}
	else if( cmd >= 30000)
	{
		if(cmd < 30000 + presets.size())
		{
			char ini[MAX_PATH] = {0};
			//GetDLLPath(ftp->GetDLL(),ini,MAX_PATH);
			WCHAR folder[MAX_PATH+1] = {0};
			UINT bufSize=MAX_PATH+1;
			DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH, folder, &bufSize);
			strcpy(ini, WStringToString(folder).c_str());

			strcat(ini,"\\ftppresets\\");
			strcat(ini,presets[cmd-30000].c_str());
			//MessageBox(0,ini,0,0);

			char group[] = "Preset";
			char buf[1024];

			GetPrivateProfileString(group,"Name","",buf,1024,ini);
			ftp->SetName(buf);

			GetPrivateProfileString(group,"Host","",buf,1024,ini);
			ftp->SetHost(buf);

			ftp->SetPort( GetPrivateProfileInt(group,"Port",21,ini) );

			GetPrivateProfileString(group,"Username","",buf,1024,ini);
			ftp->SetUsername(buf);

			GetPrivateProfileString(group,"PW","",buf,1024,ini);
			ftp->SetPassword(buf);
		
			ftp->SetIsPassive( GetPrivateProfileInt(group,"Passive",TRUE,ini) == TRUE);
		
			GetPrivateProfileString(group,"Dir","",buf,1024,ini);
			ftp->SetDir(buf);

			GetPrivateProfileString(group,"LinkFormat","",buf,1024,ini);
			ftp->SetLinkFormat(buf);
		}
	}
	else if(cmd >= 10000)
	{
		// The user performed an action of the default menu (because the ID of the menu-item
		// is greater than 20,000 ), so we let AveDesk handle the action.
		DeskletDoDefaultMenuCommand(ftp->GetWindow(),10000,cmd);
	}
	else
	{
		if(cmd>0 && cmd <= links.size())
		{
			OpenClipboard(ftp->GetWindow());
			EmptyClipboard();

			std::string l = links[cmd-1];

			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,l.size()+1);
			char* memCpy = (char*)GlobalLock(hMem);

			memcpy((void*)memCpy,(void*)l.c_str(),l.size()+1);
			GlobalUnlock(hMem);

			SetClipboardData(CF_TEXT,hMem);

			CloseClipboard();
		}
	}

	return TRUE;
}

BOOL OpenFileNameMultipleFiles(std::vector<std::string>& files, HWND hwnd, HINSTANCE hInstance)
{
	char buffer[MAX_PATH*1024] = {0};
	char fileTitle[MAX_PATH] = {0};
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = hInstance;
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "All Files(*.*)\0*.*\0\0";
	ofn.lpstrFile  = buffer;
	ofn.nMaxFile = sizeof(buffer)/sizeof(buffer[0]);
	ofn.lpstrFileTitle = fileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	if( GetOpenFileName(&ofn) )
	{
		char dir[MAX_PATH] = {0};
		strncpy(dir,buffer,ofn.nFileOffset);
		dir[ofn.nFileOffset] = '\0';
		char* curPos = &buffer[0];
		curPos += ofn.nFileOffset;
		while( *curPos )
		{
			char file[MAX_PATH*2] = {0};
			strcpy(file,dir);
			PathAddBackslash(file);
			PathAppend(file,curPos);
			//ftp->AddFile(file);
			files.push_back(file);
			curPos += strlen(curPos)+1;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL __stdcall OnLeftButtonClick(FTP* ftp, POINT* p, SIZE *s)
{
	std::vector<std::string> files;
	if(OpenFileNameMultipleFiles(files, ftp->GetWindow(), ftp->GetDLL()))
	{
		ftp->UploadMultipleFiles(files);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
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

FTP* __stdcall OnCreateFromSkinW(HWND hwnd,HINSTANCE hInstance, CSkinEnumerator* se, const SkinInfo* info)
{

	FTP* desklet = OnCreate(hwnd,hInstance,NULL,NULL);


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
