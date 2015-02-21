#include "stdafx.h"
#include "BitmapCache.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

#include <boost/functional/hash/hash.hpp>
#include <sstream>

std::wstring CBitmapCache::HashFileName(const std::wstring fname)
{
	boost::hash<std::wstring> string_hash;
	std::size_t h  = string_hash(fname);

	std::wstringstream ws;
	ws << std::hex << h;

	return ws.str();
}

CBitmapCache::CBitmapCache(void)
{
}

CBitmapCache::~CBitmapCache(void)
{
}


std::wstring CBitmapCache::GetCacheFileName(const std::wstring& fname)
{
std::wstring hash(HashFileName(fname));

	WCHAR filename[MAX_PATH] = {0};
	if(_App != NULL)
		wcscpy_s(filename, MAX_PATH,_App->GetAveDeskUserPath().c_str());
	PathAppendW(filename, L"Cache\\Bitmaps");
	PathAppendW(filename, hash.c_str());
	wcscat_s(filename, MAX_PATH, L".bmp");

	return filename;
}

BOOL CBitmapCache::IsAvailableInCache(const BitmapCacheEntry& entry)
{
	std::wstring fname(GetCacheFileName(entry.name));

	if(!PathFileExists(fname.c_str()))
		return FALSE;
	
	HANDLE h = ::CreateFileW(fname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == h)
		return FALSE;

	FILETIME ft;
	::GetFileTime(h, NULL, NULL, &ft);
	LONG res = CompareFileTime(&ft, &entry.timestamp);

	::CloseHandle(h);

	return 0 == res;
}

Bitmap* CBitmapCache::GetFromCachePlus(const BitmapCacheEntry& entry)
{
	std::wstring fname(GetCacheFileName(entry.name));

	if(!PathFileExists(fname.c_str()))
		return NULL;
	
	HANDLE h = ::CreateFileW(fname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == h)
		return NULL;

	FILETIME ft;
	::GetFileTime(h, NULL, NULL, &ft);
	LONG res = ::CompareFileTime(&ft, &entry.timestamp);

	Bitmap* bmp = NULL;
	if(0 == res)
		bmp = new Bitmap(fname.c_str());

	::CloseHandle(h);

	return bmp;
}

HBITMAP CBitmapCache::GetFromCache(const BitmapCacheEntry& entry, SIZE* s)
{
	HBITMAP hBmp = NULL;
	Bitmap* bmp = GetFromCachePlus(entry);
	if(bmp != NULL)
	{
		bmp->GetHBITMAP(NULL, &hBmp);
		if(s != NULL)
		{
			s->cx = bmp->GetWidth();
			s->cy = bmp->GetHeight();
		}
	}

	delete bmp;

	return hBmp;
}

void CBitmapCache::MakeSureCacheDirExists()
{
	WCHAR dirName[MAX_PATH] = {0};
	wcscpy_s(dirName, MAX_PATH,_App->GetAveDeskUserPath().c_str());
	PathAppendW(dirName, L"Cache");
	if(!PathFileExists(dirName))
		::CreateDirectory(dirName, NULL);

	PathAppendW(dirName, L"Bitmaps");
	if(!PathFileExists(dirName))
		::CreateDirectory(dirName, NULL);

}

void CBitmapCache::AddToCachePlus(const BitmapCacheEntry& entry, Bitmap* bmp)
{

	MakeSureCacheDirExists();
	std::wstring fname(GetCacheFileName(entry.name));

	CLSID encoderCLSID = {0};
	GetEncoderClsid(L"image/png", &encoderCLSID);
	bmp->Save(fname.c_str(), &encoderCLSID, NULL);

	HANDLE h = ::CreateFileW(fname.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(h != INVALID_HANDLE_VALUE)
	{
		::SetFileTime(h, NULL, NULL, &entry.timestamp);
		::CloseHandle(h);
	}
}
void CBitmapCache::AddToCache(const BitmapCacheEntry& entry, HBITMAP hBmp, SIZE s)
{

	Bitmap* bmp = new Bitmap(s.cx, s.cy);
	BitmapData data;
	Rect rc(0,0,s.cx, s.cy);
	bmp->LockBits(&rc, ImageLockModeWrite,PixelFormat32bppPARGB, &data);
	GetBitmapBits(hBmp,data.Stride * data.Height, data.Scan0);
	bmp->UnlockBits(&data);

	AddToCachePlus(entry, bmp);
	delete bmp;
}
