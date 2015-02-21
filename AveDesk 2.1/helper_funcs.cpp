// filename:		helper_funcs.cpp
// Description:		Implementation  of commonly used functions in helper_funcs.h
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!
//
//

#include "stdafx.h"
#include "constants.h"
#include "helper_funcs.h"
#include <windows.h>
#include <math.h> //cmath not defined in vc6


#define BUFF_SIZE   1024
BOOL IsAdmin()
{
    HANDLE          hToken = NULL;
    PSID            pAdminSid = NULL;
    BYTE            buffer[BUFF_SIZE];
    PTOKEN_GROUPS   pGroups = (PTOKEN_GROUPS)buffer; 
    DWORD           dwSize;             // buffer size
    DWORD           i;
    BOOL            bSuccess;
    SID_IDENTIFIER_AUTHORITY siaNtAuth = SECURITY_NT_AUTHORITY;

    // get token handle
    if ( !OpenProcessToken ( GetCurrentProcess ( ), TOKEN_QUERY, &hToken ) )
        return false;
    
    bSuccess = GetTokenInformation ( hToken, TokenGroups, 
                                    (LPVOID)pGroups, BUFF_SIZE, 
                                    &dwSize );
    CloseHandle ( hToken );
    if ( !bSuccess )
        return false;
    if ( !AllocateAndInitializeSid ( &siaNtAuth, 2, 
                                     SECURITY_BUILTIN_DOMAIN_RID,
                                     DOMAIN_ALIAS_RID_ADMINS,
                                     0,0,0,0,0,0, &pAdminSid ) )
        return false;

    bSuccess = FALSE;
    for ( i = 0; (i < pGroups->GroupCount) && !bSuccess; i++ )
    {
        if ( EqualSid ( pAdminSid, pGroups->Groups[i].Sid ) )
            bSuccess = TRUE;
    }
    FreeSid ( pAdminSid );
    
    return bSuccess;
}


void DeleteDirectory(const std::wstring& path)
{

	WCHAR ws[MAX_PATH] = {0};
	wcscpy(ws,path.c_str());
	PathAppendW(ws,L"\\*.*");
	
	WCHAR wPath[MAX_PATH];
	wcscpy(wPath,path.c_str());
	
	WIN32_FIND_DATAW data = {0};	
	HANDLE h = FindFirstFile(ws,&data);
	if(INVALID_HANDLE_VALUE != h)
	{
		do
		{
			if(wcscmp(data.cFileName,L".") != 0 && wcscmp(data.cFileName,L"..") != 0)
			{
				WCHAR file[MAX_PATH] = {0};
				wcscpy(file,path.c_str());
				PathAppendW(file,data.cFileName);

				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					DeleteDirectory(file);
				else
					DeleteFile(file);
			}
		}while(FindNextFile(h,&data));
	}
	FindClose(h);
	RemoveDirectoryW(wPath);
}

std::wstring CreateGUIDString()
{
	std::wstring ret;

	LPOLESTR wszCLSID = NULL;
	GUID guid = {0};
	CoCreateGuid(&guid);
	StringFromCLSID(guid, &wszCLSID);
	int nLen = lstrlenW(wszCLSID);

	ret = wszCLSID;
	CoTaskMemFree(wszCLSID);

	return ret;
}

BOOL GetIsRunningWin2k()
{
	OSVERSIONINFO version = {0};
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&version);

	return (version.dwMajorVersion == 5 && version.dwMinorVersion < 1);
}

BOOL GetIsRunningVista()
{
	OSVERSIONINFO version = {0};
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&version);

	return (version.dwMajorVersion == 6);
}

//--------------------------------------------------------------------------------------------
int WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int iValue, LPCTSTR lpFileName)
{
	// post: the value of lpKeyName in section lpAppName and in INI-file lpFileName has
	//			been returned

	/*
	char szNumber[100];

	strcpy(szNumber, "");
	itoa(iValue, szNumber, 10);

	return WritePrivateProfileString(lpAppName, lpKeyName, szNumber, lpFileName);
	*/
	return 0;
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
void GetFileNameW(const WCHAR* filepath, WCHAR* buffer, int buffersize)
{
	// pre:  filepath and buffer are not 0, buffersize is the maximum size of chars copied
	// post: the filename in filepath is stored in buffer, this means that from filepath
	//			c:\blabla\yada.png buffer is filled with yada.png

	if(!filepath || !buffer)return;

	int i=wcslen(filepath)-1;
	while( i >= 0 && buffersize-- >0 && filepath[i]!='\\' && filepath[i]!='/')i--;
	wcscpy(buffer,filepath+i+1);
}
//--------------------------------------------------------------------------------------------
char* lowercase(char* str)
{
	// pre:  str is not 0
	// post: str has been converted to an all-lowercase-characters string and str
	//			has been returned again

	if(!str)return 0;

	for(unsigned int i=0;i<strlen(str);i++)
		str[i] = static_cast<char>(tolower(str[i]));

	return str;
}
WCHAR* lowercaseW(WCHAR* str)
{
	// pre:  str is not 0
	// post: str has been converted to an all-lowercase-characters string and str
	//			has been returned again

	if(!str)return 0;

	for(unsigned int i=0;i<wcslen(str);i++)
		str[i] = static_cast<WCHAR>(tolower(str[i]));

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
	WIN32_FIND_DATAA d;
	ZeroMemory(&d,sizeof(d));
	HANDLE h=FindFirstFileA(file,&d);

	// don't forget to close the handle, otherwise we have a mem-leak
	FindClose(h);

	// if the filename retrieved from file and the filename retrieved
	// by getting the WIN32_FIND_DATA are the same when compared (not case-sensitive)
	// then this file DOES exist, otherwise it DOESN'T exist
	return strcmp(lowercase(tmp),lowercase(d.cFileName))==0;
}
bool FileExistsW(const WCHAR* file)
{
	// pre:  file is not 0 and file should be an absolute path
	// post: true has been returned if the file named by file does exist, otherwise false
	//			has been returned

	if(!file)return false;

	// get the filename of the file and store it in tmp
	WCHAR tmp[MAX_PATH] = {0};
	GetFileNameW(file,tmp,sizeof(tmp));

	// get WIN32_FIND_DATA for the file pointed to file
	WIN32_FIND_DATAW d;
	ZeroMemory(&d,sizeof(d));
	HANDLE h=FindFirstFileW(file,&d);

	// don't forget to close the handle, otherwise we have a mem-leak
	FindClose(h);

	// if the filename retrieved from file and the filename retrieved
	// by getting the WIN32_FIND_DATA are the same when compared (not case-sensitive)
	// then this file DOES exist, otherwise it DOESN'T exist
	return _wcsicmp(tmp,d.cFileName)==0;
}
//--------------------------------------------------------------------------------------------
void GetDLLPath(HMODULE h,char* path, DWORD nSize)
{
	// pre:  h is not 0 and path is not 0, nSize determines the maximum number of
	//		 characters path can hold
	// post: the path (without the filename) where module h is located is stored
	//		  in path

	GetModuleFileNameA(h,path,MAX_PATH);

	for(int i=strlen(path)-1;i>0;--i)
	{
		if(path[i]=='\\' || path[i]=='/')
		{
			path[i+1] = '\0';
			break;
		}
	}
}
void GetDLLPathW(HMODULE h,WCHAR* path, DWORD nSize)
{
	// pre:  h is not 0 and path is not 0, nSize determines the maximum number of
	//		 characters path can hold
	// post: the path (without the filename) where module h is located is stored
	//		  in path

	GetModuleFileNameW(h,path,MAX_PATH);

	for(int i=wcslen(path)-1;i>0;--i)
	{
		if(path[i]=='\\' || path[i]=='/')
		{
			path[i+1] = '\0';
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------
bool strcmp_nocase(const char* str1, const char* str2)
{
	// pre:  str1 and str2 are not 0
	// post: if str1 and str2 are the same when compared not case-sensitive, then
	//		  true is returned, otherwise false is returned
	
	if(!str1 || !str2)return false;

	int i=0;
	while(str1[i]!=0 && str2[i]!=0 && tolower(str1[i]) == tolower(str2[i]))i++;
	return str1[i]==0 && str2[i]==0;
}

// ave: 06/6 - begin
//--------------------------------------------------------------------------------------------
HGLOBAL GlobalClone(HGLOBAL hglobIn)
{
	// post: a clone of hglobIn has been returned.

	// source: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnwui/html/ddhelp_pt2.asp

    HGLOBAL hglobOut = NULL;

    LPVOID pvIn = GlobalLock(hglobIn);
    if (pvIn)
	{
        SIZE_T cb = GlobalSize(hglobIn);
        HGLOBAL hglobOut = GlobalAlloc(GMEM_FIXED, cb);
        if (hglobOut)
		{
            CopyMemory(hglobOut, pvIn, cb);
        }
        GlobalUnlock(hglobIn);
    }

    return hglobOut;
}
//--------------------------------------------------------------------------------------------
IUnknown *GetCanonicalIUnknown(IUnknown* punk)
{
	// post: the canonical IUnknown of punk has been returned

	// source: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnwui/html/ddhelp_pt2.asp

    IUnknown *punkCanonical;
    if (punk && SUCCEEDED(punk->QueryInterface(IID_IUnknown,
                                               (void**)&punkCanonical))) 
	{
        punkCanonical->Release();
    }
	else
	{
        punkCanonical = punk;
    }
    return punkCanonical;
}
//--------------------------------------------------------------------------------------------
// ave: 06/6 - end
std::wstring StringToWString(const std::string& str)
{
	// post: a multibyte string has been converted to a unicode wstring equivalent

	std::wstring ret;

	WCHAR* tmp = new WCHAR[ str.length() + 2];
	memset(static_cast<void*>(tmp),0,str.length()+2);

	MultiByteToWideChar(GetACP(),0,str.c_str(),-1,tmp,str.length()+1 );

	ret = tmp;

	delete[] tmp;

	return ret;
}
//--------------------------------------------------------------------------------------------

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
//--------------------------------------------------------------------------------------------
void ApplyQualityTo(UINT quality,Graphics* g)
{
	// pre:  g is not NULL
	// post: the quality for this docklet is applied to graphics object g
	if(!g)return;

	switch(quality)
	{
		case QUALITY_LOWEST:
			g->SetSmoothingMode(SmoothingModeNone);
			g->SetInterpolationMode(InterpolationModeLowQuality);
		break;

		case QUALITY_LOW:
			g->SetSmoothingMode(SmoothingModeHighSpeed);
			g->SetInterpolationMode(InterpolationModeHighQuality);
		break;

		default: // DEFAULT TOO!
		case QUALITY_AVERAGE:
			g->SetSmoothingMode(SmoothingModeHighQuality);
			g->SetInterpolationMode(InterpolationModeHighQuality);
		break;

		case QUALITY_GOOD:
			g->SetSmoothingMode(/*SmoothingModeAntiAlias8x4*/SmoothingModeAntiAlias);
			g->SetInterpolationMode(InterpolationModeNearestNeighbor);
		break;

		case QUALITY_BEST:
			g->SetSmoothingMode(/*SmoothingModeAntiAlias8x8*/SmoothingModeAntiAlias);
			g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		break;
	}

}
//--------------------------------------------------------------------------------------------

	void FixRotationF(int rotation, SizeF& s, PointF& p)
	{		
		if(180 == rotation)
		{
			return;
		}

		const REAL pi = 3.14192f;
		const REAL rad = pi / 180.0f;

		float w = s.Width, h = s.Height, left = p.X, top = p.Y;

		rotation = abs(rotation % 360);
		int origRotation = rotation;
		if(rotation > 270)
			rotation = rotation - 270;
		else if(rotation > 180)
			rotation = rotation - 180;
		else if(rotation > 90)
			rotation = rotation - 90;


		float radRotation = static_cast<float>(rotation) / 180.0f * pi;
		float beta = static_cast<float>(90 - rotation) / 180.0f * pi;
		float alpha = radRotation;

		float a = 0, b = 0, c = 0, d = 0;

		if(origRotation <= 90 || (origRotation > 180 && origRotation <= 270))
		{
			a = cos (alpha) * w;
			c = sin (alpha) * w;
	
			b=  cos (beta) * h;
			d = sin (beta) * h;
		}
		else
		{
			a = cos (alpha) * h;
			c = sin (alpha) * h;
	
			b = cos (beta) * w;
			d = sin (beta) * w;
		}

		float w2 = (float)w;
		float h2 = (float)h;
		w = (b + a);
		h = (c + d);

		left = ((w - w2) / 2);
		top  = ((h - h2) / 2);
	
		p.X = left;
		p.Y = top;

		s.Width = w;
		s.Height = h;
	}

	void FixRotation(int rotation, SIZE& s, POINT& p)
	{		
		// quick fix for 180 bug:
		if(180 == rotation)
		{
			return;
		}

		const REAL pi = 3.14192f;
		const REAL rad = pi / 180.0f;

		int w = s.cx, h = s.cy, left = p.x, top = p.y;

		rotation = abs(rotation % 360);
		int origRotation = rotation;
		if(rotation > 270)
			rotation = rotation - 270;
		else if(rotation > 180)
			rotation = rotation - 180;
		else if(rotation > 90)
			rotation = rotation - 90;


		float radRotation = static_cast<float>(rotation) / 180.0f * pi;
		float beta = static_cast<float>(90 - rotation) / 180.0f * pi;
		float alpha = radRotation;

		float a = 0, b = 0, c = 0, d = 0;

		if(origRotation <= 90 || (origRotation > 180 && origRotation <= 270))
		{
			a = cos (alpha) * w;
			c = sin (alpha) * w;
	
			b=  cos (beta) * h;
			d = sin (beta) * h;
		}
		else
		{
			a = cos (alpha) * h;
			c = sin (alpha) * h;
	
			b = cos (beta) * w;
			d = sin (beta) * w;
		}

		float w2 = (float)w;
		float h2 = (float)h;
		w = (int)(b + a);
		h = (int)(c + d);

		left = (int)((w - w2) / 2);
		top  = (int)((h - h2) / 2);
	
		p.x = left;
		p.y = top;

		s.cx = w;
		s.cy = h;
	}
//--------------------------------------------------------------------------------------------
Bitmap* HICONToGDIPlusBitmap(HICON icon)
// pre:  icon is not NULL
// post: A GDI+ bitmap object constructed from icon. If not succesfull, NULL has been returned.
// note: This function fixes the behaviour of GDIPlus::Bitmap::FromHICON() which
//       does not use the per-pixel alpha information of 32 bits icons.
{
	if(NULL == icon)
		return NULL;

	HDC screenDC = GetDC(NULL);
	int screenBitDepth = GetDeviceCaps(screenDC,BITSPIXEL);
	ReleaseDC(NULL,screenDC);

	if(32 == screenBitDepth)
	{
		ICONINFO iconInfo = {0};
		iconInfo.fIcon = TRUE;
		GetIconInfo(icon,&iconInfo);

		int height = iconInfo.xHotspot*2;
		int width = iconInfo.xHotspot*2;

		Bitmap* bmp = new Bitmap(width, height);

		BitmapData data;
		Rect r(0,0,bmp->GetWidth(),bmp->GetHeight());
		bmp->LockBits(&r,ImageLockModeWrite,PixelFormat32bppARGB,&data);
		
		// copies the colour icon, but not yet masked.
		GetBitmapBits(iconInfo.hbmColor,height * width *4,data.Scan0);

		boost::shared_ptr<Bitmap> mask( Bitmap::FromHBITMAP(iconInfo.hbmMask,NULL) );
		BitmapData maskData;

		// use 32BPP, because that will align nicely on DWORD boundaries.
		mask->LockBits(&r,ImageLockModeRead,PixelFormat32bppARGB,&maskData);


		BYTE* maskPtr = (BYTE*)maskData.Scan0;
		BYTE* dataPtr = (BYTE*)data.Scan0;

		int numScanLines = maskData.Height;
		int numPixelPerScanLine = maskData.Width;

		// roughly mask the bitmap with its mask
		for(int y=0;y<numScanLines; ++y)
		{
			BYTE* dataScanLineStart = dataPtr;
			BYTE* maskScanLineStart = maskPtr;
			for(int x=0;x<numPixelPerScanLine;++x)
			{
				if(*(maskPtr+1) == 0)
				{
					DWORD* dwPtr = (DWORD*) dataPtr;
					if( ((*dwPtr) & 0xFF000000) == 0)
						*dwPtr = *dwPtr | 0xFF000000;
				}

				dataPtr += sizeof(DWORD);
				maskPtr += sizeof(DWORD);
						
			}

			dataPtr = dataScanLineStart + data.Stride;
			maskPtr = maskScanLineStart + maskData.Stride;
		}

		bmp->UnlockBits(&data);
		mask->UnlockBits(&maskData);


		DeleteObject(iconInfo.hbmColor);
		DeleteObject(iconInfo.hbmMask);

		return bmp;
		
	}
	else
	{
		// GDI+ is grown-up enough to convert icons with a smaller bitdepth than 32.
		return Bitmap::FromHICON(icon);
	}

}
//--------------------------------------------------------------------------------------------
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	// SOURCE: MSDN

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