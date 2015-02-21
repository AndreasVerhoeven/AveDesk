// AveInstRunScriptAction.h: interface for the CAveInstRunScriptAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTRUNSCRIPTACTION_H__80A685ED_A919_4F9A_A432_A8397E5FBC3B__INCLUDED_)
#define AFX_AVEINSTRUNSCRIPTACTION_H__80A685ED_A919_4F9A_A432_A8397E5FBC3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"

#include <string>

class CAveInstRunScriptAction : public CAveInstAction  
{
protected:
	std::wstring m_ScriptFileName;

public:
	CAveInstRunScriptAction();
	virtual ~CAveInstRunScriptAction();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);

};

#endif // !defined(AFX_AVEINSTRUNSCRIPTACTION_H__80A685ED_A919_4F9A_A432_A8397E5FBC3B__INCLUDED_)
