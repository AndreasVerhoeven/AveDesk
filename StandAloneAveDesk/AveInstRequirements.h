// AveInstRequirements.h: interface for the CAveInstRequirements class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREMENTS_H__B80E5652_BB11_46B7_AC6A_2F71F72AAFA9__INCLUDED_)
#define AFX_AVEINSTREQUIREMENTS_H__B80E5652_BB11_46B7_AC6A_2F71F72AAFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>
#include <list>

class CAveInstReqItem;
class CAveInstallerEnvironment;
class CAveInstError;

class CAveInstRequirements
{
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

protected:
	std::list<boost::shared_ptr<CAveInstReqItem> > m_Items;
public:

	CAveInstRequirements();
	virtual ~CAveInstRequirements();
	HRESULT LoadFromNode(XMLNode& rootNode);

	bool DoesMeetRequirements(CAveInstallerEnvironment& env, CAveInstError& error);

};

#endif // !defined(AFX_AVEINSTREQUIREMENTS_H__B80E5652_BB11_46B7_AC6A_2F71F72AAFA9__INCLUDED_)
