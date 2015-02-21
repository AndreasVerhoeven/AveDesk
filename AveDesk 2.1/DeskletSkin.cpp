// DeskletSkin.cpp : Implementation of CAveDeskApp and DLL registration.

#include "stdafx.h"
#include "AveDesk.h"
#include "DeskletSkin.h"
#include "DeskletSkinEnumerator.h"
#include "DeskletSkinEnumerator.h"
#include "DeskletSkinInfo.h"
/////////////////////////////////////////////////////////////////////////////
//

CDeskletSkin::CDeskletSkin()
{  
	m_Enumerator = NULL;
}

CDeskletSkin::~CDeskletSkin()
{
	if(m_Enumerator)
		m_Enumerator->Release();
}

STDMETHODIMP CDeskletSkin::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDeskletSkin,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

DeskletSkinEnumerator* CDeskletSkin::GetEnumerator()
{
	return m_Enumerator;
}

DeskletSkinInfo* CDeskletSkin::GetSkinInfo()
{
	return m_SkinInfo;
}

void CDeskletSkin::SetData(DeskletSkinEnumerator* enumerator, DeskletSkinInfo* info)
{
	if(m_Enumerator)
		m_Enumerator->Release();

	m_Enumerator = enumerator;
	if(m_Enumerator)
		m_Enumerator->AddRef();

	m_SkinInfo = info;
}

STDMETHODIMP CDeskletSkin::get_Key(BSTR *pVal)
{
	if( NULL == pVal)
		return E_INVALIDARG;

	if( NULL == m_SkinInfo )
		return E_UNEXPECTED;

	*pVal = SysAllocString(m_SkinInfo->GetKey().c_str());

	return S_OK;
}

STDMETHODIMP CDeskletSkin::get_Number(long *pVal)
{
	if( NULL == pVal)
		return E_INVALIDARG;

	if( NULL == m_SkinInfo )
		return E_UNEXPECTED;

	*pVal = static_cast<long>(m_SkinInfo->GetNum());

	return S_OK;
}

STDMETHODIMP CDeskletSkin::get_Name(BSTR *pVal)
{
	if( NULL == pVal)
		return E_INVALIDARG;

	if( NULL == m_SkinInfo )
		return E_UNEXPECTED;

	*pVal = SysAllocString(m_SkinInfo->GetName().c_str());

	return S_OK;
}

STDMETHODIMP CDeskletSkin::get_Author(BSTR *pVal)
{
	if( NULL == pVal)
		return E_INVALIDARG;

	if( NULL == m_SkinInfo )
		return E_UNEXPECTED;

	*pVal = SysAllocString(m_SkinInfo->GetAuthor().c_str());

	return S_OK;
}

STDMETHODIMP CDeskletSkin::get_Version(long *pVal)
{
	if( NULL == pVal)
		return E_INVALIDARG;

	if( NULL == m_SkinInfo )
		return E_UNEXPECTED;

	*pVal = static_cast<long>(m_SkinInfo->GetVersion());

	return S_OK;
}

STDMETHODIMP CDeskletSkin::get_Comments(BSTR *pVal)
{
	if( NULL == pVal)
		return E_INVALIDARG;

	if( NULL == m_SkinInfo )
		return E_UNEXPECTED;

	*pVal = SysAllocString(m_SkinInfo->GetNotes().c_str());

	return S_OK;
}
