#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include "../DeskletSDK.h"
#include "../DeskletSDK.cpp"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "F:\iTunesCOM\ITunesCOMInterface_i.c"
#include "F:\iTunesCOM\iTunesCOMInterface.h"

#ifndef __ATLBASE_H__
#include <ATLBASE.h>
#endif

#ifndef __ATLCONV_H__
#include <ATLCONV.H>
#endif

#include "resource.h"

#include <vector>
#include <string>
#include <sstream>

using namespace Gdiplus;

const TIMERID		= 285772;
const SCROLLERID	= 303292;


std::wstring FormatTime(long time)
{
	std::wstringstream wss;

	long hours(time / 3600);
	if(hours  > 0 )
		wss << hours << L":";
	time -= hours*3600;

	long minutes(time / 60);
	if( minutes < 10)
		wss << L"0";
	
	time -= minutes * 60;


	wss << minutes << L":";

	long seconds(time % 60);
	if(seconds < 10)
		wss << L"0";

	wss << seconds;

	return wss.str();
}



Rect GetPrivateProfileRect(const char* section, const char* ini)
{
	return Rect(	GetPrivateProfileInt(section,"Left",0,ini),
					GetPrivateProfileInt(section,"Top",0,ini),
					GetPrivateProfileInt(section,"Width",0,ini),
					GetPrivateProfileInt(section,"Height",0,ini)
				);
}

class Skin
{
private:
	char name[MAX_PATH], author[MAX_PATH], infoText[MAX_PATH];
	WCHAR font[MAX_PATH];
	Color textColor;
	REAL fontSize;

	WCHAR font2[MAX_PATH];
	Color textColor2;
	REAL fontSize2;

	Rect display, progress, info, counter, prev, next, pause, stop, ipod, mute, open, quit,
		toggleiTunesWnd;
	Color innerKnob, outerKnob;

	UINT width, height, knobWidth, knobHeight;
	BOOL useKnobBmp, stretchKnob;

	Rect albumArt;
	BOOL showAlbumArt;

	BOOL drawOverlay, drawOverlayActualSize;
	UINT albumArtAlpha;

public:

	Skin(const char* ini)
	{
		GetPrivateProfileString("Info","Name","Unknown",name,MAX_PATH,ini);
		GetPrivateProfileString("Info","Author","Unknown",author,MAX_PATH,ini);
		GetPrivateProfileString("Info","Info","",infoText,MAX_PATH,ini);

		char tmpFont[MAX_PATH]= {0};
		GetPrivateProfileString("Font","Facename","Arial",tmpFont,MAX_PATH,ini);
		MultiByteToWideChar(CP_OEMCP,0,tmpFont,-1,font,MAX_PATH);
		textColor = Color(	GetPrivateProfileInt("Font","ColorA",255,ini),
							GetPrivateProfileInt("Font","ColorR",0,ini),
							GetPrivateProfileInt("Font","ColorG",0,ini),
							GetPrivateProfileInt("Font","ColorB",0,ini)
						);

		fontSize = (REAL) GetPrivateProfileInt("Font","Size",12,ini);

		char tmpFont2[MAX_PATH]= {0};
		GetPrivateProfileString("CounterFont","Facename",tmpFont,tmpFont2,MAX_PATH,ini);
		MultiByteToWideChar(CP_OEMCP,0,tmpFont2,-1,font2,MAX_PATH);
		textColor2 = Color(	GetPrivateProfileInt("CounterFont","ColorA",GetPrivateProfileInt("Font","ColorA",255,ini),ini),
							GetPrivateProfileInt("CounterFont","ColorR",GetPrivateProfileInt("Font","ColorR",255,ini),ini),
							GetPrivateProfileInt("CounterFont","ColorG",GetPrivateProfileInt("Font","ColorG",255,ini),ini),
							GetPrivateProfileInt("CounterFont","ColorB",GetPrivateProfileInt("Font","ColorB",255,ini),ini)
						);

		fontSize2 = (REAL) GetPrivateProfileInt("CounterFont","Size",fontSize,ini);

		display	= GetPrivateProfileRect("DisplayPane",ini);
		progress= GetPrivateProfileRect("ProgressBar",ini);
		info	= GetPrivateProfileRect("InfoPane",ini);
		counter = GetPrivateProfileRect("CounterPane",ini);
		prev	= GetPrivateProfileRect("PreviousButton",ini);
		next	= GetPrivateProfileRect("NextButton",ini);
		pause	= GetPrivateProfileRect("PausePlayButton",ini);
		stop	= GetPrivateProfileRect("StopButton",ini);
		ipod	= GetPrivateProfileRect("UpdateIPodButton",ini);
		mute	= GetPrivateProfileRect("MuteButton",ini);
		open	= GetPrivateProfileRect("OpenButton",ini);
		quit	= GetPrivateProfileRect("QuitButton",ini);
		toggleiTunesWnd = GetPrivateProfileRect("ToggleITunesWindowButton",ini);

		albumArt	= GetPrivateProfileRect("AlbumArt",ini);
		showAlbumArt= GetPrivateProfileInt("AlbumArt","Show",FALSE,ini);
		drawOverlay = GetPrivateProfileInt("AlbumArt","DrawOverlay",FALSE,ini);
		drawOverlayActualSize = GetPrivateProfileInt("AlbumArt","DrawOverlayActualSize",TRUE,ini);
		albumArtAlpha = GetPrivateProfileInt("AlbumArt","Alpha",255,ini);


		innerKnob = Color(	GetPrivateProfileInt("InnerKnob","ColorA",255,ini),
							GetPrivateProfileInt("InnerKnob","ColorR",0,ini),
							GetPrivateProfileInt("InnerKnob","ColorG",0,ini),
							GetPrivateProfileInt("InnerKnob","ColorB",0,ini)
						);
		outerKnob = Color(	GetPrivateProfileInt("OuterKnob","ColorA",255,ini),
							GetPrivateProfileInt("OuterKnob","ColorR",0,ini),
							GetPrivateProfileInt("OuterKnob","ColorG",0,ini),
							GetPrivateProfileInt("OuterKnob","ColorB",0,ini)
						);

		width = GetPrivateProfileInt("Bg","Width",266,ini);
		height= GetPrivateProfileInt("Bg","Height",108,ini);

		knobWidth = GetPrivateProfileInt("Knob","Width",6,ini);
		knobHeight= GetPrivateProfileInt("Knob","Height",6,ini);

		useKnobBmp = GetPrivateProfileInt("Knob","UseBitmap",FALSE,ini);
		stretchKnob = GetPrivateProfileInt("Knob","StretchKnob",FALSE,ini);

	}

	WCHAR*		GetFontFace() { return font;};
	Color&	    GetTextColor() { return textColor;};
	REAL	    GetFontSize() { return fontSize;};

	WCHAR*		GetCounterFontFace() { return font2;};
	Color&	    GetCounterTextColor() { return textColor2;};
	REAL	    GetCounterFontSize() { return fontSize2;};

	Rect&		GetStopRect() { return stop;};
	Rect&		GetIpodRect() { return ipod;};
	Rect&		GetMuteRect() { return mute;};
	Rect&		GetOpenRect() { return open;};
	Rect&		GetQuitRect() { return quit;};

	Rect&		GetDisplayRect() { return display;};
	Rect&		GetProgressRect() { return progress;};
	Rect&		GetInfoRect() { return info;};
	Rect&		GetCounterRect() { return counter;};
	Rect&		GetPrevRect() { return prev;};
	Rect&		GetNextRect() { return next;};
	Rect&		GetPauseRect() { return pause;};
	Rect&		GetToggleITunesWindowRect() { return toggleiTunesWnd;};

	Color&		GetKnobInnerColor() { return innerKnob;};
	Color&		GetKnobOuterColor() { return outerKnob; };

	UINT		GetWidth() { return width;};
	UINT		GetHeight() { return height;};

	UINT		GetKnobWidth() { return knobWidth;};
	UINT		GetKnobHeight() { return knobHeight;};

	BOOL		GetUseKnobBitmap()	{ return useKnobBmp;};
	BOOL		GetStretchKnob()	{ return stretchKnob;};


	Rect&		GetAlbumArtRect() { return albumArt;};
	BOOL		GetShowAlbumArt() { return showAlbumArt;};

	BOOL		GetDrawOverlay() { return drawOverlay;};
	BOOL		GetDrawOverlayActualSize() { return drawOverlayActualSize;};
	UINT		GetAlbumArtAlpha() { return albumArtAlpha;};

};

class iTunesWidget // :  public CComObjectRoot, public _IiTunesEvents
{
private:
/*	BEGIN_COM_MAP(iTunesWidget)
    COM_INTERFACE_ENTRY(_IiTunesEvents)
	END_COM_MAP( )*/

	HWND hwndDesklet;
	HINSTANCE dll;
	CComPtr<IiTunes> app;
	CComPtr<IITOperationStatus> status;

	bool wasPaused;

	BOOL showTimePlayed;
	int counter;

	BOOL hideWhenNotLoaded;
	BOOL autoHideiTunes;

	std::wstring scrollText;
	bool isScrolling;
	int w,l;
	Bitmap* scrollBmp, *knobBmp;

	bool doScroll;
	bool scrollBack;
	int scrollCounter;

	Skin* skin;

	char skinDir[MAX_PATH];

	CComBSTR songName;


public:
	iTunesWidget(HWND hwndDesklet, HINSTANCE dll) :
		hwndDesklet( hwndDesklet), dll (dll), app(0), wasPaused(true), showTimePlayed(TRUE), counter(0),
		hideWhenNotLoaded(FALSE), autoHideiTunes(FALSE), isScrolling(false), w(0), l(0), scrollBmp(0),
		doScroll(false), scrollBack(false), scrollCounter(0), skin(0), knobBmp(0)
	{
		strcpy(skinDir,"default");

		CoInitialize(0);
		if(FindWindow("iTunes",0) )
			app.CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
	}

	~iTunesWidget()
	{
		delete knobBmp;
		knobBmp = 0;
		delete scrollBmp;
		scrollBmp = 0;
		CoUninitialize();
		if(GetAutoHideiTunes())
		{
			HWND iTunesWnd = FindWindow("iTunes",0);
			if(iTunesWnd)
			{
				ShowWindow(iTunesWnd,SW_SHOWNA);
			}
		}
		delete skin;
		skin = 0;
	};

	void ResetSongName()
	{
		songName.Empty();
	}

	void SetSkin(const char* dir)
	{
		strcpy(skinDir,dir);
		char bgPath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(GetWindow(),bgPath);
		PathAppend(bgPath,"/skins/");
		PathAppend(bgPath,skinDir);
		PathAppend(bgPath,"/images/bg.png");
		DeskletSetImageFile(GetWindow(),bgPath);

		char ini[MAX_PATH] = {0};
		GetDLLPath(GetDLL(),ini,MAX_PATH);
		PathAppend(ini,"skins\\");
		PathAppend(ini,skinDir);
		PathAppend(ini,"\\skin.ini");
		skin = new Skin(ini);

		ResetSongName();
		
		if(skin->GetShowAlbumArt() )
		{
			BOOL success(FALSE);
			UINT lid = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",&success);
			if(!success)
			{
				lid = DeskletLayerAdd(GetWindow(),"iTunesWidget.AlbumArt");
				if(lid>1)
				{
					DeskletLayerMoveBottom(GetWindow(),lid,FALSE);
					lid = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",NULL);
					DeskletLayerMoveUp(GetWindow(),lid,FALSE);
					lid = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",NULL);
				}
			}
				Rect& sr3 = skin->GetAlbumArtRect();
				RECT r = {0};
				r.left = sr3.X;
				r.top = sr3.Y;
				r.right = sr3.X + sr3.Width;
				r.bottom = sr3.Y + sr3.Height;
				DeskletLayerSetPlacement(GetWindow(),lid,FALSE,TRUE,&r,skin->GetWidth(),skin->GetHeight());
		}
		else
		{
			BOOL success(FALSE);
			UINT lid = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",&success);
			if(success)
				DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
		}

		BOOL success = FALSE;
		UINT id = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Pause_Button",&success);
		if(!success)
		{
			id = DeskletLayerAdd(GetWindow(),"iTunesWidget.Pause_Button");
		}
			Rect& sr = skin->GetPauseRect();
			RECT r = {0};
			r.left = sr.X;
			r.top = sr.Y;
			r.right = sr.X + sr.Width;
			r.bottom = sr.Y + sr.Height;
			DeskletLayerSetPlacement(GetWindow(),id,FALSE,TRUE,&r,skin->GetWidth(),skin->GetHeight());
				
			char pausePath[MAX_PATH] = {0};
			DeskletGetRelativeFolder(GetWindow(),pausePath);
			PathAppend(pausePath,"skins\\");
			PathAppend(pausePath,skinDir);
			PathAppend(pausePath,"\\images\\pause.png");
			DeskletLayerSetImageFile(GetWindow(),id,FALSE,pausePath);


		success = FALSE;
		id = 0;
		id = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Info",&success);
		if(!success)
		{
			id = DeskletLayerAdd(GetWindow(),"iTunesWidget.Info");
		}
			Rect& sr2 = skin->GetDisplayRect();
			r.left = sr2.X;
			r.top = sr2.Y;
			r.right = sr2.X + sr2.Width;
			r.bottom = sr2.Y + sr2.Height;
			DeskletLayerSetPlacement(GetWindow(),id,FALSE,TRUE,&r,skin->GetWidth(),skin->GetHeight());

		delete knobBmp;
		knobBmp = 0;
		if(skin->GetUseKnobBitmap() )
		{
			char knobPath[MAX_PATH] = {0};
			DeskletGetRelativeFolder(GetWindow(),knobPath);
			PathAppend(knobPath,"/skins/");
			PathAppend(knobPath,skinDir);
			PathAppend(knobPath,"/images/knob.png");
			knobBmp = DeskletLoadGDIPlusImage(knobPath);
		}

		SIZE s= {0};
		s.cx = skin->GetWidth();
		s.cy = skin->GetHeight();
		DeskletSetSize(GetWindow(),&s);
	}

	HWND GetWindow()const { return hwndDesklet;};
	HINSTANCE GetDLL()const { return dll;};

	void ReadFromIni(const char* ini, const char* section);
	void SaveToIni(const char* ini, const char* section);

	CComPtr<IiTunes>& GetApp(){ return app;};
	CComPtr<IITOperationStatus>& GetStatus() { return status;};

	void Redraw();

	void SetHideWhenNotLoaded(BOOL val){hideWhenNotLoaded = val;}
	BOOL GetHideWhenNotLoaded()const { return hideWhenNotLoaded;};

	bool GetScrollBackwards()const { return scrollBack;};
	void SetScrollBackwards(bool v){ scrollBack = v;};

	int  GetScrollCounter()const{ return scrollCounter;};
	void SetScrollCounter(int i){scrollCounter = i ;};

	void BeginScrolling(std::wstring text)
	{
		scrollBack=false;
		isScrolling = true; 
		scrollText = text;
		l = 0;
		
		Gdiplus::FontFamily  fontFamily(skin->GetFontFace());
		SolidBrush  black(skin->GetTextColor());

		GraphicsPath path(Gdiplus::FillModeAlternate);
		StringFormat format(0, LANG_NEUTRAL);
		format.SetLineAlignment(StringAlignmentNear);
		format.SetAlignment(StringAlignmentNear);


		RectF rc(0,0,4000,skin->GetInfoRect().Height);

		path.AddString(scrollText.c_str(), -1, &fontFamily, 0, skin->GetFontSize(), rc, &format);

		Rect rcBounds;
		path.GetBounds(&rcBounds);

		delete scrollBmp;
		scrollBmp = 0;
		scrollBmp = new Bitmap(rcBounds.Width+10,skin->GetInfoRect().Height+10);
		w = scrollBmp->GetWidth();

		Graphics g(scrollBmp);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		g.FillPath(&black,&path);

		SetTimer(GetWindow(),SCROLLERID,100,0);
	};


	std::wstring& GetScrollingText(){ return scrollText; };
	int GetScrollingWidth()const{return w;};

	bool NeedScrolling(std::wstring text)
	{
		Gdiplus::FontFamily  fontFamily(skin->GetFontFace());
		SolidBrush  black(skin->GetTextColor());

		GraphicsPath path(Gdiplus::FillModeAlternate);
		StringFormat format(0, LANG_NEUTRAL);
		//format.SetLineAlignment(StringAlignmentCenter);
		format.SetAlignment(StringAlignmentNear);


		RectF rc(0,0,4000,skin->GetInfoRect().Height);

		path.AddString(scrollText.c_str(), -1, &fontFamily, 0, skin->GetFontSize(), rc, &format);

		Rect rcBounds;
		path.GetBounds(&rcBounds);
		return ( rcBounds.Width > skin->GetInfoRect().Width);
	}

	void SetDoScroll(bool val){doScroll = val;};
	bool GetDoScroll()const { return doScroll;};

	void EndScrolling()
	{
		KillTimer(GetWindow(),SCROLLERID);
		delete scrollBmp;
		scrollBmp = 0;
		isScrolling = false;
	};

	bool IsScrolling()const
	{
		return isScrolling;
	};

	void SetAutoHideiTunes(BOOL val)
	{
		autoHideiTunes = val;
		if(autoHideiTunes)
		{
			HWND iTunesWnd = FindWindow("iTunes",0);
			if(iTunesWnd)
			{
				if(GetApp())
					ShowWindow(iTunesWnd,SW_HIDE);
			}	
		}
		else
		{
			HWND iTunesWnd = FindWindow("iTunes",0);
			if(iTunesWnd)
			{
				if(GetApp())
					ShowWindow(iTunesWnd,SW_SHOWNA);
			}	
		}
	};
	BOOL GetAutoHideiTunes()const { return autoHideiTunes;};

	BOOL CheckLoadedStatus()
	{
		if(GetApp() )
		{
			BSTR version = {0};
			if(GetApp())
			{
				if(FAILED(GetApp()->get_Version(&version)) )
				{
					GetApp().Release();
				}
			}
			else
			{
				SysFreeString(version);
			}
			return GetApp() ? TRUE : FALSE;
		}
		else
		{
			if(FindWindow("iTunes",0) )
			{
				SetAutoHideiTunes(GetAutoHideiTunes());
				app.CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
				return GetApp() ? TRUE : FALSE;
			}
			else
			{
				return FALSE;
			}
		}
	}

	void SetScrollingLeft(int left) { l = left;};
	int  GetScrollingLeft()const { return l;};

	void ToggleShowTimePlayed(){showTimePlayed = !showTimePlayed;};

	int Count()
	{
		counter++;
		if(counter > 18)
			counter = 0;

		return counter;
	}

	Skin* GetSkin()const {return skin;};

	const char* GetSkinDir()const { return skinDir;};
	
#define STDMETHOD __stdcall

	HRESULT  virtual STDMETHOD OnDatabaseChangedEvent (/*[in]*/ VARIANT deletedObjectIDs,/*[in]*/ VARIANT changedObjectIDs) 
	{
		return S_OK;
	}

 
	HRESULT  virtual STDMETHOD OnPlayerPlayEvent (/*[in]*/ VARIANT iTrack) 
	{
		return S_OK;
	}

 
	HRESULT  virtual STDMETHOD OnPlayerStopEvent (/*[in]*/ VARIANT iTrack) 
	{
		return S_OK;
	}

 
	HRESULT  virtual STDMETHOD OnPlayerPlayingTrackChangedEvent (/*[in]*/ VARIANT iTrack) 
	{
		return S_OK;

	}
 
	HRESULT  OnCOMCallsDisabledEvent (/*[in]*/ ITCOMDisabledReason reason) 
	{
		return S_OK;
	}
 
	HRESULT  virtual STDMETHOD OnCOMCallsEnabledEvent () 
	{
		return S_OK;
	}

 
	HRESULT  virtual STDMETHOD OnQuittingEvent () 
	{
		return S_OK;
	}
};


void iTunesWidget::ReadFromIni(const char* ini, const char* section)
{
	showTimePlayed = GetPrivateProfileInt(section,"ShowTimePlayed",showTimePlayed,ini);
	SetHideWhenNotLoaded( GetPrivateProfileInt(section,"HideWhenNotLoaded",GetHideWhenNotLoaded(),ini) );
	SetAutoHideiTunes( GetPrivateProfileInt(section,"AutoHideiTunes",GetAutoHideiTunes(),ini) );
	GetPrivateProfileString(section,"SkinDir",skinDir,skinDir,MAX_PATH,ini);
	SetSkin(skinDir);
}

void iTunesWidget::SaveToIni(const char* ini, const char* section)
{
	WritePrivateProfileInt(section,"ShowTimePlayed",showTimePlayed,ini);
	WritePrivateProfileInt(section,"HideWhenNotLoaded",GetHideWhenNotLoaded(),ini);
	WritePrivateProfileInt(section,"AutoHideiTunes",GetAutoHideiTunes(),ini);
	WritePrivateProfileString(section,"SkinDir",skinDir,ini);
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"iTunesWidget");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,":)");
	*iVersion = 110;
}

void iTunesWidget::Redraw()
{
		BOOL isLoaded =CheckLoadedStatus(); 
		if(!isLoaded)
		{
			if ( GetHideWhenNotLoaded() )
			{
				DeskletHide(GetWindow() );
			}
		}
		else
		{
			DeskletShow(GetWindow() );
		}
		
		ITPlayerState  st;
		if(GetApp())
		{
			GetApp()->get_PlayerState(&st);
		}
		bool isPaused = st != ITPlayerStatePlaying;

		if(wasPaused != isPaused)
		{
			BOOL success(FALSE);
			UINT id = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Pause_Button",&success);
			if(id == 0)
			{
				id = DeskletLayerAdd(GetWindow(),"iTunesWidget.Pause_Button");
				Rect& sr = skin->GetPauseRect();
				RECT r = {0};
				r.left = sr.X;
				r.top = sr.Y;
				r.right = sr.X + sr.Width;
				r.bottom = sr.Y + sr.Height;
				DeskletLayerSetPlacement(GetWindow(),id,FALSE,TRUE,&r,skin->GetWidth(),skin->GetHeight());
				
				char pausePath[MAX_PATH] = {0};
				DeskletGetRelativeFolder(GetWindow(),pausePath);
				PathAppend(pausePath,"skins\\");
				PathAppend(pausePath,skinDir);
				PathAppend(pausePath,"\\images\\pause.png");
				DeskletLayerSetImageFile(GetWindow(),id,FALSE,pausePath);
			}
			DeskletLayerSetVisibility(GetWindow(),id,FALSE,isPaused);
		}

		if(isLoaded && (!wasPaused || IsScrolling()))
		{
			if(skin->GetShowAlbumArt() )
			{
				CComBSTR trackName;
				CComPtr<IITTrack> track;
				GetApp()->get_CurrentTrack(&track);
				if(track)
				{
					track->get_Name(&trackName);
				}

				BOOL success(FALSE);
				UINT lid = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",&success);
				if(!success)
				{
					lid = DeskletLayerAdd(GetWindow(),"iTunesWidget.AlbumArt");
					Rect& sr = skin->GetAlbumArtRect();
					RECT r = {0};
					r.left = sr.X;
					r.top = sr.Y;
					r.right = sr.X + sr.Width;
					r.bottom = sr.Y + sr.Height;
					DeskletLayerSetPlacement(GetWindow(),lid,FALSE,TRUE,&r,skin->GetWidth(),skin->GetHeight());
				}
	
				if(!trackName)
				{
					DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);	
				}
				else
				{
					if(!(trackName == songName) )
					{
						CComPtr<IITArtworkCollection>artworks;
						track->get_Artwork(&artworks);
						if(artworks)
						{
							long count(0);
							artworks->get_Count(&count);
							if(count > 0)
							{
								CComPtr<IITArtwork> artwork;
								artworks->get_Item(1,&artwork);
								if(artwork)
								{
									char path[MAX_PATH] = {0};
									GetDLLPath(GetDLL(),path,MAX_PATH);
									PathAppend(path,"temp");

									WCHAR wPath[MAX_PATH] = {0};
									MultiByteToWideChar(CP_OEMCP,0,path,-1,wPath,MAX_PATH);

									CComBSTR bstrPath(wPath);
									if(SUCCEEDED(artwork->SaveArtworkToFile(bstrPath)))
									{
										Rect& sr = skin->GetAlbumArtRect();
										Bitmap* bmp = new Bitmap(sr.Width,sr.Height);

										Graphics g(bmp);
										g.SetSmoothingMode(SmoothingModeAntiAlias);
										g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

										Bitmap* art = DeskletLoadGDIPlusImage(path);
										
										ColorMatrix cm = {0};
										cm.m[0][0] = 1.0f;
										cm.m[1][1] = 1.0f;
										cm.m[2][2] = 1.0f;
										cm.m[3][3] = 1.0f;
										cm.m[4][4] = 1.0f;

										cm.m[3][3] =  ((float)skin->GetAlbumArtAlpha()/255.0f);
										ImageAttributes ia;
										ia.SetColorMatrix(&cm);

										if(art)
											g.DrawImage(art,Rect(0,0,sr.Width,sr.Height),0,0,art->GetWidth(),art->GetHeight(),UnitPixel,&ia,0,0);

										//g.DrawImage(art,0,0,sr.Width,sr.Height,UnitPixel);

										if(skin->GetDrawOverlay() )
										{
											char ovPath[MAX_PATH] = {0};
											DeskletGetRelativeFolder(GetWindow(),ovPath);
											PathAppend(ovPath,"/skins/");
											PathAppend(ovPath,skinDir);
											PathAppend(ovPath,"/images/overlay.png");
											Bitmap* overlay = DeskletLoadGDIPlusImage(ovPath);

											if(skin->GetDrawOverlayActualSize() && overlay)
												g.DrawImage(overlay,0,0,overlay->GetWidth(),overlay->GetHeight() );
											else
												g.DrawImage(overlay,0,0,sr.Width,sr.Height);

											delete overlay;
										}
										

										delete art;
										art = 0;
										
										DeskletLayerSetImage(GetWindow(),lid,FALSE,bmp,TRUE);
										DeskletLayerSetVisibility(GetWindow(),lid,FALSE,TRUE);

									}
									else
									{
										DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
									}
								}
								else
								{
									DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
								}
							}
							else
							{
								DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
							}
						}
						else
						{
							DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
						}

					}
					else
					{
					//	DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
					}
				}

				songName = trackName;
			}
			



			Bitmap* bmp = new Bitmap(skin->GetDisplayRect().Width,skin->GetDisplayRect().Height);
			Graphics g(bmp);
			BOOL success=FALSE;
			UINT id = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Info",&success);
			if(id == 0)
			{
				id = DeskletLayerAdd(GetWindow(),"iTunesWidget.Info");
				Rect& sr = skin->GetDisplayRect();
				RECT r = {0};
				r.left = sr.X;
				r.top = sr.Y;
				r.right = sr.X + sr.Width;
				r.bottom = sr.Y + sr.Height;
				DeskletLayerSetPlacement(GetWindow(),id,FALSE,TRUE,&r,skin->GetWidth(),skin->GetHeight());
			}
			if(!GetApp())
			{
				g.SetSmoothingMode(SmoothingModeAntiAlias);
				g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
				Gdiplus::FontFamily  fontFamily(skin->GetFontFace());
				SolidBrush  black(skin->GetTextColor());

				GraphicsPath path(Gdiplus::FillModeAlternate), path2(Gdiplus::FillModeAlternate);
				StringFormat format(0, LANG_NEUTRAL);
				//format.SetLineAlignment(StringAlignmentCenter);
				format.SetAlignment(StringAlignmentCenter);


				//RectF rc(8,18,120,20);

				path.AddString(L"iTunes not loaded", -1, &fontFamily, 0, skin->GetFontSize(), skin->GetInfoRect(), &format);
				g.FillPath(&black, &path);
			}
			else
			{
				long curPos(0);
				GetApp()->get_PlayerPosition(&curPos);
				CComPtr<IITTrack> track;
				GetApp()->get_CurrentTrack(&track);
				if(track)
				{
					long length(0);
					track->get_Duration(&length);

					CComBSTR artist;
					HRESULT hr = S_OK;
					if(counter < 9)
						hr = track->get_Artist(&artist);

					if (counter >= 9 || FAILED(hr) || !artist  || artist[0] == 0)
						track->get_Name(&artist);

					g.SetSmoothingMode(SmoothingModeAntiAlias);
					g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
					Gdiplus::FontFamily  fontFamily(skin->GetFontFace());
					SolidBrush  black(skin->GetTextColor());

					std::wstring ns =  artist.operator BSTR();
					if((ns != scrollText))
					{
						scrollText = ns;
						if(GetDoScroll() && NeedScrolling(GetScrollingText() ))
							BeginScrolling(GetScrollingText() );
						else
							EndScrolling();
					}

					GraphicsPath  path(FillModeAlternate), path2(FillModeAlternate);
					GraphicsPath  path3(FillModeAlternate);
					StringFormat format(0, LANG_NEUTRAL);
					//format.SetLineAlignment(StringAlignmentCenter);
					format.SetAlignment(StringAlignmentCenter);


					if(!IsScrolling() )
					{
						//RectF rc(8,18,120,20);

						path.AddString(artist, -1, &fontFamily, 0, skin->GetFontSize(), skin->GetInfoRect(), &format);
						g.FillPath(&black, &path);
					}
					else
					{
						//g.DrawImage(scrollBmp,0,0);
						g.DrawImage(scrollBmp,skin->GetInfoRect().X,skin->GetInfoRect().Y,l,0,skin->GetInfoRect().Width,skin->GetInfoRect().Height+10,UnitPixel);
					}

					long d = ( length - curPos);
					if(showTimePlayed)
						d = curPos;
					//RectF rc2(8,50,120,20);

					FontFamily fontFamily2(skin->GetCounterFontFace() );
					SolidBrush cbrush(skin->GetCounterTextColor() );
					path2.AddString(FormatTime(d).c_str(),-1,&fontFamily2,0,skin->GetCounterFontSize(),skin->GetCounterRect(),&format);
					g.FillPath(&cbrush,&path2);


					//SolidBrush green(Color(200,0,255,0));
					try
					{
						Rect& sr = skin->GetProgressRect();
						float percentageplayed = ( ( (float)curPos) / ( (float)length) ) * 100;
						if(!skin->GetUseKnobBitmap() )
						{
							if(!skin->GetStretchKnob() )
							{
								path3.AddEllipse((int)((float)sr.Width * percentageplayed / 100.0f) + sr.X,sr.Y,skin->GetKnobWidth(),skin->GetKnobHeight());
								SolidBrush color( skin->GetKnobInnerColor());
								Pen   pen ( skin->GetKnobOuterColor(),1);

								g.FillPath(&color,&path3);
								g.DrawPath(&pen,&path3);
							}
							else
							{
								SolidBrush color( skin->GetKnobInnerColor());
								Pen   pen ( skin->GetKnobOuterColor(),1);
								Rect pr(sr.X,sr.Y,(int)((float)sr.Width * percentageplayed / 100.0f),skin->GetKnobHeight());
								path3.AddRectangle(pr );
								g.FillPath(&color,&path3);
								g.DrawPath(&pen,&path3);
							}
						}
						else
						{
							if(!skin->GetStretchKnob() )
								g.DrawImage(knobBmp,(int)((float)sr.Width * percentageplayed / 100.0f) + sr.X,sr.Y,skin->GetKnobWidth(),skin->GetKnobHeight());
							else
								g.DrawImage(knobBmp,sr.X,sr.Y,0,0,(int)((float)sr.Width * percentageplayed / 100.0f),skin->GetKnobHeight(),UnitPixel);

						}

						//SolidBrush color( Color(255,73,149,237));
						//Pen   pen ( Color(255,34,71,128),1);
				//	g.FillRectangle(&color,4.0f,42.0f,(135.0f * percentageplayed / 100.0f),5.0f);
					//g.DrawImage(knob,(int)(135.0f * percentageplayed / 100.0f),41,knob->GetWidth(),knob->GetHeight() );
					}
					catch(...)
					{

					}
				
				}
			}

			DeskletLayerSetImage(GetWindow(),id,TRUE,bmp,TRUE);

		}
		wasPaused = isPaused;
}

iTunesWidget* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE dll,const char* ini, const char* section)
{
	iTunesWidget* iTunes = new iTunesWidget(hwndDesklet,dll);

	if(ini && section)
		iTunes->ReadFromIni(ini,section);
	else
		iTunes->SetSkin("Default");

	iTunes->Redraw();
	if(iTunes->GetApp() || iTunes->GetHideWhenNotLoaded() )
		SetTimer(hwndDesklet,TIMERID,1000,0);

	DeskletActivateExtendedDragDrop(hwndDesklet);

	return iTunes;
}

void __stdcall OnSave(iTunesWidget* iTunes, const char* ini, const char* section, BOOL bUnused)
{
	iTunes->SaveToIni(ini,section);
}

void __stdcall OnDestroy(iTunesWidget* iTunes, HWND hwndDesklet)
{
	if(iTunes->GetApp())
		KillTimer(hwndDesklet,TIMERID);
	SleepEx(10,0);
	delete iTunes;
}

void __stdcall OnProcessMessage(iTunesWidget* iTunes, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_TIMER && wParam == TIMERID)
	{
		//if(!iTunes->IsScrolling())
			iTunes->Count();

		if(iTunes->GetStatus())
		{
			VARIANT_BOOL inProgress = { FALSE };
			iTunes->GetStatus()->get_InProgress(&inProgress);
			if(!inProgress)
			{
				CComPtr<IITTrackCollection> tracks;
				iTunes->GetStatus()->get_Tracks(&tracks);
				if(tracks)
				{
					long count(0);
					tracks->get_Count(&count);
					if(count > 0)
					{
						CComPtr<IITTrack> track;
						// first track
						tracks->get_Item(1,&track);
						if(track)
						{
							track->Play();
						}
						iTunes->GetStatus().Release();
					}
				}
			}
		}
		iTunes->Redraw();
	}
	else
	{
		if(msg == WM_TIMER && wParam == SCROLLERID)
		{
			if(!iTunes->GetScrollBackwards())
			{
				if(iTunes->GetScrollingWidth() - iTunes->GetScrollingLeft() > iTunes->GetSkin()->GetInfoRect().Width )
				{
					iTunes->SetScrollingLeft(iTunes->GetScrollingLeft() +2 );
				}
				else
				{
					iTunes->SetScrollCounter(iTunes->GetScrollCounter() + 1);
					if(iTunes->GetScrollCounter() > 5)
					{
						iTunes->SetScrollCounter(0);
						iTunes->SetScrollBackwards(true);
					}
				}
			}
			else
			{
				if(iTunes->GetScrollingLeft() > 0 )
				{
					iTunes->SetScrollingLeft(iTunes->GetScrollingLeft() -2 );
				}
				else
				{
					iTunes->SetScrollCounter(iTunes->GetScrollCounter() + 1);
					if(iTunes->GetScrollCounter() > 5)
					{
						iTunes->SetScrollCounter(0);
						iTunes->SetScrollBackwards(false);
					}
				}
			}
			iTunes->Redraw();
		}
	}
}

inline bool PointInRect(Rect& r,POINT& p)
{
	return (p.x > r.X && p.x < (r.X+r.Width) && p.y > (r.Y) && p.y < (r.Y + r.Height));
}

BOOL __stdcall OnLeftButtonClick(iTunesWidget* iTunes, POINT* p, SIZE* s)
{

	iTunes->CheckLoadedStatus();

	if(!iTunes->GetApp())
	{
		iTunes->GetApp().CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
		if(iTunes->GetApp())
			SetTimer(iTunes->GetWindow(),TIMERID,1000,0);
		iTunes->SetAutoHideiTunes(iTunes->GetAutoHideiTunes());
	}
	if(iTunes->GetApp())
	{
		Rect& pr = iTunes->GetSkin()->GetPauseRect();
		Rect& nr = iTunes->GetSkin()->GetNextRect();
		Rect& br = iTunes->GetSkin()->GetPrevRect();
		Rect& cr = iTunes->GetSkin()->GetCounterRect();
		Rect& dr = iTunes->GetSkin()->GetDisplayRect();
		Rect& prr= iTunes->GetSkin()->GetProgressRect();

		if(PointInRect(iTunes->GetSkin()->GetStopRect(),*p ) )
		{
			iTunes->GetApp()->Stop();
		}
		else if(PointInRect(iTunes->GetSkin()->GetIpodRect(),*p  ) )
		{
			iTunes->GetApp()->UpdateIPod();
		}
		else if(PointInRect(iTunes->GetSkin()->GetMuteRect(),*p  ) )
		{
			VARIANT_BOOL isMuted = { VARIANT_FALSE };
			iTunes->GetApp()->get_Mute(&isMuted);
			if(isMuted == VARIANT_TRUE)
				isMuted = VARIANT_FALSE;
			else
				isMuted = VARIANT_TRUE;
			iTunes->GetApp()->put_Mute(isMuted);
		}
		else if(PointInRect(iTunes->GetSkin()->GetOpenRect(),*p  ))
		{
			char buffer[MAX_PATH*200] = {0};
			char fileTitle[MAX_PATH] = {0};
			OPENFILENAME ofn = {0};
			ofn.lStructSize = sizeof(ofn);
			ofn.hInstance = iTunes->GetDLL();
			ofn.hwndOwner = iTunes->GetWindow();
			ofn.lpstrFilter = "MP3 Files(*.mp3)\0*.mp3\0All Files(*.*)\0*.*\0\0";
			ofn.lpstrFile  = buffer;
			ofn.nMaxFile = sizeof(buffer)/sizeof(buffer[0]);
			ofn.lpstrFileTitle = fileTitle;
			ofn.nMaxFileTitle = MAX_PATH;
			ofn.Flags = /*OFN_ALLOWMULTISELECT |*/ OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			if(GetOpenFileName(&ofn) )
			{
				char dir[MAX_PATH] = {0};
				strncpy(dir,buffer,ofn.nFileOffset);
				dir[ofn.nFileOffset] = '\0';
				char* curPos = &buffer[0];
				curPos += ofn.nFileOffset;

				OLECHAR wPath[MAX_PATH] = {0};
				MultiByteToWideChar(CP_OEMCP,0,buffer,-1,wPath,MAX_PATH);
				CComBSTR bstrFile(wPath);

				CComPtr<IITLibraryPlaylist> library;
				if(iTunes->GetApp() )
				{
					iTunes->GetApp()->get_LibraryPlaylist(&library);	
					if(library)
					{
						library->AddFile(bstrFile,&iTunes->GetStatus());
					}
				}
			}
		/*	if( GetOpenFileName(&ofn) )
			{

				char dir[MAX_PATH] = {0};
				strncpy(dir,buffer,ofn.nFileOffset);
				dir[ofn.nFileOffset] = '\0';
				char* curPos = &buffer[0];
				curPos += ofn.nFileOffset;

				CComPtr<IITLibraryPlaylist> library;
				if(iTunes->GetApp() )
					iTunes->GetApp()->get_LibraryPlaylist(&library);

				std::vector<std::wstring>files;
				while( *curPos )
				{
					OLECHAR path[MAX_PATH]= {0};
					char file[MAX_PATH*2] = {0};
					strcpy(file,dir);
					PathAddBackslash(file);
					PathAppend(file,curPos);
					curPos += strlen(curPos)+1;


					MultiByteToWideChar(CP_OEMCP,0,file,-1,path,MAX_PATH);
					files.push_back(std::wstring(path));
				}

				HRESULT hr= S_OK;

				SAFEARRAYBOUND aDim[1];
				aDim[0].lLbound= 0;
				aDim[0].cElements= files.size();
				SAFEARRAY *pSA= SafeArrayCreate(VT_VARIANT,1,aDim);
				if (pSA != NULL)
				{
					for (long i= 0; i < files.size(); ++i)
					{ 
						CComVariant out;
						out.ChangeType(VT_BSTR);
						out.bstrVal = SysAllocString(files[i].c_str() );


						long index[1] = {i};
						if (hr= SafeArrayPutElement(pSA, index, &out))
						{
							SafeArrayDestroy(pSA);
							return FALSE;
						}
					}
				}

				if(library)
				{
					CComVariant vr;
					vr.ChangeType(VT_ARRAY|VT_BSTR);
					vr.parray = pSA;
					hr = library->AddFiles(&vr,&iTunes->GetStatus());

				}

				SafeArrayDestroy(pSA);

				return TRUE;
			}*/
		}
		else if(PointInRect(iTunes->GetSkin()->GetQuitRect(),*p ) )
		{
			iTunes->GetApp()->Quit();
		}
		else if(PointInRect(iTunes->GetSkin()->GetToggleITunesWindowRect(),*p ) )
		{
			HWND hwnd = FindWindow("iTunes",0);
			if(hwnd)
			{
				if(IsWindowVisible(hwnd))
					ShowWindow(hwnd,SW_HIDE);
				else
					ShowWindow(hwnd,SW_SHOW);
			}
		}
		else if(p->x > pr.X && p->x < (pr.X+pr.Width) && p->y > (pr.Y) && p->y < (pr.Y + pr.Height))
		{
			iTunes->GetApp()->PlayPause();
			iTunes->Redraw();
		}
		else if(p->x > nr.X && p->x < (nr.X+nr.Width) && p->y > (nr.Y) && p->y < (nr.Y + nr.Height))
		{
			iTunes->GetApp()->NextTrack();
			iTunes->Redraw();
		}
		else if(p->x > br.X && p->x < (br.X+br.Width) && p->y > br.Y && p->y < (br.Y+br.Height) )
		{
			iTunes->GetApp()->BackTrack();
			iTunes->Redraw();
		}
		else if(p->x > (dr.X+cr.Y) && p->x < (dr.X+cr.X+cr.Width) && p->y > (dr.Y+cr.Y) && p->y < (dr.Y+cr.Y+cr.Height))
		{
			iTunes->ToggleShowTimePlayed();
			iTunes->Redraw();
		}
		else if(p->x > (dr.X+prr.X) && p->x < (dr.X+prr.X+prr.Width) && p->y > (prr.Y+dr.Y) && p->y < (prr.Y+dr.Y+prr.Height))
		{
			CComPtr<IITTrack> track;
			iTunes->GetApp()->get_CurrentTrack(&track);
			if(track)
			{
				long length(0);
				track->get_Duration(&length);
				int pos = p->x - (dr.X+prr.X);
				float percentage = ((float)pos) / (float)(prr.Width) * 100.0f;
				iTunes->GetApp()->put_PlayerPosition((((float)length) *( percentage/100.0f)));
			}
			iTunes->Redraw();
		}
	}
	else
	{
		MessageBox(0,"You seem to have the wrong version of iTunes.",0,0);
	}
	return TRUE;
}

BOOL __stdcall OnRightButtonClick(iTunesWidget* iTunes, POINT* p, SIZE* s)
{
	iTunes->CheckLoadedStatus();

	if(!iTunes->GetApp())
	{
		iTunes->GetApp().CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
		if(iTunes->GetApp())
			SetTimer(iTunes->GetWindow(),TIMERID,1000,0);
		iTunes->SetAutoHideiTunes(iTunes->GetAutoHideiTunes());
	}

	HMENU menu = DeskletGetDefaultMenu(iTunes->GetWindow(),20000);
	AppendMenu(menu,MF_SEPARATOR,0,"-");

	std::vector<CComPtr<IITPlaylist> > v_playlists;
	CComPtr<IiTunes>& app = iTunes->GetApp();
	if(app)
	{
		CComPtr<IITSourceCollection> sources;
		app->get_Sources(&sources);
		if(sources)
		{
			long count(0);
			sources->get_Count(&count);
			for(long i=0;i<count;++i)
			{
				CComPtr<IITSource> source;
				sources->get_Item(i,&source);
				if(source)
				{
					HMENU plmenu = CreatePopupMenu();
					BSTR name;
					source->get_Name(&name);
					CComPtr<IITPlaylistCollection> playlists;
					source->get_Playlists(&playlists);
					if(playlists)
					{
						long plcount(0);
						playlists->get_Count(&plcount);
						for(long pli=0; pli < plcount; ++pli)
						{
							CComPtr<IITPlaylist> playlist;
							playlists->get_Item(pli,&playlist);
							if(playlist)
							{
								BSTR plname;
								playlist->get_Name(&plname);
								AppendMenuW(plmenu,MF_STRING,v_playlists.size(),plname);
								SysFreeString(plname);
								v_playlists.push_back(playlist);
							}
						}
					}
					AppendMenuW(menu,MF_POPUP ,(UINT)plmenu,name);

					SysFreeString(name);
				}
			}
		}
	}
	POINT pt = {0};
	GetCursorPos(&pt);
	int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, iTunes->GetWindow(),0);
	if(cmd > 20000)
	{
		DeskletDoDefaultMenuCommand(iTunes->GetWindow(),20000,cmd);
	}
	else if(cmd > 0 && cmd <= v_playlists.size() )
	{
		CComPtr<IITPlaylist>& playlist = v_playlists[cmd];
		playlist->PlayFirstTrack();
	}
	DestroyMenu(menu);
	return TRUE;
}

BOOL __stdcall OnMouseWheelForward(iTunesWidget* iTunes, WORD fKeysDown, int numOfScrolls, int x, int y)
{
	if(iTunes->GetApp())
	{
		long volume(0);
		iTunes->GetApp()->get_SoundVolume(&volume);
		if(volume<100)
		{
			VARIANT_BOOL mute = {FALSE};
			iTunes->GetApp()->put_Mute(mute);
			volume+=5;
			iTunes->GetApp()->put_SoundVolume(volume);
		}
	}
	return TRUE;
}

BOOL __stdcall OnMouseWheelBackward(iTunesWidget* iTunes, WORD fKeysDown, int numOfScrolls, int x, int y)
{
	if(iTunes->GetApp())
	{
		long volume(0);
		iTunes->GetApp()->get_SoundVolume(&volume);
		if(volume>0)
		{
			volume-=4;
			iTunes->GetApp()->put_SoundVolume(volume);
		}
	}
	return TRUE;
}

BOOL __stdcall OnDragEnter(iTunesWidget* iTunes,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	return TRUE;
}

BOOL __stdcall OnDragLeave(iTunesWidget* iTunes)
{
	return TRUE;
}

BOOL __stdcall OnDrop(iTunesWidget* iTunes,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(IsDataObjectHDROP(obj) )
	{
		if(!iTunes->GetApp())
		{
			iTunes->GetApp().CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
			if(iTunes->GetApp())
				SetTimer(iTunes->GetWindow(),TIMERID,1000,0);
		}	
		
		if(iTunes->GetStatus())iTunes->GetStatus().Release();

		DropStruct* ds = LockHDROP(obj);
	
		int num = DragQueryFile(ds->hDrop, -1, 0, 0);

		CComPtr<IITLibraryPlaylist> library;
		if(iTunes->GetApp() )
			iTunes->GetApp()->get_LibraryPlaylist(&library);

		CComBSTR firstFile;
		for(int i=0;i<num;++i)
		{
			OLECHAR path[MAX_PATH]= {0};
			char	lp[MAX_PATH] = {0};
			DragQueryFileW(ds->hDrop,i,path,MAX_PATH);
			DragQueryFile(ds->hDrop,i,lp,MAX_PATH);
			CComBSTR file(path);

			char ext[5] = {0};
			int sl = strlen(lp);
			if(sl > 4)
			{
				strcpy(ext,lp+sl-4);
				ext[0] = tolower(ext[0]);
				ext[1] = tolower(ext[1]);
				ext[2] = tolower(ext[2]);
				ext[3] = tolower(ext[3]);
			}
			
			if( strcmp(ext,".png") == 0	|| 
				strcmp(ext,".jpg") == 0	|| 
				strcmp(ext,".bmp") == 0	|| 
				strcmp(ext,"jpeg") == 0
			)
			{
				CComPtr<IITTrack> track;
				iTunes->GetApp()->get_CurrentTrack(&track);
				if(track)
				{
					CComPtr<IITArtwork> artwork;
					track->AddArtworkFromFile(file,&artwork);
					iTunes->ResetSongName();
				}
			}
			else
			{
				if(num==0)
					firstFile = file;
				if(library)
				{
					HRESULT hr = {0};
					hr = library->AddFile(file.operator BSTR(),&iTunes->GetStatus());
				}
			}
		}

		UnlockHDROP(ds);

	}
	return TRUE;
}

BOOL __stdcall OnMouseMove(iTunesWidget* iTunes, int x, int y)
{
	/*if(x > 106 + 8 && x < 106+8 + 120 && y > 15+8  && y < 15 + 8 + 20)
	{
		if(!iTunes->IsScrolling() && iTunes->NeedScrolling(iTunes->GetScrollingText() ))
		{
			iTunes->SetDoScroll(true);
			iTunes->BeginScrolling(iTunes->GetScrollingText() );
		}
	}
	else
	{
		if(iTunes->IsScrolling())
		{
			iTunes->SetDoScroll(false);
			iTunes->EndScrolling();
			iTunes->Redraw();
		}
	}*/
	return TRUE;
}

BOOL __stdcall OnMouseOn(iTunesWidget* iTunes)
{
	iTunes->SetDoScroll(true);
	if(!iTunes->IsScrolling() && iTunes->NeedScrolling(iTunes->GetScrollingText() ))
	{
		iTunes->BeginScrolling(iTunes->GetScrollingText() );
	}
	SetCapture(iTunes->GetWindow() );
	return TRUE;
}

BOOL __stdcall OnMouseOut(iTunesWidget* iTunes)
{
	iTunes->SetDoScroll(false);
	if(iTunes->IsScrolling())
	{
		iTunes->EndScrolling();
		iTunes->Redraw();
	}
	
	ReleaseCapture();
	return TRUE;
}

int __stdcall iTunesConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	iTunesWidget* iTunes = (iTunesWidget*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			iTunes = (iTunesWidget*)GetProp(hDlg,"lpData");
			if(!iTunes)return TRUE;

			CheckDlgButton(hDlg,IDC_HIDEWIDGET, iTunes->GetHideWhenNotLoaded());
			CheckDlgButton(hDlg,IDC_HIDEITUNES, iTunes->GetAutoHideiTunes() );

			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_HIDEWIDGET: case IDC_HIDEITUNES:
					if(HIWORD(wParam)==BN_CLICKED)
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
					if(iTunes)
					{
						iTunes->SetHideWhenNotLoaded( IsDlgButtonChecked(hDlg,IDC_HIDEWIDGET) );
						iTunes->SetAutoHideiTunes( IsDlgButtonChecked(hDlg,IDC_HIDEITUNES) );
					}
				break;
			}
		break;
	}
	return FALSE;
}

int __stdcall SkinConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	iTunesWidget* iTunes = (iTunesWidget*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			iTunes = (iTunesWidget*)GetProp(hDlg,"lpData");
			if(!iTunes)return TRUE;
			
			{
				char path[MAX_PATH] = {0};
				GetDLLPath(iTunes->GetDLL(),path,MAX_PATH);
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
				SendDlgItemMessage(hDlg,IDC_SKIN,CB_SELECTSTRING,0,(LPARAM)iTunes->GetSkinDir());
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
					if(iTunes)
					{
						char path[MAX_PATH] = {0};
						GetDlgItemText(hDlg,IDC_SKIN,path,MAX_PATH);
						iTunes->SetSkin(path);
					}
				break;
			}
		break;
	}
	return FALSE;
}

INT __stdcall OnAddPropertySheets(iTunesWidget* iTunes, int numPresent)
{
	PROPSHEETPAGE psp = {0};

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = iTunes->GetDLL();
	psp.pszTitle = "iTunes interaction";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_ITUNESSETTINGS);
	psp.pfnDlgProc = iTunesConfigProc;
	psp.lParam = (LPARAM)iTunes;

	DeskletAddPropertySheet(iTunes->GetWindow(),CreatePropertySheetPage(&psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = iTunes->GetDLL();
	psp.pszTitle = "Skin";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_SKIN);
	psp.pfnDlgProc = SkinConfigProc;
	psp.lParam = (LPARAM)iTunes;

	return DeskletAddPropertySheet(iTunes->GetWindow(),CreatePropertySheetPage(&psp));
}