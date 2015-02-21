struct AveStream
{
	SIZE_T len;
	SIZE_T used;
	HLOCAL hBuffer;
};

AveStream AveCreateStream(SIZE_T initSize);
void AveFreeStream(AveStream* stream);
BOOL AveWriteStream(AveStream* stream, LPVOID bytes, DWORD numBytes);