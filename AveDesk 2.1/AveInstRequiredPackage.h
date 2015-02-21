// AveInstRequiredPackage.h: interface for the CAveInstRequiredPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREDPACKAGE_H__D6C4BE23_1BB5_43F7_A9C5_38EF9F266E65__INCLUDED_)
#define AFX_AVEINSTREQUIREDPACKAGE_H__D6C4BE23_1BB5_43F7_A9C5_38EF9F266E65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstReqItem.h"

class CAveInstRequiredPackage : public CAveInstReqItem  
{
protected:
	std::wstring m_GUID;
public:
	CAveInstRequiredPackage();
	virtual ~CAveInstRequiredPackage();

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error);
	virtual HRESULT LoadFromNode(XMLNode node);

};

#endif // !defined(AFX_AVEINSTREQUIREDPACKAGE_H__D6C4BE23_1BB5_43F7_A9C5_38EF9F266E65__INCLUDED_)
