// IAveResources.cpp: implementation of the IAveResources class.
//
//////////////////////////////////////////////////////////////////////

#include "IAveResources.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IAveResources::IAveResources(const char* dirName) :
	dirName(dirName)
{

}

IAveResources::~IAveResources()
{

}

std::string IAveResources::GetFilename(const char* resName)
{
	if(NULL == resName)
		return "";

	std::string fName = dirName + resName;

	return fName;
}

Bitmap* IAveResources::GetBitmap(const char* resName)
{
	return DeskletLoadGDIPlusImage(GetFilename(resName).c_str());
}