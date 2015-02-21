// NetworkConnectionBehaviour.h: interface for the CNetworkConnectionBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKCONNECTIONBEHAVIOUR_H__8472C181_32C8_4156_9AAE_C6718C079156__INCLUDED_)
#define AFX_NETWORKCONNECTIONBEHAVIOUR_H__8472C181_32C8_4156_9AAE_C6718C079156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShellItemBehaviour.h"

class CNetworkConnectionBehaviour  : public CShellItemBehaviour
{
public:
	CNetworkConnectionBehaviour(IShortcutDesklet* desklet);

	virtual ~CNetworkConnectionBehaviour();
	virtual DWORD OnMonitorMessage(CPidl& pidl,LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter);
	virtual std::wstring ParseSubLabel(CPidl &pidl, const std::wstring &format);

	static bool CanHandlePidl(CPidl& pidl, std::wstring);

};

#endif // !defined(AFX_NETWORKCONNECTIONBEHAVIOUR_H__8472C181_32C8_4156_9AAE_C6718C079156__INCLUDED_)
