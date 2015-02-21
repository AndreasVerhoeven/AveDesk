// DeskletInfo.cpp : Implementierung von CDeskletInfo
#include "stdafx.h"
#include "AveDesk.h"
#include "DeskletInfo.h"
#include "dockletdll.h"

/////////////////////////////////////////////////////////////////////////////
// CDeskletInfo

CDeskletInfo::~CDeskletInfo()
{
	if( m_Skins )
	{
		m_Skins->Release();
		m_Skins = NULL;
	}
}

STDMETHODIMP CDeskletInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDeskletInfo
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
    if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

#define STRIP_PROP(n) \
STDMETHODIMP CDeskletInfo::get_##n(BSTR *pVal) \
{ \
  USES_CONVERSION; \
  CComBSTR((m_owner->Get##n())).CopyTo(pVal); \
	return S_OK; \
}

STRIP_PROP(Name)
STRIP_PROP(Author);
STRIP_PROP(Comments);
STRIP_PROP(FileName);

STDMETHODIMP CDeskletInfo::get_Version(int *pVal)
{
  *pVal = m_owner->GetVersion();
	return S_OK;
}

STDMETHODIMP CDeskletInfo::get_Skins(IDeskletSkins **pVal)
{
	// TODO: Add your implementation code here
	if(NULL == m_Skins)
	{
		CComObject<CDeskletSkins>::CreateInstance(&m_Skins);
		m_Skins->AddRef();
		DeskletSkinEnumerator* dse = DeskletSkinEnumerator::Create(m_owner);
		m_Skins->SetData(dse);
	}
		
	return m_Skins->QueryInterface(pVal);
}
