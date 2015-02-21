// DownloadableFileDescription.cpp: implementation of the CDownloadableFileDescription class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloadableFileDescription.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloadableFileDescription::CDownloadableFileDescription()
{

}

CDownloadableFileDescription::~CDownloadableFileDescription()
{

}

std::wstring CDownloadableFileDescription::ReadNodeText(XMLNode rootNode, const WCHAR* name)
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

HRESULT CDownloadableFileDescription::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	m_URL	= ReadNodeText(node,L"URL");
	m_GUID	= ReadNodeText(node,L"GUID");

	return S_OK;
}

std::wstring CDownloadableFileDescription::GetURL()const
{
	return m_URL;
}

std::wstring CDownloadableFileDescription::GetGUID()const
{
	return m_GUID;
}

void CDownloadableFileDescription::SetURL(const std::wstring& url)
{
	m_URL = url;
}

void CDownloadableFileDescription::SetGUID(const std::wstring& guid)
{
	m_GUID = guid;
}

bool CDownloadableFileDescription::IsEmpty()const
{
	return m_URL.empty() && m_GUID.empty();
}