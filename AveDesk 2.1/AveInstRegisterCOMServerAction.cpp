// AveInstRegisterCOMServerAction.cpp: implementation of the CAveInstRegisterCOMServerAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRegisterCOMServerAction.h"
#include "AveInstallerEnvironment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRegisterCOMServerAction::CAveInstRegisterCOMServerAction() :
	m_UseAlias(false)
{

}

CAveInstRegisterCOMServerAction::~CAveInstRegisterCOMServerAction()
{

}

HRESULT CAveInstRegisterCOMServerAction::LoadFromNode(XMLNode& node)
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

	CComBSTR bstrText;
	if(FAILED( hRes = node->get_text(&bstrText)))
		return hRes;

	if(bstrText != NULL)
		m_FileName = static_cast<WCHAR*>(bstrText);

	return S_OK;
}

HRESULT CAveInstRegisterCOMServerAction::DoAction(CAveInstallerEnvironment& env)
{
	HRESULT hRes = S_OK;
	WCHAR path[MAX_PATH] = {0};

	if(m_UseAlias)
	{
		wcscpy(path,env.GetFileAlias(m_FileName).c_str() );
	}
	else
	{
		wcscpy(path,env.GetAveDeskEnvironment().GetInstallPath().c_str() );
		PathAppendW(path, m_FileName.c_str() );
	}

	HMODULE hMod = LoadLibraryW(path);

	if(hMod)
	{
	
		int (__stdcall *DllRegisterServerFunc)() = (int (__stdcall *)()) GetProcAddress(hMod,"DllRegisterServer");

		if(DllRegisterServerFunc)
		{
			DllRegisterServerFunc();
			hRes = S_OK;
		}
		else
		{
			hRes = E_FAIL;
		}

		FreeLibrary(hMod);
	}
	else
	{
		hRes = E_FAIL;
	}

	return hRes;
}