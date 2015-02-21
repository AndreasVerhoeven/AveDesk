// ShellItemBehaviour.h: interface for the CShellItemBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLITEMBEHAVIOUR_H__D29E6897_3C0D_4BE8_BAD1_8C3FFC8FC958__INCLUDED_)
#define AFX_SHELLITEMBEHAVIOUR_H__D29E6897_3C0D_4BE8_BAD1_8C3FFC8FC958__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Behaviour.h"


class CShellItemBehaviour : public CBehaviour  
{
protected:
	IShortcutDesklet* m_Desklet;
public:
	virtual std::wstring ParseSubLabel(CPidl &pidl, const std::wstring &format);
	static bool CanHandlePidl(CPidl &pidl, std::wstring fileName);
	virtual DWORD OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter);

	virtual std::vector<std::wstring> GetCustomSublabelItems();

	CShellItemBehaviour(IShortcutDesklet* desklet);
	virtual ~CShellItemBehaviour();

	IShortcutDesklet* GetShortcutDesklet()const { return m_Desklet;};

	virtual std::wstring GetCustomKeyName();

	bool ParseTokenAt(CPidl &pidl, const std::wstring &format, std::wstringstream& output, int& index);

	virtual std::map<std::wstring,std::wstring> GetMapOfImageNames();
};

#endif // !defined(AFX_SHELLITEMBEHAVIOUR_H__D29E6897_3C0D_4BE8_BAD1_8C3FFC8FC958__INCLUDED_)
