#pragma once

#include <windows.h>

// PreviewRequestData flags
const DWORD PREVIEW_REQUEST_WANT_LARGE_BITMAP	= 0x01;	// [in]
const DWORD PREVIEW_REQUEST_WANT_SMALL_BITMAP	= 0x02; // [in]
const DWORD PREVIEW_REQUEST_SMALL_AND_BIG_SAME	= 0x04; // [out]

#pragma pack(push, 1)
struct PreviewBitmapRequestData
{
	DWORD dwSize;				// sizeof(PreviewData) [in]

	DWORD flags;				// the flags for this request [in, out]

	SIZE bmpSize;				// bitmap size of the request [in, out]
	HBITMAP hBmp;				// bitmap handle to be filled [in]

	BOOL bStop;					// TRUE iff the request should be canceled. [in]

	LPARAM lParam;				// token that can be set by the callee. [out] Can be used
								// in calls to OnStopSkinPreviewBitmap() to idenitify the
								// request to stop.
};

#pragma pack(pop)