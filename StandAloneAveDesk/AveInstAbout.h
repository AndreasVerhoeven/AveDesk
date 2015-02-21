// AveInstAbout.h: interface for the CAveInstAbout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTABOUT_H__F6A7A708_08DC_4425_82FA_48F0F857B39D__INCLUDED_)
#define AFX_AVEINSTABOUT_H__F6A7A708_08DC_4425_82FA_48F0F857B39D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CAveInstAbout  
{
private:
	std::wstring m_Author;
	std::wstring m_Email;
	std::wstring m_HomePage;
	std::wstring m_Name;
	std::wstring m_Version;
	std::wstring m_Description;
	std::wstring m_URL;
	std::wstring m_GUID;

public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

protected:
	std::wstring ReadNodeText(XMLNode rootNode, const WCHAR* name)const;

public:

	CAveInstAbout();
	virtual ~CAveInstAbout();

	std::wstring GetAuthor()const;
	std::wstring GetEmail()const;
	std::wstring GetHomePage()const;
	std::wstring GetName()const;
	std::wstring GetVersion()const;
	std::wstring GetDescription()const;
	std::wstring GetURL()const;
	std::wstring GetGUID()const;

	virtual HRESULT LoadFromNode(XMLNode node);

};

#endif // !defined(AFX_AVEINSTABOUT_H__F6A7A708_08DC_4425_82FA_48F0F857B39D__INCLUDED_)
