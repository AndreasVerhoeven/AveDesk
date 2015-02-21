// AveInstRequiredInstallerVersion.h: interface for the CAveInstRequiredInstallerVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREDINSTALLERVERSION_H__0FDE98AB_E39B_4BD1_8662_3A6ABE67ACDC__INCLUDED_)
#define AFX_AVEINSTREQUIREDINSTALLERVERSION_H__0FDE98AB_E39B_4BD1_8662_3A6ABE67ACDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstReqItem.h"

class CAveInstRequiredInstallerVersion : public CAveInstReqItem  
{
protected:
	int m_MajorVersion;
	int m_MinorVersion;
public:
	CAveInstRequiredInstallerVersion();
	virtual ~CAveInstRequiredInstallerVersion();

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& err);

	virtual HRESULT LoadFromNode(XMLNode node);

};

#endif // !defined(AFX_AVEINSTREQUIREDINSTALLERVERSION_H__0FDE98AB_E39B_4BD1_8662_3A6ABE67ACDC__INCLUDED_)
