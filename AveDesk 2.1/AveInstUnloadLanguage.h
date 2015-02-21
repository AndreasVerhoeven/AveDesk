// AveInstUnloadLanguage.h: interface for the CAveInstUnloadLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTUNLOADLANGUAGE_H__A5BC7ABA_011B_4421_BE82_1C106C2E0FA8__INCLUDED_)
#define AFX_AVEINSTUNLOADLANGUAGE_H__A5BC7ABA_011B_4421_BE82_1C106C2E0FA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"

class CAveInstUnloadLanguage : public CAveInstAction  
{
public:
	CAveInstUnloadLanguage();
	virtual ~CAveInstUnloadLanguage();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);

};

#endif // !defined(AFX_AVEINSTUNLOADLANGUAGE_H__A5BC7ABA_011B_4421_BE82_1C106C2E0FA8__INCLUDED_)
