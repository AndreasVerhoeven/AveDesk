// FlipAnimation.h: interface for the CFlipAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLIPANIMATION_H__747CAD61_5F99_4309_B517_127AF4134E5F__INCLUDED_)
#define AFX_FLIPANIMATION_H__747CAD61_5F99_4309_B517_127AF4134E5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UNICODE
#define _UNICODE

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

#include "helper_funcs.h"

#define D3DFVF_AVEVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct AveVertex
{
    FLOAT x, y, z;
	D3DXVECTOR3 n;
	DWORD color;
	FLOAT u, v;
};

class CFlipAnimation
{

protected:

	SIZE                    optimalSize;    // The optimal size, always a power of 2

	Bitmap*					frontBitmap;    // GDI+ front bitmap
	Bitmap*                 backBitmap;     // GDI+ back bitmap

	IDirect3D9*             direct3D;       // DirectX main variable
	IDirect3DDevice9*       directDevice;   // DirectX device
	IDirect3DVertexBuffer9* vertexBuffer;   // DirectX vertex buffer
	IDirect3DSurface9*      renderTarget;   // DirectX render target
	IDirect3DSurface9*      sysMemSurface;  // DirectX render target
	IDirect3DTexture9*		frontTexture;   // DirectX front texture
	IDirect3DTexture9*      backTexture;    // DirectX back texture

	D3DXMATRIX viewMatrix;		            // DirectX view matrix
	D3DXMATRIX worldMatrix;                 // DirectX world matrix
	D3DXMATRIX projectionMatrix;            // DirectX projection matrix

	bool useFrontBuffer;                    // indicates if the frontbuffer is used currently.
	bool rotationIsZero;					// indicates that the rotation is zero.
	bool reduceRenderSize;                  // indicates if the render size should be reduced to the minimum size.

	void  ReleaseAll();
	void  CalculateOptimalSize();
	float LookupCamDistance();

	void CalculateProjectionMatrix();
	void CalculateViewMatrix();
	void CalculateWorldMatrix();

	HRESULT InitVertexBuffer();
	HRESULT InitTexture(IDirect3DTexture9** texture, Bitmap* bitmap);
	HRESULT InitSysMemSurface();
	HRESULT InitRenderTarget();
	HRESULT PostInitialize();
	HRESULT CreateD3DPresentParameters(HWND hwnd, int windowWidth, int windowHeight, D3DPRESENT_PARAMETERS& params);
	HRESULT SetUpVertices();
	HRESULT Render();
	HRESULT Render2D();
	HRESULT GetBitmapFromRenderTarget(Bitmap** bmp, bool usedFrontBitmap, float rotationUsed);
	HRESULT NextRotation(float radRotation);
	HRESULT ResetTexturePosition();

	static SIZE CalculateOptimalSizeFor(int w, int h);
	HRESULT DoLightning(const D3DXVECTOR3& dir);

public:
	CFlipAnimation(Bitmap* front, Bitmap* back);
	~CFlipAnimation();

	HRESULT Initialize(HWND hwnd);
	HRESULT Uninitialize();

	bool IsInitializedOk()const;

	bool FlipToBack(float rotation, Bitmap** bmp);
	bool FlipToFront(float rotation, Bitmap** bmp);

	HRESULT RenderWithoutRotation(bool useFront, Bitmap** bmp);

	void SetReduceRenderSize(bool val);
	bool GetReduceRenderSize()const;
};

#endif // !defined(AFX_FLIPANIMATION_H__747CAD61_5F99_4309_B517_127AF4134E5F__INCLUDED_)
