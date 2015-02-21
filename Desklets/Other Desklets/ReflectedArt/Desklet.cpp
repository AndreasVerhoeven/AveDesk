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
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <math.h>
#include <sstream>

#import "C:\Program Files\Winamp\Plugins\gen_activewa.dll"

SIZE size = {128, 256};
RECT artRect = {0,0,128,128};
RECT reflectionRect={0,127,128,255};


struct ReflectionParams
{
	float visiblePortion;
	int startAlpha;
	int endAlpha;
};

BOOL GetPrivateProfileBOOLW(const WCHAR* appName, const WCHAR* key, BOOL defVal, const WCHAR* fname)
{
	WCHAR boolString[100] = {0};
	GetPrivateProfileStringW(appName, key, (defVal ? L"1" : L"0"), boolString, sizeof(boolString) / sizeof(boolString[0]), fname);

	return	wcsicmp(boolString, L"true") == 0 ||
			wcsicmp(boolString, L"1") == 0 ||
			wcsicmp(boolString, L"yes") == 0 ||
			wcsicmp(boolString, L"ok") == 0;
}


Rect GetPrivateProfileRectW(const WCHAR* appName, Rect defRect, const WCHAR* fname)
{
	int l = GetPrivateProfileIntW(appName, L"left", defRect.X, fname);	
	int t = GetPrivateProfileIntW(appName, L"top", defRect.Y, fname);	
	int w = GetPrivateProfileIntW(appName, L"width", defRect.Height, fname);	
	int h = GetPrivateProfileIntW(appName, L"height", defRect.Width, fname);	

	return Rect(l, t, w, h);
}

struct Skin
{
	BOOL useFront;
	Rect bgRect;
	BOOL useOverlay;
	Rect overlayRect;
};

Bitmap* makeReflection(Bitmap* img, ReflectionParams params)
{
	Rect rc(0,0,img->GetWidth(), img->GetHeight());
	Bitmap* bmp = new Bitmap(rc.Width, rc.Height);
	BitmapData dest, source;
	bmp->LockBits(&rc, ImageLockModeWrite, PixelFormat32bppARGB, &dest);
	img->LockBits(&rc, ImageLockModeRead, PixelFormat32bppARGB, &source);


	DWORD* srcPtr = (DWORD*) source.Scan0;
	srcPtr += source.Stride/4 * source.Height;
	DWORD* destPtr = (DWORD*) dest.Scan0;

	float visiblePortion = 100.0f / float(params.visiblePortion);
	float startAlpha(params.startAlpha);
	float alphaPerLine(float(startAlpha - params.endAlpha) / (float(dest.Height)/visiblePortion));
	float curLineAlpha(startAlpha);

	for(int y=0; y < int(float(dest.Height)/visiblePortion); ++y)
	{
		DWORD* srcBuf  = srcPtr - source.Stride/4;
		DWORD* destBuf = destPtr;
		for(int x=0; x < dest.Width; ++x)
		{
			BYTE* alphaByte = (BYTE*)destBuf;
			alphaByte += 3;
			*destBuf++ = *srcBuf++;
			float alphaVal(*alphaByte);
			alphaVal *= curLineAlpha / 255.0f;
			*alphaByte = BYTE(alphaVal);
		}

		destPtr += (dest.Stride/4);
		srcPtr  -= (source.Stride/4);

		curLineAlpha -= alphaPerLine;
	}

	img->UnlockBits(&source);
	bmp->UnlockBits(&dest);

	return bmp;
}

#include <atlbase.h> //includes windows

extern CComModule _Module;


#include <atlcom.h>
#include <atlapp.h>
#include <atlmisc.h>
#include <gdiplus.h>
#include <atlwin.h>
#include <atlctrls.h>
#include <atldlgs.h>

class ATL_NO_VTABLE Desklet :
	public CComObjectRoot,
	public IDispatch
{
public:


	BOOL bKeepOff;
	DWORD dwCookie;

	BOOL autoHide;
	BOOL showCDCase;

	ActiveWinamp::IApplication* winampApp;

	BEGIN_COM_MAP (Desklet)
		COM_INTERFACE_ENTRY_IID(IID_IDispatch, Desklet)
		COM_INTERFACE_ENTRY_IID(DIID__IiTunesEvents, Desklet)
	END_COM_MAP()

	// manual IDispatch
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo){return E_NOTIMPL;}
        HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT , LCID , ITypeInfo **){return E_NOTIMPL;}
        HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID , LPOLESTR *, UINT , LCID , DISPID *){return E_NOTIMPL;}
//		HRESULT STDMETHODCALLTYPE Invoke(...)// see below


	HWND hwnd;
	HINSTANCE dll;

	UINT artId;
	UINT reflectionId;

	CComPtr<IiTunes> app;
	BOOL winAmpStatus;
	int checkCounter;

	ReflectionParams reflParams;


	std::wstring albumartFilename;
	WCHAR noArtFileName[MAX_PATH];
	std::wstring songName;

	std::wstring tmpFileName;

	Skin skin;

	bool isConnectedToApp()
	{
		return app.p != NULL || winAmpStatus;
	}
	

	void DoCreate(HWND hwnd, HINSTANCE dll, const char* ini, const char* group)
	{
		showCDCase = TRUE;
		winampApp = NULL;
		winAmpStatus = FALSE;
		autoHide = FALSE;
		this->hwnd = hwnd;
		this->dll = dll;
		checkCounter = 0;
		bKeepOff = FALSE;

		skin.useFront = TRUE;
		skin.bgRect.X = 14;
		skin.bgRect.Y = 3;
		skin.bgRect.Width = 112;
		skin.bgRect.Height = 123;

		skin.useOverlay = TRUE;
		skin.overlayRect.X = 10;
		skin.overlayRect.Y = 10;
		skin.overlayRect.Width = 152;
		skin.overlayRect.Height = 149;


		WCHAR tempPath[MAX_PATH] = {0};
		WCHAR tmpFname[MAX_PATH] = {0};
		GetTempPathW(MAX_PATH, tempPath);
		GetTempFileNameW(tempPath, L"ave", 0, tmpFname);
		tmpFileName = tmpFname;

		reflParams.visiblePortion = 2.5f;
		reflParams.startAlpha = 100;
		reflParams.endAlpha = 0;

		WCHAR iniFile[MAX_PATH] = {0};
		GetDLLPathW(dll, iniFile, MAX_PATH);
		PathAppendW(iniFile, L"settings.ini");
		
		skin.useFront = GetPrivateProfileBOOLW(L"bg", L"useFront", skin.useFront, iniFile);
		skin.bgRect = GetPrivateProfileRectW(L"bg",  skin.bgRect, iniFile);

		skin.useOverlay = GetPrivateProfileBOOLW(L"overlay", L"useOverlay", skin.useOverlay, iniFile);
		skin.overlayRect = GetPrivateProfileRectW(L"overlay",  skin.overlayRect, iniFile);

		//WCHAR visStr[100] = {0};
		reflParams.visiblePortion = GetPrivateProfileIntW(L"reflection", L"visiblePortionPercentage", reflParams.visiblePortion, iniFile);
		reflParams.visiblePortion = max(reflParams.visiblePortion, 1);
		reflParams.visiblePortion = min(reflParams.visiblePortion, 100);

		reflParams.startAlpha = GetPrivateProfileIntW(L"reflection", L"startAlpha", reflParams.startAlpha, iniFile);
		reflParams.endAlpha = GetPrivateProfileIntW(L"reflection", L"endAlpha", reflParams.endAlpha, iniFile);

		memset(noArtFileName, 0, sizeof(noArtFileName));
		GetDLLPathW(dll, noArtFileName, MAX_PATH);
		PathAppendW(noArtFileName, L"images\\noart.png");

		if(!(ini && group))
		{
			DeskletSetLabel(hwnd, "");
			DeskletSetSize(hwnd, &size);
		}
		else
		{
			autoHide   = GetPrivateProfileInt(group, "autoHide", autoHide, ini);
			showCDCase = GetPrivateProfileInt(group, "showCDCase", showCDCase, ini);
		}

		ClearAllLayers();

		artId = DeskletLayerAdd(hwnd, "art");
		DeskletLayerSetPlacement(hwnd, artId, FALSE, TRUE, &artRect, size.cx, size.cy);
		
		reflectionId = DeskletLayerAdd(hwnd, "reflection");
		DeskletLayerSetPlacement(hwnd, reflectionId, FALSE, TRUE, &reflectionRect, size.cx, size.cy);

		CheckAudioApps();
		if(!isConnectedToApp())
		{
			UpdateFromFile(noArtFileName);
			if(autoHide)
				DeskletHide(hwnd);
		}
		else
		{
			GetTunesInfo();
			if(albumartFilename.length() > 0)
					UpdateFromFile(albumartFilename.c_str());
				else
					UpdateFromFile(noArtFileName);
		}


		SetTimer(hwnd, 3, 1000, NULL);
		SetTimer(hwnd, 4, 1000, NULL);
	}

	void UpdateFromFile(const WCHAR* file)
	{
		Bitmap* bmp = new Bitmap(file);
		//Image* img = (Image*)bmp;
		Bitmap* bmpForLayer = new Bitmap(128 , 128);
		Graphics g(bmpForLayer);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		if(showCDCase && skin.useFront)
		{
			WCHAR fPath[MAX_PATH] = {0};
			DeskletGetRelativeFolderW(hwnd, fPath);
			PathAppendW(fPath, L"images\\front.png");
			Bitmap* front = DeskletLoadGDIPlusImageW(fPath);
			g.DrawImage(front, RectF(0, 0, bmpForLayer->GetWidth(), bmpForLayer->GetHeight()), 0, 0, front->GetWidth(), front->GetHeight(), UnitPixel, 0, 0, 0);
			delete front;

			g.DrawImage(bmp, skin.bgRect);
		}
		else
		{
			g.DrawImage(bmp, RectF(0, 0, bmpForLayer->GetWidth(), bmpForLayer->GetHeight()), 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel, 0, 0, 0);
		}

		if(showCDCase && skin.useOverlay)
		{
			WCHAR fPath[MAX_PATH] = {0};
			DeskletGetRelativeFolderW(hwnd, fPath);
			PathAppendW(fPath, L"images\\overlay.png");
			Bitmap* overlay = DeskletLoadGDIPlusImageW(fPath);
			g.DrawImage(overlay, skin.overlayRect, 0, 0, overlay->GetWidth(), overlay->GetHeight(), UnitPixel, 0, 0, 0);
	
			delete overlay;
		}

		delete bmp;

		Bitmap* refl = makeReflection(bmpForLayer, reflParams);
		DeskletLayerSetImage(hwnd, reflectionId, FALSE, refl, TRUE);

		DeskletLayerSetImage(hwnd, artId, TRUE, bmpForLayer, TRUE);
	}

	void ClearAllLayers()
	{
		while(DeskletLayerCount(hwnd) > 0)
			DeskletLayerRemove(hwnd, 0, FALSE);
	}

	static std::wstring BStrToWString(BSTR bstr)
	{
		if(NULL == bstr)
			return L"";
		else
			return static_cast<WCHAR*>(bstr);
	}

	void GetTunesInfo()
	{
		std::wstring oldSongName = songName;
		songName = L"";
		//artistName = L"";
		//elapsedTime = L"";

		if(NULL == app.p)
		{
			albumartFilename = L"";
			return;
		}
		CComPtr<IITTrack> track;
		app->get_CurrentTrack(&track);
		if(NULL == track.p)
		{
			albumartFilename = L"";
			return;
		}

		CComBSTR trackName;
		track->get_Name(&trackName);
		songName = BStrToWString(trackName);

		if(songName != oldSongName)
		{
			albumartFilename = L"";

			//CComBSTR trackArtist;
			//track->get_Artist(&trackArtist);
			//artistName = BStrToWString(trackArtist);

			//app->get_PlayerPosition(&elapsed);
			//track->get_Duration(&duration);

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
						CComBSTR bstrFilename(tmpFileName.c_str());
						if(SUCCEEDED(artwork->SaveArtworkToFile(bstrFilename)))
							albumartFilename = tmpFileName;		
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

	}

	void GetWinAmpInfo()
	{
		std::wstring oldSongName = songName;
		songName = L"";

		CheckWinAmp();
		if(winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return;
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(mip != NULL)
			{
				CComBSTR fileName;
				mip->get_Filename(&fileName);
				songName = BStrToWString(fileName);
				if(songName != oldSongName)
				{
					albumartFilename = L"";
					if(songName.length() > 0)
					{
						WCHAR fname[MAX_PATH] = {0};
						wcscpy(fname, songName.c_str());
						 PathRemoveFileSpecW(fname);
						 PathAppendW(fname, L"folder.jpg");
						 if(PathFileExistsW(fname))
							albumartFilename = fname;
					}
				}
			}
		}
	}

	void Update()
	{
		if(isConnectedToApp())
		{
			std::wstring oldAlbumArt = albumartFilename;
			std::wstring oldSongName = songName;
			if(app.p != NULL)
				GetTunesInfo();
			//UpdateTunesInfo();

			if(winAmpStatus)
				GetWinAmpInfo();

			if(oldAlbumArt != albumartFilename || songName != oldSongName)
			{
				if(albumartFilename.length() > 0)
					UpdateFromFile(albumartFilename.c_str());
				else
					UpdateFromFile(noArtFileName);
			}
		}
	}

	void bindTunes(bool fade=true)
	{
		app.CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
		AtlAdvise(app, this, DIID__IiTunesEvents, &dwCookie );

		if(autoHide && fade)
			DeskletFadeIn(hwnd, TRUE, USE_DEFAULT_DURATION);
	}

	void unbindTunes(bool fade=true)
	{
		if(app.p != NULL)
		{
			albumartFilename = L"";
			songName = L"";
			UpdateFromFile(noArtFileName); 

			bKeepOff = TRUE;
			AtlUnadvise(app, DIID__IiTunesEvents, dwCookie);
			app.Release();

			if(autoHide && fade)
				DeskletFadeOut(hwnd, TRUE, USE_DEFAULT_DURATION);
		}
	}

	HRESULT STDMETHODCALLTYPE Invoke( 
    /* [in] */ DISPID dispIdMember,
    /* [in] */ REFIID riid,
    /* [in] */ LCID lcid,
    /* [in] */ WORD wFlags,
    /* [out][in] */ DISPPARAMS *pDispParams,
    /* [out] */ VARIANT *pVarResult,
    /* [out] */ EXCEPINFO *pExcepInfo,
    /* [out] */ UINT *puArgErr)
{
	//AtlTrace("IT48: Change %ld \n", dispIdMember);
	switch(dispIdMember)
	{
	case 0x00000009:
		//AtlTrace("IT48: Going off the hook...\n");
		if( app.p != NULL )
		{
			unbindTunes();
		}
		break;
	};
	return S_OK;
}


	void CheckAudioApps()
	{
		if(0 == checkCounter)
		{
			CheckTunes();
			CheckWinAmp();
		}
		else
		{
			checkCounter--;
		}
	}

	void bindWinAmp(bool fade=true)
	{
		HRESULT hr = CoCreateInstance(__uuidof(ActiveWinamp::Application), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ActiveWinamp::IApplication), (LPVOID*)&winampApp);
		winAmpStatus = (winampApp != NULL);

		if(winAmpStatus && autoHide && fade)
			DeskletFadeIn(hwnd, TRUE, USE_DEFAULT_DURATION);
	}

	void unbindWinAmp(bool fade=true)
	{
		if(winAmpStatus)
		{
			winAmpStatus = FALSE;
			if(winampApp != NULL)
				winampApp->Release();

			winampApp = NULL;

			if(autoHide && fade)
				DeskletFadeOut(hwnd, TRUE, USE_DEFAULT_DURATION);
		}
	}

	void CheckWinAmp()
	{
		HWND ampWnd = FindWindow("Winamp v1.x", NULL);
		BOOL ampLoaded = IsWindow(ampWnd);

		if(ampLoaded && !isConnectedToApp())
		{
			bindWinAmp();
		}
		else if(!ampLoaded && winAmpStatus)
		{
			unbindWinAmp(); 
		}
	}

	void CheckTunes()
	{
		HWND tunesWnd = FindWindow("iTunes", NULL);
		BOOL tunesLoaded = IsWindow(tunesWnd);

		if(tunesLoaded && bKeepOff)
			bKeepOff = TRUE;
		else if(!tunesLoaded && bKeepOff)
		{
			bKeepOff = FALSE;
		}
		else if(tunesLoaded && !bKeepOff && !isConnectedToApp())
		{
			if(NULL == app.p)
			{
				bindTunes();
				GetTunesInfo();
				Update();
			}
		
		}
	}
};

Desklet* CALLBACK OnCreate(HWND hwnd, HINSTANCE dll, const char* ini, const char* group)
{
//	return new Desklet(hwnd, dll, ini, group);

	CComObject<Desklet>* d = NULL;
	CComObject<Desklet>::CreateInstance(&d);
	d->AddRef();
	d->DoCreate(hwnd, dll, ini, group);
	return d;
}

void __stdcall OnSave(Desklet *d, const char *ini, const char *group, BOOL bIsForExport)
{
	WritePrivateProfileInt(group, "AutoHide", d->autoHide, ini);
	WritePrivateProfileInt(group, "showCDCase", d->showCDCase, ini);
}

void CALLBACK OnDestroy(Desklet* d, HWND hwnd)
{
	KillTimer(d->hwnd, 3);
	KillTimer(d->hwnd, 4);
	d->unbindWinAmp(false);
	d->unbindTunes(false);
	d->Release();
//	delete d;
}
void CALLBACK OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName, "Reflectioned AlbumArt");
	strcpy(szAuthor, "Andreas Verhoeven");
	(*iVersion) = 200;
	strcpy(szNotes,"Shows albumart from iTunes or Winamp on the desktop with a nice reflection effect.\nRight click for settings.");
}

void CALLBACK OnProcessMessage(Desklet* d, HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if(3 == w)
		d->CheckAudioApps();
	else if(4 == w)
		d->Update();
}

BOOL CALLBACK OnRightButtonClick(Desklet *d, POINT* p, SIZE *s)
{
	HMENU menu = DeskletGetDefaultMenu(d->hwnd, 1000);
	AppendMenu(menu, MF_SEPARATOR, 0, "-");
	AppendMenu(menu, MF_STRING | MF_BYCOMMAND, 1, (d->autoHide ? "Disable Auto-Hide" : "Enable Auto-Hide"));
	AppendMenu(menu, MF_STRING | MF_BYCOMMAND, 2, (d->showCDCase ? "Disable CD-Case" : "Enable CD-Case"));

	DWORD mp = GetMessagePos();
	POINT pt = {GET_X_LPARAM(mp), GET_Y_LPARAM(mp) };
	int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, d->hwnd,0);
	DestroyMenu(menu);
	
	if(cmd >= 1000)
		DeskletDoDefaultMenuCommand(d->hwnd, 1000, cmd);
	else
	{
		if(1 == cmd)
		{
			if(d->autoHide)
			{
				d->autoHide = FALSE;
				if(!d->isConnectedToApp())
					DeskletFadeIn(d->hwnd, TRUE, USE_DEFAULT_DURATION);
			}
			else
			{
				d->autoHide = TRUE;
				if(!d->isConnectedToApp())
					DeskletFadeOut(d->hwnd, TRUE, USE_DEFAULT_DURATION);
			}
		}
		else if(2 == cmd)
		{
			d->showCDCase = !d->showCDCase;
			d->albumartFilename = L"";
			d->songName = L"";
			d->Update();
		}
	}

	return TRUE;
}