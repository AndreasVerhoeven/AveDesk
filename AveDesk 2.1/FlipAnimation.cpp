// FlipAnimation.cpp: implementation of the CFlipAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlipAnimation.h"


#include <map> // for lookup table
#include "helper_funcs.h"

CFlipAnimation::CFlipAnimation(Bitmap* front, Bitmap* back) :
	direct3D(NULL),
	directDevice(NULL),
	vertexBuffer(NULL),
	renderTarget(NULL),
	sysMemSurface(NULL),
	frontTexture(NULL),
	backTexture(NULL),
	frontBitmap(front),
	backBitmap(back),
	useFrontBuffer(true),
	rotationIsZero(true),
	reduceRenderSize(false)
{
	// initialize COM and OLE system
	CoInitialize(NULL);
	OleInitialize(NULL);
}

CFlipAnimation::~CFlipAnimation()
{
	if(directDevice != NULL)
		Uninitialize();

	// unitialize COM and OLE system again, since we have initialized it
	// in the c`tor
	CoUninitialize();
	OleUninitialize();
}

bool CFlipAnimation::IsInitializedOk()const
{
	return directDevice != NULL;
}

void CFlipAnimation::SetReduceRenderSize(bool val)
{
	reduceRenderSize = val;
}

bool CFlipAnimation::GetReduceRenderSize()const
{
	return reduceRenderSize;
}

HRESULT CFlipAnimation::Uninitialize()
{
	ReleaseAll();

	return S_OK;
}

void CFlipAnimation::ReleaseAll()
{
	// release all used objects safely
	SafeRelease(direct3D);
	SafeRelease(directDevice);
	SafeRelease(vertexBuffer);
	SafeRelease(renderTarget);
	SafeRelease(sysMemSurface);
	SafeRelease(frontTexture);
	SafeRelease(backTexture);
}

SIZE CFlipAnimation::CalculateOptimalSizeFor(int w, int h)
{
	int optimalWidth = 1;
	int optimalHeight = 1;
	while(w > optimalWidth && optimalWidth > 0)
		optimalWidth *= 2;

	while(h > optimalHeight && optimalHeight > 0)
		optimalHeight *= 2;

	//if(optimalHeight < biggestWidth / 16.0f + biggestHeight)
	//	optimalHeight *= 2;

	if(optimalHeight < h + (h  / 5))
	{
		optimalHeight *= 2;
		//optimalWidth  *= 2;
	}

	SIZE s = {optimalWidth, optimalHeight};
	return s;
}


HRESULT CFlipAnimation::DoLightning(const D3DXVECTOR3& dir)
{
	HRESULT hRes = S_OK;

	 // Fill in a light structure defining our light
	D3DLIGHT9 light = {D3DLIGHT_DIRECTIONAL};
	ZeroMemory(&light, sizeof(light));
	light.Type       = D3DLIGHT_DIRECTIONAL;
	float intensityDiffuse = 0.5f;
	light.Diffuse.r  = intensityDiffuse;
	light.Diffuse.g  = intensityDiffuse;
	light.Diffuse.b  = intensityDiffuse;
	light.Diffuse.a  = intensityDiffuse;
	//..light.Ambient.a = 1.0f;//= light.Specular.b = light.Specular.g = light.Specular.r = 1.0f;
	float intensity = 0.5f;
	light.Ambient.a = intensity;
	light.Ambient.r = intensity;
	light.Ambient.g = intensity;
	light.Ambient.b = intensity;
	light.Range=1000.0f;
	D3DXVECTOR3 negDir = dir;
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &negDir);

	// Tell the device about the light and turn it on
	hRes = directDevice->SetLight( 0, &light );
	hRes = directDevice->LightEnable( 0, TRUE );
	hRes = directDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DMATERIAL9 material = {0};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse.r = material.Ambient.r = 1.0f;
	material.Diffuse.g = material.Ambient.g = 1.0f;
	material.Diffuse.b = material.Ambient.b = 1.0f;
	material.Diffuse.a = material.Ambient.a = 1.0f;

	hRes = directDevice->SetMaterial( &material );

	hRes = directDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	hRes = directDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
//	directDevice->SetRenderState(D3DRS_AMBIENT, 0xFF00FFFF);

	return hRes;
}

void CFlipAnimation::CalculateOptimalSize()
{
	// calculates the optimal (a power of 2) size, for the supplied
	// frontBitmap and backBitmap

	bool backWidthBiggerThanFront = backBitmap->GetWidth() > frontBitmap->GetWidth();
	bool backHeightBiggerThanFront = backBitmap->GetHeight() > frontBitmap->GetHeight();
	UINT biggestWidth  = ((backWidthBiggerThanFront) ? backBitmap->GetWidth() : frontBitmap->GetWidth());
	UINT biggestHeight = ((backHeightBiggerThanFront) ? backBitmap->GetHeight() : frontBitmap->GetHeight());
	
	//bool widthBiggerThanHeight = biggestWidth > biggestHeight;
	//UINT biggest = ((widthBiggerThanHeight) ? biggestWidth : biggestHeight);

	DWORD optimalWidth = 1;
	DWORD optimalHeight = 1;
	while(biggestWidth > optimalWidth && optimalWidth > 0)
		optimalWidth *= 2;

	while(biggestHeight > optimalHeight && optimalHeight > 0)
		optimalHeight *= 2;

	//if(optimalHeight < biggestWidth / 16.0f + biggestHeight)
	//	optimalHeight *= 2;

	if(optimalHeight < biggestHeight + (biggestHeight  / 5))
	{
		optimalHeight *= 2;
		//optimalWidth  *= 2;
	}

	// we always use a square texture
	optimalSize.cx = optimalWidth;
	optimalSize.cy = optimalHeight;
}


HRESULT CFlipAnimation::InitVertexBuffer()
{
	assert(directDevice != NULL);

	if(NULL == directDevice)
		return E_FAIL;

	// four vertices are used, each represent a 
	// corner of an image.
	HRESULT hRes = directDevice->CreateVertexBuffer(
			sizeof(AveVertex) * 4,
			D3DUSAGE_WRITEONLY,
			D3DFVF_AVEVERTEX,
			D3DPOOL_MANAGED,
			&vertexBuffer,
			NULL);

	return hRes;
}

HRESULT CFlipAnimation::InitTexture(IDirect3DTexture9** texture, Bitmap* bitmap)
{
	assert(directDevice != NULL);
	assert(texture != NULL);
	assert(bitmap != NULL);

	if(NULL == directDevice)
		return E_FAIL;

	if(NULL == texture)
		return E_INVALIDARG;

	if(NULL == bitmap)
		return E_INVALIDARG;


	HRESULT hRes = S_OK;

	// create a stream in which we will save the bitmap
	IStream* stream = NULL;
	hRes = CreateStreamOnHGlobal(NULL, TRUE, &stream);
	if(FAILED(hRes))
	{
		return hRes;
	}

	// save the bitmap as a PNG, because that will persist alpha data
	CLSID encoderCLSID = {0};
	GetEncoderClsid(L"image/bmp", &encoderCLSID);
	bitmap->Save(stream, &encoderCLSID, NULL);


	// get the hglobal for this stream
	HGLOBAL hGlobal = NULL;
	hRes = GetHGlobalFromStream(stream, &hGlobal);
	if(FAILED(hRes))
	{
		SafeRelease(stream);
		return hRes;
	}

	// create a texture from the in-memory saved bitmap
	void* memPtr = GlobalLock(hGlobal);
	DWORD memSize = (DWORD)GlobalSize(hGlobal);
	hRes = D3DXCreateTextureFromFileInMemoryEx(directDevice, memPtr, memSize, 0, 0, 1, 0,
                            D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,     D3DTEXF_POINT,
                              D3DTEXF_NONE , 0, NULL, NULL, texture);


	// release the stream, and thus release the memory used to save the bitmap in
	GlobalUnlock(hGlobal);
	SafeRelease(stream);

	return hRes;
}

HRESULT CFlipAnimation::InitSysMemSurface()
{
	assert(directDevice != NULL);

	if(NULL == directDevice)
		return E_FAIL;

	HRESULT hRes = S_OK;

	// create a ARGB (alphablended) system memory surface which 
	// will be used to quickly transfer the offscreen render target
	// to the system memory, so we can more efficiently copy
	// the rendertarget to a bitmap
	hRes = directDevice->CreateOffscreenPlainSurface(
			optimalSize.cx, optimalSize.cy,
			D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM,
			&sysMemSurface, NULL);

	if(FAILED(hRes))
	{
		return hRes;
	}

	return S_OK;
}

HRESULT CFlipAnimation::InitRenderTarget()
{
	assert(directDevice != NULL);

	if(NULL == directDevice)
		return E_FAIL;

	HRESULT hRes = S_OK;

	// create an offscreen render target whereto the animation
	// will be 'written' by directx
	hRes = directDevice->CreateRenderTarget
			(optimalSize.cx, optimalSize.cy,
				D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE  , 0, TRUE, &renderTarget, NULL);
	if(FAILED(hRes))
	{
		return hRes;
	}

	// set the rendertarget we created as the main rendertarget
	hRes = directDevice->SetRenderTarget(0, renderTarget);
	if(FAILED(hRes))
	{
		SafeRelease(renderTarget);
		return hRes;
	}

	return S_OK;
}

float CFlipAnimation::LookupCamDistance()
{
	// lookup table for 30degrees
	//  see, and http://www.projects.ex.ac.uk/trol/scol/calpyr.htm with d/f (180-deg)/2 degrees
	std::map<UINT, float> distances;
	distances[1]    = 1.8660254f;
	distances[2]    = 3.73205081f;
	distances[4]    = 7.46410162f;
	distances[8]    = 14.9282032f;
	distances[16]   = 29.8564065f;
	distances[32]   = 59.7128129f;
	distances[64]   = 119.425626f;
	distances[128]  = 238.851252f;
	distances[256]  = 477.702503f;
	distances[512]  = 955.405007f;
	distances[1024] = 1910.81001f;

	UINT x = optimalSize.cx;
	IDirect3DTexture9* texture = frontTexture;
	if(!useFrontBuffer)
		texture = backTexture;

	IDirect3DSurface9* surface = NULL;
	texture->GetSurfaceLevel(0, &surface);
	if(surface != NULL)
	{
		D3DSURFACE_DESC desc;
		surface->GetDesc(&desc);
		x = desc.Width;
		surface->Release();	
	}

	return distances[optimalSize.cx];
}

void CFlipAnimation::CalculateProjectionMatrix()
{
	D3DXMatrixIdentity(&projectionMatrix);
	float zNearPlane = 1.0f;
	float zFarPlane  = 0.0f ;
	float aspect = 1.0f;
	float camDistance = LookupCamDistance();
	float camAngle = DegreeToRad(30.0f);
	D3DXMatrixPerspectiveFovRH(&projectionMatrix, camAngle, aspect, zNearPlane, zFarPlane);
}

void CFlipAnimation::CalculateViewMatrix()
{
	// set up view matrix
	float camDistance = LookupCamDistance();

	D3DXMatrixIdentity(&viewMatrix);
	D3DXVECTOR3 eyeVec(0.0f, 0.0f, camDistance);
	D3DXVECTOR3 lookAtVec(0.0f ,0.0f, 0.0f);
	D3DXVECTOR3 camUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtRH(&viewMatrix, &eyeVec, &lookAtVec, &camUpVec);
	DoLightning(eyeVec);

	UINT w = frontBitmap->GetWidth();
	UINT h = frontBitmap->GetHeight();
	IDirect3DTexture9* texture = frontTexture;
	if(!useFrontBuffer)
	{
		texture = backTexture;
		w = backBitmap->GetWidth();
		h = backBitmap->GetHeight();
	}

	SIZE texSize = CalculateOptimalSizeFor(w,h);
	SIZE s = optimalSize;

	IDirect3DSurface9* surface = NULL;
	texture->GetSurfaceLevel(0, &surface);
	if(surface != NULL)
	{
		D3DSURFACE_DESC desc;
		surface->GetDesc(&desc);
		s.cx = desc.Width;
		s.cy = desc.Height;
		surface->Release();	
	}

	float xMul = 1.0f;
	float yMul = 1.0f;
	if(optimalSize.cx != optimalSize.cy)
		yMul = float(optimalSize.cx)  / float(optimalSize.cy);

	if(optimalSize.cy != texSize.cy)
		yMul *= float(texSize.cy)  / float(optimalSize.cy);

	D3DXMATRIX scalingMatrix;
	D3DXMatrixIdentity(&scalingMatrix);
	float xScaling(static_cast<float>(optimalSize.cx)  / static_cast<float>(texSize.cx) * xMul);
	float yScaling(static_cast<float>(optimalSize.cy) / static_cast<float>(texSize.cy) * yMul);
	D3DXMatrixScaling(&scalingMatrix, xScaling, yScaling, 1.0f);

	viewMatrix = viewMatrix * scalingMatrix;
}

void CFlipAnimation::CalculateWorldMatrix()
{
	D3DXMatrixIdentity(&worldMatrix);

}

HRESULT CFlipAnimation::PostInitialize()
{
	assert(directDevice != NULL);
	
	if(NULL == directDevice)
		return E_FAIL;

	CalculateProjectionMatrix();
	CalculateViewMatrix();
	CalculateWorldMatrix();
	
	HRESULT hRes = S_OK;

	hRes = directDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = directDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
		if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = directDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
		if(FAILED(hRes))
	{
		return hRes;
	}

//	hRes = directDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//	if(FAILED(hRes))
//	{
//		return hRes;
//	}

	hRes = directDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  TRUE);
	if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = directDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = directDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = directDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//hRes = directDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);



	return S_OK;
}

HRESULT CFlipAnimation::SetUpVertices()
{
	UINT w = frontBitmap->GetWidth();
	UINT h = frontBitmap->GetHeight();
	IDirect3DTexture9* texture = frontTexture;
	if(!useFrontBuffer)
	{
		texture = backTexture;
		w = backBitmap->GetWidth();
		h = backBitmap->GetHeight();
	}

	SIZE s = optimalSize;

	IDirect3DSurface9* surface = NULL;
	texture->GetSurfaceLevel(0, &surface);
	if(surface != NULL)
	{
		D3DSURFACE_DESC desc;
		surface->GetDesc(&desc);
		s.cx = desc.Width;
		s.cy = desc.Height;
		surface->Release();	
	}

	float panelWidth  = static_cast<float>(w);
	float panelHeight = static_cast<float>(h);

	assert(vertexBuffer != NULL);

	if(NULL == vertexBuffer)
		return E_FAIL;

	HRESULT hRes = S_OK;
	AveVertex* vertices = NULL;

	hRes = vertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&vertices), 0);
	if(FAILED(hRes))
	{
		return hRes;
	}

	vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = 0xffffffff;


	float reposX = 0.5f;
	float reposY = -0.5f;
	if(w % 2 != 0)
		reposX = 0.0f;
	if(h % 2 != 0)
		reposY = 0.0f;

	float z = 0.0f;

	float texW = static_cast<float>(w) / static_cast<float>(s.cx);
	float texH = static_cast<float>(h) / static_cast<float>(s.cy);

	// topleft
	vertices[0].x = - panelWidth / 2.0f - reposX;
	vertices[0].y = panelHeight / 2.0f - reposY;
	vertices[0].z = z;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	// topright
	vertices[1].x = panelWidth / 2.0f - reposX;
	vertices[1].y = panelHeight / 2.0f - reposY;
	vertices[1].z = z;
	vertices[1].u = texW;
	vertices[1].v = 0.0f;

	// bottomright
	vertices[2].x = panelWidth / 2.0f - reposX;
	vertices[2].y = -panelHeight / 2.0f - reposY;
	vertices[2].z = z;
	vertices[2].u = texW;
	vertices[2].v = texH;

	// bottomleft
	vertices[3].x = - panelWidth / 2.0f - reposX;
	vertices[3].y = - panelHeight / 2.0f - reposY;
	vertices[3].z = z;
	vertices[3].u = 0.0f;
	vertices[3].v = texH;

	vertices[0].n = vertices[1].n = vertices[2].n = vertices[3].n =  D3DXVECTOR3(0,0,-1);

	hRes = vertexBuffer->Unlock();
	if(FAILED(hRes))
	{
		return hRes;
	}

	return S_OK;
}

HRESULT CFlipAnimation::CreateD3DPresentParameters(HWND hwnd, int windowWidth, int windowHeight, D3DPRESENT_PARAMETERS& params)
{
	// get displaymode for the current adapter
	D3DDISPLAYMODE displayMode = {0};
	HRESULT hRes = direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
	if(FAILED(hRes))
		return hRes;

	params.Windowed               = TRUE;
	params.BackBufferCount        = 1L;
	params.EnableAutoDepthStencil = TRUE;
	params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	params.BackBufferFormat       = displayMode.Format;
	params.BackBufferWidth        = windowWidth;
	params.BackBufferHeight       = windowHeight;
	params.AutoDepthStencilFormat = D3DFMT_D16;
	params.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	params.MultiSampleType        = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality     = 0L;

	return S_OK;
}

HRESULT CFlipAnimation::NextRotation(float radRotation)
{
	HRESULT hRes = S_OK;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixIdentity(&rotationMatrix);
	D3DXMatrixRotationY(&rotationMatrix, radRotation);

	D3DXMATRIX finalWorldMatrix = worldMatrix * rotationMatrix;
	hRes = directDevice->SetTransform(D3DTS_WORLD, &finalWorldMatrix);
	if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = Render();

	return hRes;
}

HRESULT CFlipAnimation::RenderWithoutRotation(bool useFront, Bitmap** bmp)
{
	assert(bmp != NULL);

	if(NULL == bmp)
		return E_INVALIDARG;

	*bmp = NULL;

	HRESULT hRes = S_OK;
	bool oldUseFrontBuffer = useFrontBuffer;

	useFrontBuffer = useFront;
	hRes = NextRotation(0.0f);
	if(SUCCEEDED(hRes))
		hRes = GetBitmapFromRenderTarget(bmp, useFront, 0.0f);

	useFrontBuffer = oldUseFrontBuffer;

	return hRes;
}

HRESULT CFlipAnimation::ResetTexturePosition()
{
	HRESULT hRes = S_OK;
	if(frontBitmap->GetWidth() != backBitmap->GetWidth() ||
		frontBitmap->GetHeight() != backBitmap->GetHeight())
	{
		CalculateWorldMatrix();
		CalculateViewMatrix();
		CalculateProjectionMatrix();

		hRes = SetUpVertices();
		hRes = directDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
		hRes = directDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
		hRes = directDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
	}

	return hRes;
}

bool CFlipAnimation::FlipToBack(float rotation, Bitmap** bmp)
{
	HRESULT hRes = S_OK;

	if(rotation > 180)
		rotation = 180;
	else if(rotation < 0)
		rotation  = 0;

	rotationIsZero = rotation == 0 || rotation == 180;

	if(!useFrontBuffer && rotation <= 90)
	{
		useFrontBuffer = true;
		hRes = ResetTexturePosition();
	}

	if(rotation > 90 && useFrontBuffer)
	{
		useFrontBuffer = false;
		hRes = ResetTexturePosition();
	}

	float workRotation = rotation;
	if(workRotation < 90)
	{
		workRotation = 360 - workRotation;
	}
	else
	{
		workRotation = 180 - workRotation;
	}

	hRes =  NextRotation(DegreeToRad(static_cast<float>(workRotation)));
	hRes = GetBitmapFromRenderTarget(bmp, useFrontBuffer, rotation);

	return rotation >= 180;
}


bool CFlipAnimation::FlipToFront(float rotation, Bitmap** bmp)
{
	HRESULT hRes = S_OK;

	if(rotation > 180)
		rotation = 180;
	else if(rotation < 0)
		rotation  = 0;

	rotationIsZero = rotation == 0 || rotation == 180;

	if(useFrontBuffer && rotation <= 90)
	{
		useFrontBuffer = false;
		hRes = ResetTexturePosition();
	}


	if(rotation > 90 && !useFrontBuffer)
	{
		useFrontBuffer = true;
		hRes = ResetTexturePosition();
	}

	float workRotation = rotation;
	if(workRotation > 90)
	{
		//workRotation = 180 - (workRotation - 90);
		workRotation = 270 + (workRotation - 90);
	}

	hRes =  NextRotation(DegreeToRad(static_cast<float>(workRotation)));
	hRes = GetBitmapFromRenderTarget(bmp, useFrontBuffer, rotation);

	return rotation >= 180;
}

HRESULT CFlipAnimation::GetBitmapFromRenderTarget(Bitmap** bmp, bool usedFrontBitmap, float rotationUsed)
{
	assert(bmp != NULL);
	assert(renderTarget != NULL);
	assert(sysMemSurface != NULL);

	if(NULL == bmp)
		return E_INVALIDARG;

	if(NULL == renderTarget)
		return E_FAIL;

	if(NULL == sysMemSurface)
		return E_FAIL;

	*bmp = NULL;

	HRESULT hRes = S_OK;

	UINT imageWidth  = frontBitmap->GetWidth();
	UINT imageHeight = frontBitmap->GetHeight();

	if(!usedFrontBitmap)
	{
		imageWidth  = backBitmap->GetWidth();
		imageHeight = backBitmap->GetHeight();
	}

	float cosOfRotation = cos(DegreeToRad(rotationUsed));
	float widthUsed = abs(static_cast<float>(imageWidth) * cosOfRotation);


	float xCentre = static_cast<float>(optimalSize.cx) / 2.0f;
	float yCentre = static_cast<float>(optimalSize.cy) / 2.0f;

	RECT rectToLock = {0, 0, optimalSize.cx, optimalSize.cy};
	//RECT rectToLock = { static_cast<int>(xCentre - widthUsed / 2.0f), 0, static_cast<int>(xCentre + widthUsed / 2.0f), optimalSize.cy};
	D3DLOCKED_RECT lockedRect = {0};

	hRes = directDevice->GetRenderTargetData(renderTarget, sysMemSurface);
	if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = sysMemSurface->LockRect(&lockedRect, &rectToLock, D3DLOCK_READONLY);
	if(FAILED(hRes))
	{
		return hRes;
	}

	BitmapData data;
	Bitmap* renderBitmap = new Bitmap(optimalSize.cx, optimalSize.cy);
	Rect r(0, 0, renderBitmap->GetWidth(), renderBitmap->GetHeight());
	renderBitmap->LockBits(&r,ImageLockModeWrite, PixelFormat32bppARGB,&data);
	CopyMemory(reinterpret_cast<void*>(data.Scan0),reinterpret_cast<void*>(lockedRect.pBits),lockedRect.Pitch * data.Height);

	hRes = sysMemSurface->UnlockRect();

	renderBitmap->UnlockBits(&data);
	
	if(FAILED(hRes))
	{
		delete renderBitmap;
		renderBitmap = NULL;
	}

	
	if(reduceRenderSize)
	{
		Bitmap* finalBitmap = new Bitmap(static_cast<UINT>(widthUsed), optimalSize.cy);
		Graphics g(finalBitmap);
	
		UINT srcX = static_cast<UINT>(xCentre - widthUsed / 2.0f);
		UINT srcY = 0;
		g.DrawImage(renderBitmap, 0, 0, srcX, srcY, finalBitmap->GetWidth(), finalBitmap->GetHeight(), UnitPixel);
	
		delete renderBitmap;
		renderBitmap = finalBitmap;
	}

	*bmp = renderBitmap;

	return hRes;
}

HRESULT CFlipAnimation::Render2D()
{
	assert(directDevice != NULL);

    if(NULL == directDevice)
		return E_FAIL;

	assert(vertexBuffer != NULL);

	if(NULL == vertexBuffer)
		return E_FAIL;

	HRESULT hRes = S_OK;

	// if there is no rotation, we use a different filtering
	// for better detail.
	if(!rotationIsZero)
	{
		directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //D3DTEXF_ANISOTROPIC
		directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  //D3DTEXF_ANISOTROPIC
		directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}
	else
	{
		directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}

	float camDistance = LookupCamDistance();
	D3DXVECTOR3 eyeVec(0.0f, 0.0f, camDistance);
	DoLightning(eyeVec);


	// feed the vertexbuffer into the device
	hRes = directDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(AveVertex));
	if(FAILED(hRes))
	{
		return hRes;
	}

	// give the device our vertex format
	directDevice->SetFVF(D3DFVF_AVEVERTEX);
	if(FAILED(hRes))
	{
		return hRes;
	}

	// don't use mipmaps for rendering -> increased quality!
	hRes = directDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, (DWORD)-1);
	if(FAILED(hRes))
	{
		return hRes;
	}

	// set the texture to draw
	IDirect3DTexture9* texture = frontTexture;
	if(!useFrontBuffer)
		texture = backTexture;
	hRes = directDevice->SetTexture(0,texture);
	if(FAILED(hRes))
	{
		return hRes;
	}

	// don't use mipmaps for rendering -> increased quality!
	hRes = directDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, (DWORD)-1);
	if(FAILED(hRes))
	{
		return hRes;
	}

	// draw the texture onto the trainglefan
	hRes = directDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	if(FAILED(hRes))
	{
		return hRes;
	}

	return S_OK;
}


HRESULT CFlipAnimation::Render()
{
	assert(directDevice != NULL);

    if(NULL == directDevice)
		return E_FAIL;

	HRESULT hRes = S_OK;

    // Clear the backbuffer
    hRes = directDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0 );
	if(FAILED(hRes))
	{
		return hRes;
	}
        
    // Begin the scene
    hRes = directDevice->BeginScene();
	if(FAILED(hRes))
	{
		return hRes;
	}
    
    HRESULT render2dHRes = Render2D();
	// we dont do nothing if it fails, because we need to cleanup the other stuff
    
    // End the scene
    hRes = directDevice->EndScene();
	if(FAILED(hRes))
	{
		return hRes;
	}
    
    
    // Present the backbuffer contents to the display
    hRes = directDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hRes))
	{
		return hRes;
	} 

	return render2dHRes;
}

HRESULT CFlipAnimation::Initialize(HWND hwnd)
{	
	HRESULT hRes = S_OK;

	try
	{
		//backBitmap = frontBitmap;

		// A backBitmap and a frontBitmap are mandatory
		if(NULL == backBitmap)
			throw E_FAIL;

		if(NULL == frontBitmap)
			throw E_FAIL;

		if(!IsWindow(hwnd))
			throw E_FAIL;

		// calculate the optimal size for this animation
		CalculateOptimalSize();
	
		// create direct3D object
		direct3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(NULL == direct3D)
			throw E_FAIL;

		// create the parameters used to set up the direct3d device
		D3DPRESENT_PARAMETERS params = {0};
		hRes = CreateD3DPresentParameters(hwnd, optimalSize.cx, optimalSize.cy, params);
		CheckHRES(hRes);

		DWORD behaviourFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
		D3DDEVTYPE ref = D3DDEVTYPE_HAL;
		D3DCAPS9 caps;
		if(SUCCEEDED(direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, ref, &caps)))
		{
			if(caps.DevCaps &  D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			{
				behaviourFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			}
			else
			{
				behaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			}
		}
		else
		{
			ref = D3DDEVTYPE_REF;
			behaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		// create a direct3d device
		hRes = direct3D->CreateDevice(D3DADAPTER_DEFAULT, ref, hwnd, behaviourFlags, &params, &directDevice);
		CheckHRES(hRes);

		// create the vertexbuffer that will hold the vertices for this animation.
		hRes = InitVertexBuffer();
		CheckHRES(hRes);

		// initialize the back texture that will hold the image of the backBitmap
		// on a texture
		hRes = InitTexture(&backTexture, backBitmap);
		CheckHRES(hRes);

		// initialize the front texture that will hold the image of the frontBitmap
		// on a texture
		hRes = InitTexture(&frontTexture, frontBitmap);
		CheckHRES(hRes);
		//backTexture = frontTexture;
		//backTexture->AddRef();
		//backBitmap = frontBitmap;


		// initialize the render target that will be used as the output
		// surface
		hRes = InitRenderTarget();
		CheckHRES(hRes);

		// initialize the sysmem surface that we will copy the
		// render target to, to make copying to a bitmap faster
		hRes = InitSysMemSurface();
		CheckHRES(hRes);

		// do the post initialize actions
		hRes = PostInitialize();
		CheckHRES(hRes);

		// finally, set up the vertices for the frontBuffer first
		hRes = SetUpVertices();
		CheckHRES(hRes);


	}
	catch(HRESULT& hr)
	{
		ReleaseAll();
		hRes = hr;
	}

	return hRes;
}