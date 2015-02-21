// AveInstRequiredAveDeskVersion.h: interface for the CAveInstRequiredAveDeskVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREDAVEDESKVERSION_H__A7797164_E796_4157_B939_60A642FFAE3D__INCLUDED_)
#define AFX_AVEINSTREQUIREDAVEDESKVERSION_H__A7797164_E796_4157_B939_60A642FFAE3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstReqItem.h"

class CAveInstRequiredAveDeskVersion : public CAveInstReqItem  
{
protected:
	int m_MajorVersion;
	int m_MinorVersion;

public:
	CAveInstRequiredAveDeskVersion();
	virtual ~CAveInstRequiredAveDeskVersion();

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& err);

	virtual HRESULT LoadFromNode(XMLNode node);

};

#endif // !defined(AFX_AVEINSTREQUIREDAVEDESKVERSION_H__A7797164_E796_4157_B939_60A642FFAE3D__INCLUDED_)
