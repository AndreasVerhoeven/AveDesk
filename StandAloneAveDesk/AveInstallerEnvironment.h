// AveInstallerEnvironment.h: interface for the CAveInstallerEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTALLERENVIRONMENT_H__4CE6AB46_773B_445E_810D_15BE6675C2D5__INCLUDED_)
#define AFX_AVEINSTALLERENVIRONMENT_H__4CE6AB46_773B_445E_810D_15BE6675C2D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveEnvironment.h"
#include <string>
#include <map>
#include <vector>

class CAveInstaller;
class CAveInstProgressIndicator;
class CAveInstSuspendedDeskletsList;

class CAveInstallerEnvironment  
{
protected:
	std::map<std::wstring, std::wstring> m_FileAliases;

	CAveEnvironment m_Environment;
	std::wstring m_TempPath;

	HWND m_hWnd;

	int m_MajorVersion;
	int m_MinorVersion;

	bool m_IsSilentInstall;

	CAveInstProgressIndicator* m_ProgressIndicator;
	CAveInstSuspendedDeskletsList* m_SuspendedDeskletsList;

	std::vector<std::wstring>* m_DependenciesList;

public:
	CAveInstallerEnvironment();

	virtual void SetTempPath(const std::wstring& path);

	virtual ~CAveInstallerEnvironment();
	virtual std::wstring GetTempPath();

	virtual CAveEnvironment GetAveDeskEnvironment();

	virtual void SetFileAlias(const std::wstring& alias, const std::wstring name);
	virtual std::wstring GetFileAlias(const std::wstring &alias);

	virtual HWND GetHWND();
	virtual void SetHWND(HWND hwnd);

	virtual int GetMajorVersion();
	virtual int GetMinorVersion();

	virtual bool IsSilentInstall()const;
	virtual void SetIsSilentIstall(bool b);

	virtual void SetVersion(int major, int minor);

	virtual CAveInstProgressIndicator* GetInstallerProgressIndicator();
	virtual void SetProgressIndicator(CAveInstProgressIndicator* ind);

	virtual CAveInstSuspendedDeskletsList* GetSuspendedDeskletsList();
	virtual void SetSuspendedDeskletsList(CAveInstSuspendedDeskletsList* sdl);

	virtual void SetDependenciesList(std::vector<std::wstring>* v);
	virtual void AddDependency(const std::wstring& val);

	virtual bool IsPackageInstalled(const std::wstring& guid);

	virtual void ReportError(const std::wstring& message);
	virtual void ReportErrorByID(DWORD id);

};

#endif // !defined(AFX_AVEINSTALLERENVIRONMENT_H__4CE6AB46_773B_445E_810D_15BE6675C2D5__INCLUDED_)
