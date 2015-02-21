// RecycleBinBehaviour.h: interface for the CRecycleBinBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECYCLEBINBEHAVIOUR_H__CDAB26FD_ADE8_4D26_8616_76004746AECC__INCLUDED_)
#define AFX_RECYCLEBINBEHAVIOUR_H__CDAB26FD_ADE8_4D26_8616_76004746AECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShellItemBehaviour.h"

#include "IShortcutDesklet.h"

class CRecycleBinBehaviour : public CShellItemBehaviour  
{
private:
	ULARGE_INTEGER m_NumItems;
	ULARGE_INTEGER m_SizeInBytes;

	bool m_IsInit;

	IShortcutDesklet* m_Desklet;

	UINT m_ShellIdListClipFormat;
	UINT m_AveDeskDeskletFormat;

	bool m_CanDrop;
	bool m_DropIsDrive;
	bool m_InDeleteMode;

public:
	CRecycleBinBehaviour(IShortcutDesklet* desklet);
	virtual ~CRecycleBinBehaviour();

	void UpdateItems();

	static bool CanHandlePidl(CPidl& pidl, std::wstring fileName);
	virtual std::wstring ParseSubLabel(CPidl& pidl,const std::wstring& format);
	virtual DWORD OnMonitorMessage(CPidl& pidl,LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter);

	virtual std::vector<std::wstring> GetCustomSublabelItems();

	virtual HWND CreateIconWindow(HINSTANCE hInstance, HWND hwndParent) {return NULL;};

	virtual std::wstring GetCustomKeyName();

	virtual std::map<std::wstring,std::wstring> GetMapOfImageNames();

	virtual BOOL DragEnter(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect); 
	virtual BOOL DragOver(DWORD keys, POINTL pnt, DWORD *effect); 
	virtual BOOL Drop(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect); 
	virtual BOOL DragLeave(); 

	virtual BOOL GetSpecialDropEffect(DWORD& effect);
};

#endif // !defined(AFX_RECYCLEBINBEHAVIOUR_H__CDAB26FD_ADE8_4D26_8616_76004746AECC__INCLUDED_)
