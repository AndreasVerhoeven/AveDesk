// AveInstRequiredToAcceptText.h: interface for the CAveInstRequiredToAcceptText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREDTOACCEPTTEXT_H__4A4FF78D_43D2_4CE0_BE87_9AAB4E117BB3__INCLUDED_)
#define AFX_AVEINSTREQUIREDTOACCEPTTEXT_H__4A4FF78D_43D2_4CE0_BE87_9AAB4E117BB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstReqItem.h"

#include <string>

class CAveInstRequiredToAcceptText : public CAveInstReqItem  
{
protected:
	std::wstring m_Text;
public:
	CAveInstRequiredToAcceptText();
	virtual ~CAveInstRequiredToAcceptText();

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error);
	virtual HRESULT LoadFromNode(XMLNode node);
};

#endif // !defined(AFX_AVEINSTREQUIREDTOACCEPTTEXT_H__4A4FF78D_43D2_4CE0_BE87_9AAB4E117BB3__INCLUDED_)
