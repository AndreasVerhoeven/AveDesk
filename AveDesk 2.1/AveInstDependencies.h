// AveInstDependencies.h: interface for the CAveInstDependencies class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTDEPENDENCIES_H__5F19CFDD_B747_4F88_8C5B_80E70AF6E2A9__INCLUDED_)
#define AFX_AVEINSTDEPENDENCIES_H__5F19CFDD_B747_4F88_8C5B_80E70AF6E2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

class CAveInstDependencies  
{
	std::vector<std::wstring> m_List;

public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

	CAveInstDependencies();
	virtual ~CAveInstDependencies();

	virtual HRESULT LoadFromNode(XMLNode node);

};

#endif // !defined(AFX_AVEINSTDEPENDENCIES_H__5F19CFDD_B747_4F88_8C5B_80E70AF6E2A9__INCLUDED_)
