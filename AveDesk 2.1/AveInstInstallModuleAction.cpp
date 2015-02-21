// AveInstInstallModuleAction.cpp: implementation of the CAveInstInstallModuleAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstInstallModuleAction.h"
#include "AveInstallerEnvironment.h"
#include "Modules.h"

extern CModules* _Modules;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstInstallModuleAction::CAveInstInstallModuleAction() :
	m_UseAlias(false)
{

}

CAveInstInstallModuleAction::~CAveInstInstallModuleAction()
{

}

HRESULT CAveInstInstallModuleAction::LoadFromNode(XMLNode& node)
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

HRESULT CAveInstInstallModuleAction::DoAction(CAveInstallerEnvironment& env)
{
	HRESULT hRes = S_OK;

	if(m_UseAlias)
	{
		std::wstring alias = env.GetFileAlias(m_FileName);
		if(!alias.empty())
			_Modules->LoadModule(alias);
	}
	else
	{
		CAveInstallerEnvironment tmpEnv = env; // use a temporary environment
		m_Dir.PerformOperation(*this,tmpEnv,tmpEnv.GetAveDeskEnvironment().GetInstallPath(),tmpEnv.GetAveDeskEnvironment().GetInstallPath());
	}

	return (hRes = S_OK);
}

bool CAveInstInstallModuleAction::FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	if(PathFileExists(dstPath.c_str()))
	{
		_Modules->LoadModule(dstPath);
	}

	return true;
}

bool CAveInstInstallModuleAction::DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	return PathFileExists(dstPath.c_str()) == TRUE;
}