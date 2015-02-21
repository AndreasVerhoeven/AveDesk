// AveInstDoIfAction.h: interface for the CAveInstDoIfAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTDOIFACTION_H__180C6F7A_6ED3_4C0D_B2C8_B41CE0569714__INCLUDED_)
#define AFX_AVEINSTDOIFACTION_H__180C6F7A_6ED3_4C0D_B2C8_B41CE0569714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"

#include <memory>
#include "AveInstReqItem.h"

class CAveInstDoIfAction : public CAveInstAction  
{
protected:
	boost::shared_ptr<CAveInstReqItem> m_Requirement;
	boost::shared_ptr<CAveInstAction> m_If;
	boost::shared_ptr<CAveInstAction> m_Else;
public:
	CAveInstDoIfAction();
	virtual ~CAveInstDoIfAction();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);

};

#endif // !defined(AFX_AVEINSTDOIFACTION_H__180C6F7A_6ED3_4C0D_B2C8_B41CE0569714__INCLUDED_)
