// AveInstError.cpp: implementation of the CAveInstError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstError::CAveInstError()
{

}

CAveInstError::~CAveInstError()
{

}

void CAveInstError::SetMessage(const std::wstring& msg)
{
	m_Message = msg;
}

void CAveInstError::SetHomePage(const std::wstring& homepage)
{
	m_HomePage = homepage;
}

std::wstring CAveInstError::GetMessage()const
{
	return m_Message;
}

std::wstring CAveInstError::GetHomePage()const
{
	return m_HomePage;
}


std::wstring CAveInstError::ReadNodeText(XMLNode rootNode, const WCHAR* name)
{
	XMLNode node;
	CComBSTR bstrText;
	CComBSTR bstrName(name);
	if(	SUCCEEDED(rootNode->selectSingleNode(bstrName,&node)) &&
		node.p != NULL &&
		SUCCEEDED(node->get_text(&bstrText))
		&& bstrText != NULL)
	{
		return static_cast<WCHAR*>(bstrText);
	}
	else
	{
		return L"";
	}
}

CDownloadableFileDescription& CAveInstError::GetDownloadableFileDescription()
{
	return m_DownloadableFileDescription;
}

void CAveInstError::SetDownloadableFileDescription(CDownloadableFileDescription& descr)
{
	m_DownloadableFileDescription = descr;
}

HRESULT CAveInstError::LoadFromNode(XMLNode &node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	m_Message		= ReadNodeText(node,L"Message");
	m_HomePage		= ReadNodeText(node,L"HomePage");

	XMLNode descrNode;
	CComBSTR descrName(L"DownloadableFile");
	if(	SUCCEEDED( hRes = node->selectSingleNode(descrName,&descrNode)) && descrNode.p != NULL)
		hRes = m_DownloadableFileDescription.LoadFromNode(descrNode);

	return S_OK;
}