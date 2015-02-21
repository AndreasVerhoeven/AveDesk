// AveInstShowTextAction.h: interface for the CAveInstShowTextAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTSHOWTEXTACTION_H__85A45B76_AB51_4156_B663_DBFB9D1137BF__INCLUDED_)
#define AFX_AVEINSTSHOWTEXTACTION_H__85A45B76_AB51_4156_B663_DBFB9D1137BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"
#include <string>

class CAveInstShowTextAction : public CAveInstAction  
{
protected:
	std::wstring m_Caption;
	std::wstring m_Text;

	std::wstring ReadNodeText(XMLNode& node, const std::wstring& defaultText, const std::wstring& name);
public:
	CAveInstShowTextAction();
	virtual ~CAveInstShowTextAction();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);

};

#endif // !defined(AFX_AVEINSTSHOWTEXTACTION_H__85A45B76_AB51_4156_B663_DBFB9D1137BF__INCLUDED_)
