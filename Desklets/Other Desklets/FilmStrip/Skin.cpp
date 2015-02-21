// Skin.cpp: implementation of the CSkin class.
//
//////////////////////////////////////////////////////////////////////

#include "Skin.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Rect GetPrivateProfileRect(const char* section, const char* ini)
{
	return Rect(	GetPrivateProfileInt(section,"Left",0,ini),
					GetPrivateProfileInt(section,"Top",0,ini),
					GetPrivateProfileInt(section,"Width",0,ini),
					GetPrivateProfileInt(section,"Height",0,ini)
				);
}

CSkin::CSkin(const char* ini)
{
	memset(infoName,0,sizeof(infoName));
	memset(infoAuthor,0,sizeof(infoAuthor));
	memset(infoComments,0,sizeof(infoComments));

	GetPrivateProfileString("Info","Name",infoName,infoName,sizeof(infoName)/sizeof(infoName[0]),ini);
	GetPrivateProfileString("Info","Author",infoAuthor,infoAuthor,sizeof(infoAuthor)/sizeof(infoAuthor[0]),ini);
	GetPrivateProfileString("Info","Info",infoComments,infoComments,sizeof(infoComments)/sizeof(infoComments[0]),ini);

	frame = GetPrivateProfileRect("Frame",ini);

	isVertical = GetPrivateProfileInt("General","IsVertical",FALSE,ini)==TRUE ? true : FALSE;

	width = GetPrivateProfileInt("Bg","Width",0,ini);
	height= GetPrivateProfileInt("Bg","Height",0,ini);

	marginBetweenTwoFrames = GetPrivateProfileInt("Frames","MarginBetweenTwoFrames",0,ini);
	leftMargin = GetPrivateProfileInt("Frames","LeftMargin",0,ini);
	rightMargin= GetPrivateProfileInt("Frames","RightMargin",0,ini);
}

CSkin::~CSkin()
{

}