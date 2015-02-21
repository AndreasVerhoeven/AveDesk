//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef AVEDESKUTIL_CPP_20C293AB93848F210EDD9119292C3938471A
#define AVEDESKUTIL_CPP_20C293AB93848F210EDD9119292C3938471A

#include "AveDeskUtil.hpp"

#ifdef AVE_DESKLET_USE_STD_STRING

Bitmap* AveDeskUtil::LoadGDIPlusImage(std::string& imageFilePath)
{
	return ::DeskletLoadGDIPlusImage(imageFilePath.c_str() );
}

BOOL AveDeskUtil::BrowseForImage(HWND hwndParent, std::string& imageFilePath)
{
	char buf[MAX_PATH+2];
	memset(buf,0,MAX_PATH+2);

	strcpy_s(buf,MAX_PATH+2,imageFilePath.c_str() );

	BOOL bRet = ::DeskletBrowseForImage(hwndParent,buf);

	imageFilePath = buf;

	return bRet;
}

HWND AveDeskUtil::LoadDesklet(std::string& name, std::string& ini, std::string& iniSection, void*& p, BOOL makeVisible)
{
	return ::DeskletLoad(name.c_str(),ini.c_str(),iniSection.c_str(),&p,makeVisible);
}

void AveDeskUtil::DoAction(UINT action)
{
	::DeskletManager(action);
}

#else


#endif//AVE_DESKLET_USE_STD_STRING


#endif//AVEDESKUTIL_CPP_20C293AB93848F210EDD9119292C3938471A