// AveEnvironment.h: interface for the CAveEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEENVIRONMENT_H__6FAF2A02_18B9_4F40_BBB2_C7A079277E59__INCLUDED_)
#define AFX_AVEENVIRONMENT_H__6FAF2A02_18B9_4F40_BBB2_C7A079277E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <string>

class CAveEnvironment  
{
public:
	CAveEnvironment();
	virtual ~CAveEnvironment();

	int GetMajorVersion();
	int GetMinorVersion();

	virtual std::wstring GetInstallPath();
	virtual std::wstring GetDeskletsPath();
	virtual std::wstring GetDockletsPath();
	virtual std::wstring GetEffectsPath();
	virtual std::list<std::wstring> GetInstalledModules();
	virtual std::wstring GetLabelsPath();
	virtual std::wstring GetLanguagesPath();
	virtual std::wstring GetThemesPath();
	virtual std::wstring GetUserPath();

};

#endif // !defined(AFX_AVEENVIRONMENT_H__6FAF2A02_18B9_4F40_BBB2_C7A079277E59__INCLUDED_)
