// AveInstRequiredPackage.cpp: implementation of the CAveInstRequiredPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRequiredPackage.h"
#include "DownloadableFileDescription.h"
#include "AveInstallerEnvironment.h"
#include "AveInstError.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequiredPackage::CAveInstRequiredPackage()
{

}

CAveInstRequiredPackage::~CAveInstRequiredPackage()
{

}

bool CAveInstRequiredPackage::DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error)
{
	if(env.IsPackageInstalled(m_GUID))
	{
		return true;
	}
	else
	{
		if(m_Error.GetMessage().empty())
		{
			CString str;
			str.Format(IDS_PACKAGENOTINSTALLED,m_GUID.c_str());
			m_Error.SetMessage(str.operator LPCTSTR());
		}

		if(m_Error.GetDownloadableFileDescription().GetGUID().empty())
		{
			m_Error.GetDownloadableFileDescription().SetGUID(m_GUID);
		}

		error = m_Error;

		return false;
	}
}

HRESULT CAveInstRequiredPackage::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	if( NULL == node.p)
		return (hRes = E_INVALIDARG);

	hRes = CAveInstReqItem::LoadFromNode(node);

	CComBSTR bstrName(L"GUID");
	XMLNode guidNode;
	if(FAILED(hRes = node->selectSingleNode(bstrName,&guidNode)) || NULL == guidNode.p)
		return hRes;

	CComBSTR bstrGUID;
	if(FAILED(hRes = guidNode->get_text(&bstrGUID)) || NULL == static_cast<WCHAR*>(bstrGUID))
		return hRes;

	WCHAR* wGUID = static_cast<WCHAR*>(bstrGUID);
	m_GUID = wGUID;

	return (hRes = S_OK);
}