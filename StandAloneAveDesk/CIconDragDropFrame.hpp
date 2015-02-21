// filename:		CGlobalSettingsTraySheet.hpp
// Description:		Definition of class CGlobalSettingsMoveSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"

#ifndef CICONDRAGDROPFRAME_HPP_102894945
#define CICONDRAGDROPFRAME_HPP_102894945

#pragma once

class CIconDragDropFrame : public CStatic
{
	private:
		char selIconFile[MAX_PATH];
		long selIconIndex;
		HICON hIcon;

	public:
		CIconDragDropFrame(HWND hwnd=NULL);
		~CIconDragDropFrame();

		const char* GetIconFile()const;
		long GetIconIndex()const;

		void SetIconByFileName(const char* iconFile, long iconIndex=0);
};

#endif//CICONDRAGDROPFRAME_HPP_102894945