#include "stdafx.h"
#include "avestream.h"

AveStream AveCreateStream(SIZE_T initSize)
{
	AveStream stream = {0};
	stream.len = initSize;
	stream.hBuffer = LocalAlloc(LMEM_FIXED, initSize);
	return stream;
}

void AveFreeStream(AveStream* stream)
{
	LocalFree(stream->hBuffer);
	stream->hBuffer = NULL;
	stream->len = 0;
	stream->used = 0;
}

BOOL AveWriteStream(AveStream* stream, LPVOID bytes, DWORD numBytes)
{
	if(NULL == stream)
		return FALSE;

	if(NULL == stream->hBuffer)
		return FALSE;

	if(NULL == bytes)
		return FALSE;

	if(stream->used + numBytes > stream->len)
	{
		SIZE_T newLen = stream->used + numBytes;
		HLOCAL newBuf = LocalReAlloc(stream->hBuffer, newLen, 0);
		if(newBuf == NULL)
			return FALSE;

		stream->hBuffer = newBuf;
	}

	memcpy(((BYTE*)stream->hBuffer)+stream->used, bytes, numBytes);
	stream->used += numBytes;

	return TRUE;
}