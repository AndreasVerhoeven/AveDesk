// FolderBehaviour.h: interface for the CFolderBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOLDERBEHAVIOUR_H__AB193C72_4B34_4992_9680_396DA9151CC9__INCLUDED_)
#define AFX_FOLDERBEHAVIOUR_H__AB193C72_4B34_4992_9680_396DA9151CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShellItemBehaviour.h"

class CFolderBehaviour;

typedef struct _UpdateParams
{
public:

	bool countFileSize;
	bool recursive;
	std::wstring path;
	CFolderBehaviour* obj;

} UpdateParams, *LPUpdateParams;

class CFolderBehaviour : public CShellItemBehaviour  
{
private:
	bool m_IsDirty;

	ULARGE_INTEGER	m_NumOfFiles;
	ULARGE_INTEGER	m_NumOfFolders;
	ULARGE_INTEGER	m_TotalFileSize;
	bool			m_WasRecursive;
	bool			m_WasCountSize;
	bool			m_TempUpdate;

	enum DropType { DropMove, DropCopy, DropNone };

	DropType m_DropType;
	bool m_DropOnSameDrive;

	IShortcutDesklet* m_Desklet;

	bool m_Stop;
	HANDLE m_ThreadHandle;

	void UpdateItems(bool countFileSize, bool recursive, std::wstring path);

	void BeginUpdating(bool countFileSize, bool recursive, std::wstring path);
	void EndUpdating();

	static DWORD __stdcall UpdateThread(LPVOID lpData);
public:
	CFolderBehaviour(IShortcutDesklet* desklet);
	virtual ~CFolderBehaviour();


	virtual DWORD OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter);

	static bool CanHandlePidl(CPidl& pidl, std::wstring fileName);
	virtual std::wstring ParseSubLabel(CPidl& pidl,const std::wstring& format);

	virtual std::vector<std::wstring> GetCustomSublabelItems();

	virtual std::wstring GetCustomKeyName();

	virtual std::map<std::wstring,std::wstring> GetMapOfImageNames();

	virtual BOOL DragEnter(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect);
	virtual BOOL DragOver(DWORD keys, POINTL pnt, DWORD *effect);
	virtual BOOL Drop(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect);
	virtual BOOL DragLeave();
};

#endif // !defined(AFX_FOLDERBEHAVIOUR_H__AB193C72_4B34_4992_9680_396DA9151CC9__INCLUDED_)
