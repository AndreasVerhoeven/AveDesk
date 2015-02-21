// Skin.h: interface for the CSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKIN_H__2BDF1940_8365_45FB_BBDC_CFA4A4F24BDC__INCLUDED_)
#define AFX_SKIN_H__2BDF1940_8365_45FB_BBDC_CFA4A4F24BDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4503)
#pragma warning(disable:4786)


#include <windows.h>
#include <wininet.h>
#include <gdiplus.h>
#include <shlguid.h>
using namespace Gdiplus;

class CSkin  
{

public:
	const char* GetInfoName()const { return infoName;};
	const char* GetInfoAuthor()const { return infoAuthor;};
	const char* GetInfoComments()const { return infoComments;};

	Rect GetFrameRect()const { return frame;};
	int	 GetWidth()const { return width;};
	int  GetHeight()const { return height;};
	int  GetMarginBetweenTwoFrames()const { return marginBetweenTwoFrames;};;
	int  GetLeftMargin()const { return leftMargin;};
	int  GetRightMargin()const{ return rightMargin;};

	bool IsVertical() { return isVertical;};

	CSkin() {};
	CSkin(const char* ini);
	virtual ~CSkin();

private:
	Rect frame;
	int width;
	int height;
	int marginBetweenTwoFrames;
	int leftMargin;
	int rightMargin;
	bool isVertical;

	char infoName[1024];
	char infoAuthor[1024];
	char infoComments[1024];
};

#endif // !defined(AFX_SKIN_H__2BDF1940_8365_45FB_BBDC_CFA4A4F24BDC__INCLUDED_)
