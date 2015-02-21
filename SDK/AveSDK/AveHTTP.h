//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

// AveHTTP.h: interface for the CAveHTTP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEHTTP_H__885D9DF4_CB0F_416C_81E6_34E28F9E1534__INCLUDED_)
#define AFX_AVEHTTP_H__885D9DF4_CB0F_416C_81E6_34E28F9E1534__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <string>
#include <wininet.h>
#pragma comment(lib,"wininet.lib")

class CAveHTTP  
{
protected:
	HINTERNET hInet;
public:
	CAveHTTP(const std::wstring& appName);
	virtual ~CAveHTTP();

	BOOL RetrievePage(const std::wstring& url, std::wstring& out);
	static std::wstring CAveHTTP::Encode(const std::wstring& str);

};

#endif // !defined(AFX_AVEHTTP_H__885D9DF4_CB0F_416C_81E6_34E28F9E1534__INCLUDED_)
