//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef AVEDESKUTIL_HPP_20C293AB93848F210EDD9119292C3938471A
#define AVEDESKUTIL_HPP_20C293AB93848F210EDD9119292C3938471A

#include <windows.h>
#include "DeskletSDK.h"

#define AVE_DESKLET_USE_STD_STRING
#ifdef AVE_DESKLET_USE_STD_STRING
#include <string>
#endif//AVE_DESKLET_USE_STD_STRING

class AveDeskUtil
{
public:
	#ifdef AVE_DESKLET_USE_STD_STRING
	static Bitmap* LoadGDIPlusImage(std::string& imageFilePath);
	static BOOL BrowseForImage(HWND hwndParent, std::string& imageFilePath);
	static HWND LoadDesklet(std::string& name, std::string& ini, std::string& iniSection, void*& p, BOOL makeVisible=FALSE);
	static void DoAction(UINT action);
	#else

	#endif//AVE_DESKLET_USE_STD_STRING
};

#endif//AVEDESKUTIL_HPP_20C293AB93848F210EDD9119292C3938471A