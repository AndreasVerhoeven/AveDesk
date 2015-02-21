// AveInstReqItem.cpp: implementation of the CAveInstReqItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstReqItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstReqItem::CAveInstReqItem()
{

}

CAveInstReqItem::~CAveInstReqItem()
{

}

HRESULT CAveInstReqItem::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;
	XMLNode errorNode;

	CComBSTR bstrError(L"OnError");
	if(FAILED(hRes = node->selectSingleNode(bstrError,&errorNode)))
		return hRes;


	return (hRes = m_Error.LoadFromNode(errorNode));
}