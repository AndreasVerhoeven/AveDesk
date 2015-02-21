// INI_API_Interceptor.cpp: implementation of the CINI_API_Interceptor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INI_API_Interceptor.h"

#include "detours.h"
#pragma comment(lib,"detours.lib")
/*
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DETOUR_TRAMPOLINE( BOOL WritePrivateProfileStringTrampoline(

    LPCTSTR lpAppName,	// pointer to section name 
    LPCTSTR lpKeyName,	// pointer to key name 
    LPCTSTR lpString,	// pointer to string to add 
    LPCTSTR lpFileName 	// pointer to initialization filename 
   ),
	WritePrivateProfileString
)

BOOL WritePrivateProfileStringDetour(

    LPCTSTR lpAppName,	// pointer to section name 
    LPCTSTR lpKeyName,	// pointer to key name 
    LPCTSTR lpString,	// pointer to string to add 
    LPCTSTR lpFileName 	// pointer to initialization filename 
   )
{
	if(_stricmp(lpFileName,"DETOUR")==0)
	{
		MessageBox(NULL,lpString,lpKeyName,0);
	}
	else
	{
		return WritePrivateProfileStringTrampoline(lpAppName,lpKeyName,lpString,lpFileName);
	}

	return TRUE;
}*/

CINI_API_Interceptor* CINI_API_Interceptor::s_Instance = NULL;
IIniInterceptor* CINI_API_Interceptor::s_Interceptor = NULL;

CINI_API_Interceptor::CINI_API_Interceptor() :
	m_RefCount(0)
{
	//DetourFunctionWithTrampoline((PBYTE)WritePrivateProfileStringTrampoline,(PBYTE)WritePrivateProfileStringDetour);
}

CINI_API_Interceptor::~CINI_API_Interceptor()
{
	//DetourRemove((PBYTE)WritePrivateProfileStringTrampoline,(PBYTE)WritePrivateProfileStringDetour);

	s_Instance = NULL;
	s_Interceptor = NULL;
}

CINI_API_Interceptor* CINI_API_Interceptor::GetInstance(IIniInterceptor* interceptor)
{
	s_Interceptor = interceptor;

	if(!s_Instance)
		s_Instance = new CINI_API_Interceptor;

	s_Instance->AddRef();

	return s_Instance;
}

ULONG CINI_API_Interceptor::AddRef()
{
	m_RefCount++;
	return m_RefCount;
}

ULONG CINI_API_Interceptor::Release()
{
	m_RefCount--;
	if(m_RefCount==0)
	{
		delete this;
		s_Instance = NULL;
		return 0;
	}
	else
	{
		return m_RefCount;
	}
}
