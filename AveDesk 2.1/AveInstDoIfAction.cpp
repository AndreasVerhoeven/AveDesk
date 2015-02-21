// AveInstDoIfAction.cpp: implementation of the CAveInstDoIfAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstDoIfAction.h"
#include "AveInstRequirementsFactory.h"
#include "AveInstActionsFactory.h"
#include "AveInstError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstDoIfAction::CAveInstDoIfAction()
{

}

CAveInstDoIfAction::~CAveInstDoIfAction()
{

}

HRESULT CAveInstDoIfAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);


	CComBSTR ifName(L"If");
	XMLNode ifNode;
	if(FAILED(hRes = node->selectSingleNode(ifName,&ifNode)))
		return hRes;

	if(FAILED(hRes = CAveInstRequirementsFactory::CreateRequirementItem(ifNode, m_Requirement)))
		return hRes;

	CComBSTR thenName(L"Then");
	XMLNode thenNode;
	if(FAILED(hRes = node->selectSingleNode(thenName,&thenNode)))
		return hRes;

	if(FAILED(hRes = CAveInstActionsFactory::CreateActionItem(thenNode,m_If)))
		return hRes;

	// An Else-node is not mandatory!
	CComBSTR elseName(L"Else");
	XMLNode elseNode;
	if(SUCCEEDED(hRes = node->selectSingleNode(elseName,&elseNode)))
	{
		hRes = CAveInstActionsFactory::CreateActionItem(elseNode,m_Else);
	}


	return (hRes = S_OK);
}

HRESULT CAveInstDoIfAction::DoAction(CAveInstallerEnvironment& env)
{
	HRESULT hRes = S_OK;

	if(NULL == m_Requirement.get())
		return (hRes = E_FAIL);

	CAveInstError dummyError;
	bool res = m_Requirement->DoesMeetRequirement(env,dummyError);
	if(res)
	{
		if(NULL == m_If.get())
			return (hRes = E_FAIL);

		return m_If->DoAction(env);
	}
	else
	{
		// An Else-node is not mandatory, so if no Else was present, just
		// do nothing

		if(m_Else.get())
			return m_Else->DoAction(env);
	}


	return (hRes = S_OK);
}