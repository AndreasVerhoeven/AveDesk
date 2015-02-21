// XMLSaver.h: interface for the CXMLSaver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLSAVER_H__4B0C8224_6776_4098_A9E4_BDA278B7415C__INCLUDED_)
#define AFX_XMLSAVER_H__4B0C8224_6776_4098_A9E4_BDA278B7415C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPersistable.h"

class CXMLSaver : public ISaver  
{
private:
	typedef CComPtr<IXMLDOMNode> XMLNode;

	CComPtr<IXMLDOMDocument> m_Doc;
	XMLNode m_CurrentNode;

public:
	CXMLSaver::CXMLSaver(const WCHAR* rootNode);
	virtual ~CXMLSaver() {};

	HRESULT GoToParentNode();
	HRESULT AddNode(const WCHAR* name);

	IXMLDOMDocument* GetDOMDocument();

	HRESULT Save(const WCHAR* fileName);
	HRESULT SaveToStream(IStream* stream);

	virtual HRESULT SaveString(const std::wstring& t, const WCHAR* name);
	virtual HRESULT SavePersistableClass(IPersistable& p, const WCHAR* name);

};

#endif // !defined(AFX_XMLSAVER_H__4B0C8224_6776_4098_A9E4_BDA278B7415C__INCLUDED_)
