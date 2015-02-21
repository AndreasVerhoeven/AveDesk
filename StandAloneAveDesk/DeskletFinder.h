// DeskletFinder.h: interface for the CDeskletFinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETFINDER_H__58BA44BA_1D89_4DD1_A767_97407E80464D__INCLUDED_)
#define AFX_DESKLETFINDER_H__58BA44BA_1D89_4DD1_A767_97407E80464D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

#include "FoundDeskletEntry.h"

class DockletDLL;

class CDeskletFinder  
{
protected:
	HANDLE m_ThreadHandle;
	HWND m_CallbackHwnd;
	UINT m_Msg;
	BOOL m_Stop;
	std::wstring m_MainDirName;

	std::vector<CFoundDeskletEntry*> m_Data;

	static DWORD __stdcall ThisThreadFunc(LPVOID lpData);

	CDeskletFinder* m_Parent;

	CRITICAL_SECTION cs;

public:
	CDeskletFinder(const std::wstring& mainDirName);
	virtual ~CDeskletFinder();

	virtual void SetParent(CDeskletFinder* parent){m_Parent = parent;};

	virtual DWORD OnRun();
	virtual void CheckFolder(const std::wstring baseFolder);
	virtual void CheckSkins(DockletDLL* dll);

	virtual void Start(HWND callbackHwnd, UINT msg);
	virtual void Stop(DWORD timeOut);

	virtual CFoundDeskletEntry* GetEntry(UINT index);

	virtual void Lock();
	virtual void Unlock();
	virtual std::vector<CFoundDeskletEntry*>& GetData();

};

#endif // !defined(AFX_DESKLETFINDER_H__58BA44BA_1D89_4DD1_A767_97407E80464D__INCLUDED_)
