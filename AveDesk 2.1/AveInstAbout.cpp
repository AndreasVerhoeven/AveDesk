// AveInstAbout.cpp: implementation of the CAveInstAbout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstAbout.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstAbout::CAveInstAbout()
{

}

CAveInstAbout::~CAveInstAbout()
{

}

std::wstring CAveInstAbout::ReadNodeText(XMLNode rootNode, const WCHAR* name)const
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

std::wstring CAveInstAbout::GetAuthor()const
{
	return m_Author;
}

std::wstring CAveInstAbout::GetEmail()const
{
	return m_Email;
}

std::wstring CAveInstAbout::GetHomePage()const
{
	return m_HomePage;
}

std::wstring CAveInstAbout::GetName()const
{
	return m_Name;
}

std::wstring CAveInstAbout::GetVersion()const
{
	return m_Version;
}

std::wstring CAveInstAbout::GetDescription()const
{
	return m_Description;
}

std::wstring CAveInstAbout::GetURL()const
{
	return m_URL;
}

std::wstring CAveInstAbout::GetGUID()const
{
	return m_GUID;
}
	std::wstring m_Author;
	std::wstring m_Email;
	std::wstring m_HomePage;
	std::wstring m_Name;
	std::wstring m_Version;
	std::wstring m_Description;
	std::wstring m_URL;
	std::wstring m_GUID;


HRESULT CAveInstAbout::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	m_Author		= ReadNodeText(node,L"Author"); 
	m_Email			= ReadNodeText(node,L"Email"); 	
	m_HomePage		= ReadNodeText(node,L"HomePage"); 
	m_Name			= ReadNodeText(node,L"Name"); 
	m_Version		= ReadNodeText(node,L"Version"); 
	m_Description	= ReadNodeText(node,L"Description"); 
	m_URL			= ReadNodeText(node,L"URL"); 
	m_GUID			= ReadNodeText(node,L"GUID"); 

	return hRes;
}