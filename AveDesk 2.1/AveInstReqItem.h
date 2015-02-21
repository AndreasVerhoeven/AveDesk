// AveInstReqItem.h: interface for the CAveInstReqItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQITEM_H__3B304ED4_EFDF_4A62_B569_4E0EB6BE434C__INCLUDED_)
#define AFX_AVEINSTREQITEM_H__3B304ED4_EFDF_4A62_B569_4E0EB6BE434C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "AveInstError.h"

class CAveInstallerEnvironment;

class CAveInstReqItem  
{
protected:
	CAveInstError m_Error;
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

	CAveInstReqItem();
	virtual ~CAveInstReqItem() = 0;

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error) = 0;
	virtual HRESULT LoadFromNode(XMLNode node) = 0;

};

#endif // !defined(AFX_AVEINSTREQITEM_H__3B304ED4_EFDF_4A62_B569_4E0EB6BE434C__INCLUDED_)
