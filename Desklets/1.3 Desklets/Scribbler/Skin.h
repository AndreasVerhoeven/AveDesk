// Skin.h: interface for the CSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKIN_H__BCBFE3A8_06B4_4D74_A1D8_B79D90D39017__INCLUDED_)
#define AFX_SKIN_H__BCBFE3A8_06B4_4D74_A1D8_B79D90D39017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#include <string>

#include "../AveControls/aveconstants.h"
#pragma comment(lib, "../AveControls.lib")
#include "../AveControls.h"

#include <vector>

struct ColorEntry
{
	WCHAR name[MAX_PATH];
	RECT pos;
	DWORD color;
};

class CSkin  
{
public:
	std::wstring name;
	std::wstring author;
	std::wstring comment;
	std::wstring version;
	std::wstring ini;
	std::wstring dir;

	SIZE front, front2;
	SIZE back, back2;

	RECT flippie;
	RECT drawingArea;
	RECT butBig;
	RECT butMed;
	RECT butSmall;
	RECT backDoneButton;
	RECT butClear;

	POINT closePos;

	DWORD defaultColor;

	std::vector<ColorEntry> colors;

	static int ReadFontStyle(const WCHAR* section, const WCHAR* ini);
	static RECT ReadRect(const WCHAR* section, const WCHAR* ini);
	static DWORD ReadColor(const WCHAR* section, const WCHAR* ini);
	static AVE_FONT2 ReadFont2(const WCHAR* section, const WCHAR* ini);
	static AVE_FONT ReadFont(const WCHAR* section, const WCHAR* ini);
	static AVE_SHADOWPROPERTIES ReadShadow(const WCHAR* section, const WCHAR* ini);
	static BOOL ReadBOOL(const WCHAR* section, const WCHAR* key, const WCHAR* ini, BOOL defVal = FALSE);

	BOOL Load(const WCHAR* ini);

public:
	CSkin();
	~CSkin();

};

#endif // !defined(AFX_SKIN_H__BCBFE3A8_06B4_4D74_A1D8_B79D90D39017__INCLUDED_)
