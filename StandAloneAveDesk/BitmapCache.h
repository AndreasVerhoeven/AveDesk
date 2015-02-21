#pragma once

#include "DeskletBitmapCacheEntry.h"
#include <string>

class CBitmapCache
{
protected:
	std::wstring HashFileName(const std::wstring fname);
public:
	CBitmapCache(void);
public:
	~CBitmapCache(void);

	virtual void MakeSureCacheDirExists();
	virtual std::wstring GetCacheFileName(const std::wstring& fname);
	virtual BOOL IsAvailableInCache(const BitmapCacheEntry& entry);
	virtual HBITMAP GetFromCache(const BitmapCacheEntry& entry, SIZE* s);
	virtual Bitmap* GetFromCachePlus(const BitmapCacheEntry& entry);
	virtual void AddToCache(const BitmapCacheEntry& entry, HBITMAP hBmp, SIZE s);
	virtual void AddToCachePlus(const BitmapCacheEntry& entry, Bitmap* bmp);
};
