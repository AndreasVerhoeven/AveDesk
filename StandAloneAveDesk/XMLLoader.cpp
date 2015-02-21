// XMLLoader.cpp: implementation of the CXMLLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLLoader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define USE_UNSAFECHECKHRES HRESULT _hRes;
#define UNSAFE_CHECKHRES(r) if(FAILED( _hRes = (r)) ) return _hRes;

CRITICAL_SECTION CXMLLoader::cs = {0};

HRESULT CXMLLoader::SelectChildNode(const WCHAR* name)
{
	USE_UNSAFECHECKHRES;

	if(NULL == m_CurrentNode.p)
		return E_FAIL;

	XMLNode retNode;

	CComBSTR bstrXPath( name );
	UNSAFE_CHECKHRES (m_CurrentNode->selectSingleNode(bstrXPath,&retNode) );
	m_CurrentNode = retNode;

	return ((m_CurrentNode.p != NULL) ? S_OK : E_FAIL);
}

CXMLLoader::CXMLLoader()
{

}

ILoader* CXMLLoader::Clone()
{
	EnterCriticalSection(&cs);
	CXMLLoader* loader = new CXMLLoader;
	loader->m_CurrentNode.p = m_CurrentNode.p;
	if(loader->m_CurrentNode.p)loader->m_CurrentNode.p->AddRef();
	loader->m_Doc.p = m_Doc.p;
	if(loader->m_Doc.p)loader->m_Doc.p->AddRef();
	loader->m_NodeList.p = m_NodeList.p;
	if(loader->m_NodeList.p)loader->m_NodeList.p->AddRef();
	LeaveCriticalSection(&cs);

	return loader;
}

HRESULT CXMLLoader::GoToParentNode()
{
	USE_UNSAFECHECKHRES;

	if(NULL == m_CurrentNode.p)
		return E_FAIL;

	XMLNode parentNode;
	UNSAFE_CHECKHRES( m_CurrentNode->get_parentNode(&parentNode) );
	m_CurrentNode = parentNode;

	return S_OK;
}

HRESULT CXMLLoader::SelectNodes(const WCHAR* expr)
{
	USE_UNSAFECHECKHRES;

	if(NULL == m_CurrentNode.p)
		return E_FAIL;

	CComBSTR bstrExpr( expr );

	CComPtr<IXMLDOMNodeList> nodesList;
	UNSAFE_CHECKHRES( m_CurrentNode->selectNodes(bstrExpr,&nodesList) );

	m_NodeList = nodesList;

	return S_OK;
}

long CXMLLoader::NumberOfNodesInList()
{
	if(m_NodeList.p != NULL)
	{
		long lCount(0);

		if( FAILED( m_NodeList->get_length(&lCount) ) )
			return 0;
		else
			return lCount;
	}
	else
	{
		return 0;
	}
}

bool CXMLLoader::SelectNodeFromList(long lIndex)
{
	if(m_NodeList.p != NULL)
	{
		long lCount(0);
		if(FAILED( m_NodeList->get_length(&lCount) ))
			return false;

		if(lIndex >= 0 && lIndex < lCount)
		{
			XMLNode newNode;
			if(FAILED( m_NodeList->get_item(lIndex,&newNode) ) )
				return false;

			m_CurrentNode = newNode;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void CXMLLoader::Reset()
{
	m_CurrentNode = m_Doc;
}

CXMLLoader::CXMLLoader(const WCHAR* fileName, const WCHAR* rootNode,bool isXMLString)
{
	CHECKHRES( m_Doc.CoCreateInstance(CLSID_DOMDocument) );
	m_Doc->put_preserveWhiteSpace(VARIANT_TRUE);


	CComBSTR bstrFileName( fileName );
	CComVariant varFileName( bstrFileName );
	VARIANT_BOOL vbSuccess = { VARIANT_FALSE };


	if(!isXMLString)
		CHECKHRES ( m_Doc->load(varFileName,&vbSuccess) );
	else
		CHECKHRES ( m_Doc->loadXML(bstrFileName,&vbSuccess) );


	if(rootNode != NULL)
	{
		CComBSTR bstrXPath( rootNode );
		CHECKHRES ( m_Doc->selectSingleNode(bstrXPath,&m_CurrentNode)  );
	}
	else
	{
		m_CurrentNode = m_Doc;
	}
}

CXMLLoader::CXMLLoader(IStream* stream, const WCHAR* rootNode)
{
	ATLASSERT(stream != NULL);

	CHECKHRES( m_Doc.CoCreateInstance(CLSID_DOMDocument) );
	m_Doc->put_preserveWhiteSpace(VARIANT_TRUE);

	CComVariant varSource( stream );
	varSource.punkVal = stream;
	varSource.vt = VT_UNKNOWN;
	VARIANT_BOOL vbSuccess = { VARIANT_FALSE };

	CHECKHRES ( m_Doc->load(varSource,&vbSuccess) );


	if(rootNode != NULL)
	{
		CComBSTR bstrXPath( rootNode );
		CHECKHRES ( m_Doc->selectSingleNode(bstrXPath,&m_CurrentNode)  );
	}
	else
	{
		m_CurrentNode = m_Doc;
	}
}

CXMLLoader::CXMLLoader(IXMLDOMDocument* pDoc, const WCHAR* rootNode)
{
	m_Doc = pDoc;

	if(rootNode != NULL)
	{
		CComBSTR bstrXPath( rootNode );
		CHECKHRES ( m_Doc->selectSingleNode(bstrXPath,&m_CurrentNode)  );
	}
	else
	{
		m_CurrentNode = m_Doc;
	}
}

CXMLLoader::CXMLLoader(XMLNode& node)
{
	m_CurrentNode = node;
}

HRESULT CXMLLoader::LoadString(std::wstring& t, const WCHAR* name)
{	
	USE_UNSAFECHECKHRES;

	if(!m_CurrentNode.p)
		return E_FAIL;

	CComBSTR bstrName( name );
	XMLNode node;
	UNSAFE_CHECKHRES ( m_CurrentNode->selectSingleNode(bstrName,&node) );

	if(NULL == node.p)
		return E_FAIL;
		
	CComBSTR bstrText;
	UNSAFE_CHECKHRES( node->get_text(&bstrText) );
	
	t = static_cast<WCHAR*>(bstrText);

	return S_OK;
}

HRESULT CXMLLoader::LoadPersistableClass(IPersistable& p, const WCHAR* name)
{
	USE_UNSAFECHECKHRES;

	if(!m_CurrentNode.p)
		return E_FAIL;

	if(name != NULL && name[0] != L'\0')
	{
		UNSAFE_CHECKHRES(SelectChildNode(name));
	}


	HRESULT hRes = p.Load(*this);

	if(name != NULL && name[0] != L'\0')
	{
		UNSAFE_CHECKHRES(GoToParentNode());
	}

	return hRes;
}
