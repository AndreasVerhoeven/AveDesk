// SkinEnumerator.cpp : Implementation of CAveDeskApp and DLL registration.

#include "stdafx.h"
#include "AveDesk.h"
#include "SkinEnumerator.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CSkinEnumerator::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ISkinEnumerator,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CSkinEnumerator::Next(BSTR *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
