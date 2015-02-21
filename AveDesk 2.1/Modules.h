// Modules.h: interface for the CModules class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULES_H__484D79A9_EBC5_47BE_B1A7_454EC3654DF9__INCLUDED_)
#define AFX_MODULES_H__484D79A9_EBC5_47BE_B1A7_454EC3654DF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class CModule;

class CModules  
{
private:
	std::vector<CModule*> m_Modules;
	DWORD m_ThreadID;
	HWND m_Hwnd;
public:
	CModules(DWORD threadID, HWND hwnd);
	~CModules();

	CModule* LoadModule(const std::wstring& fileName);
	void RemoveModule(int index);
	std::vector<CModule*> GetCurrentModules();
	void ClearCurrentModules();
};

#endif // !defined(AFX_MODULES_H__484D79A9_EBC5_47BE_B1A7_454EC3654DF9__INCLUDED_)
