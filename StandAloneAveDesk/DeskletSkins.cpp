// DeskletSkins.cpp : Implementation of CAveDeskApp and DLL registration.

#include "stdafx.h"
#include "AveDesk.h"
#include "DeskletSkins.h"
#include "DeskletSkin.h"
#include "DeskletSkinInfo.h"
#include "helper_funcs.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CDeskletSkins::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDeskletSkins,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

CDeskletSkins::~CDeskletSkins()
{
	if(m_Enumerator != NULL)
		m_Enumerator->Release();
	std::vector<DeskletSkinInfo*>::iterator iter = m_Infos.begin(); 
	for(; iter != m_Infos.end(); ++iter)
	{
		(*iter)->Clear();
		delete (*iter);
	}
}

STDMETHODIMP CDeskletSkins::get_Count(/*[out, retval]*/ long *pVal)
{
	if(NULL == pVal)
		return E_INVALIDARG;

	*pVal = m_Infos.size();

	return S_OK;
}

STDMETHODIMP CDeskletSkins::get_Item(VARIANT *Index, IDeskletSkin **pVal)
{
  CComVariant var = *Index;
  if( var.vt == VT_I4 || SUCCEEDED(var.ChangeType(VT_I4) ) )
  {
    if( var.lVal < 0 || var.lVal > static_cast<long>(m_Infos.size()) )
      return E_INVALIDARG;

	  CComObject<CDeskletSkin> *skin;
      CComObject<CDeskletSkin>::CreateInstance(&skin);
	  skin->SetData(m_Enumerator,m_Infos[var.lVal]);
	  skin->QueryInterface(pVal);

	  return S_OK;
  }
  if( var.vt == VT_BSTR || SUCCEEDED(var.ChangeType(VT_BSTR)) )
  {
	  std::vector<DeskletSkinInfo*>::iterator iter = m_Infos.begin(); 
	  for(; iter != m_Infos.end(); ++iter)
	  {
		    const WCHAR* wStr = static_cast<const WCHAR*>(var.bstrVal);
			if(_wcsicmp((*iter)->GetName().c_str(), wStr) == 0)
			{
				CComObject<CDeskletSkin> *skin;
				CComObject<CDeskletSkin>::CreateInstance(&skin);
				skin->SetData(m_Enumerator,*iter);
				skin->QueryInterface(pVal);
				return S_OK;
			}
	  }
  }
  return E_INVALIDARG;
}
/* -- HERD: Can't tell why no worky
typedef CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
                  _Copy<VARIANT>, std::vector<CComVariant> > VarVarEnum;

 template <class EnumType, class CollType>
    HRESULT CreateSTLEnumerator(IUnknown** ppUnk, IUnknown* pUnkForRelease, CollType& collection)
    {
        if (ppUnk == NULL)
            return E_POINTER;
        *ppUnk = NULL;

        CComObject<EnumType>* pEnum = NULL;
        HRESULT hr = CComObject<EnumType>::CreateInstance(&pEnum);

        if (FAILED(hr))
            return hr;

        hr = pEnum->Init(pUnkForRelease, collection);

        if (SUCCEEDED(hr))
            hr = pEnum->QueryInterface(ppUnk);

        if (FAILED(hr))
            delete pEnum;

        return hr;

    } // CreateSTLEnumerator

*/

STDMETHODIMP CDeskletSkins::get__NewEnum(IUnknown **pVal)
{
	/*
	std::vector<CComVariant> vec;

	std::vector<DeskletSkinInfo>::iterator iter = m_Infos.begin();
	for(; iter != m_Infos.end(); ++iter)
	{
		CComObject<CDeskletSkin>* skin;
		CComObject<CDeskletSkin>::CreateInstance(&skin);
		skin->SetData(m_Enumerator,*iter);


		CComVariant var;
		var.vt = VT_DISPATCH;
		skin->QueryInterface(IID_IDispatch,(void**)&var.pdispVal);
		vec.push_back(var);
	}

	return CreateSTLEnumerator<VarVarEnum>(pVal,NULL, vec);
	*/
	return DoVectorNewEnum<std::vector<DeskletSkinInfo*> >(m_Infos, pVal);
}

void CDeskletSkins::SetData(DeskletSkinEnumerator* enumerator)
{
	if(m_Enumerator != NULL)
		m_Enumerator->Release();

	m_Enumerator = enumerator;
	if(m_Enumerator != NULL)
	{
		m_Enumerator->AddRef();

		m_Enumerator->Reset();
		DeskletSkinInfo *info = new DeskletSkinInfo;
		while(m_Enumerator->Next(info))
		{			
			m_Infos.push_back(info);
			info = new DeskletSkinInfo;
		}
		// last one is always unused
		delete info;
	}
}