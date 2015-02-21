// AveInstUnloadModuleAction.cpp: implementation of the CAveInstUnloadModuleAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstUnloadModuleAction.h"
#include "AveInstallerEnvironment.h"
#include "Modules.h"
#include "Module.h"

extern CModules* _Modules;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstUnloadModuleAction::CAveInstUnloadModuleAction() :
	m_UseAlias(false)
{

}

CAveInstUnloadModuleAction::~CAveInstUnloadModuleAction()
{

}

void CAveInstUnloadModuleAction::UnloadModuleByFileName(const std::wstring& fname)
{
	std::vector<CModule*> modules = _Modules->GetCurrentModules();	


	std::vector<CModule*>::iterator iter = modules.begin();
	int index(0);
	while(iter != modules.end())
	{
		CModule* m = *iter;
		if(_wcsicmp(m->GetFilePath(),fname.c_str()) == 0)
		{
			_Modules->RemoveModule(index);
			modules = _Modules->GetCurrentModules();
			iter = modules.begin();
		}
		else
		{
			index++;
			iter++;
		}
	}
}

HRESULT CAveInstUnloadModuleAction::LoadFromNode(XMLNode& node)
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

HRESULT CAveInstUnloadModuleAction::DoAction(CAveInstallerEnvironment& env)
{
	HRESULT hRes = S_OK;

	if(m_UseAlias)
	{
		std::wstring alias = env.GetFileAlias(m_FileName);
		if(!alias.empty())
		{
			UnloadModuleByFileName(alias);
		}
	}
	else
	{
		CAveInstallerEnvironment tmpEnv = env; // use a temporary environment
		m_Dir.PerformOperation(*this,tmpEnv,tmpEnv.GetAveDeskEnvironment().GetInstallPath(),tmpEnv.GetAveDeskEnvironment().GetInstallPath());
	}

	return (hRes = S_OK);
}

bool CAveInstUnloadModuleAction::FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	if(PathFileExists(dstPath.c_str()))
	{
		UnloadModuleByFileName(dstPath);
	}

	return true;
}

bool CAveInstUnloadModuleAction::DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	return PathFileExists(dstPath.c_str()) == TRUE;
}