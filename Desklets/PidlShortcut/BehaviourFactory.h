// BehaviourFactory.h: interface for the CBehaviourFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEHAVIOURFACTORY_H__D57AD06D_189B_4097_805D_A5B0C21E7700__INCLUDED_)
#define AFX_BEHAVIOURFACTORY_H__D57AD06D_189B_4097_805D_A5B0C21E7700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>
#include "Behaviour.h"

#include "IShortcutDesklet.h"

class CPidl;

class CBehaviourFactory  
{
private:
	CBehaviourFactory();
	~CBehaviourFactory();

public:

	static std::auto_ptr<CBehaviour> CreateBehaviourForPidl(CPidl& pidl, std::wstring fileName, IShortcutDesklet* desklet);

	
};

#endif // !defined(AFX_BEHAVIOURFACTORY_H__D57AD06D_189B_4097_805D_A5B0C21E7700__INCLUDED_)
