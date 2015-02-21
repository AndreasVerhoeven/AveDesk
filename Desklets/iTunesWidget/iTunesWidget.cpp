#pragma warning(disable:4786)

#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#pragma comment(lib,"gdiplus.lib")

#include "../../SDK/AVESDK/DeskletSDK.h"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

//! iTunes specific
#include "F:\iTunesCOM\ITunesCOMInterface_i.c"
#include "F:\iTunesCOM\iTunesCOMInterface.h"

#import "C:\Program Files\Winamp\Plugins\gen_activewa.dll"

#include "C:\Program Files\foobar2000\comserver2\include\foobar2000_h.h"


#include "E:\Coding\WMSDK\WMPSDK9\include\wmp.h"

#ifndef __ATLBASE_H__
#include <ATLBASE.h>
#endif

#ifndef __ATLCONV_H__
#include <ATLCONV.H>
#endif

#include "resource.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include "image_funcs.h"

#include <vector>
#include <string>
#include <sstream>
#include <memory>

using namespace Gdiplus;

const TIMERID		= 285772;
const SCROLLERID	= 303292;

#define WM_APP_REDRAW WM_APP + 2939
#define WM_APP_STATUS_CHANGE WM_APP + 2940

CComModule _Module;

#include <atlbase.h> //includes windows

extern CComModule _Module;


#include <atlcom.h>
#include <atlapp.h>
#include <atlmisc.h>
#include <gdiplus.h>
#include <atlwin.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "F:\iTunesCOM\iTunesCOMInterface.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")


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


class ATL_NO_VTABLE TunesChecker :
	public CComObjectRoot,
	public IDispatch
{
public:


	BOOL bKeepOff;
	DWORD dwCookie;

	UINT broadcastMessage;
	BEGIN_COM_MAP (TunesChecker)
		COM_INTERFACE_ENTRY_IID(IID_IDispatch, TunesChecker)
		COM_INTERFACE_ENTRY_IID(DIID__IiTunesEvents, TunesChecker)
	END_COM_MAP()

	// manual IDispatch
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo){return E_NOTIMPL;}
        HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT , LCID , ITypeInfo **){return E_NOTIMPL;}
        HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID , LPOLESTR *, UINT , LCID , DISPID *){return E_NOTIMPL;}
//		HRESULT STDMETHODCALLTYPE Invoke(...)// see below


	ActiveWinamp::IApplication* winampApp;
	CComPtr<IiTunes> app;
	BOOL winAmpStatus;
	int checkCounter;

	std::wstring albumartFilename;
	WCHAR noArtFileName[MAX_PATH];
	std::wstring songName;

	std::wstring tmpFileName;

	HWND timerHwnd;

	HWND checkerWindow;

	CComDispatchDriver wmpChecker;
	CComQIPtr<IWMPCore, &__uuidof(IWMPCore)> wmp;

	void setupWMPChecker()
	{
		CLSID clsid;
		CLSIDFromString(L"{2667A030-07F5-45F3-B064-8ACA073B1F94}",&clsid);
		CoCreateInstance(clsid,NULL,CLSCTX_ALL,IID_IDispatch,(LPVOID*)&wmpChecker);
	}

	void bindWMP()
	{
		if(wmpChecker)
		{
			CComVariant var;
			if(SUCCEEDED(wmpChecker.GetPropertyByName(L"Core",&var)))
				wmp = var.pdispVal;
		}
	}

	void unbindWMP()
	{
		if(wmp)
			wmp.Release();
	}


	void bindWinAmp()
	{
		HRESULT hr = CoCreateInstance(__uuidof(ActiveWinamp::Application), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ActiveWinamp::IApplication), (LPVOID*)&winampApp);
		winAmpStatus = (winampApp != NULL);
	}

	void unbindWinAmp()
	{
		if(winAmpStatus)
		{
			winAmpStatus = FALSE;
			if(winampApp != NULL)
				winampApp->Release();

			winampApp = NULL;
		}
	}

	void CheckWMP()
	{
		if(wmpChecker != NULL)
		{
			CComVariant var;
			wmpChecker.GetPropertyByName(L"Running",&var);
			BOOL wmpRunning = var.boolVal;
			if(wmpRunning && !isConnectedToApp())
			{
				bindWMP();
			}
			else if(!wmpRunning && wmp)
			{
				unbindWMP();
			}

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

	bool isConnectedToApp()
	{
		return app.p != NULL || winAmpStatus || wmp.p != NULL;
	}
	
	static void RegClass(HINSTANCE dll)
	{
		WNDCLASS cls = {0};
		cls.hInstance = dll;
		cls.lpfnWndProc = &TunesChecker::winproc;
		cls.lpszClassName = "AveTunesNotifyWnd";

		RegisterClass(&cls);
	}

	void DoCreate(HWND hwnd, HINSTANCE hInstance)
	{
		broadcastMessage = ::RegisterWindowMessageW(L"AveTunesWinampNotification_Close");

		setupWMPChecker();

		winampApp = NULL;
		winAmpStatus = FALSE;

		timerHwnd = hwnd;

		checkCounter = 0;
		bKeepOff = FALSE;

		CheckAudioApps();

		SetTimer(timerHwnd, 3000, 1000, NULL);
		//SetTimer(timerHwnd, 4000, 1000, NULL);

		RegClass(hInstance);
		checkerWindow = ::CreateWindowExA(0,"AveTunesNotifyWnd","",0,0,0,0,0, HWND_MESSAGE,0, hInstance, (void*)this);
		::SetPropW(checkerWindow, L"ptr", (HANDLE)(void*)this);
	}

	void TearDown()
	{
		::DestroyWindow(checkerWindow);
	}

	static LRESULT CALLBACK winproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		if(GetPropW(hwnd, L"ptr") != 0)
		{
			TunesChecker* tc = (TunesChecker*)GetPropW(hwnd, L"ptr");
			if(msg == tc->broadcastMessage)
			{
				tc->unbindWinAmp();
			}
		}

		return ::DefWindowProc(hwnd, msg, w, l);
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
	}


	void Update()
	{
		if(isConnectedToApp())
		{
			::SendMessage(timerHwnd, WM_APP_REDRAW, 0, 0);
		}
	}

	void bindTunes(bool fade=true)
	{
		app.CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
		AtlAdvise(app, this, DIID__IiTunesEvents, &dwCookie );
		::SendMessage(timerHwnd, WM_APP_STATUS_CHANGE, 0, 1L);
	}

	void unbindTunes(bool fade=true)
	{
		if(app.p != NULL)
		{
			albumartFilename = L"";
			songName = L"";

			bKeepOff = TRUE;
			AtlUnadvise(app, DIID__IiTunesEvents, dwCookie);
			app.Release();

			::SendMessage(timerHwnd, WM_APP_STATUS_CHANGE, 0, 0L);
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
			CheckWMP();
		}
		else
		{
			checkCounter--;
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


static BOOL ExtractArtworkFromMP3(const WCHAR* mp3, const WCHAR* outputPath)
{
	HANDLE h = ::CreateFileW(mp3, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == h)
		return FALSE;

	char header[10] = {0};
	DWORD numRead(0);
	::ReadFile(h, header, 10, &numRead, NULL);

	if(header[0] != 'I' || header[1] != 'D' || header[2] != '3')
	{
		::CloseHandle(h);
		return FALSE;
	}

	int majorVer = header[3];
	int minorVer = header[4];

	DWORD tagLength = long((signed char)header[6])* 255 * 255 * 255 + long((signed char)header[7]) * 255 * 255  + 
						long((signed char)header[8]) * 255 + long((signed char)header[9]);
	
	BYTE* buf = new BYTE[tagLength];
	::ZeroMemory(buf, tagLength);
	::ReadFile(h, buf, tagLength, &numRead, NULL);

	BOOL bRes = FALSE;

	BYTE* ptr = buf;
	bool goOn(true);
	DWORD numUsed = 0;
	while(goOn && numUsed < tagLength)
	{
		DWORD size = long((unsigned char)ptr[4])* 255 * 255 * 255 + long((unsigned char)ptr[5]) * 255 * 255  + 
						long((unsigned char)ptr[6]) * 255 + long((unsigned char)ptr[7]);
		//memcpy(&size, ptr+4, 4);

		if(ptr[0] == 'A' && ptr[1] == 'P' && ptr[2] == 'I' && ptr[3] == 'C')
		{
			ptr += 10;

			DWORD num(0);

			// got albumart
			BYTE encoding = *ptr++;
			num++;
			// skip mime type
			while(*ptr++ != 0)num++;
			ptr++;
			num++;

			BYTE picType = *ptr++;
			num++;

			/*
			// skip description
			while(*ptr++ != 0)
				num++;
			ptr++;
			num++;*/


			HANDLE h2 = ::CreateFileW(outputPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(INVALID_HANDLE_VALUE != h2)
			{
				DWORD numWritten(0);
				::WriteFile(h2, ptr, size-num, &numWritten, NULL);
				::CloseHandle(h2);
			}

			goOn = false;

			bRes = TRUE;

		}

		numUsed+= 10;
		numUsed+= size;

		ptr += 10;
		ptr += size;
	}


	delete[] buf;


	
	::CloseHandle(h);

	return bRes;
}

static std::wstring BStrToWString(BSTR bstr)
{
	if(NULL == bstr)
		return L"";
	else
		return static_cast<WCHAR*>(bstr);
}

class MusicApp
{
public:
	TunesChecker* tc;

	MusicApp() : tc (NULL)
	{

	}

	BOOL AddFile(const WCHAR* wPath, CComPtr<IITOperationStatus>& status)
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComBSTR bstrFile(wPath);
			CComPtr<IITLibraryPlaylist> library;
			{
				app->get_LibraryPlaylist(&library);	
				if(library)
				{
					library->AddFile(bstrFile,&status);
					return TRUE;
				}
			}
		}
		else if(tc->winampApp)
		{
			CComBSTR bstrPath(wPath);
			ActiveWinamp::IMediaItemPtr track = tc->winampApp->LoadItem(&bstrPath);
			if(track)
			{
				track->Enqueue();
					ActiveWinamp::IPlaylistPtr plp;
					plp = tc->winampApp->Playlist;
					if(plp)
					{
						//long pos(0);
						//track->get_Position(&pos);
						//plp->put_Position(pos);
					}
			}
		}
		else if(tc->wmp)
		{
			CComBSTR bstrPath(wPath);
			tc->wmp->put_URL(bstrPath);
			CComPtr<IWMPControls> controls;
			tc->wmp->get_controls(&controls);
			if(controls)
				controls->play();
		}

		return FALSE;
	}

	bool IsLoaded()
	{
		return tc->isConnectedToApp();
	}

	void Init(TunesChecker* pTc)
	{
		tc = pTc;
	}	

	std::wstring GetExtraInfo(const char* infoType)
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComBSTR trackName;
			CComPtr<IITTrack> track;
			app->get_CurrentTrack(&track);
			if(!track)return L"";
			
				BSTR bstr = {0};

			if(stricmp(infoType,"genre")==0)
				track->get_Genre(&bstr);
			else if(stricmp(infoType,"name")==0)
				track->get_Name(&bstr);
			else if(stricmp(infoType,"album")==0)
				track->get_Album(&bstr);
			else if(stricmp(infoType,"artist")==0)
				track->get_Artist(&bstr);
			else if(stricmp(infoType,"comment")==0)
				track->get_Comment(&bstr);
			else if(stricmp(infoType,"composer")==0)
				track->get_Composer(&bstr);
			else if(stricmp(infoType,"kind")==0)
				track->get_KindAsString(&bstr);
			else if(stricmp(infoType,"length")==0)
				track->get_Time(&bstr);
			else if(stricmp(infoType, "year") == 0)
			{
				long year(0);
				if(SUCCEEDED(track->get_Year(&year)))
				{
					std::wstringstream ws;
					ws << year;
					bstr = SysAllocString(ws.str().c_str());
				}
			}

			if(!bstr)
				return L"";

			std::wstring ret( static_cast<WCHAR*>(bstr));
			SysFreeString(bstr);
			bstr = NULL;

			return ret;
		}
		else if(tc->winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr track;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return L"";
			long pos(0);
			plp->get_Position(&pos);
			track = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(track != NULL)
			{
				CComBSTR bstr;
			
				if(stricmp(infoType,"genre")==0)
					track->get_Genre(&bstr);
				else if(stricmp(infoType,"name")==0)
					track->get_Title(&bstr);
				else if(stricmp(infoType,"album")==0)
					track->get_Album(&bstr);
				else if(stricmp(infoType,"artist")==0)
					track->get_Artist(&bstr);
			//	else if(stricmp(infoType,"comment")==0)
			//		track->get_Comment(&bstr);
				//else if(stricmp(infoType,"composer")==0)
				//	track->get_Composer(&bstr);
				else if(stricmp(infoType,"kind")==0)
					track->get_Genre(&bstr);
				else if(stricmp(infoType,"length")==0)
				{
					long time(0);
					track->get_Length(&time);
					bstr = CComBSTR(FormatTime(time).c_str());
				}
			/*	else if(stricmp(infoType, "year") == 0)
				{
					long year(0);
					if(SUCCEEDED(track->get_Year(&year)))
					{
						std::wstringstream ws;
						ws << year;
						bstr = CComBSTR(ws.str().c_str());
					}
				}	
				*/

				return BStrToWString(bstr);
			}
		}
		else if(tc->wmp != NULL)
		{
			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			if(media != NULL)
			{
				CComBSTR bstr;
			
				if(stricmp(infoType,"genre")==0)
					media->getItemInfo(CComBSTR(L"WM/Genre"), &bstr);
				else if(stricmp(infoType,"name")==0)
					media->get_name(&bstr);
				else if(stricmp(infoType,"album")==0)
					media->getItemInfo(CComBSTR(L"WM/AlbumTitle"), &bstr);
				else if(stricmp(infoType,"artist")==0)
					media->getItemInfo(CComBSTR(L"WM/AlbumArtist"), &bstr);
				else if(stricmp(infoType,"comment")==0)
					media->getItemInfo(CComBSTR(L"WM/Description"), &bstr);
				else if(stricmp(infoType,"composer")==0)
					media->getItemInfo(CComBSTR(L"WM/Composer"), &bstr);
				else if(stricmp(infoType,"kind")==0)
					media->getItemInfo(CComBSTR(L"WM/Genre"), &bstr);
				else if(stricmp(infoType, "year") == 0)
					media->getItemInfo(CComBSTR(L"WM/Year"), &bstr);
				else if(stricmp(infoType,"length")==0)
				{
					double duration(0.0);
					media->get_duration(&duration);
					bstr = CComBSTR(FormatTime((long)duration).c_str());
				}

				return BStrToWString(bstr);
			}
		}

		return L"";
	}

	bool IsPaused()
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			ITPlayerState  st;
			app->get_PlayerState(&st);
			bool isPaused = st != ITPlayerStatePlaying;

			return isPaused;
		}
		else if(tc->winampApp != NULL)
		{
			long state(0);
			tc->winampApp->get_PlayState(&state);
			return state != 1;
		}
		else if(tc->wmp != NULL)
		{
			WMPPlayState state;
			tc->wmp->get_playState(&state);
			return wmppsPaused == state;
		}

		return FALSE;
	}

	long GetSoundVolume()
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			long sound(0);
			app->get_SoundVolume(&sound);	

			return sound;
		}
		else if(tc->winampApp != NULL)
		{
			BYTE volume(0);
			tc->winampApp->get_Volume(&volume);
			return long( float(volume) / 2.55f);
		}
		else if(tc->wmp)
		{
			CComPtr<IWMPSettings> settings;
			tc->wmp->get_settings(&settings);
			if(settings)
			{
				long volume(0);
				settings->get_volume(&volume);
				return volume;
			}
		}

		return 0;
	}

	long GetTrackRating()
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComPtr<IITTrack>track;
			app->get_CurrentTrack(&track);
			if(track)
			{
				long rating(0);
				track->get_Rating(&rating);
				return rating;
			}
		}
		else if(tc->winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return 0;
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(mip != NULL)
			{
				BYTE rating(0);
				mip->get_Rating(&rating);
				if(rating > 5)rating = 0;
				return long(rating)*20;
			}
		}
		else if(tc->wmp != NULL)
		{
			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			if(media != NULL)
			{
				CComBSTR rating;
				media->getItemInfo(CComBSTR(L"UserRating"), &rating);
				std::wstring ws = BStrToWString(rating);
				std::wstringstream ss;
				ss << ws;
				long lRating(0);
				ss >> lRating;
				return lRating+1;
			}
		}

		return 0;
	}

	std::wstring GetTrackName()
	{
		return GetTrackName2(NULL);
	}

	std::wstring GetTrackPath()
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComPtr<IITTrack>track;
			app->get_CurrentTrack(&track);

			if(track)
			{
				CComPtr<IITFileOrCDTrack> fileTrack;
				track.QueryInterface(&fileTrack);
				if(fileTrack)
				{
					CComBSTR bstrPath;
					if(SUCCEEDED(fileTrack->get_Location(&bstrPath)))
						return bstrPath;
				}
			}
		}
		else if(tc->winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return L"";
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(mip != NULL)
			{
				CComBSTR fileName;
				mip->get_Filename(&fileName);
				return BStrToWString(fileName);
			}
		}
		else if(tc->wmp != NULL)
		{
			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			if(media != NULL)
			{
				CComBSTR bstr;
				media->get_sourceURL(&bstr);
				return BStrToWString(bstr);
			}
		}

		return L"";
	}

	BOOL ExtractTrackArtwork(const WCHAR* path)
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComPtr<IITTrack>track;
			app->get_CurrentTrack(&track);

			if(track)
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
							CComBSTR bstrPath(path);
							if(SUCCEEDED(artwork->SaveArtworkToFile(bstrPath)))
								return TRUE;
						}
					}
				}
			}
		}
		else if(tc->winampApp)
		{
			std::wstring fp(GetTrackPath());
			if(fp.length() > 0)
				return ExtractArtworkFromMP3(fp.c_str(), path);

		}
		else if(tc->wmp)
		{
			std::wstring fp(GetTrackPath());
			if(fp.length() > 0)
				return ExtractArtworkFromMP3(fp.c_str(), path);

		}

		return FALSE;
	}

	long GetPlayerPosition()
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			long curPos(0);
			app->get_PlayerPosition(&curPos);
			return curPos;
		}
		else if(tc->winampApp != NULL)
		{
			long pos(0);
			tc->winampApp->get_Position(&pos);
			return pos / 1000;
		}
		else if(tc->wmp)
		{

			CComPtr<IWMPControls> controls;
			tc->wmp->get_controls(&controls);
			if(controls)
			{
				double pos(0.0);
				controls->get_currentPosition(&pos);
				return (long)pos;
			}
		}

		return 0;
	}

	BOOL HasTrack()
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComPtr<IITTrack> track;
			app->get_CurrentTrack(&track);
			return track.p != NULL;
		}
		else if(tc->winampApp!= NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return FALSE;
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			return mip != NULL;
		}
		else if(tc->wmp)
		{
			
			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			return media.p != NULL;
		}

		return FALSE;
	}

	long GetTrackDuration()
	{
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComPtr<IITTrack> track;
			app->get_CurrentTrack(&track);
			if(track)
			{
				long length(0);
				track->get_Duration(&length);
				return length;
			}
		}
		else if(tc->winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return 0;
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(mip != NULL)
			{
				long length(0);
				mip->get_Length(&length);
				return length;
			}
		}
		else if(tc->wmp)
		{

			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			if(media)
			{
				double duration(0.0);
				media->get_duration(&duration);
				return (long)duration;
			}
		}

		return 0;
	}

	BOOL StartPlaylist(long index)
	{
		if(tc->app)
		{
			
			long counter(0);
			CComPtr<IITSourceCollection> sources;
			tc->app->get_Sources(&sources);
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
						CComPtr<IITPlaylistCollection> playlists;
						source->get_Playlists(&playlists);
						if(playlists)
						{
							long plcount(0);
							playlists->get_Count(&plcount);
							for(long pli=1; pli <= plcount; ++pli)
							{
								CComPtr<IITPlaylist> playlist;
								playlists->get_Item(pli,&playlist);
								if(playlist)
								{
									if(index == counter)
									{

										playlist->PlayFirstTrack();
										return TRUE;
									}
									++counter;
								}
							}
						}
					}
				}
			}
		}
		else if(tc->wmp)
		{
			CComPtr<IWMPPlaylistCollection> col;
			tc->wmp->get_playlistCollection(&col);
			if(col)
			{
				CComPtr<IWMPPlaylistArray> arr;
				col->getAll(&arr);
				if(arr)
				{
					long count(0);
					arr->get_count(&count);
					CComPtr<IWMPPlaylist> list;
					arr->item((long)index, &list);
					if(list)
					{
						tc->wmp->put_currentPlaylist(list);
						CComPtr<IWMPControls> controls;
						tc->wmp->get_controls(&controls);
						if(controls)
							controls->play();
					}
	
				}
			}
		}
		
		return FALSE;
	}

	std::vector<std::wstring> GetPlaylists()
	{
		std::vector<std::wstring> pls;

		if(tc->app)
		{
			CComPtr<IITSourceCollection> sources;
			tc->app->get_Sources(&sources);
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
						BSTR name;
						source->get_Name(&name);
						CComPtr<IITPlaylistCollection> playlists;
						source->get_Playlists(&playlists);
						if(playlists)
						{
							long plcount(0);
							playlists->get_Count(&plcount);
							for(long pli=1; pli <= plcount; ++pli)
							{
								CComPtr<IITPlaylist> playlist;
								playlists->get_Item(pli,&playlist);
								if(playlist)
								{
									BSTR plname;
									playlist->get_Name(&plname);
									//AppendMenuW(plmenu,MF_STRING,v_playlists.size()+1000,plname);
									pls.push_back(plname);
									SysFreeString(plname);
									
								}
							}
						}
						//AppendMenuW(menu,MF_POPUP ,(UINT)plmenu,name);

						SysFreeString(name);
					}
				}
			}
		}
		else if(tc->wmp)
		{
			CComPtr<IWMPPlaylistCollection> col;
			tc->wmp->get_playlistCollection(&col);
			if(col)
			{
				CComPtr<IWMPPlaylistArray> arr;
				col->getAll(&arr);
				if(arr)
				{
					long count(0);
					arr->get_count(&count);
					for(int i=0; i<count; ++i)
					{
						CComPtr<IWMPPlaylist> list;
						arr->item((long)i, &list);
						if(list)
						{
							CComBSTR name;
							list->get_name(&name);
							pls.push_back(BStrToWString(name));
						}
					}
				}
			}
		}

		return pls;
	}

	std::wstring GetTrackArtist(BOOL* ok)
	{
	
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComPtr<IITTrack> track;
			app->get_CurrentTrack(&track);
			if(track)
			{
				CComBSTR artist;
				HRESULT hr = S_OK;
				hr = track->get_Artist(&artist);

				if(ok)
					*ok = SUCCEEDED(hr);

				return artist ? artist : L"";
			}
		}
		else if(tc->winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return L"";
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(mip != NULL)
			{
				CComBSTR artist;
				HRESULT hr = mip->get_Artist(&artist);
				if(ok)
					*ok = SUCCEEDED(hr);
				return BStrToWString(artist);
			}
		}
		else if(tc->wmp != NULL)
		{
			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			if(media != NULL)
			{
				CComBSTR bstr;
				HRESULT hr = media->getItemInfo(CComBSTR(L"WM/AlbumArtist"), &bstr);
				if(ok)
					*ok = SUCCEEDED(hr);

				return BStrToWString(bstr);
			}
		}

		if(ok)
			*ok = FALSE;

		return L"";
	}

	std::wstring GetTrackName2(BOOL* ok)
	{
	
		CComPtr<IiTunes>& app = tc->app;
		if(app)
		{
			CComPtr<IITTrack> track;
			app->get_CurrentTrack(&track);
			if(track)
			{
				CComBSTR artist;
				HRESULT hr = S_OK;
				hr = track->get_Name(&artist);

				if(ok)
					*ok = SUCCEEDED(hr);

				return artist;
			}
		}
		else if(tc->winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return L"";
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(mip != NULL)
			{
				CComBSTR title;
				HRESULT hr = mip->get_Title(&title);

				if(ok)
					*ok = SUCCEEDED(hr);

				return BStrToWString(title);
			}
		}
		else if(tc->wmp != NULL)
		{
			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			if(media != NULL)
			{
				CComBSTR bstr;
				HRESULT hr = media->get_name(&bstr);
				if(ok)
					*ok = SUCCEEDED(hr);

				return BStrToWString(bstr);
			}
		}

		if(ok)
			*ok = FALSE;

		return L"";
	}

	BOOL HasWindow()
	{
		// TODO: implement
		return TRUE;
	}

	BOOL HideWindow()
	{
		// TODO: implement
		return TRUE;
	}

	BOOL ShowWindow()
	{
		// TODO: implement
		return TRUE;
	}

	BOOL IsWindowVisible()
	{
		// TODO: implement
		return TRUE;
	}

	void Stop()
	{
		if(tc->app)
			tc->app->Stop();
		else if(tc->wmp)
		{

			CComPtr<IWMPControls> controls;
			tc->wmp->get_controls(&controls);
			if(controls)
			{
				controls->stop();
			}
		}
	}

	void UpdateIPod()
	{
		if(tc->app)
			tc->app->UpdateIPod();
	}

	BOOL StartApp(int pref)
	{
		if(0 == pref) // iTunes
		{
			tc->app.CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
			BOOL bRes = tc->app.p ? TRUE : FALSE;
			tc->app = NULL;
			return bRes;
		}
		else if(1 == pref) // winamp
		{

		}
		else if(2 == pref)
		{
			if(tc->wmpChecker)
			{
				CComVariant var(VARIANT_TRUE);
				tc->wmpChecker.PutPropertyByName(CComBSTR(L"Running"), &var);
			}
		}

		return FALSE;
	}

	BOOL AddArtWorkToTrack(const WCHAR* path)
	{
		if(tc->app)
		{
			CComPtr<IITTrack> track;
			tc->app->get_CurrentTrack(&track);
			if(track)
			{
				CComBSTR file(path);

				CComPtr<IITArtwork> artwork;
				track->AddArtworkFromFile(file,&artwork);
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL GetIsMuted()
	{
		if(tc->app)
		{
			VARIANT_BOOL isMuted = { VARIANT_FALSE };
			tc->app->get_Mute(&isMuted);

			return isMuted != VARIANT_FALSE;
		}
		else if(tc->winampApp != NULL)
		{
			return GetSoundVolume() == 0;
		}
		else if(tc->wmp)
		{
			CComPtr<IWMPSettings> settings;
			tc->wmp->get_settings(&settings);
			if(settings)
			{
				VARIANT_BOOL muted = VARIANT_FALSE;
				settings->get_mute(&muted);
				return muted != VARIANT_FALSE;
			}
		}

		return FALSE;
	}

	void SetIsMuted(BOOL val)
	{
		if(tc->app)
		{
			VARIANT_BOOL isMuted = {VARIANT_FALSE};
			if(val)isMuted = VARIANT_TRUE;

			tc->app->put_Mute(isMuted);
		}
		else if(tc->winampApp != NULL)
		{
			SetSoundVolume(val ? 0 : 100);
		}
		else if(tc->wmp)
		{
			CComPtr<IWMPSettings> settings;
			tc->wmp->get_settings(&settings);
			if(settings)
			{
				VARIANT_BOOL muted = !val ? VARIANT_FALSE : VARIANT_TRUE;
				settings->put_mute(muted);
			}
		}

	}

	void Quit()
	{
		if(tc->app)
			tc->app->Quit();
		else if(tc->winampApp)
		{
			long res(0);
			tc->winampApp->PostMsg(WM_QUIT, 0, 0);
		}
		else if(tc->wmp)
		{
			if(tc->wmpChecker)
			{
				CComVariant var(VARIANT_FALSE);
				tc->wmpChecker.PutPropertyByName(CComBSTR(L"Running"), &var);
			}
		}
	}

	void PlayPause()
	{
		if(tc->app)
			tc->app->PlayPause();
		else if(tc->winampApp)
		{
			if(IsPaused())
				tc->winampApp->Play();
			else
				tc->winampApp->Pause();
		}
		else if(tc->wmp)
		{

			CComPtr<IWMPControls> controls;
			tc->wmp->get_controls(&controls);
			if(controls)
			{
				if(IsPaused())
					controls->play();
				else
					controls->pause();
			}
		}
	}

	void Next()
	{
		if(tc->app)
			tc->app->NextTrack();
		else if(tc->winampApp)
			tc->winampApp->Skip();
		else if(tc->wmp)
		{

			CComPtr<IWMPControls> controls;
			tc->wmp->get_controls(&controls);
			if(controls)
			{
				controls->next();
			}
		}
	}

	void Prev()
	{
		if(tc->app)
			tc->app->BackTrack();
		else if(tc->winampApp)
			tc->winampApp->Previous();
		else if(tc->wmp)
		{

			CComPtr<IWMPControls> controls;
			tc->wmp->get_controls(&controls);
			if(controls)
			{
				controls->previous();
			}
		}
	}

	void SetRating(long rating)
	{
		if(tc->app)
		{
			CComPtr<IITTrack> track;
			tc->app->get_CurrentTrack(&track);
			if(track)
				track->put_Rating(rating);
		}
		else if(tc->winampApp != NULL)
		{
			USES_CONVERSION;
			ActiveWinamp::IPlaylistPtr plp;
			ActiveWinamp::IMediaItemPtr mip;
			plp = tc->winampApp->Playlist;  //Cant do pWaApp->Playlist cause Playlist is a class name & a property
			if(NULL == plp)
				return;
			long pos(0);
			plp->get_Position(&pos);
			mip = (ActiveWinamp::IMediaItemPtr)plp->Item[pos];
			if(mip != NULL)
			{
				mip->put_Rating(rating/20);
			}
		}
			else if(tc->wmp != NULL)
		{
			CComPtr<IWMPMedia> media;
			tc->wmp->get_currentMedia(&media);
			if(media != NULL)
			{
				std::wstring wsRating;
				std::wstringstream ws;
				ws << rating;
				ws >> wsRating;
				CComBSTR rating(wsRating.c_str());
				media->setItemInfo(CComBSTR(L"UserRating"), rating);
			}
		}
	}

	void SetSoundVolume(long sound)
	{
		if(tc->app)
		{
			tc->app->put_SoundVolume(sound);
		}
		else if(tc->winampApp)
			tc->winampApp->put_Volume(long(sound * 255 / 100));
		else if(tc->wmp)
		{
			CComPtr<IWMPSettings> settings;
			tc->wmp->get_settings(&settings);
			if(settings)
			{
				settings->put_volume(sound);
			}
		}
	}

	#define IPC_JUMPTOTIME 106
	#define WM_WA_IPC 0x400

	void SetPlayerPosition(long position)
	{
		if(tc->app)
			tc->app->put_PlayerPosition(position);
		else if(tc->winampApp)
			tc->winampApp->SendMsg( WM_WA_IPC, position*1000, IPC_JUMPTOTIME);
			//tc->winampApp->put_Position(position * 1000);
		else if(tc->wmp)
		{

			CComPtr<IWMPControls> controls;
			tc->wmp->get_controls(&controls);
			if(controls)
			{
				double pos(position);
				controls->put_currentPosition(pos);
			}
		}
	}
};


void GetFileNameW(const WCHAR* filepath, WCHAR* buffer, int buffersize)
{
	// pre:  filepath and buffer are not 0, buffersize is the maximum size of chars copied
	// post: the filename in filepath is stored in buffer, this means that from filepath
	//			c:\blabla\yada.png buffer is filled with yada.png

	if(!filepath || !buffer)return;

	int i=wcslen(filepath)-1;
	while( i >= 0 && buffersize-- >0 && filepath[i]!='\\' && filepath[i]!='/')i--;
	wcscpy(buffer,filepath+i+1);
}

bool FileExistsW(const WCHAR* file)
{
	// pre:  file is not 0 and file should be an absolute path
	// post: true has been returned if the file named by file does exist, otherwise false
	//			has been returned

	if(!file)return false;

	// get the filename of the file and store it in tmp
	WCHAR tmp[MAX_PATH] = {0};
	GetFileNameW(file,tmp,sizeof(tmp));

	// get WIN32_FIND_DATA for the file pointed to file
	WIN32_FIND_DATAW d;
	ZeroMemory(&d,sizeof(d));
	HANDLE h=FindFirstFileW(file,&d);

	// don't forget to close the handle, otherwise we have a mem-leak
	FindClose(h);

	// if the filename retrieved from file and the filename retrieved
	// by getting the WIN32_FIND_DATA are the same when compared (not case-sensitive)
	// then this file DOES exist, otherwise it DOESN'T exist
	return wcsicmp(tmp,d.cFileName)==0;
}

void MakeFolderPathOfFileNameW(std::wstring& path)
{
	// pre:  h is not 0 and path is not 0, nSize determines the maximum number of
	//		 characters path can hold
	// post: the path (without the filename) where module h is located is stored
	//		  in path

	for(int i=path.size()-1;i>0;--i)
	{
		if(path[i]=='\\' || path[i]=='/')
		{
			path = path.substr(0,i+1);
			//path[i+1] = '\0';
			break;
		}
	}
}

Rect GetPrivateProfileRect(const char* section, const char* ini)
{
	return Rect(	GetPrivateProfileInt(section,"Left",0,ini),
					GetPrivateProfileInt(section,"Top",0,ini),
					GetPrivateProfileInt(section,"Width",0,ini),
					GetPrivateProfileInt(section,"Height",0,ini)
				);
}

class ShadowInfo
{
	private:
		BOOL	doShadow;
		BOOL	isLazyShadow;
		int		xOffset;
		int		yOffset;
		int		extraWidth;
		int		extraHeight;
		Color	c;
		float	blur;

	public:
		ShadowInfo(const char* section, const char* ini)
		{	
			doShadow = GetPrivateProfileInt(section,"Shadow",FALSE,ini);

			isLazyShadow= GetPrivateProfileInt(section,"ShadowIsLazy",FALSE,ini);
			xOffset		= GetPrivateProfileInt(section,"ShadowXOffset",1,ini);
			yOffset		= GetPrivateProfileInt(section,"ShadowYOffset",0,ini);
			extraWidth	= GetPrivateProfileInt(section,"ShadowExtraWidth",0,ini);
			extraHeight	= GetPrivateProfileInt(section,"ShadowExtraHeight",0,ini);

			c = Color(	GetPrivateProfileInt(section,"ShadowColorA",225,ini),
						GetPrivateProfileInt(section,"ShadowColorR",0,ini),
						GetPrivateProfileInt(section,"ShadowColorG",0,ini),
						GetPrivateProfileInt(section,"ShadowColorB",0,ini)
					);

			blur = float( GetPrivateProfileInt(section,"ShadowBlurRatio",10,ini) ) / 10;

		}

		BOOL	GetDoShadow() { return doShadow;};
		BOOL	GetIsLazyShadow() { return isLazyShadow;};
		Rect	TransformRect(Rect& rc)
		{
			Rect r = rc;
			r.X += xOffset;
			r.Y += yOffset;
			r.Width += extraWidth;
			r.Height += extraHeight;

			return r;
		}

		Color	GetColor() { return c;};
		float	GetBlur() { return blur;};
};

class CachedShadow
{
private:
	std::wstring str;
	std::auto_ptr<Bitmap> bmp;

public:
	CachedShadow() :
	  bmp(NULL) 
	{

	}

	CachedShadow(std::wstring str, Bitmap* bmp)
		: str(str),bmp(bmp)
	{

	}

	CachedShadow(CachedShadow& cs)
	{
		str = cs.str;
		bmp = cs.bmp;
		//cs.bmp = NULL;
	}

	~CachedShadow()
	{
		//delete bmp;
	}

	CachedShadow& operator =(CachedShadow& cs)
	{
		str = cs.str;
		bmp = cs.bmp;
		//cs.bmp = NULL;
		return *this;
	}
	
	void Clear()
	{
		str = L"";
		bmp = std::auto_ptr<Bitmap>(NULL);
	}

	std::wstring GetString()const{return str;};
	Bitmap* GetBitmap()const{ return &(*bmp);};
};

class ExtraInfo
{
	private:
		Rect r;
		WCHAR font[MAX_PATH];
		REAL fontSize;
		int fontStyle;
		int fontHAlignment;
		Color color;

		char informationType[100];

		ShadowInfo si;
		CachedShadow cs;

	public:

		Rect GetRect()const{return r;};
		WCHAR* GetFontFace(){return font;};
		REAL  GetFontSize(){return fontSize;};
		int  GetFontStyle(){return fontStyle;};
		int  GetFontHAlignment(){return fontHAlignment;};
		Color GetColor(){return color;};


		ExtraInfo(const char* ini, const char* section) :
			si(section,ini)
		{
			char tmpFont[MAX_PATH]= {0};
			GetPrivateProfileString(section,"Facename","Arial",tmpFont,MAX_PATH,ini);
			MultiByteToWideChar(GetACP(),0,tmpFont,-1,font,MAX_PATH);
			color = Color(	GetPrivateProfileInt(section,"ColorA",255,ini),
							GetPrivateProfileInt(section,"ColorR",0,ini),
							GetPrivateProfileInt(section,"ColorG",0,ini),
							GetPrivateProfileInt(section,"ColorB",0,ini)
						);

			r= GetPrivateProfileRect(section,ini);

			fontSize = (REAL) GetPrivateProfileInt(section,"Size",12,ini);

			fontStyle = GetPrivateProfileInt(section,"Style",0,ini);
			fontHAlignment = GetPrivateProfileInt(section,"HAlignment",1,ini);

			GetPrivateProfileString(section,"Type","",informationType,100,ini);

		}

		std::string GetInformationType()const
		{
			return std::string(informationType);
		}

		void Draw(Graphics& g, MusicApp& ma)//CComPtr<IiTunes>& app)
		{
			//if(!app)return;

			if(!ma.HasTrack())return;

			//CComBSTR trackName;
			//CComPtr<IITTrack> track;
			//app->get_CurrentTrack(&track);
			//if(!track)return;

			Gdiplus::FontFamily  fontFamily(font);
			SolidBrush  sb(color);

			GraphicsPath path(Gdiplus::FillModeAlternate);
			StringFormat format(0, LANG_NEUTRAL);
			format.SetAlignment((StringAlignment)fontHAlignment);

			//BSTR bstr = {0};
			std::wstring str(ma.GetExtraInfo(informationType));
			
			/*
			if(stricmp(informationType,"genre")==0)
				track->get_Genre(&bstr);
			else if(stricmp(informationType,"name")==0)
				track->get_Name(&bstr);
			else if(stricmp(informationType,"album")==0)
				track->get_Album(&bstr);
			else if(stricmp(informationType,"artist")==0)
				track->get_Artist(&bstr);
			else if(stricmp(informationType,"comment")==0)
				track->get_Comment(&bstr);
			else if(stricmp(informationType,"composer")==0)
				track->get_Composer(&bstr);
			else if(stricmp(informationType,"kind")==0)
				track->get_KindAsString(&bstr);
			else if(stricmp(informationType,"length")==0)
				track->get_Time(&bstr);
			else if(stricmp(informationType, "year") == 0)
			{
				long year(0);
				if(SUCCEEDED(track->get_Year(&year)))
				{
					std::wstringstream ws;
					ws << year;
					bstr = SysAllocString(ws.str().c_str());
				}
			}

			if(!bstr)
				return;
			*/

			if(si.GetDoShadow() && si.GetBlur() > 0)
			{
				if(cs.GetString() != str )
				{
					Rect rcDraw = GetRect();

					GraphicsPath path4(FillModeAlternate);
					SolidBrush shadowBrush( si.GetColor() );
					Rect rcShadow = GetRect();
					rcShadow.X= rcShadow.Y = 5;

					Bitmap shadow(rcDraw.Width + 10,rcDraw.Height + 10);
					Graphics g2(&shadow);
					g2.SetSmoothingMode(SmoothingModeAntiAlias);
					g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
					path4.AddString(str.c_str(), -1, &fontFamily, fontStyle, fontSize, rcShadow, &format);
					g2.FillPath(&shadowBrush, &path4);

					Bitmap* blurred = GetBlur(&shadow,si.GetBlur());
					cs = CachedShadow(str,blurred);
				}

				if(cs.GetBitmap())
				{
					Rect r = si.TransformRect(GetRect());
					r.X -= 5;
					r.Y -= 5;
					r.Width += 10;
					r.Height += 10;
					g.DrawImage(cs.GetBitmap(),r.X,r.Y,r.Width,r.Height);
				}
			}

			path.AddString(str.c_str(), -1, &fontFamily, fontStyle, fontSize, r, &format);

			//SysFreeString(bstr);

			g.FillPath(&sb, &path);
		}
};

class ChangeSkinHotSpot
{
private:
	Rect r,r2;
	char skinName[MAX_PATH], oldSkinName[MAX_PATH];
	int x,y;
	int x2,y2;
	bool isOnMouseOver;
	std::vector<std::string>frames;

	int newBgWidth, newBgHeight;

	bool useCurrentSkinSizeForAnim;

	BOOL NoHideDuringAnimation;
	BOOL SoundShowDuringAnimation, ProgressShowDuringAnimation, DisplayShowDuringAnimation, CounterShowDuringAnimation, InfoShowDuringAnimation;
	BOOL AlbumArtShowDuringAnimation, RatingShowDuringAnimation;

	BOOL fadeAlbumArtIn;

	std::vector<int>ShowExtraPanesDuringAnimation;

	Rect newDisplayRect, newAlbumArtRect, newPausePlayRect;

	int numberOfFadeFrames;

	char SoundForAnimation[MAX_PATH];
	char SoundForAnimationMouseOut[MAX_PATH];

public:
	ChangeSkinHotSpot()
	{

	};

	ChangeSkinHotSpot(const char* ini, const char* section)
	{
		NoHideDuringAnimation = GetPrivateProfileInt(section,"NoHideDuringAnimation",FALSE,ini);

		SoundShowDuringAnimation    = GetPrivateProfileInt(section,"SoundShowDuringAnimation",FALSE,ini);
		ProgressShowDuringAnimation = GetPrivateProfileInt(section,"ProgressShowDuringAnimation",FALSE,ini);
		RatingShowDuringAnimation	= GetPrivateProfileInt(section,"RatingShowDuringAnimation",FALSE,ini);
		CounterShowDuringAnimation  = GetPrivateProfileInt(section,"CounterShowDuringAnimation",FALSE,ini);
		InfoShowDuringAnimation     = GetPrivateProfileInt(section,"InfoShowDuringAnimation",FALSE,ini);
		DisplayShowDuringAnimation  = GetPrivateProfileInt(section,"DisplayShowDuringAnimation",FALSE,ini);
		AlbumArtShowDuringAnimation = GetPrivateProfileInt(section,"AlbumArtShowDuringAnimation",FALSE,ini);


		int showCount = GetPrivateProfileInt(section,"ShowExtraPanesDuringAnimationCount",0,ini);
		for(int z=0;z<showCount;++z)
		{
			std::stringstream ss;
			ss << "ShowExtraPanesDuringAnimation" << z+1;

			ShowExtraPanesDuringAnimation.push_back(	GetPrivateProfileInt(section,ss.str().c_str(),FALSE,ini) );
		}

		fadeAlbumArtIn = GetPrivateProfileInt(section,"FadeAlbumArtIn",FALSE,ini);
		numberOfFadeFrames = GetPrivateProfileInt(section,"NumberOfFadeFrames",10,ini);
		if(numberOfFadeFrames<1)
			numberOfFadeFrames=1;

		newBgWidth = GetPrivateProfileInt(section,"NewBGWidth",0,ini);
		newBgHeight = GetPrivateProfileInt(section,"newBgHeight",0,ini);

		r= GetPrivateProfileRect(section,ini);

		r2 = Rect(	GetPrivateProfileInt(section,"NewLeft",0,ini),
					GetPrivateProfileInt(section,"NewTop",0,ini),
					GetPrivateProfileInt(section,"NewWidth",0,ini),
					GetPrivateProfileInt(section,"NewHeight",0,ini)
				);

		newDisplayRect	=	Rect(	GetPrivateProfileInt(section,"newDisplayLeft",0,ini),
									GetPrivateProfileInt(section,"newDisplayTop",0,ini),
									GetPrivateProfileInt(section,"NewDisplayWidth",0,ini),
									GetPrivateProfileInt(section,"NewDisplayHeight",0,ini)
								);
		newAlbumArtRect	=	Rect(	GetPrivateProfileInt(section,"NewAlbumArtLeft",0,ini),
									GetPrivateProfileInt(section,"NewAlbumArtTop",0,ini),
									GetPrivateProfileInt(section,"NewAlbumArtWidth",0,ini),
									GetPrivateProfileInt(section,"NewAlbumArtHeight",0,ini)
								);
		newPausePlayRect=	Rect(	GetPrivateProfileInt(section,"NewPausePlayLeft",0,ini),
									GetPrivateProfileInt(section,"NewPausePlayTop",0,ini),
									GetPrivateProfileInt(section,"NewPausePlayWidth",0,ini),
									GetPrivateProfileInt(section,"NewPausePlayHeight",0,ini)
								);

		x = GetPrivateProfileInt(section,"XOffset",0,ini);
		y = GetPrivateProfileInt(section,"YOffset",0,ini);

		x2 = GetPrivateProfileInt(section,"MouseOutXOffset",0,ini);
		y2 = GetPrivateProfileInt(section,"MouseOutYOffset",0,ini);


		memset(skinName,0,MAX_PATH);
		GetPrivateProfileString(section,"NewSkinName",skinName,skinName,MAX_PATH,ini);

		memset(oldSkinName,0,MAX_PATH);
		GetPrivateProfileString(section,"MouseOutSkinName",oldSkinName,oldSkinName,MAX_PATH,ini);

		char type[100] = {0};
		GetPrivateProfileString(section,"Type","click",type,100,ini);
		isOnMouseOver  = (stricmp(type,"hover")==0) ;

		int framesCount = GetPrivateProfileInt(section,"EasyAnimationFramesCount",0,ini);
		for(int i=0;i<framesCount;++i)
		{
			char tmp[MAX_PATH] = {0};
			std::stringstream ss;
			ss << "EasyAnimationFrame";
			ss << i+1;
			GetPrivateProfileString(section,ss.str().c_str(),tmp,tmp,MAX_PATH,ini);
			frames.push_back(tmp);
		}

		useCurrentSkinSizeForAnim = GetPrivateProfileInt(section,"UseCurrentSkinSizeForAnimation",FALSE,ini)!=FALSE;

		GetPrivateProfileString(section,"SoundForAnimation","",SoundForAnimation,MAX_PATH,ini);
		GetPrivateProfileString(section,"SoundForAnimationMouseOut","",SoundForAnimationMouseOut,MAX_PATH,ini);
	}

	std::vector<std::string>& GetFrames(){return frames;};

	const char* GetSoundForAnimation(){return SoundForAnimation;};
	const char* GetSoundForAnimationMouseOut(){return SoundForAnimationMouseOut;};

	Rect GetRect()const{return r;};
	Rect GetNewRect()const{return r2;};
	const char* GetSkinName()const{return skinName;};
	const char* GetOldSkinName()const{return oldSkinName;};
	bool IsMouseOver()const{return isOnMouseOver;};
	int GetX()const{return x;};
	int GetY()const{return y;};

	int GetHotSpotX()const{return x2;};
	int GetHotSpotY()const{return y2;};

	bool GetUseCurrentSkinSizeForAnim()const{return useCurrentSkinSizeForAnim;};

	BOOL GetNoHideDuringAnimation()const{return NoHideDuringAnimation;};
	BOOL GetSoundShowDuringAnimation()const{return SoundShowDuringAnimation;};
	BOOL GetRatingShowDuringAnimation()const{return RatingShowDuringAnimation;};
	BOOL GetProgressShowDuringAnimation()const{return ProgressShowDuringAnimation;};
	BOOL GetDisplayShowDuringAnimation()const{return DisplayShowDuringAnimation;};
	BOOL GetCounterShowDuringAnimation()const{return CounterShowDuringAnimation;};
	BOOL GetInfoShowDuringAnimation()const{return InfoShowDuringAnimation;};

	BOOL GetAlbumArtShowDuringAnimation()const{return AlbumArtShowDuringAnimation;};

	BOOL GetfadeAlbumArtIn()const{return fadeAlbumArtIn;};

	std::vector<int> GetShowExtraPanesDuringAnimation()const{return ShowExtraPanesDuringAnimation;};

	Rect GetNewDisplayRect()const{return newDisplayRect;};
	Rect GetNewAlbumArtRect()const{return newAlbumArtRect;};
	Rect GetNewPausePlayRect()const{return newPausePlayRect;};

	int GetNewBgWidth()const{return newBgWidth;};
	int GetNewBgHeight()const{return newBgHeight;};

	int GetNumberOfFadeFrames()const{return numberOfFadeFrames;};

};

#define DIR_LEFT_RIGHT 0
#define DIR_RIGHT_LEFT 1
#define DIR_TOP_BOTTOM 2
#define DIR_BOTTOM_TOP 3


// this class holds the data for a skin.
class Skin
{
private:
	char name[MAX_PATH], author[MAX_PATH], infoText[MAX_PATH];
	WCHAR font[MAX_PATH];
	Color textColor;
	REAL fontSize;
	int fontStyle;
	int fontHAlignment;

	WCHAR font2[MAX_PATH];
	Color textColor2;
	REAL fontSize2;
	int fontStyle2;
	int fontHAlignment2;

	ShadowInfo fontShadow, fontShadow2;

	Rect display, progress, info, counter, prev, next, pause, stop, ipod, mute, open, quit,
		toggleiTunesWnd, rating, sound;
	Color innerKnob, outerKnob;

	int ratingDirection;
	int soundDirection;

	UINT width, height, knobWidth, knobHeight;
	BOOL useKnobBmp, stretchKnob;

	Rect albumArt, albumArtWorkRect;
	BOOL showAlbumArt;

	BOOL drawOverlay, drawOverlayActualSize;
	UINT albumArtAlpha;

	std::vector<ExtraInfo*>infos;
	std::vector<ChangeSkinHotSpot*>hotSpots;

	BOOL useSoundKnob;

	bool noCloseButton;
	int closeButtonX;
	int closeButtonY;

public:

	bool GetNoCloseButton()const
	{
		return noCloseButton;
	}

	POINT GetClosePoint()const
	{
		POINT p = {closeButtonX, closeButtonY};
		return p;
	}

	Skin(const char* ini) :
	  fontShadow("Font",ini), fontShadow2("CounterFont",ini)
	{
		albumArtWorkRect = 	Rect(	GetPrivateProfileInt("AlbumArt","ArtWorkLeft",0,ini),
					GetPrivateProfileInt("AlbumArt","ArtWorkTop",0,ini),
					GetPrivateProfileInt("AlbumArt","ArtWorkWidth",0,ini),
					GetPrivateProfileInt("AlbumArt","ArtWorkHeight",0,ini)
				);
		
		GetPrivateProfileString("Info","Name","Unknown",name,MAX_PATH,ini);
		GetPrivateProfileString("Info","Author","Unknown",author,MAX_PATH,ini);
		GetPrivateProfileString("Info","Info","",infoText,MAX_PATH,ini);

		char tmpFont[MAX_PATH]= {0};
		GetPrivateProfileString("Font","Facename","Arial",tmpFont,MAX_PATH,ini);
		MultiByteToWideChar(GetACP(),0,tmpFont,-1,font,MAX_PATH);
		textColor = Color(	GetPrivateProfileInt("Font","ColorA",255,ini),
							GetPrivateProfileInt("Font","ColorR",0,ini),
							GetPrivateProfileInt("Font","ColorG",0,ini),
							GetPrivateProfileInt("Font","ColorB",0,ini)
						);

		fontSize = (REAL) GetPrivateProfileInt("Font","Size",12,ini);

		fontStyle = GetPrivateProfileInt("Font","Style",0,ini);
		fontHAlignment = GetPrivateProfileInt("Font","HAlignment",1,ini);
		//fontShadow = ShadowInfo("Font",ini);

		char tmpFont2[MAX_PATH]= {0};
		GetPrivateProfileString("CounterFont","Facename",tmpFont,tmpFont2,MAX_PATH,ini);
		MultiByteToWideChar(GetACP(),0,tmpFont2,-1,font2,MAX_PATH);
		textColor2 = Color(	GetPrivateProfileInt("CounterFont","ColorA",GetPrivateProfileInt("Font","ColorA",255,ini),ini),
							GetPrivateProfileInt("CounterFont","ColorR",GetPrivateProfileInt("Font","ColorR",255,ini),ini),
							GetPrivateProfileInt("CounterFont","ColorG",GetPrivateProfileInt("Font","ColorG",255,ini),ini),
							GetPrivateProfileInt("CounterFont","ColorB",GetPrivateProfileInt("Font","ColorB",255,ini),ini)
						);

		fontSize2 = (REAL) GetPrivateProfileInt("CounterFont","Size",fontSize,ini);

		fontStyle2 = GetPrivateProfileInt("CounterFont","Style",0,ini);
		fontHAlignment2 = GetPrivateProfileInt("CounterFont","HAlignment",1,ini);
		//fontShadow2 = ShadowInfo("CounterFont",ini);

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
		rating	= GetPrivateProfileRect("RatingPane",ini);
		sound	= GetPrivateProfileRect("SoundPane",ini);

		ratingDirection = GetPrivateProfileInt("RatingPane","Direction",DIR_LEFT_RIGHT,ini);
		soundDirection  = GetPrivateProfileInt("SoundPane ","Direction",DIR_LEFT_RIGHT,ini);
		useSoundKnob	= GetPrivateProfileInt("SoundPane","UseKnob",FALSE,ini);

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

		width = GetPrivateProfileInt("Bg","Width",0,ini);
		height= GetPrivateProfileInt("Bg","Height",0,ini);

		noCloseButton = GetPrivateProfileInt("CloseButton", "NoCloseButton", 0, ini) != 0;
		closeButtonX = GetPrivateProfileInt("CloseButton", "X", 0, ini);
		closeButtonY = GetPrivateProfileInt("CloseButton", "Y", 0, ini);

		knobWidth = GetPrivateProfileInt("Knob","Width",0,ini);
		knobHeight= GetPrivateProfileInt("Knob","Height",0,ini);

		useKnobBmp = GetPrivateProfileInt("Knob","UseBitmap",FALSE,ini);
		stretchKnob = GetPrivateProfileInt("Knob","StretchKnob",FALSE,ini);

		int infoCount =0;
		infoCount = GetPrivateProfileInt("ExtraInfos","Count",infoCount,ini);
		for(int i=0;i<infoCount;++i)
		{
			std::stringstream ss;
			ss << "ExtraInfo-";
			ss << i+1;
			infos.push_back(new ExtraInfo(ini,ss.str().c_str()) );
		}

		int hsCount =0;
		hsCount = GetPrivateProfileInt("ChangeSkinHotSpots","Count",hsCount,ini);
		for(int ii=0;ii<hsCount;++ii)
		{
			std::stringstream ss;
			ss << "ChangeSkinHotSpot-";
			ss << ii+1;
			ChangeSkinHotSpot* hs = new ChangeSkinHotSpot(ini,ss.str().c_str());
			hotSpots.push_back(hs );
		}

	}

	~Skin()
	{
		for(int i=0;i<infos.size();++i)
		{
			delete infos[i];
			infos[i] = NULL;
		}
		infos.clear();

		for(int ii=0;ii<hotSpots.size();++ii)
		{
			delete hotSpots[ii];
			hotSpots[ii] = NULL;
		}
		hotSpots.clear();

	}

	const char* GetName()const{return name;};
	const char* GetAuthor()const{return author;};
	const char* GetInfo()const{return infoText;};

	std::vector<ExtraInfo*> GetInfos(){return infos;};
	std::vector<ChangeSkinHotSpot*> GetHotSpots(){return hotSpots;};

	WCHAR*		GetFontFace() { return font;};
	Color&	    GetTextColor() { return textColor;};
	REAL	    GetFontSize() { return fontSize;};
	int			GetFontStyle() {return fontStyle;};
	int			GetFontHAlignment() { return fontHAlignment;};
	ShadowInfo&	GetFontShadow() { return fontShadow;};

	WCHAR*		GetCounterFontFace() { return font2;};
	Color&	    GetCounterTextColor() { return textColor2;};
	REAL	    GetCounterFontSize() { return fontSize2;};
	int			GetCounterStyle() {return fontStyle2;};
	int			GetCounterHAlignment() { return fontHAlignment2;};
	ShadowInfo&	GetCounterShadow() { return fontShadow2;};

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

	Rect& GetRatingRect(){return rating;};
	int   GetRatingDirection(){return ratingDirection;};

	Rect& GetSoundRect(){return sound;};
	int   GetSoundDirection(){return soundDirection;};

	BOOL  GetUseSoundKnob()const{return useSoundKnob;};

	Rect&		GetAlbumArtWorkRect(){return albumArtWorkRect;};

};

class iTunesWidget // :  public CComObjectRoot, public _IiTunesEvents
{
private:
/*	BEGIN_COM_MAP(iTunesWidget)
    COM_INTERFACE_ENTRY(_IiTunesEvents)
	END_COM_MAP( )*/

	HWND hwndDesklet;
	HINSTANCE dll;

	BOOL shouldRedraw;

	
	CComObject<TunesChecker>* tc;

	//! iTunes specific
	//CComPtr<IiTunes> app;
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

	bool useFolderForArtwork;

	Skin* skin;

	char skinDir[MAX_PATH];

	CComBSTR songName;

	Bitmap* ratingBmp, *soundBmp, *soundKnobBmp;

	long oldRating;
	long oldSound;

	bool use5StarsRating;

	bool bgErased;

	bool mouseWasOnHotSpot;
	Rect hotSpotRect;
	std::string revertSkin;
	POINT hotSpotOffset;
	bool isHovering;
	std::vector<std::string>hotSpotFrames;

	ChangeSkinHotSpot revertHotSpot;

	bool neverPlaySoundsInSkins;

	bool wasLoaded;

	int wasLoadedCounter;

	CachedShadow infoShadow, counterShadow;

	void SetAlbumArtToLayer(const char* path, UINT lid);
	void SetAlbumArtToLayerW(const WCHAR* wPath, UINT lid);

	WCHAR albumArtTemp[MAX_PATH];

	MusicApp musicApp;

public:
	UINT broadcastMessage;

public:
	iTunesWidget(HWND hwndDesklet, HINSTANCE dll) :
		hwndDesklet( hwndDesklet), dll (dll), wasPaused(true), showTimePlayed(TRUE), counter(0),
		hideWhenNotLoaded(FALSE), autoHideiTunes(FALSE), isScrolling(false), w(0), l(0), scrollBmp(0),
		doScroll(false), scrollBack(false), scrollCounter(0), skin(0), knobBmp(0),ratingBmp(0), oldRating(0),
		use5StarsRating(true), bgErased(true), soundBmp(0), oldSound(-1),mouseWasOnHotSpot(false),
		isHovering(false), soundKnobBmp(0), neverPlaySoundsInSkins(true), wasLoaded(false), wasLoadedCounter(0),
		useFolderForArtwork(true),
		shouldRedraw(TRUE), tc(NULL)
	{
		broadcastMessage = ::RegisterWindowMessageW(L"AveTunesWinampNotification_Close");

		DeskletAsyncSetPointer(hwndDesklet,this);

		DeskletSetLabel(hwndDesklet, "");

		DeskletSetImageSetStatus(hwndDesklet, FALSE);

		WCHAR tempPath[MAX_PATH] = {0};
		WCHAR tmpFname[MAX_PATH] = {0};
		GetTempPathW(MAX_PATH, tempPath);
		GetTempFileNameW(tempPath, L"ave", 0, tmpFname);
		wcscpy(albumArtTemp, tmpFname);
		//tmpFileName = tmpFname;


		strcpy(skinDir,"default");

		CoInitialize(0);

		CComObject<TunesChecker>::CreateInstance(&tc);
		tc->AddRef();
		tc->DoCreate(hwndDesklet, dll);
		musicApp.Init(tc);
	}

	int GetPrefStart()
	{
		return 0;
	}

	MusicApp& GetMusicApp(){return musicApp;};

	void SetUseFolderForArtwork(bool v){useFolderForArtwork = v;};
	bool GetUseFolderForArtwork()const { return useFolderForArtwork;};

	void SetMouseWasOnHotSpot(bool v){mouseWasOnHotSpot=v;};
	bool GetMouseWasOnHotSpot()const{return mouseWasOnHotSpot;};

	Rect GetHotSpotRect()const{return hotSpotRect;};
	void SetHotSpotRect(Rect& r){hotSpotRect=r;};

	std::string GetRevertSkin()const{return revertSkin;};
	void SetRevertSkin(std::string& s){revertSkin=s;};

	void SetHotSpotOffset(POINT& p){hotSpotOffset = p;};
	POINT GetHotSpotOffset()const{return hotSpotOffset;};

	void SetIsHovering(bool v){isHovering=v;};
	bool GetIsHovering()const{return isHovering;};

	void SetHotSpotFrames(std::vector<std::string>& v){hotSpotFrames = v;};
	std::vector<std::string>& GetHotSpotFrames(){return hotSpotFrames;};

	bool GetNeverPlaySoundsInSkins()const{return neverPlaySoundsInSkins;};
	void SetNeverPlaySoundsInSkins(bool b){neverPlaySoundsInSkins=b;};

	~iTunesWidget()
	{
		
		delete ratingBmp;
		ratingBmp = 0;

		delete soundBmp;
		soundBmp = 0;

		delete knobBmp;
		knobBmp = 0;

		delete scrollBmp;
		scrollBmp = 0;

		delete soundKnobBmp;
		soundKnobBmp = 0;

		if(GetAutoHideiTunes())
		{
			//if iTunes was hiddne, make the window visible again if it is running
			HWND iTunesWnd = FindWindow("iTunes",0);
			if(iTunesWnd)
			{
				ShowWindow(iTunesWnd,SW_SHOWNA);
			}
		}
		delete skin;
		skin = 0;

		tc->unbindWinAmp();
		tc->unbindTunes();
		tc->Release();

		tc->TearDown();

		CoUninitialize();
	};

	void ResetSongName()
	{
		songName.Empty();
	}


	void ResetLayers()
	{

			BOOL suc = FALSE;
			UINT remID = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",&suc);
			if(suc)DeskletLayerRemove(GetWindow(),remID,FALSE);
			suc = FALSE;
			remID = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Pause_Button",&suc);
			if(suc)DeskletLayerRemove(GetWindow(),remID,FALSE);
			suc = FALSE;
			remID = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Info",&suc);
			if(suc)DeskletLayerRemove(GetWindow(),remID,FALSE);


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

			BOOL vis = FALSE;
			if(wasPaused)vis = TRUE;
			DeskletLayerSetVisibility(GetWindow(),id,FALSE,vis);
				
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
		
	}

	void CheckAudioApps()
	{
		if(tc != NULL)
			tc->CheckAudioApps();
	}

	void SetSkin(const char* dir, bool noRedraw=false, bool noRemoveLayers=false)
	{
		USES_CONVERSION;

		//Skin* oldSkin = skin;

		strcpy(skinDir,dir);
		//DeskletSetImageFile(GetWindow(),bgPath);
		bgErased = true;

		char ini[MAX_PATH] = {0};
		GetDLLPath(GetDLL(),ini,MAX_PATH);
		PathAppend(ini,"skins\\");
		PathAppend(ini,skinDir);
		PathAppend(ini,"\\skin.ini");
		skin = new Skin(ini);

		SkinInfo skinInfo = {0};
		skinInfo.dwSize = sizeof(SkinInfo);

		WCHAR sn[MAX_PATH] = {0};
		WCHAR sa[MAX_PATH] = {0};
		WCHAR sc[MAX_PATH] = {0};

		GetPrivateProfileStringW(L"Info",L"Name",A2W(dir),sn,MAX_PATH,A2W(ini));
		GetPrivateProfileStringW(L"Info",L"Author",L"Unknown",sa,MAX_PATH,A2W(ini));
		GetPrivateProfileStringW(L"Info",L"Info",L"",sc,MAX_PATH,A2W(ini));
		skinInfo.version = GetPrivateProfileIntW(L"Info",L"Version",270,A2W(ini)); 

		WCHAR wDir[MAX_PATH] = {0};
		wcscpy(wDir,A2W(dir));
		skinInfo.wKey = wDir;
		skinInfo.wName = sn;
		skinInfo.wAuthor = sa;
		skinInfo.wNotes = sc;

		DeskletSetSkinInfoW(GetWindow(),&skinInfo);

		ResetSongName();

		delete ratingBmp;
		ratingBmp = 0;

		delete soundBmp;
		soundBmp = 0;

		delete soundKnobBmp;
		soundKnobBmp = 0;

		delete knobBmp;
		knobBmp = 0;

		if(!noRemoveLayers)
		{
			BOOL suc = FALSE;
			UINT remID = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",&suc);
			if(suc)DeskletLayerRemove(GetWindow(),remID,FALSE);
			suc = FALSE;
			remID = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Pause_Button",&suc);
			if(suc)DeskletLayerRemove(GetWindow(),remID,FALSE);
			suc = FALSE;
			remID = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.Info",&suc);
			if(suc)DeskletLayerRemove(GetWindow(),remID,FALSE);
		}

		if(!noRemoveLayers)
		{
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

			BOOL vis = FALSE;
			if(wasPaused && !noRedraw)vis = TRUE;
			DeskletLayerSetVisibility(GetWindow(),id,FALSE,vis);
				
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
		}

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
		POINT pos = {0};
		DeskletGetPosition(GetWindow(),&pos);
		RECT nr = {0};
		nr.left = pos.x;
		nr.top = pos.y;
		nr.right = nr.left + s.cx;
		nr.bottom = nr.top + s.cy;
		DeskletMove(GetWindow(),&nr,FALSE);
		if(!noRedraw)
			Redraw(true);

		infoShadow.Clear();
		counterShadow.Clear();

		if(skin->GetNoCloseButton())
			DeskletSetClosePoint(GetWindow(), NULL, 0, 0);
		else
			DeskletSetClosePoint(GetWindow(), &skin->GetClosePoint(), skin->GetWidth(), skin->GetHeight());


		//delete oldSkin;
		//DeskletSetSize(GetWindow(),&s);
		//DeskletSetImageFile(GetWindow(),bgPath);
	}

	void SetShouldRedraw(BOOL v){shouldRedraw = v;};
	BOOL GetShouldRedraw()const { return shouldRedraw;};

	HWND GetWindow()const { return hwndDesklet;};
	HINSTANCE GetDLL()const { return dll;};

	void ReadFromIni(const char* ini, const char* section);
	void SaveToIni(const char* ini, const char* section);

	CComPtr<IiTunes>& GetApp(){ return tc->app;};
	CComPtr<IITOperationStatus>& GetStatus() { return status;};

	void Redraw(bool force=false,bool useRevertHotspot=false);

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


		RectF rc(5,5,4010,skin->GetInfoRect().Height+10);

		path.AddString(scrollText.c_str(), -1, &fontFamily, skin->GetFontStyle(), skin->GetFontSize(), rc, &format);

		Rect rcBounds;
		path.GetBounds(&rcBounds);

		delete scrollBmp;
		scrollBmp = 0;
		scrollBmp = new Bitmap(rcBounds.Width+20,skin->GetInfoRect().Height+20);
		w = scrollBmp->GetWidth();

		Graphics g(scrollBmp);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		ShadowInfo& si = skin->GetFontShadow();
		if(si.GetDoShadow() && si.GetBlur() > 0)
		{
			Rect rcDraw = rcBounds;

			Rect r = si.TransformRect(rcDraw);

			GraphicsPath path4(FillModeAlternate);
			SolidBrush shadowBrush( si.GetColor() );
			RectF rcShadow = rc;
			rcShadow.Width += 5;
			rcShadow.Height+= 5;
			rcShadow.X= rcShadow.Y = 5;

			Bitmap shadow(rcDraw.Width + 30,rcDraw.Height + 30);
			Graphics g2(&shadow);
			g2.SetSmoothingMode(SmoothingModeAntiAlias);
			g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			path4.AddString(scrollText.c_str(), -1, &fontFamily, skin->GetFontStyle(), skin->GetFontSize(), rcShadow, &format);
			g2.FillPath(&shadowBrush, &path4);

			Bitmap* blurred = GetBlur(&shadow,si.GetBlur());

			if(blurred)
			{
				Rect r(0,0,blurred->GetWidth(),blurred->GetHeight());
				r = si.TransformRect(r);
				//r.X -= 7;
				//r.Y -= 7;
				//r.Width = 30;
				//r.Height = 30;
				g.DrawImage(blurred,r.X,r.Y,r.Width,r.Height);
			}
			delete blurred;
		}

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

		path.AddString(scrollText.c_str(), -1, &fontFamily, skin->GetFontStyle(), skin->GetFontSize(), rc, &format);

		Rect rcBounds;
		path.GetBounds(&rcBounds);
		return ( skin->GetInfoRect().Width > 0 && rcBounds.Width > skin->GetInfoRect().Width);
	}

	void SetRevertHotSpot(ChangeSkinHotSpot& hs){revertHotSpot=hs;};
	ChangeSkinHotSpot GetRevertHotSpot(){return revertHotSpot;};

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
		// toggles iTunes window

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

	void NotifyWinampClosing()
	{
		tc->unbindWinAmp();
		MessageBoxW(0, L"went down", 0, 0);
	}

	bool NeedsToRedraw()
	{
		try
		{

		if(musicApp.IsLoaded())
		{
			if(!wasLoaded)
			{
				wasLoaded = true;
				return true;
			}

			if(counter % 9 == 0 && skin->GetInfoRect().Width != 0 && skin->GetInfoRect().Height != 0)
				return true;

//			ITPlayerState  st;
//			GetApp()->get_PlayerState(&st);
//			bool isPaused = st != ITPlayerStatePlaying;
			bool isPaused(musicApp.IsPaused());

			if(isPaused == false && skin->GetCounterRect().Width != 0 && skin->GetCounterRect().Height != 0)
				return true;

			if(isPaused == false && skin->GetProgressRect().Width != 0 && skin->GetProgressRect().Height != 0)
				return true;

			long sound(musicApp.GetSoundVolume());
			//tc->app->get_SoundVolume(&sound);

			if(sound != oldSound && skin->GetSoundRect().Width != 0 && skin->GetSoundRect().Height != 0)
				return true;
				
			if(musicApp.HasTrack())
			{
				long rating(musicApp.GetTrackRating());
				if(oldRating != rating && skin->GetRatingRect().Width != 0 && skin->GetRatingRect().Height != 0)
					return true;

				BOOL ok(FALSE);
				std::wstring trackName(musicApp.GetTrackName2(&ok));
				if(ok && !songName)
				{
					if(!skin->GetShowAlbumArt() )
						songName = CComBSTR(trackName.c_str());
					return true;
				}

				if(ok && trackName != std::wstring(static_cast<WCHAR*>(songName)))
				{
					if(!skin->GetShowAlbumArt() )
						songName = CComBSTR(trackName.c_str());
					return true;
				}

			}

			return false;

			/*CComPtr<IITTrack>track;
			GetApp()->get_CurrentTrack(&track);
			if(track)
			{
				long rating(0);
				track->get_Rating(&rating);
				if(oldRating != rating && skin->GetRatingRect().Width != 0 && skin->GetRatingRect().Height != 0)
					return true;


				CComBSTR trackName;
				track->get_Name(&trackName);
				if(trackName && !songName)
				{
					if(!skin->GetShowAlbumArt() )
						songName = trackName;
					return true;
				}

				if(trackName && std::wstring(static_cast<WCHAR*>(trackName)) != std::wstring(static_cast<WCHAR*>(songName)))
				{
					if(!skin->GetShowAlbumArt() )
						songName = trackName;
					return true;
				}
			}

			return false;
			*/
		}
		else
		{
			bool ret = wasLoaded;

			wasLoaded = false;

			return ret;
		}

		}
		catch(...)
		{
			tc->unbindWinAmp();
			return true;
		}
	}

	BOOL CheckLoadedStatus()
	{
		return tc->isConnectedToApp();
		// checks if iTunes is *still* loaded.
		/*
		
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

			wasLoadedCounter = 10;

			return GetApp() ? TRUE : FALSE;
		}
		else
		{
			if(FindWindow("iTunes",0) && wasLoadedCounter == 0 )
			{
				SetAutoHideiTunes(GetAutoHideiTunes());
				app.CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);

				return GetApp() ? TRUE : FALSE;
			}
			else
			{
				wasLoadedCounter--;
				if(wasLoadedCounter<0)
					wasLoadedCounter=0;
				return FindWindow("iTunes",0) ? TRUE : FALSE;
			}
		}*/
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

	bool GetUseFiveStarRating()const{return use5StarsRating;};
	void SetUseFiveStarRating(bool v)
	{
		use5StarsRating=v;
		oldRating=-1;
	};
};


void iTunesWidget::ReadFromIni(const char* ini, const char* section)
{
	showTimePlayed = GetPrivateProfileInt(section,"ShowTimePlayed",showTimePlayed,ini);
	SetHideWhenNotLoaded( GetPrivateProfileInt(section,"HideWhenNotLoaded",GetHideWhenNotLoaded(),ini) );
	SetAutoHideiTunes( GetPrivateProfileInt(section,"AutoHideiTunes",GetAutoHideiTunes(),ini) );
	GetPrivateProfileString(section,"SkinDir",skinDir,skinDir,MAX_PATH,ini);

	SetNeverPlaySoundsInSkins( GetPrivateProfileInt(section,"NeverPlaySoundsInSkin",GetNeverPlaySoundsInSkins(),ini)==TRUE );
	
	SetUseFiveStarRating(GetPrivateProfileInt(section,"Use5Rating",TRUE,ini)==TRUE);

	SetUseFolderForArtwork(GetPrivateProfileInt(section,"UseFolderForArtWork",FALSE,ini)==TRUE);

	SetSkin(skinDir);
}

void iTunesWidget::SaveToIni(const char* ini, const char* section)
{
	WritePrivateProfileInt(section,"ShowTimePlayed",showTimePlayed,ini);
	WritePrivateProfileInt(section,"HideWhenNotLoaded",GetHideWhenNotLoaded(),ini);
	WritePrivateProfileInt(section,"AutoHideiTunes",GetAutoHideiTunes(),ini);
	WritePrivateProfileString(section,"SkinDir",skinDir,ini);
	WritePrivateProfileInt(section,"Use5Rating",GetUseFiveStarRating(),ini);
	WritePrivateProfileInt(section,"UseFolderForArtWork",GetUseFolderForArtwork(),ini);
	WritePrivateProfileInt(section,"NeverPlaySoundsInSkin",GetNeverPlaySoundsInSkins(),ini);
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"AveTunes");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"This desklet is a skinnable iTunes, WinAmp and Windows Media Player remote controller.");
	*iVersion = 500;
}

void iTunesWidget::SetAlbumArtToLayer(const char* path, UINT lid)
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
	{
		Rect& artWorkRect = skin->GetAlbumArtWorkRect();
		if(artWorkRect.Width == 0 || artWorkRect.Height == 0)
 			g.DrawImage(art,Rect(0,0,sr.Width,sr.Height),0,0,art->GetWidth(),art->GetHeight(),UnitPixel,&ia,0,0);
		else
			g.DrawImage(art,artWorkRect,0,0,art->GetWidth(),art->GetHeight(),UnitPixel,&ia,0,0);
	}

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


void iTunesWidget::SetAlbumArtToLayerW(const WCHAR* wPath, UINT lid)
{
	char path[MAX_PATH+1] = {0};
	WideCharToMultiByte(GetACP(),0,wPath,-1,path,MAX_PATH,NULL,NULL);
	SetAlbumArtToLayer(path,lid);
}

BOOL __stdcall OnVisibilityNotify(iTunesWidget* iTunes, BOOL fNewState, DWORD dwFlags)
{
	if(iTunes)
		iTunes->SetShouldRedraw(fNewState);

	return TRUE;
}

void iTunesWidget::Redraw(bool force,bool useRevertHotspot)
{
	try
	{
		BOOL isLoaded =CheckLoadedStatus(); 
		if(!isLoaded)
		{
			if ( GetHideWhenNotLoaded() )
			{
				DeskletFadeOut(GetWindow(), TRUE, -1);
				//DeskletHide(GetWindow() );
			}
		}
		else
		{
			DeskletFadeIn(GetWindow(), TRUE, -1);
			//DeskletShow(GetWindow() );
		}
		
		/*
		ITPlayerState  st;
		if(GetApp())
		{
			GetApp()->get_PlayerState(&st);
		}
		bool isPaused = st != ITPlayerStatePlaying;
		*/
		bool isPaused(musicApp.IsPaused());

		if(wasPaused != isPaused || force)
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

		if(isLoaded && (!wasPaused || IsScrolling()) || force || (((isLoaded)? true : false) !=wasLoaded))
		{
			if(isLoaded && skin->GetShowAlbumArt() )
			{
				BOOL ok(FALSE);
				CComBSTR trackName(musicApp.GetTrackName2(&ok).c_str());
				/*ComPtr<IITTrack> track;
				if(GetApp() ) GetApp()->get_CurrentTrack(&track);
				if(track)
				{
					track->get_Name(&trackName);
				}*/

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
	
				if(!ok || (useRevertHotspot && revertHotSpot.GetAlbumArtShowDuringAnimation() == FALSE) )
				{
					DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);	
				}
				else
				{

					if(!(trackName == songName) )
					{
						std::wstring strArtWorkInFolder;
						bool artWorkInFolderExists = false;

						if(GetUseFolderForArtwork() )
						{
							if(musicApp.HasTrack())
							{
								strArtWorkInFolder = musicApp.GetTrackPath();
								MakeFolderPathOfFileNameW(strArtWorkInFolder);
								strArtWorkInFolder = strArtWorkInFolder + L"folder.jpg";
								artWorkInFolderExists = FileExistsW(strArtWorkInFolder.c_str() );
							}
							/*CComPtr<IITFileOrCDTrack> fileTrack;
							track.QueryInterface(&fileTrack);
							if(fileTrack)
							{
								CComBSTR bstrPath;
	
								if(SUCCEEDED(fileTrack->get_Location(&bstrPath)))
								{
									strArtWorkInFolder = static_cast<WCHAR*>(bstrPath);
									MakeFolderPathOfFileNameW(strArtWorkInFolder);
									strArtWorkInFolder = strArtWorkInFolder + L"folder.jpg";
	
									artWorkInFolderExists = FileExistsW(strArtWorkInFolder.c_str() );
								}
							}*/
						}
						

						if(musicApp.ExtractTrackArtwork(albumArtTemp))
						{
							SetAlbumArtToLayerW(albumArtTemp,lid);
						}
						else
						{
							if(!artWorkInFolderExists)
								DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
							else
								SetAlbumArtToLayerW(strArtWorkInFolder.c_str(),lid);
						}

						/*

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
									CComBSTR bstrPath(albumArtTemp);
									if(SUCCEEDED(artwork->SaveArtworkToFile(bstrPath)))
									{
										SetAlbumArtToLayerW(albumArtTemp,lid);
									}
									else
									{
										if(!artWorkInFolderExists)
											DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
										else
											SetAlbumArtToLayerW(strArtWorkInFolder.c_str(),lid);
									}
								}
								else
								{
									if(!artWorkInFolderExists)
										DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
									else
										SetAlbumArtToLayerW(strArtWorkInFolder.c_str(),lid);
								}
							}
							else
							{
								if(!artWorkInFolderExists)
									DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
								else
									SetAlbumArtToLayerW(strArtWorkInFolder.c_str(),lid);
							}
						}
						else
						{
							if(!artWorkInFolderExists)
								DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
							else
								SetAlbumArtToLayerW(strArtWorkInFolder.c_str(),lid);
						}
						*/

					}
					else
					{
					//	DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);
					}
				}

				songName = trackName;
			}
			else
			{
				BOOL success(FALSE);
				UINT lid = DeskletLayerGetLayer(GetWindow(),"iTunesWidget.AlbumArt",&success);
				if(success)
					DeskletLayerSetVisibility(GetWindow(),lid,FALSE,FALSE);

				ResetSongName(); 

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
			if(!musicApp.IsLoaded())
			{
				g.SetSmoothingMode(SmoothingModeAntiAlias);
				g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
				Gdiplus::FontFamily  fontFamily(skin->GetFontFace());
				SolidBrush  black(skin->GetTextColor());

				GraphicsPath path(Gdiplus::FillModeAlternate), path2(Gdiplus::FillModeAlternate);
				StringFormat format(0, LANG_NEUTRAL);
				//format.SetLineAlignment(StringAlignmentCenter);
				format.SetAlignment((StringAlignment)skin->GetFontHAlignment());


				//RectF rc(8,18,120,20);

				if((!useRevertHotspot || revertHotSpot.GetInfoShowDuringAnimation()==TRUE) && skin->GetInfoRect().Width != 0 && skin->GetInfoRect().Height!=0)
				{
					ShadowInfo& si = skin->GetFontShadow();
					if(si.GetDoShadow() && si.GetBlur() > 0)
					{
						Rect rcDraw = skin->GetInfoRect();

						Rect r = si.TransformRect(rcDraw);

						GraphicsPath path4(FillModeAlternate);
						SolidBrush shadowBrush( si.GetColor() );
						Rect rcShadow = skin->GetInfoRect();
						rcShadow.X= rcShadow.Y = 5;

						Bitmap shadow(rcDraw.Width + 10,rcDraw.Height + 10);
						Graphics g2(&shadow);
						g2.SetSmoothingMode(SmoothingModeAntiAlias);
						g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
						path4.AddString(L"iTunes not loaded", -1, &fontFamily, skin->GetFontStyle(), skin->GetFontSize(), rcShadow, &format);
						g2.FillPath(&shadowBrush, &path4);

						Bitmap* blurred = GetBlur(&shadow,si.GetBlur());

						if(blurred)
						{
							Rect r = si.TransformRect(rcDraw);
							r.X -= 5;
							r.Y -= 5;
							r.Width += 10;
							r.Height += 10;
							g.DrawImage(blurred,r.X,r.Y,r.Width,r.Height);
						}
						//g.DrawImage(blurred,float(rcDraw.X)-5,float(rcDraw.Y)-5,(float)blurred->GetWidth()+2,(float)blurred->GetHeight()+2);
						//g.DrawImage(&shadow,float(rcDraw.X)+0.5f,float(rcDraw.Y)+0.5f,float(rcDraw.Width),float(rcDraw.Height));

						delete blurred;

					}

					path.AddString(L"iTunes not loaded", -1, &fontFamily, skin->GetFontStyle(), skin->GetFontSize(), skin->GetInfoRect(), &format);
					g.FillPath(&black, &path);
				}
			}
			else
			{
				if(!useRevertHotspot || revertHotSpot.GetSoundShowDuringAnimation()==TRUE)
				{
					if(skin->GetSoundRect().Width != 0 && skin->GetSoundRect().Height != 0)
					{

						long sound(musicApp.GetSoundVolume());
						//GetApp()->get_SoundVolume(&sound);
						if(oldSound != sound)
						{
								delete soundBmp;
								soundBmp = 0;
						}

						Rect& r = skin->GetSoundRect();

						int dir = skin->GetSoundDirection();
						if(skin->GetUseSoundKnob())
						{
							if(!soundKnobBmp)
							{
								char path1[MAX_PATH] = {0};
								DeskletGetRelativeFolder(GetWindow(),path1);
								PathAppend(path1,"\\skins\\");
								PathAppend(path1,skinDir);
								PathAppend(path1,"\\images\\sound_knob.png");
								soundKnobBmp = DeskletLoadGDIPlusImage(path1);
							}

							float p = float(sound)/100.0f;

							if(dir == DIR_LEFT_RIGHT)
							{
								g.DrawImage(soundKnobBmp,float(r.X + r.Width * p), (float)r.Y,soundKnobBmp->GetWidth(),soundKnobBmp->GetHeight() );
							}
							else if(dir == DIR_RIGHT_LEFT)
							{
								g.DrawImage(soundKnobBmp,(float)(r.X + r.Width - (r.Width * p)), (float)r.Y,soundKnobBmp->GetWidth(),soundKnobBmp->GetHeight() );
							}
							else if(dir == DIR_TOP_BOTTOM)
							{
								g.DrawImage(soundKnobBmp,(float)r.X, (float)(r.Y + r.Height*p),soundKnobBmp->GetWidth(),soundKnobBmp->GetHeight() );
							}
							else if(dir == DIR_BOTTOM_TOP)
							{
								g.DrawImage(soundKnobBmp,(float)r.X, (float)(r.Y + + r.Height -  r.Height*p),soundKnobBmp->GetWidth(),soundKnobBmp->GetHeight() );
							}
						}
						else if(!soundBmp)
						{
							soundBmp = new Bitmap(r.Width,r.Height);
							
							char path1[MAX_PATH] = {0};
							DeskletGetRelativeFolder(GetWindow(),path1);
							PathAppend(path1,"\\skins\\");
							PathAppend(path1,skinDir);
							PathAppend(path1,"\\images\\sound_unfilled.png");
							Bitmap* soundUnfilled = DeskletLoadGDIPlusImage(path1);

							DeskletGetRelativeFolder(GetWindow(),path1);
							PathAppend(path1,"\\skins\\");
							PathAppend(path1,skinDir);
							PathAppend(path1,"\\images\\sound_filled.png");
							Bitmap* soundFilled = DeskletLoadGDIPlusImage(path1);

							Graphics g2(soundBmp);
							g2.SetSmoothingMode(SmoothingModeAntiAlias);
							g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);

							float p = float(sound)/100.0f;
							int dir = skin->GetSoundDirection();
							if(dir == DIR_LEFT_RIGHT)
							{
								RectF r_(0.f,0.f,float(r.Width) * p,float(r.Height));
								g2.DrawImage(soundFilled,r_,0.f,0.f,float(r.Width) * p,float(r.Height),UnitPixel);
								RectF rf(float(r.Width) * p,0.0f, float(r.Width) - float(r.Width)*p,float(r.Height));
								g2.DrawImage(soundUnfilled,rf,float(r.Width) * p,0.0f, float(r.Width) - float(r.Width)*p,float(r.Height),UnitPixel);
							}
							else if(dir == DIR_RIGHT_LEFT)
							{
								RectF r_(0.f,0.f,float(r.Width) * p,(float)r.Height);
								g2.DrawImage(soundUnfilled,r_,0.f,0.f,float(r.Width) * p,(float)r.Height,UnitPixel);
								RectF rf(float(r.Width) * p,0.f, (float)r.Width - float(r.Width)*p,float(r.Height));
								g2.DrawImage(soundFilled,rf,float(r.Width) * p,0.f, (float)r.Width - float(r.Width)*p,float(r.Height),UnitPixel);
							}
							else if(dir == DIR_TOP_BOTTOM)
							{
								RectF r_(0.f,0.f,(float)r.Width,float(r.Height) * p);
								g2.DrawImage(soundFilled,r_,0.f,0.f,(float)r.Width,float(r.Height) * p,UnitPixel);
								RectF rf(0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p);
								g2.DrawImage(soundUnfilled,rf,0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p,UnitPixel);
							}
							else if(dir == DIR_BOTTOM_TOP)
							{
								RectF r_(0.f,0.f,(float)r.Width,float(r.Height) * p);
								g2.DrawImage(soundUnfilled,r_,0.f,0.f,(float)r.Width,float(r.Height) * p,UnitPixel);
								RectF rf(0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p);
								g2.DrawImage(soundFilled,rf,0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p,UnitPixel);
							}

							delete soundUnfilled;
							delete soundFilled;
						}

						g.DrawImage(soundBmp,r.X,r.Y,r.Width,r.Height);

						oldSound = sound;

					}
				}

				long curPos(musicApp.GetPlayerPosition());
				//GetApp()->get_PlayerPosition(&curPos);
				//CComPtr<IITTrack> track;
				//GetApp()->get_CurrentTrack(&track);
				//if(track)
				if(musicApp.HasTrack())
				{
					g.SetSmoothingMode(SmoothingModeAntiAlias);
					g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

					std::vector<int> extraInfoShowDuringAnimation = revertHotSpot.GetShowExtraPanesDuringAnimation();
					for(int i = 0;i<skin->GetInfos().size();++i)					
					{

						if(!useRevertHotspot || extraInfoShowDuringAnimation[i] ==TRUE)
							skin->GetInfos()[i]->Draw(g,musicApp );
					}

					long length(musicApp.GetTrackDuration());
					//track->get_Duration(&length);

					BOOL ok(FALSE);


					CComBSTR artist;
					HRESULT hr = S_OK;
					if(counter < 9)
						artist = CComBSTR(musicApp.GetTrackArtist(&ok).c_str());
						//hr = track->get_Artist(&artist);

					if (counter >= 9 || !ok || !artist  || artist[0] == 0)
						artist = CComBSTR(musicApp.GetTrackName2(&ok).c_str());
						//track->get_Name(&artist);


					if(skin->GetRatingRect().Width != 0 && skin->GetRatingRect().Height != 0)
					{
						if(!useRevertHotspot || revertHotSpot.GetRatingShowDuringAnimation()==TRUE)
						{
							long rating(musicApp.GetTrackRating());
							//track->get_Rating(&rating);
							if(oldRating != rating)
							{
									delete ratingBmp;
									ratingBmp = 0;
							}
	
							Rect& r = skin->GetRatingRect();

							if(!ratingBmp)
							{
								ratingBmp = new Bitmap(r.Width,r.Height);
								
								char path1[MAX_PATH] = {0};
								DeskletGetRelativeFolder(GetWindow(),path1);
								PathAppend(path1,"\\skins\\");
								PathAppend(path1,skinDir);
								PathAppend(path1,"\\images\\rating_unfilled.png");
								Bitmap* ratingUnfilled = DeskletLoadGDIPlusImage(path1);

								DeskletGetRelativeFolder(GetWindow(),path1);
								PathAppend(path1,"\\skins\\");
								PathAppend(path1,skinDir);
								PathAppend(path1,"\\images\\rating_filled.png");
								Bitmap* ratingFilled = DeskletLoadGDIPlusImage(path1);

								Graphics g2(ratingBmp);
								g2.SetSmoothingMode(SmoothingModeAntiAlias);
								g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);

								long r2 = rating;
								if(GetUseFiveStarRating() )
									r2 = ( long(r2) / 20 * 20);
								float p = float(r2)/100.0f;
								int dir = skin->GetRatingDirection();
								if(dir == DIR_LEFT_RIGHT)
								{
									RectF r_(0.f,0.f,float(r.Width) * p,float(r.Height));
									g2.DrawImage(ratingFilled,r_,0.f,0.f,float(r.Width) * p,float(r.Height),UnitPixel);
									RectF rf(float(r.Width) * p,0.0f, float(r.Width) - float(r.Width)*p,float(r.Height));
									g2.DrawImage(ratingUnfilled,rf,float(r.Width) * p,0.0f, float(r.Width) - float(r.Width)*p,float(r.Height),UnitPixel);
								}
								else if(dir == DIR_RIGHT_LEFT)
								{
									RectF r_(0.f,0.f,float(r.Width) * p,(float)r.Height);
									g2.DrawImage(ratingUnfilled,r_,0.f,0.f,float(r.Width) * p,(float)r.Height,UnitPixel);
									RectF rf(float(r.Width) * p,0.f, (float)r.Width - float(r.Width)*p,float(r.Height));
									g2.DrawImage(ratingFilled,rf,float(r.Width) * p,0.f, (float)r.Width - float(r.Width)*p,float(r.Height),UnitPixel);
								}
								else if(dir == DIR_TOP_BOTTOM)
								{
									RectF r_(0.f,0.f,(float)r.Width,float(r.Height) * p);
									g2.DrawImage(ratingFilled,r_,0.f,0.f,(float)r.Width,float(r.Height) * p,UnitPixel);
									RectF rf(0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p);
									g2.DrawImage(ratingUnfilled,rf,0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p,UnitPixel);
								}
								else if(dir == DIR_BOTTOM_TOP)
								{
									RectF r_(0.f,0.f,(float)r.Width,float(r.Height) * p);
									g2.DrawImage(ratingUnfilled,r_,0.f,0.f,(float)r.Width,float(r.Height) * p,UnitPixel);
									RectF rf(0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p);
									g2.DrawImage(ratingFilled,rf,0.f,float(r.Height) * p, (float)r.Width, (float)r.Height - float(r.Height) *p,UnitPixel);
								}

								delete ratingUnfilled;
								delete ratingFilled;
							}

							g.DrawImage(ratingBmp,r.X,r.Y,r.Width,r.Height);
	
							oldRating = rating;
						}
					}
	
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
					format.SetAlignment((StringAlignment)skin->GetFontHAlignment());


					if((!useRevertHotspot || revertHotSpot.GetInfoShowDuringAnimation()==TRUE) && skin->GetInfoRect().Width !=0 && skin->GetInfoRect().Height != 0)
					{
						if(!IsScrolling() )
						{
							ShadowInfo& si = skin->GetFontShadow();
							if(si.GetDoShadow() && si.GetBlur() > 0)
							{
								if(infoShadow.GetString() != std::wstring( static_cast<WCHAR*>(artist) ) )
								{
									Rect rcDraw = skin->GetInfoRect();

									GraphicsPath path4(FillModeAlternate);
									SolidBrush shadowBrush( si.GetColor() );
									Rect rcShadow = skin->GetInfoRect();
									rcShadow.X= rcShadow.Y = 5;

									Bitmap shadow(rcDraw.Width + 10,rcDraw.Height + 10);
									Graphics g2(&shadow);
									g2.SetSmoothingMode(SmoothingModeAntiAlias);
									g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
									path4.AddString(artist, -1, &fontFamily, skin->GetFontStyle(), skin->GetFontSize(), rcShadow, &format);
									g2.FillPath(&shadowBrush, &path4);

									Bitmap* blurred = GetBlur(&shadow,si.GetBlur());
									infoShadow = CachedShadow(std::wstring( static_cast<WCHAR*>(artist) ),blurred);
								}

								if(infoShadow.GetBitmap())
								{
									Rect r = si.TransformRect(skin->GetInfoRect());
									r.X -= 5;
									r.Y -= 5;
									r.Width += 10;
									r.Height += 10;
									g.DrawImage(infoShadow.GetBitmap(),r.X,r.Y,r.Width,r.Height);
								}
							}

							path.AddString(artist, -1, &fontFamily, skin->GetFontStyle(), skin->GetFontSize(), skin->GetInfoRect(), &format);
							g.FillPath(&black, &path);

						}
						else
						{
							g.DrawImage(scrollBmp,skin->GetInfoRect().X-5,skin->GetInfoRect().Y-5,l,0,skin->GetInfoRect().Width,skin->GetInfoRect().Height+10,UnitPixel);
						}
					}

					if((!useRevertHotspot || revertHotSpot.GetCounterShowDuringAnimation()==TRUE) && skin->GetCounterRect().Width && skin->GetCounterRect().Height != 0 )
					{
						long d = ( length - curPos);
						if(showTimePlayed)
							d = curPos;

						format.SetAlignment((StringAlignment)skin->GetCounterHAlignment());
	
						FontFamily fontFamily2(skin->GetCounterFontFace() );
						SolidBrush cbrush(skin->GetCounterTextColor() );

						std::wstring txt = FormatTime(d).c_str();

						ShadowInfo& si = skin->GetCounterShadow();
						if(si.GetDoShadow() && si.GetBlur() > 0)
						{
							if(counterShadow.GetString() != txt )
							{
								Rect rcDraw = skin->GetCounterRect();

								GraphicsPath path4(FillModeAlternate);
								SolidBrush shadowBrush( si.GetColor() );
								Rect rcShadow = skin->GetCounterRect();
								rcShadow.X= rcShadow.Y = 5;

								Bitmap shadow(rcDraw.Width + 10,rcDraw.Height + 10);
								Graphics g2(&shadow);
								g2.SetSmoothingMode(SmoothingModeAntiAlias);
								g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
								path4.AddString(txt.c_str(),-1,&fontFamily2,skin->GetCounterStyle(),skin->GetCounterFontSize(),rcShadow,&format);
								g2.FillPath(&shadowBrush, &path4);

								Bitmap* blurred = GetBlur(&shadow,si.GetBlur());
								counterShadow = CachedShadow(txt,blurred);
							}

							if(counterShadow.GetBitmap())
							{
								Rect r = si.TransformRect(skin->GetCounterRect());
								r.X -= 5;
								r.Y -= 5;
								r.Width += 10;
								r.Height += 10;
								g.DrawImage(counterShadow.GetBitmap(),r.X,r.Y,r.Width,r.Height);
							}
						}

						path2.AddString(txt.c_str(),-1,&fontFamily2,skin->GetCounterStyle(),skin->GetCounterFontSize(),skin->GetCounterRect(),&format);
						g.FillPath(&cbrush,&path2);
					}


					if(!useRevertHotspot || revertHotSpot.GetProgressShowDuringAnimation()==TRUE)
					{
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
								{
									RectF rc((float)sr.X,(float)sr.Y,((float)sr.Width * percentageplayed / 100.0f),(float)skin->GetKnobHeight());
									g.DrawImage(knobBmp,rc,0,0,(int)((float)sr.Width * percentageplayed / 100.0f),skin->GetKnobHeight(),UnitPixel);
								}
	
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
			}

			// workaround for AveDesk1.1 bug
			if(bgErased)
			{
				DeskletLayerSetImage(GetWindow(),id,FALSE,bmp,TRUE);
				char bgPath[MAX_PATH] = {0};
				DeskletGetRelativeFolder(GetWindow(),bgPath);
				PathAppend(bgPath,"\\skins\\");
				PathAppend(bgPath,skinDir);
				PathAppend(bgPath,"\\images\\bg.png");
				DeskletSetImageFile(GetWindow(),bgPath);
				DeskletLayerSetVisibility(GetWindow(),0,TRUE,TRUE);
				bgErased = false;
			}
			else
			{
				DeskletLayerSetImage(GetWindow(),id,TRUE,bmp,TRUE);
			}

		}
		wasPaused = isPaused;
		wasLoaded = isLoaded ? true : false;

	}
	catch(...)
	{
		tc->unbindWinAmp();
	}
}

iTunesWidget* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE dll,const char* ini, const char* section)
{
	iTunesWidget* iTunes = new iTunesWidget(hwndDesklet,dll);

	if(ini && section)
		iTunes->ReadFromIni(ini,section);
	else
		iTunes->SetSkin("Default");

	iTunes->Redraw();
	//if(iTunes->GetApp() || iTunes->GetHideWhenNotLoaded() )
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
	//if(iTunes->GetApp())
	KillTimer(hwndDesklet,TIMERID);
	SleepEx(10,0);
	delete iTunes;
}

void __stdcall OnProcessMessage(iTunesWidget* iTunes, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(iTunes->broadcastMessage == msg)
	{
		iTunes->NotifyWinampClosing();
	}
	else if(WM_APP_STATUS_CHANGE == msg)
	{
		iTunes->SetAutoHideiTunes(iTunes->GetAutoHideiTunes());
	}
	if(msg == WM_APP_REDRAW)
	{
		iTunes->Redraw();
	}
	else if(msg == WM_TIMER && wParam == 3000)
	{
		iTunes->CheckAudioApps();
	}
	else if(msg == WM_TIMER && wParam == TIMERID)
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
		if(!iTunes->IsScrolling())
		{
			BOOL isLoaded = iTunes->CheckLoadedStatus();
			if(iTunes->GetHideWhenNotLoaded())
			{
				if(isLoaded && !DeskletIsVisible(iTunes->GetWindow()))
					DeskletShow(iTunes->GetWindow());
				else if(!isLoaded && DeskletIsVisible(iTunes->GetWindow()))
					DeskletHide(iTunes->GetWindow());
			}

			if(iTunes->NeedsToRedraw() && iTunes->GetShouldRedraw() )
				iTunes->Redraw();
		}
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
			if(iTunes->GetShouldRedraw())
				iTunes->Redraw();
		}
		else if(msg==WM_INITMENUPOPUP)
		{
			UINT uPos = (UINT)LOWORD(lParam);
			if(uPos != 0)
			{
				uPos++;
			}
		}
	}
}

inline bool PointInRect(Rect& r,POINT& p)
{
	return (p.x > r.X && p.x < (r.X+r.Width) && p.y > (r.Y) && p.y < (r.Y + r.Height));
}

DoHotSpotsClick(iTunesWidget* iTunes, POINT* p, SIZE* s)
{
			std::vector<ChangeSkinHotSpot*> hotSpots = iTunes->GetSkin()->GetHotSpots();
			for(int i=0;i<hotSpots.size();++i)
			{
				if(!hotSpots[i]->IsMouseOver() && PointInRect(hotSpots[i]->GetRect(),*p) && stricmp(hotSpots[i]->GetSkinName(),iTunes->GetSkinDir())!=0)
				{
					ChangeSkinHotSpot* sel = hotSpots[i];

					Skin* oldSkin = iTunes->GetSkin();

					SIZE s = {0};
					DeskletGetSize(iTunes->GetWindow(),&s);
					POINT p = {0};
					DeskletGetPosition(iTunes->GetWindow(),&p);
					RECT r = {0};
					r.left = p.x + hotSpots[i]->GetX();
					r.right = r.left + s.cx;
					r.top = p.y + hotSpots[i]->GetY();
					r.bottom = r.top + s.cy;

					iTunes->SetMouseWasOnHotSpot(false);

					std::vector<std::string>& frames = hotSpots[i]->GetFrames();
					if(frames.size() ==0 )
					{
						if(hotSpots[i]->GetSkinName()[0] != 0)
						{

							DeskletMove(iTunes->GetWindow(),&r,FALSE);
							iTunes->SetSkin(hotSpots[i]->GetSkinName() );
						}
						else
						{
							DeskletMove(iTunes->GetWindow(),&r,TRUE);
						}
					}
					else
					{
						RECT r2	 = {0};
						POINT p2 = {0};
						SIZE s2  = {0};
						DeskletGetPosition(iTunes->GetWindow(),&p2);
						DeskletGetSize(iTunes->GetWindow(),&s2);
						r2.left = p2.x;
						r2.right = r2.left + s2.cx;
						r2.top = p2.y;
						r2.bottom = r2.top + s2.cy;

						char oldSkinDir[MAX_PATH] = {0};
						strcpy(oldSkinDir,iTunes->GetSkinDir());
						if(!hotSpots[i]->GetUseCurrentSkinSizeForAnim())
						{
							DeskletMove(iTunes->GetWindow(),&r,FALSE);
						}
						
						iTunes->SetRevertHotSpot(*hotSpots[i]);
						if(sel->GetNoHideDuringAnimation() )
						{
							iTunes->Redraw(true,true);
						}

						iTunes->SetSkin(hotSpots[i]->GetSkinName(),true,sel->GetNoHideDuringAnimation()==TRUE);
						RECT r3	 = {0};
						POINT p3 = {0};
						SIZE s3  = {0};
						DeskletGetPosition(iTunes->GetWindow(),&p3);
						DeskletGetSize(iTunes->GetWindow(),&s3);
						r3.left = p3.x;
						r3.right = r3.left + s3.cx;
						r3.top = p3.y;
						r3.bottom = r3.top + s3.cy;
						if(hotSpots[i]->GetUseCurrentSkinSizeForAnim())
						{
							DeskletMove(iTunes->GetWindow(),&r2,FALSE);
						}

						if(sel->GetNoHideDuringAnimation() && !sel->GetUseCurrentSkinSizeForAnim() )
						{
							BOOL success(FALSE);
							UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetAlbumArtRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,sel->GetNewBgWidth(),sel->GetNewBgHeight());
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Pause_Button",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetPauseRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,sel->GetNewBgWidth(),sel->GetNewBgHeight());
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Info",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetDisplayRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,sel->GetNewBgWidth(),sel->GetNewBgHeight());
							}
						}

						if(sel->GetSoundForAnimation()[0] != 0 && !iTunes->GetNeverPlaySoundsInSkins())
						{

							char path1[MAX_PATH] = {0};
							GetDLLPath(iTunes->GetDLL(),path1,MAX_PATH);
							PathAppend(path1,"skins\\");
							PathAppend(path1,oldSkinDir);
							PathAppend(path1,sel->GetSoundForAnimation());
							//MessageBox(0,path1,0,0);
							PlaySound(path1,NULL,SND_FILENAME | SND_ASYNC);

						}

						for(int ii=0;ii<frames.size();++ii)
						{
							char path1[MAX_PATH] = {0};
							DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
							PathAppend(path1,"\\skins\\");
							PathAppend(path1,oldSkinDir);
							PathAppend(path1,"\\images\\");
							PathAppend(path1,frames[ii].c_str());
							DeskletSetImageFile(iTunes->GetWindow(),path1);
						}

						if(hotSpots[i]->GetUseCurrentSkinSizeForAnim())
						{
							DeskletMove(iTunes->GetWindow(),&r3,FALSE);
						}


						if(sel->GetNoHideDuringAnimation() )
						{
							BOOL success(FALSE);
							UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);
							if(success)
							{	
								Rect& sr3 =iTunes->GetSkin()->GetAlbumArtRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,iTunes->GetSkin()->GetWidth(),iTunes->GetSkin()->GetHeight());
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Pause_Button",&success);
							if(success)
							{	
								Rect& sr3 =iTunes->GetSkin()->GetPauseRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,iTunes->GetSkin()->GetWidth(),iTunes->GetSkin()->GetHeight());

								char pausePath[MAX_PATH] = {0};
								DeskletGetRelativeFolder(iTunes->GetWindow(),pausePath);
								PathAppend(pausePath,"skins\\");
								PathAppend(pausePath,iTunes->GetSkinDir());
								PathAppend(pausePath,"\\images\\pause.png");
								DeskletLayerSetImageFile(iTunes->GetWindow(),lid,FALSE,pausePath);
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Info",&success);
							if(success)
							{	
								Rect& sr3 =iTunes->GetSkin()->GetDisplayRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,iTunes->GetSkin()->GetWidth(),iTunes->GetSkin()->GetHeight());
							}
						}

						/*char path1[MAX_PATH] = {0};
						DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
						PathAppend(path1,"\\skins\\");
						PathAppend(path1,iTunes->GetSkinDir());
						PathAppend(path1,"\\images\\bg.png");
						//PathAppend(path1,frames[ii].c_str());
						DeskletSetImageFile(iTunes->GetWindow(),path1);
						*/

						BOOL success(FALSE);
						UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);

						if(sel->GetfadeAlbumArtIn())
						{
							if(success)
								DeskletLayerSetAlpha(iTunes->GetWindow(),lid,FALSE,0);
						}

						iTunes->Redraw(true);

						if(sel->GetfadeAlbumArtIn())
						{
							if(success)
							{ 
								for(int z=0;z<sel->GetNumberOfFadeFrames();z++)
								{
									DeskletLayerSetAlpha(iTunes->GetWindow(),lid,TRUE,255.0f/float(sel->GetNumberOfFadeFrames()) * (z+1));

								}
							}
						}
					
					}
					delete oldSkin;


					return TRUE;
				}
			}

			return FALSE;
}

BOOL __stdcall OnLeftButtonClick(iTunesWidget* iTunes, POINT* p, SIZE* s)
{

	iTunes->CheckLoadedStatus();

	/*
	if(!iTunes->GetApp())
	{
		iTunes->GetApp().CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
		if(iTunes->GetApp())
			SetTimer(iTunes->GetWindow(),TIMERID,1000,0);
		iTunes->SetAutoHideiTunes(iTunes->GetAutoHideiTunes());
	}*/

	MusicApp& ma = iTunes->GetMusicApp();

	if(ma.IsLoaded())
	{
		Rect& pr = iTunes->GetSkin()->GetPauseRect();
		Rect& nr = iTunes->GetSkin()->GetNextRect();
		Rect& br = iTunes->GetSkin()->GetPrevRect();
		Rect& cr = iTunes->GetSkin()->GetCounterRect();
		Rect& dr = iTunes->GetSkin()->GetDisplayRect();
		Rect& prr= iTunes->GetSkin()->GetProgressRect();
		Rect& ir = iTunes->GetSkin()->GetInfoRect();
		Rect& rr = iTunes->GetSkin()->GetRatingRect();
		Rect& sr = iTunes->GetSkin()->GetSoundRect();

		if(PointInRect(iTunes->GetSkin()->GetStopRect(),*p ) )
		{
			ma.Stop();
			//iTunes->GetApp()->Stop();
			return TRUE;
		}
		else if(PointInRect(iTunes->GetSkin()->GetIpodRect(),*p  ) )
		{
			ma.UpdateIPod();
			//iTunes->GetApp()->UpdateIPod();
			return TRUE;
		}
		else if(PointInRect(iTunes->GetSkin()->GetMuteRect(),*p  ) )
		{
			BOOL muted = ma.GetIsMuted();
			muted = !muted;
			ma.SetIsMuted(muted);
			/*
			VARIANT_BOOL isMuted = { VARIANT_FALSE };
			iTunes->GetApp()->get_Mute(&isMuted);
			if(isMuted == VARIANT_TRUE)
				isMuted = VARIANT_FALSE;
			else
				isMuted = VARIANT_TRUE;
			iTunes->GetApp()->put_Mute(isMuted);
			*/
			return TRUE;
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
				MultiByteToWideChar(GetACP(),0,buffer,-1,wPath,MAX_PATH);
				//CComBSTR bstrFile(wPath);

				ma.AddFile(wPath, iTunes->GetStatus());
			}
			return TRUE;
		}
		else if(PointInRect(iTunes->GetSkin()->GetQuitRect(),*p ) )
		{
			ma.Quit();

			//iTunes->GetApp()->Quit();
			//iTunes->GetApp() = 0;
			return TRUE;


		}
		else if(PointInRect(iTunes->GetSkin()->GetToggleITunesWindowRect(),*p ) )
		{
			if(ma.HasWindow())
			{
				if(ma.IsWindowVisible())
					ma.HideWindow();
				else
					ma.ShowWindow();

				return TRUE;
			}
			/*
			HWND hwnd = FindWindow("iTunes",0);
			if(hwnd)
			{
				if(IsWindowVisible(hwnd))
					ShowWindow(hwnd,SW_HIDE);
				else
					ShowWindow(hwnd,SW_SHOWNORMAL);

				return TRUE;
			}
			*/
		}
		else if(p->x > pr.X && p->x < (pr.X+pr.Width) && p->y > (pr.Y) && p->y < (pr.Y + pr.Height))
		{
			ma.PlayPause();
			//iTunes->GetApp()->PlayPause();
			iTunes->Redraw();
			return TRUE;
		}
		else if(p->x > nr.X && p->x < (nr.X+nr.Width) && p->y > (nr.Y) && p->y < (nr.Y + nr.Height))
		{
			ma.Next();
			//iTunes->GetApp()->NextTrack();
			iTunes->Redraw();
			return TRUE;
		}
		else if(p->x > br.X && p->x < (br.X+br.Width) && p->y > br.Y && p->y < (br.Y+br.Height) )
		{
			ma.Prev();
			//iTunes->GetApp()->BackTrack();
			iTunes->Redraw();
			return TRUE;
		}
		else if(p->x > (dr.X+ir.X) && p->x < (dr.X+ir.X+ir.Width) && p->y > (dr.Y+ir.Y) && p->y < (dr.Y+ir.Y+ir.Height))
		{
			while(iTunes->Count() % 9 != 1);
			iTunes->Redraw();
			return TRUE;
		}
		else if(p->x > (dr.X+cr.X) && p->x < (dr.X+cr.X+cr.Width) && p->y > (dr.Y+cr.Y) && p->y < (dr.Y+cr.Y+cr.Height))
		{
			iTunes->ToggleShowTimePlayed();
			iTunes->Redraw();
			return TRUE;
		}
		else if(p->x > (dr.X+rr.X) && p->x < (dr.X+rr.X+rr.Width) && p->y > (rr.Y+dr.Y) && p->y < (rr.Y+dr.Y+rr.Height))
		{
		//	CComPtr<IITTrack> track;
		//	iTunes->GetApp()->get_CurrentTrack(&track);
		//	if(track)
			if(ma.HasTrack())
			{
				long rating(0);
				int dir = iTunes->GetSkin()->GetRatingDirection();
				if(dir == DIR_LEFT_RIGHT)
				{
					int pos = p->x - (dr.X+rr.X);
					float percentage = ((float)pos) / (float)(rr.Width) * 100.0f;
					rating = (long)percentage;
				}
				else if ( dir == DIR_RIGHT_LEFT)
				{
					int pos = p->x - (dr.X+rr.X);
					float percentage = 100.0f - ( ((float)pos) / (float)(rr.Width) * 100.0f);
					rating = (long)percentage;
				}
				else if ( dir == DIR_TOP_BOTTOM)
				{
					int pos = p->y - (dr.Y+rr.Y);
					float percentage = ((float)pos) / (float)(rr.Height) * 100.0f;
					rating = (long)percentage;
				}
				else if ( dir == DIR_BOTTOM_TOP)
				{
					int pos = p->y - (dr.Y+rr.Y);
					float percentage = 100.0f - (((float)pos) / (float)(rr.Height) * 100.0f);
					rating = (long)percentage;
				}
				if(iTunes->GetUseFiveStarRating() )
				{
					if(rating < 10)
						rating = 0;
					else if(rating > 87)
						rating = 100;
					else
						rating = long(rating/20) * 20 + 20;
				}

				//track->put_Rating(rating);
				ma.SetRating(rating);
			}
			iTunes->Redraw();
			return TRUE;
		}
		else if(p->x > (dr.X+sr.X) && p->x < (dr.X+sr.X+sr.Width) && p->y > (sr.Y+dr.Y) && p->y < (sr.Y+dr.Y+sr.Height))
		{
			//if(iTunes->GetApp())
			{
				long sound(0);
				int dir = iTunes->GetSkin()->GetSoundDirection();
				if(dir == DIR_LEFT_RIGHT)
				{
					int pos = p->x - (dr.X+sr.X);
					float percentage = ((float)pos) / (float)(sr.Width) * 100.0f;
					sound = (long)percentage;
				}
				else if ( dir == DIR_RIGHT_LEFT)
				{
					int pos = p->x - (dr.X+sr.X);
					float percentage = 100.0f - ( ((float)pos) / (float)(sr.Width) * 100.0f);
					sound = (long)percentage;
				}
				else if ( dir == DIR_TOP_BOTTOM)
				{
					int pos = p->y - (dr.Y+sr.Y);
					float percentage = ((float)pos) / (float)(sr.Height) * 100.0f;
					sound = (long)percentage;
				}
				else if ( dir == DIR_BOTTOM_TOP)
				{
					int pos = p->y - (dr.Y+sr.Y);
					float percentage = 100.0f - (((float)pos) / (float)(sr.Height) * 100.0f);
					sound = (long)percentage;
				}
				if(sound>100)sound=100;
				//iTunes->GetApp()->put_SoundVolume(sound);
				ma.SetSoundVolume(sound);
			}
			iTunes->Redraw();
			return TRUE;
		}
		else
		{
			if(DoHotSpotsClick(iTunes, p, s))
				return TRUE;
		}

		if(p->x > (dr.X+prr.X) && p->x < (dr.X+prr.X+prr.Width) && p->y > (prr.Y+dr.Y) && p->y < (prr.Y+dr.Y+prr.Height))
		{
			//CComPtr<IITTrack> track;
			//iTunes->GetApp()->get_CurrentTrack(&track);
			//if(track)
			if(ma.HasTrack())
			{
				long length(ma.GetTrackDuration());
				//track->get_Duration(&length);
				int pos = p->x - (dr.X+prr.X);
				float percentage = ((float)pos) / (float)(prr.Width) * 100.0f;
				long newPos = (long)(((float)length) *( percentage/100.0f));
				ma.SetPlayerPosition(newPos);
				//iTunes->GetApp()->put_PlayerPosition((((float)length) *( percentage/100.0f)));
			}
			iTunes->Redraw();
		}

	}
	else
	{
		//MessageBox(0,"You seem to have the wrong version of iTunes.\nYou will at least need version 4.5 of iTunes.\nPlease check Apple\'s website for an update.","iTunesWidget",MB_ICONERROR);
		if(DoHotSpotsClick(iTunes, p, s))
			return TRUE;
	}
	return TRUE;
}

BOOL __stdcall OnRightButtonClick(iTunesWidget* iTunes, POINT* p, SIZE* s)
{
	MusicApp& ma = iTunes->GetMusicApp();

	iTunes->CheckLoadedStatus();

	if(!ma.IsLoaded())
	{
		if(ma.StartApp(iTunes->GetPrefStart()))
		{
			SetTimer(iTunes->GetWindow(),TIMERID,1000,0);
			iTunes->SetAutoHideiTunes(iTunes->GetAutoHideiTunes());
		}

	}

	HMENU menu = DeskletGetDefaultMenu(iTunes->GetWindow(),1);
	AppendMenu(menu,MF_SEPARATOR,0,"-");

	long playlistCount(0);
	if(ma.IsLoaded())
	{
		HMENU plmenu = CreatePopupMenu();

		long count(0);
		std::vector<std::wstring> pls = ma.GetPlaylists();
		for(std::vector<std::wstring>::iterator iter = pls.begin(); iter != pls.end(); ++iter)
		{
			std::wstring name(*iter);
			AppendMenuW(plmenu,MF_STRING,count+1000,name.c_str());

			++count;
		}
		playlistCount = pls.size();

		AppendMenuW(menu,MF_POPUP ,(UINT)plmenu,L"Library");
	}

	/*
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
						for(long pli=1; pli <= plcount; ++pli)
						{
							CComPtr<IITPlaylist> playlist;
							playlists->get_Item(pli,&playlist);
							if(playlist)
							{
								BSTR plname;
								playlist->get_Name(&plname);
								AppendMenuW(plmenu,MF_STRING,v_playlists.size()+1000,plname);
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
	*/
	POINT pt = {0};
	GetCursorPos(&pt);
	int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, iTunes->GetWindow(),0);
	if(cmd < 1000)
	{
		DeskletDoDefaultMenuCommand(iTunes->GetWindow(),1,cmd);
	}
	else if(cmd >= 1000 && cmd <= playlistCount+1000 )
	{
		ma.StartPlaylist(cmd-1000);
	}
	DestroyMenu(menu);
	return TRUE;
}

BOOL __stdcall OnMouseWheelForward(iTunesWidget* iTunes, WORD fKeysDown, int numOfScrolls, int x, int y)
{
	MusicApp& ma = iTunes->GetMusicApp();
	if(ma.IsLoaded())
	{
		long volume(ma.GetSoundVolume());
		if(volume < 100)
		{
			ma.SetIsMuted(FALSE);
			volume+=5;
			ma.SetSoundVolume(volume);
		}
		iTunes->Redraw();
	}

	return TRUE;
	/*
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
		iTunes->Redraw(true);
	}
	
	return TRUE;
	*/
}

BOOL __stdcall OnMouseWheelBackward(iTunesWidget* iTunes, WORD fKeysDown, int numOfScrolls, int x, int y)
{
		MusicApp& ma = iTunes->GetMusicApp();
	if(ma.IsLoaded())
	{
		long volume(ma.GetSoundVolume());
		if(volume > 0)
		{
			volume-=5;
			ma.SetSoundVolume(volume);
		}
		iTunes->Redraw();
	}

	return TRUE;

	/*
	if(iTunes->GetApp())
	{
		long volume(0);
		iTunes->GetApp()->get_SoundVolume(&volume);
		if(volume>0)
		{
			volume-=4;
			iTunes->GetApp()->put_SoundVolume(volume);
		}
		iTunes->Redraw(true);
	}
	return TRUE;
	*/
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
		MusicApp& ma = iTunes->GetMusicApp();
		if(!ma.IsLoaded())
		{
			if(ma.StartApp(iTunes->GetPrefStart()))
			{
					SetTimer(iTunes->GetWindow(),TIMERID,1000,0);
			}

		}

		/*
		if(!iTunes->GetApp())
		{
			iTunes->GetApp().CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_ALL);
			if(iTunes->GetApp())
				SetTimer(iTunes->GetWindow(),TIMERID,1000,0);
		}	*/
		
		if(iTunes->GetStatus())iTunes->GetStatus().Release();

		DropStruct* ds = LockHDROP(obj);
	
		int num = DragQueryFile(ds->hDrop, -1, 0, 0);

		//CComPtr<IITLibraryPlaylist> library;
		//if(iTunes->GetApp() )
		//	iTunes->GetApp()->get_LibraryPlaylist(&library);

		CComBSTR firstFile;
		for(int i=0;i<num;++i)
		{
			OLECHAR path[MAX_PATH]= {0};
			char	lp[MAX_PATH] = {0};
			DragQueryFileW(ds->hDrop,i,path,MAX_PATH);
			DragQueryFile(ds->hDrop,i,lp,MAX_PATH);
			//CComBSTR file(path);

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
			
			if( strcmp(ext,".png") == 0  || 
				strcmp(ext,".jpg") == 0  || 
				strcmp(ext,".bmp") == 0  || 
				strcmp(ext,"jpeg") == 0)
			{
				MusicApp& ma = iTunes->GetMusicApp();
				if(ma.IsLoaded() && ma.HasTrack())
				{
					ma.AddArtWorkToTrack(path);
					iTunes->ResetSongName();
				}

				/*
				CComPtr<IITTrack> track;
				Tunes->GetApp()->get_CurrentTrack(&track);
				if(track)
				{
					CComPtr<IITArtwork> artwork;
					track->AddArtworkFromFile(file,&artwork);
					iTunes->ResetSongName();
				}*/
			}
			else
			{
				if(num==0)
					firstFile = CComBSTR(path);

				MusicApp& ma = iTunes->GetMusicApp();
				if(ma.IsLoaded())
				{
					ma.AddFile(path, iTunes->GetStatus());
				}
				//if(library)
				//{
				//	HRESULT hr = {0};
				//	hr = library->AddFile(file.operator BSTR(),&iTunes->GetStatus());
				//}
			}
		}

		UnlockHDROP(ds);

	}
	return TRUE;
}


BOOL __stdcall OnMouseMove(iTunesWidget* iTunes, int x,int y)
{
	if(iTunes->GetIsHovering())return TRUE;

	iTunes->SetIsHovering(true);

	POINT p= {x,y};
	
	if(iTunes->GetMouseWasOnHotSpot() && !PointInRect(iTunes->GetHotSpotRect(),p) )
	{
		iTunes->SetMouseWasOnHotSpot(false);
		if(iTunes->GetRevertSkin().size() > 0 && stricmp(iTunes->GetRevertSkin().c_str(),iTunes->GetSkinDir())!=0)
		{
			ChangeSkinHotSpot sel = iTunes->GetRevertHotSpot();

			Skin* oldSkin = iTunes->GetSkin();

			SIZE s = {0};
			DeskletGetSize(iTunes->GetWindow(),&s);
			POINT p = {0};
			DeskletGetPosition(iTunes->GetWindow(),&p);
			RECT r = {0};
			r.left = p.x + iTunes->GetHotSpotOffset().x;
			r.right = r.left + s.cx;
			r.top = p.y + iTunes->GetHotSpotOffset().y;
			r.bottom = r.top + s.cy;

			std::vector<std::string> frames = iTunes->GetHotSpotFrames();
			if(frames.size() == 0)
			{
				DeskletMove(iTunes->GetWindow(),&r,FALSE);
				iTunes->SetSkin(iTunes->GetRevertSkin().c_str() );	
			}
			else
			{
				RECT  oldR = {0};
				SIZE  oldS = {0};
				POINT oldP = {0};
				DeskletGetSize(iTunes->GetWindow(),&oldS);
				DeskletGetPosition(iTunes->GetWindow(),&oldP);
				oldR.left = oldP.x;
				oldR.top = oldP.y;
				oldR.right = oldR.left + oldS.cx;
				oldR.bottom = oldR.top + oldS.cy;
				char oldSkinDir[MAX_PATH] = {0};
				strcpy(oldSkinDir,iTunes->GetRevertSkin().c_str());

				if(sel.GetNoHideDuringAnimation() )
				{
						iTunes->Redraw(true,true);
				}

				iTunes->SetSkin(iTunes->GetRevertSkin().c_str(),true,sel.GetNoHideDuringAnimation()==TRUE);
				RECT  newR = {0};
				DeskletGetSize(iTunes->GetWindow(),&oldS);
				DeskletGetPosition(iTunes->GetWindow(),&oldP);
				newR.left = oldP.x;
				newR.top = oldP.y;
				newR.right = newR.left + oldS.cx;
				newR.bottom = oldR.top + oldS.cy;
				DeskletMove(iTunes->GetWindow(),&oldR,FALSE);

					if(sel.GetNoHideDuringAnimation() && !sel.GetUseCurrentSkinSizeForAnim() )
					{
							int xScale = sel.GetNewBgWidth();
							int yScale = sel.GetNewBgHeight();
							if(0 == xScale)
								xScale = oldSkin->GetWidth();
							if(0 == yScale)
								yScale = oldSkin->GetHeight();


							BOOL success(FALSE);
							UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetAlbumArtRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale, yScale);
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Pause_Button",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetPauseRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale, yScale);
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Info",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetDisplayRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale, yScale);
							}
						}

				if(sel.GetSoundForAnimationMouseOut()[0] != 0 && !iTunes->GetNeverPlaySoundsInSkins())
				{

					char path1[MAX_PATH] = {0};
					GetDLLPath(iTunes->GetDLL(),path1,MAX_PATH);
					PathAppend(path1,"skins\\");
					PathAppend(path1,oldSkinDir);
					PathAppend(path1,sel.GetSoundForAnimationMouseOut());
					PlaySound(path1,NULL,SND_FILENAME | SND_ASYNC);

				}


				for(int i=frames.size()-1;i>=0;--i)
				{
					char path1[MAX_PATH] = {0};
					DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
					PathAppend(path1,"\\skins\\");
					PathAppend(path1,oldSkinDir);
					PathAppend(path1,"\\images\\");
					PathAppend(path1,frames[i].c_str());
					DeskletSetImageFile(iTunes->GetWindow(),path1);
				}
				DeskletMove(iTunes->GetWindow(),&newR,FALSE);

				char path1[MAX_PATH] = {0};
				DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
				PathAppend(path1,"\\skins\\");
				PathAppend(path1,iTunes->GetSkinDir());
				PathAppend(path1,"\\images\\bg.png");
				//PathAppend(path1,frames[ii].c_str());
				DeskletSetImageFile(iTunes->GetWindow(),path1);

				if(sel.GetNoHideDuringAnimation() )
					{
						iTunes->ResetLayers();
					}

						BOOL success(FALSE);
						UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);

						if(sel.GetfadeAlbumArtIn())
						{
							if(success)
								DeskletLayerSetAlpha(iTunes->GetWindow(),lid,FALSE,0);
						}

						iTunes->Redraw(true);

						if(sel.GetfadeAlbumArtIn())
						{
							if(success)
							{ 
								for(int z=0;z<sel.GetNumberOfFadeFrames();z++)
								{
									DeskletLayerSetAlpha(iTunes->GetWindow(),lid,TRUE,255.0f/float(sel.GetNumberOfFadeFrames()) * (z+1));

								}
							}
						}

				//char path1[MAX_PATH] = {0};
				//DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
				//PathAppend(path1,"\\skins\\");
				//PathAppend(path1,iTunes->GetSkinDir());
				//PathAppend(path1,"\\images\\bg.png");
				//DeskletSetImageFile(iTunes->GetWindow(),path1);
			}

			delete oldSkin;
		}
	}


	std::vector<ChangeSkinHotSpot*> hotSpots = iTunes->GetSkin()->GetHotSpots();
	for(int i=0;i<hotSpots.size();++i)
	{
		if(hotSpots[i]->IsMouseOver() && PointInRect(hotSpots[i]->GetRect(),p))
		{
			ChangeSkinHotSpot* sel = hotSpots[i];

			Skin* oldSkin = iTunes->GetSkin();

			if(stricmp(hotSpots[i]->GetSkinName(),iTunes->GetSkinDir())!=0)
			{
				SIZE s = {0};
				DeskletGetSize(iTunes->GetWindow(),&s);
				POINT p = {0};
				DeskletGetPosition(iTunes->GetWindow(),&p);
				RECT r = {0};
				r.left = p.x + hotSpots[i]->GetX();
				r.right = r.left + s.cx;
				r.top = p.y + hotSpots[i]->GetY();
				r.bottom = r.top + s.cy;
				std::vector<std::string> frames = hotSpots[i]->GetFrames();
				if(frames.size() == 0)
				{
					if(hotSpots[i]->GetSkinName()[0] != 0)
					{
						DeskletMove(iTunes->GetWindow(),&r,FALSE);
						iTunes->SetSkin(hotSpots[i]->GetSkinName() );
					}
					else
					{
						DeskletMove(iTunes->GetWindow(),&r,TRUE);
					}
				}
				else
				{
					char oldSkinDir[MAX_PATH] = {0};
					strcpy(oldSkinDir,iTunes->GetSkinDir());
					DeskletMove(iTunes->GetWindow(),&r,FALSE);

					iTunes->SetRevertHotSpot(*hotSpots[i]);
					if(sel->GetNoHideDuringAnimation() )
					{
						iTunes->Redraw(true,true);
					}

					iTunes->SetSkin(hotSpots[i]->GetSkinName(),true,sel->GetNoHideDuringAnimation()==TRUE);
						
						if(sel->GetNoHideDuringAnimation() && !sel->GetUseCurrentSkinSizeForAnim() )
						{
							int xScale = sel->GetNewBgWidth();
							int yScale = sel->GetNewBgHeight();
							if(0 == xScale)
								xScale = oldSkin->GetWidth();
							if(0 == yScale)
								yScale = oldSkin->GetHeight();

							BOOL success(FALSE);
							UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetAlbumArtRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale,yScale);
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Pause_Button",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetPauseRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale,yScale);
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Info",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetDisplayRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale,yScale);
							}
						}

					if(sel->GetSoundForAnimation()[0] != 0 && !iTunes->GetNeverPlaySoundsInSkins())
					{

						char path1[MAX_PATH] = {0};
						GetDLLPath(iTunes->GetDLL(),path1,MAX_PATH);
						PathAppend(path1,"skins\\");
						PathAppend(path1,oldSkinDir);
						PathAppend(path1,sel->GetSoundForAnimation());
						PlaySound(path1,NULL,SND_FILENAME | SND_ASYNC);

					}

					for(int i=0;i<frames.size();++i)
					{
						char path1[MAX_PATH] = {0};
						DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
						PathAppend(path1,"\\skins\\");
						PathAppend(path1,oldSkinDir);
						PathAppend(path1,"\\images\\");
						PathAppend(path1,frames[i].c_str());
						DeskletSetImageFile(iTunes->GetWindow(),path1);
					}

					char path1[MAX_PATH] = {0};
					DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
					PathAppend(path1,"\\skins\\");
					PathAppend(path1,iTunes->GetSkinDir());
					PathAppend(path1,"\\images\\bg.png");
					//PathAppend(path1,frames[ii].c_str());
					DeskletSetImageFile(iTunes->GetWindow(),path1);

						if(sel->GetNoHideDuringAnimation() )
						{
							iTunes->ResetLayers();
						}

						BOOL success(FALSE);
						UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);

						if(sel->GetfadeAlbumArtIn())
						{
							if(success)
								DeskletLayerSetAlpha(iTunes->GetWindow(),lid,FALSE,0);
						}

						iTunes->Redraw(true);

						if(sel->GetfadeAlbumArtIn())
						{
							if(success)
							{ 
								for(int z=0;z<sel->GetNumberOfFadeFrames();z++)
								{
									DeskletLayerSetAlpha(iTunes->GetWindow(),lid,TRUE,255.0f/float(sel->GetNumberOfFadeFrames()) * (z+1));

								}
							}
						}
				}
			}

			iTunes->SetMouseWasOnHotSpot(true);
			Rect r2(hotSpots[i]->GetRect().X - hotSpots[i]->GetX(), hotSpots[i]->GetRect().Y - hotSpots[i]->GetY(),hotSpots[i]->GetRect().Width,hotSpots[i]->GetRect().Height);
			if(hotSpots[i]->GetNewRect().Width == 0 || hotSpots[i]->GetNewRect().Height == 0)
				iTunes->SetHotSpotRect(r2);
			else
				iTunes->SetHotSpotRect(hotSpots[i]->GetNewRect());
			POINT hsP = {hotSpots[i]->GetHotSpotX(),hotSpots[i]->GetHotSpotY() };
			iTunes->SetHotSpotOffset(hsP);
			iTunes->SetHotSpotFrames(hotSpots[i]->GetFrames());
			iTunes->SetRevertSkin(std::string(hotSpots[i]->GetOldSkinName()) );
			
			delete oldSkin;
			break;
		}
	}
	iTunes->SetIsHovering(false);
	return TRUE;
}

BOOL __stdcall OnMouseOn(iTunesWidget* iTunes)
{
	iTunes->SetDoScroll(true);
	if(!iTunes->IsScrolling() && iTunes->NeedScrolling(iTunes->GetScrollingText() ))
	{
		iTunes->BeginScrolling(iTunes->GetScrollingText() );
	}
	//SetCapture(iTunes->GetWindow() );
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

	if(iTunes->GetMouseWasOnHotSpot())
	{
		iTunes->SetMouseWasOnHotSpot(false);

		if(iTunes->GetRevertSkin().size() > 0 && stricmp(iTunes->GetRevertSkin().c_str(),iTunes->GetSkinDir())!=0)
		{
			ChangeSkinHotSpot sel = iTunes->GetRevertHotSpot();

			Skin* oldSkin = iTunes->GetSkin();

			SIZE s = {0};
			DeskletGetSize(iTunes->GetWindow(),&s);
			POINT p = {0};
			DeskletGetPosition(iTunes->GetWindow(),&p);
			RECT r = {0};
			r.left = p.x + iTunes->GetHotSpotOffset().x;
			r.right = r.left + s.cx;
			r.top = p.y + iTunes->GetHotSpotOffset().y;
			r.bottom = r.top + s.cy;

			std::vector<std::string> frames = iTunes->GetHotSpotFrames();
			if(frames.size() == 0)
			{
				DeskletMove(iTunes->GetWindow(),&r,FALSE);
				iTunes->SetSkin(iTunes->GetRevertSkin().c_str() );	
			}
			else
			{
				RECT  oldR = {0};
				SIZE  oldS = {0};
				POINT oldP = {0};
				DeskletGetSize(iTunes->GetWindow(),&oldS);
				DeskletGetPosition(iTunes->GetWindow(),&oldP);
				oldR.left = oldP.x;
				oldR.top = oldP.y;
				oldR.right = oldR.left + oldS.cx;
				oldR.bottom = oldR.top + oldS.cy;
				char oldSkinDir[MAX_PATH] = {0};
				strcpy(oldSkinDir,iTunes->GetRevertSkin().c_str());

				if(sel.GetNoHideDuringAnimation() )
				{
						iTunes->Redraw(true,true);
				}

				iTunes->SetSkin(iTunes->GetRevertSkin().c_str(),true,sel.GetNoHideDuringAnimation()==TRUE);
				RECT  newR = {0};
				DeskletGetSize(iTunes->GetWindow(),&oldS);
				DeskletGetPosition(iTunes->GetWindow(),&oldP);
				newR.left = oldP.x;
				newR.top = oldP.y;
				newR.right = newR.left + oldS.cx;
				newR.bottom = oldR.top + oldS.cy;
				DeskletMove(iTunes->GetWindow(),&oldR,FALSE);

					if(sel.GetNoHideDuringAnimation() && !sel.GetUseCurrentSkinSizeForAnim() )
						{
							int xScale = sel.GetNewBgWidth();
							int yScale = sel.GetNewBgHeight();
							if(0 == xScale)
								xScale = oldSkin->GetWidth();
							if(0 == yScale)
								yScale = oldSkin->GetHeight();


							BOOL success(FALSE);
							UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetAlbumArtRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale, yScale);
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Pause_Button",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetPauseRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale, yScale);
							}

							success = FALSE;
							lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.Info",&success);
							if(success)
							{	
								Rect& sr3 =oldSkin->GetDisplayRect();
								RECT r = {0};
								r.left = sr3.X;
								r.top = sr3.Y;
								r.right = sr3.X + sr3.Width;
								r.bottom = sr3.Y + sr3.Height;
								DeskletLayerSetPlacement(iTunes->GetWindow(),lid,FALSE,TRUE,&r,xScale, yScale);
							}
						}

				if(sel.GetSoundForAnimationMouseOut()[0] != 0 && !iTunes->GetNeverPlaySoundsInSkins())
				{

					char path1[MAX_PATH] = {0};
					GetDLLPath(iTunes->GetDLL(),path1,MAX_PATH);
					PathAppend(path1,"skins\\");
					PathAppend(path1,oldSkinDir);
					PathAppend(path1,sel.GetSoundForAnimationMouseOut());
					PlaySound(path1,NULL,SND_FILENAME | SND_ASYNC);

				}

				for(int i=frames.size()-1;i>=0;--i)
				{
					char path1[MAX_PATH] = {0};
					DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
					PathAppend(path1,"\\skins\\");
					PathAppend(path1,oldSkinDir);
					PathAppend(path1,"\\images\\");
					PathAppend(path1,frames[i].c_str());
					DeskletSetImageFile(iTunes->GetWindow(),path1);
				}
				DeskletMove(iTunes->GetWindow(),&newR,FALSE);

				char path1[MAX_PATH] = {0};
						DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
						PathAppend(path1,"\\skins\\");
						PathAppend(path1,iTunes->GetSkinDir());
						PathAppend(path1,"\\images\\bg.png");
						//PathAppend(path1,frames[ii].c_str());
						DeskletSetImageFile(iTunes->GetWindow(),path1);

				if(sel.GetNoHideDuringAnimation() )
					{
						iTunes->ResetLayers();
					}

						BOOL success(FALSE);
						UINT lid = DeskletLayerGetLayer(iTunes->GetWindow(),"iTunesWidget.AlbumArt",&success);

						if(sel.GetfadeAlbumArtIn())
						{
							if(success)
								DeskletLayerSetAlpha(iTunes->GetWindow(),lid,FALSE,0);
						}

						iTunes->Redraw(true);

						if(sel.GetfadeAlbumArtIn())
						{
							if(success)
							{ 
								for(int z=0;z<sel.GetNumberOfFadeFrames();z++)
								{
									DeskletLayerSetAlpha(iTunes->GetWindow(),lid,TRUE,255.0f/float(sel.GetNumberOfFadeFrames()) * (z+1));

								}
							}
						}

				//char path1[MAX_PATH] = {0};
				//DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
				//PathAppend(path1,"\\skins\\");
				//PathAppend(path1,iTunes->GetSkinDir());
				//PathAppend(path1,"\\images\\bg.png");
				//DeskletSetImageFile(iTunes->GetWindow(),path1);
			}

			delete oldSkin;
		}
		/*if(iTunes->GetRevertSkin().size() > 0 && stricmp(iTunes->GetRevertSkin().c_str(),iTunes->GetSkinDir())!=0)
		{
			Skin* oldSkin = iTunes->GetSkin();

			SIZE s = {0};
			DeskletGetSize(iTunes->GetWindow(),&s);
			POINT p = {0};
			DeskletGetPosition(iTunes->GetWindow(),&p);
			RECT r = {0};
			r.left = p.x + iTunes->GetHotSpotOffset().x;
			r.right = r.left + s.cx;
			r.top = p.y + iTunes->GetHotSpotOffset().y;
			r.bottom = r.top + s.cy;


			std::vector<std::string>& frames = iTunes->GetHotSpotFrames();
			if(frames.size() == 0)
			{
				DeskletMove(iTunes->GetWindow(),&r,FALSE);
				iTunes->SetSkin(iTunes->GetRevertSkin().c_str() );	
			}
			else
			{
				RECT  oldR = {0};
				SIZE  oldS = {0};
				POINT oldP = {0};
				DeskletGetSize(iTunes->GetWindow(),&oldS);
				DeskletGetPosition(iTunes->GetWindow(),&oldP);
				oldR.left = oldP.x;
				oldR.top = oldP.y;
				oldR.right = oldR.left + oldS.cx;
				oldR.bottom = oldR.top + oldS.cy;
				char oldSkinDir[MAX_PATH] = {0};
				strcpy(oldSkinDir,iTunes->GetRevertSkin().c_str());
				iTunes->SetSkin(iTunes->GetRevertSkin().c_str(),true );
				RECT  newR = {0};
				DeskletGetSize(iTunes->GetWindow(),&oldS);
				DeskletGetPosition(iTunes->GetWindow(),&oldP);
				newR.left = oldP.x;
				newR.top = oldP.y;
				newR.right = newR.left + oldS.cx;
				newR.bottom = newR.top + oldS.cy;
				DeskletMove(iTunes->GetWindow(),&oldR,FALSE);
				for(int i=frames.size()-1;i>=0;--i)
				{
					char path1[MAX_PATH] = {0};
					DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
					PathAppend(path1,"\\skins\\");
					PathAppend(path1,oldSkinDir);
					PathAppend(path1,"\\images\\");
					PathAppend(path1,frames[i].c_str());
					DeskletSetImageFile(iTunes->GetWindow(),path1);
				}
				DeskletMove(iTunes->GetWindow(),&newR,FALSE);
				char path1[MAX_PATH] = {0};
				DeskletGetRelativeFolder(iTunes->GetWindow(),path1);
				PathAppend(path1,"\\skins\\");
				PathAppend(path1,iTunes->GetSkinDir());
				PathAppend(path1,"\\images\\bg.png");
				DeskletSetImageFile(iTunes->GetWindow(),path1);
				iTunes->Redraw(true);
			}
			delete oldSkin;
		}*/
	}
	
	//ReleaseCapture();
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
			CheckDlgButton(hDlg,IDC_USE5STARSRATING, iTunes->GetUseFiveStarRating() ? TRUE : FALSE);
			CheckDlgButton(hDlg,IDC_ARTWORK, iTunes->GetUseFolderForArtwork() ? TRUE : FALSE);

			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_HIDEWIDGET: case IDC_HIDEITUNES: case IDC_USE5STARSRATING: case IDC_ARTWORK:
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
						iTunes->SetUseFiveStarRating( IsDlgButtonChecked(hDlg,IDC_USE5STARSRATING)==BST_CHECKED );
						iTunes->SetUseFolderForArtwork( IsDlgButtonChecked(hDlg,IDC_ARTWORK)==BST_CHECKED );
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
				if( SendDlgItemMessage(hDlg,IDC_SKIN,CB_SELECTSTRING,0,(LPARAM)iTunes->GetSkinDir()) == CB_ERR)
				{
					SendDlgItemMessage(hDlg,IDC_SKIN,CB_ADDSTRING,0,(LPARAM)iTunes->GetSkinDir());
					SendDlgItemMessage(hDlg,IDC_SKIN,CB_SELECTSTRING,0,(LPARAM)iTunes->GetSkinDir());
				}

				SetDlgItemText(hDlg,IDC_NAME,iTunes->GetSkin()->GetName());
				SetDlgItemText(hDlg,IDC_AUTHOR,iTunes->GetSkin()->GetAuthor());
				SetDlgItemText(hDlg,IDC_INFO,iTunes->GetSkin()->GetInfo());

				CheckDlgButton(hDlg,IDC_NEVERSOUNDS,iTunes->GetNeverPlaySoundsInSkins() ? TRUE : FALSE);
			}

			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_NEVERSOUNDS:
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

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
						iTunes->SetNeverPlaySoundsInSkins(IsDlgButtonChecked(hDlg,IDC_NEVERSOUNDS)==BST_CHECKED);
						char path[MAX_PATH] = {0};
						GetDlgItemText(hDlg,IDC_SKIN,path,MAX_PATH);
						Skin* oldSkin = iTunes->GetSkin();
						iTunes->SetSkin(path);
						delete oldSkin;

						SetDlgItemText(hDlg,IDC_NAME,iTunes->GetSkin()->GetName());
						SetDlgItemText(hDlg,IDC_AUTHOR,iTunes->GetSkin()->GetAuthor());
						SetDlgItemText(hDlg,IDC_INFO,iTunes->GetSkin()->GetInfo());
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

#include <map>

class CSkinEnumerator
{
private:
	WCHAR skinsPath[MAX_PATH];
	std::map<std::wstring,std::wstring> m_Skins;

public:
	CSkinEnumerator(HINSTANCE hInstance, const WCHAR* deskletPath)
	{
		WCHAR path[MAX_PATH] = {0};
		wcscpy(path,deskletPath);
		PathAppendW(path,L"skins\\");

		wcscpy(skinsPath,path);

		std::wstring s = path;
		s += L"*.*";

		WIN32_FIND_DATAW data = {0};
		HANDLE h = FindFirstFileW(s.c_str(),&data);
		if(h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(wcscmp(data.cFileName,L".")!=0 && wcscmp(data.cFileName,L"..")!=0 && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					m_Skins[data.cFileName] = data.cFileName;
				}while(FindNextFileW(h,&data));
			FindClose(h);
		}
	};

	UINT size()const { return m_Skins.size();};

	std::wstring getKey(UINT num)
	{
		std::map<std::wstring,std::wstring>::iterator iter = m_Skins.begin();
		std::advance(iter,num);
		//iter = num;
		return iter->first;
	};

	std::wstring getValue(std::wstring key)
	{
		return m_Skins[key];
	};

	std::wstring GetSkinsPath()const
	{
		return skinsPath;
	}
};

CSkinEnumerator* __stdcall OnCreateSkinEnumeratorW(HINSTANCE hInstance, const WCHAR* deskletPath)
{
	return new CSkinEnumerator(hInstance, deskletPath);
}

void __stdcall OnDestroySkinEnumeratorW(CSkinEnumerator* se)
{
	delete se;
}

INT __stdcall OnGetSkinW(CSkinEnumerator* se,SkinInfo* info)
{
	if(info->num >= se->size() )
		return AVEDESK_NO_MORE_SKINS;

	std::wstring skin = se->getKey(info->num);

	if(info->maxKeyLength < skin.length()+1 || info->maxNameLength < skin.length()+1)
	{
		info->maxKeyLength = skin.length() + 1;	
		info->maxNameLength = skin.length() + 1;
		return AVEDESK_BUF_TOO_SMALL;
	}

	wcscpy(info->wKey,skin.c_str());
	wcscpy(info->wName,skin.c_str());


	WCHAR ini[MAX_PATH];
	wcscpy(ini,se->GetSkinsPath().c_str());
	PathAppendW(ini,se->getValue(skin).c_str());
	PathAppendW(ini,L"skin.ini");

	GetPrivateProfileStringW(L"Info",L"Name",skin.c_str(),info->wName,info->maxNameLength,ini);
	GetPrivateProfileStringW(L"Info",L"Author",L"Unknown",info->wAuthor,info->maxAuthorLength,ini);
	GetPrivateProfileStringW(L"Info",L"Info",L"",info->wNotes,info->maxNotesLength,ini);
	info->version = GetPrivateProfileIntW(L"Info",L"Version",270,ini);

	return AVEDESK_MORE_SKINS;
}

iTunesWidget* __stdcall OnCreateFromSkinW(HWND hwnd,HINSTANCE hInstance, CSkinEnumerator* se, SkinInfo* skinInfo)
{
	USES_CONVERSION;

	iTunesWidget* desklet = OnCreate(hwnd,hInstance,NULL,NULL);
	if(se != NULL)
		desklet->SetSkin( W2A(se->getValue(skinInfo->wKey).c_str()) );
	else
		desklet->SetSkin(W2A(skinInfo->wKey));
	return desklet;
}