// Module.h: interface for the CModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULE_H__C7545D6D_C90F_4101_97B3_6C71FF463F5E__INCLUDED_)
#define AFX_MODULE_H__C7545D6D_C90F_4101_97B3_6C71FF463F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <string>

class CModule  
{
protected:
	HMODULE m_hMod;
	void* m_Pointer;
	CString m_Name, m_Author, m_Description;
	CString m_Path;
	int m_Version;
public:
	CModule(const std::wstring& fileName);
	virtual ~CModule();

	HMODULE GetHMODULE()const { return m_hMod;};
	CString GetName()const{ return m_Name;};
	CString GetAuthor()const{return m_Author;};
	CString GetDescription()const { return m_Description;};
	int     GetVersion()const{return m_Version;};
	void* GetPointer(){return m_Pointer;};
	CString GetFilePath()const { return m_Path;};

};

#endif // !defined(AFX_MODULE_H__C7545D6D_C90F_4101_97B3_6C71FF463F5E__INCLUDED_)
