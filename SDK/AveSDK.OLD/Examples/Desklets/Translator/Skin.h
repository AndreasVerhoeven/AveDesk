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

#include "../../../aveconstants.h"
#pragma comment(lib, "../../../AveControls.lib")
#include "../../../AveControls.h"

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

	RECT backDoneButton;
	RECT backLogo;
	RECT textfieldFrom, textfieldFrom2;
	RECT textfieldTo, textfieldTo2;
	RECT comboboxFrom;
	RECT comboboxFromBg;
	RECT comboboxTo;
	RECT comboboxToBg;
	RECT flippie;
	RECT scrollbarFrom;
	RECT scrollbarTo;

	POINT closePos;

	AVE_FONT2 comboboxFromFont;
	AVE_FONT2 comboboxToFont;

	AVE_SHADOWPROPERTIES comboboxFromShadow;
	AVE_SHADOWPROPERTIES comboboxToShadow;
	BOOL comboboxFromDrawShadow;
	BOOL comboboxToDrawShadow;

	AVE_FONT textfieldFromFont;
	AVE_FONT textfieldToFont;

	DWORD comboboxFromColor;
	DWORD comboboxToColor;
	DWORD textfieldFromColor;
	DWORD textfieldToColor;

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
