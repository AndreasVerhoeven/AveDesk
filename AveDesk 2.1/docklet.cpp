// filename:		docklet.cpp
// Description:		Implementation of class Docklet
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					This class is a wrapper class for the OdDockletInstance class
//					and handles additional settings only used by the GUI.
//					In some way, this class is a decorator.
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "docklet.h"
#include "dockletdll.h"
#include <windows.h>
#include "test.h"
#include "callback_funcs.h"
#include "Layer.h"
#include <vector>
#include <shlobj.h>
#include "DrawPluginInstance.h"
#include "EnumDeskletDragDropFormats.h"
#include "resource.h"
#include "DeskletLabelSheet.hpp"
#include "DeskletAdvancedSheet.hpp"
#include "DeskletAppearanceSheet.h"
#include "IPersistable.h"
#include "AveDeskGlobalSettings.hpp"
#include "constants.h"
#include "DeskletSkinEnumerator.h"
#include "TrayIconAnimation.h"
#include "SuckyAnimation.h"

#include "CloseButton.h"

#include "ControlDlg.h"

#include "DeskletStyles.h"
#include "DeskletHooks.h"

UINT Docklet::clipFormat = 0;
CRITICAL_SECTION Docklet::cs = {0};

extern AveDeskGlobalSettings _globalSettings;

#include "Policies.h"
extern CPolicies _Policies;

//Docklet* Docklet::defaultDocklet=NULL;
Docklet* Docklet::defaultDocklet = new Docklet(NULL,true);
char	Docklet::defaultImage[MAX_PATH] = "";

Docklet::DockletsMap Docklet::Docklets;
CObservable* Docklet::s_GlobalDockletsEventNotifier = NULL;

LRESULT Docklet::OnClose(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DeskletRemove(m_hWnd, TRUE);
	return 0;
}

HRESULT Docklet::DataRequest(FORMATETC *pFormatetc, STGMEDIUM *pmedium)
{
	if(pFormatetc->cfFormat == clipFormat && (pFormatetc->tymed & TYMED_HGLOBAL) != 0)
	{
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,sizeof(HWND));
		HWND* hwnd  = (HWND*) GlobalLock(hMem);
		*hwnd = docklet->GetWindow();
		GlobalUnlock(hMem);
		pmedium->tymed = TYMED_HGLOBAL;
		pmedium->hGlobal = hMem;
		pmedium->pUnkForRelease = 0;
		return S_OK;
	}

	if(docklet->DragDropDataRequest(pFormatetc,pmedium))
		return S_OK;
	else
		return E_UNEXPECTED;
}

HRESULT STDMETHODCALLTYPE Docklet::QueryInterface(REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IDataObject))
	{
		*ppvObject = static_cast<IDataObject *>(this);
	}
	else if (IsEqualIID(riid, IID_IDropSource))
	{
        *ppvObject = static_cast<IDropSource *>(this);
	}
	else if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget))
	{
		*ppvObject = static_cast<IDropTarget *>(this);
	}
	else
	{
		*ppvObject = NULL;
        return E_NOINTERFACE;
    }

    AddRef();

    return S_OK;
}

ULONG STDMETHODCALLTYPE Docklet::AddRef(void)
{
	m_nRefCnt++;
	return m_nRefCnt;
}

ULONG STDMETHODCALLTYPE Docklet::Release(void)
{
	m_nRefCnt--;
	if(m_nRefCnt > 0)
		return m_nRefCnt;

	delete this;

	return 0;
}

HRESULT STDMETHODCALLTYPE Docklet::DragEnter(IDataObject * obj, DWORD keys, POINTL ptl, DWORD *effect)
{
	isInDropMode = true;

	IDropTargetImpl<Docklet>::DragEnter(obj,keys,ptl,effect);

	//PostMessage(WM_MOUSEMOVE,keys,MAKEWORD(ptl.x,ptl.y) );
	docklet->SetMouseOver(true);
	docklet->MouseOn();

	if(docklet)
		docklet->DragEnter(obj,keys,ptl,effect);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Docklet::DragOver(DWORD keys, POINTL ptl , DWORD *effect)
{
	isInDropMode = true;

	IDropTargetImpl<Docklet>::DragOver(keys,ptl,effect);

	//PostMessage(WM_MOUSEMOVE,keys,MAKEWORD(ptl.x,ptl.y) );
	docklet->MouseMove(ptl.x,ptl.y);

	if(docklet)
		docklet->DragOver(keys,ptl,effect);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Docklet::Drop(IDataObject * obj, DWORD keys, POINTL ptl, DWORD *effect)
{ 
	IDropTargetImpl<Docklet>::Drop(obj,keys,ptl,effect);

	if(docklet)
		docklet->Drop(obj,keys,ptl,effect);

	docklet->SetMouseOver(false);
	docklet->MouseOut();

	isInDropMode = false;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Docklet::DragLeave(void)
{
	IDropTargetImpl<Docklet>::DragLeave();

	//PostMessage(WM_MOUSELEAVE,0,0);

	if(docklet)
		docklet->DragLeave();

	docklet->SetMouseOver(false);
	docklet->MouseOut();

	isInDropMode = false;

	return S_OK;
}

Docklet* Docklet::FindDocklet(HWND hwnd)
{
	// post: if this docklet is found it has been returned, otherwise
	//		 0 has been returned

	EnterCriticalSection(&cs);
	Docklet* ret = NULL;

	try
	{
		DockletsMap::iterator iter = Docklets.find(hwnd);
	
		if(iter != Docklets.end() )
		{
			LeaveCriticalSection(&cs);
			return iter->second;
		}
	}
	catch(...)
	{

	}
	LeaveCriticalSection(&cs);

	return ret;
}

void Docklet::Remove()
{
	//if(GetCurrentThread == 
	//PostQuitMessage(0);
	PostMessage(WM_AVEACTION,0,AVEACTION_REMOVE);
	//PostThreadMessage(threadId,WM_AVEACTION,0,AVEACTION_REMOVE);
}

void Docklet::RemoveWithAnimation()
{
	PostMessage(WM_AVEACTION, 0, AVEACTION_REMOVEANIMATED);
}

docklets::OdDockletInstance* Docklet::GetDocklet()
{
	// post: this Docklet-objects OdDocklet has been returned
	return docklet;
}

void Docklet::SetNoRedraw(bool val)
{
	if(val != noRedraw)
	{
		noRedraw = val;
		if(!noRedraw && docklet->GetIsDirty())
			Redraw();

		// TEST
		//if(noRedraw)
			//SendMessage(WM_NULL, 0L, 0L);
	}
}

extern bool _noStartAnimation;
void Docklet::FadeAllLoadedIn()
{
	std::vector<HWND>hwnds;
	std::vector<BYTE> alphas;

	EnterCriticalSection(&cs);

	for(DockletsMap::iterator iter = Docklets.begin(); iter != Docklets.end(); ++iter)
	{
		Docklet* d = iter->second;
		if(d->isInitialized && d->isVisible && d->GetStyle() != STYLE_SHOWCASE)
		{
			d->docklet->VisibilityNotify(TRUE,0);
			d->noRedraw = true;
			hwnds.push_back(d->GetHWnd());
			alphas.push_back(d->GetDocklet()->GetAlpha() );
		}
		else
		{
			d->noFadeIn = false;
		}
	}

	LeaveCriticalSection(&cs);

	BOOL fFade = FALSE;
	SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &fFade, 0);
	if(fFade && !_noStartAnimation)
	{
		DWORD maxAnimTime = 450; // ms = 3sec
		DWORD timeDone = 0;
		int startFrame = 0;
		int endFrame = 255;
		int diffFrame = endFrame - startFrame;
		float add = 1;

		float curFrame = static_cast<float>(startFrame);

		while(static_cast<int>(curFrame) < endFrame && timeDone < maxAnimTime)
		{
			LARGE_INTEGER hpCounterFrequency = {0};
			QueryPerformanceFrequency(&hpCounterFrequency);

			LARGE_INTEGER hpCounterStart = {0};
			QueryPerformanceCounter(&hpCounterStart);

			DWORD startTime = GetTickCount();

			for(UINT i=0;i<hwnds.size();++i)
			{
				Docklet* d = FindDocklet(hwnds[i]);
				if(d)
				{
					if(curFrame < d->docklet->GetAlpha())
					{
						if(d && d->isVisible && !d->isShowCased)
						{		
							if(curFrame==0)
							{
								POINT p = d->docklet->GetCorrectedPos();
								SIZE s = d->docklet->GetCorrectedSize();
								Bitmap* bmp = d->docklet->Draw();
								d->SetBitmap(bmp,(BYTE)add,p,s);
								d->ShowWindow(SW_SHOWNA);
								if(!d->GetLabelOnlyOnMouseOver())
								{
									d->label.UpdateAlphaOnly((BYTE)add);
									d->label.ShowWindow(SW_SHOWNA);
								}
							}
							else
							{
								d->SetAlpha((BYTE)curFrame);
	
							}
		
						}
					}
					
				}
			}

			DWORD frameLength = GetTickCount() - startTime;

			LARGE_INTEGER hpCounterEnd = {0};
			QueryPerformanceCounter(&hpCounterEnd);

			float hpLength = float(hpCounterEnd.QuadPart - hpCounterStart.QuadPart) / float(hpCounterFrequency.QuadPart);
			hpLength *= 1000;

			timeDone += (DWORD)hpLength;
			float f = static_cast<float>(hpLength) / static_cast<float>(maxAnimTime);
			add = diffFrame *f;
			if(add == 0)
				add = 1;

			curFrame += add;
			if(curFrame > endFrame)
				curFrame = (float)endFrame;
		}

		for(UINT i=0;i<hwnds.size();++i)
		{
			Docklet* d = FindDocklet(hwnds[i]);
			if(d)
			{
				d->SetAlpha(0);
			}

		}


		for(UINT ii=0;ii<hwnds.size();++ii)
		{
			Docklet* d = FindDocklet(hwnds[ii]);
			if(d && d->isVisible)
			{
				d->SetAlpha(d->docklet->GetAlpha());

				d->SetNoRedraw(false);
			}
		}

	}
	else
	{
		for(UINT i=0;i<hwnds.size();++i)
		{
			Docklet* d = FindDocklet(hwnds[i]);
			if(d && d->isVisible && !d->isShowCased)
			{
				POINT p = d->docklet->GetCorrectedPos();
				SIZE s = d->docklet->GetCorrectedSize();
				Bitmap* bmp = d->docklet->Draw();
				d->SetBitmap(bmp,d->docklet->GetAlpha(),p,s);

				d->ShowWindow(SW_SHOWNA);
				if(!d->GetLabelOnlyOnMouseOver())
					d->label.ShowWindow(SW_SHOWNA);

				d->SetNoRedraw(false);
			}

		}
	}
}


extern bool _noCloseAnimation;
void Docklet::FadeAllOut()
{
	std::vector<HWND>hwnds;
	std::vector<BYTE> alphas;

	EnterCriticalSection(&cs);

	for(DockletsMap::iterator iter = Docklets.begin(); iter != Docklets.end(); ++iter)
	{
		hwnds.push_back(iter->second->GetHWnd());
		alphas.push_back(iter->second->GetDocklet()->GetAlpha() );
		iter->second->noRedraw = true;
	}

	LeaveCriticalSection(&cs);

	BOOL fFade = FALSE;
	SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &fFade, 0);
	if(fFade && !_noCloseAnimation)
	{
		DWORD maxAnimTime = 450; // ms = 3sec
		DWORD timeDone = 0;
		int startFrame = 255;
		int endFrame = 0;
		int diffFrame = endFrame - startFrame;
		float add = -1;

		float curFrame = static_cast<float>(startFrame);

		while(static_cast<int>(curFrame) > endFrame && timeDone < maxAnimTime)
		{
			LARGE_INTEGER hpCounterFrequency = {0};
			QueryPerformanceFrequency(&hpCounterFrequency);

			LARGE_INTEGER hpCounterStart = {0};
			QueryPerformanceCounter(&hpCounterStart);

			DWORD startTime = GetTickCount();

			for(UINT i=0;i<hwnds.size();++i)
			{
				Docklet* d = FindDocklet(hwnds[i]);
				if(d)
				{
					if(curFrame < d->docklet->GetAlpha())
						d->SetAlpha((BYTE)curFrame);
					
				}
			}

			DWORD frameLength = GetTickCount() - startTime;

			LARGE_INTEGER hpCounterEnd = {0};
			QueryPerformanceCounter(&hpCounterEnd);

			float hpLength = float(hpCounterEnd.QuadPart - hpCounterStart.QuadPart) / float(hpCounterFrequency.QuadPart);
			hpLength *= 1000;

			timeDone += (DWORD)hpLength;
			float f = static_cast<float>(hpLength) / static_cast<float>(maxAnimTime);
			add = diffFrame *f;
			if(add == 0)
				add = -1;

			curFrame += add;
			if(curFrame < endFrame)
				curFrame = (float)endFrame;
		}

		for(UINT i=0;i<hwnds.size();++i)
		{
			Docklet* d = FindDocklet(hwnds[i]);
			if(d)
			{
				d->SetAlpha(0);
			}

		}
	}
}
void Docklet::RepairDesktopWindows()
{
	EnterCriticalSection(&cs);
	for(DockletsMap::iterator iter = Docklets.begin(); iter != Docklets.end(); ++iter)
	{
		Docklet *d = iter->second;
		if(d->GetStyle() == STYLE_ICON && !d->IsWindow())
		{
			d->isCrashed = true;
			// here we could do more than just that!
		}
	}
	LeaveCriticalSection(&cs);
}

void Docklet::ReleaseAll()
// post:  all Docklet objects and thus desklets will be released.
{
	// first, notify every observer that we are about to release all desklets.
	NotifyGlobalDockletsEvent(OM_AllDeskletsAboutToBeReleased,0L);

	std::vector<HANDLE> handles;
	std::vector<Docklet*> hwnds;

	// get all threadhandles and hwnds for all docklet objects, safely.
	EnterCriticalSection(&cs);

	for(DockletsMap::iterator iter = Docklets.begin(); iter != Docklets.end(); ++iter)
	{
		hwnds.push_back(iter->second);
		handles.push_back(iter->second->threadHandle);
		iter->second->isInDestroyedState = true;
	}

	LeaveCriticalSection(&cs);

	// now, send a REMOVE message to each desklet.
	for(std::vector<Docklet*>::iterator iter2 = hwnds.begin(); iter2 != hwnds.end(); ++iter2)
		PostThreadMessage((*iter2)->threadId,WM_AVEACTION,0,AVEACTION_REMOVE);

	// and wait until all desklets have been destroyed by waiting for all their thread handles.
	if(handles.size() > 0)
	{
		bool bWait = true;
		while(bWait)
		{
			MSG msg;
			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			
			while(::PeekMessage(&msg, (HWND)INVALID_HANDLE_VALUE, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			PostThreadMessage(_App->GetThreadID(), WM_NULL, 0,0);
			DWORD dwRetVal = 0;
			if(handles.size() > 0)
				dwRetVal =  ::MsgWaitForMultipleObjects(
					handles.size(), &handles[0], TRUE, INFINITE, QS_ALLEVENTS);

			if(dwRetVal == (WAIT_OBJECT_0 + handles.size()))
				continue;

			if(dwRetVal != WAIT_TIMEOUT)
			{
				bWait = false;
				break;
			}

		}
	}

	//was: WaitForMultipleObjects(handles.size(),&handles[0],TRUE,INFINITE);
}

Docklet::Docklet(DockletDLL* dockletDLL, bool isDefaultValues) : 
	m_nRefCnt(1),
	snapToGrid(false),
	mouseIsOver(false),
	moving(false),
	isVisible(true),
	docklet(NULL),
	labelHwnd(NULL),
	isInitialized(false),
	configDlgIsVisible(false),
	hiddenParentHwnd(NULL),
	h(NULL),
	propHwnd(NULL),
	appearanceTabHwnd(NULL),
	dll(dockletDLL),
	isShowCased(false),
	label(this),
	isClickThru(false),
	ignoreLeftClicks(false),
	ignoreRightClicks(false),
	m_isDefaultValues(isDefaultValues),
	lockPosition(false),
	labelOnlyOnMouseOver(false),
	labelType(LABEL_REGULAR),
	style(STYLE_ICON),
	isInDropMode(false),
	threadId(0),
	threadHandle(NULL),
	isInitializedEvent(NULL),
	isInDestroyedState(false),
	noRedraw(false),
	noFadeIn(false),
	normalCursor(NULL),
	hoverCursor(NULL),
	fadeMs(1500),
	stopFading(false),
	m_ImageCannotBeSet(false),
	m_DeskletClickingMode(CLICKMODE_DESKLET_DEFAULT),
	m_CloseButton(NULL),
	isCrashed(false),
	isDestroying(false)
	// pre:  dockletDLL is not NULL
	// post: a Docklet-object has been created

{
	// because every docklet now runs in its own thread, we
	// need to call OleInit() and CoInit() in the c`tor to make
	// sure the COM subsystem can be used correctly.
	//OleInitialize(0);
	//CoInitialize(0);

	// initialize the flipping data
	m_FlippingData.isFlipping = FALSE;
	m_ClosePoint.x = 0;
	m_ClosePoint.y = 0;
	m_UseClosePoint = true;
	m_ClosePointScale.cx = 0;
	m_ClosePointScale.cy = 0;

	if(isDefaultValues)
	{
		docklet = new docklets::OdDockletInstance(NULL,NULL,NULL);		
	}
	else
	{
		normalCursor = LoadCursor(NULL,IDC_ARROW);
		hoverCursor = LoadCursor(NULL,IDC_HAND);


		// every Docklet object gets an EVENT that will be raised
		// when the docklet is initialized. 
		std::wstringstream ws;
		ws << L"AveInitEvent" << (void*)this;
		isInitializedEvent = CreateEvent(NULL,TRUE,FALSE,ws.str().c_str() );

		// create an not activated instance of this docklet
		docklet = dockletDLL->GetDocklet()->CreateNotActivatedInstance();
	
		// first, apply all defaults to this docklet
		ApplyDefaults();

		if(label.GetBg())
			label.GetBg()->AddRef();
		
		labelPos.x = 0;
		labelPos.y = 0;
	
		label.SetDockletParent(this);
	}
}

UINT Docklet::GetRealClickingMode()const
{
	if(CLICKMODE_DESKLET_DEFAULT == m_DeskletClickingMode)
	{
		return _globalSettings.GetClickMode();
	}
	else
	{
		if(CLICKMODE_DESKLET_SINGLE == m_DeskletClickingMode)
			return CLICKMODE_SINGLE;
		else
			return CLICKMODE_DOUBLE;
	}
}

void Docklet::DoNextFlip()
{
	bool done = false;
	Bitmap* bmp = NULL;
	if(m_FlippingData.params.flipToBack)
		done = m_FlippingData.animation->FlipToBack(m_FlippingData.rotation, &bmp);
	else
		done = m_FlippingData.animation->FlipToFront(m_FlippingData.rotation, &bmp);

	SIZE bmpSize = {bmp->GetWidth(), bmp->GetHeight() };
	POINT p = {0};

	POINT pos = GetDocklet()->GetPos();
	int xCentre = pos.x + m_FlippingData.xCentre;
 	int	yCentre = pos.y + m_FlippingData.yCentre;

	p.x = static_cast<int>( static_cast<float>(xCentre) - static_cast<float>(bmpSize.cx) / 2.0f);
	p.y = static_cast<int>( static_cast<float>(yCentre) - static_cast<float>(bmpSize.cy) / 2.0f);

	SetBitmap(*bmp, GetDocklet()->GetAlpha(), p, bmpSize);

	delete bmp;

	m_FlippingData.rotation += m_FlippingData.rotationSpeed;

	if(done)
	{
		::KillTimer(m_hWnd, m_FlippingData.timerId);
		EndFlipping();
	}
}

void Docklet::DoNextSuckStep()
{
	bool done = false;
	Bitmap* bmp = NULL;
	done = m_SuckingData.animation->Next(&bmp);

	if(bmp != NULL)
	{
		//Graphics g(bmp);
		//g.Clear(Color(255,255,0,0));
		SIZE bmpSize = {bmp->GetWidth(), bmp->GetHeight() };

		float progressFactor = m_SuckingData.animation->GetProgressFactor();
		//progressFactor += 0.1f;
		if(progressFactor > 1.0f)
			progressFactor = 1.0f;
		BYTE bAlpha = static_cast<BYTE>(static_cast<float>(GetDocklet()->GetAlpha())* progressFactor);

		if(!_globalSettings.GetFadeOutWhileDoingClosingEffect())
			bAlpha = 255;

		POINT pos = GetDocklet()->GetPos();
		SetBitmap(*bmp, bAlpha, pos, bmpSize);

		if(m_CloseButton)
			m_CloseButton->ChangeAlpha(static_cast<BYTE>(255.0f * progressFactor));
	}

	delete bmp;
	
	if(done)
	{
		::KillTimer(m_hWnd, m_SuckingData.timerId);
		EndSucking();
	}
}

LRESULT Docklet::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !wParam )
	{
		// Multimeter compatibility fix:
		// MM starts with a timer ID of 0!
		return 0;
	}
	else if(wParam == m_FlippingData.timerId)
	{
		DoNextFlip();
	}
	else if(wParam == m_SuckingData.timerId)
	{
		DoNextSuckStep();
	}
	return 0;
}

void Docklet::SetClosePoint(const POINT& p, const int xScale, const int yScale)
{
	m_ClosePoint = p;
	m_ClosePointScale.cx = xScale;
	m_ClosePointScale.cy = yScale;

	if(m_CloseButton)
		m_CloseButton->NotifyChangedClosePoint();
}

POINT Docklet::GetClosePoint()const
{
	return m_ClosePoint;
}
SIZE Docklet::GetClosePointScale()const
{
	return m_ClosePointScale;
}

void Docklet::SetUseClosePoint(bool val)
{
	if(val != m_UseClosePoint)
	{
		m_UseClosePoint = val;

		if(m_CloseButton)
			m_CloseButton->NotifyChangedClosePoint();
	}
}

bool Docklet::GetUseClosePoint()const
{
	return m_UseClosePoint;
}

BOOL Docklet::StartSucking(bool close)
{
	SetNoRedraw(true);

	if(close)
	{
		label.ShowWindow(SW_HIDE);
	}

	std::vector<UINT> oldLayersVisible;
	std::vector<Layer*>& layers = GetDocklet()->GetLayers();
	for(UINT layerIndex =0; layerIndex < layers.size(); ++layerIndex)
	{
		Layer*& l = layers[layerIndex];
		if(l->GetVisibility())
			oldLayersVisible.push_back(layerIndex);
	}

	m_SuckingData.close = close;
	m_SuckingData.bitmap = GetDocklet()->DrawLayers(oldLayersVisible, GetDocklet()->GetSize());

	
	m_SuckingData.animation = new CSuckyAnimation(m_SuckingData.bitmap);
	if(!_globalSettings.GetDisableCloseEffect() && SUCCEEDED(m_SuckingData.animation->Initialize(hiddenParentHwnd)))
	{
		POINT p = RemapPoint(m_ClosePoint, GetDocklet()->GetSize(), m_ClosePointScale.cx, m_ClosePointScale.cy);
		m_SuckingData.animation->SetVanishPoint(p);

		m_SuckingData.timerId = 45059; // magic constant

		float speed = static_cast<float>( _globalSettings.GetClosingSpeed() ) / 1000.0f;

		if(GetKeyState(VK_SHIFT) < 0)
		{
			m_SuckingData.timerInterval  = 20;
			speed /= 3.0f;
			m_SuckingData.animation->SetSpeed(0.008f);
		}
		else
		{
			m_SuckingData.timerInterval  = 20;
			m_SuckingData.animation->SetSpeed(speed);
		}

		(UINT) ::SetTimer(m_hWnd, m_SuckingData.timerId, m_SuckingData.timerInterval, NULL);
	}
	else
	{
		EndSucking();
	}

	return TRUE;
}

BOOL Docklet::StartFlipping(const FlipParams* params)
{
	if(NULL == params)
	{
		EndFlipping();
		return TRUE;
	}

	if(	(params->useLayers && NULL == params->newLayersVisible) || 
		(!params->useLayers && NULL == params->newBitmap)
	)
	{
		return FALSE;
	}

	{
		SetNoRedraw(true);

		m_FlippingData.oldLayersVisible.clear();
		m_FlippingData.newLayersVisible.clear();

		SIZE oldSize = GetDocklet()->GetSize();
		SIZE newSize = GetDocklet()->GetSize();

		if(params->oldDeskletSize != NULL)
			oldSize = *params->oldDeskletSize;

		if(params->newDeskletSize != NULL)
			newSize = *params->newDeskletSize;

		m_FlippingData.oldBitmap = NULL;
		m_FlippingData.newBitmap = NULL;

		m_FlippingData.newDeskletSize = newSize;
		m_FlippingData.oldDeskletSize = oldSize;

		if(params->useLayers)
		{
			//std::vector<UINT> oldLayersVisible;
			//std::vector<UINT> newLayersVisible;

			if(NULL == params->oldLayersVisible)
			{
				std::vector<Layer*>& layers = GetDocklet()->GetLayers();
				for(UINT layerIndex =0; layerIndex < layers.size(); ++layerIndex)
				{
					Layer*& l = layers[layerIndex];
					if(l->GetVisibility())
						m_FlippingData.oldLayersVisible.push_back(layerIndex);
				}
			}
			else
			{
				for(UINT i=0; i<params->oldLayersVisibleCount; ++i)
					m_FlippingData.oldLayersVisible.push_back(params->oldLayersVisible[i]);
			}

			for(UINT i=0; i<params->newLayersVisibleCount; ++i)
					m_FlippingData.newLayersVisible.push_back(params->newLayersVisible[i]);

			m_FlippingData.oldBitmap = GetDocklet()->DrawLayers(m_FlippingData.oldLayersVisible, m_FlippingData.oldDeskletSize);
			m_FlippingData.newBitmap = GetDocklet()->DrawLayers(m_FlippingData.newLayersVisible, m_FlippingData.newDeskletSize);
		}
		else
		{
			if(params->oldBitmap != NULL)
				m_FlippingData.oldBitmap = params->oldBitmap;
			else
			{
				std::vector<Layer*>& layers = GetDocklet()->GetLayers();
				for(UINT layerIndex =0; layerIndex < layers.size(); ++layerIndex)
				{
					Layer*& l = layers[layerIndex];
					if(l->GetVisibility())
						m_FlippingData.oldLayersVisible.push_back(layerIndex);
				}

				m_FlippingData.oldBitmap = GetDocklet()->DrawLayers(m_FlippingData.oldLayersVisible, m_FlippingData.oldDeskletSize);
			}

			m_FlippingData.newBitmap = params->newBitmap;
		}

		// DO ANIMATION!

		Bitmap* front = m_FlippingData.oldBitmap;
		Bitmap* back  = m_FlippingData.newBitmap;
		if(!params->flipToBack)
		{
			front = m_FlippingData.newBitmap;
			back  = m_FlippingData.oldBitmap;
		}

		//POINT pos = GetDocklet()->GetPos();
		m_FlippingData.xCentre = oldSize.cx  / 2;
		m_FlippingData.yCentre = oldSize.cy / 2;

		m_FlippingData.params = *params;

		m_FlippingData.animation = new CFlipAnimation(front, back);
		if(!_globalSettings.GetDisableFlipEffect() && SUCCEEDED(m_FlippingData.animation->Initialize(hiddenParentHwnd)))
		{
			m_FlippingData.timerId = 498238; // magic constant
			m_FlippingData.timerInterval  = 20;
			m_FlippingData.rotationSpeed = static_cast<float>(_globalSettings.GetFlippingSpeed());
			if(GetKeyState(VK_SHIFT) < 0)
				m_FlippingData.rotationSpeed = static_cast<float>(_globalSettings.GetFlippingSpeed()) / 10.0f;
			m_FlippingData.rotation = 0.0f;
			m_FlippingData.isFlipping  = TRUE;

			(UINT) ::SetTimer(m_hWnd, m_FlippingData.timerId, m_FlippingData.timerInterval, NULL);
		}
		else
		{
			m_FlippingData.isFlipping = TRUE;
			EndFlipping();
		}

		return TRUE;
	}
	//else
	//{
	//	return FALSE;
	//}
}

void Docklet::EndSucking()
{
	m_SuckingData.animation->Uninitialize();
	delete m_SuckingData.bitmap;
	delete m_SuckingData.animation;
	m_SuckingData.animation = NULL;

	if(m_SuckingData.close)
	{
		SetVisibility(false);
		BeforeRemoval();
		Remove();
	}
	else
	{
		SetNoRedraw(false);
	}
}

extern void CALLBACK DeskletSetImage(HWND, Image*, BOOL);
void Docklet::EndFlipping()
{
	if(m_FlippingData.isFlipping)
	{
		m_FlippingData.animation->Uninitialize();
		delete m_FlippingData.animation;
		m_FlippingData.animation = NULL;

			if(m_FlippingData.params.useLayers)
			{
				std::vector<Layer*>& layers = GetDocklet()->GetLayers();
				for(UINT layerIndex =0; layerIndex < layers.size(); ++layerIndex)
				{
					Layer*& l = layers[layerIndex];
					l->SetVisibility(false);
				}

				for(UINT i=0; i<m_FlippingData.newLayersVisible.size(); ++i)
				{
					UINT index = m_FlippingData.newLayersVisible[i];
					if(index < layers.size())
						layers[index]->SetVisibility(true);
				}

				delete m_FlippingData.newBitmap;
			}
			else
			{
				// we destroy all other layers!
				std::vector<Layer*>& layers = GetDocklet()->GetLayers();
				for(UINT layerIndex =0; layerIndex < layers.size(); ++layerIndex)
					delete layers[layerIndex];
				layers.clear();

				DeskletSetImage(m_hWnd, m_FlippingData.newBitmap, TRUE);
			}

			delete m_FlippingData.oldBitmap;

			if(m_FlippingData.newDeskletSize.cx  != docklet->GetSize().cx || m_FlippingData.newDeskletSize.cy != docklet->GetSize().cy)
			{
				SIZE oldSize = docklet->GetSize();
				POINT oldPos = docklet->GetPos();
				POINT centre = {oldPos.x + oldSize.cx / 2, oldPos.y + oldSize.cy / 2};
				POINT newPos = { centre.x - m_FlippingData.newDeskletSize.cx / 2, centre.y - m_FlippingData.newDeskletSize.cy / 2};
				docklet->SetPos(newPos.x, newPos.y);
				docklet->SetSize(m_FlippingData.newDeskletSize.cx, m_FlippingData.newDeskletSize.cy);
			}

			GetDocklet()->SetIsDirty(true);
			docklet->InvalidateAll();
			SetNoRedraw(false);
			Redraw(true);

			m_FlippingData.oldBitmap = NULL;
			m_FlippingData.newBitmap = NULL;


		m_FlippingData.isFlipping = FALSE;
		if(m_FlippingData.params.callback != NULL)
			m_FlippingData.params.callback(m_FlippingData.params.callbackData, &m_FlippingData.params);
	}
}

Docklet::~Docklet()
// post: this docklet object has been destructed
{
	DropElem();
	delete docklet;

	if(dll != NULL)
	{
		dll->Release();	
		dll = NULL;
	}


	if(isInitializedEvent != NULL)
		CloseHandle(isInitializedEvent);

	// ave: 06/6
	ClearSetDataList();
	
    //!herd
    CloseHandle(h);

    // because every docklet now runs in its own thread, we need to call 
    // the matching CoUninit and OleUninit in the d`tor, since they 
    // also have been called in the c`tor.
    //CoUninitialize();
    //OleUninitialize();
}

// macro to quickly apply default values to the current instance of this docklet object
#define APPLYMEMBER(n) this->n = defaultDocklet->n;

void Docklet::ApplyDefaults()
// pre: docklet is not 0
// post: the default values has been applied to docklet
{
	if(defaultDocklet)
	{
		APPLYMEMBER(ignoreLeftClicks);
		APPLYMEMBER(ignoreRightClicks);
		APPLYMEMBER(isClickThru);
		APPLYMEMBER(label);
		APPLYMEMBER(labelOnlyOnMouseOver);
		APPLYMEMBER(labelType);
		APPLYMEMBER(lockPosition);
		APPLYMEMBER(style);
		APPLYMEMBER(snapToGrid);
	
		this->docklet->CopyValues(defaultDocklet->GetDocklet() );
	}

	// we need to actually use the SetIconFileName() function for the default image
	// to be loaded and used.
	SetIconFileName(defaultImage);	
}

void Docklet::SetupWindow()
// post: the desklets window has been set up
{

	// create a hidden parent window for this desklet. This is done so that
	// the desklet's window will not show up in the taskmanager's Windows List.

	// Because we will set this window as the parent of the real desklet window,
	// the window manager thinks it's hidden, because visibility is inheritated from
	// parent window. Later on, we will set the parent of the real desklet
	// to NULL, so it can act on its own. The taskmanager, however, will still
	// think that the window is hidden, so it will not show up in the taskmanagers
	// Windows List.
	hiddenParentHwnd = CreateWindow(_T("AveDummyClass"),_T("AveDummyWindow"),0,0,1,1,0,0,0, _Module.GetModuleInstance(),0);

	RECT r = {0};
	Create(hiddenParentHwnd,r,_T("AveDesklet"),WS_POPUP | WS_OVERLAPPED,WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	// ditch the default cursor, because we set the cursor dynamically (1.2+)
	SetClassLong(m_hWnd,GCL_HCURSOR,static_cast<LONG>(NULL));

	// we dont want this window to show up in the taskbar
	ModifyStyle(0,WS_POPUP | WS_OVERLAPPED);

	// initially, hide it and don't use the hiddent parent window anymore
	ShowWindow(SW_HIDE);
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW,0);
	SetParent(NULL);

	EnsureWindowIsLayered();
	
}

void Docklet::SetupLabel()
// post: the label of this desklet has been set up
{
	labelHwnd = label.SetupWindow(hiddenParentHwnd);

	// update the labels position after creating the label,
	// to make sure it's displayed at it's right position.
	UpdateLabelPosition();
}


// This is a custom struct to pass multiple parameters
// to a thread. It's used to load a desklet from another
// thread.
typedef struct _LoadDockletParams
{
	Docklet* d;					// docklet object that should be used for the newly to load docklet (cannot be NULL)
	ILoader* loader;			// the ILoader that is used to load the desklet (can be NULL, in case of a totally new desklet)
	DeskletSkinEnumerator* ed;	// The SkinEnumerator to be used if a specific skin needs to be loaded (can be NULL, in case of no skin)
	bool hasSkin;				// true iff there is skinInfo
	SkinInfoWrapper* skinInfo;  // the skinInfo that describes the skin to use (can be NULL, in case of no skin)
	POINT offsetPos;			// the point to offset the initial position over. is (0, 0) in most cases

} LoadDockletParams;

HANDLE Docklet::GetIsInitializedEventHandle()
{
	return isInitializedEvent;
}

size_t Docklet::Size()
{
	EnterCriticalSection(&cs);
	size_t size = Docklets.size();
	LeaveCriticalSection(&cs);
	return size;
}


extern DWORD _AnimTimeInMS;
DWORD __stdcall Docklet::ThisThreadFunc(LPVOID lpData)
{
	CoInitialize(NULL);
	OleInitialize(NULL);

	LoadDockletParams* ldp = reinterpret_cast<LoadDockletParams*>(lpData);

	Docklet* d= ldp->d;

	FORMATETC clipFormatetc = {0};
	clipFormatetc.cfFormat = (CLIPFORMAT)clipFormat;
	clipFormatetc.tymed = TYMED_HGLOBAL;
	clipFormatetc.lindex = -1;
	clipFormatetc.dwAspect = DVASPECT_CONTENT;
	d->AddFormat(clipFormatetc);

	d->SetupWindow();
	d->toolTip.Create(d->m_hWnd, d);
	d->SetupLabel();
	d->threadId = GetCurrentThreadId();

	//CRITICAL_SECTION cs = {0};
	//InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);
	Docklets.insert(std::make_pair(d->m_hWnd,d));
	LeaveCriticalSection(&cs);

	//DeleteCriticalSection(&cs);

	CDeskletStyles* styles = CDeskletStyles::GetInstance();
	if(d->styleName.length() == 0)
	{
		d->styleName = styles->GetUniqueName(d->style);
	}
	else
	{
		d->style = styles->GetIDForUniqueName(d->styleName.c_str());
	}

	d->m_CloseButton = CCloseButton::CreateCloseButton(d->GetHWnd());
	d->m_CloseButton->BindDocklet(d);

	// BUGFIX: these weren't always applied right, so we re-apply them here.
	d->SetClickThru(d->isClickThru);
	// BUGFIX: we first apply normal style, then the real style to fix
	//         problems with the closebutton in desktop icon style
	UINT realStyle = d->GetStyle();
	d->ApplyStyle(STYLE_NORMAL );
	//d->ApplyStyle(realStyle);
	d->ShowWindow(SW_HIDE);

	//CTrayIconAnimation trayAnim(_App->GetMainWindowHandle(), TRUE);

	d->docklet->Activate(ldp->loader,d->m_hWnd,d->docklet->GetModule(), ldp->ed, ldp->skinInfo );
	NotifyGlobalDockletsEvent(OM_DeskletAdded,reinterpret_cast<LPARAM>(d->GetHWnd()));

	//trayAnim.Stop();

	EnterCriticalSection(&CXMLLoader::cs);
	delete ldp->loader;
	delete ldp->skinInfo;
	LeaveCriticalSection(&CXMLLoader::cs);

	if(ldp->ed != NULL)
		ldp->ed->Release();

	// LEGACY: For Docklet compatibility. obsolete now, since Object Dock doesn't handle this
	//			properly neither.
	// ---- DragAcceptFiles(d->docklet->GetWindow(),d->docklet->AcceptDropFiles());


	d->ApplyStyle(realStyle);

	BOOL fFade = FALSE;

	/*
	d->UpdateLabelPosition();
	d->UpdateLabelWindow();

	POINT curPoint = d->GetDocklet()->GetPos();
	curPoint.x += ldp->offsetPos.x;
	curPoint.y += ldp->offsetPos.y;
	d->GetDocklet()->SetPos(curPoint.x,curPoint.y);

	d->isInitialized = true;

	if(d->isInitializedEvent)
		SetEvent(d->isInitializedEvent);

	if(!d->noFadeIn)
	{
		// BUGFIX: new desklets not showing up on the showcase level. Gotta do this more prettier, but it's
		//         a quick fix.

		VARIANT_BOOL isShowCaseActive = {VARIANT_FALSE};
		if(_ShowCase)
			_ShowCase->get_Active(&isShowCaseActive);

		if(isShowCaseActive == VARIANT_TRUE)
			d->SetIsShowCased(d->GetStyle() == STYLE_SHOWCASE || _ShowCase->GetIsActiveOnAllDesklets());

		bool shouldBeVisible = d->GetIsVisible() && (d->GetStyle() != STYLE_SHOWCASE || d->GetIsShowCased());

		if(d->GetIsShowCased())
		{
			d->SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
			::SetWindowPos(d->labelHwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
		}


		SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &fFade, 0);

		if(shouldBeVisible)
		{
				d->docklet->VisibilityNotify(TRUE,0);
				if(fFade)
				{
					d->FadeIn(_AnimTimeInMS);
				}
				else
				{
					d->GetLabel().ShowWindow(SW_SHOWNA);
					d->ShowWindow(SW_SHOWNA);
				}
					
		}
		else
		{
			d->docklet->VisibilityNotify(FALSE,0);
		}

	}

	// FIX: always redraw to be sure desklets don't turn out blank.
	d->Redraw();

	delete ldp;
	*/

	// FIX: always redraw to be sure desklets don't turn out blank.
	d->Redraw();

	::PostMessage(d->m_hWnd, WM_NULL, 0, 0);

	bool doInit=true;

	d->m_Msg;
	while(GetMessage(&d->m_Msg,0,0,0))
	{
		if(doInit)
		{
			doInit = false;

			
			POINT pos = d->docklet->GetPos();
			SIZE size = d->docklet->GetSize();
			RECT r = {pos.x, pos.y, pos.x + size.cx, pos.y + size.cy};
			CDeskletHooks* hooks = CDeskletHooks::GetInstance();
			if(hooks != NULL)
				hooks->onPositionChange(d->m_hWnd, &r);

			d->docklet->SetPos(r.left, r.top);
			d->docklet->SetSize(r.right - r.left, r.bottom - r.top);

			d->UpdateLabelPosition();
			d->UpdateLabelWindow();

			POINT curPoint = d->GetDocklet()->GetPos();
			curPoint.x += ldp->offsetPos.x;
			curPoint.y += ldp->offsetPos.y;
			d->GetDocklet()->SetPos(curPoint.x,curPoint.y);

			d->isInitialized = true;

			if(d->isInitializedEvent)
				SetEvent(d->isInitializedEvent);

			BOOL fFade = FALSE;
			if(!d->noFadeIn)
			{
				// BUGFIX: new desklets not showing up on the showcase level. Gotta do this more prettier, but it's
				//         a quick fix.

				VARIANT_BOOL isShowCaseActive = {VARIANT_FALSE};
				if(_ShowCase)
					_ShowCase->get_Active(&isShowCaseActive);

				if(isShowCaseActive == VARIANT_TRUE)
					d->SetIsShowCased(d->GetStyle() == STYLE_SHOWCASE || _ShowCase->GetIsActiveOnAllDesklets());

				bool shouldBeVisible = d->GetIsVisible() && (d->GetStyle() != STYLE_SHOWCASE || d->GetIsShowCased());

				if(d->GetIsShowCased())
				{
					d->SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
					::SetWindowPos(d->labelHwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
				}


				SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &fFade, 0);

				if(shouldBeVisible)
				{
						d->docklet->VisibilityNotify(TRUE,0);
						if(fFade)
						{
							d->FadeIn(_AnimTimeInMS);
						}
						else
						{
							d->GetLabel().ShowWindow(SW_SHOWNA);
							d->ShowWindow(SW_SHOWNA);
						}
							
				}
				else
				{
					d->docklet->VisibilityNotify(FALSE,0);
				}

			}

			delete ldp;
		}

		if(d->m_Msg.message == WM_AVEACTION && d->m_Msg.lParam == AVEACTION_REMOVE)
			break;

		TranslateMessage(&d->m_Msg);
        DispatchMessage(&d->m_Msg);
	}

	d->isDestroying = true;

	SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &fFade, 0);
	if(!d->isInDestroyedState && fFade)
		d->FadeOut(_AnimTimeInMS);

	d->m_CloseButton->UnbindDocklet();
	d->m_CloseButton->Release();
	d->m_CloseButton = NULL;

	//InitializeCriticalSection(&cs);

	//AutoCriticalSection destroyCS;
	//destroyCS.Lock();
	d->GetDocklet()->Destroy();

	d->stopFading = true;
	if(d->faderThread)
		WaitForSingleObject(d->faderThread,INFINITE);

	CDeskletStyles::DESKLET_STYLE_CHANGE_DATA dscd = {0};
	dscd.dwSize = sizeof(dscd);
	dscd.oldStyle = d->style;
	dscd.newStyle = (UINT)-1;
	dscd.deskletHwnd = d->m_hWnd;
	dscd.labelHwnd = d->labelHwnd;
	styles = CDeskletStyles::GetInstance();
	styles->OnStyleChange(&dscd);

	EnterCriticalSection(&cs);
	Docklets.erase(d->m_hWnd);
	if(Docklets.size() == 0)
	{
		if(_Policies.GetBoolValue(L"runtime//desklets//exitWhenLastDeskletIsClosed", FALSE))
			_App->Close();
	}
	LeaveCriticalSection(&cs);
	//DeleteCriticalSection(&cs);

	d->NotifyEvent(OM_AboutToBeRemoved);

	if( !d->isCrashed )
	{
		d->DestroyWindow();
		d->label.DestroyWindow();
	}

	HANDLE h = d->threadHandle;

	delete d;

	//destroyCS.Unlock();

	OleUninitialize ();
	CoUninitialize ();

	ExitThread(0);

	CloseHandle(h);

	return 0;
}

Docklet* Docklet::LoadDocklet(const WCHAR* filename, ILoader* loader, DeskletSkinEnumerator* ed, SkinInfoWrapper* skinInfo, bool dontMakeVisible)
{
	POINT offsetPosNilNil = {0, 0};
	return LoadDocklet(filename, loader, ed, skinInfo, dontMakeVisible, offsetPosNilNil);
}

Docklet* Docklet::LoadDocklet(const WCHAR* filename, ILoader* loader, DeskletSkinEnumerator* ed, SkinInfoWrapper* skinInfo, bool dontMakeVisible, POINT offsetPos)
{
	// pre: filename is not 0
	// post: the docklet has been loaded
	
	if(!filename)return NULL;
	
	DockletDLL* dockletDLL = DockletDLL::LoadDockletDLL(filename);
	if(!dockletDLL)	// Bail out on error/ the docklet cannot be located, so we can't instantiate it.
		return NULL;

	// create a new Docklet
	Docklet* d = new Docklet(dockletDLL);

	// read from previously saved settings if necesary
	ILoader* clonedLoader = NULL;
	if(loader)
		clonedLoader = loader->Clone();

	if(clonedLoader)
		clonedLoader->LoadPersistableClass(*d,L"");

	LoadDockletParams* params = new LoadDockletParams;
	params->offsetPos = offsetPos;
	params->d = d;
	params->loader = clonedLoader;

	if(ed)
		ed->AddRef();

	params->ed = ed;

	params->hasSkin = skinInfo != NULL;

	if(skinInfo)
		params->skinInfo = skinInfo->Clone();
	else
		params->skinInfo = NULL;

	d->noFadeIn = dontMakeVisible;

	d->threadHandle = CreateThread(NULL,0,ThisThreadFunc,(LPVOID)params,0,&d->threadId);

	return d;


/*	// read from previously saved settings if necesary
	if(loader)
		loader->LoadPersistableClass(*d,L"");


	d->SetupWindow();
	d->SetupLabel();

	Docklets.insert(std::make_pair(d->m_hWnd,d));

	// BUGFIX: these weren't always applied right, so we re-apply them here.
	d->SetClickThru(d->isClickThru);
	d->ApplyStyle(d->GetStyle() );

	d->ShowWindow(SW_HIDE);

	d->docklet->Activate(loader,d->m_hWnd,d->docklet->GetModule(), ed, skin);

	// LEGACY: For Docklet compatibility. obsolete now, since Object Dock doesn't handle this
	//			properly neither.
	// ---- DragAcceptFiles(d->docklet->GetWindow(),d->docklet->AcceptDropFiles());
	
	
	d->Redraw();

	d->isInitialized = true;

	return d;
*/
}

void Docklet::SetClickThru(bool val)
{	
	// post: isClickThru has been set to val and the EXSTYLE has been updated
	//		 to reflect the current clickThru value

	if(m_isDefaultValues)
	{
		isClickThru = val;
		return;
	}


	LONG l = GetWindowLong(GWL_EXSTYLE);

	if(val)
		l = l | WS_EX_TRANSPARENT;
	else if(isClickThru)
		l = l ^ WS_EX_TRANSPARENT;

	SetWindowLong(GWL_EXSTYLE,l);

	l = ::GetWindowLong(labelHwnd,GWL_EXSTYLE);

	if(val)
		l = l | WS_EX_TRANSPARENT;
	else if(isClickThru)
		l = l ^ WS_EX_TRANSPARENT;

	::SetWindowLong(labelHwnd,GWL_EXSTYLE,l);

	isClickThru = val;
}

bool Docklet::GetClickThru()
{
	// post: isClickThru has been returned
	return isClickThru;
}

void Docklet::SetIgnoreRightClicks(bool val)
{
	// post: ignoreRightClicks has been set to val
	ignoreRightClicks = val;
}

bool Docklet::GetIgnoreRightClicks()
{
	// post: ignoreRightClicks has been returned: 
	return ignoreRightClicks;
}

void Docklet::ApplyStyle(UINT s, bool doNotChangeZOrder)
{
	// post: style has been set to s and the windows parent/STYLE has been updated
	//		 to reflect the docklets style

	if(m_isDefaultValues)
	{
		style = s;
		CDeskletStyles* styles = CDeskletStyles::GetInstance();
		styleName = styles->GetUniqueName(style);
		return;
	}

	if(style == STYLE_SHOWCASE)
	{
		if(isVisible)
		{
			ShowWindow(SW_SHOWNA);
			::ShowWindow(labelHwnd,SW_SHOWNA);	
			docklet->VisibilityNotify(TRUE,0);
		}

		::SetWindowLong(m_hWnd, 0xFFFFFFF8, (LONG)hiddenParentHwnd);
		::SetWindowLong(labelHwnd, 0xFFFFFFF8, (LONG)hiddenParentHwnd);
	}

	CDeskletStyles::DESKLET_STYLE_CHANGE_DATA dscd = {0};
	dscd.dwSize = sizeof(dscd);
	dscd.oldStyle = style;
	dscd.newStyle = s;
	dscd.deskletHwnd = m_hWnd;
	dscd.labelHwnd = labelHwnd;
	CDeskletStyles* styles = CDeskletStyles::GetInstance();
	styleName = styles->GetUniqueName(style);
	styles->OnStyleChange(&dscd);


	if(s == STYLE_SHOWCASE)
	{
		::SetWindowLong(m_hWnd, 0xFFFFFFF8, (LONG)_ShowCase->m_hWnd);
		::SetWindowLong(labelHwnd, 0xFFFFFFF8, (LONG)_ShowCase->m_hWnd);

		if(IsWindowVisible() )
		{
			ShowWindow(SW_HIDE);
			::ShowWindow(labelHwnd,SW_HIDE);
			docklet->VisibilityNotify(FALSE,0);

			bool clickThru = GetClickThru();
			SetClickThru(!clickThru);
			SetClickThru(clickThru);
		}
	
	}
	
	// if the new style is STYLE_ICON we need to set it as a parent of Progman to prevent
	// it from hiding when minimize all is performed, otherwise if the style was STYLE_ICON
	// we need need to get rid of the parent/STYLE.
	HWND parent = ::FindWindow(_T("Progman"),NULL);
	// litestep fix:
	if( !parent )
		parent = ::FindWindow(_T("DesktopBackgroundClass"),NULL);

	// VISTA fix: can't no longer use the old 'become' a child of the desktop window hack
	//			 luckily, always on bottom will work in build 5742.
	if(s == STYLE_ICON && ::IsWindow(parent) && !GetIsRunningVista())
	{
		// find the Progman parent and set it as a child
		::SetParent(m_hWnd,parent);
		ModifyStyle(WS_CHILD,0);

		// do the same for the label
		::SetParent(labelHwnd,parent);
		label.ModifyStyle(WS_CHILD,0);
	}
	else
	{
		parent = NULL;
		// remove Program as a parent
		::SetParent(m_hWnd,NULL);
		LONG l = GetWindowLong(GWL_STYLE);
		l |= WS_POPUP;
		//l |= WS_CHILD;
		//l = l ^ WS_CHILD;
		l |= WS_OVERLAPPED;
		SetWindowLong(GWL_STYLE,l);

		// do the same for the label
		::SetParent(labelHwnd,NULL);
		l = ::GetWindowLong(labelHwnd,GWL_STYLE);
		l |= WS_POPUP;
		//l |= WS_CHILD;
		//l = l ^ WS_CHILD;
		l |= WS_OVERLAPPED;
		::SetWindowLong(labelHwnd,GWL_STYLE,l);
	}

	style = s;

	
	POINT dPos = docklet->GetCorrectedPos();
	POINT lPos = GetLabelPos();
	::UpdateLayeredWindow(GetHWnd(), NULL, &dPos, NULL, NULL, NULL, 0, NULL, 0);
	::UpdateLayeredWindow(GetLabelHwnd(), NULL, &lPos, NULL, NULL, NULL, 0, NULL, 0);

	HWND hwndInsertAfter = GetHwndInsertAfterFromStyle();

	if(!doNotChangeZOrder)
	{
		// set the z-order position of the docklets window and the docklets label
		SetWindowPos(hwndInsertAfter,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
		::SetWindowPos(labelHwnd,hwndInsertAfter,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}

	styleName = styles->GetUniqueName(style);

	if(m_CloseButton)
		m_CloseButton->NotifyZOrderingChange(hwndInsertAfter, parent);
}

HWND Docklet::GetHwndInsertAfterFromStyle()
{
	// post: the z-order HWND has been returned based on the current style

	switch(style)
	{
		case STYLE_NORMAL:	return HWND_NOTOPMOST;
		case STYLE_BOTTOM:	return HWND_BOTTOM;
		case STYLE_TOP:		return HWND_TOPMOST;
		case STYLE_ICON:	return GetIsRunningVista() ? HWND_BOTTOM : HWND_TOPMOST;
		case STYLE_SHOWCASE:return (HWND)-1;
	}

		CDeskletStyles::DESKLET_STYLE_CHANGE_DATA dscd = {0};
		dscd.dwSize = sizeof(dscd);
		dscd.oldStyle = style;
		dscd.newStyle = style;
		dscd.deskletHwnd = m_hWnd;
		dscd.labelHwnd = labelHwnd;
		CDeskletStyles* styles = CDeskletStyles::GetInstance();
		if(styles->OnGetInsertHwnd(&dscd))
			return dscd.deskletHwnd;
		else
			return HWND_NOTOPMOST;
}


const char* Docklet::GetIconFileName()
{
	std::vector<Layer*>& layers = docklet->GetLayers();
	for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++ iter)
	{
		Layer* l = *iter;
		if(l->GetVisibility() && l->GetCanBeIcon())
		{
			return l->GetFileName();
		}
	}
	return "";
}

void Docklet::SetIconFileName(const char* filename)
{
	strcpy(image,filename);

	std::vector<Layer*>& layers = docklet->GetLayers();
	for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++ iter)
	{
		Layer* l = *iter;
		if(l->GetVisibility() && l->GetCanBeIcon())
			l->SetFileName(filename);
	}
}

void Docklet::UpdateLabelWindow()
{
	if(labelType == LABEL_NONE || (GetLabelOnlyOnMouseOver() ? GetLabel().GetAlpha() : GetDocklet()->GetAlpha())<=0 || !labelHwnd) 
	{
		label.SetIsDirty(true);
		return;
	}

	label.UpdateLabel(docklet->GetQuality(),GetLabelOnlyOnMouseOver() ? label.GetAlpha() : docklet->GetAlpha(), docklet->GetIsSelected() );
 }

void Docklet::UpdateLabelPosition()
{

	if(!GetLabelHwnd())
		return;
	
	SIZE s	= GetDocklet()->GetSize();
	POINT p	= GetDocklet()->GetPos();
	switch(GetLabel().GetPosition() )
	{
		case LABEL_TOP:
			GetLabelPos().x = p.x + (s.cx/2) - GetLabel().GetWidth()/2;
			GetLabelPos().y = p.y - GetLabel().GetHeight() - GetLabel().GetMargin();
		break;

		case LABEL_LEFT:
			GetLabelPos().x = p.x - GetLabel().GetWidth() - GetLabel().GetMargin();
			GetLabelPos().y = p.y + s.cy/2 - GetLabel().GetHeight()/2;
		break;

		case LABEL_RIGHT:
			GetLabelPos().x = p.x + s.cx + GetLabel().GetMargin();
			GetLabelPos().y = p.y + s.cy/2 - GetLabel().GetHeight()/2;
		break;

		case LABEL_BOTTOM:
		default:
			GetLabelPos().x = p.x + (s.cx/2) - GetLabel().GetWidth()/2;
			GetLabelPos().y = p.y + s.cy + GetLabel().GetMargin();
		break;
	}


	//::MoveWindow(GetLabelHwnd(),GetLabelPos().x,GetLabelPos().y,GetLabel().GetWidth(),GetLabel().GetHeight(),FALSE);
	::UpdateLayeredWindow(GetLabelHwnd(), NULL, &GetLabelPos(), NULL, NULL, NULL, 0, NULL, 0);
}

void Docklet::Redraw(bool force)
{
	if(SupportsFeature(DESKLET_SUPPORTS_ASYNC_PAINTING))
	{
		::PostMessageW(m_hWnd, WM_PAINT, 0, (LPARAM)(force ? TRUE : FALSE));
		return;
	}

	if(GetCurrentThreadId() != threadId)
	{
		::PostMessage(m_hWnd, WM_AVEACTION, force ? TRUE : FALSE, AVEACTION_REDRAW);
		return;
	}

	if(isInDestroyedState || noRedraw)
		return;


	if(!force)
	{
		if(!isVisible)
		{
			docklet->SetIsDirty(true);
			return;
		}
		
		if(style == STYLE_SHOWCASE && !isShowCased)
		{
			docklet->SetIsDirty(true);
			return;
		}
	}

	toolTip.OnResizeParent(m_hWnd, docklet->GetSize());

	
	Bitmap* bmp = docklet->Draw();
	
	POINT p = docklet->GetCorrectedPos();
	SIZE  s = docklet->GetCorrectedSize();

	SetBitmap(bmp,docklet->GetAlpha(),p,s);

	NotifyAll(true,static_cast<LPARAM>(OM_Repaint),reinterpret_cast<LPARAM>(GetHWnd()));

	docklet->SetIsDirty(false);
	

	//PostMessage(WM_PAINT,0,force ? TRUE : FALSE);
}

void Docklet::SetVisibility(bool v)
{
	if(v == isVisible && isInitialized == true )
		return;

	if(v == true)
	{
		if(GetStyle() == STYLE_SHOWCASE)
		{
			CComPtr<IShowCase>showCase;
			_App->get_ShowCase(&showCase);
			if(showCase)
			{
				VARIANT_BOOL isActive = { VARIANT_FALSE };
				showCase->get_Active(&isActive);
				if(isActive == VARIANT_TRUE && GetIsVisible() == false)
				{
					if(isInitialized)
					{
						docklet->VisibilityNotify(TRUE);

						if(docklet->GetIsDirty())
							Redraw(true);

						SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
						::SetWindowPos(GetLabelHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
					}
				}
			}
		}
		else
		{
			if(isInitialized)
			{
				docklet->VisibilityNotify(TRUE);

				if(docklet->GetIsDirty())
					Redraw(true);

				ShowWindow(SW_NORMAL);
				::ShowWindow(GetLabelHwnd(),SW_NORMAL);
			}
		}
		isVisible = true;
		if(GetDocklet() && GetDocklet()->GetDrawPlugin())
			GetDocklet()->GetDrawPlugin()->Event(EVENT_ONSHOW);
	}
	else
	{
		docklet->VisibilityNotify(FALSE);

		ShowWindow(SW_HIDE);
		::ShowWindow(GetLabelHwnd(),SW_HIDE);
		isVisible = false;

		if(GetDocklet() && GetDocklet()->GetDrawPlugin())
			GetDocklet()->GetDrawPlugin()->Event(EVENT_ONHIDE);
	}
}

int Docklet::AsyncDeskletPropertySheet(HWND owner, PropertyDialogStruct* pds, bool noAddPropSheet, BOOL dontWait)
{
	if(dontWait)
	{
		PostMessage(WM_AVEACTION,static_cast<WPARAM>(NULL),AVEACTION_CONFIGDLG);
		return 0;
	}
	else if(GetCurrentThreadId() == threadId || GetIsDefaultValues())
	{
		return DeskletPropertySheet(owner,pds,noAddPropSheet);
	}
	else
	{
		PROPSHEETPARAMS propSheetParams;
		propSheetParams.owner = owner;
		propSheetParams.pds = pds;
		propSheetParams.noAddPropSheets = noAddPropSheet;
		return SendMessage(WM_AVEACTION,reinterpret_cast<WPARAM>(&propSheetParams),AVEACTION_CONFIGDLG);
	}
}

bool Docklet::SupportsFeature(DWORD flag)
{
	return docklet != NULL && docklet->SupportsFeature(flag);
}

int Docklet::DeskletPropertySheet(HWND owner, PropertyDialogStruct* pds, bool noAddPropSheet)
{
	if(!_Policies.GetBoolValue(L"runtime//desklet//propertyDialog//allow", TRUE))
		return 0;

	if(appearanceTabHwnd != 0)
	{
		::SetFocus(::GetParent(appearanceTabHwnd));
		return propertyPages.size();
	}

	DrawPluginInstance* dp = GetDocklet()->GetDrawPlugin();
	if(dp)dp->Event(EVENT_ONCONFIGURE);

	propertyPages.clear();

	//AddDefaultPropertyPages();

	CDeskletAppearanceSheet appearanceSheet(this);
	if(_Policies.GetBoolValue(L"runtime//desklet//propertyDialog//appearance//allow", TRUE))
		propertyPages.push_back(CreatePropertySheetPage(appearanceSheet));

	CDeskletLabelSheet labelSheet(this);
	if(!SupportsFeature(DESKLET_NO_LABEL) && _Policies.GetBoolValue(L"runtime//desklet//propertyDialog//label//allow", TRUE))
		propertyPages.push_back(CreatePropertySheetPage(labelSheet));

	CDeskletAdvancedSheet advancedSheet(this);
	if(_Policies.GetBoolValue(L"runtime//desklet//propertyDialog//advanced//allow", TRUE))
		propertyPages.push_back(CreatePropertySheetPage(advancedSheet));

	if(_Policies.GetBoolValue(L"runtime//desklets//propertyDialogs//other//allow", TRUE))
	{
		std::vector<WrapperTabPage> wd;

		if(pds)
		{

			wd.resize(pds->pagesCount);

			for(UINT i=0;i<pds->pagesCount;++i)
			{
				wd[i].dlgProc = pds->pages[i].dlgProc;
				wd[i].lParam = pds->lParam;

				PROPSHEETPAGE psp = {0};
				psp.dwSize = sizeof(psp);
				psp.dwFlags = PSP_USETITLE;
				psp.hInstance = pds->pages[i].hInstance;
				psp.pszTitle = StringToWString(pds->pages[i].title).c_str();
				psp.pszTemplate = StringToWString(pds->pages[i].resourceName).c_str();
				psp.pfnDlgProc = WrapperProc;
				psp.lParam = (LPARAM)&wd[i];

				propertyPages.push_back(CreatePropertySheetPage(&psp));
				
			}
		}

		if(!noAddPropSheet)
			GetDocklet()->AddPropertySheets(propertyPages.size());
	}

	CString caption;
	caption.LoadString(IDS_PROPCAPTION);

	HPROPSHEETPAGE* pages = new HPROPSHEETPAGE[propertyPages.size()];
	for(UINT i=0;i<propertyPages.size(); ++i)
	{
		pages[i] = propertyPages[i];
	}

	PROPSHEETHEADER hdr = {0};
	hdr.dwSize = sizeof(PROPSHEETHEADER);
	hdr.dwFlags = PSH_DEFAULT;
	hdr.nStartPage = 0;
	hdr.nPages = propertyPages.size();
	hdr.phpage = &propertyPages[0];
	hdr.hwndParent = owner;
	hdr.pszCaption = caption;
	hdr.pfnCallback = 0;

	int res =PropertySheet(&hdr);

	propHwnd = 0;

	delete[] pages;
	propertyPages.clear();

	return res;
}

int Docklet::AddPropertyPage(HPROPSHEETPAGE hpg)
{
	if(hpg)
		propertyPages.push_back(hpg);

	return propertyPages.size();
}


HRESULT Docklet::StartDragDrop(POINT pt)
{
	DWORD dwEffect;

	CComPtr<IDataObject> data;
	HRESULT res = QueryInterface(IID_IDataObject,(void**)&data);

	if(FAILED(res))
		return res; // BAIL OUT on FAIL

	SIZE s = docklet->GetSize();
	Bitmap bmp(s.cx,s.cy);
	Graphics g(&bmp);
	docklet->DrawDragDropImage(g,s);
	SetDragImage(data,bmp,pt);

	return BeginDrag(data,&dwEffect);
}

HMENU Docklet::GetDefaultMenu(int startId)
{
	CMenu parent;
	parent.LoadMenu( IDR_POPUP );

	HMENU retMenu = CreatePopupMenu();
	
	
	CMenu menu;
	menu = parent.GetSubMenu(1);

	menu.CheckMenuItem(ID_DESKLETPOPUPMENU_LOCKPOSITION, GetLockPosition() ? MF_CHECKED : MF_UNCHECKED);
	menu.CheckMenuItem(ID_DESKLETPOPUPMENU_SHOWSUBLABEL, label.GetShowSubLabel() ? MF_CHECKED : MF_UNCHECKED);

	BOOL noneAllowed = !_Policies.GetBoolValue(L"runtime//desklet//menu//allow", TRUE);
	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showLockPositionItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_LOCKPOSITION, MF_BYCOMMAND);

	if(noneAllowed || SupportsFeature(DESKLET_NO_LABEL) || !_Policies.GetBoolValue(L"runtime//desklet//menu//showSubLabelItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_SHOWSUBLABEL, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showDuplicateItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_DUPLICATETHISDESKLET, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showCloseItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_REMOVEDESKLET, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showPropertiesItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_DESKLETPROPERTIES, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showControlPanelItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_CONTROLPANEL, MF_BYCOMMAND);

	
	int menuCount = menu.GetMenuItemCount();

	MENUITEMINFO info = {0};
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;


	for(int i=0; i < menuCount; i++)
	{
		CString text;

		menu.GetMenuString(i,text.GetBuffer(1024),1024,MF_BYPOSITION);
		AppendMenu(retMenu,menu.GetMenuState(i,MF_BYPOSITION),startId + i, text);

		text.ReleaseBuffer();
	}

	return retMenu;
}


extern BOOL RemoveDeskletAndSaveConfig(HWND,BOOL,BOOL);
extern void __stdcall DuplicateDesklet(HWND,BOOL,HWND*);

void Docklet::DoDefaultMenuCommand(int startId,int cmd)
{
	CMenu parent;
	parent.LoadMenu( IDR_POPUP );

	CMenu menu;
	menu = parent.GetSubMenu(1);

	BOOL noneAllowed = !_Policies.GetBoolValue(L"runtime//desklet//menu//allow", TRUE);
	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showLockPositionItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_LOCKPOSITION, MF_BYCOMMAND);

	if(noneAllowed || SupportsFeature(DESKLET_NO_LABEL) || !_Policies.GetBoolValue(L"runtime//desklet//menu//showSubLabelItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_SHOWSUBLABEL, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showDuplicateItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_DUPLICATETHISDESKLET, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showCloseItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_REMOVEDESKLET, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showPropertiesItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_DESKLETPROPERTIES, MF_BYCOMMAND);

	if(noneAllowed || !_Policies.GetBoolValue(L"runtime//desklet//menu//showControlPanelItem", TRUE))
		RemoveMenu(menu, ID_DESKLETPOPUPMENU_CONTROLPANEL, MF_BYCOMMAND);

	UINT id = menu.GetMenuItemID(cmd-startId);

	switch(id)
	{
		case ID_DESKLETPOPUPMENU_DESKLETPROPERTIES: // config
			if(!docklet->Configure())
			{
				AsyncDeskletPropertySheet(NULL); // or m_hWnd as owner, now, but they won't block the desklet.
			}
		break;

		case ID_DESKLETPOPUPMENU_REMOVEDESKLET:// remove
			::RemoveDeskletAndSaveConfig(m_hWnd,TRUE,TRUE);
		break;

		case ID_DESKLETPOPUPMENU_LOCKPOSITION: // lock
			lockPosition = ! lockPosition;
		break;

		case ID_DESKLETPOPUPMENU_SHOWSUBLABEL: // sublabel
			label.SetShowSubLabel( !label.GetShowSubLabel() );
			UpdateLabelWindow();
		break;

		case ID_DESKLETPOPUPMENU_DUPLICATETHISDESKLET: // duplicate:
			::DuplicateDesklet(m_hWnd,true,NULL);
		break;

		case 5: // none

		break;

		case ID_DESKLETPOPUPMENU_CONTROLPANEL: // control panel
            //_Dlg.ShowOrActivate();
			_App->ShowControlPanel(); 
		break;
	}
}

// message handlers and stuff

LRESULT Docklet::OnRightButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	//StartSucking(true);
	//return 0;

	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

	if(GetIgnoreRightClicks())
		return TRUE;

	RECT r = {0};
	GetClientRect(&r);
	SIZE s = SIZEFROMRECT(r);

	// 1.3: make holding the controlkey on right click ALWAYS show the default menu
	if(GetKeyState(VK_CONTROL) < 0 || !docklet->RightButtonClick(&pt,&s, (DWORD)wParam))
	{
		const int TAKESNAPSHOTITEM = 230393;
		// if the client desklet didn't handle the right button click, we show a default menu on right click.

		POINT msgPt = {GET_X_LPARAM(GetMessagePos()),GET_Y_LPARAM(GetMessagePos())};

		CMenu menu = GetDefaultMenu(1);
		if(GetKeyState(VK_SHIFT) < 0)
			AppendMenuW(menu, MF_STRING, 230393,L"Take Snapshot");

		int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, msgPt.x, msgPt.y, m_hWnd);

		if(TAKESNAPSHOTITEM == cmd)
		{
			Bitmap* bmp = docklet->Draw();
			if(bmp != NULL)
			{
				CFileDialog fd(FALSE,						// FALSE for SaveDialog
					_T("png"),								// default extension
					_T(""),									// filename
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,				// flags
					_T("PNG Files(*.png)\0*.png\0All Files(*.*)\0*.*\0\0"), // filter
					NULL);

				if( fd.DoModal() == TRUE)
				{
					USES_CONVERSION;

					CLSID encoderCLSID = {0};
					GetEncoderClsid(L"image/png", &encoderCLSID);
					bmp->Save(T2W(fd.m_szFileName), &encoderCLSID, NULL);
				}
			}
		}
		else if(cmd > 0)
		{
			DoDefaultMenuCommand(1,cmd);
		}
	}

	return 0;
}

void Docklet::StartMoving()
{
	SetCapture();
	moving = true;
	m_StartMovingDeskletPos = docklet->GetCorrectedPos();

	::SendMessage(GetHWnd(), WM_ENTERSIZEMOVE, 0L, 0L);
}

void Docklet::OnMovingMouse()
{
	DWORD msgPos = GetMessagePos();
	POINT pt = {GET_X_LPARAM(msgPos),GET_Y_LPARAM(msgPos)};

	int xDiff = m_StartMovingCursorPos.x - pt.x;
	int yDiff = m_StartMovingCursorPos.y - pt.y;

	SIZE s = docklet->GetSize();
	RECT r = {m_StartMovingDeskletPos.x - xDiff, m_StartMovingDeskletPos.y - yDiff, 0, 0};
	r.right  = r.left + s.cx;
	r.bottom = r.top  + s.cy;

	::SendMessage(GetHWnd(), WM_MOVING, (WPARAM)WMSZ_TOP, (LPARAM)&r);

	POINT newPos = {r.left, r.top};
	UpdateLayeredWindow(GetHWnd(), NULL, &newPos, NULL, NULL, NULL, 0, NULL, 0);
}


void Docklet::EndMoving()
{
	ReleaseCapture();
	moving = false;
	::SendMessage(GetHWnd(), WM_EXITSIZEMOVE, 0L, 0L);
}

LRESULT Docklet::OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

	//

	SetCapture();

	bHandled = FALSE;

	POINT p = pt;
	ClientToScreen(&p);

	RECT r ={0,0};
	GetClientRect(&r);
	SIZE s = SIZEFROMRECT(r);

	
	bool shouldNotMove = false;
	shouldNotMove = docklet->MouseDown(&pt, &s, static_cast<DWORD>(wParam));
	if(!shouldNotMove)
	{
		Layer* l = docklet->GetLayerAtPoint(pt, true);
		if(l != NULL)
			shouldNotMove = l->GetIsNotMovable();
	}

	if(!shouldNotMove)
	{	
		// BUGFIX: when GetLockPosition() was true, there wasn't checked if the drag actually was a drag-op, instead of
		//         a move-op.

		DWORD msgPos = GetMessagePos();
		m_StartMovingCursorPos.x = GET_X_LPARAM(msgPos);
		m_StartMovingCursorPos.y = GET_Y_LPARAM(msgPos);

		if(::DragDetect(m_hWnd,p) )
		{
			if(GetAsyncKeyState(VK_SHIFT) < 0 )
			{
				StartDragDrop(pt);
				return 0;
			}
			else if(!GetLockPosition())
			{

				moving = true;

				DrawPluginInstance* dp = docklet->GetDrawPlugin();
				if(dp)dp->Event(EVENT_ONSTARTMOVE);

				ReleaseCapture(); 
				//SendMessage(WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0L);
				StartMoving();

				return 0;
			}
			else
			{
				ReleaseCapture();
				return 0;
			}
		}
		else
		{
			return OnLeftButtonUp(uMsg,wParam,lParam,bHandled);
		}
	}
	else
	{
		return 0;
	}
}

LRESULT Docklet::OnLeftButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(moving)
	{
		EndMoving();
		return 0;
	}

	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

	bHandled = TRUE;

	if(GetKeyState(VK_CONTROL) < 0)
	{
		RECT r ={0,0};
		GetClientRect(&r);
		SIZE s = SIZEFROMRECT(r);

		// FIX: send an mouse up before we hand over control to the
		//      propertysheets to let the desklet clean up any capturing.
		docklet->MouseUp(&pt, &s, static_cast<DWORD>(wParam));

		if(m_CloseButton!= NULL && m_CloseButton->IsWindowVisible())
			m_CloseButton->Hide();
		AsyncDeskletPropertySheet(NULL); // or m_hWnd as owner, now, but it won't block the desklet.
	}
	else
	{
		RECT r ={0,0};
		GetClientRect(&r);
		SIZE s = SIZEFROMRECT(r);

		bool handled  = docklet->MouseUp(&pt, &s, static_cast<DWORD>(wParam));
		if(!handled && GetRealClickingMode() == CLICKMODE_SINGLE && !GetIgnoreLeftClicks())
		{				
			docklet->LeftButtonClick(&pt,&s);
		}
	}

	ReleaseCapture();

	return 0;
}

LRESULT Docklet::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!::IsWindow(m_hWnd) || !isInitialized)
		return 0;

	bHandled = TRUE;
	
	docklet->Select();
	
	if(label.NeedsRedrawOnSelect() )
		UpdateLabelWindow();

	return 0;
}

LRESULT Docklet::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!::IsWindow(m_hWnd) || !isInitialized)
		return 0;

	bHandled = TRUE;
	
	docklet->Deselect();

	if(::IsWindow(label) && label.NeedsRedrawOnSelect() )
		UpdateLabelWindow();

	return 0;
}

LRESULT Docklet::OnLeftButtonDoubleClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

	bHandled = TRUE;

	if(GetRealClickingMode() == CLICKMODE_DOUBLE && !GetIgnoreLeftClicks() )
	{
		RECT r = {0};
		GetClientRect(&r);
		SIZE s = SIZEFROMRECT(r);

		docklet->LeftButtonClick(&pt,&s);
	}

	return 0;
}

LRESULT Docklet::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	if(moving)
	{	
		DWORD msgPos = GetMessagePos();
		int xPos = GET_X_LPARAM(msgPos);
		int yPos = GET_Y_LPARAM(msgPos);

		int virtKey = (int)wParam;
		if(VK_ESCAPE == virtKey)
		{

			DWORD msgPos = GetMessagePos();
			m_StartMovingCursorPos.x = xPos;
			m_StartMovingCursorPos.y = yPos;

			OnMovingMouse();
			EndMoving();
		}
		else if(VK_LEFT == virtKey)
			SetCursorPos(xPos - 10, yPos);
		else if(VK_RIGHT == virtKey)
			SetCursorPos(xPos + 10, yPos);
		else if(VK_UP == virtKey)
			SetCursorPos(xPos, yPos - 10);
		else if(VK_DOWN == virtKey)
			SetCursorPos(xPos, yPos + 10);

		return 0;
	}

	if(	m_CloseButton != NULL &&
		appearanceTabHwnd == NULL &&
		GetKeyState(VK_CONTROL) < 0 &&
		GetUseClosePoint() &&
		!m_CloseButton->IsWindowVisible())
	{
		m_CloseButton->Show();
	}

	return 0;
}

LRESULT Docklet::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(moving)
	{
		OnMovingMouse();
		return 0;
	}

	if(m_FlippingData.isFlipping)
		return 0;

	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

	bHandled = TRUE;
	
	if(!mouseIsOver )
	{
		mouseIsOver = true;

		if(m_CloseButton != NULL &&
			appearanceTabHwnd == NULL &&
			GetUseClosePoint() &&
			(GetIsShowCased() || GetKeyState(VK_CONTROL) < 0))
		{
			m_CloseButton->Show();
		}

		TRACKMOUSEEVENT tme = {0};
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		BOOL bRet = TrackMouseEvent(&tme);

		if(_globalSettings.GetHoverSelection() )
		{
			TCHAR className[MAX_PATH] = {0};
			HWND foregroundHwnd = GetForegroundWindow();
			GetClassName(foregroundHwnd,className,MAX_PATH);
			DWORD foregroundWindowPID(0);
			GetWindowThreadProcessId(foregroundHwnd,&foregroundWindowPID);

			// check if the Progman window was being the foreground window, only then we would switch to 
			// desktop-icon style for hover selecting, just to match Windows Behaviour on this point.

			// FIX: sometimes the classname of the window that owns the progman-desktop is WorkerW
			if(GetStyle() != STYLE_ICON || GetCurrentProcessId() == foregroundWindowPID ||
					_tcscmp(className,TEXT("Progman")) == 0 || _tcscmp(className,TEXT("WorkerW")) == 0)
				::SetTimer(m_hWnd,FOCUS_TIMER,_globalSettings.GetHoverSelectionDelay(),SelectionTimerProc);
		}
		
		docklet->SetMouseOver(true);
		docklet->MouseOn();

		if(GetLabelType() == LABEL_REGULAR && GetLabelOnlyOnMouseOver())
		{
			label.SetFadingIn(true);
			label.SetTimer(FADER_TIMER,labelFaderSpeed);
		}
	}

	docklet->MouseMove(pt.x,pt.y);

	return 0;
}

VOID CALLBACK Docklet::SelectionTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
	Docklet* d = FindDocklet(hwnd);
	if(d)
	{
		::KillTimer(hwnd,FOCUS_TIMER);
		HMODULE hMod = LoadLibrary(_T("user32.dll"));
		VOID (__stdcall *SwitchToThisWindow)(HWND,BOOL) = (VOID (__stdcall *)(HWND,BOOL)) GetProcAddress(hMod,"SwitchToThisWindow");
		SwitchToThisWindow(hwnd,FALSE);
		FreeLibrary(hMod);

		/*if(d->GetMouseIsOver())
		{
			BOOL bHandled(FALSE);
			d->OnSetFocus(WM_SETFOCUS,0L,0L,bHandled);
		}
		else
		{
			BOOL bHandled(FALSE);
			d->OnKillFocus(WM_KILLFOCUS,0L,0L,bHandled);
		}*/
	}

}

LRESULT Docklet::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	
	if( mouseIsOver )
	{
		mouseIsOver = false;

		if(m_CloseButton != NULL && GetUseClosePoint())
			m_CloseButton->Hide();

		//if(_globalSettings.GetHoverSelection() )
		//	::SetTimer(m_hWnd,FOCUS_TIMER,_globalSettings.GetHoverSelectionDelay(), SelectionTimerProc);

		//SetCursor(oldCursor);

		docklet->SetMouseOver(false);
		docklet->MouseOut();

		if(GetLabelType() == LABEL_REGULAR && GetLabelOnlyOnMouseOver() )
		{
			label.SetFadingIn(false);
			::SetTimer(labelHwnd,FADER_TIMER,labelFaderSpeed,NULL);
		}
	}

	return 0;
}

LRESULT Docklet::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	
	WORD fwKeys = LOWORD(wParam);
	int zDelta = (short) HIWORD(wParam);
	int xPos = (short) GET_X_LPARAM(lParam);
	int yPos = (short) GET_Y_LPARAM(lParam);

	// FIX: mouse wheel coordinates are actually in screen-coordinates, not client coordinates
	//      so they need to be transformed before passing them on.
	POINT p = {xPos, yPos};
	ScreenToClient(&p);
	xPos = p.x;
	yPos = p.y;

	if(zDelta > 0)
		docklet->MouseWheelForward(fwKeys,zDelta/max(WHEEL_DELTA,1),xPos,yPos);
	else
		docklet->MouseWheelBackward(fwKeys,zDelta/max(WHEEL_DELTA,1),xPos,yPos);

	return 0;
}

LRESULT Docklet::OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	moving = false;

	bHandled = TRUE;

	RECT r = {0};
	GetWindowRect(&r);

	//if(r)
	//{
		if(GetSnapToGrid())
		{
			POINT p = {0};
			GetCursorPos(&p);

			r.left = int((p.x)/ _globalSettings.GetGridX())*_globalSettings.GetGridX() + _globalSettings.GetGridXMargin();
			r.top = int((p.y) / _globalSettings.GetGridY())*_globalSettings.GetGridY() + _globalSettings.GetGridYMargin();

			if(r.left < _globalSettings.GetGridXMargin())r.left = _globalSettings.GetGridXMargin();
			if(r.top < _globalSettings.GetGridYMargin())r.top = _globalSettings.GetGridXMargin();


			RECT r2 = r;
			if(docklet->GetDrawShadow() )
			{
				r2.left -= Layer::ratio/10;
				r2.top -=  Layer::ratio/10;
			}
			//UpdateLayeredWindow(docklet->GetHWnd(), NULL, &p2, NULL, NULL, NULL, 0, NULL, 0);
			POINT nP = {r2.left, r2.top};
			UpdateLayeredWindow(GetHWnd(), NULL,  &nP, NULL, NULL, NULL, 0, NULL, 0);
			//SetWindowPos(NULL,&r2,SWP_NOSIZE | SWP_NOZORDER);

			/*if(docklet->GetDrawShadow() )
			{
				r.left =r.left - Layer::ratio/10;
				r.top  = r.top - Layer::ratio/10;
			}*/
		}


	docklet->SetUncorrectedPos(r.left,r.top);

	if(m_CloseButton != NULL)
		m_CloseButton->ResetPosition();

	/*
	int l = r.left;
	int t = r.top;

	if(docklet->GetDrawShadow())
	{
		l += Layer::ratio/10;
		t += Layer::ratio/10;
	}

	int rot = docklet->GetRotation();
	if(rot != 0)
	{
		SIZE s = docklet->GetSize();

		POINT p = {0,0};
		FixRotation(rot,s,p);
		l += p.x;
		t += p.y;
	}

	docklet->SetPos(l,t);
	*/


	//}


	DrawPluginInstance* dp = docklet->GetDrawPlugin();
	if(dp)
		dp->Event(EVENT_ONENDMOVE);

	// CHANGED: we don't change the alpha anymore on move, so setting it back isn't 
	//			needed neither anymore.
	//docklet->SetAlpha(oldAlpha);
	//d->Redraw();

	UpdateLabelPosition();

	NotifyAll(true,static_cast<LPARAM>(OM_PositionChanged),reinterpret_cast<LPARAM>(m_hWnd));

	return 0;
}

LRESULT Docklet::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	
	// CHANGED: we don't change the alpha any more on move.
	//d->GetDocklet()->SetAlpha(125);
	//d->Redraw();

	
	RECT* r=(RECT*)lParam;
	if(NULL == r)
		return 0;

	CDeskletHooks* hooks = CDeskletHooks::GetInstance();
	if(hooks != NULL)
		hooks->onPositionChange(m_hWnd, r);

	docklet->SetUncorrectedPos(r->left,r->top);

	UpdateLabelPosition();
	NotifyAll(true,static_cast<LPARAM>(OM_PositionChanged),reinterpret_cast<LPARAM>(m_hWnd));

	if(m_CloseButton != NULL)
		m_CloseButton->ResetPosition();

	return 0;
}

LRESULT Docklet::OnWindowsPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

	bHandled = TRUE;

	if(isInitialized && !isDestroying)
	{
		CDeskletStyles::DESKLET_STYLE_CHANGE_DATA dscd = {0};
		dscd.dwSize = sizeof(dscd);
		dscd.oldStyle = style;
		dscd.newStyle = style;
		dscd.deskletHwnd = m_hWnd;
		dscd.labelHwnd = labelHwnd;
		CDeskletStyles* styles = CDeskletStyles::GetInstance();
		MSG msg = m_Msg;
		msg.message = WM_WINDOWPOSCHANGING;
		msg.lParam = lParam;
		msg.wParam = wParam;
		styles->OnMessage(&dscd, &msg);
	}

	
	WINDOWPOS* w = (WINDOWPOS*)lParam;
	if(w != NULL && w->flags & SWP_SHOWWINDOW)
	{
		if(docklet)
			docklet->VisibilityNotify(TRUE);

		if(isShowCased && docklet->GetIsDirty())
			Redraw(true);
	}
	else if(w != NULL && w->flags & SWP_HIDEWINDOW)
	{
		if(m_CloseButton != NULL)
			m_CloseButton->NotifyDeskletHide();

		if(docklet)
			docklet->VisibilityNotify(FALSE,0);
	}

	if(w != NULL && !GetIsShowCased() && GetStyle() == STYLE_BOTTOM)
	{
		w->hwndInsertAfter = HWND_BOTTOM;
	}

	if(w != NULL && _ShowCase && (w->flags & SWP_NOZORDER) != 0 
		&& (GetIsShowCased() || GetStyle() == STYLE_SHOWCASE))
	{
		::SetWindowPos(_ShowCase->m_hWnd, w->hwnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	}

	/*
	if(!(w->flags & SWP_NOZORDER))
	{
		if(m_CloseButton != NULL)
			m_CloseButton->NotifyZOrderingChange(w->hwndInsertAfter, NULL);
	}*/

	return 0;
}

/*LRESULT Docklet::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::MessageBox(0,_T("destroyed"),0,0);
	return 0;
}*/

void Docklet::AsyncFade(bool fadingIn, DWORD ms)
{
	stopFading = true;
	WaitForSingleObject(faderThread,INFINITE);
	isFadingIn = fadingIn;

	stopFading = false;
	DWORD dummy(0);
	fadeMs = ms;
	faderThread = CreateThread(NULL,0,FaderThread,reinterpret_cast<LPVOID>(this),0,&dummy);
}

DWORD __stdcall Docklet::FaderThread(LPVOID lpData)
{
	Docklet* d = reinterpret_cast<Docklet*>(lpData);
	if(NULL == d)
	{
		ExitThread(1);
		return 1;
	}

	if(d->isFadingIn)
		d->FadeIn(d->fadeMs);
	else
		d->FadeOut(d->fadeMs);

	d->SetVisibility(d->isFadingIn);

	// CRITICALSECTION!
	CloseHandle(d->faderThread);
	d->faderThread = NULL;


	ExitThread(0);
	return 0;
}


void Docklet::FadeIn(DWORD ms)
{
	//if(!isVisible)
	//	return;
		
	if(style == STYLE_SHOWCASE && isShowCased == false)
		return;
	
	POINT p = docklet->GetCorrectedPos();
	SIZE  s = docklet->GetCorrectedSize();

	int pixels = s.cx * s.cy;
	//int add = pixels / 3000;
	//if(add < 1)add = 1;
	//int add = 12;

	SetNoRedraw(true);

	Bitmap* bmp = docklet->Draw();
	SetBitmap(bmp,0,p,s);


	DWORD maxAnimTime = ms;
	if(-1 == maxAnimTime)
		maxAnimTime = _AnimTimeInMS;

	DWORD timeDone = 0;
	int startFrame = 0;
	int endFrame = docklet->GetAlpha();
	int diffFrame = endFrame - startFrame;
	float add = 1;

	float curFrame = static_cast<float>(startFrame);

	while(!stopFading && static_cast<int>(curFrame) < endFrame && timeDone < maxAnimTime)
	{
		LARGE_INTEGER hpCounterFrequency = {0};
		QueryPerformanceFrequency(&hpCounterFrequency);

		LARGE_INTEGER hpCounterStart = {0};
		QueryPerformanceCounter(&hpCounterStart);

		DWORD startTime = GetTickCount();

		SetAlpha(static_cast<BYTE>(curFrame));
		if(curFrame == 0)
		{
			if(GetIsShowCased())
			{
				SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

				if(!GetLabelOnlyOnMouseOver() && labelType == LABEL_REGULAR)
				{
					::SetWindowPos(labelHwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				}
			}
			else
			{
				ShowWindow(SW_NORMAL);
				if(!GetLabelOnlyOnMouseOver() && labelType == LABEL_REGULAR)
					label.ShowWindow(SW_NORMAL);
			}
		}

		DWORD frameLength = GetTickCount() - startTime;

		LARGE_INTEGER hpCounterEnd = {0};
		QueryPerformanceCounter(&hpCounterEnd);

		float hpLength = float(hpCounterEnd.QuadPart - hpCounterStart.QuadPart) / float(hpCounterFrequency.QuadPart);
		hpLength *= 1000;

		timeDone += (DWORD)hpLength;
		float f = static_cast<float>(hpLength) / static_cast<float>(maxAnimTime);
		add = diffFrame *f;
		if(add == 0)
			add = 1;

		curFrame += add;
	}

	if(!stopFading)
		SetAlpha((BYTE)endFrame);

	SetNoRedraw(false);
}

void Docklet::FadeOut(DWORD ms)
{
	//if(!isVisible)
	//	return;
		
	if(style == STYLE_SHOWCASE && isShowCased == false)
		return;

	
	POINT p = docklet->GetCorrectedPos();
	SIZE  s = docklet->GetCorrectedSize();

	int pixels = s.cx * s.cy;
	//int add = pixels / 3000;
	//if(add < 1)add = 1;
	//int add = 12;

	Bitmap* bmp = docklet->Draw();
	SetBitmap(bmp,docklet->GetAlpha(),p,s);	
	
	DWORD maxAnimTime = ms;
	if(-1 == maxAnimTime)
		maxAnimTime = _AnimTimeInMS;

	DWORD timeDone = 0;
	int startFrame = docklet->GetAlpha();
	int endFrame = 0;
	int diffFrame = endFrame - startFrame;
	float add = -1;

	float curFrame = static_cast<float>(startFrame);

	while(!stopFading && static_cast<int>(curFrame) > endFrame && timeDone < maxAnimTime)
	{
		LARGE_INTEGER hpCounterFrequency = {0};
		QueryPerformanceFrequency(&hpCounterFrequency);

		LARGE_INTEGER hpCounterStart = {0};
		QueryPerformanceCounter(&hpCounterStart);

		DWORD startTime = GetTickCount();

		SetAlpha(static_cast<BYTE>(curFrame));

		DWORD frameLength = GetTickCount() - startTime;

		LARGE_INTEGER hpCounterEnd = {0};
		QueryPerformanceCounter(&hpCounterEnd);

		float hpLength = float(hpCounterEnd.QuadPart - hpCounterStart.QuadPart) / float(hpCounterFrequency.QuadPart);
		hpLength *= 1000;

		timeDone += (DWORD)hpLength;
		float f = static_cast<float>(hpLength) / static_cast<float>(maxAnimTime);
		add = diffFrame *f;
		if(add == 0)
			add = -1;

		curFrame += add;
		if(curFrame < endFrame)
			curFrame = (float)endFrame;
	}

	if(!stopFading)
	{
		//ShowWindow(SW_HIDE);
		//label.ShowWindow(SW_HIDE);
	}

	docklet->SetIsDirty(false);

}

void Docklet::OnFinalMessage(HWND hwnd)
{
	//Docklets.erase(hwnd);
	//Release();
}

LRESULT Docklet::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	BOOL fShow = (BOOL) wParam;

	docklet->VisibilityNotify(fShow,0);

	if(isShowCased && fShow && docklet->GetIsDirty())
	{
		Redraw(true);
	}

	return 0;
}

LRESULT Docklet::OnAveSaveAction(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ISaver* saver = reinterpret_cast<ISaver*>(wParam);

	if( NULL == saver)
		return static_cast<LRESULT>(E_FAIL);
	else
	{
		HANDLE h = reinterpret_cast<HANDLE>(lParam);

		HRESULT hRes = (Save(*saver));

		if(NULL != h)
			SetEvent(h);

		return static_cast<LRESULT>(hRes);
		
	}
}

bool Docklet::Configure()
{
	if(!docklet->HasCustomOnConfigure())
		return false;

	if(GetCurrentThreadId() == threadId || GetIsDefaultValues())
		return docklet->Configure();
	else
		return SendMessage(WM_AVEACTION,0L,AVEACTION_CONFIGURE) == TRUE;
}

LRESULT Docklet::OnAveAction(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(lParam == AVEACTION_CONFIGURE)
	{
		return docklet->Configure() ? TRUE : FALSE;
	}
	if(lParam == AVEACTION_FADEIN)
	{
		DWORD ms = static_cast<DWORD>(wParam);
		stopFading = false;
		FadeIn(ms);
	}
	else if(lParam == AVEACTION_FADEOUT)
	{
		DWORD ms = static_cast<DWORD>(wParam);
		stopFading = false;
		FadeOut(ms);
	}
	else if(lParam == AVEACTION_REMOVE)
	{
		PostQuitMessage(0);
	}
	else if(lParam == AVEACTION_REMOVEANIMATED)
	{
		StartSucking(true);
	}
	else if(lParam == AVEACTION_CONFIGDLG)
	{
		PROPSHEETPARAMS* propSheetParams = reinterpret_cast<PROPSHEETPARAMS*>(wParam);
		if(NULL != propSheetParams)
		{
			return static_cast<LRESULT>(DeskletPropertySheet(propSheetParams->owner,propSheetParams->pds,propSheetParams->noAddPropSheets));
		}
		else
		{
			return static_cast<LRESULT>(DeskletPropertySheet(NULL) );
		}
	}
	else if(lParam == AVEACTION_GETSAFEOBJECT)
	{
		// script dispatches need to be accessed on the same thread
		// that created them, hence we marshal the interface
		// across single threaded apartment boundaries
		IStream *pStr = NULL;
		IDispatch *pDisp = (IDispatch*)wParam;
		HRESULT hr = S_OK;
		if( pDisp )
		{
			hr = CoMarshalInterThreadInterfaceInStream(
				IID_IDispatch, pDisp, &pStr);
			if( pStr )
			{
				pDisp->Release();
				return (LRESULT)pStr;
			}
		}
	}
	else if(AVEACTION_GETSAFEELEM == lParam)
	{
		safeElem * se = (safeElem*)wParam;
		se->Marshal();
	}
	else if(AVEACTION_REDRAW == lParam)
	{
		Redraw(wParam != FALSE);
	}


	return 0;
}

HRESULT Docklet::SaveAsync(ISaver &saver)
{
	//CTrayIconAnimation trayAnim(_App->GetMainWindowHandle(), TRUE);

	if(isCrashed || GetCurrentThreadId() == threadId || 	GetIsDefaultValues())
		return Save(saver);
	else
	{
		std::wstringstream ws;
		ws << L"SaverEvent" << this;
		HANDLE h = CreateEventW(NULL,FALSE,FALSE,ws.str().c_str());

		HRESULT hRes = static_cast<HRESULT>(PostMessage(WM_AVESAVEACTION,reinterpret_cast<WPARAM>(&saver),reinterpret_cast<LPARAM>(h)));

		WaitForSingleObject(h,INFINITE);
		CloseHandle(h);

		return hRes;
	}
}

LRESULT Docklet::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);


	bool force = (lParam != FALSE);

	if(isInDestroyedState || noRedraw)
		return 1;


	if(!force)
	{
		if(!isVisible)
		{
			docklet->SetIsDirty(true);
			return 1;
		}
		
		if(style == STYLE_SHOWCASE && !isShowCased)
		{
			docklet->SetIsDirty(true);
			return 1;
		}
	}

	
	Bitmap* bmp = docklet->Draw();
	
	POINT p = docklet->GetCorrectedPos();
	SIZE  s = docklet->GetCorrectedSize();

	SetBitmap(bmp,docklet->GetAlpha(),p,s);

	NotifyAll(true,static_cast<LPARAM>(OM_Repaint),reinterpret_cast<LPARAM>(GetHWnd()));

	docklet->SetIsDirty(false);


	return 1;
}

void Docklet::SetAlpha(BYTE bAlpha)
{
	if( isCrashed )
		return;
	
	if(bAlpha == 255)
	{
		POINT dPos = docklet->GetCorrectedPos();
		SIZE dSize = docklet->GetCorrectedSize();
		SetBitmap(docklet->Draw(),255,&dPos,&dSize );
	}
	else
		CLayeredWindowImpl<Docklet>::SetAlpha(bAlpha);

		
	label.UpdateAlphaOnly(bAlpha);
}

BOOL Docklet::WaitForDockletBeingInitialized(DWORD dwMilliseconds)
{
	if(!isInitializedEvent)
		return FALSE;

	DWORD res = WaitForSingleObject(isInitializedEvent,dwMilliseconds);

	return res == WAIT_OBJECT_0;
}

std::wstring Docklet::GetRelativeDLLFile()
{
	WCHAR dockletFile[MAX_PATH] = {0};
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	GetModuleFileNameW(GetDocklet()->GetModule(),dockletFile,sizeof(dockletFile)/sizeof(dockletFile[0]));
	lowercaseW(path);
	lowercaseW(dockletFile);
	wcscpy(dockletFile,dockletFile+wcslen(path));

	return dockletFile;

}

HRESULT Docklet::SaveToSingleFile(const WCHAR* fileName)
{
	if(NULL == fileName )
		return E_INVALIDARG;

	
	CXMLSaver saver(L"AveDesk");
	try
	{
		CHECKHRES(saver.AddNode(L"Desklet"));
		CHECKHRES(saver.SaveString(GetRelativeDLLFile(),L"DLL"));
		CHECKHRES(SaveAsync(saver));
	}
	catch(HRESULT& hRes)
	{
		return hRes;
	}

	Bitmap* bmp = docklet->Draw();
	boost::shared_ptr<Bitmap> previewBitmap;
	if(bmp != NULL)
		previewBitmap = boost::shared_ptr<Bitmap>( (Bitmap*)( ((Image*)bmp)->Clone() ) );

	if(_globalSettings.GetAddPreviewInThemeFiles() && previewBitmap.get() != NULL)
	{
		DeleteFile(fileName);
		try
		{
			HRESULT hRes = S_OK;
			CComPtr<IStorage> storage;
			hRes = StgCreateStorageEx(fileName,	STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_TRANSACTED,
						STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, (void**)&storage);

			if(SUCCEEDED(hRes) && storage.p != NULL)
			{
				HRESULT hRes = S_OK;
				CComPtr<IStream> deskletsStream;
				if(FAILED(hRes = storage->CreateStream(L"Desklets", STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, NULL, &deskletsStream)))
					throw hRes;

				if(NULL == deskletsStream.p)
					throw (hRes = E_FAIL);

				if(FAILED(hRes = saver.SaveToStream(deskletsStream)))
					throw hRes;

				CComPtr<IStream> previewStream;
				if(FAILED(hRes = storage->CreateStream(L"Preview", STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, NULL, &previewStream)))
					throw hRes;

				if(NULL == previewStream.p)
					throw (hRes = E_FAIL);

				CLSID encoderCLSID = {0};
				GetEncoderClsid(L"image/png",&encoderCLSID);

				if(previewBitmap.get() != NULL)
					previewBitmap->Save(previewStream,&encoderCLSID, NULL);

				if(FAILED(hRes = storage->Commit(STGC_DEFAULT)))
					throw hRes;
				
			}
		}
		catch(HRESULT&)
		{
			return saver.Save(fileName);
		}
	}
	else
	{
		return saver.Save(fileName);
	}

	return S_OK;

}

HRESULT Docklet::LoadFromXMLNode(CComPtr<IXMLDOMNode>& node, Docklet** docklet, BOOL makeVisible)
{
	if(NULL == node.p)
		return E_INVALIDARG;
	try
	{
		CXMLLoader loader(node);
		CHECKHRES(loader.SelectChildNode(L"Desklet"));

		std::wstring dockletDLLFile;
		CHECKHRES(loader.LoadString(dockletDLLFile,L"DLL"));
		WCHAR path[MAX_PATH];
		wcscpy(path, _App->GetAveDeskDataDir().c_str());
		wcscat(path,dockletDLLFile.c_str());

		Docklet* d = Docklet::LoadDocklet(path,&loader,NULL,NULL,makeVisible == FALSE);

		if(NULL != d && NULL != docklet)
			*docklet = d;

		return S_OK;
	}
	catch(HRESULT& hRes)
	{
		return hRes;
	}
}

HRESULT Docklet::LoadFromSingleFile(const WCHAR* fileName, Docklet** docklet, BOOL makeVisible)
{
	if(NULL == fileName)
		return E_INVALIDARG;

	boost::shared_ptr<CXMLLoader> xmlLoader;

	if(StgIsStorageFile(fileName) == S_OK) // explicit check for S_OK
	{
		HRESULT hRes = S_OK;
		CComPtr<IStorage> storage;
		hRes = StgOpenStorageEx(fileName, STGM_READ | STGM_DIRECT | STGM_SHARE_DENY_WRITE, STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, reinterpret_cast<void**>(&storage));
		if(SUCCEEDED(hRes) && storage.p != NULL)
		{
			CComPtr<IStream> stream;
			hRes = storage->OpenStream(L"Desklets", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, &stream);
			if(SUCCEEDED(hRes) && stream.p != NULL)
			{
				xmlLoader = boost::shared_ptr<CXMLLoader>(new CXMLLoader(stream));
				xmlLoader->SelectChildNode(L"AveDesk");
			}
		}
	}
	else
	{
		xmlLoader = boost::shared_ptr<CXMLLoader>(new CXMLLoader(fileName, L"AveDesk"));
	}

	try
	{
		CHECKHRES(xmlLoader->SelectChildNode(L"Desklet"));

		std::wstring dockletDLLFile;
		CHECKHRES(xmlLoader->LoadString(dockletDLLFile,L"DLL"));
		WCHAR path[MAX_PATH];
		wcscpy(path, _App->GetAveDeskDataDir().c_str());
		wcscat(path,dockletDLLFile.c_str());

		Docklet* d = Docklet::LoadDocklet(path,xmlLoader.get(),NULL,NULL,makeVisible == FALSE);

		if(NULL != d && NULL != docklet)
			*docklet = d;

		return S_OK;
	}
	catch(HRESULT& hRes)
	{
		return hRes;
	}
}

void Docklet::NotifyEvent(OM_Event event)
{
	NotifyAll(true,static_cast<LPARAM>(event),reinterpret_cast<LPARAM>(m_hWnd));
}

void Docklet::NotifyGlobalDockletsEvent(OM_Event event, LPARAM val2)
{
	if(s_GlobalDockletsEventNotifier != NULL)
	{
		s_GlobalDockletsEventNotifier->NotifyAll(true,static_cast<LPARAM>(event),val2);	
	}
}

IObservable* Docklet::GetGlobalDockletsEventNotifier()
{
	// TODO: make thread safe etc..
	if( NULL == s_GlobalDockletsEventNotifier)
	{
		s_GlobalDockletsEventNotifier = new CObservable;
	}

	return s_GlobalDockletsEventNotifier;
}


int Docklet::SetMainLabel(const std::wstring& val)
{
	std::wstring oldLabel = GetLabel().GetMainCaption();

	GetLabel().SetMainCaption(val);

	if(val !=  oldLabel)
	{
		NotifyEvent(OM_CaptionChanged);
		UpdateLabelWindow();
	}

	return val.length();
}

void Docklet::SetSkinInfo(SkinInfo* wrapper)
{
	if(NULL == wrapper)
		m_SkinInfo.Clear();
	else
		m_SkinInfo = DeskletSkinInfo(*wrapper);

	NotifyEvent(OM_SkinChanged);
}

DeskletSkinInfo& Docklet::GetSkinInfo()
{
	return m_SkinInfo;
}

void Docklet::SetImageCannotBeSet(bool value)
{
	if(m_ImageCannotBeSet != value)
	{
		m_ImageCannotBeSet = value;
		NotifyAll(true,static_cast<LPARAM>(OM_ImageCannotBeSetChanged),static_cast<LPARAM>(value ? TRUE: FALSE));
	}

}

bool Docklet::GetImageCannotBeSet() const
{
	return m_ImageCannotBeSet;
}

HCURSOR Docklet::ChangeCursor(HCURSOR hCursor)
{
	oldCursor = normalCursor;
	normalCursor = hCursor;
	return SetCursor(normalCursor);
}

LRESULT Docklet::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if(_globalSettings.GetHoverSelection() )
		SetCursor(hoverCursor);
	else
		SetCursor(normalCursor);

	return TRUE;
}

bool Docklet::BeforeRemoval()
{
	return docklet->BeforeRemoval();
}