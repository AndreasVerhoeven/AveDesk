// AveInstActions.h: interface for the CAveInstActions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTACTIONS_H__0A47F1EC_6AC7_474B_B5D1_09F23FE3C7DF__INCLUDED_)
#define AFX_AVEINSTACTIONS_H__0A47F1EC_6AC7_474B_B5D1_09F23FE3C7DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>
#include <list>

class CAveInstAction;
class CAveInstallerEnvironment;

class CAveInstActions  
{
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

protected:
	std::list<boost::shared_ptr<CAveInstAction> > m_Items;
public:
	CAveInstActions();
	virtual ~CAveInstActions();

	HRESULT LoadFromNode(XMLNode& rootNode);
	virtual HRESULT DoActions(CAveInstallerEnvironment& env);

};

#endif // !defined(AFX_AVEINSTACTIONS_H__0A47F1EC_6AC7_474B_B5D1_09F23FE3C7DF__INCLUDED_)
