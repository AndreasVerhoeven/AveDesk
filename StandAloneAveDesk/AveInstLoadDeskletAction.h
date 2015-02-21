// AveInstLoadDeskletAction.h: interface for the CAveInstLoadDeskletAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTLOADDESKLETACTION_H__3DCF8378_0D6B_4377_86E2_3A0652FAD8E1__INCLUDED_)
#define AFX_AVEINSTLOADDESKLETACTION_H__3DCF8378_0D6B_4377_86E2_3A0652FAD8E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"

class CAveInstLoadDeskletAction : public CAveInstAction  
{
protected:
	XMLNode m_Node;
public:
	CAveInstLoadDeskletAction();
	virtual ~CAveInstLoadDeskletAction();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTLOADDESKLETACTION_H__3DCF8378_0D6B_4377_86E2_3A0652FAD8E1__INCLUDED_)
