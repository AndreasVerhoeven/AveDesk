// AveInstRequirements.cpp: implementation of the CAveInstRequirements class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AveInstReqItem.h"
#include "AveInstRequirements.h"
#include "AveInstError.h"
#include "AveInstallerEnvironment.h"
#include "AveInstRequirementsFactory.h"
#include "AveInstFileFetcher.h"
#include "AveInstaller.h"

#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequirements::CAveInstRequirements()
{

}

CAveInstRequirements::~CAveInstRequirements()
{

}

HRESULT CAveInstRequirements::LoadFromNode(XMLNode& rootNode)
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
		if(SUCCEEDED( hRes = nodeList->get_item(lIndex,&newNode) ) && newNode.p != NULL)
		{
			boost::shared_ptr<CAveInstReqItem> reqItem;
			hRes = CAveInstRequirementsFactory::CreateRequirementItem(newNode, reqItem);

			if(SUCCEEDED(hRes))
				m_Items.push_back(reqItem);
		}
	}

	return S_OK;
}

bool CAveInstRequirements::DoesMeetRequirements(CAveInstallerEnvironment& env, CAveInstError& error)
{
	for(std::list<boost::shared_ptr<CAveInstReqItem> >::iterator iter = m_Items.begin(); iter != m_Items.end(); ++iter)
	{
		boost::shared_ptr<CAveInstReqItem> & req = *iter;
		if(req->DoesMeetRequirement(env, error) == false)
		{
			// try to recover from the error in the requirement by downloading
			// a file that was supplied in the installer-script.
			// if no such file was supplied, then the installation fails anyhow
			if(!error.GetDownloadableFileDescription().IsEmpty())
			{
				WCHAR path[MAX_PATH] = {0};
				GetTempPath(MAX_PATH, path);
				std::wstringstream ws;
				ws << this << GetTickCount() << L".zip";
				PathAppendW(path,ws.str().c_str());


				CAveInstFileFetcher fetcher;
				HRESULT hRes = fetcher.FetchFile(error.GetDownloadableFileDescription(),env.GetInstallerProgressIndicator(),path);
				if(SUCCEEDED(hRes))
				{
					CAveInstaller inst;
					inst.SetSuspendedDeskletsList(env.GetSuspendedDeskletsList());
					hRes = inst.Install(path,env.GetHWND(),TRUE,env.GetInstallerProgressIndicator());

					DeleteFileW(path);

					return SUCCEEDED(hRes);
				}
				else
				{
					return false;
				}
			}
			else
			{
				// there is no way we can recover from the error in the requirement.
				return false;
			}
		}
	}

	return true;
}