// AveInstShowTextAction.cpp: implementation of the CAveInstShowTextAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstShowTextAction.h"
#include "AveInstallerEnvironment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstShowTextAction::CAveInstShowTextAction()
{

}

CAveInstShowTextAction::~CAveInstShowTextAction()
{

}

std::wstring CAveInstShowTextAction::ReadNodeText(XMLNode& node, const std::wstring& defaultText, const std::wstring& name)
{
	HRESULT hRes = S_OK;
	std::wstring text;

	XMLNode itemNode;
	CComBSTR bstrItemNodeName(name.c_str());
	CComBSTR bstrText;
	if(SUCCEEDED( hRes = node->selectSingleNode(bstrItemNodeName,&itemNode)) && 
		itemNode.p != NULL && 
		SUCCEEDED(hRes =  itemNode->get_text(&bstrText)) && bstrText != NULL)
	{
		text = static_cast<WCHAR*>(bstrText);
	}
	else
	{
		text = defaultText;
	}

	return text;
}

HRESULT CAveInstShowTextAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_INVALIDARG);

	m_Caption	= ReadNodeText(node,L"AveDesk Installer",L"Caption");
	m_Text		= ReadNodeText(node,L"",L"Text");

	return (hRes = S_OK);
}

HRESULT CAveInstShowTextAction::DoAction(CAveInstallerEnvironment& env)
{

	// showing text (a messagebox) is not silent!
	if(!env.IsSilentInstall())
		MessageBoxW(env.GetHWND(),m_Text.c_str(), m_Caption.c_str(), MB_ICONINFORMATION);

	return S_OK;
}