// AveInstNotRequired.cpp: implementation of the CAveInstNotRequired class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstNotRequired.h"
#include "AveInstRequirementsFactory.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstNotRequired::CAveInstNotRequired()
{

}

CAveInstNotRequired::~CAveInstNotRequired()
{

}

bool CAveInstNotRequired::DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error)
{
	if(m_NotItem.get() )
	{
		bool doesMeetRequirement =  !m_NotItem->DoesMeetRequirement(env,error);

		if(!doesMeetRequirement)
		{
			if(!m_Error.GetMessage().empty())
				error = m_Error;
			else
			{
				CString str;
				str.Format(IDS_REQUIREMENTMETERROR, error.GetMessage() .c_str() );
				error.SetMessage( str.operator LPCTSTR() );
			}
		}

		return doesMeetRequirement;
	}
	else
	{
		return false;
	}
}


HRESULT CAveInstNotRequired::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	hRes = CAveInstReqItem::LoadFromNode(node);

	XMLNode notNode;

	CComBSTR bstrItem(L"Item");
	if(FAILED(hRes = node->selectSingleNode(bstrItem,&notNode)))
		return hRes;

	hRes = CAveInstRequirementsFactory::CreateRequirementItem(notNode,m_NotItem);

	return hRes;
}