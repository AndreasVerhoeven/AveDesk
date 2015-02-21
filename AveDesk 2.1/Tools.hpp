// perhaps merge this class into AveDeskGlobalSettings
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_HPP__1251FC4F_7D64_4184_A83F_2C37D8CBAC02__INCLUDED_)
#define AFX_TOOLS_HPP__1251FC4F_7D64_4184_A83F_2C37D8CBAC02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
class CTool
{
public:
	void Call(CString & newArgs);
	void Call();
    CString Display, Execute, Arguments, Location;
    static HWND m_hWnd;
};

class CTools  
{
private:
    CString ini;

public:
    std::vector<CTool> tools;
	void ReadData();
	CTools();
	virtual ~CTools();
    CString getIni(){return ini;}
    void SaveAll();
};

extern CTools _Tools;
#endif // !defined(AFX_TOOLS_HPP__1251FC4F_7D64_4184_A83F_2C37D8CBAC02__INCLUDED_)
