// INI_API_Interceptor.h: interface for the CINI_API_Interceptor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INI_API_INTERCEPTOR_H__D9587046_E808_42D6_B813_AFED9AC76284__INCLUDED_)
#define AFX_INI_API_INTERCEPTOR_H__D9587046_E808_42D6_B813_AFED9AC76284__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IIniInterceptor
{
public: 
	virtual bool WritePrivateProfileStringW(const WCHAR* appName, const WCHAR* keyName, const WCHAR* str, const WCHAR* fileName) = 0;
	virtual bool GetPrivateProfileStringW(const WCHAR* appName, const WCHAR* keyName, const WCHAR* defVal,WCHAR* str, DWORD size, const WCHAR* fileName) = 0;
	virtual bool GetPrivateProfileIntW(int& retVal, const WCHAR* appName, const WCHAR* keyName, INT defVal, const WCHAR* fileName) = 0;
};

class CINI_API_Interceptor  
{
private:
	static CINI_API_Interceptor* s_Instance;
	static IIniInterceptor* s_Interceptor;

	ULONG m_RefCount;

	CINI_API_Interceptor();
	~CINI_API_Interceptor();

public:
	CINI_API_Interceptor* GetInstance(IIniInterceptor* interceptor);

	ULONG AddRef();
	ULONG Release();
}; 

#endif // !defined(AFX_INI_API_INTERCEPTOR_H__D9587046_E808_42D6_B813_AFED9AC76284__INCLUDED_)
