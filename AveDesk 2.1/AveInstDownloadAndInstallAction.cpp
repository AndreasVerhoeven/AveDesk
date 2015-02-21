// AveInstDownloadAndInstallAction.cpp: implementation of the CAveInstDownloadAndInstallAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstDownloadAndInstallAction.h"
#include "AveInstFileFetcher.h"
#include "AveInstaller.h"
#include "AveInstallerEnvironment.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstDownloadAndInstallAction::CAveInstDownloadAndInstallAction()
{

}

CAveInstDownloadAndInstallAction::~CAveInstDownloadAndInstallAction()
{

}

HRESULT CAveInstDownloadAndInstallAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	XMLNode descrNode;
	CComBSTR descrName(L"DownloadableFile");
	if(	SUCCEEDED( hRes = node->selectSingleNode(descrName,&descrNode)) && descrNode.p != NULL)
		hRes = m_FileDescr.LoadFromNode(descrNode);


	return hRes;
}


HRESULT CAveInstDownloadAndInstallAction::DoAction(CAveInstallerEnvironment& env)
{
	HRESULT hRes = S_OK;

	WCHAR path[MAX_PATH] = {0};
	GetTempPath(MAX_PATH, path);
	std::wstringstream ws;
	ws << this << GetTickCount() << L".zip";
	PathAppendW(path,ws.str().c_str());


	CAveInstFileFetcher fetcher;
	hRes = fetcher.FetchFile(m_FileDescr,env.GetInstallerProgressIndicator(),path);
	if(SUCCEEDED(hRes))
	{
		CAveInstaller inst;
		inst.SetSuspendedDeskletsList(env.GetSuspendedDeskletsList());
		hRes = inst.Install(path,env.GetHWND(),TRUE,env.GetInstallerProgressIndicator());

		DeleteFileW(path);
	}
	else
	{
		hRes = E_FAIL;
	}

	return hRes;
}