//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.


// SCRIBBLER WIDGET.
// by Andreas Verhoeven, 2005.
// Allows one to draw notes

// STATUS:
// + completely done


// DeskletSDK for Desklet* functions.
#include "../DeskletSDK.h"


// AveControls specific includes
#include "../AveControls/aveconstants.h"
#include "../ControlsDLL/AveControls.h"
#pragma comment(lib, "../AveControls.lib")

// some C++ includes
#include <cstring>	// for std::basic_string<T> operator==()
#include <string>	// for std::wstring
#include <sstream>	// for std::wstringstream
#include "skin.h"	// skin related stuff
#include <shlwapi.h>// for Path* functions
#include <objbase.h>// for CoCreateGuid
#include <shlobj.h>	// for DROPFILES
#include <windowsx.h>// for GET_X_PARAM and GET_Y_PARAM macros

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ole32.lib")

#include "skinenumerator.h"
#include "resource.h"


// fade animation constants
const FLIP_FADE_TIMER_ID			= 2393;
const FLIP_TIMER_INTERVAL			= 10;
const flipFadeDiff					= 20;
const flipFadeMax					= 255;

const CLEAR_ANIM_TIMER_ID			= 3004;
const CLEAR_ANIM_TIMER_INTERVAL		= 10;
const CLEAR_ANIM_DELTA				= 8;

const SIZE_BIG						= 40;
const SIZE_MEDIUM					= 20;
const SIZE_SMALL					= 10;

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

std::wstring CreateGUIDString()
{
	std::wstring ret;

	LPOLESTR wszCLSID = NULL;
	GUID guid = {0};
	CoCreateGuid(&guid);
	StringFromCLSID(guid, &wszCLSID);
	int nLen = lstrlenW(wszCLSID);

	ret = wszCLSID;
	CoTaskMemFree(wszCLSID);

	return ret;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

#include <map>
#include <sstream>

class Widget
{
public:
	// desklet handle (hwnd) and DLL handle (dll).
	HWND hwnd;
	HINSTANCE dll;

	// control related handles.
	AVEHANDLE resources;
	AVEHANDLE container;

	AVEHANDLE butBig;
	AVEHANDLE butMed;
	AVEHANDLE butSmall;
	AVEHANDLE doneBut;
	AVEHANDLE butClear;

	std::map<AVEHANDLE, ColorEntry> colorButtons;
	std::wstring guid;

	Bitmap* bmp;
	Graphics* g;

	BOOL hasCapture;

	POINT prevPoint;

	BOOL flipVisible;
	BOOL isInBackMode;

	HCURSOR oldCursor;

	UINT clearProgress;

	CSkin skin;

	UINT brushSize;
	Color brushColor;
	AVEHANDLE selectedColorButton;

	// internal flag keeping track of the flip status
	BOOL isFlipVisible;
	UINT flipAlpha;
	BOOL flipIsFadingIn;

	Widget(HWND hwnd, HINSTANCE dll, const char* group, const char* ini) : hwnd(hwnd), dll(dll), bmp(NULL), g(NULL), hasCapture(FALSE),
		isFlipVisible(FALSE),
		flipAlpha(0),
		isInBackMode(FALSE),
		brushSize(20),
		brushColor(250, 250, 250, 250),
		selectedColorButton(NULL),
		clearProgress(0)
	{
		DeskletActivateExtendedDragDrop(hwnd);

		FORMATETC f = {0};
		f.cfFormat = CF_HDROP;
		f.dwAspect = DVASPECT_CONTENT;
		f.lindex = -1;
		f.ptd = 0;
		f.tymed = TYMED_HGLOBAL;
		DeskletAddDragDropFormat(hwnd,&f);

		prevPoint.x = 0;
		prevPoint.y = 0;

		skin.dir = L"ChalkBoard";

		if(group != NULL && ini != NULL)
		{
			WCHAR wGuid[1024] = {0};
			GetPrivateProfileStringW(StringToWString(group).c_str(), L"GUID", L"" ,wGuid, 1024, StringToWString(ini).c_str());
			guid = wGuid;

			brushSize = GetPrivateProfileIntW(StringToWString(group).c_str(), L"BrushSize", brushSize, StringToWString(ini).c_str());
			brushColor.SetValue(GetPrivateProfileIntW(StringToWString(group).c_str(), L"BrushColor", brushColor.GetValue(), StringToWString(ini).c_str()));

			WCHAR wSkinDir[MAX_PATH] = {0};
			GetPrivateProfileStringW(StringToWString(group).c_str(), L"SkinDir", skin.dir.c_str(), wSkinDir, MAX_PATH, StringToWString(ini).c_str());
			skin.dir = wSkinDir;
		}
		

		if(guid.length() == 0)
		{
			guid = CreateGUIDString();
			guid = guid.substr(1, guid.length()-2);
		}


		LoadSkin(skin.dir.c_str());
		if(NULL == group || NULL == ini)
		{
			brushColor.SetValue(skin.defaultColor);
		}

		// first clear any possible layers, then
		// setup OUR background layers,
		// and finally, setup the controls.
		ClearAllLayers();
		SetupBackgroundLayers();
		SetupDrawingLayer();
		SetupControls();

		ApplySkin();

		UpdateSizeButton();
	}


	~Widget()
	{
		// let's not forget to clean up our own mess....
		AveFreeControlContainer(container);
		AveFreeResources(resources);

		delete g;
		delete bmp;
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

		brushColor.SetValue(skin.defaultColor);

		for(std::map<AVEHANDLE, ColorEntry>::iterator iter = colorButtons.begin(); iter != colorButtons.end(); ++iter)
		{
			std::wstringstream ss;
			ss << L"colorbutton" << iter->second.name; 

			if(iter->first != NULL)
			{
				AveRemoveControlFromContainer(container, WStringToString(ss.str()).c_str());
				AveFreeControl(iter->first);
			}
		}
		colorButtons.clear();
		selectedColorButton = NULL;
	}

	void ApplySkin()
	{
		SkinInfo skinInfo = {0};
		skinInfo.dwSize = sizeof(skinInfo);
		skinInfo.wAuthor = const_cast<WCHAR*>(skin.author.c_str());
		skinInfo.wName = const_cast<WCHAR*>(skin.name.c_str());
		skinInfo.wNotes = const_cast<WCHAR*>(skin.comment.c_str());

		int version(130);
		std::wstringstream ws;
		ws << skin.version;
		ws >> version;
		skinInfo.version = version;
		DeskletSetSkinInfoW(hwnd, &skinInfo);

		DeskletSetSize(hwnd, &skin.front);
		DeskletSetClosePoint(hwnd, &skin.closePos, skin.front.cx, skin.front.cy);

		UINT front  = DeskletLayerGetLayer(hwnd, "frontside", FALSE);
		SetImageFileToLayer(front, L"frontside.png");

		UINT back  = DeskletLayerGetLayer(hwnd, "backside", FALSE);
		SetImageFileToLayer(back, L"backside.png");

		UINT flipLayer = DeskletLayerGetLayer(hwnd, "flip", FALSE);
		SetImageFileToLayer(flipLayer, L"flip.png");
		DeskletLayerSetPlacement(hwnd, flipLayer, FALSE, TRUE, &skin.flippie, skin.front.cx, skin.front.cy);

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

		AveLockControl(butClear);
		// bugfix: image wasn't updated for the button, so we reset the names with an invalid file
		AveSendMessage(butClear,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butClear,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butClear,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"clear.png");
		AveSendMessage(butClear,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"clear_sel.png");
		AveSendMessage(butClear, AV_SET_RETRIEVEIMAGESFROMRES, TRUE, 0L);
		AveSendMessage(butClear, AV_SET_RECT, (WPARAM)&skin.butClear, (LPARAM)&skin.front);
		AveSendMessage(butClear, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnClearButtonClick);
		AveSendMessage(butClear, AV_UPDATE, 0L, 0L);
		AveUnlockControl(butClear);

		AveLockControl(butBig);
		// bugfix: image wasn't updated for the button, so we reset the names with an invalid file
		AveSendMessage(butBig,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butBig,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butBig,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"big.png");
		AveSendMessage(butBig,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"big_sel.png");
		AveSendMessage(butBig,AV_SET_PRESSEDIMAGEFILE, FALSE, (LPARAM)L"big_sel.png");
		AveSendMessage(butBig, AV_SET_RETRIEVEIMAGESFROMRES, TRUE, 0L);
		AveSendMessage(butBig, AV_SET_RECT, (WPARAM)&skin.butBig, (LPARAM)&skin.front);
		AveSendMessage(butBig, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnSizeButtonClick);
		AveSendMessage(butBig, AV_UPDATE, 0L, 0L);
		AveUnlockControl(butBig);

		AveLockControl(butMed);
		// bugfix: image wasn't updated for the button, so we reset the names with an invalid file
		AveSendMessage(butMed,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butMed,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butMed,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"med.png");
		AveSendMessage(butMed,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"med_sel.png");
		AveSendMessage(butMed,AV_SET_PRESSEDIMAGEFILE, FALSE, (LPARAM)L"med_sel.png");
		AveSendMessage(butMed, AV_SET_RETRIEVEIMAGESFROMRES, TRUE, 0L);
		AveSendMessage(butMed, AV_SET_RECT, (WPARAM)&skin.butMed, (LPARAM)&skin.front);
		AveSendMessage(butMed, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnSizeButtonClick);
		AveSendMessage(butMed, AV_UPDATE, 0L, 0L);
		AveUnlockControl(butMed);


		AveLockControl(butSmall);
		// bugfix: image wasn't updated for the button, so we reset the names with an invalid file
		AveSendMessage(butSmall,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butSmall,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"");
		AveSendMessage(butSmall,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)L"small.png");
		AveSendMessage(butSmall,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"small_sel.png");
		AveSendMessage(butSmall,AV_SET_PRESSEDIMAGEFILE, FALSE, (LPARAM)L"small_sel.png");
		AveSendMessage(butSmall, AV_SET_RETRIEVEIMAGESFROMRES, TRUE, 0L);
		AveSendMessage(butSmall, AV_SET_RECT, (WPARAM)&skin.butSmall, (LPARAM)&skin.front);
		AveSendMessage(butSmall, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnSizeButtonClick);
		AveSendMessage(butSmall, AV_UPDATE, 0L, 0L);
		AveUnlockControl(butSmall);

		UINT counter(0);
		for(std::vector<ColorEntry>::iterator iter = skin.colors.begin(); iter != skin.colors.end(); ++iter, ++counter)
		{
			ColorEntry& entry = *iter;

			std::stringstream ss;
			ss << "colorbutton" << WStringToString(entry.name); 
			AVEHANDLE but= AveCreateControl(container, ss.str().c_str(), AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
			AveLockControl(but);
			std::wstring butName(L"");
			butName.append(entry.name);
			butName.append(L".png");
			std::wstring selButName(entry.name);
			selButName.append(L"_sel.png");
			AveSendMessage(but,AV_SET_NORMALIMAGEFILE, FALSE, (LPARAM)butName.c_str());
			AveSendMessage(but,AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)selButName.c_str());
			AveSendMessage(but,AV_SET_PRESSEDIMAGEFILE, FALSE, (LPARAM)selButName.c_str());
			AveSendMessage(but, AV_SET_RETRIEVEIMAGESFROMRES, TRUE, 0L);
			AveSendMessage(but, AV_SET_RECT, (WPARAM)&entry.pos, (LPARAM)&skin.front);
			AveSendMessage(but, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnColorButtonClick);
			AveSendMessage(but, AV_UPDATE, 0L, 0L);
			AveUnlockControl(but);
			AveAddControlToContainer(container, but);
			DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, AveSendMessage(but, AV_GET_LAYERID, 0L, 0L), TRUE);

			colorButtons[but] = entry;

			if(brushColor.GetValue() == entry.color)
			{
				AveSendMessage(but, AV_IMAGEBUTTON_SET_ISPRESSED, TRUE, 0L);
				selectedColorButton = but;
			}
		}

		AveUnlockContainer(container);

		SetupDrawingLayer();

	}

	
	static BOOL __stdcall OnClearButtonClick(Widget* w, AVEHANDLE but, const POINT* pt, const SIZE* s, DWORD keys)
	{
		w->clearProgress = 0;
		SetTimer(w->hwnd, CLEAR_ANIM_TIMER_ID, CLEAR_ANIM_TIMER_INTERVAL, NULL);
		return TRUE;
	}

	static BOOL __stdcall OnColorButtonClick(Widget* w, AVEHANDLE but, const POINT* pt, const SIZE* s, DWORD keys)
	{
		if(w->selectedColorButton != NULL)
			AveSendMessage(w->selectedColorButton, AV_IMAGEBUTTON_SET_ISPRESSED, FALSE, 0L);

		AveSendMessage(but, AV_IMAGEBUTTON_SET_ISPRESSED, TRUE, 0L);
		w->selectedColorButton = but;
		w->brushColor.SetValue(w->colorButtons[but].color);
		return TRUE;
	}

	static BOOL __stdcall OnSizeButtonClick(Widget* w, AVEHANDLE but, const POINT* pt, const SIZE* s, DWORD keys)
	{
		if(but == w->butBig)
		{
			w->brushSize = SIZE_BIG;
			w->UpdateSizeButton();
		}
		else if(but == w->butMed)
		{
			w->brushSize = SIZE_MEDIUM;
			w->UpdateSizeButton();
		}
		else if(but == w->butSmall)
		{
			w->brushSize = SIZE_SMALL;
			w->UpdateSizeButton();
		}

		return TRUE;
	}

	Color GetColor()
	{
		return brushColor;
	}

	float GetBrushSize()
	{
		return static_cast<float>(brushSize) / 10.0f;
	}

	SIZE GetSize()
	{
		// this function receives the actual size based upon
		// the mode the widget is in.
		return skin.front;
	}

	void ClearAllLayers()
	{
		// this function deletes all layers from the desklet.

		while(DeskletLayerCount(hwnd) > 0)
			DeskletLayerRemove(hwnd, 0, FALSE);
	}

	std::string GetBordImageFileName()
	{
		return "AveChalkboard";
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
		// this function sets up the background layers.

		// first lock the desklet for invalidating.
		DeskletLockInvalidating(hwnd);

		UINT front  = DeskletLayerAdd(hwnd, "frontside");
		SetImageFileToLayer(front, L"frontside.png");

		UINT back  = DeskletLayerAdd(hwnd, "backside");
		SetImageFileToLayer(back, L"backside.png");
		DeskletLayerSetVisibility(hwnd, back, FALSE, FALSE);

		UINT flipLayer = DeskletLayerAdd(hwnd, "flip");
		SetImageFileToLayer(flipLayer, L"flip.png");
		DeskletLayerSetVisibility(hwnd, flipLayer, FALSE, FALSE);
		DeskletLayerSetPlacement(hwnd, flipLayer, FALSE, TRUE, &skin.flippie, skin.front.cx, skin.front.cy);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, flipLayer, TRUE);

		// unlock the desklet for invalidating.
		DeskletUnlockInvalidating(hwnd, FALSE);
	}

	void UpdateSizeButton()
	{
		BOOL smallButSel = brushSize == SIZE_SMALL;
		BOOL medButSel = brushSize == SIZE_MEDIUM;
		BOOL bigButSel = brushSize == SIZE_BIG;

		if(AveSendMessage(butSmall, AV_IMAGEBUTTON_GET_ISPRESSED, 0L, 0L) != smallButSel)
		{
			AveSendMessage(butSmall, AV_IMAGEBUTTON_SET_ISPRESSED, smallButSel, 0L);
			AveSendMessage(butSmall, AV_UPDATE, 0L, 0L);
		}

		if(AveSendMessage(butMed, AV_IMAGEBUTTON_GET_ISPRESSED, 0L, 0L) != medButSel)
		{
			AveSendMessage(butMed, AV_IMAGEBUTTON_SET_ISPRESSED, medButSel, 0L);
			AveSendMessage(butMed, AV_UPDATE, 0L, 0L);
		}

		if(AveSendMessage(butBig, AV_IMAGEBUTTON_GET_ISPRESSED, 0L, 0L) != bigButSel)
		{
			AveSendMessage(butBig, AV_IMAGEBUTTON_SET_ISPRESSED, bigButSel, 0L);
			AveSendMessage(butBig, AV_UPDATE, 0L, 0L);
		}
	}

	void SetupDrawingLayer()
	{

		// first lock the desklet for invalidating.
		DeskletLockInvalidating(hwnd);

		UINT draw = 0;
		BOOL success(FALSE);
		draw = DeskletLayerGetLayer(hwnd, "draw_overlay", &success);
		if(!success)
			draw   = DeskletLayerAdd(hwnd, "draw_overlay");

		delete g;
		g = NULL;
		delete bmp;
		bmp = NULL;
		
		Bitmap* oldBitmap = NULL;
		if(guid.length() > 0 && PathFileExistsW(GetSavePath(TRUE).c_str()))
			oldBitmap = new Bitmap(GetSavePath(FALSE).c_str());


		SIZE s = {skin.drawingArea.right - skin.drawingArea.left, skin.drawingArea.bottom - skin.drawingArea.top};
		bmp = new Bitmap(s.cx, s.cy);
		g = new Graphics(bmp);
		g->SetSmoothingMode(SmoothingModeAntiAlias);
		g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		if(oldBitmap != NULL)
			g->DrawImage(oldBitmap, Rect(0,0, oldBitmap->GetWidth(), oldBitmap->GetHeight()), 0,0, oldBitmap->GetWidth(), oldBitmap->GetHeight(), UnitPixel, 0,0,0);
		DeskletLayerSetPlacement(hwnd, draw, FALSE, TRUE, &skin.drawingArea, skin.front.cx, skin.front.cy);
		DeskletLayerSetImage(hwnd, draw, FALSE, bmp, FALSE);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, draw, TRUE);

		// unlock the desklet for invalidating.
		DeskletUnlockInvalidating(hwnd, FALSE);

		delete oldBitmap;
	}


	static BOOL __stdcall OnDoneButtonClick(Widget* w, AVEHANDLE but, const POINT* pt, const SIZE* s, DWORD keys)
	{
		w->Flip();
		return TRUE;
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

		doneBut = AveCreateControl(container, "back_done", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, doneBut);

		butBig = AveCreateControl(container, "but_big", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, butBig);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, AveSendMessage(butBig, AV_GET_LAYERID, 0L, 0L), TRUE);

		butMed = AveCreateControl(container, "but_med", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, butMed);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, AveSendMessage(butMed, AV_GET_LAYERID, 0L, 0L), TRUE);

		butSmall = AveCreateControl(container, "but_small", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, butSmall);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, AveSendMessage(butSmall, AV_GET_LAYERID, 0L, 0L), TRUE);

		butClear = AveCreateControl(container, "but_clear", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveAddControlToContainer(container, butClear);
		DeskletLayerSetNoAlphaBlendedHitTesting(hwnd, AveSendMessage(butClear, AV_GET_LAYERID, 0L, 0L), TRUE);
		
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
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "frontsize", NULL));
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "but_big", NULL));
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "but_med", NULL));
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "but_small", NULL));
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "draw_overlay", NULL));
		frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "but_clear", NULL));
		for(std::map<AVEHANDLE, ColorEntry>::iterator iter = colorButtons.begin(); iter != colorButtons.end(); ++iter)
		{
			std::wstringstream ss;
			ss << L"colorbutton" << iter->second.name; 
			frontLayers.push_back(DeskletLayerGetLayer(hwndDesklet, WStringToString(ss.str()).c_str(), NULL));
		}

		std::vector<UINT> backLayers;
		backLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "backside", NULL));
		backLayers.push_back(DeskletLayerGetLayer(hwndDesklet, "back_done", NULL));


		params.newDeskletSize = &skin.back;
		params.oldDeskletSize = &skin.front;
		params.oldLayersVisible = &frontLayers[0];
		params.newLayersVisible = &backLayers[0];
		params.oldLayersVisibleCount = frontLayers.size();
		params.newLayersVisibleCount = backLayers.size();
		if(isInBackMode)
		{
			params.newDeskletSize = &skin.front;
			params.oldDeskletSize = &skin.back;

			params.oldLayersVisible = &backLayers[0];
			params.newLayersVisible = &frontLayers[0];

			params.oldLayersVisibleCount = backLayers.size();
			params.newLayersVisibleCount = frontLayers.size();
		}

		flipVisible = FALSE;
		DeskletFlip(hwnd, &params);

		isInBackMode = !isInBackMode;
	}

	std::wstring GetSavePath(BOOL createDirIfNeeded)
	{
		WCHAR path[MAX_PATH] = {0};
		UINT bufLen=MAX_PATH;
		DeskletGetDirectoryW(AVEDESK_SETTING_THEMESPATH, path, &bufLen);
		PathAppendW(path, L"ChalkBoard");
		if(createDirIfNeeded && !PathFileExistsW(path))
			CreateDirectoryW(path, NULL);
		PathAppendW(path, (guid+L".png").c_str());

		return path;
	}
};



void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	// give the world some information about this widget.
	strcpy(szName,"ChalkBoard");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Make notes by drawing them, instead of typing.");
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
	}
	else
	{
		// we currently don't load nor save any properties, but
		// this would be the right place to do so.
	}
	DeskletRedraw(hwndDesklet);

	return w;
}

Widget* __stdcall OnCreateFromSkinW(HWND hwnd,HINSTANCE hInstance, CSkinEnumerator* se, SkinInfo* skinInfo)
{
	Widget* desklet = OnCreate(hwnd,hInstance,NULL,NULL);
	if(se != NULL)
		desklet->LoadSkin(se->getValue(skinInfo->wKey).c_str());
	else
		desklet->LoadSkin(skinInfo->wKey);

	desklet->ApplySkin();
	return desklet;
}


void __stdcall OnDestroy(Widget* t, HWND hwndDesklet)
{
	// when our widget is destroyed,
	// delete the widget object which does further cleaning.
	delete t;
}

BOOL __stdcall OnLayerRightClick(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseRightClick(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	const STARTID   = 3003;
	const CLIPBOARD = 1;
	const SAVE      = 2;

	HMENU menu = DeskletGetDefaultMenu(t->hwnd, STARTID);
	if(t->bmp != NULL)
	{
		AppendMenuW(menu,MF_SEPARATOR,0,L"-");
		AppendMenuW(menu,MF_STRING,CLIPBOARD,L"Copy To ClipBoard");
		AppendMenuW(menu,MF_STRING,SAVE,L"Save As...");
	}

	POINT p = {GET_X_LPARAM(GetMessagePos()), GET_Y_LPARAM(GetMessagePos())};
	int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, p.x, p.y, 0, t->hwnd,0);
	if(cmd >= STARTID)
	{
		DeskletDoDefaultMenuCommand(t->hwnd,STARTID,cmd);
	}
	else
	{
		if(SAVE == cmd)
		{
			WCHAR buffer[MAX_PATH+1] = {0};
			WCHAR fileTitle[MAX_PATH] = {0};
			OPENFILENAMEW ofn = {0};
			ofn.lStructSize = sizeof(ofn);
			ofn.hInstance = t->dll;
			ofn.hwndOwner = t->hwnd;
			ofn.lpstrFilter = L"PNG File(*.png)\0*.png\0All Files(*.*)\0*.*\0\0";
			ofn.lpstrFile  = buffer;
			ofn.nMaxFile = sizeof(buffer)/sizeof(buffer[0]);
			ofn.lpstrFileTitle = fileTitle;
			ofn.nMaxFileTitle = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			if(GetSaveFileNameW(&ofn))
			{
				if(t->bmp != NULL)
				{
					CLSID pngClsid = {0};
					GetEncoderClsid(L"image/png",&pngClsid);
					t->bmp->Save(buffer,&pngClsid,NULL);
				}
			}
		}
		else if(CLIPBOARD == cmd)
		{
			if(NULL == t->bmp)
				return TRUE;

			HDC screenDC = GetDC(NULL);
			HDC dc = CreateCompatibleDC(screenDC);
			HBITMAP hBitmap = CreateCompatibleBitmap(screenDC, t->bmp->GetWidth(), t->bmp->GetHeight());
			SelectObject(dc, (HGDIOBJ)hBitmap);
			Graphics* g = new Graphics(dc);
			g->Clear(Color(255,255,255,255));

			Bitmap* bg = DeskletLoadGDIPlusImageW(t->CreateSkinnedPath(L"frontside.png").c_str());
			g->DrawImage(bg, Rect(0,0, t->bmp->GetWidth(), t->bmp->GetHeight()), t->skin.drawingArea.left,t->skin.drawingArea.top,t->bmp->GetWidth(), t->bmp->GetHeight(), UnitPixel, 0,0,0);
			g->DrawImage(t->bmp, Rect(0,0, t->bmp->GetWidth(), t->bmp->GetHeight()), 0,0,t->bmp->GetWidth(), t->bmp->GetHeight(), UnitPixel, 0,0,0);
			delete bg;
			delete g;
			DeleteDC(dc);
			ReleaseDC(NULL, screenDC);

			if(OpenClipboard(t->hwnd))
			{
				EmptyClipboard() ;
				SetClipboardData(CF_BITMAP, (HANDLE)hBitmap);
				CloseClipboard();
			}
			else
			{
				DeleteObject((HGDIOBJ)hBitmap);
			}

		}
	}

	DestroyMenu(menu);


	return TRUE;
}


BOOL __stdcall OnLayerClick(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// when we receive a layer click, we forward this mouse click to the container,
	// which, in turns, will forward it to the right control. If the container or its controlled
	// handled the mouse clikc, TRUE has been returned. Otherwise, FALSE.
	if(AveForwardMouseClick(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	if(strcmp(layerName, "flip") == 0)
	{
		t->Flip();
	}


	// let's know we handle this.
	return TRUE;
}

BOOL __stdcall OnLayerMouseDown(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// we'll also forward layer mouse down notifications.
	BOOL bRet = AveForwardMouseDown(w->container, layerId, layerName, pt, s, keys);


	if(strcmp(layerName, "draw_overlay") == 0)
	{
		float brushSize = w->GetBrushSize();
		w->prevPoint = *pt;

		DeskletLayerSetCapture(w->hwnd, layerId);
		w->hasCapture = TRUE;

		SolidBrush black(w->GetColor());
		Rect pointRect(pt->x - brushSize/2.0f, pt->y - brushSize/2.0f, brushSize,brushSize);
		w->g->FillEllipse(&black, pointRect);
		RECT invalRect = {pointRect.X, pointRect.Y, pointRect.X + pointRect.Width, pointRect.Y + pointRect.Height};
		invalRect.left += w->skin.drawingArea.left;
		invalRect.right += w->skin.drawingArea.left;
		invalRect.top += w->skin.drawingArea.top;
		invalRect.bottom += w->skin.drawingArea.top;
		DeskletInvalidateRect(w->hwnd, &invalRect, &w->skin.front, TRUE);
		return TRUE;
	}

	return bRet;
}

BOOL __stdcall OnLayerMouseWheelForward(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	// forward mouse wheel notifications.
	if(AveForwardMouseWheelForward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelBackward(Widget* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(AveForwardMouseWheelBackward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseUp(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseUp(w->container, layerId, layerName, pt, s, keys))
		return TRUE;

	if(w->hasCapture && strcmp(layerName, "draw_overlay") == 0)
	{
		DeskletLayerReleaseCapture(w->hwnd);
		w->hasCapture = FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseOn(Widget* w, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	BOOL bRes = AveForwardMouseOn(w->container, layerId, layerName, pt, s);

	if(strcmp(layerName, "draw_overlay") == 0)
	{
		HCURSOR cross = LoadCursor(NULL, IDC_CROSS);
		w->oldCursor = DeskletSetCursor(w->hwnd, cross);
	}
	else if(strcmp(layerName, "flip") == 0)
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

	if(strcmp(layerName, "draw_overlay") == 0 && w->hasCapture)
	{
		float brushSize = w->GetBrushSize();
		Pen pen(w->GetColor(),brushSize);
		pen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
		Rect pointRect(pt->x - brushSize/2.0f, pt->y - brushSize/2.0f, brushSize, brushSize);
		w->g->DrawLine(&pen, w->prevPoint.x, w->prevPoint.y, pt->x, pt->y);


		GraphicsPath path;
		path.AddLine(w->prevPoint.x, w->prevPoint.y, pt->x, pt->y);
		Rect pathRect;
		path.GetBounds(&pathRect, NULL, &pen);
		RECT invalRect = {pathRect.X, pathRect.Y, pathRect.X + pathRect.Width, pathRect.Y + pathRect.Height};
		invalRect.left += w->skin.drawingArea.left;
		invalRect.right += w->skin.drawingArea.left;
		invalRect.top += w->skin.drawingArea.top;
		invalRect.bottom += w->skin.drawingArea.top;
		DeskletInvalidateRect(w->hwnd, &invalRect, &w->skin.front, TRUE);
		
		w->prevPoint = *pt;

		return FALSE;
	}

	return FALSE;
}


BOOL __stdcall OnLayerMouseOut(Widget* w, UINT layerId, const char* layerName)
{
	BOOL bRes = AveForwardMouseOut(w->container, layerId, layerName);

	if(strcmp(layerName, "draw_overlay") == 0)
	{
		DeskletSetCursor(w->hwnd, w->oldCursor);
	}
	else if(strcmp(layerName, "flip") == 0)
	{
		DeskletSetCursor(w->hwnd, w->oldCursor);
	}


	return bRes;
}

BOOL __stdcall OnBeforeRemoval(Widget* w)
{
	DeskletLayerSetImage(w->hwnd, DeskletLayerGetLayer(w->hwnd, "draw_overlay", NULL), FALSE, NULL, FALSE);
	delete w->g;
	w->g = NULL;
	delete w->bmp;
	w->bmp = NULL;

	std::wstring filePath(w->GetSavePath(FALSE));
	if(filePath.length() > 0)
		DeleteFileW(filePath .c_str());

	return TRUE;
}

void __stdcall OnSave(Widget* t, const char* ini, const char* group, BOOL bIsForExport)
{
	std::wstring filePath(t->GetSavePath(FALSE));
	if(filePath.length() > 0)
		DeleteFileW(filePath .c_str());


	t->guid = CreateGUIDString();
	t->guid = t->guid.substr(1, t->guid.length()-2);

	WritePrivateProfileStringW(StringToWString(group).c_str(), L"GUID", t->guid.c_str(), StringToWString(ini).c_str());
	WritePrivateProfileInt(group, "BrushSize", t->brushSize, ini);
	WritePrivateProfileInt(group, "BrushColor", t->brushColor.GetValue(), ini);
	WritePrivateProfileStringW(StringToWString(group).c_str(), L"SkinDir", t->skin.dir.c_str(), StringToWString(ini).c_str());


	if(t->bmp != NULL)
	{
		CLSID pngClsid = {0};
		GetEncoderClsid(L"image/png",&pngClsid);
		t->bmp->Save(t->GetSavePath(TRUE).c_str(),&pngClsid,NULL);
	}

}

void __stdcall OnProcessMessage(Widget* t, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!t)
		return;

	if(WM_TIMER == msg && CLEAR_ANIM_TIMER_ID == wParam)
	{
		UINT height = t->skin.drawingArea.bottom - t->skin.drawingArea.top;

		t->clearProgress = min(height, CLEAR_ANIM_DELTA + t->clearProgress);

		RECT invalRect = t->skin.drawingArea;
		invalRect.top = invalRect.top + t->clearProgress - CLEAR_ANIM_DELTA;
		invalRect.bottom = invalRect.top + t->clearProgress;

		CompositingMode oldMode = t->g->GetCompositingMode();
		t->g->SetCompositingMode(CompositingModeSourceCopy);

		SolidBrush clearBrush(Color(0,0,0,0));
		t->g->FillRectangle(&clearBrush, 0, t->clearProgress - CLEAR_ANIM_DELTA, t->skin.drawingArea.right - t->skin.drawingArea.left, CLEAR_ANIM_DELTA);
		t->g->SetCompositingMode(oldMode);
		DeskletInvalidateRect(t->hwnd, &invalRect, &t->skin.front, TRUE);

		if(t->clearProgress >= height)
			KillTimer(t->hwnd, CLEAR_ANIM_TIMER_ID);
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
		// we also forward windows messages to our container.
		// this way, the inputbox can respond to keys being pressed, for example.
		if(AveForwardMessage(t->container, msg, wParam, lParam))
			return;
	}
}

BOOL __stdcall OnMouseMove(Widget* t, int x, int y)
{
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

BOOL __stdcall OnMouseOut(Widget* t)
{
	if(t->isFlipVisible)
	{
		t->isFlipVisible = FALSE;
		t->flipIsFadingIn = FALSE;
		SetTimer(t->hwnd, FLIP_FADE_TIMER_ID, FLIP_TIMER_INTERVAL, NULL);
	}

	return TRUE;
}
int __stdcall SkinConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	Widget* w = (Widget*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			w = (Widget*)GetProp(hDlg,"lpData");
			if(!w)return TRUE;
			
			{
				char path[MAX_PATH] = {0};
				GetDLLPath(w->dll,path,MAX_PATH);
				PathAppend(path,"skins\\");
				std::string s = path;
				s += "*.*";
				WIN32_FIND_DATA data = {0};
				HANDLE h = FindFirstFile(s.c_str(),&data);
				if(h != INVALID_HANDLE_VALUE)
				{
					do
					{
						if(strcmp(data.cFileName,".")!=0 && strcmp(data.cFileName,"..")!=0 && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
							SendDlgItemMessage(hDlg,IDC_SKIN,CB_ADDSTRING,0,(LPARAM)data.cFileName);

					}while(FindNextFile(h,&data));
					FindClose(h);
				}
				SendDlgItemMessageW(hDlg,IDC_SKIN,CB_SELECTSTRING,0,(LPARAM)w->skin.dir.c_str());

				SetDlgItemTextW(hDlg,IDC_NAME,w->skin.name.c_str());
				SetDlgItemTextW(hDlg,IDC_AUTHOR,w->skin.author.c_str());
				SetDlgItemTextW(hDlg,IDC_INFO,w->skin.comment.c_str());
			}

			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_SKIN:
					if(HIWORD(wParam)==CBN_SELCHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
					if(w != NULL)
					{
						char path[MAX_PATH] = {0};
						GetDlgItemText(hDlg,IDC_SKIN,path,MAX_PATH);
						w->LoadSkin(StringToWString(path).c_str());
						w->ApplySkin();
						DeskletRedraw(w->hwnd);

						SetDlgItemTextW(hDlg,IDC_NAME,w->skin.name.c_str());
						SetDlgItemTextW(hDlg,IDC_AUTHOR,w->skin.author.c_str());
						SetDlgItemTextW(hDlg,IDC_INFO,w->skin.comment.c_str());
					}
				break;
			}
		break;
	}
	return FALSE;
}

BOOL __stdcall OnDragDropDataRequest(Widget* w,FORMATETC* format,STGMEDIUM* medium)
{
	// if a CF_HDROP is requested, we handle the request by setting up 
	// a HDROP structure in global memory and assigning it the the SRGMEDIUM.
	if(format->cfFormat == CF_HDROP)
	{
		char* m_szFiles = new char[(MAX_PATH+1) * 1 ];
		m_szFiles[0] = 0;

		int m_nLen = 0;

		char* p = m_szFiles;

		std::wstring file = w->GetSavePath(TRUE);
		if(w->bmp != NULL)
		{
			CLSID pngClsid = {0};
			GetEncoderClsid(L"image/png",&pngClsid);
			w->bmp->Save(file.c_str(),&pngClsid,NULL);
		}

		std::string s = WStringToString(file);
		strcpy(p,s.c_str());
		p += s.size()+1;
		m_nLen += s.size() + 1;

		HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, 
         sizeof(DROPFILES) + (m_nLen + 2) * sizeof(TCHAR)); 
		LPDROPFILES pDropFiles = (LPDROPFILES) ::GlobalLock(hglbCopy);
		pDropFiles->pFiles = sizeof(DROPFILES);

		GetCursorPos(&pDropFiles->pt);
		pDropFiles->fNC = TRUE;
		pDropFiles->fWide = FALSE; // ANSI charset
		LPTSTR lptstrCopy = (LPTSTR) pDropFiles;
		lptstrCopy += pDropFiles->pFiles;
		memcpy(lptstrCopy, m_szFiles, m_nLen * sizeof(TCHAR)); 

		lptstrCopy[m_nLen] =  '\0';    // null character 
		lptstrCopy[m_nLen+1] = '\0';    // null character 
	    ::GlobalUnlock(hglbCopy); 

		medium->hGlobal = hglbCopy;
	    medium->pUnkForRelease = NULL;
		medium->tymed = TYMED_HGLOBAL;
		
		delete[] m_szFiles;

		return TRUE;
	}
	return FALSE;
}

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
}