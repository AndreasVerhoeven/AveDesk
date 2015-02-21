// AveEnvironment.cpp: implementation of the CAveEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveEnvironment.h"
#include "helper_funcs.h"

#include "Application.h"
#include "test.h"
#include "Modules.h"
#include "Module.h"
#include <vector>


extern CComObject<CApplication> *_App;
extern CModules* _Modules;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAveEnvironment::CAveEnvironment()
{

}

CAveEnvironment::~CAveEnvironment()
{

}


int CAveEnvironment::GetMajorVersion()
{
	return 1;
}

int CAveEnvironment::GetMinorVersion()
{
	return 3;
}

std::wstring CAveEnvironment::GetInstallPath()
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());

	return path;
}

std::wstring CAveEnvironment::GetDeskletsPath()
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	PathAppendW(path,L"Desklets\\");

	return path;
}

std::wstring CAveEnvironment::GetLabelsPath()
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	PathAppendW(path,L"Labels\\");

	return path;
}	
std::wstring CAveEnvironment::GetLanguagesPath()
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	PathAppendW(path,L"Languages\\");

	return path;
}

std::wstring CAveEnvironment::GetDockletsPath()
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	PathAppendW(path,L"Docklets\\");

	return path;
}

std::wstring CAveEnvironment::GetEffectsPath()
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	PathAppendW(path,L"Effects\\");

	return path;
}

std::list<std::wstring> CAveEnvironment::GetInstalledModules()
{
	std::list<std::wstring> retList;
	std::vector<CModule*> modules = _Modules->GetCurrentModules();

	for(std::vector<CModule*>::iterator iter = modules.begin(); iter != modules.end(); ++iter)
	{
		CModule* m = *iter;
		WCHAR path[MAX_PATH] = {0};
		GetModuleFileName(m->GetHMODULE(),path,MAX_PATH);
		retList.push_back(path);
	}

	return retList;
}

std::wstring CAveEnvironment::GetThemesPath()
{
	return _App->GetAveDeskThemeDir();
}

std::wstring CAveEnvironment::GetUserPath()
{
	return _App->GetAveDeskUserPath();
}