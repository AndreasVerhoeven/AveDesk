// DriveBehaviour.h: interface for the CDriveBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEBEHAVIOUR_H__0E1667A2_05EF_4774_BBA4_75C1C05F0455__INCLUDED_)
#define AFX_DRIVEBEHAVIOUR_H__0E1667A2_05EF_4774_BBA4_75C1C05F0455__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShellItemBehaviour.h"

class CDriveBehaviour : public CShellItemBehaviour  
{
protected:
	ULARGE_INTEGER m_FreeBytes;
	ULARGE_INTEGER m_TotalBytes;
	bool		   m_IsInit;
public:
	CDriveBehaviour(IShortcutDesklet* desklet);
	virtual ~CDriveBehaviour();

	virtual std::wstring ParseSubLabel(CPidl &pidl, const std::wstring &format);
	static bool CanHandlePidl(CPidl &pidl, std::wstring fileName);

	bool ParseTokenAt(CPidl &pidl, const std::wstring &format, std::wstringstream& output, int& index);
	virtual DWORD OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter);

	void GetDriveInfo();

	virtual std::vector<std::wstring> GetCustomSublabelItems();

};

#endif // !defined(AFX_DRIVEBEHAVIOUR_H__0E1667A2_05EF_4774_BBA4_75C1C05F0455__INCLUDED_)
