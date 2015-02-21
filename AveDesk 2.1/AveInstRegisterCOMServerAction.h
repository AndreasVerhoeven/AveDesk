// AveInstRegisterCOMServerAction.h: interface for the CAveInstRegisterCOMServerAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREGISTERCOMSERVERACTION_H__455C3E73_900C_4FE8_9C66_EBCC65A9EA51__INCLUDED_)
#define AFX_AVEINSTREGISTERCOMSERVERACTION_H__455C3E73_900C_4FE8_9C66_EBCC65A9EA51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"

#include <string>

class CAveInstRegisterCOMServerAction : public CAveInstAction  
{
public:
	bool m_UseAlias;
	std::wstring m_FileName;
public:
	CAveInstRegisterCOMServerAction();
	virtual ~CAveInstRegisterCOMServerAction();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env); 
};

#endif // !defined(AFX_AVEINSTREGISTERCOMSERVERACTION_H__455C3E73_900C_4FE8_9C66_EBCC65A9EA51__INCLUDED_)
