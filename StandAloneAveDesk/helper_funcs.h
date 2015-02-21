// filename:		helper_funcs.h
// Description:		Prototypes of commonly used functions
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!
//
//


#ifndef HELPER_FUNCS_H_303001_
#define HELPER_FUNCS_H_303001_

#include "avedesk.h"
#include <functional>
#include <memory>
#include <string>
// DirectX Header Files
#include <d3dx9.h>

#include <gdiplus.h>
using namespace Gdiplus;

void DeleteDirectory(const std::wstring& path);
std::wstring CreateGUIDString();
BOOL GetIsRunningWin2k() ;
BOOL GetIsRunningVista();
int WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int iValue, LPCTSTR lpFileName);
void GetFileName(const char* filepath, char* buffer, int buffersize);
char* lowercase(char* s);
bool FileExists(const char* file);
void GetDLLPath(HMODULE h,char* path, DWORD nSize);
bool strcmp_nocase(const char* str1, const char* str2);
HGLOBAL GlobalClone(HGLOBAL hglobIn);
IUnknown *GetCanonicalIUnknown(IUnknown* punk);
Bitmap* HICONToGDIPlusBitmap(HICON icon);

void GetFileNameW(const WCHAR* filepath, WCHAR* buffer, int buffersize);
WCHAR* lowercaseW(WCHAR* s);
bool FileExistsW(const WCHAR* file);
void GetDLLPathW(HMODULE h,WCHAR* path, DWORD nSize);

void FixRotation(int rotation, SIZE& s, POINT& p);
void FixRotationF(int rotation, SizeF& s, PointF& p);
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

SIZE inline SIZEFROMRECT(const RECT& r)
{
	SIZE s = {r.right - r.left,r.bottom - r.top};
	return s;
}

void inline SIZEFROMRECT(SIZE& s, const RECT& r)
{
	s.cx = r.right - r.left;
	s.cy = r.bottom - r.top;
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

void ApplyQualityTo(UINT quality,Graphics* g);


std::wstring StringToWString(const std::string& str);
std::string WStringToString(const std::wstring& wstr);

inline int round_float_greater(float f)
{
	int i = (int)f;
	if(f - i > 0.0f)
		i += 1;

	return i;
}

template<typename R> int RECTWIDTH(R& r)
{
	return r.right - r.left;
}

template<typename R> int RECTHEIGHT(R& r)
{
	return r.bottom - r.top;
}

template<typename T, typename TScaleType> T RemapPoint(const T& t, const SIZE& s, TScaleType txScale=100, TScaleType tyScale=100)
{
	T nT;
	nT.x = nT.y = 0;

	float xScale = static_cast<float>(txScale);
	float yScale = static_cast<float>(tyScale);
	
	if(s.cx == 0 || s.cy == 0)
		return nT;

	if(xScale == 0.0f || yScale == 0.0f)
		return nT;

	float xRatio = float(s.cx) / xScale;
	float yRatio = float(s.cy) / yScale;

#pragma warning(push, 1)
	nT.x = static_cast<float>(t.x) * xRatio;
	nT.y = static_cast<float>(t.y) * yRatio;
#pragma warning(pop)
	return nT;
}


template<typename TSizeType, typename TScaleType> inline float RemapPointF(float t, TSizeType tsize, TScaleType toldScale)
{
	float f;
	f = 0.0f;

	float oldScale = static_cast<float>(toldScale);
	float size = static_cast<float>(tsize);

	if(0 == size)
		return f;

	if(0.0f == oldScale)
		return f;

	float ratio = size / oldScale;
	f = t * ratio;

	return f;
}

template<typename TScaleType> inline RectF RemapRectF(const RectF& r, const SIZE& s, TScaleType oldXScale, TScaleType oldYScale)
{
	RectF rf = r;

	rf.X = RemapPointF(rf.X, static_cast<float>(s.cx), oldXScale);
	rf.Y = RemapPointF(rf.Y, static_cast<float>(s.cy), oldYScale);
	rf.Width = RemapPointF(rf.Width, static_cast<float>(s.cx), oldXScale);
	rf.Height = RemapPointF(rf.Height, static_cast<float>(s.cy), oldYScale);

	return rf;
}

inline RectF RECTTORECTF(const RECT& r)
{
	return RectF(static_cast<float>(r.left), static_cast<float>(r.top), static_cast<float>(r.right - r.left), static_cast<float>(r.bottom - r.top));
}

template<typename TScaleType> inline RECT RemapRect(const RECT& r, const SIZE& s, TScaleType xScale=100, TScaleType yScale=100)
{
	POINT p1 = {r.left, r.top};
	POINT p2 = {r.right, r.bottom};

	p1 = RemapPoint(p1, s, xScale, yScale);
	p2 = RemapPoint(p2, s, xScale, yScale);

	RECT ret = {p1.x, p1.y, p2.x, p2.y};

	return ret;
}

//herd! some place to put com templates in
/*
    O: the owner of the COM decorator (class Docklet)
*/
template <class O>
class CoElementOwnerImpl
{
	public:
	bool StoreOwner(O *owner)
	{
		//weak reference
		//m_owner->AddRef();
		m_owner = owner;
		return true;
	}
	CoElementOwnerImpl() :
		m_owner(NULL)
	{
	}

	~CoElementOwnerImpl()
	{
		if( m_owner )
		{
			//weak reference
			//m_owner->Release();
			m_owner = NULL;
		}
	}
	O *m_owner;
};

class CoElementUnk
{
public:
	virtual bool GetWrapperDisp(IDispatch **unkRet) = 0;
	virtual bool GetWrapperUnk(IID iid, void **unkRet) = 0;
};

class safeCreatable
{
public:
	virtual HWND GetHWnd()
	{
		return NULL;
	}
	virtual DWORD GetThreadID()const
	{
		return NULL;
	}
};

class safeElem
{
public:
	HANDLE evt;
	IUnknown *pUnk;
	IStream *pStr;
	IID iid;
	CoElementUnk *pSender;

	safeElem(IID iidSrc, CoElementUnk *pThis)
	{
		pUnk = NULL;
		pStr = NULL;
		evt = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		iid = iidSrc;
		pSender = pThis;
	}
	~safeElem()
	{
		::CloseHandle(evt);
	}
	void Marshal()
	{
		IUnknown *pSrc = NULL;
		pSender->GetWrapperUnk(iid, (void**)&pSrc);
		::CoMarshalInterThreadInterfaceInStream(
			iid, pSrc, &pStr);
		::SetEvent(evt);
		pSrc->Release();
	}
	void UnMarshal(void **pDest)
	{
		while(true)
		{
			MSG msg;
			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			if( ::MsgWaitForMultipleObjects(
				1, &evt, TRUE, 1000, QS_ALLEVENTS) == WAIT_OBJECT_0 )
			{
				::CoGetInterfaceAndReleaseStream(
					pStr, iid, pDest);
				return;
			}
		}
	}
};

#ifndef	WM_AVEACTION 
#define WM_AVEACTION WM_USER + 3495
#endif
#ifndef AVEACTION_GETSAFEELEM
#define AVEACTION_GETSAFEELEM 512
#endif

template <class O, class T, class I>
class safeCreate :
	public CoElementUnk,
	public safeCreatable
{
public:
	virtual bool IsLocal()
	{
		DWORD tid = GetThreadID();
		return ( 0 == tid || GetCurrentThreadId() == tid );
	}

	virtual bool GetLocal(I **typedRet) = 0;
	
	virtual bool GetRemote(I **typedRet)
	{
		DWORD tid = GetThreadID();
		CComPtr<I> ptr;
		safeElem se(__uuidof(*typedRet), this);

		HWND wnd = GetHWnd();
		if( ::IsWindow(wnd) )
			//desklets and other enumerated objects
			::SendMessage(GetHWnd(), WM_AVEACTION, (WPARAM)&se, AVEACTION_GETSAFEELEM);
		else
			//_App, _Dbg etc. created on the main thread
			::PostThreadMessage(tid, WM_AVEACTION, (WPARAM)&se, AVEACTION_GETSAFEELEM);

		se.UnMarshal((void**)&ptr.p);
		
		if( typedRet )
			ptr.QueryInterface(typedRet);
		
		return (ptr.p)?true:false;
	}

	virtual bool GetSafeInstance(I **typedRet)
	{
		if( IsLocal() )
			return GetLocal(typedRet);
		else
			return GetRemote(typedRet);

	}
};

template <class O, class T, class I>
class safeCreatableImpl :
	public safeCreate<O, T, I>
{
public:
	bool GetLocal(I **typedRet)
	{
		CComObject<O>* pT = reinterpret_cast<CComObject<O>*>(this);
		return SUCCEEDED(pT->QueryInterface(typedRet));
	}
	bool GetWrapperDisp(IDispatch **typedRet)
	{
		CComObject<O>* pT = reinterpret_cast<CComObject<O>*>(this);
		return SUCCEEDED(pT->QueryInterface(typedRet));
	}
	bool GetWrapperUnk(IID iid, void **typedRet)
	{
		CComObject<O>* pT = (CComObject<O>*)this;
		return SUCCEEDED(pT->QueryInterface(iid, typedRet));
	}
};

/*
    O: the owner of the COM decorator (class Docklet)
    T: the COM implementation class (class CDesklet)
    I: the COM interface to expose (interface IDesklet)
*/
template <class O, class T, class I>
class CoElement : 
	public safeCreate<O, T, I>
{
public:
	bool GetLocal(I **typedRet)
	{
		if( NULL == m_wrp )
		{
			CComObject<T>::CreateInstance(&m_wrp);
			m_wrp->AddRef();

			CoElementOwnerImpl<O> *pT = (CoElementOwnerImpl<O> *)m_wrp;
			O* pO = (O*)this;
			pT->StoreOwner(pO);
		}
		if( typedRet )
			m_wrp->QueryInterface(typedRet);
		return (m_wrp)?true:false;
	}
	
	bool GetWrapper(I **typedRet)
	{
		return GetSafeInstance(typedRet);
	}

	bool GetWrapperDisp(IDispatch **unkRet)
	{
		I *typedRet = NULL;
		if( GetWrapper(&typedRet) && typedRet )
		{
			typedRet->QueryInterface(IID_IDispatch, (void**)unkRet);
			typedRet->Release();
			return true;
		}
		return false;
	}

	bool GetWrapperUnk(IID iid, void** unkRet)
	{
		I *typedRet = NULL;
		if( GetWrapper(&typedRet) && typedRet )
		{
			typedRet->QueryInterface(iid, unkRet);
			typedRet->Release();
			return true;
		}
		return false;
	}

	CoElement() : 
		m_wrp(NULL)
	{}

	~CoElement()
	{
		DropElem();
	}
	void DropElem()
	{
		if( m_wrp )
		{
			m_wrp->Release();
			m_wrp = NULL;
		}
	}

	CComObject<T> *m_wrp;
};
/*
template <class T>
class CoVectorCollection
{
};

template <class T>
class CoMapCollection : 
  public CComObjectRoot,
  public IEnumVARIANT
{
public:
  BEGIN_COM_MAP(CoMapCollection<T>)
	  COM_INTERFACE_ENTRY(IEnumVARIANT)
  END_COM_MAP()
  
  
  // IEnumVariant
  HRESULT STDMETHODCALLTYPE Next( 
    ULONG celt,
    VARIANT *rgVar,
    ULONG *pCeltFetched)
  {
    if( celt > size - act )
      return S_FALSE;

    ULONG actact = act;
    for( ULONG i = actact; i < actact + celt; i++ )
    {
      if( pCeltFetched )
        *pCeltFetched = 1 + i - actact;

      VariantCopy(&rgVar[i - actact], &var[i]);
      act++;
    }

    return S_OK;
  }

  HRESULT STDMETHODCALLTYPE Skip( 
    ULONG celt)
  {
    return S_OK;
  }

  HRESULT STDMETHODCALLTYPE Reset()
  {
    act = 0;
    return S_OK;
  }

  virtual HRESULT STDMETHODCALLTYPE Clone( 
    IEnumVARIANT **ppEnum)
  {
    return E_OUTOFMEMORY;
  }

  // implementation
  CoMapCollection()
  {
    m_map = NULL;
    act = 0;
  }

  ~CoMapCollection()
  {
    for( ULONG i = 0; i < size; i++ )
    {
      VariantClear(&var[i]);
    }
    delete var;
  }

  void Attach(T* newMap)
  {
    m_map = newMap;
    size = m_map->size();
    var = new VARIANT[size];
    
    it = m_map->begin();
    for( ULONG i = 0; i < size; i++)
    {
      VariantInit(&var[i]);
      var[i].vt = VT_DISPATCH;
      CoElementUnk *ptr = it->second;
      ptr->GetWrapperDisp(&var[i].pdispVal);
      it++;
    }
    
  }
  T *m_map;
  T::iterator it;
  ULONG size, act;
  VARIANT *var;
};
*/

typedef CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
                              _Copy<VARIANT> > CComVarArrEnum;
typedef CComObject<CComVarArrEnum> CComEnumObject;

template<class T>
HRESULT __stdcall DoMapNewEnum(T & m_map, LPUNKNOWN * pVal)
{
  T::iterator m_it;    
  long lCount = m_map.size();
  // a STL map is not suitable for CComEnum,
  // so we have to convert it into an array.
  VARIANT* var = new VARIANT[lCount];

  int i = 0;

  // iterate the STL map
  for( m_it = m_map.begin(); m_it != m_map.end(); m_it++, i++ )
  {
    VariantInit( &var[i] );
    CoElementUnk *ptr = m_it->second;
    IDispatch *pCon = NULL;
    ptr->GetWrapperDisp(&pCon);
    var[i].vt = VT_DISPATCH;

    // we don't have to force an implicit addref
    // VariantClear will discard all references
    pCon->QueryInterface( IID_IDispatch, 
      (void**) &var[i].pdispVal );
    pCon->Release();
  }

  // CComEnumObject is typedef'd above
  CComEnumObject* pVar = new CComEnumObject;
  // Create a single instance enumerator object
  pVar->Init( &var[0], &var[i], NULL, AtlFlagCopy );

  lCount--;

  // all is in CComEnumObject now, so clear the VARIANT-array
  while( lCount >= 0 )
  {
     VariantClear( &var[lCount] );
     lCount--;
  }
  delete [] var;

  HRESULT hr = pVar->QueryInterface( IID_IUnknown, 
                 (void**) pVal );
  return hr;
}

template<class T>
HRESULT __stdcall DoVectorNewEnum(T & m_vector, LPUNKNOWN * pVal)
{
  T::iterator m_it;    
  long lCount = m_vector.size();
  // a STL vector is not suitable for CComEnum,
  // so we have to convert it into an array.
  VARIANT* var = new VARIANT[lCount];

  int i = 0;

  // iterate the STL vector
  for( m_it = m_vector.begin(); m_it != m_vector.end(); m_it++, i++ )
  {
    VariantInit( &var[i] );
    CoElementUnk *ptr = (*m_it);
    IDispatch *pCon = NULL;
    ptr->GetWrapperDisp(&pCon);
    var[i].vt = VT_DISPATCH;

    // we don't have to force an implicit addref
    // VariantClear will discard all references
    pCon->QueryInterface( IID_IDispatch, 
      (void**) &var[i].pdispVal );
    pCon->Release();
  }

  // CComEnumObject is typedef'd above
  CComEnumObject* pVar = new CComEnumObject;
  // Create a single instance enumerator object
  pVar->Init( &var[0], &var[i], NULL, AtlFlagCopy );

  lCount--;

  // all is in CComEnumObject now, so clear the VARIANT-array
  while( lCount >= 0 )
  {
     VariantClear( &var[lCount] );
     lCount--;
  }
  delete [] var;

  HRESULT hr = pVar->QueryInterface( IID_IUnknown, 
                 (void**) pVal );
  return hr;
}

struct CComBSTRLessIgnCase : 
  std::binary_function<CComBSTR, CComBSTR, bool> 
{
  bool operator()(const CComBSTR& _X, const CComBSTR& _Y) const
	{
    return 0 > _wcsicmp(_X, _Y);
  }
};

template<class T>
HRESULT __stdcall DoMapInplaceEnum(T & m_map, LPUNKNOWN * pVal)
{
  T::iterator m_it;    
  long lCount = m_map.size();
  // a STL map is not suitable for CComEnum,
  // so we have to convert it into an array.
  VARIANT* var = new VARIANT[lCount];

  int i = 0;

  // iterate the STL map
  for( m_it = m_map.begin(); m_it != m_map.end(); m_it++, i++ )
  {
    VariantInit( &var[i] );
    IDispatch *ptr = m_it->second;
    var[i].vt = VT_DISPATCH;
    // we have to force an implicit addref
    // vb will discard all references
    ptr->QueryInterface(IID_IDispatch, (void**)&var[i].pdispVal);

  }

  // CComEnumObject is typedef'd above
  CComEnumObject* pVar = new CComEnumObject;
  // Create a single instance enumerator object
  pVar->Init( &var[0], &var[i], NULL, AtlFlagCopy );

  lCount--;

  // all is in CComEnumObject now, so clear the VARIANT-array
  while( lCount >= 0 )
  {
     VariantClear( &var[lCount] );
     lCount--;
  }
  delete [] var;

  HRESULT hr = pVar->QueryInterface( IID_IUnknown, 
                 (void**) pVal );
  return hr;
}


#endif//HELPER_FUNCS_H_303001_