// DeskletMain.cpp file


#include <windows.h>
#include <atlbase.h>
#include <gdiplus.h>
#include "../SDK/DeskletSDK.h"
#include "../SDK/AveControls.h"
#include "../SDK/AveConstants.h"
#include "../SDK/AveFlags.h"
#pragma comment(lib, "../SDK/AveControls.lib")
#include <string>
#include "F:\iTunesCOM\ITunesCOMInterface_i.c"
#include "F:\iTunesCOM\iTunesCOMInterface.h"
#include "FlipAnimation.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <math.h>
#include <sstream>


SIZE size = {360+158+10, 160};
RECT bgRect = {0,37, 0 + 400, 37 + 86};
RECT frameRect = {32, -50,  32 + 128, -50 + 256};
RECT songRect = {160, 40, 160 + 360, 40 + 24};
RECT artistRect = {160, 65, 160 + 360, 65 + 24};

RECT leftRect = {0, 37, 0 + 158, 37 + 86};
RECT midRect = {0 + 158, 37, 0 + 158 + 0, 37 + 86};
int rightWidth = 10;
int midMaxWidth = 460;
int leftWidth = 158;
int bgHeight = 86;


AVE_SHADOWPROPERTIES props = {
	sizeof(AVE_SHADOWPROPERTIES),
	0,
	0,
	0,
	0,
	0xFFFFFFFF,
	2.5f,
	50
};


const NORMALROTATION = 8;
const FULLSPEED = 20;
const SLOWSPEED = 5;

const TIMEMODE_INCREASING = 0;
const TIMEMODE_DECREASING = 1;

class Desklet
{
	HWND hwnd;
	HINSTANCE dll;
	std::wstring dllPath;

	int rotation;
	int delta;
	int totalRotation;
	int frameCounter;
	CFlipAnimation* anim;

	Bitmap* back;
	Bitmap* oldBack;
	Bitmap* front;
	Bitmap* frame;

	CComPtr<IiTunes> app;

	std::wstring songName;
	std::wstring artistName;
	std::wstring elapsedTime;
	std::wstring albumartFilename;

	int checkCounter;

	AVEHANDLE res;
	AVEHANDLE container;
	AVEHANDLE lblSongName;
	AVEHANDLE lblArtistName;
	AVEHANDLE lblTimeLeft;

	UINT midWidth;
	int timeMode;
	long elapsed;
	long duration;

public:
	Desklet(HWND hwnd, HINSTANCE dll, const char* ini, const char* group) :
		hwnd(hwnd),
		dll(dll),
		rotation(0),
		anim(NULL),
		totalRotation(0),
		frame(NULL),
		front(NULL),
		back(NULL),
		oldBack(NULL),
		delta(5),
		checkCounter(0),
		frameCounter(0),
		res(NULL),
		container(NULL),
		lblSongName(NULL),
		lblTimeLeft(NULL),
		lblArtistName(NULL),
		midWidth(0),
		elapsed(0),
		duration(0),
		timeMode(TIMEMODE_DECREASING)
	{
		DeskletSetImageSetStatus(hwnd, FALSE);
		DeskletSetSize(hwnd, &size);

		midWidth = CalcStringWidth(L"iTunes not started..", songRect);

		SetDLLPath();
		frame = DeskletLoadGDIPlusImageW(GetImageFileName(L"frame.png").c_str());
		if(!(ini && group))
		{
			DeskletSetLabel(hwnd, "");
		}

		ClearAllLayers();
		SetupLayers();
		SetupControls();

		CheckTunes();
		if(NULL == app.p)
			ChangeFrameLayer();

		SetTimer(hwnd, 3, 5000, NULL);
		SetTimer(hwnd, 4, 1000, NULL);
	}

	static UINT CalcStringWidth(const WCHAR* str, RECT r)
	{
		AVE_FONT2 fn = GetFont();
		Rect rc(0, 0, r.right - r.left,r.bottom - r.top);

		FontFamily ff(fn.faceName);

		GraphicsPath gp;
		gp.AddString(str,-1, &ff,(FontStyle)fn.style, fn.size,rc,0);

		Rect rcBounds;
		gp.GetBounds(&rcBounds);

		return rcBounds.Width + 10;
	}

	static AVE_FONT2 GetFont()
	{
		AVE_FONT2 fn = {0};
		wcscpy(fn.faceName , L"Verdana");
		fn.size = 12.0f;
		fn.style = FontStyleBold;
		return fn;
	}

	AVEHANDLE MakeLabel(const char* name, RECT* r)
	{
		AVE_FONT2 fn = GetFont();

		AVEHANDLE lbl = AveCreateControl(container, name, AVE_LABEL_CONTROL, 0, NULL);
		AveLockControl(lbl);
		AveSendMessage(lbl, AV_SET_RECT, (WPARAM)r, (LPARAM)&size);
		AveSendMessage(lbl, AV_SET_FONT2, 0L, (LPARAM)&fn);
		AveSendMessage(lbl, AV_SET_SHADOWPROPERTIES, (WPARAM)TRUE, (LPARAM)&props);
		AveSendMessage(lbl, AV_SET_TEXTCOLOR, 0L, (LPARAM)Color(255,0,0,0).GetValue());
		AveSendMessage(lbl, AV_SET_ALIGNMENT, (WPARAM)ALIGNMENT_FAR, (LPARAM)ALIGNMENT_NEAR);

		AveUnlockControl(lbl);
		AveAddControlToContainer(container, lbl);

		return lbl;
	}

	void SetupControls()
	{
		res = AveCreateResourcesW(GetImageFileName(L"controls\\").c_str());
		container = AveCreateControlContainer(hwnd, res);

		AveLockContainer(container);

		int mw = GetMidWidth() + leftRect.right;
		RECT tlRect = {mw - 40, 100, mw+10, 120};
		lblSongName = MakeLabel("songname", &songRect);
		lblArtistName = MakeLabel("artistname", &artistRect);
		lblTimeLeft = MakeLabel("timeleft", &tlRect);

		AveSendMessage(lblSongName, AV_SET_TEXT, 0L, (LPARAM)L"iTunes Not Started");
		AveSendMessage(lblTimeLeft, AV_SET_TEXT, 0L, (LPARAM)L"00:00");

		AveUnlockContainer(container);
	}

	void SetDLLPath()
	{
		WCHAR wPath[MAX_PATH] = {0};
		GetDLLPathW(dll, wPath, MAX_PATH);
		dllPath = wPath;
	}

	void ClearAllLayers()
	{
		while(DeskletLayerCount(hwnd) > 0)
			DeskletLayerRemove(hwnd, 0, FALSE);
	}

	std::wstring GetImageFileName(WCHAR* img)
	{
		WCHAR fname[MAX_PATH] = {0};
		GetDLLPathW(dll, fname, MAX_PATH);
		wcscat(fname, L"images\\");
		wcscat(fname, img);

		return fname;
	}

	void SetLayerImage(UINT lid, WCHAR* img, BOOL redraw=FALSE)
	{
		DeskletLayerSetImageFileW(hwnd, lid, redraw, GetImageFileName(img).c_str());
	}

	UINT GetMidWidth()
	{
		return midWidth;
	}

	void SetupLayers()
	{
		UINT leftId = DeskletLayerAdd(hwnd, "left");
		DeskletLayerSetPlacement(hwnd, leftId, FALSE, TRUE, &leftRect, size.cx, size.cy);
		SetLayerImage(leftId, L"left.png");



		UINT midId = DeskletLayerAdd(hwnd, "mid");
		int mw = GetMidWidth();
		RECT r = midRect;
		r.right += mw;
		RECT r2 = {0, 0, mw, bgHeight};
		DeskletLayerSetPlacement(hwnd, midId, FALSE, TRUE, &r, size.cx, size.cy);
		DeskletLayerSetImageClipRect(hwnd, midId, FALSE, TRUE, &r2, midMaxWidth, bgHeight);
		SetLayerImage(midId, L"mid.png");


		UINT rightId = DeskletLayerAdd(hwnd, "right");
		RECT r3 = {r.right, r.top, r.right + rightWidth, r.bottom};
		DeskletLayerSetPlacement(hwnd, rightId, FALSE, TRUE, &r3, size.cx, size.cy);
		SetLayerImage(rightId, L"right.png");

		UINT frameId = DeskletLayerAdd(hwnd, "frame");
		DeskletLayerSetPlacement(hwnd, frameId, FALSE, TRUE, &frameRect, size.cx, size.cy);
		SetLayerImage(frameId, L"no_artwork.png");
	}

	static std::wstring BStrToWString(BSTR bstr)
	{
		if(NULL == bstr)
			return L"";
		else
			return static_cast<WCHAR*>(bstr);
	}

	void UpdateMid()
	{
		UINT midId = DeskletLayerGetLayer(hwnd, "mid", NULL);
		int mw = GetMidWidth();
		RECT r = midRect;
		r.right += mw;
		RECT r2 = {0, 0, mw, bgHeight};
		DeskletLayerSetPlacement(hwnd, midId, FALSE, TRUE, &r, size.cx, size.cy);
		DeskletLayerSetImageClipRect(hwnd, midId, FALSE, TRUE, &r2, midMaxWidth, bgHeight);
		SetLayerImage(midId, L"mid.png");


		UINT rightId = DeskletLayerGetLayer(hwnd, "right", NULL);
		RECT r3 = {r.right, r.top, r.right + rightWidth, r.bottom};
		DeskletLayerSetPlacement(hwnd, rightId, FALSE, TRUE, &r3, size.cx, size.cy);
		SetLayerImage(rightId, L"right.png");

		int tlLeft = GetMidWidth() + leftRect.right;
		RECT tlRect = {tlLeft - 40, 100, tlLeft+10, 120};
		AveSendMessage(lblTimeLeft, AV_SET_RECT, (WPARAM)&tlRect, (LPARAM)&size);

	}

	void UpdateTunesInfo()
	{
		int oldMidWidth = midWidth;

		UpdateTimeDisplayString();

		if(app.p != NULL)
		{
			midWidth = 0;

			AveSendMessage(lblSongName, AV_SET_TEXT, 0L, (LPARAM)songName.c_str());
			midWidth = max(midWidth, CalcStringWidth(songName.c_str(), songRect));

			AveSendMessage(lblArtistName, AV_SET_TEXT, 0L, (LPARAM)artistName.c_str());
			midWidth = max(midWidth, CalcStringWidth(artistName.c_str(), artistRect));

			AveSendMessage(lblTimeLeft, AV_SET_TEXT, 0L, (LPARAM)elapsedTime.c_str());
			RECT tlRect = {0, 0, 50, 20};
			midWidth = max(midWidth, CalcStringWidth(elapsedTime.c_str(), artistRect));
		}
		else
		{
			AveSendMessage(lblSongName, AV_SET_TEXT, 0L, (LPARAM)L"iTunes not started.");
			AveSendMessage(lblArtistName, AV_SET_TEXT, 0L, (LPARAM)L"");
			AveSendMessage(lblTimeLeft, AV_SET_TEXT, 0L, (LPARAM)L"");

			midWidth = CalcStringWidth(L"iTunes not started..", songRect);
		}

		if(midWidth != oldMidWidth)
			UpdateMid();
	}

	void GetTunesInfo()
	{
		songName = L"";
		artistName = L"";
		elapsedTime = L"";
		albumartFilename = L"";

		if(NULL == app.p)
			return;
		CComPtr<IITTrack> track;
		app->get_CurrentTrack(&track);
		if(NULL == track.p)
			return;

		CComBSTR trackName;
		track->get_Name(&trackName);
		songName = BStrToWString(trackName);

		CComBSTR trackArtist;
		track->get_Artist(&trackArtist);
		artistName = BStrToWString(trackArtist);

		app->get_PlayerPosition(&elapsed);
		track->get_Duration(&duration);

		// try to get albumart fname
		CComPtr<IITArtworkCollection>artworks;
		track->get_Artwork(&artworks);
		if(artworks != NULL)
		{
			long count(0);
			artworks->get_Count(&count);
			if(count > 0)
			{
				CComPtr<IITArtwork> artwork;
				artworks->get_Item(1,&artwork);
				if(artwork)
				{
					WCHAR tmpFilename[MAX_PATH] = {0};
					GetTempPathW(MAX_PATH, tmpFilename);
					PathAppendW(tmpFilename, L"ave_albumart");
					CComBSTR bstrFilename(tmpFilename);
					if(SUCCEEDED(artwork->SaveArtworkToFile(bstrFilename)))
						albumartFilename = tmpFilename;		
				}
			}
		}

		// if no albumart was found, try to use folder.jpg
		if(albumartFilename.length() == 0)
		{
			CComPtr<IITFileOrCDTrack> fileTrack;
			track.QueryInterface(&fileTrack);
			if(fileTrack)
			{
				CComBSTR bstrPath;
	
				if(SUCCEEDED(fileTrack->get_Location(&bstrPath)))
				{
					std::wstring wsPath = BStrToWString(bstrPath);
					if(wsPath.length() > 0)
					{
						WCHAR fname[MAX_PATH] = {0};
						wcscpy(fname, wsPath.c_str());
						 PathRemoveFileSpecW(fname);
						 PathAppendW(fname, L"folder.jpg");
						 if(PathFileExistsW(fname))
							albumartFilename = fname;
					}
				}
			}
		}

	}

	static std::wstring FormatTime(long time)
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

	void UpdateTimeDisplayString()
	{
		if(TIMEMODE_INCREASING == timeMode)
			elapsedTime = FormatTime(elapsed);
		else if(TIMEMODE_DECREASING == timeMode)
			elapsedTime = FormatTime(duration - elapsed);
	}

	Bitmap* CreateFrameBitmap()
	{
		Bitmap* bmp = NULL;

		if(albumartFilename.length() == 0)
			bmp = DeskletLoadGDIPlusImageW(GetImageFileName(L"no_artwork.png").c_str());
		else
		{
			Image* img = frame;
			bmp = (Bitmap*)img->Clone();
			Graphics g(bmp);
			g.SetSmoothingMode(SmoothingModeAntiAlias);
			g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			Bitmap art(albumartFilename.c_str());
			g.DrawImage(&art, 10,10,108,108);
		}

		return bmp;
	}

	void ChangeFrameLayer()
	{
		if(anim != NULL)
			return;

		UINT frameId = DeskletLayerGetLayer(hwnd, "frame", NULL);

		if(GetKeyState(VK_SHIFT) < 0)
			delta = SLOWSPEED;
		else
			delta = FULLSPEED;

		delete front;
		front = DeskletLoadGDIPlusImageW(GetImageFileName(L"frame.png").c_str());
		delete back;
		back  = CreateFrameBitmap();
		if(NULL == oldBack)
			oldBack = DeskletLoadGDIPlusImageW(GetImageFileName(L"no_artwork.png").c_str());
		anim = new CFlipAnimation(front, oldBack);
		if(SUCCEEDED(anim->Initialize(hwnd)))
		{
			frameCounter = 0;
			SetTimer(hwnd, 2, 25, NULL);
		}
		else
		{
			//MessageBox(NULL, "uhoh", 0, 0);
		}
	}

	void NextFrame()
	{
		if(anim != NULL)
		{
			UINT frameId = DeskletLayerGetLayer(hwnd, "frame", NULL);

			Bitmap* bmp = NULL;

			rotation = (rotation + delta) % 180;
			totalRotation += delta;
			if(rotation % 180 == 0)
			{
				if(oldBack != back)
				{
					anim->RebindTexture(false, back);
					delete oldBack;
					oldBack = back;
				}
				anim->SwapBackAndFront();
			}

			if(!(totalRotation >= (360 + NORMALROTATION)))
			{
				anim->FlipToFront(rotation, &bmp);
				if(bmp != NULL)
					DeskletLayerSetImage(hwnd, frameId, TRUE, bmp, TRUE);		
			}

			if(totalRotation >= (360 + NORMALROTATION))
			{
				totalRotation = totalRotation % 360;
				anim->FlipToFront(NORMALROTATION, &bmp);
				if(bmp != NULL)
					DeskletLayerSetImage(hwnd, frameId, TRUE, bmp, TRUE);	

				delete anim;
				anim = NULL;
				back = NULL;
			}

			frameCounter++;
		}
	}

	~Desklet()
	{
		delete anim;
		delete back;
		delete oldBack;
		delete front;
		delete frame;

		AveFreeControlContainer(container);
		AveFreeResources(res);
	}

	void Update()
	{
		if(app.p != NULL)
		{
			std::wstring oldAlbumArt = albumartFilename;
			GetTunesInfo();
			UpdateTunesInfo();

			if(oldAlbumArt != albumartFilename)
				ChangeFrameLayer();
		}
	}

	void CheckTunes()
	{
		if(0 == checkCounter)
		{
			if(NULL == app.p && FindWindow("iTunes", NULL) != NULL)
			{
				app.CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
				GetTunesInfo();
				UpdateTunesInfo();
				ChangeFrameLayer();
			}

		}
		else
		{
			checkCounter--;
		}
	}
};


Desklet* CALLBACK OnCreate(HWND hwnd, HINSTANCE dll, const char* ini, const char* group)
{
	return new Desklet(hwnd, dll, ini, group);
}


void CALLBACK OnDestroy(Desklet* d, HWND hwnd)
{
	delete d;
}
void CALLBACK OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName, "Perspective AlbumArt");
	strcpy(szAuthor, "Andreas Verhoeven");
	(*iVersion) = 100;
	strcpy(szNotes,"Shows albumart and has some nice effects - prototype desklet.");
}

void CALLBACK OnProcessMessage(Desklet* d, HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if(WM_TIMER == msg)
	{
		if(2 == w)
			d->NextFrame();
		else if(3 == w)
			d->CheckTunes();
		else if(4 == w)
			d->Update();

	}
}