// FoundDeskletEntry.cpp: implementation of the CFoundDeskletEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FoundDeskletEntry.h"

#include <algorithm>

#include "DockletDLL.h"
#include <sstream>

#include "Docklet.h"
#include "Od_Docklet.hpp"
#include "helper_funcs.h"

#include "BitmapCache.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFoundDeskletEntry::CFoundDeskletEntry(DockletDLL* dll, DeskletSkinEnumerator* enumerator, DeskletSkinInfo* skinInfo) :
	dll(dll), enumerator(enumerator), skinInfo(skinInfo),
	m_ThreadHandle(NULL), m_CallbackHwnd(NULL), m_Msg(0), m_Stop(FALSE),
	m_ThreadHandle2(NULL), m_CallbackHwnd2(NULL), m_Msg2(0), m_Stop2(FALSE),
	m_hBmp(NULL),
	m_hBmpLarge(NULL),
	m_lParam(0L),
	largeBitmapLoadedCorrectly(FALSE),
	m_TriedLoadingLarge(FALSE)
{
	ZeroMemory(&requestDataSmall, sizeof(requestDataSmall));
	ZeroMemory(&requestDataLarge, sizeof(requestDataLarge));

	if(skinInfo != NULL)
	{
		if(!(_wcsicmp(dll->GetName(), L"AveScripter") == 0 || 
			_wcsicmp(dll->GetName(), L"SysStats") == 0)
		  )
		{
			m_Name = dll->GetName();
			m_Name += L": ";
		}

		m_Name += skinInfo->GetName();
		m_FinalDescription = skinInfo->GetAuthor();
		m_FinalDescription += L"\n";
		m_FinalDescription += skinInfo->GetNotes();
	}

	if(dll != NULL && m_Name.length() == 0)
	{
		m_Name = dll->GetName();
		m_FinalDescription = dll->GetAuthor();
		m_FinalDescription += L"\n";
		m_FinalDescription += dll->GetComments();
	}

	if(m_hBmp != NULL)
		DeleteObject(m_hBmp);
}

std::wstring CFoundDeskletEntry::GetRealName()const
{
	if(dll != NULL)
		return dll->GetName();
	else
		return L"";
}

std::wstring CFoundDeskletEntry::GetAuthor()const
{
	if(dll != NULL)
	{
		if(skinInfo != NULL)
			return skinInfo->GetAuthor();
		else
			return std::wstring(dll->GetAuthor());
	}
	else
	{
		return L"";
	}
}

std::wstring CFoundDeskletEntry::GetVersion()const
{
	if(dll != NULL)
	{
		if(skinInfo != NULL)
			return skinInfo->GetVersionAsString();
		else
		{
			std::wstringstream wss;
			wss << dll->GetVersion();
			return wss.str();
		}
	}
	else
	{
		return L"";
	}
}

BOOL CFoundDeskletEntry::AreSmallAndLargePreviewTheSame()const
{
	return 	(requestDataSmall.flags & PREVIEW_REQUEST_SMALL_AND_BIG_SAME) ||
			(requestDataLarge.flags & PREVIEW_REQUEST_SMALL_AND_BIG_SAME) ||
			(smallFileName.length() > 0 && _wcsicmp(smallFileName.c_str(), bigFileName.c_str()) == 0);
}

Docklet* CFoundDeskletEntry::Instantiate(BOOL allowLoadFromStock, BOOL hiddenByDefault, POINT* pt)
{
	Docklet* d = NULL;

	bool dontMakeVisible = hiddenByDefault != FALSE;

	if(NULL == skinInfo)
	{
		if(allowLoadFromStock) // TODO: add code for user setting
		{
			WCHAR dockletFile[MAX_PATH] = {0};
			WCHAR path[MAX_PATH] = {0};
			wcscpy(path, _App->GetAveDeskDataDir().c_str());
			wcscpy(dockletFile,dll->GetFileName());
			lowercaseW(path);
			lowercaseW(dockletFile);
			wcscpy(dockletFile,dockletFile+wcslen(path));

			USES_CONVERSION;
			CString fileName = W2T(dockletFile);
			fileName.Replace(TEXT('\\'),TEXT('_'));
			fileName.Replace(TEXT('/'),TEXT('_'));
	
			WCHAR pathName[MAX_PATH] = {0};
			wcscpy(pathName,_App->GetAveDeskUserPath().c_str());
			PathAppendW(pathName,L"\\Defaults\\");
			PathAppendW(pathName,T2W(const_cast<TCHAR*>(fileName.operator LPCTSTR())));
			PathRenameExtensionW(pathName,L".avedesklet");
			if(FileExistsW(pathName))
			{
				Docklet::LoadFromSingleFile(pathName,&d,!hiddenByDefault);
			}
			else
			{
				if(pt != NULL)
					d = Docklet::LoadDocklet(dll->GetFileName(),NULL,NULL,NULL,dontMakeVisible, *pt);
				else
					d = Docklet::LoadDocklet(dll->GetFileName(),NULL,NULL,NULL,dontMakeVisible);
			}
		}
		else
		{	
			if(pt != NULL)
				d = Docklet::LoadDocklet(dll->GetFileName(),NULL,NULL,NULL,dontMakeVisible, *pt);
			else
				d = Docklet::LoadDocklet(dll->GetFileName(),NULL,NULL,NULL,dontMakeVisible);
		}
	}
	else
	{
		SkinInfoWrapper* skinInfoWrapper = NULL;
		skinInfoWrapper = skinInfo->GetSkinInfoWrapper();
		if(pt != NULL)
			d = Docklet::LoadDocklet(dll->GetFileName(),NULL,enumerator,skinInfoWrapper,dontMakeVisible, *pt);
		else
			d = Docklet::LoadDocklet(dll->GetFileName(),NULL,enumerator,skinInfoWrapper,dontMakeVisible);
		DeleteSkinInfoWrapper(skinInfoWrapper);
	}

	if(d != NULL && dll->GetDocklet() != NULL && !dll->GetDocklet()->SupportBitmapPreviews())
	{
		d->WaitForDockletBeingInitialized(INFINITE);
		CBitmapCache cache;
		BitmapCacheEntry cacheEntry = {0};
		cacheEntry.timestamp.dwLowDateTime = 0x00FF0000;
		cacheEntry.timestamp.dwHighDateTime = 0x00FF0000;
		cacheEntry.dwSize = sizeof(cacheEntry);
		cacheEntry.type = CACHE_TYPE_LARGE_BITMAP;

		if(skinInfo != NULL)
		{
			SkinInfoWrapper* skinInfoWrapper = NULL;
			skinInfoWrapper = skinInfo->GetSkinInfoWrapper();
			wcscpy_s(cacheEntry.name, MAX_CACHENAME, skinInfoWrapper->key);
			wcscat_s(cacheEntry.name, MAX_CACHENAME, skinInfoWrapper->name);
			DeleteSkinInfoWrapper(skinInfoWrapper);
		}
		else
		{
			wcscpy_s(cacheEntry.name, MAX_CACHENAME, dll->GetFileName());
		}

		Bitmap* bmp = d->GetDocklet()->Draw();
		cache.AddToCachePlus(cacheEntry, bmp);		
	}

	return d;
}

BOOL CFoundDeskletEntry::MatchesSearchString(const std::wstring& search, DWORD flags)
{

	if(flags & SEARCH_FLAG_NAME)
	{
		std::wstring ws = dll->GetName();
		std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
		if(wcsstr(ws.c_str(), search.c_str()) != NULL)
			return TRUE;

		if(skinInfo != NULL)
		{
			std::wstring ws = skinInfo->GetName();
			std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
			if(wcsstr(ws.c_str(), search.c_str()) != NULL)
				return TRUE;
		}
	}

	if(flags & SEARCH_FLAG_DESCRIPTION)
	{
		if(skinInfo != NULL)
		{
			std::wstring ws = skinInfo->GetNotes();
			std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
			if(wcsstr(ws.c_str(), search.c_str()) != NULL)
				return TRUE;
		}
		else
		{
			std::wstring ws = dll->GetComments();
			std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
			if(wcsstr(ws.c_str(), search.c_str()) != NULL)
				return TRUE;
		}
	}

	if(flags & SEARCH_FLAG_AUTHOR)
	{
		if(skinInfo != NULL)
		{
			std::wstring ws = skinInfo->GetAuthor();
			std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
			if(wcsstr(ws.c_str(), search.c_str()) != NULL)
				return TRUE;
		}
		else
		{
			std::wstring ws = dll->GetAuthor();
			std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
			if(wcsstr(ws.c_str(), search.c_str()) != NULL)
				return TRUE;
		}
	}

	return FALSE;
}

CFoundDeskletEntry::~CFoundDeskletEntry()
{
	StopCreatingPreviewBitmap(INFINITE);
	StopCreatingLargePreviewBitmap(INFINITE);

	delete skinInfo;
	skinInfo = NULL;

	if(enumerator)
		enumerator->Release();
	enumerator = NULL;

	if(dll != NULL)
		dll->Release();

	dll = NULL;

	::DeleteObject(m_hBmp);
	::DeleteObject(m_hBmpLarge);
}

#define TRYIMAGE(N) \
	if(NULL == bmp && !PathFileExistsW(fname)) \
	{ \
		wcscpy(fname, basePath); \
		PathAppendW(fname, N); \
	} 

DWORD CFoundDeskletEntry::OnCreateLargePreview()
{
	if(m_hBmpLarge != NULL)
		DeleteObject(m_hBmpLarge);

	if(dll != NULL && enumerator != NULL)
	{
		docklets::OdDocklet* docklet = dll->GetDocklet();
		if(docklet != NULL)
		{
			SkinInfoWrapper* skinInfoWrapper = NULL;
			if(skinInfo != NULL)
				skinInfoWrapper = skinInfo->GetSkinInfoWrapper();

			BitmapCacheEntry cacheEntry = {0};
			cacheEntry.dwSize = sizeof(cacheEntry);
			cacheEntry.type = CACHE_TYPE_LARGE_BITMAP;

			CBitmapCache cache;
			BOOL inCache = FALSE;
			UINT cacheRes = docklet->GetSkinPreviewBitmapCacheEntry(enumerator->GetPointer(), skinInfoWrapper, &cacheEntry);
			if(AVEDESK_CACHE_ENTRY_FILLED == cacheRes)
				inCache = cache.IsAvailableInCache(cacheEntry);
			else if(AVEDESK_CACHE_NOT_SUPPORTED == cacheRes && !docklet->SupportBitmapPreviews())
			{
				if(skinInfo != NULL)
				{
					wcscpy_s(cacheEntry.name, MAX_CACHENAME, skinInfoWrapper->key);
					wcscat_s(cacheEntry.name, MAX_CACHENAME, skinInfoWrapper->name);
				}
				else
				{
					wcscpy_s(cacheEntry.name, MAX_CACHENAME, dll->GetFileName());
				}
				cacheEntry.timestamp.dwLowDateTime = 0x00FF0000;
				cacheEntry.timestamp.dwHighDateTime = 0x00FF0000;
				inCache = cache.IsAvailableInCache(cacheEntry);
			}

			if(!inCache)
			{
				ZeroMemory(&requestDataLarge, sizeof(requestDataLarge));
				requestDataLarge.dwSize = sizeof(requestDataLarge);
				requestDataLarge.flags = PREVIEW_REQUEST_WANT_LARGE_BITMAP;
				if(Docklet::defaultDocklet != NULL && Docklet::defaultDocklet->GetDocklet() != NULL)
					requestDataLarge.bmpSize = Docklet::defaultDocklet->GetDocklet()->GetSize();
				else
					requestDataLarge.bmpSize.cx = requestDataLarge.bmpSize.cy = 128;

				requestDataLarge.bStop = FALSE;
				bool res = docklet->GetSkinPreviewBitmap(enumerator->GetPointer(), skinInfoWrapper, &requestDataLarge);
				DeleteSkinInfoWrapper(skinInfoWrapper);
				if(res)
				{
					largeBitmapLoadedCorrectly = TRUE;
					m_hBmpLarge = requestDataLarge.hBmp;
					m_LargeSize = requestDataLarge.bmpSize;

					if(AVEDESK_CACHE_ENTRY_FILLED == cacheRes)
						cache.AddToCache(cacheEntry, m_hBmpLarge, m_LargeSize);

					::PostMessageW(m_CallbackHwnd2, m_Msg2, m_lParam, (LPARAM)this);
					return 0;
				}
			}
			else
			{
				largeBitmapLoadedCorrectly = TRUE;

				m_hBmpLarge = cache.GetFromCache(cacheEntry, &m_LargeSize);
				DeleteSkinInfoWrapper(skinInfoWrapper);
				::PostMessageW(m_CallbackHwnd2, m_Msg2, m_lParam, (LPARAM)this);
				return 0;
			}
		}
	}

	Bitmap* bmp = NULL;

	if(dll != NULL)
	{
		WCHAR basePath[MAX_PATH] = {0};
		wcscpy(basePath,dll->GetFileName());
		PathRemoveFileSpecW(basePath);

		if(skinInfo != NULL)
		{
			if(PathFileExistsW(skinInfo->GetKey().c_str()))
			{
				wcscpy(basePath, skinInfo->GetKey().c_str());
				PathRemoveFileSpecW(basePath);
			}
			else
			{
				WCHAR maybePath[MAX_PATH] = {0};
				wcscpy(maybePath,dll->GetFileName());
				PathRemoveFileSpecW(maybePath);
				PathAppendW(maybePath, skinInfo->GetKey().c_str());
				if(PathFileExistsW(maybePath))
					wcscpy(basePath, maybePath);
				else
				{
					WCHAR maybePath[MAX_PATH] = {0};
					wcscpy(maybePath,dll->GetFileName());
					PathRemoveFileSpecW(maybePath);
					PathAppendW(maybePath, L"Skins\\");
					PathAppendW(maybePath, skinInfo->GetKey().c_str());
					if(PathFileExistsW(maybePath))
						wcscpy(basePath, maybePath);
					else
					{
						WCHAR maybePath[MAX_PATH] = {0};
						wcscpy(maybePath,dll->GetFileName());
						PathRemoveFileSpecW(maybePath);
						PathAppendW(maybePath, skinInfo->GetName().c_str());
						if(PathFileExistsW(maybePath))
							wcscpy(basePath, maybePath);
					}
				}
			}
		}

		WCHAR fname[MAX_PATH] = {0};
		wcscpy(fname, basePath);
		PathAppendW(fname, L"largeicon.png");
		TRYIMAGE(L"bg.png")
		TRYIMAGE(L"front.png")
		TRYIMAGE(L"frontside.png")
		TRYIMAGE(L"preview.png")
		TRYIMAGE(L"screenshot.png")
		TRYIMAGE(L"preview.png")
		TRYIMAGE(L"screenshot.jpg")
		TRYIMAGE(L"preview.jpg")
		TRYIMAGE(L"normal.png")
		TRYIMAGE(L"default.png")
		TRYIMAGE(L"images/largeicon.png")
		TRYIMAGE(L"images/bg.png")
		TRYIMAGE(L"images/front.png")
		TRYIMAGE(L"images/frontside.png")
		TRYIMAGE(L"images/normal.png")
		TRYIMAGE(L"images/default.png")

		if(PathFileExistsW(fname))
		{
			bigFileName = fname;
			bmp = new Bitmap(fname);
		}
	}

	if(bmp != NULL)
	{
		largeBitmapLoadedCorrectly = TRUE;
		m_LargeSize.cx = bmp->GetWidth();
		m_LargeSize.cy = bmp->GetHeight();
		bmp->GetHBITMAP(NULL, &m_hBmpLarge);
		delete bmp;
		bmp = NULL;

		if(m_CallbackHwnd2 != NULL)
			::PostMessageW(m_CallbackHwnd2, m_Msg2, m_lParam, (LPARAM)this);
	}
	else
	{
		largeBitmapLoadedCorrectly = FALSE;
	}

	m_TriedLoadingLarge = TRUE;

	return 0;
}

BOOL CFoundDeskletEntry::FailedLoadingLargePreview()
{
	return m_TriedLoadingLarge;
}

DWORD CFoundDeskletEntry::OnCreatePreview()
{
	if(m_hBmp != NULL)
		DeleteObject(m_hBmp);


	if(dll != NULL && enumerator != NULL)
	{
		docklets::OdDocklet* docklet = dll->GetDocklet();
		if(docklet != NULL)
		{
			SkinInfoWrapper* skinInfoWrapper = NULL;
			if(skinInfo != NULL)
				skinInfoWrapper = skinInfo->GetSkinInfoWrapper();

			BitmapCacheEntry cacheEntry = {0};
			cacheEntry.dwSize = sizeof(cacheEntry);
			cacheEntry.type = CACHE_TYPE_SMALL_BITMAP;

			CBitmapCache cache;
			BOOL inCache = FALSE;
			UINT cacheRes = docklet->GetSkinPreviewBitmapCacheEntry(enumerator->GetPointer(), skinInfoWrapper, &cacheEntry);
			if(AVEDESK_CACHE_ENTRY_FILLED == cacheRes)
				inCache = cache.IsAvailableInCache(cacheEntry);
			
			if(!inCache)
			{
				ZeroMemory(&requestDataSmall, sizeof(requestDataSmall));
				requestDataSmall.dwSize = sizeof(requestDataSmall);
				requestDataSmall.flags = PREVIEW_REQUEST_WANT_SMALL_BITMAP;
				requestDataSmall.bmpSize.cx = 80;
				requestDataSmall.bmpSize.cy = 80;
				requestDataSmall.bStop = FALSE;

				bool res = docklet->GetSkinPreviewBitmap(enumerator->GetPointer(), skinInfoWrapper, &requestDataSmall);
				DeleteSkinInfoWrapper(skinInfoWrapper);
				if(res)
				{
					m_hBmp = requestDataSmall.hBmp;
					if(AVEDESK_CACHE_ENTRY_FILLED == cacheRes)
						cache.AddToCache(cacheEntry, m_hBmp, requestDataSmall.bmpSize);

					::PostMessageW(m_CallbackHwnd, m_Msg, m_lParam, (LPARAM)this);
					return 0;
				}
			}
			else
			{
				m_hBmpLarge = cache.GetFromCache(cacheEntry, &m_LargeSize);
				DeleteSkinInfoWrapper(skinInfoWrapper);
				::PostMessageW(m_CallbackHwnd2, m_Msg2, m_lParam, (LPARAM)this);
				return 0;
			}
		}
	}

	Bitmap* bmp = NULL;
	if(dll != NULL)
	{
		WCHAR basePath[MAX_PATH] = {0};
		wcscpy(basePath,dll->GetFileName());
		PathRemoveFileSpecW(basePath);

		if(skinInfo != NULL)
		{
			if(PathFileExistsW(skinInfo->GetKey().c_str()))
			{
				wcscpy(basePath, skinInfo->GetKey().c_str());
				PathRemoveFileSpecW(basePath);
			}
			else
			{
				WCHAR maybePath[MAX_PATH] = {0};
				wcscpy(maybePath,dll->GetFileName());
				PathRemoveFileSpecW(maybePath);
				PathAppendW(maybePath, skinInfo->GetKey().c_str());
				if(PathFileExistsW(maybePath))
					wcscpy(basePath, maybePath);
				else
				{
					WCHAR maybePath[MAX_PATH] = {0};
					wcscpy(maybePath,dll->GetFileName());
					PathRemoveFileSpecW(maybePath);
					PathAppendW(maybePath, L"Skins\\");
					PathAppendW(maybePath, skinInfo->GetKey().c_str());
					if(PathFileExistsW(maybePath))
						wcscpy(basePath, maybePath);
					else
					{
						WCHAR maybePath[MAX_PATH] = {0};
						wcscpy(maybePath,dll->GetFileName());
						PathRemoveFileSpecW(maybePath);
						PathAppendW(maybePath, skinInfo->GetName().c_str());
						if(PathFileExistsW(maybePath))
							wcscpy(basePath, maybePath);
					}
				}
			}
		}

		WCHAR fname[MAX_PATH] = {0};
		wcscpy(fname, basePath);
		PathAppendW(fname, L"icon.png");

		if(!PathFileExistsW(fname) && dll != NULL && 
			dll->GetDocklet() != NULL && !dll->GetDocklet()->SupportBitmapPreviews()
			)
		{
			CBitmapCache cache;
			BitmapCacheEntry cacheEntry = {0};
			cacheEntry.timestamp.dwLowDateTime = 0x00FF0000;
			cacheEntry.timestamp.dwHighDateTime = 0x00FF0000;
			cacheEntry.dwSize = sizeof(cacheEntry);
			cacheEntry.type = CACHE_TYPE_LARGE_BITMAP;

			if(skinInfo != NULL)
			{
				SkinInfoWrapper* skinInfoWrapper = NULL;
				skinInfoWrapper = skinInfo->GetSkinInfoWrapper();
				wcscpy_s(cacheEntry.name, MAX_CACHENAME, skinInfoWrapper->key);
				wcscat_s(cacheEntry.name, MAX_CACHENAME, skinInfoWrapper->name);
				DeleteSkinInfoWrapper(skinInfoWrapper);
			}
			else
			{
				wcscpy_s(cacheEntry.name, MAX_CACHENAME, dll->GetFileName());
			}

			bmp = cache.GetFromCachePlus(cacheEntry);
			if(bmp != NULL)
				requestDataSmall.flags |= PREVIEW_REQUEST_SMALL_AND_BIG_SAME;
		}

		TRYIMAGE(L"bg.png")
		TRYIMAGE(L"preview.png")
		TRYIMAGE(L"screenshot.png")
		TRYIMAGE(L"preview.png")
		TRYIMAGE(L"screenshot.jpg")
		TRYIMAGE(L"preview.jpg")
		TRYIMAGE(L"normal.png")
		TRYIMAGE(L"default.png")
		TRYIMAGE(L"images/bg.png")
		TRYIMAGE(L"images/icon.png")
		TRYIMAGE(L"images/normal.png")
		TRYIMAGE(L"images/default.png")

		if(PathFileExistsW(fname))
		{
			smallFileName = fname;
			bmp = new Bitmap(fname);
		}
	}

	if(bmp != NULL)
	{
		if(bmp->GetWidth() != (UINT)m_BitmapSize.cx || bmp->GetHeight() != (UINT)m_BitmapSize.cy)
		{
			Bitmap* orig = bmp;
			bmp = new Bitmap(m_BitmapSize.cx, m_BitmapSize.cy);
			Graphics g(bmp);
			
			SIZE drawSize = m_BitmapSize;
			SIZE curSize = {orig->GetWidth(), orig->GetHeight()};
			POINT drawPoint = {0,0};
			if(curSize.cx > curSize.cy && curSize.cy != 0)
			{
				float ratio = float(curSize.cx) / float(curSize.cy);
				drawSize.cy = int( float(drawSize.cy) / ratio );
				if(drawSize.cy % 2 == 1)
					drawSize.cy += 1;
				drawPoint.y = (m_BitmapSize.cy - drawSize.cy) / 2;
			}
			else if(curSize.cy > curSize.cx && curSize.cx != 0)
			{
				float ratio = float(curSize.cy) / float(curSize.cx);
				drawSize.cx = int( float(drawSize.cx) / ratio );
				if(drawSize.cx % 2 == 1)
					drawSize.cx += 1;
				drawPoint.x = (m_BitmapSize.cx - drawSize.cx) / 2;
			}

			g.SetSmoothingMode(SmoothingModeAntiAlias);
			g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			g.DrawImage(orig, Rect(drawPoint.x,drawPoint.y, drawSize.cx, drawSize.cy), 0, 0, orig->GetWidth(), orig->GetHeight(), UnitPixel, 0, 0, 0);
			delete orig;
		}

		bmp->GetHBITMAP(NULL, &m_hBmp);
		delete bmp;
		bmp = NULL;
		::PostMessageW(m_CallbackHwnd, m_Msg, m_lParam, (LPARAM)this);
	}

	return 0;
}

DWORD __stdcall CFoundDeskletEntry::ThisThreadFunc(LPVOID lpData)
{
	CFoundDeskletEntry* f = reinterpret_cast<CFoundDeskletEntry*>(lpData);
	if(NULL == f)
		return 0;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	OleInitialize(NULL);

	DWORD dwRes = f->OnCreatePreview();

	CloseHandle(f->m_ThreadHandle);
	f->m_ThreadHandle = NULL;

	CoUninitialize();
	OleUninitialize();
	
	return dwRes;
}

DWORD __stdcall CFoundDeskletEntry::ThisThreadFunc2(LPVOID lpData)
{
	CFoundDeskletEntry* f = reinterpret_cast<CFoundDeskletEntry*>(lpData);
	if(NULL == f)
		return 0;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	OleInitialize(NULL);

	DWORD dwRes = f->OnCreateLargePreview();

	CloseHandle(f->m_ThreadHandle2);
	f->m_ThreadHandle2 = NULL;

	CoUninitialize();
	OleUninitialize();
	
	return dwRes;
}

void CFoundDeskletEntry::StartCreatingLargePreviewBitmap(HWND callbackHwnd, UINT msg)
{
	if(m_hBmpLarge != NULL)
	{
		if(callbackHwnd != NULL)
			::PostMessageW(callbackHwnd, msg, m_lParam, (LPARAM)this);
		return;
	}

	m_Stop2 = FALSE;
	m_CallbackHwnd2 = callbackHwnd;
	m_Msg2 = msg;
	DWORD dummy(0);
	m_ThreadHandle2 = CreateThread(NULL,0,ThisThreadFunc2,reinterpret_cast<LPVOID>(this),0,&dummy);
}

void CFoundDeskletEntry::StopCreatingLargePreviewBitmap(DWORD timeOut)
{
	WaitForLargeBitmapPreview(timeOut);
}

void CFoundDeskletEntry::WaitForLargeBitmapPreview(DWORD timeOut)
{
	if(m_ThreadHandle2 != NULL)
	{
		requestDataLarge.bStop = TRUE;
		if(dll != NULL && enumerator != NULL)
		{
			docklets::OdDocklet* docklet = dll->GetDocklet();
			if(docklet != NULL)
				docklet->StopSkinPreviewBitmap(enumerator->GetPointer(), &requestDataLarge);
		}

		if(m_ThreadHandle2 != NULL)
		WaitForSingleObject(m_ThreadHandle2, timeOut);
	}
}

BOOL CFoundDeskletEntry::GetLargePreview(HBITMAP* hBmp, SIZE* s)
{
	if(hBmp != NULL)
		*hBmp = m_hBmpLarge;

	if(s != NULL)
		*s = m_LargeSize;

	return largeBitmapLoadedCorrectly;
}

void CFoundDeskletEntry::StartCreatingPreviewBitmap(const SIZE& s, HWND callbackHwnd, UINT msg)
{
	m_BitmapSize = s;
	m_Stop = FALSE;
	m_CallbackHwnd = callbackHwnd;
	m_Msg = msg;
	DWORD dummy(0);
	m_ThreadHandle = CreateThread(NULL,0,ThisThreadFunc,reinterpret_cast<LPVOID>(this),0,&dummy);
}

BOOL CFoundDeskletEntry::IsRunning()const
{
	return m_ThreadHandle != NULL;
}

void CFoundDeskletEntry::StopCreatingPreviewBitmap(DWORD timeOut)
{
	if(m_ThreadHandle != NULL)
	{
		m_Stop = TRUE;
		requestDataSmall.bStop = TRUE;
		if(dll != NULL && enumerator != NULL)
		{
			docklets::OdDocklet* docklet = dll->GetDocklet();
			if(docklet != NULL)
				docklet->StopSkinPreviewBitmap(enumerator->GetPointer(), &requestDataSmall);
		}

		if(m_ThreadHandle != NULL)
			WaitForSingleObject(m_ThreadHandle, timeOut);
	}
}

std::wstring CFoundDeskletEntry::GetName()const
{
	return m_Name;
}

std::wstring CFoundDeskletEntry::GetDescription()const
{
	return m_FinalDescription;
}