//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.


// STICKYNOTES WIDGET.
// by Andreas Verhoeven, 2005.
// Allows one to draw notes

// STATUS:
// + completely done


// DeskletSDK for Desklet* functions.
#include "../../../DeskletSDK.h"

// AveControls specific includes
#include "../../../aveconstants.h"
#include "../../../AveFlags.h"
#include "../../../AveControls.h"
#pragma comment(lib, "../../../AveControls.lib")

// some C++ includes
#pragma warning (disable : 4786) // disable STL truncated identifier warning, because we don't care about it.
#include <cstring>	// for std::basic_string<T> operator==()
#include <string>	// for std::wstring
#include <sstream>	// for std::wstringstream
#include <shlwapi.h>// for Path* functions
#include <objbase.h>// for CoCreateGuid
#include <shlobj.h>	// for DROPFILES
#include <vector>	// for std::vector<T>
#include <fstream>	// for ifstream
#include <map>		// for std::map<K,V>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ole32.lib")

// fade animation constants
const int FLIP_FADE_TIMER_ID			= 2393;
const int FLIP_TIMER_INTERVAL			= 10;
const int flipFadeDiff					= 20;
const int flipFadeMax					= 255;


// This desklet is fairly simple and doesn't have support for skins.
// Therefore, all sizes are hardcoded.
SIZE size		= {239, 217};
SIZE backSize	= {239, 217};
RECT flippieRect = {213, 181, 213+13, 181+13};
RECT doneRect   = {155, 155, 155 + 55, 155 + 25};
RECT textAreaRect   = { 15, 15, 15 + 200, 15 + 183 };
RECT fontRect   = { 25, 115, 25 + 125, 115 + 25 };
RECT sizeRect   = { 160, 115, 160 + 50, 115 + 25 };
SIZE backColorSize = {2, 24};
POINT closePoint = {12, 4};

// some helper functions
//--------------------------------------------------------------------------------------------
template<typename T> T FromStringW(const std::wstring& str)
{
	std::wstringstream ws;
	ws << str;
	T t;
	ws >> t;
	return t;
}
//--------------------------------------------------------------------------------------------
template<typename T> std::wstring ToStringW(const T& t)
{
	std::wstringstream ws;
	ws << t;
	return ws.str();
}
//--------------------------------------------------------------------------------------------
std::wstring StringToWString(const std::string& str)
{
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
	std::string ret;

	char* tmp = new char[ wstr.length() + 2];
	memset(static_cast<void*>(tmp),0,wstr.length()+2);

	WideCharToMultiByte(GetACP(),0,wstr.c_str(),-1,tmp,wstr.length()+1,NULL,NULL );
	ret = tmp;
	delete[] tmp;

	return ret;
}
//--------------------------------------------------------------------------------------------
RECT MultiplyRect(const RECT& r, int fac)
{
	RECT ret = r;
	ret.left *= fac;
	ret.right *= fac;
	ret.top *= fac;
	ret.bottom *= fac;

	return ret;
}
//--------------------------------------------------------------------------------------------
SIZE MultiplySize(const SIZE& s, int fac)
{
	SIZE ret = s;
	ret.cx *= fac;
	ret.cy *= fac;

	return ret;
}
//--------------------------------------------------------------------------------------------


// This desklet uses a class to store it's data.
class Widget
{
public:
	// desklet handle (hwnd) and DLL handle (dll). Given by AveDesk to this desklet
	// when created and needed for the callback functions.
	HWND hwnd;
	HINSTANCE dll;

	// control related handles.
	AVEHANDLE resources;
	AVEHANDLE container;
	AVEHANDLE comboSize;
	AVEHANDLE comboFont;
	AVEHANDLE textArea;

	// some variables used for the flippie and cursors.
	BOOL isFlipVisible;
	UINT flipAlpha;
	BOOL flipIsFadingIn;BOOL isInBackMode;
	HCURSOR oldCursor;
	BOOL isOverBackColor;

	// variable to keep track if we use supersampling for the textArea.
	BOOL useSuperSampling;

	// some lists that hold the available fontnames, fontsizes and backcolors.
	std::vector<std::wstring> fonts;
	std::vector<std::wstring> sizes;
	std::map<std::wstring, POINT> backColors;

	// variables that keep track of the selected fontname, fontsize and background
	UINT selFontSize;
	std::wstring selFontName;
	std::wstring selBackColor;
	std::wstring text;

	// variables used for drag&drop
	BOOL acceptDrop;


	// The constructor is where we initialize this widget.
	// First, all variables are initialized to a default value in the initializer-list.
	Widget(HWND hwnd, HINSTANCE dll, const char* group, const char* ini) : 
		hwnd(hwnd), dll(dll), 
		isFlipVisible(FALSE),
		flipAlpha(0),
		isInBackMode(FALSE),
		comboSize(NULL),
		comboFont(NULL),
		textArea(NULL),
		selFontSize(20),
		selFontName(L"Comic Sans MS"),
		useSuperSampling(TRUE),
		selBackColor(L"yellow.png"),
		isOverBackColor(FALSE),
		text(L"Enter some text..."),
		acceptDrop(FALSE)
	{

		// load the font and backcolor data into the structures.
		PopulateFontList();
		PopulateSizesList();
		PopulateBackColorsList();

		// set the closepoint for this desklet.
		DeskletSetClosePoint(hwnd, &closePoint, size.cx, size.cy);
			
		DeskletActivateExtendedDragDrop(hwnd);
		FORMATETC f = {0};
		f.cfFormat = CF_UNICODETEXT;
		f.dwAspect = DVASPECT_CONTENT;
		f.lindex = -1;
		f.ptd = 0;
		f.tymed = TYMED_HGLOBAL;
		DeskletAddDragDropFormat(hwnd,&f);

		if(group != NULL && ini != NULL)
		{
			std::wstring wsGroup = StringToWString(group);
			std::wstring wsIni = StringToWString(ini);

			useSuperSampling = GetPrivateProfileIntW(wsGroup.c_str(), L"UseSuperSampling", useSuperSampling, wsIni.c_str());
			selFontSize   = GetPrivateProfileIntW(wsGroup.c_str(), L"FontSize", selFontSize, wsIni.c_str());
			DWORD textLen = GetPrivateProfileIntW(wsGroup.c_str(), L"TextLen", 0, wsIni.c_str());
			WCHAR* buf = new WCHAR[textLen + 2]; // catch possible off-by-one errors
			buf[0] = L'\0';
			GetPrivateProfileStringW(wsGroup.c_str(), L"Text", text.c_str(), buf, textLen + 1, wsIni.c_str());
			text = buf;
			delete[] buf;
			for(UINT i=0;i<text.length();++i)
			{
				if(text[i]==wchar_t(180))text[i]=L'\n';
				else if(text[i]==wchar_t(181))text[i]=L'\r';
				else if(text[i]==wchar_t(182))text[i]=L'=';
			}

			WCHAR wFont[MAX_PATH] = {0};
			GetPrivateProfileStringW(wsGroup.c_str(), L"FontName", selFontName.c_str(), wFont, MAX_PATH, wsIni.c_str());
			selFontName = wFont;

			WCHAR wBackColor[MAX_PATH] = {0};
			GetPrivateProfileStringW(wsGroup.c_str(), L"BackColor", selBackColor.c_str(), wBackColor, MAX_PATH, wsIni.c_str());
			selBackColor = wBackColor;
		}		

		// first clear any possible layers, then
		// setup OUR background layers,
		// and finally, setup the controls.
		ClearAllLayers();
		SetupBackgroundLayers();
		SetupControls();
	}


	~Widget()
	{
		// let's not forget to clean up our own mess....
		AveFreeControlContainer(container);
		AveFreeResources(resources);

	}

	void PopulateBackColorsList()
	{
		// first row 
		POINT pYellow = {32, 26};
		backColors[L"yellow.png"] = pYellow;

		POINT pOrange = {68, 26};
		backColors[L"lime.png"] = pOrange;

		POINT pGreen1 = {106, 26};
		backColors[L"aqua.png"] = pGreen1;

		POINT pGreen2 = {142, 26};
		backColors[L"blue.png"] = pGreen2;

		POINT pBlue2 = {179, 26};
		backColors[L"purple.png"] = pBlue2;

		// second row
		POINT pBlue1 = {32, 62};
		backColors[L"lila.png"] = pBlue1;

		POINT pPurple1 = {68, 62};
		backColors[L"red.png"] = pPurple1;

		POINT pPink = {106, 62};
		backColors[L"orange.png"] = pPink;

		POINT pWhite = {142, 62};
		backColors[L"gray.png"] = pWhite;

		POINT pGrey = {179, 62};
		backColors[L"white.png"] = pGrey;
	}
	
	std::wstring GetBackColorAtPlace(const POINT* pt)
	{
		const int width = 27;
		const int height = 27;

		for(std::map<std::wstring, POINT>::iterator iter = backColors.begin(); iter != backColors.end(); ++iter)
		{
			POINT p = iter->second;
			if(pt->x >= p.x && pt->y >= p.y &&
				pt->x < p.x + width && pt->y < p.y + height)
			{
				return iter->first;
			}
		}

		return L"";
	}

	void PopulateFontList()
	{
		char fontsFile[MAX_PATH] = {0};
		GetDLLPath(dll, fontsFile, MAX_PATH);
		strcat_s(fontsFile, MAX_PATH, "fonts.txt");

		std::wifstream f(fontsFile);
		std::wstring line;
		while(std::getline(f, line))
			fonts.push_back(line);

		if(fonts.size() == 0)
			fonts.push_back(L"Lucida Sans");
	}

	void PopulateSizesList()
	{
		char sizesPath[MAX_PATH] = {0};
		GetDLLPath(dll, sizesPath, MAX_PATH);
		strcat_s(sizesPath, MAX_PATH, "sizes.txt");

		std::wifstream f(sizesPath);
		std::wstring line;
		while(std::getline(f, line))
			sizes.push_back(line);

		if(sizes.size() == 0)
			sizes.push_back(L"16");
	}


	SIZE GetSize()
	{
		return size;
	}

	void ClearAllLayers()
	{
		// this function deletes all layers from the desklet.

		while(DeskletLayerCount(hwnd) > 0)
			DeskletLayerRemove(hwnd, 0, FALSE);
	}


	std::wstring CreateSkinnedPath(const WCHAR* file)
	{
		WCHAR path[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, path);
		// not using a skin currently, so use the Images folder instead.
		PathAppendW(path, L"Images");
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

	std::wstring GetCurrentColorFile()
	{
		return L"yellow.png";
	}

	UINT GetSelFontSize()
	{
		return selFontSize;
	}

	std::wstring GetSelFontName()
	{
		return selFontName;
	}

	std::wstring GetSelBackColor()
	{
		return selBackColor;
	}

	void UpdateCheckPlacement(BOOL redraw = FALSE)
	{
		BOOL success(FALSE);
		UINT check = DeskletLayerGetLayer(hwnd, "back_check", &success);
		if(!success)
			return;

		const int checkWidth = 18;
		const int checkHeight = 19;
		POINT pt = backColors[GetSelBackColor()];
		pt.x += 4;
		pt.y += 5;
		RECT r = {pt.x, pt.y, pt.x + checkWidth, pt.y + checkHeight};
		DeskletLayerSetPlacement(hwnd, check, redraw, TRUE, &r, backSize.cx, backSize.cy);
	}

	std::wstring GetTextFromArea()
	{
		int textLen = AveSendMessage(textArea, AV_GET_TEXT_LENGTH, 0L, 0L);
		textLen += 2;// zero-terminator and catch possible off-by-one errors
		WCHAR* buf = new WCHAR[textLen];
		buf[0] = L'\0';
		AveSendMessage(textArea, AV_GET_TEXT, textLen, (LPARAM)buf);
		text = buf;
		delete[] buf;

		return text;
	}

	void SetupBackgroundLayers()
	{
		// this function sets up the background layers.

		// first lock the desklet for invalidating.
		DeskletLockInvalidating(hwnd);

		UINT front  = DeskletLayerAdd(hwnd, "frontside");
		SetImageFileToLayer(front, GetSelBackColor().c_str());

		UINT back  = DeskletLayerAdd(hwnd, "backside");
		SetImageFileToLayer(back, L"backside.png");
		DeskletLayerSetVisibility(hwnd, back, FALSE, FALSE);

		UINT flipLayer = DeskletLayerAdd(hwnd, "flip");
		SetImageFileToLayer(flipLayer, L"flip.png");
		DeskletLayerSetVisibility(hwnd, flipLayer, FALSE, FALSE);
		DeskletLayerSetPlacement(hwnd, flipLayer, FALSE, TRUE, &flippieRect, size.cx, size.cy);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, flipLayer, TRUE);

		UINT checkLayer = DeskletLayerAdd(hwnd, "back_check");
		SetImageFileToLayer(checkLayer, L"check.png");
		DeskletLayerSetVisibility(hwnd, checkLayer, FALSE, FALSE);
		DeskletLayerSetIsClickThrough(hwnd, checkLayer, TRUE);
		UpdateCheckPlacement();

		// unlock the desklet for invalidating.
		DeskletUnlockInvalidating(hwnd, FALSE);
	}

	static BOOL __stdcall OnSizeSelChange(Widget* w, AVEHANDLE control, INT* newSel)
	{
		// reacts to changes in the sizes combo
		UINT selVal = (UINT)(*newSel);
		if(selVal >= 0 && selVal < w->sizes.size())
		{
			w->selFontSize = FromStringW<UINT>(w->sizes[selVal]);
			AVE_FONT f = w->GetTextAreaFont();
			AveSendMessage(w->textArea, AV_SET_FONT, 0L, (LPARAM)&f);
			AveSendMessage(w->textArea, AV_UPDATE, 0L, 0L);
			
		}
		return TRUE;
	}

	static BOOL __stdcall OnFontSelChange(Widget* w, AVEHANDLE control, INT* newSel)
	{
		// reacts to changes in the fonts combo
		UINT selVal = (UINT)(*newSel);
		if(selVal >= 0 && selVal < w->fonts.size())
		{
			w->selFontName = w->fonts[selVal];
			AVE_FONT f = w->GetTextAreaFont();
			AveSendMessage(w->textArea, AV_SET_FONT, 0L, (LPARAM)&f);
			AveSendMessage(w->textArea, AV_UPDATE, 0L, 0L);
			
		}
		return TRUE;
	}




	static BOOL __stdcall OnDoneButtonClick(Widget* w, AVEHANDLE but, const POINT* pt, const SIZE* s, DWORD keys)
	{
		w->Flip();
		return TRUE;
	}

	AVE_FONT GetTextAreaFont()
	{
		AVE_FONT f = {0};
		wcscpy_s(f.faceName, MAX_PATH, GetSelFontName().c_str());
		f.style = FontStyleBold;
		if(useSuperSampling)
			f.size = (UINT)(((FLOAT)GetSelFontSize()) *1.5f);
		else
			f.size = GetSelFontSize();

		return f;
	}

	void SetupControls()
	{

		// create some resources which we'll use to base the container on.
		// An container holds and manages the controls we create.
		// The resources are used to tell all the controls based on this container
		// where the necessary graphics are located. The resourceshandle could
		// also perform caching if necessary.
		resources = AveCreateResourcesW(CreateSkinnedPath(L"controls\\").c_str());
		container = AveCreateControlContainer(hwnd, resources);

		// we now lock the container so we can do a batch update of all the controls it holds.
		AveLockContainer(container);

		textArea = AveCreateControl(container, "text_area", AVE_TEXTFIELD_CONTROL, 0L, NULL);
		AveLockControl(textArea);
		AVE_FONT f = GetTextAreaFont();
		AveSendMessage(textArea, AV_SET_FONT, 0L, (LPARAM)&f);
		AveSendMessage(textArea, AV_SET_TEXTCOLOR, 0L, (LPARAM)Color(245,0,0,0).GetValue());
		RECT r2 = MultiplyRect(textAreaRect, 2);
		SIZE s2 = MultiplySize(size, 2);
		AveSendMessage(textArea, AV_SET_RECT, (WPARAM)&r2, (LPARAM)&s2);
		AveSendMessage(textArea, AV_SET_TEXT, 0L, (LPARAM)text.c_str());
		AveUnlockControl(textArea);
		AveAddControlToContainer(container, textArea);

		AVEHANDLE doneBut = AveCreateControl(container, "back_done", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveLockControl(doneBut);
		if(!isInBackMode)
			AveSendMessage(doneBut, AV_HIDE, 0L, 0L);
		AveSendMessage(doneBut,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"done.png");
		AveSendMessage(doneBut,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"done_pressed.png");
		AveSendMessage(doneBut, AV_SET_RETRIEVEIMAGESFROMRES, TRUE, 0L);
		AveSendMessage(doneBut, AV_SET_RECT, (WPARAM)&doneRect, (LPARAM)&backSize);
		AveSendMessage(doneBut, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnDoneButtonClick);
		AveSendMessage(doneBut, AV_UPDATE, 0L, 0L);
		AveUnlockControl(doneBut);
		AveAddControlToContainer(container, doneBut);

		comboSize = AveCreateControl(container, "back_combosize", AVE_DROPDOWNLIST_CONTROL, 0L, NULL);
		AveLockControl(comboSize);
		AveSendMessage(comboSize, AV_DROPLIST_SET_ONSELCHANGECALLBACK, (WPARAM)this, (LPARAM)OnSizeSelChange);
		AVE_FONT2 f2 = {0};
		wcscpy_s(f2.faceName, MAX_PATH, L"Lucida Sans Unicode");
		f2.style = FontStyleBold;
		f2.size = 13;
		AveSendMessage(comboSize, AV_SET_FONT2, 0L, (LPARAM)&f2);
		AveSendMessage(comboSize, AV_SET_TEXTCOLOR, 0L, (LPARAM)Color(255,255,255,255).GetValue());
		AveSendMessage(comboSize, AV_SET_RECT, (WPARAM)&sizeRect, (LPARAM)&backSize);
		for(std::vector<std::wstring>::iterator iter = sizes.begin(); iter != sizes.end(); ++iter)
			AveSendMessage(comboSize, AV_DROPLIST_ADDITEM, 0L, (LPARAM)(*iter).c_str());
		AveSendMessage(comboSize, AV_DROPLIST_SET_SEL, AveSendMessage(comboSize, AV_DROPLIST_FINDITEM, 0L, (LPARAM)ToStringW(GetSelFontSize()).c_str()), 0L);
		AveSendMessage(comboSize, AV_UPDATE, 0L, 0L);
		AveUnlockControl(comboSize);
		AveAddControlToContainer(container, comboSize);
		DeskletLayerSetVisibility(hwnd, AveSendMessage(comboSize, AV_GET_LAYERID, 0L, 0L), FALSE, FALSE);

		comboFont = AveCreateControl(container, "back_combofont", AVE_DROPDOWNLIST_CONTROL, 0L, NULL);
		AveLockControl(comboFont);
		AveSendMessage(comboFont, AV_DROPLIST_SET_ONSELCHANGECALLBACK, (WPARAM)this, (LPARAM)OnFontSelChange);
		AveSendMessage(comboFont, AV_SET_FONT2, 0L, (LPARAM)&f2);
		AveSendMessage(comboFont, AV_SET_TEXTCOLOR, 0L, (LPARAM)Color(255,255,255,255).GetValue());
		AveSendMessage(comboFont, AV_SET_RECT, (WPARAM)&fontRect, (LPARAM)&backSize);
		for(std::vector<std::wstring>::iterator iter2 = fonts.begin(); iter2 != fonts.end(); ++iter2)
			AveSendMessage(comboFont, AV_DROPLIST_ADDITEM, 0L, (LPARAM)(*iter2).c_str());
		AveSendMessage(comboFont, AV_DROPLIST_SET_SEL, AveSendMessage(comboFont, AV_DROPLIST_FINDITEM, 0L, (LPARAM)GetSelFontName().c_str()), 0L);
		AveSendMessage(comboFont, AV_UPDATE, 0L, 0L);
		AveUnlockControl(comboFont);
		AveAddControlToContainer(container, comboFont);
		DeskletLayerSetVisibility(hwnd, AveSendMessage(comboFont, AV_GET_LAYERID, 0L, 0L), FALSE, FALSE);

		// and finally, unlock the container.
		AveUnlockContainer(container);
	}

	void Flip()
	{
		FlipParams params = {0};
		params.flipToBack = !isInBackMode;
		params.useLayers = TRUE;

		HWND hwndDesklet = hwnd;

		std::vector<UINT> frontLayers;
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "frontside", NULL));
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "text_area", NULL));

		std::vector<UINT> backLayers;
		backLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "backside", NULL));
		backLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "back_done", NULL));
		backLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "back_combosize", NULL));
		backLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "back_combofont", NULL));
		backLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "back_check", NULL));

		params.oldLayersVisible = &frontLayers[0];
		params.newLayersVisible = &backLayers[0];
		params.oldLayersVisibleCount = frontLayers.size();
		params.newLayersVisibleCount = backLayers.size();
		if(isInBackMode)
		{
			params.oldLayersVisible = &backLayers[0];
			params.newLayersVisible = &frontLayers[0];

			params.oldLayersVisibleCount = backLayers.size();
			params.newLayersVisibleCount = frontLayers.size();
		}

		isFlipVisible = FALSE;
		DeskletFlip(hwnd, &params);

		isInBackMode = !isInBackMode;
	}
};



void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	// give the world some information about this widget.
	strcpy_s(szName, 1024, "StickyNotes");
	strcpy_s(szAuthor, 1024, "Andreas Verhoeven");
	strcpy_s(szNotes, 1024, "Add little sticky notes on your desktop.");
	*iVersion = 130;
}


Widget* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	Widget* w = new Widget(hwndDesklet, hInstanceDll, group, ini);
	if(NULL == ini || NULL == group)
	{
		// This widget does not use any labels when its started for the first time.
		// We can see that the widget is started for the first time
		// by seeing that ini == NULL or group == NULL
		DeskletSetLabel(hwndDesklet, "");
		DeskletSetSubLabel(hwndDesklet, "");
		DeskletSetSize(hwndDesklet, &size);
	}
	else
	{
		// we currently don't load nor save any properties, but
		// this would be the right place to do so.
	}
	DeskletRedraw(hwndDesklet);

	return w;
}


void __stdcall OnDestroy(Widget* t, HWND hwndDesklet)
{
	// when our widget is destroyed,
	// delete the widget object which does further cleaning.
	delete t;
}

BOOL __stdcall OnLayerRightClick(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseRightClick(w->container, layerId, layerName, pt, s, keys))
		return TRUE;

	return FALSE;
}


BOOL __stdcall OnLayerClick(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// when we receive a layer click, we forward this mouse click to the container,
	// which, in turns, will forward it to the right control. If the container or its controlled
	// handled the mouse clikc, TRUE has been returned. Otherwise, FALSE.
	if(AveForwardMouseClick(w->container, layerId, layerName, pt, s, keys))
		return TRUE;

	if(strcmp(layerName, "flip") == 0)
	{
		w->Flip();
		return TRUE;
	}
	else if(strcmp(layerName, "backside") ==0)
	{
		std::wstring sel = w->GetBackColorAtPlace(pt);
		if(sel.length() > 0)
		{
			w->selBackColor = sel;
			w->SetImageFileToLayerByName("frontside", sel.c_str());
			w->UpdateCheckPlacement(TRUE);
			return TRUE;
		}
	}


	// let's know we handle this.
	return TRUE;
}

BOOL __stdcall OnLayerMouseDown(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// we'll also forward layer mouse down notifications.
	BOOL bRet = AveForwardMouseDown(w->container, layerId, layerName, pt, s, keys);

	if(strcmp(layerName, "flip") == 0)
		return TRUE;

	if(strcmp(layerName, "backside") ==0)
	{
		if(w->GetBackColorAtPlace(pt).length() > 0)
		{
			return TRUE;
		}
	}

	return bRet;
}

BOOL __stdcall OnLayerMouseWheelForward(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	// forward mouse wheel notifications.
	if(AveForwardMouseWheelForward(w->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelBackward(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(AveForwardMouseWheelBackward(w->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseUp(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseUp(w->container, layerId, layerName, pt, s, keys))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseOn(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	BOOL bRes = AveForwardMouseOn(w->container, layerId, layerName, pt, s);

	if(strcmp(layerName, "flip") == 0)
	{
		HCURSOR hand = LoadCursor(NULL,  IDC_HAND);
		w->oldCursor = DeskletSetCursor(w->hwnd, hand);
	}

	return bRes;
}

BOOL __stdcall OnLayerMouseMove(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(AveForwardMouseMove(w->container, layerId, layerName, pt, s))
		return TRUE;

	if(strcmp(layerName, "backside") ==0)
	{
		if(w->GetBackColorAtPlace(pt).length() > 0)
		{
			if(!w->isOverBackColor)
			{
				w->isOverBackColor = TRUE;
				HCURSOR hand = LoadCursor(NULL,  IDC_HAND);
				w->oldCursor = DeskletSetCursor(w->hwnd, hand);	
			}
		}
		else
		{
			if(w->isOverBackColor)
			{
				w->isOverBackColor = FALSE;
				DeskletSetCursor(w->hwnd, w->oldCursor);
			}
		}
	}

	return FALSE;
}


BOOL __stdcall OnLayerMouseOut(Widget* w, UINT layerId, const char* layerName)
{
	BOOL bRes = AveForwardMouseOut(w->container, layerId, layerName);

	if(strcmp(layerName, "flip") == 0)
	{
		DeskletSetCursor(w->hwnd, w->oldCursor);
	}
	else if(strcmp(layerName, "backside") == 0)
	{
		if(w->isOverBackColor)
		{
			w->isOverBackColor = FALSE;
			DeskletSetCursor(w->hwnd, w->oldCursor);
		}
	}

	return bRes;
}

void __stdcall OnSave(Widget* w, const char* ini, const char* group, BOOL bIsForExport)
{
	std::wstring wsGroup = StringToWString(group);
	std::wstring wsIni = StringToWString(ini);

	std::wstring txt = w->GetTextFromArea();
	int l = txt.length();
	for(int i=0;i<l;i++)
	{
		if(txt[i]=='\n')txt[i]=wchar_t(180);
		else if(txt[i]=='\r')txt[i]=wchar_t(181);
		else if(txt[i]== '=')txt[i]=wchar_t(182);
	}

	WritePrivateProfileInt(group, "UseSuperSampling", w->useSuperSampling, ini);
	WritePrivateProfileInt(group, "FontSize", w->selFontSize, ini);
	WritePrivateProfileInt(group, "TextLen", txt.length(), ini);
	WritePrivateProfileStringW(wsGroup.c_str(), L"Text", txt.c_str(), wsIni.c_str());
	WritePrivateProfileStringW(wsGroup.c_str(), L"FontName", w->selFontName.c_str(), wsIni.c_str());
	WritePrivateProfileStringW(wsGroup.c_str(), L"BackColor", w->selBackColor.c_str(), wsIni.c_str());
}

void __stdcall OnProcessMessage(Widget* w, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(NULL == w)
		return;

	if(WM_TIMER == msg && FLIP_FADE_TIMER_ID == wParam)
	{
		// this timer controls the flippy fade-in aniamtion
		UINT flip_rollie = DeskletLayerGetLayer(w->hwnd, "flip", NULL);

		// calculate the new alpha level for the flippy based
		// on the fade direction (in/out)
		if(w->flipIsFadingIn)
			w->flipAlpha += flipFadeDiff;
		else
			w->flipAlpha -= flipFadeDiff;

		// set the layers alpha
		if(w->flipAlpha >= 0 && w->flipAlpha <= flipFadeMax)
			DeskletLayerSetAlpha(w->hwnd, flip_rollie, TRUE, w->flipAlpha);

		// we are done when: fading in and we reached the maximum OR
		// when fading out and we got underneath the treshold.
		// if we are done, kill the animation timer.
		if(w->flipIsFadingIn && w->flipAlpha >= flipFadeMax)
			KillTimer(hwnd, FLIP_FADE_TIMER_ID);
		else if(!w->flipIsFadingIn && w->flipAlpha < flipFadeDiff)
		{
			// if fading out, hide the flippie
			DeskletLayerSetVisibility(w->hwnd, flip_rollie, TRUE, FALSE);
			KillTimer(hwnd, FLIP_FADE_TIMER_ID);
		}
	}
	else if(w->container != NULL)
	{
		// we also forward windows messages to our container.
		// this way, the inputbox can respond to keys being pressed, for example.
		if(AveForwardMessage(w->container, msg, wParam, lParam))
			return;
	}
}

BOOL __stdcall OnMouseMove(Widget* w, int x, int y)
{
	if(!w->isFlipVisible && !w->isInBackMode)
	{
		UINT flip_rollie = DeskletLayerGetLayer(w->hwnd, "flip", NULL);
		DeskletLayerSetVisibility(w->hwnd, flip_rollie, FALSE, TRUE);
		w->isFlipVisible = TRUE;

		w->flipIsFadingIn = TRUE;
		SetTimer(w->hwnd, FLIP_FADE_TIMER_ID, FLIP_TIMER_INTERVAL, NULL);
	}

	return TRUE;
}

BOOL __stdcall OnMouseOut(Widget* w)
{
	if(w->isFlipVisible)
	{
		w->isFlipVisible = FALSE;
		w->flipIsFadingIn = FALSE;
		SetTimer(w->hwnd, FLIP_FADE_TIMER_ID, FLIP_TIMER_INTERVAL, NULL);
	}

	return TRUE;
}

BOOL CALLBACK OnDragEnter(Widget* w,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	FORMATETC fmtetc; 

    fmtetc.cfFormat = CF_UNICODETEXT; 
    fmtetc.ptd      = 0; 
    fmtetc.dwAspect = DVASPECT_CONTENT; 
    fmtetc.lindex   = -1; 
    fmtetc.tymed    = TYMED_HGLOBAL;
	
	if(!SUCCEEDED(obj->QueryGetData(&fmtetc)))
	{
		w->acceptDrop = FALSE;
		*effect = DROPEFFECT_NONE;
	}
	else
	{
		w->acceptDrop = TRUE;
	}

	return TRUE;
}

BOOL CALLBACK OnDragOver(Widget* w,DWORD keys, POINTL pnt, DWORD* effect)
{
	if(!w->acceptDrop)
	{
		*effect = DROPEFFECT_NONE;
	}
	else
	{	
	}
	return TRUE;
}

BOOL CALLBACK OnDrop(Widget* w,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	FORMATETC fmtetc; 

    fmtetc.cfFormat = CF_UNICODETEXT; 
    fmtetc.ptd      = 0; 
    fmtetc.dwAspect = DVASPECT_CONTENT; 
    fmtetc.lindex   = -1; 
    fmtetc.tymed    = TYMED_HGLOBAL; 

	if(SUCCEEDED(obj->QueryGetData(&fmtetc)))
	{
		    STGMEDIUM medium; 
			HRESULT hr = obj->GetData(&fmtetc, &medium); 

			if (!FAILED(obj->GetData(&fmtetc, &medium))) 
			{ 
				std::wstring newText;
				HGLOBAL hGlobal = medium.hGlobal;
				WCHAR* txt = (WCHAR*)GlobalLock(hGlobal);
				if(keys & MK_CONTROL)
				{
					newText = w->GetTextFromArea() + txt;
					//strncat(lpData->text,txt,10024-strlen(lpData->text));
				}
				else
				{
					newText = txt;
				}
				GlobalUnlock(hGlobal); 
				ReleaseStgMedium(&medium); 

				AveSendMessage(w->textArea, AV_SET_TEXT, 0L, (LPARAM)newText.c_str());
				return TRUE;
			}
    }
	return FALSE;
}

BOOL __stdcall OnDragDropDataRequest(Widget* w,FORMATETC* format,STGMEDIUM* medium)
{
	if(format->cfFormat == CF_UNICODETEXT)
	{

		std::wstring text = w->GetTextFromArea();
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,(text.length()+1)*sizeof(WCHAR));
		WCHAR* memCpy = (WCHAR*)GlobalLock(hMem);

		memcpy((void*)memCpy,(WCHAR*)text.c_str(),text.length() * sizeof(WCHAR));
		memCpy[text.length()] = L'\0';
		GlobalUnlock(hMem);

		medium->hGlobal = hMem;
		medium->pUnkForRelease = 0;
		medium->tymed = TYMED_HGLOBAL;
		return TRUE;
	}
	return FALSE;
}

/*
INT __stdcall OnAddPropertySheets(Widget* w, int numPresent)
{
	PROPSHEETPAGE psp = {0};

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = w->dll;
	psp.pszTitle = "Skin";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_SKIN);
	psp.pfnDlgProc = SkinConfigProc;
	psp.lParam = (LPARAM)w;

	return DeskletAddPropertySheet(w->hwnd,CreatePropertySheetPage(&psp));
}*/