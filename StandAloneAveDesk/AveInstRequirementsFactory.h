// AveInstRequirementsFactory.h: interface for the CAveInstRequirementsFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREMENTSFACTORY_H__C4376B4F_3AF4_4164_B354_CB6E60619667__INCLUDED_)
#define AFX_AVEINSTREQUIREMENTSFACTORY_H__C4376B4F_3AF4_4164_B354_CB6E60619667__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>
#include <string>

class CAveInstReqItem;

class CAveInstRequirementsFactory  
{
private:
	CAveInstRequirementsFactory();
	virtual ~CAveInstRequirementsFactory();

public:
	typedef CComPtr<IXMLDOMNode> XMLNode;
	static HRESULT GetTypeFromNode(XMLNode node,std::wstring& type);
	static HRESULT CreateRequirementItem(XMLNode node, boost::shared_ptr<CAveInstReqItem>& reqItem);

};

#endif // !defined(AFX_AVEINSTREQUIREMENTSFACTORY_H__C4376B4F_3AF4_4164_B354_CB6E60619667__INCLUDED_)
