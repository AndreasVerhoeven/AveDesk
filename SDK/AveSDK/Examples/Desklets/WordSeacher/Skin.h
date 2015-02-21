// Skin.h: interface for the CSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKIN_H__1CC2F2B3_1C1E_4960_9EA2_C38B93DAF10E__INCLUDED_)
#define AFX_SKIN_H__1CC2F2B3_1C1E_4960_9EA2_C38B93DAF10E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>						 // for std::wstring
#include "../../../aveconstants.h"		 // for AVE_FONT structure
#include <gdiplus.h>					 // for Color, FontStyle enum
using namespace Gdiplus;

// Because this desklet is skinnable (like all desklet
// should be preferably), we have a custom CSkin class
// that holds the skin properties.
class CSkin  
{
public: // instead of making this private and make a
		// a lot accessor functions, we simply make
		// those members public.
	std::wstring fileName;

	SIZE topSize;
	SIZE midSize;
	SIZE botSize;

	SIZE boxSize;
	POINT boxPlace;

	SIZE goSize;
	POINT goPlace;

	POINT closeButtonPlace;

	SIZE scrollBarSize;
	POINT scrollBarPlace;

	SIZE viewSize;
	POINT viewPlace;

	AVE_FONT boxFont;
	Color boxFontColor;
	AVE_FONT viewFont;
	Color viewFontColor;

	SIZE backSideSize;

	POINT comboSearchModePlace;
	SIZE comboSearchModeSize;

	POINT doneButtonPlace;
	SIZE doneButtonSize;

	POINT flipPlace;
	SIZE flipSize;

	POINT linkPlace;
	SIZE linkSize;
	AVE_FONT linkFont;

	SIZE ReadSize(const WCHAR* name, const WCHAR* group);
	POINT ReadPlace(const WCHAR* name, const WCHAR* group);
	AVE_FONT ReadFont(const WCHAR* name, const WCHAR* group);
	Color ReadColor(const WCHAR* name, const WCHAR* group);

public:
	CSkin();
	~CSkin();

	BOOL Read(const WCHAR* filename);
};

#endif // !defined(AFX_SKIN_H__1CC2F2B3_1C1E_4960_9EA2_C38B93DAF10E__INCLUDED_)
