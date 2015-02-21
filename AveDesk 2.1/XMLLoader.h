// XMLLoader.h: interface for the CXMLLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLLOADER_H__98F2AEE3_8C48_409F_BFC4_B917DFDEB3C6__INCLUDED_)
#define AFX_XMLLOADER_H__98F2AEE3_8C48_409F_BFC4_B917DFDEB3C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPersistable.h"

class CXMLLoader : public ILoader  
{
public:
	static CRITICAL_SECTION cs;
private:
	

	typedef CComPtr<IXMLDOMNode> XMLNode;

	CComPtr<IXMLDOMDocument> m_Doc;
	XMLNode m_CurrentNode;
	CComPtr<IXMLDOMNodeList> m_NodeList;

	CXMLLoader();

public:
	CXMLLoader(IStream* stream, const WCHAR* rootNode=NULL);
	CXMLLoader(XMLNode& node);
	CXMLLoader(const WCHAR* fileName, const WCHAR* rootNode=NULL,bool isXMLString = false);
	CXMLLoader(IXMLDOMDocument* pDoc, const WCHAR* rootNode=NULL);
	virtual ~CXMLLoader() {};

	HRESULT SelectChildNode(const WCHAR* name);
	HRESULT GoToParentNode();
	HRESULT SelectNodes(const WCHAR* expr);

	long NumberOfNodesInList();
	bool SelectNodeFromList(long lIndex);
	void Reset();


	HRESULT virtual LoadString(std::wstring& t, const WCHAR* name);
	HRESULT virtual LoadPersistableClass(IPersistable& p, const WCHAR* name);

	virtual ILoader* Clone();

};

#endif // !defined(AFX_XMLLOADER_H__98F2AEE3_8C48_409F_BFC4_B917DFDEB3C6__INCLUDED_)
