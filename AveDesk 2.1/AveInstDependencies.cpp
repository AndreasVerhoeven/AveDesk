// AveInstDependencies.cpp: implementation of the CAveInstDependencies class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstDependencies.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstDependencies::CAveInstDependencies()
{

}

CAveInstDependencies::~CAveInstDependencies()
{

}

HRESULT CAveInstDependencies::LoadFromNode(XMLNode rootNode)
{
	HRESULT hRes = S_OK;

	if(NULL == rootNode.p)
		return (hRes = E_FAIL);

	CComPtr<IXMLDOMNodeList> nodeList;
	if(FAILED(hRes = rootNode->get_childNodes(&nodeList)) || NULL == nodeList.p)
		return hRes;

	long lCount(0);
	if( FAILED( hRes = nodeList->get_length(&lCount) ) )
		return hRes;

	for(long lIndex = 0; lIndex <= lCount; ++lIndex)
	{
		XMLNode newNode;
		if(SUCCEEDED( hRes = nodeList->get_item(lIndex,&newNode)) && newNode.p != NULL )
		{
			CComBSTR bstrText;
			if(SUCCEEDED(newNode->get_text(&bstrText)) && static_cast<WCHAR*>(bstrText) != NULL)
				m_List.push_back(static_cast<WCHAR*>(bstrText));
		}
	}

	return S_OK;
}