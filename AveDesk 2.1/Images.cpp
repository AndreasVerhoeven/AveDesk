// Images.cpp : Implementierung von CImages
#include "stdafx.h"
#include "AveDesk.h"
#include "Images.h"
#include "activscp.h"

/////////////////////////////////////////////////////////////////////////////
// CImages

CImages::~CImages()
{
  ImagesMap::iterator it = imgs.begin();
  for( ; it != imgs.end(); it++)
  {
    (*it).second->Release();
  }
}

STDMETHODIMP CImages::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAveImages
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
    if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CImages::get_Count(long *pVal)
{
	*pVal = imgs.size();

	return S_OK;
}

STDMETHODIMP CImages::get_Item(VARIANT *Index, IAveImage **pVal)
{
  CComVariant var = *Index;
  ImagesMap::iterator it = imgs.begin();
  if( var.vt == VT_I4 || SUCCEEDED(var.ChangeType(VT_I4) ) )
  {
    if( var.lVal < 0 || var.lVal > (int)imgs.size() )
      return E_INVALIDARG;
    
    for( int i = 1; it != imgs.end(); i++ )
      it++;

    (*it).second->QueryInterface(pVal);
	  return S_OK;
  }
  if( var.vt == VT_BSTR || SUCCEEDED(var.ChangeType(VT_BSTR)) )
  {
    it = imgs.find(var.bstrVal);
    if( it != imgs.end() )
    {
      (*it).second->QueryInterface(pVal);
	    return S_OK;
    }
  }
  return E_INVALIDARG;
}

void CImages::LoadXML(IXMLDOMNode *nod)
{
  long count = 0;
  CComPtr<IXMLDOMNodeList> list;
  nod->selectNodes(L"image", &list.p);
  if( list.p )
  {
    list->get_length(&count);  
    for( long i = 0; i < count; i++ )
    {
      CComPtr<IXMLDOMNode> nodeChild;
      CComObject<CImage> *pr = NULL;

      list->get_item(i, &nodeChild.p);
      CComObject<CImage>::CreateInstance(&pr);
      pr->AddRef();
      pr->parent = parent;
      pr->LoadXML(nodeChild);
      pr->parent = NULL;
      // unnamed images aren't added
      if( pr->name.Length() )
        imgs[pr->name] = pr;
      else
        pr->Release();
    }
  }
}

BOOL CImages::IsKnownImage(
  LPCOLESTR pstrName, 
  DWORD dwReturnMask, 
  IUnknown * * ppiunkItem, 
  ITypeInfo * * ppti)

{
  ImagesMap::iterator it = imgs.find(pstrName);
  if( it != imgs.end() )
  {
    CComObject<CImage> *ptr = (*it).second;
    if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
    {
      ptr->GetClassInfo(ppti);
      (*ppti)->AddRef();	 // because returning
    }
    if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
    {
      return SUCCEEDED(ptr->QueryInterface(IID_IUnknown, (void**) ppiunkItem));
    }
  }
  return FALSE;
}

STDMETHODIMP CImages::get__NewEnum(IUnknown **pVal)
{
  return DoMapInplaceEnum<ImagesMap>(imgs, pVal);
}
