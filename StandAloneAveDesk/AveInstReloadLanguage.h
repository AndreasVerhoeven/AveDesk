// AveInstReloadLanguage.h: interface for the CAveInstReloadLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTRELOADLANGUAGE_H__82BC5045_D677_4572_ACA6_5B51DD0C0BA6__INCLUDED_)
#define AFX_AVEINSTRELOADLANGUAGE_H__82BC5045_D677_4572_ACA6_5B51DD0C0BA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"

class CAveInstReloadLanguage : public CAveInstAction  
{
public:
	CAveInstReloadLanguage();
	virtual ~CAveInstReloadLanguage();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTRELOADLANGUAGE_H__82BC5045_D677_4572_ACA6_5B51DD0C0BA6__INCLUDED_)
