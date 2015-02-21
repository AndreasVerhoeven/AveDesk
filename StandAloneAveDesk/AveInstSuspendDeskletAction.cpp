// AveInstSuspendDeskletAction.cpp: implementation of the CAveInstSuspendDeskletAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstSuspendDeskletAction.h"
#include "AveInstallerEnvironment.h"
#include "AveInstSuspendedDeskletsList.h"
#include "AveInstSuspendedDesklets.h"
#include "Docklet.h"
#include <vector>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstSuspendDeskletAction::CAveInstSuspendDeskletAction() :
	m_UseAlias(false)
{

}

CAveInstSuspendDeskletAction::~CAveInstSuspendDeskletAction()
{

}

void CAveInstSuspendDeskletAction::UnloadDeskletByFileName(CAveInstallerEnvironment& env, const std::wstring& fname)
{
	CAveInstSuspendedDeskletsList* sdl = env.GetSuspendedDeskletsList();
	if(NULL == sdl)
		return;

	CAveInstSuspendedDesklets& sd = sdl->GetSuspendedDesklets(fname);
	sd.AddCount();

	std::vector<Docklet*> docklets;
	EnterCriticalSection(&Docklet::cs);
	for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
	{
		Docklet* d = iter->second;
		if(d)
		{
			WCHAR path[MAX_PATH] = {0};
			GetModuleFileName(d->GetDocklet()->GetModule(),path,MAX_PATH);
			if(_wcsicmp(path,fname.c_str()) == 0)
				docklets.push_back(iter->second);
		}
	}
	LeaveCriticalSection(&Docklet::cs);

	std::vector<HANDLE> handles;
	for(std::vector<Docklet*>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
	{
		Docklet* d = *d_iter;
		if(d)
		{
			handles.push_back(d->GetThreadHandle());
			sd.Add(d);
		}
	}

	if(handles.size() > 0)
		DWORD dw = WaitForMultipleObjects(handles.size(), &handles[0], TRUE, 30000);

}


HRESULT CAveInstSuspendDeskletAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_INVALIDARG);


	CComPtr<IXMLDOMNamedNodeMap> nodeMap;
	if(SUCCEEDED(hRes = node->get_attributes(&nodeMap)) && nodeMap.p != NULL)
	{
		CComBSTR bstrName( L"UseAlias");
		XMLNode namedNode;
		if(SUCCEEDED(hRes = nodeMap->getNamedItem(bstrName,&namedNode)) && namedNode.p != NULL)
		{
			CComBSTR bstrUseAlias;
			if(SUCCEEDED( hRes = namedNode->get_text(&bstrUseAlias)) && bstrUseAlias != NULL)
			{
				const WCHAR* wUseAlias = static_cast<WCHAR*>(bstrUseAlias);
				m_UseAlias = _wcsicmp(wUseAlias,L"true") == 0 || wcscmp(wUseAlias,L"1") == 0 || _wcsicmp(wUseAlias,L"yes") == 0;
			}
		}
	}

	if(m_UseAlias)
	{
		CComBSTR bstrText;
		if(FAILED( hRes = node->get_text(&bstrText)))
			return hRes;
	
		if(bstrText != NULL)
			m_FileName = static_cast<WCHAR*>(bstrText);

		return (hRes = S_OK);
	}
	else
	{
		return (hRes = m_Dir.LoadFromNode(node));	
	}

	return (hRes = S_OK);
}

HRESULT CAveInstSuspendDeskletAction::DoAction(CAveInstallerEnvironment& env)
{
	HRESULT hRes = S_OK;

	if(m_UseAlias)
	{
		std::wstring alias = env.GetFileAlias(m_FileName);
		if(!alias.empty())
		{
			UnloadDeskletByFileName(env,alias);
		}
	}
	else
	{
		CAveInstallerEnvironment tmpEnv = env; // use a temporary environment
		m_Dir.PerformOperation(*this,tmpEnv,tmpEnv.GetAveDeskEnvironment().GetInstallPath(),tmpEnv.GetAveDeskEnvironment().GetInstallPath());
	}

	return (hRes = S_OK);
}

bool CAveInstSuspendDeskletAction::FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	if(PathFileExists(dstPath.c_str()))
	{
		UnloadDeskletByFileName(env,dstPath);
	}

	return true;
}

bool CAveInstSuspendDeskletAction::DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	return PathFileExists(dstPath.c_str()) == TRUE;
}