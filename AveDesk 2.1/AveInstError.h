// AveInstError.h: interface for the CAveInstError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTERROR_H__EAA59D2F_03C1_45AC_BBF7_435DCD6C2FBF__INCLUDED_)
#define AFX_AVEINSTERROR_H__EAA59D2F_03C1_45AC_BBF7_435DCD6C2FBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "DownloadableFileDescription.h"

class CAveInstError  
{
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

protected:
	std::wstring m_Message;
	std::wstring m_DownloadURL;
	std::wstring m_GUID;
	std::wstring m_HomePage;
	CDownloadableFileDescription m_DownloadableFileDescription;

	std::wstring ReadNodeText(XMLNode rootNode, const WCHAR* name);
public:

	CAveInstError();
	virtual ~CAveInstError();

	virtual void SetMessage(const std::wstring& msg);
	virtual void SetHomePage(const std::wstring& homepage);
	virtual void SetDownloadableFileDescription(CDownloadableFileDescription& descr);

	virtual std::wstring GetMessage()const;
	virtual std::wstring GetHomePage()const;
	virtual CDownloadableFileDescription& GetDownloadableFileDescription();

	virtual HRESULT LoadFromNode(XMLNode &node);


};

#endif // !defined(AFX_AVEINSTERROR_H__EAA59D2F_03C1_45AC_BBF7_435DCD6C2FBF__INCLUDED_)
