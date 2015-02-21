// XMLSaver.cpp: implementation of the CXMLSaver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLSaver.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define USE_UNSAFECHECKHRES HRESULT _hRes;
#define UNSAFE_CHECKHRES(r) if(FAILED( _hRes = (r)) ) return _hRes;

HRESULT CXMLSaver::GoToParentNode()
{
	USE_UNSAFECHECKHRES;

	XMLNode parentNode;
	UNSAFE_CHECKHRES( m_CurrentNode->get_parentNode(&parentNode) );
	m_CurrentNode = parentNode;

	return S_OK;
}

HRESULT CXMLSaver::AddNode(const WCHAR* name)
{
	USE_UNSAFECHECKHRES;

	CComBSTR bstrName( name );
	CComBSTR bstrNamespace( L"" );

	XMLNode newNode;

	CComVariant varNodeType(NODE_ELEMENT);
	UNSAFE_CHECKHRES( m_Doc->createNode(varNodeType,bstrName,bstrNamespace,&newNode) );

	XMLNode retNode;
	UNSAFE_CHECKHRES( m_CurrentNode->appendChild(newNode,&retNode) );

	m_CurrentNode = retNode;

	return S_OK;
}

CXMLSaver::CXMLSaver(const WCHAR* rootNode)
{
	CHECKHRES( m_Doc.CoCreateInstance(CLSID_DOMDocument) );
	m_Doc->put_preserveWhiteSpace(VARIANT_TRUE);

	CComBSTR bstrName( rootNode );
	CComBSTR bstrNamespace( L"");

	CComVariant varNodeType(NODE_ELEMENT);
	XMLNode newNode;
	CHECKHRES( m_Doc->createNode(varNodeType,bstrName,bstrNamespace,&newNode) );

	XMLNode retNode;
	CHECKHRES(m_Doc->appendChild(newNode,&retNode) );

	m_CurrentNode = retNode;
}

IXMLDOMDocument* CXMLSaver::GetDOMDocument()
{
	return m_Doc;
}

HRESULT CXMLSaver::Save(const WCHAR* fileName)
{
	CComBSTR bstrFileName(fileName);
	CComVariant varFileName( bstrFileName );
	return m_Doc->save(varFileName);
}

HRESULT CXMLSaver::SaveToStream(IStream* stream)
{
	ATLASSERT(stream != NULL);

	CComVariant varSource(stream);
	varSource.vt = VT_UNKNOWN;
	varSource.punkVal = stream;

	return m_Doc->save(varSource);
}



HRESULT CXMLSaver::SaveString(const std::wstring& t, const WCHAR* name)
{
	USE_UNSAFECHECKHRES;

	CComBSTR bstrName( name );

	CComPtr<IXMLDOMElement> element;
	UNSAFE_CHECKHRES( m_Doc->createElement(bstrName,&element) );

	CComBSTR bstrT( t.c_str() );
	//CComVariant varT( bstrT );
	//CComBSTR bstrValueStr( L"Value");

	//CHECKHRES( element->setAttribute(bstrValueStr,varT) );

	UNSAFE_CHECKHRES ( element->put_text(bstrT) );

	XMLNode retNode;
	UNSAFE_CHECKHRES( m_CurrentNode->appendChild(element,&retNode) );

	return S_OK;
}

HRESULT CXMLSaver::SavePersistableClass(IPersistable& p, const WCHAR* name)
{
	USE_UNSAFECHECKHRES;

	if(name != NULL && name[0] != L'\0')
	{
		UNSAFE_CHECKHRES(AddNode(name));
	}


	HRESULT hRes = p.Save(*this);

	if(name != NULL && name[0] != L'\0')
	{
		UNSAFE_CHECKHRES(GoToParentNode());
	}

	return hRes;
}
