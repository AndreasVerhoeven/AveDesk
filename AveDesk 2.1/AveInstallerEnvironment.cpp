// AveInstallerEnvironment.cpp: implementation of the CAveInstallerEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstallerEnvironment.h"
#include "AveInstPackageManager.h"
#include "helper_funcs.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstallerEnvironment::CAveInstallerEnvironment() :
	m_hWnd(NULL),
	m_MajorVersion(0),
	m_MinorVersion(0),
	m_ProgressIndicator(NULL),
	m_DependenciesList(NULL)
{

}

CAveInstallerEnvironment::~CAveInstallerEnvironment()
{

}

void CAveInstallerEnvironment::ReportError(const std::wstring& message)
{
	::MessageBoxW(m_hWnd, message.c_str(), NULL, MB_ICONERROR);
}

void CAveInstallerEnvironment::ReportErrorByID(DWORD id)
{
	CString str;
	str.LoadString(id);
	::MessageBox(m_hWnd, str, NULL, MB_ICONERROR);
}

bool CAveInstallerEnvironment::IsPackageInstalled(const std::wstring& guid)
{
	return CAveInstPackageManager::IsPackageInstalled(guid);
}

void CAveInstallerEnvironment::SetDependenciesList(std::vector<std::wstring>* v)
{
	m_DependenciesList = v;	
}

void CAveInstallerEnvironment::AddDependency(const std::wstring& val)
{
	if(m_DependenciesList != NULL)
	{
		if(PathFileExists(val.c_str()) && val.size() < MAX_PATH)
		{
			WCHAR path[MAX_PATH] = {0};
			wcscpy(path,val.c_str());
			WCHAR adPath[MAX_PATH] = {0};
			wcscpy(adPath, _App->GetAveDeskDataDir().c_str());
			WCHAR* relPath = wcsstr(path,adPath);
			if(relPath != NULL)
				relPath += wcslen(adPath) + 1;
			else
				relPath = path;

			m_DependenciesList->push_back(relPath);
		}
		else
			m_DependenciesList->push_back(val);
	}
}

std::wstring CAveInstallerEnvironment::GetTempPath()
{
	return m_TempPath;
}

int CAveInstallerEnvironment::GetMajorVersion()
{
	return m_MajorVersion;
}

int CAveInstallerEnvironment::GetMinorVersion()
{
	return m_MinorVersion;
}

void CAveInstallerEnvironment::SetVersion(int major, int minor)
{
	m_MajorVersion = major;
	m_MinorVersion = minor;
}

void CAveInstallerEnvironment::SetTempPath(const std::wstring& path)
{
	m_TempPath = path;
}

CAveEnvironment CAveInstallerEnvironment::GetAveDeskEnvironment()
{
	return m_Environment;
}

void CAveInstallerEnvironment::SetFileAlias(const std::wstring& alias, const std::wstring name)
{
	m_FileAliases[alias] = name;
}

std::wstring CAveInstallerEnvironment::GetFileAlias(const std::wstring &alias)
{
	return m_FileAliases[alias];
}

void CAveInstallerEnvironment::SetHWND(HWND hwnd)
{
	m_hWnd = hwnd;
}

HWND CAveInstallerEnvironment::GetHWND()
{
	return m_hWnd;
}

bool CAveInstallerEnvironment::IsSilentInstall()const
{
	return m_IsSilentInstall;
}

void CAveInstallerEnvironment::SetIsSilentIstall(bool b)
{
	m_IsSilentInstall = b;
}

CAveInstProgressIndicator* CAveInstallerEnvironment::GetInstallerProgressIndicator()
{
	return m_ProgressIndicator;
}

void CAveInstallerEnvironment::SetProgressIndicator(CAveInstProgressIndicator* ind)
{
	m_ProgressIndicator = ind;
}

CAveInstSuspendedDeskletsList* CAveInstallerEnvironment::GetSuspendedDeskletsList()
{
	return m_SuspendedDeskletsList;
}

void CAveInstallerEnvironment::SetSuspendedDeskletsList(CAveInstSuspendedDeskletsList* sdl)
{
	m_SuspendedDeskletsList = sdl;
}