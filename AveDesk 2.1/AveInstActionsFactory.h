// AveInstActionsFactory.h: interface for the CAveInstActionsFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTACTIONSFACTORY_H__1D0DFC0D_1797_438B_A9AF_9F18C8FCC6C1__INCLUDED_)
#define AFX_AVEINSTACTIONSFACTORY_H__1D0DFC0D_1797_438B_A9AF_9F18C8FCC6C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>
#include <string>

#include "AveInstAction.h"

class CAveInstActionsFactory  
{
private:
	CAveInstActionsFactory();
	virtual ~CAveInstActionsFactory();

public:
	typedef CComPtr<IXMLDOMNode> XMLNode;
	static HRESULT GetTypeFromNode(XMLNode node,std::wstring& type);
	static HRESULT CreateActionItem(XMLNode node, boost::shared_ptr<CAveInstAction>& actItem);

};

#endif // !defined(AFX_AVEINSTACTIONSFACTORY_H__1D0DFC0D_1797_438B_A9AF_9F18C8FCC6C1__INCLUDED_)
