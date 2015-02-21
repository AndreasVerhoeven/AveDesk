// AveInstNotRequired.h: interface for the CAveInstNotRequired class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTNOTREQUIRED_H__B1992D46_B51E_4682_8B16_95BFA0696C56__INCLUDED_)
#define AFX_AVEINSTNOTREQUIRED_H__B1992D46_B51E_4682_8B16_95BFA0696C56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>
#include "AveInstReqItem.h"

class CAveInstNotRequired : public CAveInstReqItem  
{
protected:
	boost::shared_ptr<CAveInstReqItem> m_NotItem;
public:
	CAveInstNotRequired();
	virtual ~CAveInstNotRequired();

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error);
	virtual HRESULT LoadFromNode(XMLNode node);
};

#endif // !defined(AFX_AVEINSTNOTREQUIRED_H__B1992D46_B51E_4682_8B16_95BFA0696C56__INCLUDED_)
