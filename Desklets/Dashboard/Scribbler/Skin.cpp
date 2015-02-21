// Skin.cpp: implementation of the CSkin class.
//
//////////////////////////////////////////////////////////////////////

#include "Skin.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkin::CSkin()
{

}

CSkin::~CSkin()
{

}

int CSkin::ReadFontStyle(const WCHAR* section, const WCHAR* ini)
{
	const size_t MAX_STYLE = 255;
	WCHAR style[MAX_STYLE] = {0};
	GetPrivateProfileStringW(section, L"FontStyle", style, style, MAX_STYLE, ini);
	int fs = 0;
	if(wcsstr(style, L"bold") != NULL)		fs |= FontStyleBold;
	if(wcsstr(style, L"italic") != NULL)	fs |= FontStyleItalic;
	if(wcsstr(style, L"underline") != NULL)	fs |= FontStyleUnderline;

	return fs;
}

RECT CSkin::ReadRect(const WCHAR* section, const WCHAR* ini)
{
	RECT r = {0};
	r.left = GetPrivateProfileIntW(section, L"X", 0, ini);
	r.top = GetPrivateProfileIntW(section, L"Y", 0, ini);
	r.right = r.left + GetPrivateProfileIntW(section, L"Width", 0, ini);
	r.bottom = r.top + GetPrivateProfileIntW(section, L"Height", 0, ini);

	return r;
}

AVE_FONT2 CSkin::ReadFont2(const WCHAR* section, const WCHAR* ini)
{
	AVE_FONT2 f = {0};
	GetPrivateProfileStringW(section, L"FaceName", L"Lucida Sans Unicode", f.faceName, sizeof(f.faceName) / sizeof(f.faceName[0]), ini);
	f.size = static_cast<float>(GetPrivateProfileIntW(section, L"FontSize", 0, ini));
	f.style = ReadFontStyle(section, ini);

	return f;
}

AVE_FONT CSkin::ReadFont(const WCHAR* section, const WCHAR* ini)
{
	AVE_FONT f = {0};
	GetPrivateProfileStringW(section, L"FaceName", L"Lucida Sans Unicode", f.faceName, sizeof(f.faceName) / sizeof(f.faceName[0]), ini);
	f.size = GetPrivateProfileIntW(section, L"FontSize", 0, ini);
	f.style = ReadFontStyle(section, ini);

	return f;
}

DWORD CSkin::ReadColor(const WCHAR* section, const WCHAR* ini)
{
	Color c(GetPrivateProfileIntW(section, L"ColorA", 255, ini),
			GetPrivateProfileIntW(section, L"ColorR", 0, ini),
			GetPrivateProfileIntW(section, L"ColorG", 0, ini),
			GetPrivateProfileIntW(section, L"ColorB", 0, ini)
		);

	return c.GetValue();
}

AVE_SHADOWPROPERTIES CSkin::ReadShadow(const WCHAR* section, const WCHAR* ini)
{
	AVE_SHADOWPROPERTIES p = {0};
	p.dwSize = sizeof(p);
	p.blurRatio = static_cast<float>(GetPrivateProfileIntW(section, L"ShadowBlurRatio", 0, ini)) / 10.0f;
	p.extraHeight = GetPrivateProfileIntW(section, L"ShadowExtraHeight", 0, ini);
	p.extraWidth = GetPrivateProfileIntW(section, L"ShadowExtraWidth", 0, ini);
	p.xOffset = GetPrivateProfileIntW(section, L"ShadowXOffset", 0, ini);
	p.yOffset = GetPrivateProfileIntW(section, L"ShadowYOffset", 0, ini);

	Color c(GetPrivateProfileIntW(section, L"ShadowColorA", 255, ini),
			GetPrivateProfileIntW(section, L"ShadowColorR", 0, ini),
			GetPrivateProfileIntW(section, L"ShadowColorG", 0, ini),
			GetPrivateProfileIntW(section, L"ShadowColorB", 0, ini)
		);

	p.color = c.GetValue();
	p.intensity = GetPrivateProfileIntW(section, L"ShadowIntensity", 0, ini);

	return p;
}

BOOL CSkin::ReadBOOL(const WCHAR* section, const WCHAR* key, const WCHAR* ini, BOOL defVal)
{
	const size_t MAX_BOOL_DATA = 100;
	WCHAR wBool[MAX_BOOL_DATA] = {0};
	GetPrivateProfileStringW(section, key, defVal ? L"yes" : L"no", wBool, MAX_BOOL_DATA, ini);

	return wcsicmp(wBool, L"yes") == 0 || wcsicmp(wBool, L"true") == 0 || wcsicmp(wBool, L"ok") == 0
					|| wcsicmp(wBool, L"1") == 0;
}


BOOL CSkin::Load(const WCHAR* ini)
{
	const size_t MAX_ABOUT_DATA = 512;

	this->ini = ini;

	// read about data
	WCHAR wName[MAX_ABOUT_DATA] = {0};
	GetPrivateProfileStringW(L"About", L"Name", L"Unnamed", wName, MAX_ABOUT_DATA, ini);
	name = wName;

	WCHAR wAuthor[MAX_ABOUT_DATA] = {0};
	GetPrivateProfileStringW(L"About", L"Author", L"Unknown", wAuthor, MAX_ABOUT_DATA, ini);
	author = wAuthor;

	WCHAR wComments[MAX_ABOUT_DATA] = {0};
	GetPrivateProfileStringW(L"About", L"Comment", L"", wComments, MAX_ABOUT_DATA, ini);
	comment = wComments;

	WCHAR wVersion[MAX_ABOUT_DATA] = {0};
	GetPrivateProfileStringW(L"About", L"Version", L"1.0", wVersion, MAX_ABOUT_DATA, ini);
	version = wVersion;

	// read type, type should match "translator"
	WCHAR wType[MAX_ABOUT_DATA] = {0};
	GetPrivateProfileStringW(L"About", L"Type", L"", wType, MAX_ABOUT_DATA, ini);
	if(wcsicmp(wType, L"ChalkBoard") != 0)
		return FALSE;

	// read general info
	front.cx = GetPrivateProfileIntW(L"General", L"FrontWidth", 0, ini);
	front2.cx = front.cx * 2;
	front.cy = GetPrivateProfileIntW(L"General", L"FrontHeight", 0, ini);
	front2.cy = front.cy * 2;
	back.cx  = GetPrivateProfileIntW(L"General", L"BackWidth", 0, ini);
	back2.cx  = back.cx * 2;
	back.cy  = GetPrivateProfileIntW(L"General", L"BackHeight", 0, ini);
	back2.cy = back.cy * 2;

	// read rectangles
	flippie			= ReadRect(L"flippie", ini);
	drawingArea		= ReadRect(L"drawing_area", ini);
	butBig			= ReadRect(L"big_button", ini);
	butMed			= ReadRect(L"medium_button", ini);
	butSmall		= ReadRect(L"small_button", ini);
	backDoneButton	= ReadRect(L"back_done_button", ini);
	butClear		= ReadRect(L"clear_button", ini);


	// closePosition
	closePos.x = GetPrivateProfileIntW(L"CloseButton", L"X", 0, ini);
	closePos.y = GetPrivateProfileIntW(L"CloseButton", L"Y", 0, ini);

	// default color
	defaultColor = ReadColor(L"DefaultColor", ini);

	// color buttons
	colors.clear();
	UINT colorCount = GetPrivateProfileIntW(L"colors", L"count", 0, ini);
	for(UINT i =0; i < colorCount; ++i)
	{
		std::wstringstream ws;
		ws << L"Color" << i + 1;
		std::wstring key(ws.str());

		ColorEntry entry = {0};
		GetPrivateProfileStringW(key.c_str(), L"Name", L"", entry.name, sizeof(entry.name) / sizeof(entry.name[0]), ini);
		entry.pos = ReadRect(key.c_str(), ini);
		entry.color = ReadColor(key.c_str(), ini);

		colors.push_back(entry);
	}

	return TRUE;
}