#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

Bitmap* __stdcall HBitmapToCppGdiPlusBitmap(HBITMAP hBitmap)
{
	if(NULL == hBitmap)
		return NULL;

	Bitmap* bmp = new Bitmap(width, height);

	BitmapData data;
	Rect r(0,0,bmp->GetWidth(),bmp->GetHeight());
	bmp->LockBits(&r,ImageLockModeWrite,PixelFormat32bppARGB,&data);
		
	// copies the colour icon, but not yet masked.
	GetBitmapBits(iconInfo.hBitmap,height * width *4,data.Scan0);

	bmp->UnlockBits(&data);
}