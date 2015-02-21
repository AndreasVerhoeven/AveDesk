// Skin.cpp: implementation of the CSkin class.
//
//////////////////////////////////////////////////////////////////////

#include "Skin.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkin::CSkin()
{

}

CSkin::~CSkin()
{

}

SIZE CSkin::ReadSize(const WCHAR* name, const WCHAR* group)
{
	// this function reads a SIZE structure from the skin-file
	SIZE s = {0,0};

	std::wstring keyWidth(name);
	keyWidth += L"Width";

	std::wstring keyHeight(name);
	keyHeight += L"Height";

	s.cx = GetPrivateProfileIntW(group, keyWidth.c_str(), 0, fileName.c_str());
	s.cy = GetPrivateProfileIntW(group, keyHeight.c_str(), 0, fileName.c_str());

	return s;
}

POINT CSkin::ReadPlace(const WCHAR* name, const WCHAR* group)
{
	// this function reads a POINT structure from the skin-file
	POINT p = {0,0};

	std::wstring keyX(name);
	keyX += L"X";
	std::wstring keyY(name);
	keyY += L"Y";

	p.x = GetPrivateProfileIntW(group, keyX.c_str(), 0, fileName.c_str());
	p.y = GetPrivateProfileIntW(group, keyY.c_str(), 0, fileName.c_str());

	return p;
}

AVE_FONT CSkin::ReadFont(const WCHAR* name, const WCHAR* group)
{
	// this function reads an AVE_FONT structure from the skin-file
	AVE_FONT af = {0};
	std::wstring keyFaceName(name);
	keyFaceName += L"FaceName";
	GetPrivateProfileStringW(group, keyFaceName.c_str(), L"Lucida Sans Unicode", af.faceName, LF_FACESIZE, fileName.c_str());

	std::wstring keySize(name);
	keySize += L"Size";
	af.size = GetPrivateProfileIntW(group, keySize.c_str(), 12, fileName.c_str());

	std::wstring keyStyles(name);
	keyStyles += L"Style";

	WCHAR styles[1024] = {0};
	GetPrivateProfileStringW(group, keyStyles.c_str(), L"bold", styles, sizeof(styles) / sizeof(styles[0]), fileName.c_str());
	if(wcsstr(styles, L"bold") != NULL)
		af.style |= FontStyleBold;
	if(wcsstr(styles, L"italic") != NULL)
		af.style |= FontStyleItalic;
	if(wcsstr(styles, L"underline") != NULL)
		af.style |= FontStyleUnderline;

	return af;
}

Color CSkin::ReadColor(const WCHAR* name, const WCHAR* group)
{
	// this function reads a color
	std::wstring keyAlpha(name);
	keyAlpha += L"Alpha";
	std::wstring keyRed(name);
	keyRed += L"Red";
	std::wstring keyGreen(name);
	keyGreen += L"Green";
	std::wstring keyBlue(name);
	keyBlue += L"Blue";

	BYTE alpha = GetPrivateProfileIntW(group, keyAlpha.c_str(), 255, fileName.c_str());
	BYTE red   = GetPrivateProfileIntW(group, keyRed.c_str(),   0,   fileName.c_str());
	BYTE green = GetPrivateProfileIntW(group, keyGreen.c_str(), 0,   fileName.c_str());
	BYTE blue  = GetPrivateProfileIntW(group, keyBlue.c_str(),  0,   fileName.c_str());

	return Color(alpha, red, green, blue);
}

BOOL CSkin::Read(const WCHAR* filename)
{
	// this function reads a skin-file

	if(NULL == filename)
		return FALSE;

	fileName = filename;

	// read the background
	botSize = ReadSize(L"bottom", L"background");
	midSize = ReadSize(L"middle", L"background");
	topSize = ReadSize(L"top", L"background");

	// read the inputbox edit
	boxSize  = ReadSize(L"edit", L"inputbox");
	boxPlace = ReadPlace(L"edit", L"inputbox");

	// read the inputbox button
	goSize  = ReadSize(L"button", L"inputbox");
	goPlace = ReadPlace(L"button", L"inputbox");

	// read general information 
	closeButtonPlace = ReadPlace(L"closeButton", L"general");

	// read the scrollbar
	scrollBarSize  = ReadSize(L"", L"scrollbar");
	scrollBarPlace = ReadPlace(L"", L"scrollbar");

	// read view
	viewSize  = ReadSize(L"", L"view");
	viewPlace = ReadPlace(L"", L"view");

	// read fonts
	boxFont =  ReadFont(L"Font", L"inputbox");
	boxFontColor = ReadColor(L"FontColor", L"inputbox");
	viewFont = ReadFont(L"Font", L"view");
	viewFontColor = ReadColor(L"FontColor", L"view");

	// read backside
	backSideSize = ReadSize(L"", L"backside");

	// read backsize comboSearchMode control
	comboSearchModePlace = ReadPlace(L"", L"back_ComboSearchMode");
	comboSearchModeSize  = ReadSize(L"", L"back_ComboSearchMode");

	// read backside doneButton control
	doneButtonPlace = ReadPlace(L"", L"back_donebutton");
	doneButtonSize  = ReadSize(L"", L"back_donebutton");

	// read flip
	flipPlace = ReadPlace(L"", L"flip");
	flipSize  = ReadSize(L"", L"flip");

	linkPlace = ReadPlace(L"", L"back_link");
	linkSize = ReadSize(L"", L"back_link");
	linkFont = ReadFont(L"", L"back_link");

	return TRUE;
}
