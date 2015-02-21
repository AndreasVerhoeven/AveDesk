// Modules.cpp: implementation of the CModules class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Modules.h"
#include "Module.h"
#include <algorithm>
#include "helper_funcs.h"
#include "constants.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModules::CModules(DWORD threadID, HWND hwnd) :
	m_ThreadID(threadID),
	m_Hwnd(hwnd)
{

}

CModules::~CModules()
{
	ClearCurrentModules();
}

CModule* CModules::LoadModule(const std::wstring& fileName)
{
	if(GetCurrentThreadId() != m_ThreadID)
	{
		return reinterpret_cast<CModule*>(SendMessage(m_Hwnd,WM_AVELOADMODULE,0L,reinterpret_cast<LPARAM>(fileName.c_str())));
	}
	else
	{
		std::wstring loadFileName = fileName;
	
		WCHAR path[MAX_PATH+1] = {0};
		wcscpy(path,fileName.c_str());	
		CModule* m = new CModule(loadFileName);
		if(m->GetHMODULE())
		{
			m_Modules.push_back(m);
		}
		else
		{
			delete m;
			m = NULL;
		}

		return m;
	}
}


void CModules::ClearCurrentModules()
{
	if(GetCurrentThreadId() != m_ThreadID)
	{
		SendMessage(m_Hwnd,WM_AVECLEARMODULES,0L,0L);
	}
	else
	{
		std::vector<CModule*> modules = GetCurrentModules();
	
		for(std::vector<CModule*>::iterator iter = modules.begin(); iter != modules.end(); ++iter)
		{
			CModule* m = *iter;
			delete m;
		}
		modules.clear();
	}
}

void CModules::RemoveModule(int index)
{
	if(GetCurrentThreadId() != m_ThreadID)
	{
		SendMessage(m_Hwnd,WM_AVEREMOVEMODULE,0L,static_cast<LPARAM>(index));
	}
	else
	{
		if(index >= 0 && index < (int)m_Modules.size())
		{
			CModule* m = *(m_Modules.begin() + index);
			m_Modules.erase(m_Modules.begin() + index);
			delete m;
		}
	}
}


std::vector<CModule*> CModules::GetCurrentModules()
{
	return m_Modules;
}