#pragma once

#include <windows.h>

// v2.0: skin preview bitmap generation constants
// cache data types
// cache data types
const DWORD CACHE_TYPE_LARGE_BITMAP		= 0x01;
const DWORD CACHE_TYPE_SMALL_BITMAP		= 0x02;

//  OnGetSkinPreviewBitmapCache() return values
const UINT AVEDESK_CACHE_ENTRY_FILLED	= 0x01;	// return when you filled in the cache entry
const UINT AVEDESK_CACHE_NOT_SUPPORTED	= 0x02; // return when your desklet does not support caching

#pragma pack(push, 1)
const size_t MAX_CACHENAME = MAX_PATH + 100;
struct BitmapCacheEntry
{
	DWORD dwSize;				// sizeof(CacheData)

	DWORD	type;				// the type of this cache entry

	WCHAR name[MAX_CACHENAME];	// name of this cache entry
	FILETIME timestamp;			// time of this cache entry

};
#pragma pack(pop)