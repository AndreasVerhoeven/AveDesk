// SuckyAnimation.cpp: implementation of the CSuckyAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuckyAnimation.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//#include "vld.h"

#include <map> // for lookup table

#include "helper_funcs.h"

CSuckyAnimation::CSuckyAnimation(Bitmap* bmp) :
	direct3D(NULL),
	directDevice(NULL),
	renderTarget(NULL),
	sysMemSurface(NULL),
	texture(NULL),
	mesh(NULL),
	bitmap(bmp),
	reduceRenderSize(false),
	farDistance(0.0f),
	firstFarDistance(0.0f),
	speed(0.01f),
	hwnd(NULL)
{
	// initialize COM and OLE system
	CoInitialize(NULL);
	OleInitialize(NULL);
}

CSuckyAnimation::~CSuckyAnimation()
{
	if(directDevice != NULL)
		Uninitialize();

	// unitialize COM and OLE system again, since we have initialized it
	// in the c`tor
	CoUninitialize();
	OleUninitialize();
}

void CSuckyAnimation::SetSpeed(float v)
{
	speed = v;
	if(speed < 0)
		speed *= -1.0f;
}

bool CSuckyAnimation::IsInitializedOk()const
{
	return directDevice != NULL;
}

void CSuckyAnimation::SetReduceRenderSize(bool val)
{
	reduceRenderSize = val;
}

bool CSuckyAnimation::GetReduceRenderSize()const
{
	return reduceRenderSize;
}

HRESULT CSuckyAnimation::Uninitialize()
{
	ReleaseAll();

	return S_OK;
}

void CSuckyAnimation::ReleaseAll()
{
	// release all used objects safely
	SafeRelease(direct3D);
	SafeRelease(directDevice);
	SafeRelease(renderTarget);
	SafeRelease(sysMemSurface);
	SafeRelease(texture);
	SafeRelease(mesh);
}

void CSuckyAnimation::CalculateOptimalSize()
{
	// calculates the optimal (a power of 2) size, for the supplied
	// bitmap

	UINT biggestWidth  = bitmap->GetWidth();
	UINT biggestHeight = bitmap->GetHeight();
	

	DWORD optimalWidth = 1;
	DWORD optimalHeight = 1;
	while(biggestWidth > optimalWidth && optimalWidth > 0)
		optimalWidth *= 2;

	while(biggestHeight > optimalHeight && optimalHeight > 0)
		optimalHeight *= 2;

	optimalSize.cx = optimalWidth;
	optimalSize.cy = optimalHeight;
}


HRESULT CSuckyAnimation::InitTexture(IDirect3DTexture9** texture, Bitmap* bitmap)
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
	GetEncoderClsid(L"image/png", &encoderCLSID);
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

HRESULT CSuckyAnimation::InitSysMemSurface()
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

HRESULT CSuckyAnimation::InitRenderTarget()
{
	assert(directDevice != NULL);

	if(NULL == directDevice)
		return E_FAIL;

	HRESULT hRes = S_OK;

	// create an offscreen render target whereto the animation
	// will be 'written' by directx
	hRes = directDevice->CreateRenderTarget
			(optimalSize.cx, optimalSize.cy,
				D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE , 0, TRUE, &renderTarget, NULL);
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

HRESULT CSuckyAnimation::PostInitialize()
{
	assert(directDevice != NULL);
	
	if(NULL == directDevice)
		return E_FAIL;


	// orthogonal projection, since we only do an animation in the 2d plane
	D3DXMatrixOrthoLH(&projectionMatrix, 1.0f, -1.0f, 0.0f, 1.0f);
	D3DXMatrixIdentity(&viewMatrix);
	D3DXMatrixIdentity(&worldMatrix);

	/*
	D3DXMATRIX Scaling, Translation;

	float xScale = static_cast<float>(bitmap->GetWidth())  / static_cast<float>(optimalSize.cx);
	float yScale = static_cast<float>(bitmap->GetHeight()) / static_cast<float>(optimalSize.cy);
	D3DXMatrixScaling(&Scaling, xScale, yScale, 1.0f);

	float xTranslate = (1.0f - xScale) / 2.0f;
	float yTranslate = (1.0f - yScale) / 2.0f;
	D3DXMatrixTranslation(&Translation, -xTranslate, -yTranslate, 0.0f);

	worldMatrix = Scaling * Translation;
	*/


	
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

	hRes = directDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hRes))
	{
		return hRes;
	}

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

	return S_OK;
}


HRESULT CSuckyAnimation::CreateD3DPresentParameters(HWND hwnd, int windowWidth, int windowHeight, D3DPRESENT_PARAMETERS& params)
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



HRESULT CSuckyAnimation::GetBitmapFromRenderTarget(Bitmap** bmp)
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

	UINT imageWidth  = bitmap->GetWidth();
	UINT imageHeight = bitmap->GetHeight();

	float widthUsed = (float)imageWidth;

	RECT rectToLock = {0, 0, optimalSize.cx, optimalSize.cy};
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
	if(data.Scan0 != NULL)
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
		// do something
	}

	*bmp = renderBitmap;

	return hRes;
}

HRESULT CSuckyAnimation::DrawMesh()
{
	assert(directDevice != NULL);
	assert(mesh != NULL);

	if(NULL == directDevice)
		return E_FAIL;

	if(NULL == mesh)
		return E_FAIL;

	HRESULT hRes = S_OK;

	hRes = directDevice->SetFVF( mesh->GetFVF() );
	if(FAILED(hRes))
	{
		return hRes;
	}

	LPDIRECT3DVERTEXBUFFER9 vb = NULL;
	LPDIRECT3DINDEXBUFFER9  ib = NULL;

	hRes = directDevice->SetRenderState(D3DRS_CULLMODE, FALSE);
	if(FAILED(hRes))
	{
		return hRes;
	}


	hRes = mesh->GetVertexBuffer(&vb);
	if(FAILED(hRes))
	{
		return hRes;
	}

	hRes = mesh->GetIndexBuffer(&ib);
	if(FAILED(hRes))
	{
		vb->Release();
		return hRes;
	}


	hRes = directDevice->SetIndices(ib);
	if(FAILED(hRes))
	{
		vb->Release();
		ib->Release();
		return hRes;
	}



	hRes = directDevice->SetStreamSource(0, vb, 0, sizeof(PositionVertex));
	if(FAILED(hRes))
	{
		vb->Release();
		ib->Release();
		return hRes;
	}

	const int faces   = mesh->GetNumFaces();
	const int indexes = mesh->GetNumVertices();
	hRes = directDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0, 0, indexes, 0, faces);
	if(FAILED(hRes))
	{
		vb->Release();
		ib->Release();
		return hRes;
	}

	vb->Release();
	ib->Release();

	return hRes;
}

HRESULT CSuckyAnimation::Render2D()
{
	assert(directDevice != NULL);

    if(NULL == directDevice)
		return E_FAIL;

	HRESULT hRes = S_OK;

	directDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //D3DTEXF_ANISOTROPIC
	directDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  //D3DTEXF_ANISOTROPIC
	directDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


	// give the device our vertex format
	directDevice->SetFVF(D3DFVF_AVEVERTEX2);
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

	// set the texture
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

	// draw the mesh
	hRes = DrawMesh();
	if(FAILED(hRes))
	{
		return hRes;
	}

	return S_OK;
}


HRESULT CSuckyAnimation::Render()
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

HRESULT CSuckyAnimation::InitMesh()
{
	assert(directDevice != NULL);

	HRESULT hRes = S_OK;

	WCHAR meshPath[MAX_PATH] = {0};
	wcscpy(meshPath, _App->GetAveDeskDataDir().c_str());
	PathAppendW(meshPath, L"Data\\closeeffect.data");

	LPD3DXBUFFER adj = NULL;
	LPD3DXBUFFER mat = NULL;
	LPD3DXBUFFER eff = NULL;
	DWORD numMaterials(0);

	hRes = D3DXLoadMeshFromX(meshPath, D3DXMESH_VB_MANAGED, directDevice, &adj, &mat, &eff,  &numMaterials, &mesh);
	assert(hRes != E_OUTOFMEMORY);
	assert(hRes != D3DERR_INVALIDCALL);

	SafeRelease(adj);
	SafeRelease(mat);
	SafeRelease(eff);

	return hRes;
}

HRESULT CSuckyAnimation::Initialize(HWND hwnd)
{	
	HRESULT hRes = S_OK;

	this->hwnd = hwnd;

	try
	{
		// A backBitmap and a frontBitmap are mandatory
		if(NULL == bitmap)
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

		// create a direct3d device
		DWORD behaviourFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		hRes = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behaviourFlags, &params, &directDevice);
		CheckHRES(hRes);

		// create the mesh that will hold the vertices for this animation.
		hRes = InitMesh();
		CheckHRES(hRes);

		// initialize the texture
		// on a texture
		hRes = InitTexture(&texture, bitmap);
		CheckHRES(hRes);

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
	}
	catch(HRESULT& hr)
	{
		ReleaseAll();
		hRes = hr;
	}

	return hRes;
}


D3DXVECTOR3 CSuckyAnimation::Transform(const D3DXVECTOR3& point, float offsetX, float offsetY)
{
	int locX = 0;
	int locY = 0;

	SIZE s = {bitmap->GetWidth(), bitmap->GetHeight() };

	D3DXVECTOR3 ret;
	ret.x = (point.x - offsetX) * s.cx + locX;
	ret.y = (point.y - offsetY) * s.cy + locY;
	ret.z = 0.0f;

	return ret;
}

D3DXVECTOR3 CSuckyAnimation::DeTransform(const D3DXVECTOR3& point, float offsetX, float offsetY)
{
	int locX = 0;
	int locY = 0;

	SIZE s = {bitmap->GetWidth(), bitmap->GetHeight() };

	D3DXVECTOR3 ret;
	ret.x = (point.x - locX) / s.cx + offsetX;
	ret.y = (point.y - locY) / s.cy + offsetY;
	ret.z = 0.0f;

	return ret;
}


float CSuckyAnimation::GetDistance(const D3DXVECTOR3& pointA, const D3DXVECTOR3& pointB)
{
	float valueX = abs(pointA.x - pointB.x);
	float valueY = abs(pointA.y - pointB.y);

	return sqrt(valueX * valueX + valueY * valueY);
}


float CSuckyAnimation::GetNewValue(float vanishPoint, float current, float pullForce)
{
	return vanishPoint * pullForce + current * (1 - pullForce);
}

void CSuckyAnimation::SetVanishPoint(const POINT& p)
{
	float xRatio = static_cast<float>(bitmap->GetWidth())  / static_cast<float>(optimalSize.cx);
	float yRatio = static_cast<float>(bitmap->GetHeight()) / static_cast<float>(optimalSize.cy);
	vanishPoint.x = static_cast<float>(static_cast<float>(p.x) * xRatio);
	vanishPoint.y = static_cast<float>(static_cast<float>(p.y) * yRatio);
	vanishPoint.z = 0.0f;
}

bool CSuckyAnimation::Next(Bitmap** bmp)
{
	assert(mesh != NULL);
	assert(bmp != NULL);

	if(NULL == bmp)
		return true;

	*bmp = NULL;

	if(NULL == mesh)
		return true;

	bool isDone = false;

	HRESULT hRes = S_OK;
	


	D3DXVECTOR3 vPoint(vanishPoint.x, vanishPoint.y, 0.0f);

	int locX = 0;
	int locY = 0;

	D3DXVECTOR3 vecA((FLOAT)locX, (FLOAT)locY, 0.0f);
	float stopDistance = GetDistance(vecA, vPoint) * 0.0001f;

	float maxFoundDistance = 0.0f;

	LPVOID data = {0};
	hRes = mesh->LockVertexBuffer(0, &data);
	PositionVertex* verts = reinterpret_cast<PositionVertex*>(data);
	if(verts != NULL)
	{
		DWORD i = 0;
		DWORD len = mesh->GetNumVertices();
		while(i < len)
		{		
			D3DXVECTOR3  pos(verts[i].x , verts[i].y, verts[i].z);
			D3DXVECTOR3 tPoint = Transform(pos, -0.5f, -0.5f);
			
			float distance = GetDistance(tPoint, vPoint);

			if(distance > maxFoundDistance)
				maxFoundDistance = distance;

			if(!(distance <= stopDistance))
			{
				float force = 0.0f;
				if(0 == farDistance)
				{
					firstFarDistance = maxFoundDistance;
					force = 0.0f;
				}
				else
				{
					float c = cos( (D3DX_PI / 1.6f) * distance  / farDistance + D3DX_PI / 2.0f) * 0.8f + 1.2f;
					force = c * c * speed;	
				}

				D3DXVECTOR3 old(
									GetNewValue(vanishPoint.x, tPoint.x, force), 
									GetNewValue(vanishPoint.y, tPoint.y, force),
									GetNewValue(vanishPoint.z, tPoint.z, force)
								);
				D3DXVECTOR3 nPoint = DeTransform(old, -0.5f, -0.5f);

				
				verts[i].x = nPoint.x;
				verts[i].y = nPoint.y;
				verts[i].z = nPoint.z;
			}
			
			i += 1;
		}
		farDistance = maxFoundDistance;

		speed *= 1.0f + speed / 2.0f;
		if(speed > 1.0f)
			speed = 1.0f;

		float delta(0.1f);
		if(maxFoundDistance <= stopDistance + delta)
			isDone = true;		
	}
	else
	{
		isDone = true;
	}
	
	mesh->UnlockVertexBuffer();	

	Render();
	GetBitmapFromRenderTarget(bmp);

	return isDone;
}

float CSuckyAnimation::GetProgressFactor()const
{
	if(firstFarDistance == 0.0f)
		return 0.0f;
	else
		return farDistance / firstFarDistance;
}	