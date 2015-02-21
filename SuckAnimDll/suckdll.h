// SuckyAnimation.h: interface for the CSuckyAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUCKYANIMATION_H__FF077D81_E5D0_45ED_A4D0_D8B927032555__INCLUDED_)
#define AFX_SUCKYANIMATION_H__FF077D81_E5D0_45ED_A4D0_D8B927032555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define UNICODE
//#define _UNICODE

// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// DirectX Header Files
#include <d3dx9.h>
#include <dxerr9.h>
#include <d3dx9core.h>
#include <d3dx9tex.h>

// OLE Header Files
#include <ole2.h>

// C++ Header Files
#include <cassert>

// GDI+ Header Files
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"ole32.lib")

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	// SOURCE: MSDN

   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

template<typename T> inline void SafeRelease(T*& t)
{
	if(t != NULL)
	{
		t->Release();
		t = NULL;
	}
}

inline void CheckHRES(HRESULT& hRes)
{
	if(FAILED(hRes))
		throw hRes;
}

inline float DegreeToRad(float deg)
{
	return deg * D3DX_PI / 180.0f;
}

#define D3DFVF_AVEVERTEX2 (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)


class CSuckyAnimation
{
	struct PositionVertex
	{
	    FLOAT x, y, z;
		FLOAT u, v;
	};

protected:

	SIZE                    optimalSize;    // The optimal size, always a power of 2

	Bitmap*					bitmap;		   // GDI+ front bitmap

	IDirect3D9*             direct3D;       // DirectX main variable
	IDirect3DDevice9*       directDevice;   // DirectX device
	IDirect3DSurface9*      renderTarget;   // DirectX render target
	IDirect3DSurface9*      sysMemSurface;  // DirectX render target
	IDirect3DTexture9*		texture;        // DirectX front texture
	ID3DXMesh*              mesh;           // DirectX mesh

	D3DXMATRIX viewMatrix;		            // DirectX view matrix
	D3DXMATRIX worldMatrix;                 // DirectX world matrix
	D3DXMATRIX projectionMatrix;            // DirectX projection matrix
	HWND hwnd;
	
	bool reduceRenderSize;

	D3DXVECTOR3 vanishPoint;
	float farDistance;
	float speed;
	float firstFarDistance;

	D3DXVECTOR3 Transform(const D3DXVECTOR3& point, float offsetX, float offsetY);
	D3DXVECTOR3 DeTransform(const D3DXVECTOR3& point, float offsetX, float offsetY);
	float GetDistance(const D3DXVECTOR3& pointA, const D3DXVECTOR3& pointB);
	float GetNewValue(float vanishPoint, float current, float pullForce);


	void  ReleaseAll();
	void  CalculateOptimalSize();
	float LookupCamDistance();

	HRESULT InitMesh();
	HRESULT InitTexture(IDirect3DTexture9** texture, Bitmap* bitmap);
	HRESULT InitSysMemSurface();
	HRESULT InitRenderTarget();
	HRESULT PostInitialize();
	HRESULT CreateD3DPresentParameters(HWND hwnd, int windowWidth, int windowHeight, D3DPRESENT_PARAMETERS& params);
	HRESULT Render();
	HRESULT Render2D();
	HRESULT GetBitmapFromRenderTarget(Bitmap** bmp);
	HRESULT DrawMesh();

public:
	CSuckyAnimation(Bitmap* bmp);
	~CSuckyAnimation();

	HRESULT Initialize(HWND hwnd);
	HRESULT Uninitialize();

	bool IsInitializedOk()const;

	bool Next(Bitmap** bmp);

	void SetVanishPoint(const POINT& p);
	void SetSpeed(float v);

	void SetReduceRenderSize(bool val);
	bool GetReduceRenderSize()const;

	float GetProgressFactor()const;
};


// HANDLE* __stdcall AveCreateSuck(HWND hwnd, HBITMAP hbmp, POINT vanishPoint, REAL speed)
//BOOL __stdcall AveFreeSuck(HANDLE h);
// BOOL __stdcall AveNextSuckStep(HANDLE h, HBITMAP* hBmp, SIZE* s, BYTE* alpha);
//
//

struct SUCKDATA
{
	CSuckyAnimation* suck;
	Bitmap* bmp;
};

SUCKDATA* __stdcall AveCreateSuck(HWND hwnd, HBITMAP hbmp, POINT vanishPoint, REAL speed)
{
	SUCKDATA* data = new SUCKDATA;
	data->suck = NULL;
	data->bmp = NULL;

	data->bmp = Bitmap::FromHBITMAP(hbmp, NULL);

	data->suck = new CSuckyAnimation(data->bmp);
	data->suck->Initialize(hwnd);
	data->suck->SetVanishPoint(vanishPoint);
	data->suck->SetSpeed(speed);
	if(!data->suck->IsInitializedOk())
	{
		delete data->suck;
		delete data;
		data = NULL;
	}

	return data;
}

BOOL __stdcall AveNextSuckStep(SUCKDATA* data, HBITMAP* hBmp, SIZE* s, BYTE* alpha)
{
	if(!data)
		return false;

	Bitmap* bmp = NULL;
	bool done = data->suck->Next(&bmp);

	float progressFactor = data->suck->GetProgressFactor();
	if(progressFactor > 1.0f)
		progressFactor = 1.0f;

	BYTE bAlpha = 255;
	if(alpha)
		bAlpha = *alpha;

	bAlpha = static_cast<BYTE>(static_cast<float>(bAlpha)* progressFactor);
	if(alpha)
		*alpha = bAlpha;

	if(bmp)
	{
		if(s)
		{
			s->cx = bmp->GetWidth();
			s->cy = bmp->GetHeight();
		}

		if(hBmp)
			bmp->GetHBITMAP(NULL, hBmp);
	}

	return done ? TRUE : FALSE;
}

BOOL __stdcall AveFreeSuck(SUCKDATA* data)
{
	if(!data)
		return FALSE;

	if(data->suck)
	{
		data->suck->Uninitialize();
		delete data->suck;
	}

	if(data->bmp)
		delete data->bmp;

	delete data;
	return TRUE;
}

#endif // !defined(AFX_SUCKYANIMATION_H__FF077D81_E5D0_45ED_A4D0_D8B927032555__INCLUDED_)
