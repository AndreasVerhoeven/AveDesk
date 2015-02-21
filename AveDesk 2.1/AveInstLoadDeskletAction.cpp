// AveInstLoadDeskletAction.cpp: implementation of the CAveInstLoadDeskletAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstLoadDeskletAction.h"
#include "AveInstallerEnvironment.h"
#include "Docklet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstLoadDeskletAction::CAveInstLoadDeskletAction()
{

}

CAveInstLoadDeskletAction::~CAveInstLoadDeskletAction()
{

}

HRESULT CAveInstLoadDeskletAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if( NULL == node.p)
		return (hRes = E_FAIL);

	m_Node = node;

	return S_OK;
}

HRESULT CAveInstLoadDeskletAction::DoAction(CAveInstallerEnvironment& env)
{
	// loading a desklet is not silent!
	if(!env.IsSilentInstall())
		return Docklet::LoadFromXMLNode(m_Node,NULL,TRUE);
	else
		return S_OK;
}