// Behaviour.h: interface for the CBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEHAVIOUR_H__E3AA8A66_5A65_428E_9156_1820B202708C__INCLUDED_)
#define AFX_BEHAVIOUR_H__E3AA8A66_5A65_428E_9156_1820B202708C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pidl.h"

#include "IShortcutDesklet.h"

#define UPDATE_SUBLABEL		2
#define UPDATE_ICON			4

#include <map>
#include <vector>
#include <string>
#include "pidl.h"

#define PRIVATE_DROPEFFECT_REMOVE	16
#define PRIVATE_DROPEFFECT_DELETE	32

class CBehaviour  
{
public:
	CBehaviour();
	virtual ~CBehaviour();

	virtual DWORD OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter) = 0;

	//static bool CanHandlePidl(const LPITEMIDLIST& pidl);
	virtual std::wstring ParseSubLabel(CPidl& pidl,const std::wstring& format) = 0;

	virtual std::vector<std::wstring> GetCustomSublabelItems() = 0;

	virtual std::wstring GetCustomKeyName() = 0;

	virtual std::map<std::wstring,std::wstring> GetMapOfImageNames() = 0;

	virtual BOOL DragEnter(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect) {return FALSE;};
	virtual BOOL DragOver(DWORD keys, POINTL pnt, DWORD *effect) { return FALSE;};
	virtual BOOL Drop(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect) { return FALSE; };
	virtual BOOL DragLeave() {return FALSE;};

	virtual BOOL GetSpecialDropEffect(DWORD& effect){return FALSE;};
};

#endif // !defined(AFX_BEHAVIOUR_H__E3AA8A66_5A65_428E_9156_1820B202708C__INCLUDED_)
