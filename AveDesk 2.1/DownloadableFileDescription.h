// DownloadableFileDescription.h: interface for the CDownloadableFileDescription class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADABLEFILEDESCRIPTION_H__48F58C9C_31D5_4976_9816_A50CA8F959E4__INCLUDED_)
#define AFX_DOWNLOADABLEFILEDESCRIPTION_H__48F58C9C_31D5_4976_9816_A50CA8F959E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CDownloadableFileDescription  
{
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

protected:
	std::wstring m_URL;
	std::wstring m_GUID;

	std::wstring ReadNodeText(XMLNode rootNode, const WCHAR* name);

public:
	CDownloadableFileDescription();
	virtual ~CDownloadableFileDescription();

	virtual HRESULT LoadFromNode(XMLNode node);

	virtual std::wstring GetURL() const;
	virtual std::wstring GetGUID() const;
	virtual bool IsEmpty()const;

	virtual void SetURL(const std::wstring& url);
	virtual void SetGUID(const std::wstring& guid);

};

#endif // !defined(AFX_DOWNLOADABLEFILEDESCRIPTION_H__48F58C9C_31D5_4976_9816_A50CA8F959E4__INCLUDED_)
