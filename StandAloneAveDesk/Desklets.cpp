// Desklets.cpp : Implementation of CDesklets
#include "stdafx.h"
#include "AveDesk.h"
#include "Desklets.h"
#include "deskletinfo.h"
#include "docklet.h"

/////////////////////////////////////////////////////////////////////////////
// CDesklets



STDMETHODIMP CDesklets::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDesklets
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CDesklets::get_Count(long *pVal)
{
	*pVal = Docklet::Size();

	return S_OK;
}

STDMETHODIMP CDesklets::get_Item(VARIANT *Index, IDesklet **pVal)
{
	CComVariant idx = *Index;
	*pVal = NULL;
	if( SUCCEEDED( idx.ChangeType(VT_I4) ) )
	{
		Docklet::DockletsMap::iterator it = Docklet::Docklets.begin();
		Docklet *ptr = NULL;

		// fetch as window
		if( ::IsWindow((HWND)idx.lVal) )
		{
			it = Docklet::Docklets.find((HWND)idx.lVal);
		}
		// fetch as index
		else if( idx.lVal < 1 || idx.lVal > (int)Docklet::Size() )
		{
			return RPC_S_INVALID_BOUND;
		}
		else
		{
			int iWhere = idx.lVal - 1;
			while( iWhere-- )
				it++;
		}

		if( Docklet::Docklets.end() != it )
		{
			ptr = (*it).second;
			ptr->GetWrapper(pVal);
		}
		// Caller must be aware: If Not App.RunningDesklets(idx) Is Nothing Then
	}
	return S_OK;
}

STDMETHODIMP CDesklets::get__NewEnum(IUnknown **pVal)
{
	return DoMapNewEnum<Docklet::DockletsMap>(Docklet::Docklets, pVal);
}

STDMETHODIMP CDesklets::Add(IDeskletInfo *What, VARIANT *Left, VARIANT *Top, VARIANT *Width, VARIANT *Height, IDesklet **pVal)
{
	CComObject<CDeskletInfo> *ptr = dynamic_cast<CComObject<CDeskletInfo> *>(What);
	if( !ptr )
		return E_INVALIDARG;

	Docklet* newDocklet = Docklet::LoadDocklet(ptr->m_owner->GetFileName(),NULL,NULL,NULL,false);
	newDocklet->WaitForDockletBeingInitialized(INFINITE);
	if( !newDocklet   )
		return E_FAIL;
	newDocklet->GetWrapper(pVal);

	CComVariant lft = (*Left);
	if( lft.vt != VT_EMPTY && SUCCEEDED( lft.ChangeType(VT_I4) ) )
		(*pVal)->Move(lft.lVal, Top, Width, Height);
	return S_OK;
}

STDMETHODIMP CDesklets::AddWithSkin(IDeskletInfo *What, IDeskletSkin *Skin, VARIANT *Left, VARIANT *Top, VARIANT *Width, VARIANT *Height, IDesklet **pVal)
{
	CComObject<CDeskletInfo> *ptr = dynamic_cast<CComObject<CDeskletInfo> *>(What);
	if( !ptr )
		return E_INVALIDARG;

	CComObject<CDeskletSkin> *ski = dynamic_cast<CComObject<CDeskletSkin> *>(Skin);
	if( !ski )
		return E_INVALIDARG;

	Docklet* newDocklet = Docklet::LoadDocklet(
							ptr->m_owner->GetFileName(),
							NULL,
							ski->GetEnumerator(),
							ski->GetSkinInfo()->GetSkinInfoWrapper(),false);

	newDocklet->WaitForDockletBeingInitialized(INFINITE);
	
	if( !newDocklet   )
		return E_FAIL;
	newDocklet->GetWrapper(pVal);

	CComVariant lft = (*Left);
	if( lft.vt != VT_EMPTY && SUCCEEDED( lft.ChangeType(VT_I4) ) )
		(*pVal)->Move(lft.lVal, Top, Width, Height);
	return S_OK;
}
