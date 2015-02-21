// Module.cpp: implementation of the CModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Module.h"
#include "helper_funcs.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModule::CModule(const std::wstring& fileName) : 
	m_hMod(NULL),
	m_Pointer(NULL),
	m_Version(0)
{
	typedef void* (__stdcall *HOnCreate)();
	typedef void (__stdcall *HOnGetInformation)(char* Name, char* Author, int* version, char* Description);

	std::wstring fname(fileName);

	if(!PathFileExistsW(fname.c_str()))
	{
		WCHAR path[MAX_PATH + 2] = {0};
		wcscpy(path, _App->GetAveDeskDataDir().c_str());
		PathAppendW(path, fname.c_str());
		fname = path;
	}

	m_hMod = LoadLibrary(fname.c_str());
	if(m_hMod)
	{
		
		WCHAR path[MAX_PATH] = {0};
		GetModuleFileName(m_hMod,path,MAX_PATH);
		m_Path = path;

		HOnCreate OnCreate = (HOnCreate) GetProcAddress(m_hMod,"OnCreate");
		if(OnCreate)
			m_Pointer = OnCreate();

		HOnGetInformation OnGetInformation = (HOnGetInformation) GetProcAddress(m_hMod,"OnGetInformation");
		if(OnGetInformation)
		{
			char name[1024]		= {0};
			char author[1024]	= {0};
			char descr[10024]	= {0};
            OnGetInformation(name,author,&m_Version,descr);

			m_Name = StringToWString(name).c_str();
			m_Author = StringToWString(author).c_str();
			m_Description = StringToWString(descr).c_str();;
		}
		else
		{
			WCHAR filename[MAX_PATH] = {0};
			GetFileNameW(path,filename,MAX_PATH);
			//PathRemoveFileSpec(path);
			m_Name = filename;
		}
	}
}

CModule::~CModule()
{
	typedef void (__stdcall *HOnDestroy)(void*);

	if(m_hMod)
	{
		HOnDestroy OnDestroy = (HOnDestroy) GetProcAddress(m_hMod,"OnDestroy");
		if(OnDestroy)
			OnDestroy(m_Pointer);

		FreeLibrary(m_hMod);
	}
}
