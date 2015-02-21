// AveInstCopyItem.cpp: implementation of the CAveInstCopyItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstCopyItem.h"
#include "AveInstallerEnvironment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstCopyItem::CAveInstCopyItem()
{

}

CAveInstCopyItem::~CAveInstCopyItem()
{

}

bool CAveInstCopyItem::CopyIntern(CAveInstallerEnvironment& env, const std::wstring& source, const std::wstring dest)
{
	env.SetFileAlias(m_Alias,dest);

	return true;
}

HRESULT CAveInstCopyItem::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	CComPtr<IXMLDOMNamedNodeMap> nodeMap;
	if(FAILED(hRes = node->get_attributes(&nodeMap)))
		return hRes;

	if(NULL == nodeMap.p)
		return (hRes = E_FAIL);


	CComBSTR bstrName( L"Alias");
	XMLNode namedNode;
	if(FAILED(hRes = nodeMap->getNamedItem(bstrName,&namedNode)))
		return hRes;

	if(NULL == namedNode.p)
		return (hRes = E_FAIL);

	CComBSTR bstrAlias;
	if(FAILED( hRes = namedNode->get_text(&bstrAlias)))
		return hRes;

	if(bstrAlias != NULL)
		m_Alias = static_cast<WCHAR*>(bstrAlias);

	return S_OK;
}