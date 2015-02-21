// IAveResources.cpp: implementation of the IAveResources class.
//
//////////////////////////////////////////////////////////////////////

#include "IAveResources.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

std::wstring IAveResources::StringToWString(const std::string& str)
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

std::string IAveResources::WStringToString(const std::wstring& wstr)
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



IAveResources::IAveResources(const char* dirName) :
	dirName( StringToWString(dirName) )
{
}

IAveResources::IAveResources(const WCHAR* dirName) :
	dirName(dirName)
{

}

IAveResources::~IAveResources()
{

}

std::string IAveResources::GetDirName()const
{
	return WStringToString(dirName);
}

std::string IAveResources::GetFilename(const char* resName)
{
	if(NULL == resName)
		return "";

	if( strstr(resName, ":") != NULL)
		return resName;

	std::string fName = WStringToString(dirName) + resName;

	return fName;
}

Bitmap* IAveResources::GetBitmap(const char* resName)
{
	return DeskletLoadGDIPlusImage(GetFilename(resName).c_str());
}

std::wstring IAveResources::GetDirNameW()const
{
	return dirName;
}

std::wstring IAveResources::GetFilenameW(const WCHAR* resName)
{
	if(NULL == resName)
		return L"";

	if( wcsstr(resName, L":") != NULL)
		return resName;

	std::wstring fName = dirName + resName;

	return fName;
}

Bitmap* IAveResources::GetBitmapW(const WCHAR* resName)
{
	return DeskletLoadGDIPlusImageW(GetFilenameW(resName).c_str());
}

void IAveResources::SetPath(const char* path)
{
	if(NULL == path)
		return;

	dirName = StringToWString(path);
}

void IAveResources::SetPathW(const WCHAR* path)
{
	if(NULL == path)
		return;

	dirName = path;
}