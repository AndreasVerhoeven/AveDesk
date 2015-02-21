// DeskletInfos.cpp : Implementierung von CDeskletInfos
#include "stdafx.h"
#include "AveDesk.h"
#include "DeskletInfos.h"
#include "DeskletInfo.h"
#include "dockletdll.h"

#include "Application.h"
extern CComObject<CApplication>* _App;


/////////////////////////////////////////////////////////////////////////////
// CDeskletInfos

STDMETHODIMP CDeskletInfos::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDeskletInfos
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
    if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CDeskletInfos::get__NewEnum(IUnknown **pVal)
{
  return DoMapNewEnum<DockletDLL::DockletsDLLMap>(DockletDLL::DockletDLLs, pVal);
}

STDMETHODIMP CDeskletInfos::get_Count(long *pVal)
{
  *pVal = DockletDLL::size();
	return S_OK;
}

STDMETHODIMP CDeskletInfos::get_Item(VARIANT *Index, IDeskletInfo **pVal)
{
  USES_CONVERSION;
	CComVariant idx = *Index;
  // index is decimal in the range of 1 to Count
  if( idx.vt == VT_I4 || SUCCEEDED( idx.ChangeType(VT_I4) ) )
  {
    if( idx.lVal < 1 || idx.lVal > (int)DockletDLL::size() )
      return RPC_S_INVALID_BOUND;

    DockletDLL *ptr = NULL;
    DockletDLL::DockletsDLLMap::iterator it = DockletDLL::begin();
    int iWhere = idx.lVal - 1;
    while( iWhere-- )
      it++;

    ptr = (*it).second;

    ptr->GetWrapper(pVal);
    return S_OK;
  }
  // index is by name of dll
  if( idx.vt == VT_BSTR || SUCCEEDED( idx.ChangeType(VT_BSTR) ) )
  {
    WCHAR *strFind = (idx.bstrVal);
    //strlwr(strFind);
	lowercaseW(strFind);
    
    DockletDLL *ptr = NULL;
    DockletDLL::DockletsDLLMap::iterator it = 
      DockletDLL::find(strFind);
    if( it != DockletDLL::end() )
    {
      ptr = (*it).second;

      ptr->GetWrapper(pVal);
      return S_OK;
    }
  }

	return E_INVALIDARG;
}

STDMETHODIMP CDeskletInfos::Refresh()
{
	WCHAR appDir[MAX_PATH] = {0};
	wcscpy(appDir, _App->GetAveDeskDataDir().c_str());
	std::wstring dir = lowercaseW(appDir);

	DockletDLL::ScanDirectoryForDesklets(dir+L"Docklets\\",true);
	DockletDLL::ScanDirectoryForDesklets(dir+L"Desklets\\",true);
	return S_OK;
}

STDMETHODIMP CDeskletInfos::Clear()
{
	return S_OK;
}
