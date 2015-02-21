// AveInstActions.cpp: implementation of the CAveInstActions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstActions.h"

#include "AveInstAction.h"
#include "AveInstActionsFactory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstActions::CAveInstActions()
{

}

CAveInstActions::~CAveInstActions()
{

}

HRESULT CAveInstActions::LoadFromNode(XMLNode& rootNode)
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

	for(long lIndex = 0; lIndex < lCount; ++lIndex)
	{
		XMLNode newNode;
		if(SUCCEEDED( hRes = nodeList->get_item(lIndex,&newNode) ) )
		{
			boost::shared_ptr<CAveInstAction> actItem;
			hRes = CAveInstActionsFactory::CreateActionItem(newNode, actItem);

			if(SUCCEEDED(hRes))
				m_Items.push_back(actItem);
		}
	}

	return S_OK;
}

HRESULT CAveInstActions::DoActions(CAveInstallerEnvironment& env)
{
	HRESULT hRes = S_OK;

	for(std::list<boost::shared_ptr<CAveInstAction> >::iterator iter = m_Items.begin(); iter != m_Items.end(); ++iter)
	{
		boost::shared_ptr<CAveInstAction>& instAction = *iter;
		hRes = instAction->DoAction(env);
		// TODO: what to do on error?
	}

	return hRes;
}