// oLayers.cpp : Implementierung von CoLayers
#include "stdafx.h"
#include "AveDesk.h"
#include "oLayers.h"
#include "docklet.h"

/////////////////////////////////////////////////////////////////////////////
// CoLayers


STDMETHODIMP CoLayers::get__NewEnum(IUnknown **pVal)
{
  return DoVectorNewEnum<std::vector<Layer*> >(docklet->GetDocklet()->GetLayers(), pVal);
}

STDMETHODIMP CoLayers::get_Count(int *pVal)
{
	*pVal = docklet->GetDocklet()->GetLayers().size();

	return S_OK;
}

STDMETHODIMP CoLayers::get_Item(VARIANT *Index, ILayer **pVal)
{
	USES_CONVERSION;
  
  if( !Index )
    return E_INVALIDARG;
  
  CComVariant idx = *Index;
  // index is decimal in the range of 1 to Count
  if( VT_I4 == idx.vt || SUCCEEDED( idx.ChangeType(VT_I4) ) )
  {
    if( idx.lVal < 1 || idx.lVal > (int)docklet->GetDocklet()->GetLayers().size() )
      return RPC_S_INVALID_BOUND;

    Layer *ptr = docklet->GetDocklet()->GetLayers()[idx.lVal - 1];

    ptr->GetWrapper(pVal);
    return S_OK;
  }

  // index is by name of layer
  if( idx.vt == VT_BSTR || SUCCEEDED( idx.ChangeType(VT_BSTR) ) )
  {
    char *strFind = W2A(idx.bstrVal);
    std::vector<Layer*>::iterator
      it = docklet->GetDocklet()->GetLayers().begin(),
      et = docklet->GetDocklet()->GetLayers().end();

    for( ; it != et; it++ )
    {
      if( !_stricmp(strFind, (*it)->GetName()) )
      {
        (*it)->GetWrapper(pVal);
        return S_OK;
      }
    }
  }

	return E_INVALIDARG;
}
