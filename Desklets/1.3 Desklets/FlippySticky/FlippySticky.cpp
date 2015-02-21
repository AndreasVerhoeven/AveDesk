#include "../DeskletSDK.h"

#include <map>
#include <list>
#include <string>
#include <iterator>
#include <fstream>

#include "osxpopupmenu.h"
#include "avebutton.h"
#include "avetextfield.h"


#define WM_OSXMENU_NOTIFY_FONT WM_APP + 304
#define WM_OSXMENU_NOTIFY_SIZE WM_APP + 305

const int STICKY_WIDTH = 225;
const int STICKY_HEIGHT = 206;

SIZE scale = {STICKY_WIDTH, STICKY_HEIGHT};

const int comboBoxLeftWidth = 5;

RECT flipitRect = {203, 170, 213, 180};
RECT checkRect  = {0, 0, 27, 24 };
RECT doneRect   = {142, 145, 142 + 58, 145 + 23};
RECT textRect   = { 15, 15, 207, 180 };
RECT fontRect   = { 18, 110, 133, 135 };
RECT sizeRect   = { 150, 110, 200, 135 };
SIZE backColorSize = {27, 24};
POINT closePoint = {10, 2};

BOOL __stdcall OnDoneButtonClick(void* ptr, UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys);

class FlippySticky : public IAveCallback
{
public:
	FlippySticky(HWND hwndDesklet, HINSTANCE hInstance) : 
		hwndDesklet(hwndDesklet),
		hInstance(hInstance),
		textField(NULL)
	{
	}

	void SetupControls()
	{
		char resPath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(hwndDesklet, resPath);
		strcat(resPath, "images\\controls\\");

		resources = new IAveResources(resPath);
		container = new IAveControlContainer(hwndDesklet, resources);

		container->Lock();

		textField = new CAveTextField(container, "text");
		textField->Lock();
		textField->SetOnlyLoseFocusOnDeskletOut(TRUE);
		textField->SetRect(&textRect, &scale);
		textField->SetFont(new Font(GetFontAsWString().c_str(), (float)fontSize, FontStyleBold));
		textField->SetTextColor(Color(225,0,0,0));
		container->Add(textField);
		textField->Unlock();

		CAveButton* but = new CAveButton(container, "back_done");
		but->Lock();
		but->SetRect(&doneRect, &scale);
		but->SetClickHandler(this, OnDoneButtonClick);
		but->Hide();
		container->Add(but);
		but->Unlock();

		container->Unlock();
	}

	~FlippySticky()
	{
		delete container;
		delete resources;
	}

	HWND      hwndDesklet;
	HINSTANCE hInstance;

	BOOL isInBackMode;
	BOOL flipitVisible;

	char bgFile[MAX_PATH];
	
	std::map<std::string, POINT> backColors;

	char font[MAX_PATH];

	std::list<std::string> fonts;
	std::list<std::string> sizes;


	std::wstring GetFontAsWString()
	{
		WCHAR wfont[sizeof(font) / sizeof(font[0])] = {0};
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, font , -1, wfont, sizeof(wfont));
		return wfont;
	}

	int fontSize;

	IAveResources* resources;
	IAveControlContainer* container;
	CAveTextField* textField;
};

void PopulateFontList(FlippySticky* sticky)
{
	char fontsFile[MAX_PATH] = {0};
	GetDLLPath(sticky->hInstance, fontsFile, MAX_PATH);
	strcat(fontsFile, "fonts.txt");

	std::ifstream f(fontsFile);
	std::string line;
	while(std::getline(f, line))
		sticky->fonts.push_back(line);

	if(sticky->fonts.size() == 0)
		sticky->fonts.push_back("Lucida Sans");
}

void PopulateSizesList(FlippySticky* sticky)
{
	char sizes[MAX_PATH] = {0};
	GetDLLPath(sticky->hInstance, sizes, MAX_PATH);
	strcat(sizes, "sizes.txt");

	std::ifstream f(sizes);
	std::string line;
	while(std::getline(f, line))
		sticky->sizes.push_back(line);

	if(sticky->sizes.size() == 0)
		sticky->sizes.push_back("16");
}

void FlipSticky(FlippySticky* sticky);

BOOL __stdcall OnDoneButtonClick(void* ptr, UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	FlippySticky* sticky = reinterpret_cast<FlippySticky*>(ptr);
	FlipSticky(sticky);

	return TRUE;
}

POINT RemapPoint(const POINT& t, const SIZE& s, float xScale=100, float yScale=100)
{
	POINT nT;
	nT.x = nT.y = 0;
	
	if(s.cx == 0 || s.cy == 0)
		return nT;

	nT.x = static_cast<long>(float(t.x) / float(s.cx) * xScale);
	nT.y = static_cast<long>(float(t.y) / float(s.cy) * yScale);

	return nT;
}

inline bool PointInRect(Rect& r,POINT& p)
{
	return (p.x > r.X && p.x < (r.X+r.Width) && p.y > (r.Y) && p.y < (r.Y + r.Height));
}

UINT SetComboboxLayer(HWND hwndDesklet, RECT* r, UINT xScale, UINT yScale, const char* name, const char* text, BOOL redraw, BOOL pressed = FALSE)
{
	// load images
	char relativePath[MAX_PATH] = {0};
	DeskletGetRelativeFolder(hwndDesklet, relativePath);

	char leftPath[MAX_PATH] = {0};
	strcpy(leftPath, relativePath);
	strcat(leftPath, "images\\controls\\combo_left.png");

	char midPath[MAX_PATH] = {0};
	strcpy(midPath, relativePath);
	strcat(midPath, "images\\controls\\combo_mid.png");

	char rightPath[MAX_PATH] = {0};
	strcpy(rightPath, relativePath);
	if(!pressed)
		strcat(rightPath, "images\\controls\\combo_right.png");
	else
		strcat(rightPath, "images\\controls\\combo_right_pressed.png");

	Bitmap* left  = DeskletLoadGDIPlusImage(leftPath);
	Bitmap* mid   = DeskletLoadGDIPlusImage(midPath);
	Bitmap* right = DeskletLoadGDIPlusImage(rightPath);


	BOOL success = FALSE;
	UINT id = DeskletLayerGetLayer(hwndDesklet, name, &success);
	if(!success)
		id = DeskletLayerAdd(hwndDesklet, name);

	int width  = r->right - r->left;
	int height = r->bottom - r->top;
	Bitmap* bmp = new Bitmap(width, height);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	Rect midRect(left->GetWidth(), 0, width - left->GetWidth() - right->GetWidth(), height);

	//g.DrawImage(left, Rect(0,0, left->GetWidth(), height), 0,0, left->GetWidth(), left->GetHeight(), UnitPixel, 0, 0, 0);
	//SolidBrush red(Color(255, 255, 0, 0));
	//g.FillRectangle(&red, Rect(0,0, left->GetWidth(), height));
	g.DrawImage(left, Rect(0,0, left->GetWidth(), height), 0,0, left->GetWidth(), left->GetHeight(), UnitPixel, 0, 0, 0);
	g.DrawImage(right, Rect(width - right->GetWidth(),0 , right->GetWidth(), height), 0,0, right->GetWidth(), right->GetHeight(), UnitPixel, 0, 0, 0);


	Bitmap mid2(mid->GetWidth(), height);
	Graphics g2(&mid2);
	g2.SetSmoothingMode(SmoothingModeAntiAlias);
	g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g2.DrawImage(mid, Rect(0,0, mid->GetWidth(), height), 0, 0, mid->GetWidth(), mid->GetHeight(), UnitPixel, 0, 0, 0);


	int midWidth = mid->GetWidth();
	int drawsNeeded = midRect.Width / midWidth;
	int filled = 0;
	for(int i=0; i < drawsNeeded; ++i)
	{
		g.DrawImage(mid, Rect(midRect.X + filled,0, mid2.GetWidth(), midRect.Height), 0, 0, mid2.GetWidth(), mid2.GetHeight(), UnitPixel, 0, 0, 0);
		filled += midWidth;
	}
	g.DrawImage(mid, Rect(midRect.X + filled,0, midRect.Width - filled, midRect.Height), 0, 0, midRect.Width - filled, mid2.GetHeight(), UnitPixel, 0, 0, 0);
	int l = 0;

	OLECHAR wtext[10024] = {0};

	Gdiplus::FontFamily  fontFamily(L"Lucida Sans Unicode");
	SolidBrush  black(Gdiplus::Color(255, 0, 0, 0));

	GraphicsPath path(Gdiplus::FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	RectF rect(midRect.X, midRect.Height / 2 - 14 / 2 + 1, midRect.Width, 14);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text , -1, wtext, sizeof(wtext));
	path.AddString(wtext, -1, &fontFamily, FontStyleBold, (float)12, rect, &format);

	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g.FillPath(&black, &path);

	DeskletLayerSetImage(hwndDesklet, id, FALSE, bmp, TRUE);
	DeskletLayerSetPlacement(hwndDesklet, id, redraw, TRUE, r, xScale, yScale);

	delete left;
	delete mid;
	delete right;

	return id;
}

void PopulateBackColors(FlippySticky* sticky)
{

	// first row 
	POINT pYellow = {18, 28};
	sticky->backColors["yellow.png"] = pYellow;

	POINT pOrange = {57, 28};
	sticky->backColors["orange.png"] = pOrange;

	POINT pGreen1 = {97, 28};
	sticky->backColors["green.png"] = pGreen1;

	POINT pGreen2 = {136, 28};
	sticky->backColors["green2.png"] = pGreen2;

	POINT pBlue2 = {175, 28};
	sticky->backColors["blue2.png"] = pBlue2;

	// second row


	POINT pBlue1 = {18, 60};
	sticky->backColors["blue.png"] = pBlue1;

	POINT pPurple1 = {57, 60};
	sticky->backColors["purple1.png"] = pPurple1;

	POINT pPink = {97, 60};
	sticky->backColors["pink.png"] = pPink;

	POINT pWhite = {136, 60};
	sticky->backColors["white.png"] = pWhite;

	POINT pGrey = {175, 60};
	sticky->backColors["grey.png"] = pGrey;

}

void PlaceCheckForColor(FlippySticky* sticky, BOOL bRepaint = FALSE)
{
	POINT backColorPoint = sticky->backColors[sticky->bgFile];

	RECT r = checkRect;
	r.left += 10;
	r.left = backColorPoint.x + 1;
	r.top  = backColorPoint.y + 1;
	r.bottom += r.top;
	r.right += r.left;

	DeskletLayerSetPlacement(sticky->hwndDesklet, DeskletLayerGetLayer(sticky->hwndDesklet,"back_check",NULL), bRepaint, TRUE, &r, STICKY_WIDTH, STICKY_HEIGHT);
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"Flippy");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Test for DeskletFlip()");
	*iVersion = 130;
}

/*
void SetTextForSticky(FlippySticky* sticky)
{
	Bitmap* bmp = new Bitmap(textRect.right - textRect.left, textRect.bottom - textRect.top);

	Graphics	g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
	OLECHAR wtext[10024];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sticky->font , -1, wtext, sizeof(wtext));

	Gdiplus::FontFamily  fontFamily(wtext);
	SolidBrush  black(Gdiplus::Color(255, 0, 0, 0));

	GraphicsPath path(Gdiplus::FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);

	RectF rect(0,0, bmp->GetWidth(), bmp->GetHeight());

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sticky->text , -1, wtext, sizeof(wtext));
	path.AddString(wtext, -1, &fontFamily, FontStyleBold, (float)sticky->fontSize, rect, &format);

	g.FillPath(&black, &path);

	DeskletLayerSetImage(sticky->hwndDesklet, DeskletLayerGetLayer(sticky->hwndDesklet, "text", NULL), TRUE, bmp, TRUE);

}*/

void __stdcall OnProcessMessage(FlippySticky* sticky, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!sticky)
		return;

	if(sticky->container != NULL)
	{
		if(sticky->container->Message(msg, wParam, lParam))
			return;
	}


	if(WM_MOUSEACTIVATE == msg)
	{
		SetFocus(hwnd);
	}
/*	else if(WM_KEYDOWN == msg)
	{
		UINT nVirtKey = (int) wParam;
		if(nVirtKey == VK_BACK)
		{
			int len = strlen(sticky->text);
			if(len > 0)
			{
				sticky->text[len - 1 ] = '\0';
				SetTextForSticky(sticky);
			}
		}
		else if(nVirtKey == VK_RETURN)
		{
			int len = strlen(sticky->text);
			if(len < sizeof(sticky->text) / sizeof(sticky->text[0]) - 1)
			{
				sticky->text[len]  = '\n';
				sticky->text[len + 1] = '\0';
				SetTextForSticky(sticky);
			}	
		}
	}

	else if(WM_CHAR == msg)
	{
		CHAR chCharCode = (TCHAR) wParam;
		if(chCharCode == VK_BACK || chCharCode == VK_RETURN)
			return;
		int len = strlen(sticky->text);
		if(len < sizeof(sticky->text) / sizeof(sticky->text[0]) - 1)
		{
			sticky->text[len]  = chCharCode;
			sticky->text[len + 1] = '\0';
			SetTextForSticky(sticky);
		}
	}*/
	else if(WM_OSXMENU_NOTIFY_FONT == msg)
	{
		const char* selectedFont = reinterpret_cast<const char*>(lParam);
		//if(selectedFont != NULL && stricmp(selectedFont, sticky->font) != 0)
		{
			if(selectedFont != NULL)
				strcpy(sticky->font, selectedFont);
			SetComboboxLayer(sticky->hwndDesklet, &fontRect, STICKY_WIDTH, STICKY_HEIGHT, "back_font", sticky->font, FALSE);
			sticky->textField->SetFont(new Font(sticky->GetFontAsWString().c_str(), sticky->fontSize, FontStyleBold));
			//SetTextForSticky(sticky);
		}
	}
	else if(WM_OSXMENU_NOTIFY_SIZE == msg)
	{
		const char* selectedSize = reinterpret_cast<const char*>(lParam);
		
		if(selectedSize != NULL)
		{
			int newSize = atoi(selectedSize);
			//if(newSize != sticky->fontSize)
			{
				sticky->fontSize = newSize;
				SetComboboxLayer(sticky->hwndDesklet, &sizeRect, STICKY_WIDTH, STICKY_HEIGHT, "back_size", selectedSize, FALSE);
				sticky->textField->SetFont(new Font(sticky->GetFontAsWString().c_str(), sticky->fontSize, FontStyleBold));
			}
		}
	}
}

FlippySticky* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	FlippySticky* sticky = new FlippySticky(hwndDesklet, hInstanceDll);
	PopulateFontList(sticky);
	PopulateSizesList(sticky);
	//sticky->hwndDesklet = hwndDesklet;
	//sticky->hInstance = hInstanceDll;
	sticky->isInBackMode = FALSE;
	sticky->flipitVisible = FALSE;
	sticky->fontSize = 14;
	PopulateBackColors(sticky);
	strcpy(sticky->bgFile, "yellow.png");
	//strcpy(sticky->text , "AveDesk 1.3 Flippy Sticky Demo!");
	strcpy(sticky->font, "Lucida Sans");

	DeskletSetClosePoint(hwndDesklet, &closePoint, STICKY_WIDTH, STICKY_HEIGHT);

	CHAR text[10024] = {0};

	if(ini && group)
	{
		GetPrivateProfileString(group, "BackgroundColor", sticky->bgFile, sticky->bgFile, sizeof(sticky->bgFile) / sizeof(sticky->bgFile[0]), ini);
		GetPrivateProfileString(group, "StickyText", text, text, sizeof(text) / sizeof(text[0]), ini);
		int l=strlen(text);
		for(int i=0;i<l;i++)
		{
			if(text[i]==char(180))text[i]='\n';
			else if(text[i]==char(181))text[i]='\r';
			else if(text[i]==char(182))text[i]='=';
		}

		GetPrivateProfileString(group, "StickyFont", sticky->font, sticky->font, sizeof(sticky->font) / sizeof(sticky->font[0]), ini);

		sticky->fontSize = GetPrivateProfileInt(group, "StickyFontSize", sticky->fontSize, ini);
	}
	else
	{
		strcpy(text, "AveDesk 1.3 Demo!");
		SIZE s = {STICKY_WIDTH, STICKY_HEIGHT};
		DeskletSetSize(sticky->hwndDesklet, &s);
	}

	// remove all layers
	while(DeskletLayerCount(hwndDesklet) > 0)
		DeskletLayerRemove(hwndDesklet, 0, FALSE);

	UINT bgLayer           = DeskletLayerAdd(hwndDesklet, "bg");
	//UINT textLayer         = DeskletLayerAdd(hwndDesklet, "text");
	UINT flipitLayer       = DeskletLayerAdd(hwndDesklet, "flipit");
	UINT backLayer         = DeskletLayerAdd(hwndDesklet, "back");
	UINT backCheckControl  = DeskletLayerAdd(hwndDesklet, "back_check");
	//UINT backDoneControl   = DeskletLayerAdd(hwndDesklet, "back_done");
	UINT backFontControl   = DeskletLayerAdd(hwndDesklet, "back_font");
	UINT backSizeControl   = DeskletLayerAdd(hwndDesklet, "back_size");

	DeskletLayerSetIsNotMovable(hwndDesklet, flipitLayer, TRUE);
	//DeskletLayerSetIsNotMovable(hwndDesklet, backDoneControl, TRUE);
	DeskletLayerSetIsNotMovable(hwndDesklet, backFontControl, TRUE);
	DeskletLayerSetIsNotMovable(hwndDesklet, backSizeControl, TRUE);

	DeskletLayerSetNoAlphaBlendedHitTesting(hwndDesklet, flipitLayer, TRUE);

	DeskletLayerSetIsClickThrough(hwndDesklet, backCheckControl, TRUE);

	char relativePath[MAX_PATH] = {0};
	DeskletGetRelativeFolder(hwndDesklet, relativePath);

	char bgPath[MAX_PATH] = {0};
	strcpy(bgPath, relativePath);
	strcat(bgPath, "images\\");
	strcat(bgPath, sticky->bgFile);

	char backPath[MAX_PATH] = {0};
	strcpy(backPath, relativePath);
	strcat(backPath, "images\\backside.png");

	char flipitPath[MAX_PATH] = {0};
	strcpy(flipitPath, relativePath);
	strcat(flipitPath, "images\\flipit.png");

	char checkPath[MAX_PATH] = {0};
	strcpy(checkPath, relativePath);
	strcat(checkPath, "images\\check.png");

//	char donePath[MAX_PATH] = {0};
//	strcpy(donePath, relativePath);
//	strcat(donePath, "images\\done.png");


	DeskletLayerSetVisibility(hwndDesklet, bgLayer, FALSE, TRUE);
	//DeskletLayerSetVisibility(hwndDesklet, textLayer, FALSE, TRUE);
	DeskletLayerSetVisibility(hwndDesklet, flipitLayer, FALSE, FALSE);
	DeskletLayerSetVisibility(hwndDesklet, backLayer, FALSE, FALSE);
	DeskletLayerSetVisibility(hwndDesklet, backCheckControl, FALSE, FALSE);
	//DeskletLayerSetVisibility(hwndDesklet, backDoneControl, FALSE, FALSE);
	DeskletLayerSetVisibility(hwndDesklet, backFontControl, FALSE, FALSE);
	DeskletLayerSetVisibility(hwndDesklet, backSizeControl, FALSE, FALSE);

	DeskletLayerSetImageFile(hwndDesklet, bgLayer, FALSE, bgPath);
	DeskletLayerSetImageFile(hwndDesklet, backLayer, FALSE, backPath);
	DeskletLayerSetImageFile(hwndDesklet, flipitLayer, FALSE, flipitPath);
	DeskletLayerSetImageFile(hwndDesklet, backCheckControl, FALSE, checkPath);
	//DeskletLayerSetImageFile(hwndDesklet, backDoneControl, FALSE, donePath);

	DeskletLayerSetPlacement(hwndDesklet, flipitLayer, FALSE, TRUE, &flipitRect, STICKY_WIDTH, STICKY_HEIGHT);
	//DeskletLayerSetPlacement(hwndDesklet, backDoneControl, FALSE, TRUE, &doneRect, STICKY_WIDTH, STICKY_HEIGHT);
	//DeskletLayerSetPlacement(hwndDesklet, textLayer, FALSE, TRUE, &textRect, STICKY_WIDTH, STICKY_HEIGHT);

	PlaceCheckForColor(sticky);
	SetComboboxLayer(hwndDesklet, &fontRect, STICKY_WIDTH, STICKY_HEIGHT, "back_font", sticky->font, FALSE);

	char fontSizeStr[10] = {0};
	itoa(sticky->fontSize, fontSizeStr, 10);
	SetComboboxLayer(hwndDesklet, &sizeRect, STICKY_WIDTH, STICKY_HEIGHT, "back_size", fontSizeStr, FALSE);

	//SetTextForSticky(sticky);

	sticky->SetupControls();

	sticky->textField->SetTextA(text);

	return sticky;
}

void __stdcall OnDestroy(FlippySticky* sticky, HWND hwndDesklet)
{
	delete sticky;
}

BOOL CheckBackColorHit(FlippySticky* sticky, const POINT& p, std::string* str)
{
	for(std::map<std::string, POINT>::iterator iter = sticky->backColors.begin();iter != sticky->backColors.end(); iter++)
	{
		RECT r = {iter->second.x, iter->second.y, iter->second.x + backColorSize.cx, iter->second.y + backColorSize.cy };
		if(PtInRect(&r, p))
		{
			if(str != NULL)
				*str = iter->first;

			return TRUE;
		}
	}

	if(str != NULL)
		*str = "";

	return FALSE;
}

BOOL CheckBackColorsForHit(FlippySticky* sticky, const POINT& p)
{
	std::string color;
	
	if(CheckBackColorHit(sticky, p, &color))
	{
			strcpy(sticky->bgFile, color.c_str());

			char relativePath[MAX_PATH] = {0};
			DeskletGetRelativeFolder(sticky->hwndDesklet, relativePath);

			char bgPath[MAX_PATH] = {0};
			strcpy(bgPath, relativePath);
			strcat(bgPath, "images\\");
			strcat(bgPath, sticky->bgFile);

			DeskletLayerSetImageFile(sticky->hwndDesklet, DeskletLayerGetLayer(sticky->hwndDesklet, "bg", NULL), FALSE, bgPath);

			PlaceCheckForColor(sticky, TRUE);

			return TRUE;
	}

	return FALSE;
}


BOOL __stdcall OnLayerClick(FlippySticky* sticky, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(sticky->container->MouseClick(layerId, layerName, pt, s, keys))
		return TRUE;

	if(strcmp(layerName, "back") == 0)
	{
		POINT rp = RemapPoint(*pt, *s, (float)STICKY_WIDTH, (float)STICKY_HEIGHT );

		if(sticky->isInBackMode)
			CheckBackColorsForHit(sticky, rp);
	}
	else if(strcmp(layerName, "flipit") == 0)
	{
			FlipSticky(sticky);
			return TRUE;
	}
	/*else if(strcmp(layerName, "back_done") == 0)
	{
		FlipSticky(sticky);

		return TRUE;
	}*/
	else if(strcmp(layerName, "back_font") == 0)
	{
		char** fonts = new char*[sticky->fonts.size()];
		int index = 0;
		for(std::list<std::string>::iterator iter = sticky->fonts.begin(); iter != sticky->fonts.end(); ++iter)
		{
			fonts[index++] = (char*)iter->c_str();
		}

		//fonts[0] = "Arial";
		//fonts[1] = "Comic Sans MS";
		//fonts[2] = "Hyena";
		//fonts[3] = "Impact";
		//fonts[4] = "Lucida Sans";
		//fonts[5] = "Verdana";
				
		MenuData data = {0};
		data.selected = 0;
		data.count = sticky->fonts.size();
		data.items = fonts;

		for(int i = 0; i < data.count; ++i)
		{
			if(stricmp(data.items[i], sticky->font) == 0)
				data.selected = i;
		}

		char resourcesPath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(sticky->hwndDesklet, resourcesPath);
		strcat(resourcesPath, "images\\controls\\menu\\");

		POINT pt = {0};
		RECT r = {0};
		GetWindowRect(sticky->hwndDesklet, &r);
		pt.x = r.left + fontRect.left;
		pt.y = r.top  + fontRect.top;


		DoOSXPopupMenu(sticky->hwndDesklet, WM_OSXMENU_NOTIFY_FONT, &data, resourcesPath, &pt);
		delete[] fonts;

		return TRUE;
	}
	else if(strcmp(layerName, "back_size") == 0)
	{
		//char** sizes = new char*[7];
		char** sizes = new char*[sticky->sizes.size()];
		int index = 0;
		for(std::list<std::string>::iterator iter = sticky->sizes.begin(); iter != sticky->sizes.end(); ++iter)
		{
			sizes[index++] = (char*)iter->c_str();
		}
		//sizes[0] = "12";
		//sizes[1] = "14";
		//sizes[2] = "16";
		//sizes[3] = "18";
		//sizes[4] = "20";
		//sizes[5] = "22";
		//sizes[6] = "26";
				
		MenuData data = {0};
		data.selected = 0;
		data.count = sticky->sizes.size();
		data.items = sizes;

		char curSizeAsString[10] = {0};
		itoa(sticky->fontSize, curSizeAsString, 10);

		for(int i = 0; i < data.count; ++i)
		{
			if(stricmp(data.items[i], curSizeAsString) == 0)
				data.selected = i;
		}

		char resourcesPath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(sticky->hwndDesklet, resourcesPath);
		strcat(resourcesPath, "images\\controls\\menu\\");

		POINT pt = {0};
		RECT r = {0};
		GetWindowRect(sticky->hwndDesklet, &r);
		pt.x = r.left + sizeRect.left;
		pt.y = r.top  + sizeRect.top;

		DoOSXPopupMenu(sticky->hwndDesklet, WM_OSXMENU_NOTIFY_SIZE, &data, resourcesPath, &pt);
		delete[] sizes;
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseDown(FlippySticky* sticky, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(sticky->container->MouseDown(layerId, layerName, pt, s, keys))
		return TRUE;

	if(strcmp(layerName, "back") == 0)
	{
		POINT rp = RemapPoint(*pt, *s, (float)STICKY_WIDTH, (float)STICKY_HEIGHT );

		if(sticky->isInBackMode)
			return CheckBackColorHit(sticky, rp, NULL);
	}
	/*else if(strcmp(layerName, "back_done") == 0)
	{
		char donePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(sticky->hwndDesklet, donePath);
		strcat(donePath, "images\\done_pressed.png");

		DeskletLayerSetImageFile(sticky->hwndDesklet, layerId, TRUE, donePath);

		return TRUE;
	}*/
	else if(strcmp(layerName, "back_font") == 0)
	{
		SetComboboxLayer(sticky->hwndDesklet, &fontRect, STICKY_WIDTH, STICKY_HEIGHT, "back_font", sticky->font, TRUE, TRUE);
		ReleaseCapture();
		OnLayerClick(sticky, layerId, layerName, pt, s, keys); 
		return TRUE;
	}
	else if(strcmp(layerName, "back_size") == 0)
	{
		char fontStr[100] = {0};
		itoa(sticky->fontSize, fontStr, 10);
		SetComboboxLayer(sticky->hwndDesklet, &sizeRect, STICKY_WIDTH, STICKY_HEIGHT, "back_size", fontStr, TRUE, TRUE);

		ReleaseCapture();
		OnLayerClick(sticky, layerId, layerName, pt, s, keys); 
		return TRUE;
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseUp(FlippySticky* sticky, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(sticky->container->MouseUp(layerId, layerName, pt, s, keys))
		return FALSE;

	/*
	if(strcmp(layerName, "back_done") == 0)
	{
		char donePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(sticky->hwndDesklet, donePath);
		strcat(donePath, "images\\done.png");

		DeskletLayerSetImageFile(sticky->hwndDesklet, layerId, TRUE, donePath);

		return FALSE;
	}*/
	else if(strcmp(layerName, "back_font") == 0)
	{
		SetComboboxLayer(sticky->hwndDesklet, &fontRect, STICKY_WIDTH, STICKY_HEIGHT, "back_font", sticky->font, TRUE, FALSE);
		return TRUE;
	}
	else if(strcmp(layerName, "back_size") == 0)
	{
		char fontStr[100] = {0};
		itoa(sticky->fontSize, fontStr, 10);
		SetComboboxLayer(sticky->hwndDesklet, &sizeRect, STICKY_WIDTH, STICKY_HEIGHT, "back_size", fontStr, TRUE, FALSE);
		return TRUE;
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseOn(FlippySticky* sticky, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(sticky->container->MouseOn(layerId, layerName, pt, s))
		return TRUE;

	// TEST CODE
	/*if(strcmp(layerName, "back_done") == 0)
	{
		char donePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(sticky->hwndDesklet, donePath);
		strcat(donePath, "images\\done_pressed.png");

		DeskletLayerSetImageFile(sticky->hwndDesklet, layerId, TRUE, donePath);

		return TRUE;
	}*/

	return FALSE;
}

BOOL __stdcall OnLayerMouseMove(FlippySticky* sticky, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(sticky->container->MouseMove(layerId, layerName, pt, s))
		return TRUE;

	return FALSE;
}


BOOL __stdcall OnLayerMouseOut(FlippySticky* sticky, UINT layerId, const char* layerName)
{
	if(sticky->container->MouseOut(layerId, layerName))
		return TRUE;

	// TEST CODE
	/*if(strcmp(layerName, "back_done") == 0)
	{
		char donePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(sticky->hwndDesklet, donePath);
		strcat(donePath, "images\\done.png");

		DeskletLayerSetImageFile(sticky->hwndDesklet, layerId, TRUE, donePath);

		return TRUE;
	}*/

	return FALSE;
}


void __stdcall OnSave(FlippySticky* sticky, const char* ini, const char* group, BOOL bIsForExport)
{
	char convertedText[10024] = {0};
	strcpy(convertedText, sticky->textField->GetTextA().c_str());

	int l = strlen(convertedText);
	for(int i=0;i<l;i++)
	{
		if(convertedText[i]=='\n')convertedText[i]=char(180);
		else if(convertedText[i]=='\r')convertedText[i]=char(181);
		else if(convertedText[i]== '=')convertedText[i]=char(182);
	}

	WritePrivateProfileString(group, "BackgroundColor", sticky->bgFile, ini);
	WritePrivateProfileString(group, "StickyText", convertedText, ini);
	WritePrivateProfileString(group, "StickyFont", sticky->font, ini);
	WritePrivateProfileInt(group, "StickyFontSize", sticky->fontSize, ini);
}

void FlipSticky(FlippySticky* sticky)
{
	FlipParams params = {0};
	params.flipToBack = !sticky->isInBackMode;
	params.useLayers = TRUE;

	HWND hwndDesklet = sticky->hwndDesklet;

	UINT frontLayers[] = {
							DeskletLayerGetLayer(hwndDesklet, "bg", NULL),
							DeskletLayerGetLayer(hwndDesklet, "text", NULL)
						};

	UINT backLayers[]  = { 
							DeskletLayerGetLayer(hwndDesklet, "back", NULL),
							DeskletLayerGetLayer(hwndDesklet, "back_check", NULL), 
							DeskletLayerGetLayer(hwndDesklet, "back_done", NULL),
							DeskletLayerGetLayer(hwndDesklet, "back_font", NULL),
							DeskletLayerGetLayer(hwndDesklet, "back_size", NULL)
						};


	params.oldLayersVisible = frontLayers;
	params.newLayersVisible = backLayers;
	params.oldLayersVisibleCount = sizeof(frontLayers) / sizeof(frontLayers[0]);
	params.newLayersVisibleCount = sizeof(backLayers) / sizeof(backLayers[0]);
	if(sticky->isInBackMode)
	{
		params.oldLayersVisible = backLayers;
		params.newLayersVisible = frontLayers;

		params.oldLayersVisibleCount = sizeof(backLayers) / sizeof(backLayers[0]);
		params.newLayersVisibleCount = sizeof(frontLayers) / sizeof(frontLayers[0]);
	}

	DeskletFlip(hwndDesklet, &params);

	sticky->isInBackMode = !sticky->isInBackMode;
}

BOOL __stdcall OnMouseMove(FlippySticky* sticky, int x, int y)
{
	SIZE s = {0};
	DeskletGetSize(sticky->hwndDesklet, &s);

	POINT p = {x,y};
	//POINT rp = RemapPoint(p,s,(float)STICKY_WIDTH, (float)STICKY_HEIGHT );

	if(!sticky->isInBackMode)
	{
		UINT flipitLayer = DeskletLayerGetLayer(sticky->hwndDesklet, "flipit", FALSE);
		if(DeskletLayerHitTest(sticky->hwndDesklet, flipitLayer, &p, &s))
		//if(PtInRect(&flipitRect, rp))
		{
			if(!sticky->flipitVisible)
				DeskletLayerSetVisibility(sticky->hwndDesklet, DeskletLayerGetLayer(sticky->hwndDesklet, "flipit", NULL), TRUE, TRUE);

			sticky->flipitVisible = TRUE;
		}
		else
		{
			if(sticky->flipitVisible)
				DeskletLayerSetVisibility(sticky->hwndDesklet, DeskletLayerGetLayer(sticky->hwndDesklet, "flipit", NULL), TRUE, FALSE);

			sticky->flipitVisible = FALSE;
		}
	}

	return TRUE;
}

BOOL __stdcall OnMouseOut(FlippySticky* sticky)
{
	if(sticky->flipitVisible)
		DeskletLayerSetVisibility(sticky->hwndDesklet, DeskletLayerGetLayer(sticky->hwndDesklet, "flipit", NULL), TRUE, FALSE);

	sticky->flipitVisible = FALSE;

	return TRUE;
}


BOOL __stdcall OnLayerMouseWheelForward(FlippySticky* sticky, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(sticky->container->MouseWheelForward(layerId, layerName, pt, s, keys, numOfScrolls))
		return FALSE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelBackward(FlippySticky* sticky, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(sticky->container->MouseWheelBackward(layerId, layerName, pt, s, keys, numOfScrolls))
		return FALSE;

	return FALSE;
}
