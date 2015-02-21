//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.


// SAMPLE DICTIONARY WIDGET.
// by Andreas Verhoeven, 2005.
// This widget will show an inputbox where the user can enter text.
// when the [enter] key is pressed, the word entered in the inputbox
// will be searched on the web. When the word is found,
// the desklet will slide out and show the definition.
// when the word isn't found, this will be notified to the user by
// recoloring the widget.

// STATUS:
// + almost done
// - skin changing/enumeration code

// ADDED:
// + close button position is now set
// + sliding animation
// + websearch + showing results
// + flip to back
// + dictionary or thesaurus search settings
// + made the flippy fade in/out

// DeskletSDK for Desklet* functions.
#include "../DeskletSDK.h"

// For CAveHTTP class
#include "../AveHTTP.h"

#include <vector> // for std::vector<T>

#include "Skin.h" // for CSkin

// AveControls specific includes
#include "../AveControls/aveconstants.h"
#include "../ControlsDLL/AveControls.h"
#pragma comment(lib, "../AveControls.lib")

// some C++ includes
#include <cstring>	// for std::basic_string<T> operator==()
#include<string>	// for std::wstring

// slide animation timer constants
const SLIDE_ANIM_TIMER_ID			= 4095;
const SLIDE_ANIM_TIMER_INTERVAL		= 15;

// fade animation constants
const FLIP_FADE_TIMER_ID			= 2393;
const FLIP_TIMER_INTERVAL			= 10;
const flipFadeDiff					= 10;
const flipFadeMax					= 150;


// some helper functions.
RECT MultiplyRect(const RECT& r, int fac)
{
	RECT ret = r;
	ret.left *= fac;
	ret.right *= fac;
	ret.top *= fac;
	ret.bottom *= fac;

	return ret;
}

SIZE MultiplySize(const SIZE& s, int fac)
{
	SIZE ret = s;
	ret.cx *= fac;
	ret.cy *= fac;

	return ret;
}


class Widget
{
public:
	// desklet handle (hwnd) and DLL handle (dll).
	HWND hwnd;
	HINSTANCE dll;

	// control related handles.
	AVEHANDLE resources;
	AVEHANDLE buttonResources;
	AVEHANDLE container;
	AVEHANDLE editBox;
	AVEHANDLE scrollBar;
	AVEHANDLE goButton;
	AVEHANDLE comboSearchMode;
	AVEHANDLE doneButton;
	AVEHANDLE hyperLink;

	// the mode and colors of this widget.
	enum Modes {FULL = 0, MINI = 1};
	enum Colors {NONE = 0, GREEN = 1, RED = 2};
	Modes mode;
	Colors color;
	enum SearchModes {DICTIONARY = 0, THESAURUS = 1};
	SearchModes searchMode;

	// internal flag if we are in backmode
	BOOL isInBackMode;

	// internal flag keeping track of the flip status
	BOOL isFlipVisible;
	BYTE flipAlpha;
	BOOL flipIsFadingIn;

	// variables that keep track of scrolling the view
	UINT viewBmpHeight;
	UINT viewScroll;

	// sliding variables
	BOOL isSliding;
	int slidingProgress, prevSlidingProgress;
	BOOL isSlidingToFullMode;
	int slidingSpeed;

	BOOL overFlippy;
	HCURSOR oldCursor;

	// variables that keep track of the skin
	std::wstring skinName;
	CSkin skin;

	Widget(HWND hwnd, HINSTANCE dll, const char* ini, const char* group) : hwnd(hwnd), dll(dll),
		mode(MINI), color(NONE), viewBmpHeight(0),
		viewScroll(0), resources(NULL), isInBackMode(FALSE),
		searchMode(DICTIONARY), isFlipVisible(FALSE),
		flipAlpha(0), isSliding(FALSE), slidingProgress(0), isSlidingToFullMode(TRUE), prevSlidingProgress(0),
		overFlippy(FALSE), oldCursor(NULL)
	{
		// first, load the settings
		LoadSettings(ini, group);

		// next, load the skin
		LoadSkin(skinName, FALSE);

		// then clear any possible layers, then
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
		AveFreeResources(buttonResources);
	}

	void LoadSkin(const std::wstring& name, BOOL redraw)
	{
		// this function loads a skin

		WCHAR skinFile[MAX_PATH] = {0};
		GetDLLPathW(dll, skinFile, MAX_PATH);
		wcscat(skinFile, L"skins\\");
		wcscat(skinFile, name.c_str());
		wcscat(skinFile, L"\\skin.ini");

		skin.Read(skinFile);

		// TODO: change the widget graphics to the right skins graphics

	}

	void LoadSettings(const char* ini, const char* group)
	{		
		// this function loads the settings of this desklet
		if(NULL == ini || NULL == group)
		{
			skinName = L"Default";
		}
		else
		{
			// load skin name
			char skin[MAX_PATH] = {0};
			GetPrivateProfileString(group, "skin", "Default", skin, MAX_PATH, ini);
			WCHAR wSkin[MAX_PATH] = {0};
			MultiByteToWideChar(GetACP(), 0, skin, -1, wSkin, MAX_PATH);
			skinName= wSkin;

			// load searchmode
			searchMode = (SearchModes) GetPrivateProfileInt(group, "SearchMode", (INT)searchMode, ini);
		}

	}

	void SaveSettings(const char* ini, const char* group)
	{
		// this function saves the settings of this desklet

		// save skin name
		char skin[MAX_PATH] = {0};
		WideCharToMultiByte(GetACP(), 0, skinName.c_str(), -1, skin, MAX_PATH, NULL, NULL);
		WritePrivateProfileString(group, "skin", skin, ini);

		// save search mode
		WritePrivateProfileInt(group, "SearchMode", (INT)searchMode, ini);
	}

	SIZE GetSize()
	{
		// this function receives the actual size based upon
		// the mode the widget is in.
		SIZE s = {skin.topSize.cx, 0};
		if(Modes::MINI == mode)
			s.cy = skin.topSize.cy + skin.botSize.cy;
		else
			s.cy = skin.topSize.cy + skin.midSize.cy + skin.botSize.cy;

		return s;
	}

	void ClearAllLayers()
	{
		// this function deletes all layers from the desklet.

		while(DeskletLayerCount(hwnd) > 0)
			DeskletLayerRemove(hwnd, 0, FALSE);
	}

	void SetLayerImage(const char* name, const WCHAR* file)
	{
		// this is a shortcut function to set an image
		// from the "images" folder for a layer quickly.

		WCHAR relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, relativePath);

		WCHAR img[MAX_PATH] = {0};
		wcscpy(img, relativePath);
		wcscat(img, L"skins\\");
		wcscat(img, skinName.c_str());
		wcscat(img, L"\\");
		wcscat(img, file);
		wcscat(img, L".png");

		BOOL success = FALSE;
		UINT lid = DeskletLayerGetLayer(hwnd, name, &success);
		if(success)
			DeskletLayerSetImageFileW(hwnd, lid, FALSE, img);	
	}

	UINT AddLayer(const char* name)
	{
		// this is a shortcut function to quickly create a layer
		// which uses the same image as its name.
		WCHAR relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, relativePath);

		WCHAR wName[MAX_PATH]= {0};
		MultiByteToWideChar(GetACP(), 0, name, -1, wName, MAX_PATH);

		WCHAR img[MAX_PATH] = {0};
		wcscpy(img, relativePath);
		wcscat(img, L"skins\\");
		wcscat(img, skinName.c_str());
		wcscat(img, L"\\");
		wcscat(img, wName);
		wcscat(img, L".png");

		UINT bgLayer = DeskletLayerAdd(hwnd, name);
		DeskletLayerSetImageFileW(hwnd, bgLayer, FALSE, img);

		return bgLayer;
	}

	int GetSpeed()
	{
		// returns the animation speed.
		// If the user holds the shift key when
		// the animation needs to start, we'll
		// animate slower.
		int speed = 8;
		if(GetKeyState(VK_SHIFT) < 0)
			speed = 2;

		return speed;
	}

	void SetupBackgroundLayers()
	{
		// this function sets up the background layers.

		// first lock the desklet for invalidating.
		DeskletLockInvalidating(hwnd);

		// we create 4 layers, 3 for the 'background' graphics,
		// one for the background of the input editbox.
		UINT top  = AddLayer("top");
		UINT mid  = AddLayer("mid");
		UINT bot  = AddLayer("bot");
		UINT box  = AddLayer("box");

		// add our flippy
		UINT flip_rollie = AddLayer("flip_rollie");
		UINT flip = AddLayer("flip");
		DeskletLayerSetVisibility(hwnd, flip_rollie, FALSE, FALSE);

		UINT backside = AddLayer("backside");
		DeskletLayerSetVisibility(hwnd, backside, FALSE, FALSE);

		// unlock the desklet for invalidating.
		DeskletUnlockInvalidating(hwnd, FALSE);
	}

	void AnimToMiniMode()
	{
		// this function animates the whole desklet to mini mode
		slidingSpeed = GetSpeed();
		isSliding = TRUE;
		isSlidingToFullMode = FALSE;
		SetTimer(hwnd, SLIDE_ANIM_TIMER_ID, SLIDE_ANIM_TIMER_INTERVAL, NULL);
	}

	void SetMiniMode(BOOL redraw)
	{
		// this function sets the desklet into mini mode.

		UINT top = DeskletLayerGetLayer(hwnd, "top", NULL);
		UINT mid = DeskletLayerGetLayer(hwnd, "mid", NULL);
		UINT bot = DeskletLayerGetLayer(hwnd, "bot", NULL);
		UINT box = DeskletLayerGetLayer(hwnd, "box", NULL);
		UINT flip = DeskletLayerGetLayer(hwnd, "flip", NULL);
		UINT flip_rollie = DeskletLayerGetLayer(hwnd, "flip_rollie", NULL);

		BOOL hasView = FALSE;
		UINT view = DeskletLayerGetLayer(hwnd, "view", &hasView);

		mode = Modes::MINI;
		SIZE s = GetSize();

		// the top and box don't need invalidating, because they'll never
		// move in the animation, so we lock invalidating when we update
		// their position/scales.
		DeskletLockInvalidating(hwnd);

		if(hasView)
			DeskletLayerSetVisibility(hwnd, view, FALSE, FALSE);

		RECT topRect = {0, 0, skin.topSize.cx, skin.topSize.cy};
		DeskletLayerSetPlacement(hwnd, top, FALSE, TRUE, &topRect, s.cx, s.cy);

		RECT boxRect = {skin.boxPlace.x, skin.boxPlace.y, skin.boxPlace.x + skin.boxSize.cx, skin.boxPlace.y + skin.boxSize.cy};
		DeskletLayerSetPlacement(hwnd, box, FALSE, TRUE, &boxRect, s.cx, s.cy);

		//AveSendMessage(scrollBar, AV_HIDE, 0L, 0L);

		ResetEditBoxRect();

		DeskletUnlockInvalidating(hwnd, FALSE);

		// the bottom needs to be replaced
		RECT botRect = {0,skin.topSize.cy, skin.botSize.cx, skin.topSize.cy + skin.botSize.cy};
		DeskletLayerSetPlacement(hwnd, bot, FALSE, TRUE, &botRect, s.cx, s.cy);

		RECT flipRect = {skin.flipPlace.x, skin.flipPlace.y + botRect.top, skin.flipPlace.x + skin.flipSize.cx, skin.flipPlace.y + botRect.top + skin.flipSize.cy};
		DeskletLayerSetPlacement(hwnd, flip_rollie, FALSE, TRUE, &flipRect, s.cx, s.cy);
		DeskletLayerSetPlacement(hwnd, flip, FALSE, TRUE, &flipRect, s.cx, s.cy);

		// the middle section will be made invisiblein mini mode.
		DeskletLayerSetVisibility(hwnd, mid, FALSE, FALSE);

		SetSize(s, redraw);
	}

	void AnimToFullMode()
	{
		// this function animates the whole desklet to full mode
		slidingSpeed = GetSpeed();
		isSliding = TRUE;
		isSlidingToFullMode = TRUE;
		if(mode != FULL)
		{
			SetFullMode(FALSE);
			DoAnimStep(slidingProgress, prevSlidingProgress, TRUE);
			prevSlidingProgress = slidingProgress;
			slidingProgress += slidingSpeed;
		}
		SetTimer(hwnd, SLIDE_ANIM_TIMER_ID, SLIDE_ANIM_TIMER_INTERVAL, NULL);
	}

	void DoAnimStep(int step, int prevStep, BOOL redraw)
	{
		// this function performs a step of the animation.

		// first, retrieve the IDs of the layers we want.
		UINT top = DeskletLayerGetLayer(hwnd, "top", NULL);
		UINT mid = DeskletLayerGetLayer(hwnd, "mid", NULL);
		UINT bot = DeskletLayerGetLayer(hwnd, "bot", NULL);
		UINT flip = DeskletLayerGetLayer(hwnd, "flip", NULL);
		UINT flip_rollie = DeskletLayerGetLayer(hwnd, "flip_rollie", NULL);
		BOOL hasView = FALSE;
		UINT view = DeskletLayerGetLayer(hwnd, "view", &hasView);

		SIZE s = GetSize();

		// next, we draw only THAT section of the middle layer based upon the progress
		// of the animation.
		// FOR EXAMPLE: When the progress is 50%, only half of the layer will be drawn.
		// by clipping the actual image rectangle.

		DeskletLayerSetVisibility(hwnd, mid, FALSE, TRUE);
		RECT midRect = {0, skin.topSize.cy, skin.midSize.cx, skin.topSize.cy + step};
		DeskletLayerSetPlacement(hwnd, mid, FALSE, TRUE, &midRect, s.cx, s.cy);

		if(hasView)
		{
			//DeskletLayerSetVisibility(hwnd, view, FALSE, Color == GREEN);	
			RECT viewRect = GetViewClipRect();

			UINT viewHeight = viewRect.bottom - viewRect.top;
			UINT viewWidth = viewRect.right - viewRect.left;
			UINT viewProgress = min(max(step  -  skin.viewPlace.y, 0), skin.viewSize.cy);
			RECT viewBounds = {0, 0, viewWidth, viewProgress};
			DeskletLayerSetImageClipRect(hwnd, view, FALSE, TRUE, &viewBounds, viewWidth, viewBmpHeight);

			RECT viewPlacement = GetViewRect();
			viewPlacement.bottom = viewProgress + viewPlacement.top;
			DeskletLayerSetPlacement(hwnd, view, FALSE, TRUE, & viewPlacement, s.cx, s.cy);
				
		}

		// draw the scrollbar properly
		RECT scrollBarRect = {skin.scrollBarPlace.x, skin.scrollBarPlace.y, skin.scrollBarPlace.x + skin.scrollBarSize.cx, skin.scrollBarPlace.y + skin.scrollBarSize.cy};
		UINT scrollBarId = AveSendMessage(scrollBar, AV_GET_LAYERID, 0L, 0L);
		UINT scrollBarHeight = scrollBarRect.bottom - scrollBarRect.top;
		UINT scrollBarWidth = scrollBarRect.right - scrollBarRect.left;
		UINT scrollBarProgress = min(max(step + skin.topSize.cy  -  scrollBarRect.top, 0), scrollBarHeight);
		RECT scrollBarBounds = {0, 0, scrollBarWidth, scrollBarProgress};
		DeskletLayerSetImageClipRect(hwnd, scrollBarId, FALSE, TRUE,
			&scrollBarBounds, scrollBarWidth, scrollBarHeight);

		RECT scrollBarPlacementRect = scrollBarRect;
		scrollBarPlacementRect.bottom = scrollBarProgress + scrollBarRect.top;
		DeskletLayerSetPlacement(hwnd, scrollBarId, FALSE, TRUE, & scrollBarPlacementRect, s.cx, s.cy);

		RECT midBounds = {0, 0, skin.midSize.cx, step};
		DeskletLayerSetImageClipRect(hwnd, mid, FALSE, TRUE, &midBounds, skin.midSize.cx, skin.midSize.cy);

		// we also move the bottom layer accordingly.
		RECT botRect = {0,skin.topSize.cy + step, skin.botSize.cx, skin.topSize.cy + skin.botSize.cy + step};
		DeskletLayerSetPlacement(hwnd, bot, redraw, TRUE, &botRect, s.cx, s.cy);
		
		RECT flipRect = {skin.flipPlace.x, skin.flipPlace.y + botRect.top, skin.flipPlace.x + skin.flipSize.cx, skin.flipPlace.y + botRect.top + skin.flipSize.cy};
		DeskletLayerSetPlacement(hwnd, flip_rollie, FALSE, TRUE, &flipRect, s.cx, s.cy);
		DeskletLayerSetPlacement(hwnd, flip, FALSE, TRUE, &flipRect, s.cx, s.cy);
	}

	void SetSize(SIZE& s, BOOL redraw)
	{
		// this function is a shortcut for setting the size of the desklet.
		// we use DeskletMove() rather than DeskletSetSize() to control
		// the redrawing.
		POINT curPos = {0,0};
		DeskletGetPosition(hwnd, &curPos);
		RECT newRect = { curPos.x, curPos.y, curPos.x + s.cx, curPos.y + s.cy};
		DeskletMove(hwnd, &newRect, redraw);

		// we also set the new closebutton position
		DeskletSetClosePoint(hwnd, &skin.closeButtonPlace, s.cx, s.cy);
	}

	void SetFullMode(BOOL redraw)
	{
		// this function sets the desklet into full mode.

		UINT top = DeskletLayerGetLayer(hwnd, "top", NULL);
		UINT mid = DeskletLayerGetLayer(hwnd, "mid", NULL);
		UINT bot = DeskletLayerGetLayer(hwnd, "bot", NULL);
		UINT box = DeskletLayerGetLayer(hwnd, "box", NULL);
		UINT flip = DeskletLayerGetLayer(hwnd, "flip", NULL);
		UINT flip_rollie = DeskletLayerGetLayer(hwnd, "flip_rollie", NULL);
		BOOL hasView = FALSE;
		UINT view = DeskletLayerGetLayer(hwnd, "view", &hasView);

		Modes oldMode = mode;
		mode= Modes::FULL;
		SIZE s = GetSize();

		// the top part (and the editbox) is always on the same place in
		// both modes, so we don't invalidate it.
		// however, we do set the rect/scale again.
		DeskletLockInvalidating(hwnd);

		if(hasView)
		{
			RECT viewRect = GetViewRect();
			DeskletLayerSetPlacement(hwnd, view, FALSE, TRUE, &viewRect, s.cx, s.cy);
			DeskletLayerSetVisibility(hwnd, view, FALSE, color == GREEN);
		}

		RECT topRect = {0, 0, skin.topSize.cx, skin.topSize.cy};
		DeskletLayerSetPlacement(hwnd, top, FALSE, TRUE, &topRect, s.cx, s.cy);

		RECT boxRect = {skin.boxPlace.x, skin.boxPlace.y, skin.boxPlace.x + skin.boxSize.cx, skin.boxPlace.y + skin.boxSize.cy};
		DeskletLayerSetPlacement(hwnd, box, FALSE, TRUE, &boxRect, s.cx, s.cy);

		AveLockContainer(container);
		RECT scrollBarRect = {skin.scrollBarPlace.x, skin.scrollBarPlace.y, skin.scrollBarPlace.x + skin.scrollBarSize.cx, skin.scrollBarPlace.y + skin.scrollBarSize.cy};
		AveSendMessage(scrollBar, AV_SET_RECT, (WPARAM)&scrollBarRect, (LPARAM)&GetSize());
		RECT viewRect = GetViewRect();
		if(color == GREEN && viewBmpHeight > viewRect.bottom - viewRect.top)
			AveSendMessage(scrollBar, AV_SHOW, 0L, 0L);

		ResetEditBoxRect();
		AveUnlockContainer(container);

		DeskletUnlockInvalidating(hwnd, FALSE);

		// the mid layer is made visible again.
		DeskletLayerSetVisibility(hwnd, mid, FALSE, TRUE);
		RECT midRect = {0, skin.topSize.cy, skin.midSize.cx, skin.topSize.cy + skin.midSize.cy};
		DeskletLayerSetPlacement(hwnd, mid, FALSE, TRUE, &midRect, s.cx, s.cy);
		DeskletLayerSetImageClipRect(hwnd, mid, FALSE, FALSE, NULL, 0, 0);

		// the bottom layer is on a different place now.
		RECT botRect = {0,skin.topSize.cy + skin.midSize.cy, skin.botSize.cx, skin.topSize.cy + skin.botSize.cy + skin.midSize.cy};
		DeskletLayerSetPlacement(hwnd, bot, oldMode == mode, TRUE, &botRect, s.cx, s.cy);

		RECT flipRect = {skin.flipPlace.x, skin.flipPlace.y + botRect.top, skin.flipPlace.x + skin.flipSize.cx, skin.flipPlace.y + botRect.top + skin.flipSize.cy};
		DeskletLayerSetPlacement(hwnd, flip_rollie, FALSE, TRUE, &flipRect, s.cx, s.cy);
		DeskletLayerSetPlacement(hwnd, flip, FALSE, TRUE, &flipRect, s.cx, s.cy);

		// finally, resize the desklet.
		if(oldMode != mode)
			SetSize(s, redraw);

	}

	static BOOL __stdcall OnScroll(Widget* w, AVEHANDLE scrollBar)
	{
		// resonds to a scroll action from the scrollbar

		UINT pos = (UINT)AveSendMessage(scrollBar, AV_SCROLLBAR_GET_POS, 0L, 0L);
		return w->ScrollViewTo(pos);
	}

	BOOL ScrollViewTo(UINT pos)
	{
		// scrolls the view to a certain position
		if(pos != viewScroll)
		{
			BOOL hasView = FALSE;
			UINT view = DeskletLayerGetLayer(hwnd, "view", &hasView);
			viewScroll = pos;
			if(hasView)
			{
				AveSendMessage(scrollBar, AV_SCROLLBAR_SET_POS, (WPARAM)pos, 0L);
				RECT clipRect = GetViewClipRect();
				RECT viewRect = GetViewRect();
				DeskletLayerSetImageClipRect(hwnd, view, TRUE, TRUE, &clipRect, viewRect.right - viewRect.left, viewBmpHeight);
				return TRUE;
			}
		}

		return FALSE;
	}

	void SetupControls()
	{
		// get the path relative to this desklet and append it
		// with the folder in this desklet that holds the images for the controls.
		WCHAR resPath[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, resPath);
		wcscat(resPath, L"\\skins\\");
		wcscat(resPath, skinName.c_str());
		wcscat(resPath, L"\\controls\\");

		WCHAR butResPath[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, butResPath);
		wcscat(butResPath, L"\\skins\\");
		wcscat(butResPath, skinName.c_str());
		wcscat(butResPath, L"\\");

		// create some resources which we'll use to base the container on.
		// An container holds and manages the controls we create.
		// The resources are used to tell all the controls based on this container
		// where the necessary graphics are located. The resourceshandle could
		// also perform caching if necessary.
		resources = AveCreateResourcesW(resPath);
		buttonResources = AveCreateResourcesW(butResPath);
		container = AveCreateControlContainer(hwnd, resources);

		// we now lock the container so we can do a batch update of all the controls it holds.
		AveLockContainer(container);

		// we now create a control based upon the container.
		// the control is named "input", which also will be the layer name (so, what's out for conflicts!)
		// we create a control of the type EDITBOX, which is a single-lined inputfield.
		// we don't pass any flags, and we set the reserved parameter nicely to NULL.
		editBox = AveCreateControl(container, "input", AVE_EDITBOX_CONTROL, 0L, NULL);

		// next, we lock the control so we can do a batch update of its properties.
		AveLockControl(editBox);

		// let's set some nice font for our control.
		AveSendMessage(editBox, AV_SET_FONT, 0L, (LPARAM)&skin.boxFont);

		// also set an initial string into the textbox
		AveSendMessage(editBox, AV_SET_TEXT, 0L, (LPARAM)L"search");

		// this inputbox only looses focus on desklet out
		AveSendMessage(editBox, AV_SET_ONLY_LOSE_FOCUS_ON_DESKLET_OUT, (WPARAM)TRUE, 0L);

		// set the callback functions we want.
		// We will receive a callback when the editbox gets the focus, when it looses the focus
		// and when the user pressed enter in the box.
		AveSendMessage(editBox, AV_SET_ONSETFOCUSCALLBACK, (WPARAM)this, (LPARAM)OnSetBoxFocus);
		AveSendMessage(editBox, AV_SET_ONKILLFOCUSCALLBACK, (WPARAM)this, (LPARAM)OnKillBoxFocus);
		AveSendMessage(editBox, AV_SET_ONENTERCALLBACK, (WPARAM)this, (LPARAM)OnBoxEnter);
		AveSendMessage(editBox, AV_SET_TEXTCOLOR, 0L, (LPARAM)skin.boxFontColor.GetValue());

		// create a 'go' button
		goButton = AveCreateControl(container, "gobutton", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveLockControl(goButton);
		AveSendMessage(goButton, AV_SET_RESOURCES, 0L, (LPARAM)buttonResources);
		AveSendMessage(goButton, AV_SET_NORMALIMAGEFILE, (WPARAM)FALSE, (LPARAM)L"GoButton.png");
		AveSendMessage(goButton, AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"GoButtonDown.png");
		AveSendMessage(goButton, AV_SET_RETRIEVEIMAGESFROMRES, (WPARAM)TRUE, 0L);
		AveSendMessage(goButton, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnGoButtonClick);

		// lets position and size this editbox.
		ResetEditBoxRect();

		AveSendMessage(goButton, AV_UPDATE, 0L, 0L);

		// we are done with this control, so we unlock it. Calls to Lock* and Unlock* functions
		// must always occur in pairs.
		AveUnlockControl(goButton);
		AveUnlockControl(editBox);

		// we add the control to the container. Notice that this isn't done implicitely by
		// AveCreateControl(container, ...): we only pass the container handle in that call
		// so the control can base its default properties upon that container.
		// Now, we explicitely add the control to the container so it will receive
		// messages.
		AveAddControlToContainer(container, editBox);
		AveAddControlToContainer(container, goButton);

		// we also send an update message to update the control.
		AveSendMessage(editBox, AV_UPDATE, 0L, 0L);

		RECT scrollBarRect = {skin.scrollBarPlace.x, skin.scrollBarPlace.y, skin.scrollBarPlace.x + skin.scrollBarSize.cx, skin.scrollBarPlace.y + skin.scrollBarSize.cy};
		scrollBar = AveCreateControl(container, "scrollbar", AVE_VERTICAL_SCROLLBAR_CONTROL, 0L, NULL); 
		AveLockControl(scrollBar);
		AveSendMessage(scrollBar, AV_SET_RECT, (WPARAM)&scrollBarRect, (LPARAM)&GetSize());
		AveSendMessage(scrollBar, AV_HIDE, 0L, 0L);
		AveSendMessage(scrollBar, AV_SCROLLBAR_SET_ONSCROLL_CALLBACK, (WPARAM)this, (LPARAM)OnScroll);
		AveUnlockControl(scrollBar);
		AveAddControlToContainer(container, scrollBar);

		// create the backside controls
		RECT comboSearchRect = {skin.comboSearchModePlace.x, skin.comboSearchModePlace.y, skin.comboSearchModePlace.x + skin.comboSearchModeSize.cx, skin.comboSearchModePlace.y + skin.comboSearchModeSize.cy};
		comboSearchMode = AveCreateControl(container, "back_combosearchmode", AVE_DROPDOWNLIST_CONTROL, 0L, NULL);
		// initially, we are not in backmode, so hide this control
		AveLockControl(comboSearchMode);
		AveSendMessage(comboSearchMode, AV_HIDE, 0L, 0L);
		AveSendMessage(comboSearchMode, AV_SET_TEXTCOLOR, 0L, (LPARAM)Color(255,255,255,255).GetValue());
		AveSendMessage(comboSearchMode, AV_SET_RECT, (WPARAM)&comboSearchRect, (LPARAM)&skin.backSideSize);
		AveSendMessage(comboSearchMode, AV_DROPLIST_ADDITEM, 0L, (LPARAM)L"Dictionary");
		AveSendMessage(comboSearchMode, AV_DROPLIST_ADDITEM, 0L, (LPARAM)L"Thesaurus");
		AveSendMessage(comboSearchMode, AV_DROPLIST_SET_SEL, (WPARAM)searchMode, 0L);
		AveSendMessage(comboSearchMode, AV_DROPLIST_SET_ONSELCHANGECALLBACK, (WPARAM)this, (LPARAM)OnSearchModeSelChange);
		AveSendMessage(comboSearchMode, AV_UPDATE, 0L, 0L);
		AveUnlockControl(comboSearchMode);
		AveAddControlToContainer(container, comboSearchMode);

		// done buttons
		RECT doneRect = {skin.doneButtonPlace.x, skin.doneButtonPlace.y, skin.doneButtonPlace.x + skin.doneButtonSize.cx, skin.doneButtonPlace.y + skin.doneButtonSize.cy};
		doneButton = AveCreateControl(container, "back_donebutton", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveLockControl(doneButton);
		AveSendMessage(doneButton, AV_HIDE, 0L, 0L);
		AveSendMessage(doneButton, AV_SET_RECT, (WPARAM)&doneRect, (LPARAM)&skin.backSideSize);
		AveSendMessage(doneButton, AV_SET_NORMALIMAGEFILE, (WPARAM)FALSE, (LPARAM)L"done.png");
		AveSendMessage(doneButton, AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"done_pressed.png");
		AveSendMessage(doneButton, AV_SET_RETRIEVEIMAGESFROMRES, (WPARAM)TRUE, 0L);
		AveSendMessage(doneButton, AV_UPDATE, 0L, 0L);
		AveSendMessage(doneButton, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnDoneButtonPress);
		AveUnlockControl(doneButton);
		AveAddControlToContainer(container, doneButton);

		// create a hyperlink on the backside
		RECT linkRect = {skin.linkPlace.x, skin.linkPlace.y, skin.linkPlace.x + skin.linkSize.cx, skin.linkPlace.y + skin.linkSize.cy};
		hyperLink = AveCreateControl(container, "back_link", AVE_HYPERLINK_CONTROL, 0L, NULL);
		AveLockControl(hyperLink);
		AveSendMessage(hyperLink, AV_HIDE, 0L, 0L);
		AveSendMessage(hyperLink, AV_HYPERLINK_SET_CLICK_CALLBACK, (WPARAM)this, (LPARAM)OnHyperLinkClick);
		AveSendMessage(hyperLink, AV_SET_RECT, (WPARAM)&linkRect, (LPARAM)&skin.backSideSize);
		AveSendMessage(hyperLink, AV_SET_TEXT, 0L, (WPARAM)L"Visit Answers.com");
		AveSendMessage(hyperLink, AV_UPDATE, 0L, 0L);
		AveUnlockControl(hyperLink);
		AveAddControlToContainer(container, hyperLink);
		
		// and finally, unlock the container.
		AveUnlockContainer(container);
	}

	static BOOL __stdcall OnHyperLinkClick(Widget* w, AVEHANDLE link, const POINT* pt, const SIZE* s, DWORD keys)
	{
		// when the hyperlink is pressed (backside), open up answers.com
		ShellExecuteW(NULL, L"open", L"http://www.answers.com", NULL, NULL, SW_SHOW);
		return TRUE;
	}

	static BOOL __stdcall OnSearchModeSelChange(Widget* w, AVEHANDLE control, INT* newSel)
	{
		// reacts to changes in the searchmode combo
		w->searchMode = (SearchModes)*newSel;
		return TRUE;
	}

	static BOOL __stdcall OnDoneButtonPress(Widget* w,AVEHANDLE button, const POINT* pt, const SIZE* s, DWORD keys)
	{
		// when the done button is pressed (backside), flip to frontside again
		w->Flip();
		return TRUE;
	}

	static BOOL __stdcall OnSetBoxFocus(Widget* w, AVEHANDLE control)
	{
		// when the editbox receives the focus, we'll show a
		// background graphic with a 'halo' around it.
		w->SetLayerImage("box", L"boxsel");
		return FALSE;
	}

	static BOOL __stdcall OnKillBoxFocus(Widget* w, AVEHANDLE control)
	{
		// when the editbox looses the focus, we simply revert back
		// to the old graphic without the 'halo'.
		w->SetLayerImage("box", L"box");
		return FALSE;
	}

	static std::wstring MakeNiceStringFromHTML(const std::wstring& ws)
	{
		// strips HTML from a string
		std::wstring ret;
		ret.reserve(ws.length());
		bool isInHTML(false);
		for(std::wstring::const_iterator iter = ws.begin(); iter != ws.end(); ++iter)
		{
			wchar_t wch = *iter;

			if(L'<' == wch)
			{
				// be sure to also convert BR
				if(iter+2 < ws.end() && tolower(*(iter+1)) == 'b' && tolower(*(iter+2)) == 'r')
					ret += '\n';

				isInHTML = true;
			}
			else if(L'>' == wch)
				isInHTML = false;
			else if(!isInHTML)
				ret += wch;
		}

		return ret;
	}

	BOOL CollectData(const std::wstring& word, std::vector<std::wstring>& items)
	{
		// this function collects the data for a word from the source website
		// It just scrapes the results from the HTML

		// we add one word ourself: avedesk :)
		if(wcsicmp(word.c_str(), L"avedesk") == 0)
		{
			items.push_back(L"A desktop enhancer created by Andreas Verhoeven. AveDesk allows pretty icons and so-called widgets (mini-applications) to be added to Windows desktops."); 
			return TRUE;
		}

		const std::wstring dictionaryStart(L"<div id=\"Dictionary\"");
		const std::wstring thesaurusStart(L"<div id=\"Thesaurus\"");
		const std::wstring divIdStart(L"<div id=");
		const std::wstring itemStart(L"<li>");
		const std::wstring itemEnd(L"</li>");
		const std::wstring baseUrl(L"http://www.answers.com/");

		std::wstring url(baseUrl + CAveHTTP::Encode(word));
		std::wstring out;

		BOOL bRes = FALSE;
		CAveHTTP http(L"AveDesk");
		if(http.RetrievePage(url, out))
		{
			std::wstring startString;
			if(DICTIONARY == searchMode)
				startString = dictionaryStart;
			else if(THESAURUS == searchMode)
				startString = thesaurusStart;

			size_t dictStart(out.find(startString));
			if(dictStart != std::wstring::npos)
			{
				out = out.substr(dictStart + startString.length(), out.length() - dictStart - startString.length());
				size_t EndDictPos = out.find(divIdStart);
				out = out.substr(0, EndDictPos);
				size_t itemStartPos;
				while((itemStartPos = out.find(itemStart)) != std::wstring::npos)
				{
					size_t itemEndPos;
					if(DICTIONARY == searchMode)
						itemEndPos = out.find(itemEnd);
					else
					{
						itemEndPos = out.find(itemStart,itemStartPos);
						size_t itemEndPos2 = out.find(L"<ol>", itemStartPos);
						if(itemEndPos2 < itemEndPos && itemEndPos2 != std::wstring::npos)
							itemEndPos = itemEndPos2;
					}

					size_t startOfWordPos = itemStartPos + itemStart.length();
					items.push_back(MakeNiceStringFromHTML(out.substr(startOfWordPos, itemEndPos - startOfWordPos)));

					size_t newPos = itemEndPos + itemEnd.length();
					out = out.substr(newPos, out.length() - newPos);

					bRes = TRUE; // this function succeeds if we have at least one hit
				}
			}
		}

		return bRes;
	}

	Bitmap* CreateView(const std::vector<std::wstring>& items)
	{
		// this function creates a view from items. The
		// view is just a bitmap which will be returned.
		UINT totalItemHeight = 0;
		UINT width = skin.viewSize.cx;
		FontFamily fontFamily(skin.viewFont.faceName);
		FontStyle fs = (FontStyle)skin.viewFont.style;
		REAL fontSize = (REAL)skin.viewFont.size;
		int left = 5; // line out by 5 pixels
		Pen dummyPen(skin.viewFontColor, 1);
		StringFormat strFormat;

		// add each item as a string on the path
		GraphicsPath path;
		for(std::vector<std::wstring>::const_iterator iter = items.begin(); iter != items.end(); ++iter)
		{
			const std::wstring& ws = *iter;
			Rect rc(left, totalItemHeight, width - left, 0x10000000);
			path.AddString(ws.c_str(),-1,&fontFamily,fs,fontSize,rc,&strFormat);

			Rect rcBounds;
			path.GetBounds(&rcBounds, NULL, &dummyPen);
			totalItemHeight = rcBounds.Height;
		}

		// create the bitmap that will be our view with the right height
		Bitmap* bmp = new Bitmap(width, totalItemHeight);

		// fill the path, which will make sure we have nice, smooth fonts
		Graphics g(bmp);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		SolidBrush black(skin.viewFontColor);
		g.FillPath(&black, &path);

		return bmp;
	}

	void SetViewJoke(const std::wstring& word)
	{
		// just a small joke.. when my name is entered - we show a
		// 'certain' image instead of a text
		WCHAR path[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, path);
		wcscat(path, L"images\\");
		wcscat(path, word.c_str());
		wcscat(path, L".png");
		Bitmap* bmp = DeskletLoadGDIPlusImageW(path);

		std::vector<std::wstring> dummy;
		SetView(dummy, bmp);
	}

	void SetView(const std::vector<std::wstring>& items, Bitmap* diffBitmap = NULL)
	{
		// this functions sets the view for the items that are passed
		// as a parameter.

		// first, make sure there is a view layer
		BOOL success = FALSE;
		UINT viewLayer = DeskletLayerGetLayer(hwnd, "view", &success);
		if(!success)
		{
			viewLayer = DeskletLayerAdd(hwnd, "view");

			// we don't want hittesting on the view-layer, because we
			// also want to receive layer messages for it when the mouse
			// is over a non-character, thus transparent, area
			DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, viewLayer, TRUE);
		}

		// show the view only if we are in FULL mode.
		DeskletLayerSetVisibility(hwnd, viewLayer, FALSE, mode == FULL);

		// create a bitmap for the view
		Bitmap* bmp = diffBitmap;
		if(NULL == bmp)
			bmp = CreateView(items);
		viewBmpHeight = bmp->GetHeight();

		// reset the scrollbar: set scrolling to zero/none, set the thumbsize of the scrollbar and
		// only make the bar visible if scrolling is really needed.
		viewScroll = 0;
		AveLockControl(scrollBar);
		AveSendMessage(scrollBar, AV_SCROLLBAR_SET_POS, 0L, 0L);
		AveSendMessage(scrollBar, AV_SCROLLBAR_SET_THUMBSIZE, skin.viewSize.cy, max(viewBmpHeight, skin.viewSize.cy));
		AveSendMessage(scrollBar, AV_SCROLLLBAR_SET_SCALE, max(viewBmpHeight, skin.viewSize.cy) - skin.viewSize.cy, 0L);

		// FIX for 1.3 controls bug
		DeskletLayerSetVisibility(hwnd,AveSendMessage(scrollBar, AV_GET_LAYERID, 0L,0L), FALSE, viewBmpHeight > skin.viewSize.cy);

		AveSendMessage(scrollBar, viewBmpHeight > skin.viewSize.cy ? AV_SHOW : AV_HIDE, 0L, 0L);
		AveUnlockControl(scrollBar);

		// place the view layer at the right position
		SIZE s = GetSize();
		RECT viewPlacement = GetViewRect();
		DeskletLayerSetPlacement(hwnd, viewLayer, FALSE, TRUE, &viewPlacement, s.cx, s.cy);

		// update the clip rectangle of the view for the current scrollposition
		RECT clipRect = GetViewClipRect();
		DeskletLayerSetImageClipRect(hwnd, viewLayer, FALSE, TRUE, &clipRect, bmp->GetWidth(), bmp->GetHeight());

		// and finally, use the view bitmap we've just created
		// as the bitmap for this layer so the user will actually see something :-)
		DeskletLayerSetImage(hwnd, viewLayer, FALSE, bmp, TRUE);
	}

	RECT GetViewRect()
	{
		// returns the view placement rectangle
		RECT viewPlacement = {skin.viewPlace.x, skin.viewPlace.y + skin.topSize.cy , skin.viewSize.cx + skin.viewPlace.x, min(viewBmpHeight, skin.viewSize.cy) + skin.viewPlace.y + skin.topSize.cy};
		return viewPlacement;
	}

	RECT GetViewClipRect()
	{
		// returns the view's clip rectangle
		RECT clipRect = {0, viewScroll,  skin.viewSize.cx, min(viewBmpHeight, skin.viewSize.cy) + viewScroll};
		return clipRect;
	}

	void HideView()
	{
		// this function will hides the view

		// hide the view if it's already made
		BOOL success = FALSE;
		UINT viewLayer = DeskletLayerGetLayer(hwnd, "view", &success);
		if(success)
			DeskletLayerSetVisibility(hwnd, viewLayer, FALSE, FALSE);

		// hide the scrollbar also. Lock it, so their won't 
		// be any unnecessary updating.
		AveLockControl(scrollBar);
		AveSendMessage(scrollBar, AV_HIDE, 0L, 0L);
		AveUnlockControl(scrollBar);
	}

	static BOOL __stdcall OnGoButtonClick(Widget* w,AVEHANDLE button, const POINT* pt, const SIZE* s, DWORD keys)
	{
		// when the user pressed enter in our editbox,
		// we'll do input validating.
		return w->LookUp();
	}

	static BOOL __stdcall OnBoxEnter(Widget* w, AVEHANDLE box)
	{
		// when the user pressed enter in our editbox,
		// we'll do input validating.
		return w->LookUp();
	}

	BOOL LookUp()
	{
		// let's receive the text the user typed into the editbox
		// by getting the length of the text, allocating a buffer
		// with suffient space and request the control to fill that buffer
		// with the actual text(AV_GET_TEXT).
		int textLen = AveSendMessage(editBox, AV_GET_TEXT_LENGTH, 0L, 0L);
		WCHAR* buf = new WCHAR[textLen + 1];
		buf[0] = '\0';
		AveSendMessage(editBox, AV_GET_TEXT, (WPARAM)textLen+1, (LPARAM)buf);
		std::wstring word = buf;
		delete[] buf;

		BOOL bRes = FALSE;

		// small joke
		if(wcsicmp(word.c_str(), L"andreas verhoeven") == 0)
		{
			SetViewJoke(L"ave");
			bRes = TRUE;
		}
		// another small joke ;)
		else if(wcsicmp(word.c_str(), L"christopher royer") == 0 || wcsicmp(word.c_str(),L"lelijk") == 0)
		{
			SetViewJoke(L"chris");
			bRes = TRUE;
		}
		else
		{
			std::vector<std::wstring> items;
			bRes = CollectData(word, items);
			if(bRes)
				SetView(items);
			else
				HideView();
		}

		// check what new color the widget should be
		// and update the graphics if necessary.
		Colors oldColor = color;

		// some sample validation
		if(bRes)
			color = GREEN;
		else if(word.empty())
			color = NONE;
		else
			color = RED;

		// only change the graphics if we really need to, that is,
		// if they are in a different color than we currently are.
		if(color != oldColor)
		{
			if(color == GREEN)
			{
				// for green... well... we set the green graphics.
				SetLayerImage("top", L"topgreen");	
				SetLayerImage("mid", L"midgreen");
				SetLayerImage("bot", L"botgreen");

				// when the input is okay and when the widget isn't already 
				// in full mode, animate to full mode.
				if(mode != FULL && !word.empty())
					AnimToFullMode();
				else
				{
					// invalidate an empty rectangle, but issue a redraw.
					RECT r = {0};
					SIZE s= {0};
					DeskletInvalidateRect(hwnd, &r, &s, TRUE);
				}	
			}
			else if(color == RED)
			{
				// idem for red.
				SetLayerImage("top", L"topred");	
				SetLayerImage("mid", L"midred");
				SetLayerImage("bot", L"botred");

				HideView();

				// when the input is okay and when the widget isn't already 
				// in full mode, animate to full mode.
				if(mode != FULL && !word.empty())
					AnimToFullMode();
				else
				{
					// invalidate an empty rectangle, but issue a redraw.
					RECT r = {0};
					SIZE s= {0};
					DeskletInvalidateRect(hwnd, &r, &s, TRUE);
				}
			}
			else
			{
				// idem for none, but...
				SetLayerImage("top", L"top");	
				SetLayerImage("mid", L"mid");
				SetLayerImage("bot", L"bot");

				// .. we also animate back to mini mode if necessary.
				if(mode != MINI)
				{
					AnimToMiniMode();
				}
				else
				{
					// invalidate an empty rectangle, but issue a redraw.
					RECT r = {0};
					SIZE s= {0};
					DeskletInvalidateRect(hwnd, &r, &s, TRUE);
				}
			}
		}
		else
		{
			if(mode != FULL && !word.empty())
				AnimToFullMode();
		}
		
		return TRUE;
	}
	void ResetEditBoxRect()
	{
		// this function resets the editboxs position and scale.
		// because we use an alpha-blended background graphic behind the actual input box,
		// we'll set it accordingly to this background graphic, but with a small offset.
		const int offsetX = 10;
		const int offsetY = 6;
		RECT r = {skin.boxPlace.x + offsetX, skin.boxPlace.y + offsetY, skin.boxPlace.x + skin.boxSize.cx - 1 - skin.goSize.cx, skin.boxPlace.y + skin.boxSize.cy - offsetY};
		RECT goRect = {skin.goPlace.x, skin.goPlace.y, skin.goPlace.x + skin.goSize.cx, skin.goPlace.y + skin.goSize.cy};
		SIZE s = GetSize();

		// here we use a TRICK to make the editbox text looks smoother,
		// we have set a twice as big font, and we now set the rect and scale but multiplied by 2.
		RECT r2 = MultiplyRect(r, 2);
		SIZE s2 = MultiplySize(s, 2);

		AveSendMessage(editBox, AV_SET_RECT, (WPARAM)&r2, (LPARAM) &s2);

		AveSendMessage(goButton, AV_SET_RECT, (WPARAM)&goRect, (LPARAM)&s);
	}

	void Flip()
	{
		// this function flips the desklet to the backside or frontside

		// we set up the flipParams structure used by DeskletFlip() first
		FlipParams params = {0};
		params.flipToBack = !isInBackMode;
		params.useLayers = TRUE;

		// next we determine the proper sizes for backa and front
		// if we are in back mode, those are swapped
		SIZE newSize = isInBackMode ? GetSize() : skin.backSideSize;
		SIZE oldSize = !isInBackMode ? GetSize() : skin.backSideSize;
		params.oldDeskletSize = &oldSize;
		params.newDeskletSize = &newSize;

		// we define the frontLayers
		UINT frontLayers[] = {
							DeskletLayerGetLayer(hwnd, "top", NULL),
							DeskletLayerGetLayer(hwnd, "bot", NULL),
							DeskletLayerGetLayer(hwnd, "box", NULL),
							DeskletLayerGetLayer(hwnd, "input", NULL),
							DeskletLayerGetLayer(hwnd, "gobutton", NULL),
							DeskletLayerGetLayer(hwnd, "flip", NULL),
							DeskletLayerGetLayer(hwnd, "flip_rollie", NULL),
							DeskletLayerGetLayer(hwnd, "mid", NULL),
							DeskletLayerGetLayer(hwnd, "view", NULL),
							DeskletLayerGetLayer(hwnd, "scrollbar", NULL)
						};

		// and the backlayers
		UINT backLayers[]  = { 
							DeskletLayerGetLayer(hwnd, "backside", NULL),
							DeskletLayerGetLayer(hwnd, "back_combosearchmode", NULL),
							DeskletLayerGetLayer(hwnd, "back_donebutton", NULL),
							DeskletLayerGetLayer(hwnd, "back_link", NULL)
						};


		// and give them to the flipparam structure, together
		params.oldLayersVisible = frontLayers;
		params.newLayersVisible = backLayers;
		params.oldLayersVisibleCount = sizeof(frontLayers) / sizeof(frontLayers[0]);
		params.newLayersVisibleCount = sizeof(backLayers) / sizeof(backLayers[0]);

		// next, we reduce the number of front layers by determining if certain
		// layers should be visible. The layers that can be hidden, are placed
		// at the end of the layers array, so we can simply reduce the count of
		// the front layers, and thus use a static array, instead a dynamic array.
		if(mode != FULL)
		{
			// not in full mode, the scrollbar & view are not visible, 
			// as well as the mid layer which is hidden
			params.oldLayersVisibleCount -= 3;
		}
		else
		{
			// if we dont have a view (yet), or the view is not visible
			// because of a bad word(color == RED), then dont show them
			// neither.
			// If the view is visible, but the scrollbar is not needed
			// to be visible, dont show the scrollbar.
			BOOL hasView=FALSE;
			DeskletLayerGetLayer(hwnd, "view", &hasView);
			if(!hasView || color != GREEN)
				params.oldLayersVisibleCount -= 2;
			else if(viewBmpHeight <= GetViewRect().bottom - GetViewRect().top)
				params.oldLayersVisibleCount -= 1;
		}

		// if we are in backmode, we flip the other way around, so 
		// we simply swap the front and backlayers
		if(isInBackMode)
		{
			params.oldLayersVisible = backLayers;
			params.newLayersVisible = frontLayers;

			UINT temp  = params.oldLayersVisibleCount;
			params.oldLayersVisibleCount = params.newLayersVisibleCount;
			params.newLayersVisibleCount = temp;
		}

		// made invisible
		isFlipVisible = FALSE;

		// flip the desklet and change our status flag.
		DeskletFlip(hwnd, &params);
		isInBackMode = !isInBackMode;
	}
};



void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	// give the world some information about this widget.
	strcpy(szName,"Word Seacher");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"This desklet allows you to search words in a dictionary or in a thesaurus.");
	*iVersion = 130;
}


Widget* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	Widget* t = new Widget(hwndDesklet, hInstanceDll, ini, group);
	if(NULL == ini || NULL == group)
	{
		// This widget does not use any labels when its started for the first time.
		// We can see that the widget is started for the first time
		// by seeing that ini == NULL or group == NULL
		DeskletSetLabel(hwndDesklet, "");
		DeskletSetSubLabel(hwndDesklet, "");
	}
	else
	{
		// we currently don't load nor save any properties, but
		// this would be the right place to do so.
	}

	// this widget always starts in mini mode. 	// just redraw, for kicks.
	t->SetMiniMode(FALSE);
	DeskletRedraw(hwndDesklet);

	return t;
}

void __stdcall OnDestroy(Widget* t, HWND hwndDesklet)
{
	// when our widget is destroyed,
	// delete the widget object which does further cleaning.
	delete t;
}


BOOL __stdcall OnLayerClick(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// when we receive a layer click, we forward this mouse click to the container,
	// which, in turns, will forward it to the right control. If the container or its controlled
	// handled the mouse clikc, TRUE has been returned. Otherwise, FALSE.
	if(AveForwardMouseClick(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	// if we click the flippy, flip to the back, otherwise
	// when the user clicked any other part of the widget besides any controls,
	// go back into mini mode if necessary.
	if(strcmp(layerName, "flip") == 0 || strcmp(layerName, "flip_rollie") == 0)
		t->Flip();
	else if(t->mode == Widget::Modes::FULL && strcmp(layerName, "view") != 0 && !t->isInBackMode)
		t->AnimToMiniMode();		

	// let's know we handle this.
	return TRUE;
}

BOOL __stdcall OnLayerRightClick(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// when we receive a layer right click, we forward this mouse click to the container,
	// which, in turns, will forward it to the right control. If the container or its controlled
	// handled the mouse rightclick, TRUE has been returned. Otherwise, FALSE.
	if(AveForwardMouseRightClick(t->container, layerId, layerName, pt, s, keys))
		return TRUE;		

	// let's know we don't handle this.
	return FALSE;
}

BOOL __stdcall OnLayerMouseDown(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// we'll also forward layer mouse down notifications.
	BOOL bRet = AveForwardMouseDown(t->container, layerId, layerName, pt, s, keys);

	// if any other layer is clicked, give control back to the input box.
	//if(!bRet)
		//AveSetFocusToControl(t->container, "input");

	return bRet;
}

BOOL __stdcall OnLayerMouseWheelForward(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	// forward mouse wheel notifications.
	if(AveForwardMouseWheelForward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;

	// lets handle scrollwheel actions when the mouse is over the view
	if(strcmp(layerName, "view") == 0)
		t->ScrollViewTo(max(((int)t->viewScroll) - 12, 0));		

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelBackward(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(AveForwardMouseWheelBackward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;

	// lets handle scrollwheel actions when the mouse is over the view
	if(strcmp(layerName, "view") == 0)
		t->ScrollViewTo(min(t->viewScroll + 12, t->viewBmpHeight - (t->GetViewRect().bottom - t->GetViewRect().top)));

	return FALSE;
}

BOOL __stdcall OnLayerMouseUp(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseUp(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseOn(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(AveForwardMouseOn(t->container, layerId, layerName, pt, s))
		return TRUE;

	if(strcmp(layerName, "flip") == 0 || strcmp(layerName, "flip_rollie") == 0)
	{
		if(!t->overFlippy)
		{
			t->overFlippy = TRUE;
			HCURSOR hand = LoadCursor(NULL, IDC_HAND);
			t->oldCursor = DeskletSetCursor(t->hwnd, hand);
		}
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseMove(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(AveForwardMouseMove(t->container, layerId, layerName, pt, s))
		return TRUE;

	return FALSE;
}


BOOL __stdcall OnLayerMouseOut(Widget* t, UINT layerId, const char* layerName)
{
	if(AveForwardMouseOut(t->container, layerId, layerName))
		return TRUE;

	if(strcmp(layerName, "flip") == 0 || strcmp(layerName, "flip_rollie") == 0)
	{
		if(t->overFlippy)
		{
			t->overFlippy = FALSE;
			t->oldCursor = DeskletSetCursor(t->hwnd, t->oldCursor);
		}
	}

	return FALSE;
}


void __stdcall OnSave(Widget* w, const char* ini, const char* group, BOOL bIsForExport)
{
	// just let the widget save our settings
	w->SaveSettings(ini, group);
}
void __stdcall OnProcessMessage(Widget* t, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!t)
		return;

	if(t->container != NULL)
	{
		// we also forward windows messages to our container.
		// this way, the inputbox can respond to keys being pressed, for example.
		if(AveForwardMessage(t->container, msg, wParam, lParam))
			return;
	}

	if(WM_SETFOCUS == msg && t->container != NULL)
	{
		// if the widget receives the focus, we'll set the focus of the control container
		// to the input box named "input". Notice we could also have used the OnSelect()
		// function, but this is just a demonstration of how to handle window messages.
		AveSetFocusToControl(t->container, "input");
	}
	else if(WM_TIMER == msg && FLIP_FADE_TIMER_ID == wParam)
	{
		// this timer controls the flippy fade-in aniamtion
		UINT flip_rollie = DeskletLayerGetLayer(t->hwnd, "flip_rollie", NULL);

		// calculate the new alpha level for the flippy based
		// on the fade direction (in/out)
		if(t->flipIsFadingIn)
			t->flipAlpha += flipFadeDiff;
		else
			t->flipAlpha -= flipFadeDiff;

		// set the layers alpha
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
	else if(WM_TIMER == msg && SLIDE_ANIM_TIMER_ID == wParam)
	{
		// this timer controls the slide animation

		if(t->isSlidingToFullMode)
		{
			// if we are sliding to full mode, just
			// draw the next 'slide animation frame' until we reached
			// the final full mode size
			if(t->slidingProgress < t->skin.midSize.cy)
			{
				t->DoAnimStep(t->slidingProgress, t->prevSlidingProgress, TRUE);
				t->prevSlidingProgress = t->slidingProgress;
				t->slidingProgress += t->slidingSpeed;
			}
			else
			{
				// if we reached the full mode, just stop
				// the animation
				t->slidingProgress = 0;
				t->prevSlidingProgress = 0;
				KillTimer(hwnd, SLIDE_ANIM_TIMER_ID);
				t->isSliding = FALSE;

				// in case slidingProgress was not equal to midSize
				if(t->slidingProgress != t->skin.midSize.cy)
					t->SetFullMode(TRUE);
			}
		}
		else
		{
			// when sliding to mini mode

			// as long as we are not in mini mode,
			// do the next frame of the animation
			if(t->slidingProgress < t->skin.midSize.cy)
			{
				t->DoAnimStep(t->skin.midSize.cy - t->slidingProgress, t->skin.midSize.cy - t->prevSlidingProgress, TRUE);
				t->prevSlidingProgress = t->slidingProgress;
				t->slidingProgress += t->slidingSpeed;;
			}
			else
			{
				// if we are in mini mode, stop the animation
				t->slidingProgress = 0;
				t->prevSlidingProgress = 0;
				KillTimer(hwnd, SLIDE_ANIM_TIMER_ID);
				t->isSliding = FALSE;
				t->SetMiniMode(t->slidingProgress != t->skin.midSize.cy);
			}

		}
	}
}

BOOL __stdcall OnMouseMove(Widget* t, int x, int y)
{
	// if the flippy is not visible and we are not in backmode (when the flippie
	// shouldn't be visible), start to fade the flippy in
	if(!t->isFlipVisible && !t->isInBackMode)
	{
		UINT flip_rollie = DeskletLayerGetLayer(t->hwnd, "flip_rollie", NULL);
		DeskletLayerSetVisibility(t->hwnd, flip_rollie, FALSE, TRUE);
		t->isFlipVisible = TRUE;

		t->flipIsFadingIn = TRUE;
		SetTimer(t->hwnd, FLIP_FADE_TIMER_ID, FLIP_TIMER_INTERVAL, NULL);
	}

	return TRUE;
}

BOOL __stdcall OnMouseOut(Widget* t)
{
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