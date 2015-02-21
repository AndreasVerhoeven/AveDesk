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

#include "cicondragdropframe.hpp"

CIconDragDropFrame::CIconDragDropFrame(HWND hwnd) :
	CStatic(hwnd),
	hIcon(0)
{
	SetIconByFileName("",0);
}

CIconDragDropFrame::~CIconDragDropFrame()
{
		if(hIcon)
		DestroyIcon(hIcon);	
}

const char* CIconDragDropFrame::GetIconFile()const
{
	return selIconFile;
}

long CIconDragDropFrame::GetIconIndex()const
{
	return selIconIndex;
}

void CIconDragDropFrame::SetIconByFileName(const char* iconFile,long iconIndex)
{
	strcpy(selIconFile,iconFile);
	selIconIndex = iconIndex;

	if(hIcon)
		DestroyIcon(hIcon);

	hIcon = ExtractIcon(GetModuleHandle(0),selIconFile,selIconIndex);
	if((int)hIcon == 1)
		hIcon = 0;

	SetIcon(hIcon);	
}