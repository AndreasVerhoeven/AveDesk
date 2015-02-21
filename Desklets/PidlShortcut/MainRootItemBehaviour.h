// MainRootItemBehaviour.h: interface for the CMainRootItemBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINROOTITEMBEHAVIOUR_H__3AC11FED_C5DA_4BFB_B225_BF36E76A59D0__INCLUDED_)
#define AFX_MAINROOTITEMBEHAVIOUR_H__3AC11FED_C5DA_4BFB_B225_BF36E76A59D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShellItemBehaviour.h"

#include "IShortcutDesklet.h"

class CMainRootItemBehaviour : 
	public virtual CShellItemBehaviour  
{
protected:
	IShortcutDesklet* m_Desklet;
public:
	CMainRootItemBehaviour(IShortcutDesklet* desklet);
	virtual ~CMainRootItemBehaviour();

	virtual DWORD OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter);

	static bool CanHandlePidl(CPidl& pidl, std::wstring);
	virtual std::wstring ParseSubLabel(CPidl& pidl,const std::wstring& format);

	virtual std::vector<std::wstring> GetCustomSublabelItems() ;
};

#endif // !defined(AFX_MAINROOTITEMBEHAVIOUR_H__3AC11FED_C5DA_4BFB_B225_BF36E76A59D0__INCLUDED_)
