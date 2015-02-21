//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

// Translator Desklet

#include "../../../DeskletSDK.h"

#include "../../../aveconstants.h"
#include "../../../aveflags.h"
#include "../../../avecontrols.h"
#include "Avepopupmenu.h"


#include <cstring>
#include<string>
#include <map>
#include <list>
#include <algorithm>
#include <sstream>

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "../../../AveHTTP.h"
#include "Skin.h"

#include "SkinEnumerator.h"


#define WM_MENU_LANG_TO   WM_APP + 303
#define WM_MENU_LANG_FROM WM_APP + 304


std::wstring StringToWString(const std::string& str)
{
	// post: a multibyte string has been converted to a unicode wstring equivalent

	std::wstring ret;

	WCHAR* tmp = new WCHAR[ str.length() + 2];
	memset(static_cast<void*>(tmp),0,str.length()+2);

	MultiByteToWideChar(GetACP(),0,str.c_str(),-1,tmp,str.length()+1 );

	ret = tmp;

	delete[] tmp;

	return ret;
}
//--------------------------------------------------------------------------------------------

std::string WStringToString(const std::wstring& wstr)
{
	// post: a unicode wstring has been converted to a multibyte wtring equivalent
	std::string ret;

	char* tmp = new char[ wstr.length() + 2];
	memset(static_cast<void*>(tmp),0,wstr.length()+2);

	WideCharToMultiByte(GetACP(),0,wstr.c_str(),-1,tmp,wstr.length()+1,NULL,NULL );

	ret = tmp;

	delete[] tmp;

	return ret;
}

template<typename K, typename V> const K& find_key_for_value(std::map<K,V>& m, const V& v, const K& defVal)
{
	for(std::map<K,V>::iterator iter = m.begin(); iter != m.end(); ++iter)
	{
		if(iter->second == v)
			return iter->first;
	}

	return defVal;
}

// fade animation constants
const int FLIP_FADE_TIMER_ID			= 2393;
const int FLIP_TIMER_INTERVAL			= 10;
const int flipFadeDiff					= 20;
const int flipFadeMax					= 255;

class Translator
{
public:
	HWND hwnd;
	HINSTANCE dll;

	AVEHANDLE resources;
	AVEHANDLE container;
	AVEHANDLE textFieldA;
	AVEHANDLE textFieldB;
	AVEHANDLE staticFrom;
	AVEHANDLE staticTo;
	AVEHANDLE barA;
	AVEHANDLE barB;
	AVEHANDLE doneBut;
	BOOL barAVisible;
	BOOL barBVisible;

	BOOL flipVisible;
	BOOL isInBackMode;

	HCURSOR oldCursor;

	std::map<std::wstring, std::wstring> codes;
	std::map<std::wstring, std::list<std::wstring> > possibs;

	std::string langFrom, langTo;

	CSkin skin;

	// internal flag keeping track of the flip status
	BOOL isFlipVisible;
	UINT flipAlpha;
	BOOL flipIsFadingIn;

public:
	Translator(HWND hwnd, HINSTANCE dll, const char* group, const char* ini) :
		hwnd(hwnd), dll(dll),
		resources(NULL), container(NULL),
		textFieldA(NULL), textFieldB(NULL),
		staticFrom(NULL), staticTo(NULL),
		langFrom("en"), langTo("nl"),
		flipVisible(FALSE), doneBut(NULL),
		isInBackMode(FALSE),
		barA(NULL),
		barAVisible(FALSE),
		barBVisible(FALSE),
		isFlipVisible(FALSE),
		flipAlpha(0)
	{
		PopulateLanguageCodes();
		PopulatePossibilities();

		if(NULL == group || NULL == ini)
		{
			skin.dir = L"Default";
		}
		else
		{
			WCHAR skinDir[MAX_PATH] = {0};
			GetPrivateProfileStringW(StringToWString(group).c_str(), L"SkinDir", L"Default", skinDir, MAX_PATH, StringToWString(ini).c_str());
			skin.dir = skinDir;
		}

		LoadSkin(skin.dir.c_str());

		ClearAllLayers();
		SetupBackgroundLayers();
		SetupControls();
		ApplySkin();
	}

	void PopulateLanguageCodes()
	{
		codes[L"zh"] = L"Chinese (Simplified)";
		codes[L"zt"] = L"Chinese (Traditional)";
		codes[L"en"] = L"English";
		codes[L"nl"] = L"Dutch";
		codes[L"fr"] = L"French";
		codes[L"de"] = L"German";
		codes[L"el"] = L"Greek";
		codes[L"it"] = L"Italian";
		codes[L"ja"] = L"Japanese";
		codes[L"ko"] = L"Korean";
		codes[L"pt"] = L"Portugese";
		codes[L"ru"] = L"Russian";
		codes[L"es"] = L"Spanish";
	}

	void PopulatePossibilities()
	{
		possibs[L"zh"].push_back(L"en");
		
		possibs[L"zt"].push_back(L"en");

		possibs[L"en"].push_back(L"zh");
		possibs[L"en"].push_back(L"zt");
		possibs[L"en"].push_back(L"nl");
		possibs[L"en"].push_back(L"fr");
		possibs[L"en"].push_back(L"de");
		possibs[L"en"].push_back(L"el");
		possibs[L"en"].push_back(L"it");
		possibs[L"en"].push_back(L"ja");
		possibs[L"en"].push_back(L"ko");
		possibs[L"en"].push_back(L"pt");
		possibs[L"en"].push_back(L"ru");
		possibs[L"en"].push_back(L"es");

		possibs[L"nl"].push_back(L"en");
		possibs[L"nl"].push_back(L"fr");

		possibs[L"fr"].push_back(L"en");
		possibs[L"fr"].push_back(L"de");
		possibs[L"fr"].push_back(L"el");
		possibs[L"fr"].push_back(L"it");
		possibs[L"fr"].push_back(L"pt");
		possibs[L"fr"].push_back(L"nl");
		possibs[L"fr"].push_back(L"es");

		possibs[L"de"].push_back(L"en");
		possibs[L"de"].push_back(L"fr");

		possibs[L"el"].push_back(L"en");
		possibs[L"el"].push_back(L"fr");

		possibs[L"it"].push_back(L"en");
		possibs[L"it"].push_back(L"fr");

		possibs[L"ja"].push_back(L"en");

		possibs[L"ko"].push_back(L"en");

		possibs[L"pt"].push_back(L"en");
		possibs[L"pt"].push_back(L"fr");

		possibs[L"ru"].push_back(L"en");

		possibs[L"es"].push_back(L"en");
		possibs[L"es"].push_back(L"fr");

	}

	~Translator()
	{
		AveFreeControlContainer(container);
		AveFreeResources(resources);
	}

	void ClearAllLayers()
	{
		while(DeskletLayerCount(hwnd) > 0)
			DeskletLayerRemove(hwnd, 0, FALSE);
	}

	void LoadSkin(const WCHAR* dir)
	{
		WCHAR ini[MAX_PATH] = {0};
		GetDLLPathW(dll, ini, MAX_PATH);
		PathAppendW(ini, L"Skins");
		PathAppendW(ini, dir);
		PathAppendW(ini, L"skin.ini");
		skin.Load(ini);
		skin.dir = dir;
	}

	void ApplySkin()
	{
		SkinInfo skinInfo = {0};
		skinInfo.dwSize = sizeof(skinInfo);
		skinInfo.wAuthor = const_cast<WCHAR*>(skin.author.c_str());
		skinInfo.wName = const_cast<WCHAR*>(skin.name.c_str());
		skinInfo.wNotes = const_cast<WCHAR*>(skin.comment.c_str());
		std::wstringstream ws;
		ws << skin.version;
		ws >> skinInfo.version;
		DeskletSetSkinInfoW(hwnd, &skinInfo);

		DeskletSetSize(hwnd, &skin.front);

		DeskletSetClosePoint(hwnd, &skin.closePos, skin.front.cx, skin.front.cy);

		SetImageFileToLayerByName("bg", L"frontside.png");
		SetImageFileToLayerByName("back", L"backside.png");
		SetImageFileToLayerByName("flip", L"flip.png");
		SetImageFileToLayerByName("popup_top", L"popup_top.png");
		SetImageFileToLayerByName("popup_bottom", L"popup_bottom.png");
		SetImageFileToLayerByName("back_logo", L"logo.png");

		UINT backSideLayer = DeskletLayerGetLayer(hwnd, "back", NULL);
		SetImageFileToLayer(backSideLayer, L"backside.png");
		DeskletLayerSetVisibility(hwnd, backSideLayer, FALSE, FALSE);

		UINT flipLayer = DeskletLayerGetLayer(hwnd, "flip", NULL);
		DeskletLayerSetPlacement(hwnd, flipLayer, FALSE, TRUE, &skin.flippie, skin.front.cx, skin.front.cy);

		UINT popupTopLayer = DeskletLayerGetLayer(hwnd, "popup_top", NULL);
		DeskletLayerSetPlacement(hwnd, popupTopLayer, FALSE, TRUE, &skin.comboboxFromBg, skin.front.cx, skin.front.cy);

		UINT popupBottomLayer = DeskletLayerGetLayer(hwnd, "popup_bottom", NULL);
		DeskletLayerSetPlacement(hwnd, popupBottomLayer, FALSE, TRUE, &skin.comboboxToBg, skin.front.cx, skin.front.cy);

		UINT logoLayer = DeskletLayerGetLayer(hwnd, "back_logo", NULL);
		DeskletLayerSetPlacement(hwnd, logoLayer, FALSE, TRUE, &skin.backLogo, skin.back.cx, skin.back.cy);


		AveSetResourcesPathW(resources, CreateSkinnedPath(L"controls\\").c_str());

		AveLockContainer(container);

		AveLockControl(doneBut);
		if(!isInBackMode)
			AveSendMessage(doneBut, AV_HIDE, 0L, 0L);
		// bugfix: image wasn't updated for the button, so we reset the names with an invalid file
		AveSendMessage(doneBut,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(doneBut,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(doneBut,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"done.png");
		AveSendMessage(doneBut,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"done_pressed.png");
		AveSendMessage(doneBut, AV_SET_RETRIEVEIMAGESFROMRES, TRUE, 0L);
		AveSendMessage(doneBut, AV_SET_RECT, (WPARAM)&skin.backDoneButton, (LPARAM)&skin.back);
		AveSendMessage(doneBut, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnDoneButtonClick);
		AveSendMessage(doneBut, AV_UPDATE, 0L, 0L);
		AveUnlockControl(doneBut);

		AveLockControl(textFieldA);
		AveSendMessage(textFieldA, AV_SET_ONENTERCALLBACK, (WPARAM)this, (LPARAM)OnEnterPressed);
		AveSendMessage(textFieldA, AV_SET_FONT, 0L, (LPARAM)&skin.textfieldFromFont);
		AveSendMessage(textFieldA, AV_SET_TEXTCOLOR, 0L, (LPARAM)skin.textfieldFromColor);
		AveSendMessage(textFieldA, AV_SET_RECT, (WPARAM)&skin.textfieldFrom2, (LPARAM)&skin.front2);
		AveSendMessage(textFieldA, AV_UPDATE, 0L, 0L);
		AveUnlockControl(textFieldA);

		AveLockControl(textFieldB);
		AveSendMessage(textFieldB, AV_SET_ONENTERCALLBACK, (WPARAM)this, (LPARAM)OnEnterPressed);
		AveSendMessage(textFieldB, AV_SET_FONT, 0L, (LPARAM)&skin.textfieldToFont);
		AveSendMessage(textFieldB, AV_SET_TEXTCOLOR, 0L, (LPARAM)skin.textfieldToColor);
		AveSendMessage(textFieldB, AV_SET_RECT, (WPARAM)&skin.textfieldTo2, (LPARAM)&skin.front2);
		AveSendMessage(textFieldB, AV_UPDATE, 0L, 0L);
		AveUnlockControl(textFieldB);

		AveLockControl(staticFrom);
		AveSendMessage(staticFrom, AV_SET_FONT2, 0L, (LPARAM)&skin.comboboxFromFont);
		AveSendMessage(staticFrom, AV_SET_TEXTCOLOR, 0L, (LPARAM)skin.comboboxFromColor);
		AveSendMessage(staticFrom, AV_SET_SHADOWPROPERTIES, skin.comboboxFromDrawShadow, (LPARAM)&skin.comboboxFromShadow);
		AveSendMessage(staticFrom, AV_SET_RECT, (WPARAM)&skin.comboboxFrom, (LPARAM)&skin.front);
		AveSendMessage(staticFrom, AV_SET_TEXT, 0L, (WPARAM) codes[StringToWString(langFrom)].c_str());
		AveUnlockControl(staticFrom);

		AveLockControl(staticTo);
		AveSendMessage(staticTo, AV_SET_FONT2, 0L, (LPARAM)&skin.comboboxToFont);
		AveSendMessage(staticTo, AV_SET_TEXTCOLOR, FALSE, (LPARAM)skin.comboboxToColor);
		AveSendMessage(staticTo, AV_SET_SHADOWPROPERTIES, skin.comboboxToDrawShadow, (LPARAM)&skin.comboboxToShadow);
		AveSendMessage(staticTo, AV_SET_RECT, (WPARAM)&skin.comboboxTo, (LPARAM)&skin.front);
		AveSendMessage(staticTo, AV_SET_TEXT, 0L, (WPARAM) codes[StringToWString(langTo)].c_str());
		AveUnlockControl(staticTo);

		AveLockControl(barA);
		AveSendMessage(barA, AV_HIDE, 0L, 0L);
		AveSendMessage(barA, AV_SET_RECT, (WPARAM)&skin.scrollbarFrom, (LPARAM)&skin.front);
		AveSendMessage(textFieldA, AV_SET_SCROLLBAR, 0L, (LPARAM)barA);
		AveSendMessage(textFieldA, AV_SET_SCROLLBARCALLBACK, (WPARAM)this, (LPARAM)OnScrollBarNotifyA);
		AveUnlockControl(barA);

		AveLockControl(barB);
		AveSendMessage(barB, AV_HIDE, 0L, 0L);
		AveSendMessage(barB, AV_SET_RECT, (WPARAM)&skin.scrollbarTo, (LPARAM)&skin.front);
		AveSendMessage(textFieldB, AV_SET_SCROLLBAR, 0L, (LPARAM)barB);
		AveSendMessage(textFieldB, AV_SET_SCROLLBARCALLBACK, (WPARAM)this, (LPARAM)OnScrollBarNotifyB);
		AveUnlockControl(barB);
		AveUnlockContainer(container);
	}

	std::wstring CreateSkinnedPath(const WCHAR* file)
	{
		WCHAR path[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, path);
		PathAppendW(path, L"Skins");
		PathAppendW(path, skin.dir.c_str());
		PathAppendW(path, file);
		return path;
	}

	void SetImageFileToLayerByName(const char* name, const WCHAR* file, BOOL redraw = FALSE)
	{
		BOOL success(FALSE);
		UINT lid = DeskletLayerGetLayer(hwnd, name, &success);
		if(success)
			SetImageFileToLayer(lid, file, redraw);
	}

	void SetImageFileToLayer(UINT lid, const WCHAR* file, BOOL redraw = FALSE)
	{
		DeskletLayerSetImageFileW(hwnd, lid, redraw, CreateSkinnedPath(file).c_str());
	}

	void SetupBackgroundLayers()
	{

		UINT bgLayer = DeskletLayerAdd(hwnd, "bg");
		SetImageFileToLayer(bgLayer, L"frontside.png");


		UINT backSideLayer = DeskletLayerAdd(hwnd, "back");
		SetImageFileToLayer(backSideLayer, L"backside.png");
		DeskletLayerSetVisibility(hwnd, backSideLayer, FALSE, FALSE);

		UINT flipLayer = DeskletLayerAdd(hwnd, "flip");
		SetImageFileToLayer(flipLayer, L"flip.png");
		DeskletLayerSetVisibility(hwnd, flipLayer, FALSE, FALSE);
		DeskletLayerSetPlacement(hwnd, flipLayer, FALSE, TRUE, &skin.flippie, skin.front.cx, skin.front.cy);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, flipLayer, TRUE);

		UINT popupTopLayer = DeskletLayerAdd(hwnd, "popup_top");
		SetImageFileToLayer(popupTopLayer, L"popup_top.png");
		DeskletLayerSetPlacement(hwnd, popupTopLayer, FALSE, TRUE, &skin.comboboxFromBg, skin.front.cx, skin.front.cy);

		UINT popupBottomLayer = DeskletLayerAdd(hwnd, "popup_bottom");
		SetImageFileToLayer(popupBottomLayer, L"popup_bottom.png");
		DeskletLayerSetPlacement(hwnd, popupBottomLayer, FALSE, TRUE, &skin.comboboxToBg, skin.front.cx, skin.front.cy);

		UINT logoLayer = DeskletLayerAdd(hwnd, "back_logo");
		DeskletLayerSetVisibility(hwnd, logoLayer, FALSE, FALSE);
		DeskletLayerSetPlacement(hwnd, logoLayer, FALSE, TRUE, &skin.backLogo, skin.back.cx, skin.back.cy);
		DeskletLayerSetIsClickThrough(hwnd, logoLayer, TRUE);
		SetImageFileToLayer(logoLayer, L"logo.png");
	}

	void SetupControls()
	{

		resources = AveCreateResourcesW(CreateSkinnedPath(L"controls\\").c_str());
		container = AveCreateControlContainer(hwnd, resources);

		AveLockContainer(container);

		doneBut = AveCreateControl(container, "back_done", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, doneBut);

		textFieldA = AveCreateControl(container, "textA", AVE_TEXTFIELD_CONTROL, TEXTFIELD_NORETURNS, NULL);
		AveAddControlToContainer(container, textFieldA);

		textFieldB = AveCreateControl(container, "textB", AVE_TEXTFIELD_CONTROL, TEXTFIELD_NORETURNS, NULL);
		AveAddControlToContainer(container, textFieldB);

		staticFrom = AveCreateControl(container, "static_from", AVE_LABEL_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, staticFrom);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, AveSendMessage(staticFrom, AV_GET_LAYERID, 0L, 0L), TRUE);

		staticTo = AveCreateControl(container, "static_to", AVE_LABEL_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, staticTo);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, AveSendMessage(staticTo, AV_GET_LAYERID, 0L, 0L), TRUE);

		barA = AveCreateControl(container, "scroll_a", AVE_VERTICAL_SCROLLBAR_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, barA);


		barB = AveCreateControl(container, "scroll_b", AVE_VERTICAL_SCROLLBAR_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, barB);

		AveUnlockContainer(container);
	}

	static BOOL __stdcall OnScrollBarNotifyA(void* data, AVEHANDLE textField, BOOL visible, DWORD* reserved)
	{
		Translator* t = reinterpret_cast<Translator*>(data);
		if(NULL == t)
			return FALSE;

		BOOL dontUpdate = FALSE;

		t->barAVisible = visible;

		AveLockControl(t->barA);
		if(visible)
		{
			AveLockControl(textField);
			RECT textRectANew = t->skin.textfieldFrom2;
			AveSendMessage(t->barA, AV_SHOW, 0L, 0L);
			int scrollWidth = t->skin.scrollbarFrom.right - t->skin.scrollbarFrom.left;
			textRectANew.right -= (scrollWidth *2);
			AveSendMessage(textField, AV_SET_RECT, (WPARAM)&textRectANew, (LPARAM)&t->skin.front2);
			AveSendMessage(textField, AV_UPDATE, 0L, 0L);
			AveUnlockControl(textField);
		}
		else
		{
			AveLockControl(textField);
			RECT textRectANew = t->skin.textfieldFrom2;
			AveSendMessage(t->barA, AV_HIDE, 0L, 0L);
			AveSendMessage(textField, AV_SET_RECT, (WPARAM)&textRectANew, (LPARAM)&t->skin.front2);
			AveSendMessage(textField, AV_UPDATE, 0L, 0L);
			AveUnlockControl(textField);
		}

		AveUnlockControl(t->barA);

		return dontUpdate;
	}

	static BOOL __stdcall OnScrollBarNotifyB(void* data, AVEHANDLE textField, BOOL visible, DWORD* reserved)
	{
		Translator* t = reinterpret_cast<Translator*>(data);
		if(NULL == t)
			return FALSE;

		BOOL dontUpdate = FALSE;

		t->barBVisible = visible;

		AveLockControl(t->barB);
		if(visible)
		{
			AveLockControl(textField);
			RECT textRectANew = t->skin.textfieldTo2;
			AveSendMessage(t->barB, AV_SHOW, 0L, 0L);
			int scrollWidth = t->skin.scrollbarTo.right - t->skin.scrollbarTo.left;
			textRectANew.right -= (scrollWidth *2);
			AveSendMessage(textField, AV_SET_RECT, (WPARAM)&textRectANew, (LPARAM)&t->skin.front2);
			AveSendMessage(textField, AV_UPDATE, 0L, 0L);
			AveUnlockControl(textField);
		}
		else
		{
			AveLockControl(textField);
			RECT textRectANew = t->skin.textfieldTo2;
			AveSendMessage(t->barB, AV_HIDE, 0L, 0L);
			AveSendMessage(textField, AV_SET_RECT, (WPARAM)&textRectANew, (LPARAM)&t->skin.front2);
			AveSendMessage(textField, AV_UPDATE, 0L, 0L);
			AveUnlockControl(textField);
		}

		AveUnlockControl(t->barB);

		return dontUpdate;
	}

	static std::wstring AssembleURL(const std::wstring& from, const std::wstring& to, const std::wstring& text)
	{
		const std::wstring baseUrl = L"http://babelfish.altavista.com/tr";
		std::wstring extraUrl = L"?lp=" + from + L"_" + to + L"&tt=urltext&intl=0&doit=done&urltext=";

		return baseUrl + extraUrl + CAveHTTP::Encode(text);
	}

	static std::wstring GetTranslatedStringFromPage(const std::wstring& page, BOOL& success)
	{
		const std::wstring divOpen = L"<div style=padding:10px;>";
		const std::wstring divClose = L"</div>";

		int start = page.find(divOpen);
		if(start != std::wstring::npos)
		{
			int end = page.find(divClose, start + 1);
			if(end != std::wstring::npos)
			{
				start += divOpen.length();
				success = TRUE;
				return page.substr(start, end - start);
			}
		}

		success = FALSE;
		return L"";
	}

	static std::wstring GetTextFromTextField(AVEHANDLE textfield)
	{
		UINT length = AveSendMessage(textfield, AV_GET_TEXT_LENGTH, 0L, 0L);
		WCHAR* buf = new WCHAR[length+1];
		ZeroMemory(buf, (length+1) * sizeof(WCHAR));
		AveSendMessage(textfield, AV_GET_TEXT, (WPARAM)length+1, (LPARAM)buf);
		std::wstring ret(buf);
		delete[] buf;

		return ret;
	}

	void Translate()
	{

		std::wstring out;
		std::wstring from(StringToWString(langFrom));
		std::wstring to(StringToWString(langTo));

		CAveHTTP http(L"AveTranslator");
		BOOL res = http.RetrievePage(AssembleURL(from, to, GetTextFromTextField(textFieldA)), out);
		if(res != FALSE)
		{
			BOOL success = FALSE;
			AveSendMessage(textFieldB, AV_SET_TEXT, 0L, (LPARAM)GetTranslatedStringFromPage(out, success).c_str());

			// try again if it failed
			if(!success)
				AveSendMessage(textFieldB, AV_SET_TEXT, 0L, (LPARAM)GetTranslatedStringFromPage(out, success).c_str());
		}
		else
		{
			MessageBoxW(hwnd, L"Could not connect to the translator service.\nPlease make sure you have an internet connection.", L"AveTranslator", MB_ICONERROR);
		}
	}

	static BOOL __stdcall OnEnterPressed(void* data, AVEHANDLE textField)
	{
		Translator* t = reinterpret_cast<Translator*>(data);
		if(NULL == t)
			return FALSE;

		t->Translate();

		return TRUE;
	}

	void SetLangTo(const std::string& key)
	{
		if(langTo != key)
		{
			langTo = key;
			AveSendMessage(staticTo, AV_SET_TEXT, 0L, (LPARAM)codes[StringToWString(langTo)].c_str());

			if(GetTextFromTextField(textFieldB).length() > 0)
				Translate();
		}
	}

	void SetLangFrom(const std::string& key)
	{
		if(langFrom != key)
		{
			langFrom = key;

			BOOL locked = FALSE;
			std::list<std::wstring>& possibsForFrom = possibs[StringToWString(langFrom)];
			if(std::find(possibsForFrom.begin(), possibsForFrom.end(), StringToWString(langTo)) == possibsForFrom.end())
			{
				locked = TRUE;
				AveLockContainer(container);
				langTo= WStringToString(*possibs[StringToWString(langFrom)].begin());
				AveSendMessage(staticTo, AV_SET_TEXT, 0L, (LPARAM)codes[StringToWString(langTo)].c_str());
			}

			if(locked)
				AveUnlockContainer(container);

			AveSendMessage(staticFrom, AV_SET_TEXT, 0L, (LPARAM)codes[StringToWString(langFrom)].c_str());
		}
	}

	static BOOL __stdcall OnDoneButtonClick(void* ptr, AVEHANDLE but, const POINT* pt, const SIZE* s, DWORD keys)
	{
		Translator* t = reinterpret_cast<Translator*>(ptr);
		t->Flip();

		return TRUE;
	}

	void Flip()
	{
		FlipParams params = {0};
		params.flipToBack = !isInBackMode;
		params.useLayers = TRUE;

		HWND hwndDesklet = hwnd;

		UINT frontLayers[] = {
								DeskletLayerGetLayer(hwndDesklet, "bg", NULL),
								DeskletLayerGetLayer(hwndDesklet, "static_from", NULL),
								DeskletLayerGetLayer(hwndDesklet, "static_to", NULL),
								DeskletLayerGetLayer(hwndDesklet, "textA", NULL),
								DeskletLayerGetLayer(hwndDesklet, "textB", NULL),
								DeskletLayerGetLayer(hwndDesklet, "popup_top", NULL),
								DeskletLayerGetLayer(hwndDesklet, "popup_bottom", NULL),
								(UINT)-1,
								(UINT)-1
							};

		UINT lessVisible = 2;
		if(barAVisible)
		{
			lessVisible -= 1;
			frontLayers[sizeof(frontLayers) / sizeof(frontLayers[0]) - 2] = DeskletLayerGetLayer(hwndDesklet, "scroll_a", NULL);
		}

		if(barBVisible)
		{
			lessVisible -= 1;
			frontLayers[sizeof(frontLayers) / sizeof(frontLayers[0]) - 1] = DeskletLayerGetLayer(hwndDesklet, "scroll_b", NULL);
		}

		UINT backLayers[]  = { 
								DeskletLayerGetLayer(hwndDesklet, "back", NULL),
								DeskletLayerGetLayer(hwndDesklet, "back_done", NULL),
								DeskletLayerGetLayer(hwndDesklet, "back_logo", NULL)
							};


		params.newDeskletSize = &skin.back;
		params.oldDeskletSize = &skin.front;
		params.oldLayersVisible = frontLayers;
		params.newLayersVisible = backLayers;
		params.oldLayersVisibleCount = sizeof(frontLayers) / sizeof(frontLayers[0]) - lessVisible;
		params.newLayersVisibleCount = sizeof(backLayers) / sizeof(backLayers[0]);
		if(isInBackMode)
		{
			params.newDeskletSize = &skin.front;
			params.oldDeskletSize = &skin.back;

			params.oldLayersVisible = backLayers;
			params.newLayersVisible = frontLayers;

			params.oldLayersVisibleCount = sizeof(backLayers) / sizeof(backLayers[0]);
			params.newLayersVisibleCount = sizeof(frontLayers) / sizeof(frontLayers[0]) - lessVisible;
		}

		flipVisible = FALSE;
		DeskletFlip(hwnd, &params);

		isInBackMode = !isInBackMode;
	}
};


void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"Translator");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Translates");
	*iVersion = 130;
}


Translator* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	Translator* t = new Translator(hwndDesklet, hInstanceDll, group, ini);
	if(!ini || !group)
	{
		DeskletSetLabel(hwndDesklet, "");
		DeskletSetSubLabel(hwndDesklet, "");
	}
	else
	{
		char codeFrom[10] = {0};
		char codeTo[10] = {0};
		
		GetPrivateProfileString(group, "fromCode", t->langFrom.c_str(), codeFrom, 10, ini);
		GetPrivateProfileString(group, "toCode", t->langTo.c_str(), codeTo, 10, ini);

		t->SetLangTo(codeTo);
		t->SetLangFrom(codeFrom);
	}

	return t;
}

Translator* __stdcall OnCreateFromSkinW(HWND hwnd,HINSTANCE hInstance, CSkinEnumerator* se, SkinInfo* skinInfo)
{
	Translator* desklet = OnCreate(hwnd,hInstance,NULL,NULL);
	if(se != NULL)
		desklet->LoadSkin(se->getValue(skinInfo->wKey).c_str());
	else
		desklet->LoadSkin(skinInfo->wKey);

	desklet->ApplySkin();
	return desklet;
}

void __stdcall OnDestroy(Translator* t, HWND hwndDesklet)
{
	delete t;
}


BOOL __stdcall OnLayerClick(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseClick(t->container,layerId, layerName, pt, s, keys))
		return TRUE;

	if(strcmp(layerName, "flip") == 0)
	{
		t->Flip();
		return TRUE;
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseDown(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseDown(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	if(strcmp(layerName, "flip") == 0)
	{
		return TRUE;
	}
	else if(strcmp(layerName, "static_from") == 0 || strcmp(layerName, "popup_top") == 0)
	{
		int selIndex = -1;
		WCHAR** strs = new WCHAR*[t->codes.size()];
		int i = 0;
		for(std::map<std::wstring, std::wstring>::iterator iter = t->codes.begin(); iter != t->codes.end(); ++iter)
		{
			if(iter->first == StringToWString(t->langFrom))
				selIndex = i;

			strs[i++] = (WCHAR*)iter->second.c_str();
		}
				
		MenuData data = {0};
		data.selected = selIndex;
		data.count = t->codes.size();
		data.items = strs;

		char resourcesPath[MAX_PATH] = {0};
		strcpy(resourcesPath, WStringToString(t->CreateSkinnedPath(L"controls\\menu\\")).c_str());

		POINT pt = {0};
		RECT r = {0};
		GetWindowRect(t->hwnd, &r);
		pt.x = r.left + t->skin.comboboxFrom.left;
		pt.y = r.top  + t->skin.comboboxFrom.top;

		BOOL success(FALSE);
		UINT popupTopLayer = DeskletLayerGetLayer(t->hwnd, "popup_top", &success);
		if(success)
			t->SetImageFileToLayer(popupTopLayer, L"popup_top_pressed.png", TRUE);

		DoAvePopupMenu(t->hwnd, WM_MENU_LANG_FROM, &data, resourcesPath, &pt);

		delete[] strs;

		return TRUE;
	}
	else if(strcmp(layerName, "static_to") == 0 || strcmp(layerName, "popup_bottom") == 0)
	{
		std::list<std::wstring>& possibsForFrom = t->possibs[StringToWString(t->langFrom)];

		int selIndex = -1;
		WCHAR** strs = new WCHAR*[possibsForFrom.size()];
		int i = 0;
		for(std::list<std::wstring>::iterator iter = possibsForFrom.begin(); iter != possibsForFrom.end(); ++iter)
		{
			std::wstring& code = *iter;
			if(code == StringToWString(t->langTo))
				selIndex = i;

			strs[i++] = (WCHAR*)t->codes[code].c_str();
		}
				
		MenuData data = {0};
		data.selected = selIndex;
		data.count = possibsForFrom.size();
		data.items = strs;

		char resourcesPath[MAX_PATH] = {0};
		strcpy(resourcesPath, WStringToString(t->CreateSkinnedPath(L"controls\\menu\\")).c_str());
		//DeskletGetRelativeFolder(t->hwnd, resourcesPath);
		//strcat(resourcesPath, "images\\controls\\menu\\");

		POINT pt = {0};
		RECT r = {0};
		GetWindowRect(t->hwnd, &r);
		pt.x = r.left + t->skin.comboboxTo.left;
		pt.y = r.top  + t->skin.comboboxTo.top;

		BOOL success(FALSE);
		UINT popupBottomLayer = DeskletLayerGetLayer(t->hwnd, "popup_bottom", &success);
		if(success)
			t->SetImageFileToLayer(popupBottomLayer, L"popup_bottom_pressed.png", TRUE);

		DoAvePopupMenu(t->hwnd, WM_MENU_LANG_TO, &data, resourcesPath, &pt);

		delete[] strs;

		return TRUE;
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelForward(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(AveForwardMouseWheelForward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return FALSE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelBackward(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(AveForwardMouseWheelBackward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return FALSE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseUp(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseUp(t->container, layerId, layerName, pt, s, keys))
		return FALSE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseOn(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(AveForwardMouseOn(t->container, layerId, layerName, pt, s))
		return TRUE;

	if(strcmp(layerName, "flip") == 0)
	{
		HCURSOR hand = LoadCursor(NULL,  IDC_HAND);
		t->oldCursor = DeskletSetCursor(t->hwnd, hand);
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseMove(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(AveForwardMouseMove(t->container, layerId, layerName, pt, s))
		return TRUE;

	return FALSE;
}


BOOL __stdcall OnLayerMouseOut(Translator* t, UINT layerId, const char* layerName)
{
	if(AveForwardMouseOut(t->container, layerId, layerName))
		return TRUE;

	if(strcmp(layerName, "flip") == 0)
	{
		DeskletSetCursor(t->hwnd, t->oldCursor);
	}

	return FALSE;
}


void __stdcall OnSave(Translator* t, const char* ini, const char* group, BOOL bIsForExport)
{
	WritePrivateProfileString(group, "fromCode", t->langFrom.c_str(), ini);
	WritePrivateProfileString(group, "toCode", t->langTo.c_str(), ini);
	WritePrivateProfileStringW(StringToWString(group).c_str(), L"SkinDir", t->skin.dir.c_str(), StringToWString(ini).c_str());
}

void __stdcall OnProcessMessage(Translator* t, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!t)
		return;

	if(WM_MENU_LANG_FROM == msg)
	{
		const WCHAR* selItem = reinterpret_cast<const WCHAR*>(lParam);
		if(selItem != NULL)
		{
			std::wstring sel(selItem);
			std::wstring def(L"en");
			std::wstring key = find_key_for_value<std::wstring, std::wstring>(t->codes, sel, def);

			BOOL success(FALSE);
			UINT popupTopLayer = DeskletLayerGetLayer(hwnd, "popup_top", &success);
			if(success)
				t->SetImageFileToLayer(popupTopLayer, L"popup_top.png", t->langFrom == WStringToString(key));


			t->SetLangFrom(WStringToString(key));
			
		}
	}
	else if(WM_MENU_LANG_TO == msg)
	{
		const WCHAR* selItem = reinterpret_cast<const WCHAR*>(lParam);
		if(selItem != NULL)
		{
			std::wstring sel(selItem);
			std::wstring def(L"en");
			std::wstring key = find_key_for_value<std::wstring, std::wstring>(t->codes, sel, def);

			BOOL success(FALSE);
			UINT popupBottomLayer = DeskletLayerGetLayer(hwnd, "popup_bottom", &success);
			if(success)
				t->SetImageFileToLayer(popupBottomLayer, L"popup_bottom.png", t->langTo == WStringToString(key));

			t->SetLangTo(WStringToString(key));
			
		}
	}
	else if(WM_TIMER == msg && FLIP_FADE_TIMER_ID == wParam)
	{
		// this timer controls the flippy fade-in aniamtion
		UINT flip_rollie = DeskletLayerGetLayer(t->hwnd, "flip", NULL);

		// calculate the new alpha level for the flippy based
		// on the fade direction (in/out)
		if(t->flipIsFadingIn)
			t->flipAlpha += flipFadeDiff;
		else
			t->flipAlpha -= flipFadeDiff;

		// set the layers alpha
		if(t->flipAlpha >= 0 && t->flipAlpha <= flipFadeMax)
			DeskletLayerSetAlpha(t->hwnd, flip_rollie, TRUE, t->flipAlpha);

		// we are done when: fading in and we reached the maximum OR
		// when fading out and we got underneath the treshold.
		// if we are done, kill the animation timer.
		if(t->flipIsFadingIn && t->flipAlpha >= flipFadeMax)
			KillTimer(hwnd, FLIP_FADE_TIMER_ID);
		else if(!t->flipIsFadingIn && t->flipAlpha < flipFadeDiff)
		{
			// if fading out, hide the flippie
			DeskletLayerSetVisibility(t->hwnd, flip_rollie, TRUE, FALSE);
			KillTimer(hwnd, FLIP_FADE_TIMER_ID);
		}
	}
	else if(t->container != NULL)
	{
		if(AveForwardMessage(t->container,msg, wParam, lParam))
			return;
	}
}

BOOL __stdcall OnMouseMove(Translator* t, int x, int y)
{
	/*
	if(!t->flipVisible && !t->isInBackMode)
	{
		t->flipVisible = TRUE;
		DeskletLayerSetVisibility(t->hwnd, DeskletLayerGetLayer(t->hwnd, "flip", NULL), TRUE, TRUE);
	}*/

	// if the flippy is not visible and we are not in backmode (when the flippie
	// shouldn't be visible), start to fade the flippy in
	if(!t->isFlipVisible && !t->isInBackMode)
	{
		UINT flip_rollie = DeskletLayerGetLayer(t->hwnd, "flip", NULL);
		DeskletLayerSetVisibility(t->hwnd, flip_rollie, FALSE, TRUE);
		t->isFlipVisible = TRUE;

		t->flipIsFadingIn = TRUE;
		SetTimer(t->hwnd, FLIP_FADE_TIMER_ID, FLIP_TIMER_INTERVAL, NULL);
	}

	return TRUE;
}

BOOL __stdcall OnMouseOut(Translator* t)
{
	/*
	if(t->flipVisible)
	{
		t->flipVisible = FALSE;
		DeskletLayerSetVisibility(t->hwnd, DeskletLayerGetLayer(t->hwnd, "flip", NULL), TRUE, FALSE);
	}*/

	// if the flippie is visible, but the mouse is moved
	// from the widget, fade the flippy out
	if(t->isFlipVisible)
	{
		t->isFlipVisible = FALSE;
		t->flipIsFadingIn = FALSE;
		SetTimer(t->hwnd, FLIP_FADE_TIMER_ID, FLIP_TIMER_INTERVAL, NULL);
	}

	return TRUE;
}

BOOL __stdcall OnLayerRightClick(Translator* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// when we receive a layer right click, we forward this mouse click to the container,
	// which, in turns, will forward it to the right control. If the container or its controlled
	// handled the mouse rightclick, TRUE has been returned. Otherwise, FALSE.
	//if(AveForwardMouseRightClick(t->container, layerId, layerName, pt, s, keys))
	//	return TRUE;		
	if(AveForwardMouseRightClick(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	// let's know we don't handle this.
	return FALSE;
}