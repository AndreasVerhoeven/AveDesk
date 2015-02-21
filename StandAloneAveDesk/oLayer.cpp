// oLayer.cpp : Implementierung von CoLayer
#include "stdafx.h"
#include "AveDesk.h"
#include "layer.h"
#include "oLayer.h"


/////////////////////////////////////////////////////////////////////////////
// CoLayer

#define STRIP_PROPR(n) \
STDMETHODIMP CoLayer::get_##n(BSTR *pVal) \
{ \
  USES_CONVERSION; \
  CComBSTR(A2W(m_owner->Get##n())).CopyTo(pVal); \
	return S_OK; \
}
/*
#define STRIP_PROPW(n) \
STDMETHODIMP CoLayer::put_##n(BSTR newVal) \
{ \
  USES_CONVERSION; \
  m_owner->Set##n(W2A(newVal)); \
	return S_OK; \
}

#define STRIP_PROPRW(n) \
  STRIP_PROPR(n) \
  STRIP_PROPW(n)
*/
#define BOOL_PROPR(a,b) \
STDMETHODIMP CoLayer::get_##a(VARIANT_BOOL *pVal) \
{ \
  if( m_owner->Get##b() ) \
    *pVal = VARIANT_TRUE; \
  else \
    *pVal = VARIANT_FALSE; \
	return S_OK; \
} 

#define BOOL_PROPW(a,b) \
STDMETHODIMP CoLayer::put_##a(VARIANT_BOOL newVal) \
{ \
  m_owner->Set##b((newVal)?true:false); \
	return S_OK; \
} 

#define BOOL_PROPRW(a,b) \
  BOOL_PROPR(a,b) \
  BOOL_PROPW(a,b)



// Add more string forwards here
STRIP_PROPR(Name)
STRIP_PROPR(FileName)

// Add more bool forwards here
BOOL_PROPRW(Visible,Visibility)
BOOL_PROPRW(CanBeIcon,CanBeIcon)

