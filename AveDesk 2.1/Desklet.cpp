// Desklet.cpp : Implementation of CDesklet
#include "stdafx.h"
#include "AveDesk.h"
#include "Desklet.h"
#include "docklet.h"
#include "application.h"
#include "DeskletSkin.h"

/////////////////////////////////////////////////////////////////////////////
// CDesklet

CDesklet::~CDesklet()
{
	pApp.Release();  
	if( colorMatrix )
		colorMatrix->Release();
	if( matrix )
		matrix->Release();
	if( layers )
		layers->Release();
}

STDMETHODIMP CDesklet::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDesklet
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
    if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CDesklet::get_Label(BSTR *pVal)
{
  //WCHAR tmp[1024];
  //GetWindowTextW(m_owner->GetHWnd(), tmp, 1024);


  *pVal = SysAllocString(m_owner->GetLabel().GetMainCaption().c_str());
	return S_OK;
}

STDMETHODIMP CDesklet::put_Label(BSTR newVal)
{
  //SetWindowTextW(m_owner->GetHWnd(), newVal);
	m_owner->SetMainLabel(static_cast<WCHAR*>(newVal));
	return S_OK;
}

STDMETHODIMP CDesklet::get_SubLabel(BSTR *pVal)
{
	*pVal = SysAllocString(m_owner->GetLabel().GetSubCaption().c_str() );
	return S_OK;
}

STDMETHODIMP CDesklet::put_SubLabel(BSTR newVal)
{
	m_owner->GetLabel().SetSubCaption( static_cast<WCHAR*>(newVal) );
	//wcscpy(m_owner->GetLabel().GetSubLabelCap, newVal);
	if(m_owner->GetLabel().GetShowSubLabel() )
		m_owner->UpdateLabelWindow();
	return S_OK;
}

STDMETHODIMP CDesklet::get_Application(IApplication **pVal)
{
	if( !pApp.p )
		_App->GetSafeInstance(&pApp.p);
	pApp.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CDesklet::Redraw()
{
	m_owner->GetDocklet()->SetIsDirty(true);
	m_owner->Redraw();

	return S_OK;
}

STDMETHODIMP CDesklet::get_ColorMatrix(IColorMatrix **pVal)
{
  if( NULL == colorMatrix )
  {
    CComObject<CColorMatrix>::CreateInstance(&colorMatrix);
    colorMatrix->AddRef();
  }/*
  if( m_owner->GetDocklet()->GetUseColorization() )
  {
    colorMatrix->Set(*m_owner->GetDocklet()->GetColorMatrix());
  }*/
  if( colorMatrix )
    colorMatrix->QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CDesklet::get_PositionMatrix(IMatrix **pVal)
{
  if( NULL == matrix )
  {
    CComObject<CMatrix>::CreateInstance(&matrix);
    matrix->AddRef();
  }
  if( matrix )
    matrix->QueryInterface(pVal);
	return S_OK;
}


STDMETHODIMP CDesklet::get_Visible(VARIANT_BOOL *pVal)
{
	if( m_owner->GetIsVisible() )
		*pVal = VARIANT_TRUE;
	else
		*pVal = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CDesklet::put_Visible(VARIANT_BOOL newVal)
{
	m_owner->SetVisibility( VARIANT_TRUE == newVal);

	return S_OK;
}

STDMETHODIMP CDesklet::get_Alpha(int *pVal)
{
	*pVal = m_owner->GetDocklet()->GetAlpha();

	return S_OK;
}

STDMETHODIMP CDesklet::put_Alpha(int newVal)
{
	m_owner->GetDocklet()->SetAlpha((BYTE)newVal);
	m_owner->SetAlpha((BYTE)newVal);

	return S_OK;
}

STDMETHODIMP CDesklet::get_Rotation(int *pVal)
{
	*pVal = m_owner->GetDocklet()->GetRotation();

	return S_OK;
}

STDMETHODIMP CDesklet::put_Rotation(int newVal)
{
	m_owner->GetDocklet()->SetRotation(newVal);

	return S_OK;
}

extern void CALLBACK DeskletSetPosition(HWND hwndDesklet,POINT *p);
STDMETHODIMP CDesklet::get_Left(int *pVal)
{
	*pVal = m_owner->GetDocklet()->GetPos().x;
	return S_OK;
}

STDMETHODIMP CDesklet::put_Left(int newVal)
{
	// more roundtrips but less code
	POINT p = m_owner->GetDocklet()->GetPos();
  if( p.x != newVal )
  {
    p.x = newVal;
    DeskletSetPosition(m_owner->GetHWnd(), &p);
  }
	return S_OK;
}

STDMETHODIMP CDesklet::get_Top(int *pVal)
{
	*pVal = m_owner->GetDocklet()->GetPos().y;
	return S_OK;
}

STDMETHODIMP CDesklet::put_Top(int newVal)
{
	// more roundtrips but less code
	POINT p = m_owner->GetDocklet()->GetPos();
  if( p.y != newVal )
  {
    p.y = newVal;
    DeskletSetPosition(m_owner->GetHWnd(), &p);
  }
	return S_OK;
}

extern void CALLBACK DeskletSetSize(HWND hwndDesklet,SIZE *s);

STDMETHODIMP CDesklet::get_Width(int *pVal)
{
	*pVal = m_owner->GetDocklet()->GetSize().cx;
	return S_OK;
}

STDMETHODIMP CDesklet::put_Width(int newVal)
{
	SIZE  s = m_owner->GetDocklet()->GetSize();
  if( s.cx != newVal )
  {
    s.cx = newVal;
    DeskletSetSize(m_owner->GetHWnd(), &s);
  }
	return S_OK;
}

STDMETHODIMP CDesklet::get_Height(int *pVal)
{
	*pVal = m_owner->GetDocklet()->GetSize().cy;
	return S_OK;
}

STDMETHODIMP CDesklet::put_Height(int newVal)
{
	SIZE  s = m_owner->GetDocklet()->GetSize();
  if( s.cy != newVal )
  {
    s.cy = newVal;
    DeskletSetSize(m_owner->GetHWnd(), &s);
  }
	return S_OK;
}

template<typename T> inline void SetValIfNotNull(T* t,const T& v)
{
	if(t)
	{
		*t = v;
	}
}
// ave 30/05: added a parameter with default value to check if
// the default value was actually returned

inline int ExtractInt(VARIANT* src, int defval,BOOL* bReturnedDefVal=NULL)
{
  SetValIfNotNull(bReturnedDefVal,FALSE);
  if( !src )
  {
	SetValIfNotNull(bReturnedDefVal,TRUE);
    return defval;
  }
  CComVariant var = *src;
  if( var.vt == VT_EMPTY )
  {
	SetValIfNotNull(bReturnedDefVal,TRUE);
    return defval;
  }
  if( var.vt == VT_I4 )
    return var.lVal;
  if( SUCCEEDED( var.ChangeType(VT_I4) ) )
    return var.lVal;

  SetValIfNotNull(bReturnedDefVal,TRUE);
  return defval;
}

extern void CALLBACK DeskletMove(HWND hwndDesklet, RECT* r, BOOL redraw);

STDMETHODIMP CDesklet::Move(int Left, VARIANT *Top, VARIANT *Width, VARIANT *Height)
{
	// fix 30/07 -ave: no more snapping position and then resizing

	// more roundtrips but less code
	POINT p = m_owner->GetDocklet()->GetPos();
	POINT p2 = p;
	p.x = Left;
	p.y = ExtractInt(Top, p.y);


	SIZE  s = m_owner->GetDocklet()->GetSize();
	SIZE  s2 = s;

	s.cx = ExtractInt(Width, s.cx);
	s.cy = ExtractInt(Height, s.cy);
 
	if( s.cx != s2.cx || s.cy != s2.cy )
	{
		RECT r = {0};
		r.left = p.x;
		r.top = p.y;
		r.right = p.x + s.cx;
		r.bottom = p.y + s.cy;
		m_owner->GetDocklet()->SetPos(p.x,p.y,false);
		m_owner->GetDocklet()->SetSize(s.cx,s.cy,false);
		m_owner->Redraw();
		//DeskletMove(m_owner->GetHWnd(),&r,TRUE);
	}
	else if( p.x != p2.x || p.y != p2.y )
    DeskletSetPosition(m_owner->GetHWnd(), &p);
	

	return S_OK;
}

STDMETHODIMP CDesklet::get_About(IDeskletInfo **pVal)
{
  m_owner->GetDLL()->GetWrapper(pVal);
  // dynamic_cast this, if needed
  about = *pVal;
	return S_OK;
}

STDMETHODIMP CDesklet::get_Layers(ILayers **pVal)
{
  if( NULL == layers )
  {
    CComObject<CoLayers>::CreateInstance(&layers);
    layers->AddRef();
    layers->docklet = m_owner;
  }
  if( layers )
    layers->QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CDesklet::PopFront()
{
  m_owner->ApplyStyle(STYLE_TOP);
	return S_OK;
}

STDMETHODIMP CDesklet::get_Style(DeskletStyles *pVal)
{
	*pVal = (DeskletStyles)m_owner->GetStyle();

	return S_OK;
}

STDMETHODIMP CDesklet::put_Style(DeskletStyles newVal)
{
	m_owner->ApplyStyle(newVal);

	return S_OK;
}

STDMETHODIMP CDesklet::Clone(IDesklet **pVal)
{
  HWND hw = NULL;
  Docklet *d = NULL;
  DuplicateDesklet(m_owner->GetHWnd(), FALSE, &hw);
  if( hw && IsWindow(hw) )
  {
    d = Docklet::FindDocklet(hw);        
    if( d )
      d->GetWrapper(pVal);
  }
	return S_OK;
}

STDMETHODIMP CDesklet::get_HWnd(int *pVal)
{
	*pVal = (int)m_owner->GetHWnd();
	return S_OK;
}

STDMETHODIMP CDesklet::get_ObjectModel(IDispatch **pVal)
{
	
	
	
	if( GetCurrentThreadId() == m_owner->GetThreadID() )
		m_owner->GetDocklet()->GetModel(pVal);
	else
	{
		HRESULT hr = S_OK;
		IDispatch *pDisp = this;
		
		IStream *pStr = NULL;
		pStr = (IStream*)::SendMessage(
			m_owner->GetDocklet()->GetWindow(),
			WM_AVEACTION,(WPARAM)pDisp,AVEACTION_GETSAFEOBJECT);

		if( pStr )
		{
			IDispatch *pDisp2 = NULL;
			hr = CoGetInterfaceAndReleaseStream(
				pStr, IID_IDispatch, (void**)&pDisp2);

			CComQIPtr<IDesklet> ths = pDisp2;
			if( ths.p )
				ths->get_ObjectModel(pVal);
			pDisp2->Release();
		}
	}
	return S_OK;
}

STDMETHODIMP CDesklet::get_Tag(VARIANT *pVal)
{
  VariantCopy(pVal, &tag);	

	return S_OK;
}

STDMETHODIMP CDesklet::put_Tag(VARIANT newVal)
{
	tag = newVal;

	return S_OK;
}

extern void CALLBACK DeskletFadeIn(HWND,BOOL,DWORD);
STDMETHODIMP CDesklet::FadeIn(/*[in]*/VARIANT* async, long durationInMs)
{

	if( NULL == m_owner)
		return E_FAIL;

	CComVariant var = *async;

	if(var.vt == VT_EMPTY)
	{
		DeskletFadeIn(m_owner->GetHWnd(),FALSE,durationInMs);
	}
	else if(var.vt == VT_BOOL)
	{
		DeskletFadeIn(m_owner->GetHWnd(), var.boolVal != VARIANT_FALSE, durationInMs);
	}
	else
	{
		return E_INVALIDARG;
	}

	return S_OK;
}


extern void CALLBACK DeskletFadeOut(HWND,BOOL, DWORD);
STDMETHODIMP CDesklet::FadeOut(VARIANT *async, long durationInMs)
{
	if( NULL == m_owner)
		return E_FAIL;

	CComVariant var = *async;

	if(var.vt == VT_EMPTY)
	{
		DeskletFadeOut(m_owner->GetHWnd(),FALSE, durationInMs);
	}
	else if(var.vt == VT_BOOL)
	{
		DeskletFadeOut(m_owner->GetHWnd(),var.boolVal != VARIANT_FALSE, durationInMs);
	}
	else
	{
		return E_INVALIDARG;
	}

	return S_OK;
}

STDMETHODIMP CDesklet::get_Skin(IDeskletSkin **pVal)
{
	if( NULL == m_owner)
		return E_FAIL;

	if(NULL == pVal)
		return E_INVALIDARG;

	
	CComObject<CDeskletSkin> *skin;
	CComObject<CDeskletSkin>::CreateInstance(&skin);
	skin->SetData(NULL, &m_owner->GetSkinInfo());
	return skin->QueryInterface(pVal);
}

STDMETHODIMP CDesklet::putref_Skin(IDeskletSkin *newVal)
{
	CComObject<CDeskletSkin> *ptr = dynamic_cast<CComObject<CDeskletSkin> *>(newVal);
	if(NULL == ptr)
		return E_INVALIDARG;

	DeskletSkinEnumerator* dse	= ptr->GetEnumerator();
	DeskletSkinInfo *dsi		= ptr->GetSkinInfo();
	SkinInfoWrapper* wrapper	= dsi->GetSkinInfoWrapper();

	bool result = m_owner->GetDocklet()->SetSkinW(dse, wrapper);

	DeleteSkinInfoWrapper(wrapper);

	return S_OK;
}
