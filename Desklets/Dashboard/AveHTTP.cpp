//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

// AveHTTP.cpp: implementation of the CAveHTTP class.
//
//////////////////////////////////////////////////////////////////////

#include "AveHTTP.h"
#include "mem_buffer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveHTTP::CAveHTTP(const std::wstring& appName) :
	hInet(NULL)
{
	hInet = InternetOpenW(appName.c_str(), INTERNET_OPEN_TYPE_PRECONFIG,0,0,0);
}

CAveHTTP::~CAveHTTP()
{
	InternetCloseHandle(hInet);
	hInet = NULL;
}

BOOL CAveHTTP::RetrievePage(const std::wstring& url, std::wstring& out)
{
	if(!hInet)
		return FALSE;

	BOOL result = FALSE;

	HINTERNET hFile = InternetOpenUrlW(hInet,url.c_str(),
		L"Accept-Charset: utf-8, iso-8859-5, unicode-1-1;",
		-1,INTERNET_FLAG_RAW_DATA,0);

	if(hFile != NULL)
	{
		mem_buffer buffer(15000);
		DWORD dwNumRead(0);
		do
		{
			BYTE tmp[6400+1] = {1};
			dwNumRead=0;
			InternetReadFile(hFile,tmp,6400,&dwNumRead);
			if(dwNumRead > 0)
			{
				buffer.write(tmp, dwNumRead);
			}
		}while(dwNumRead > 0);

		BYTE zeroByte = '\0';
		buffer.write(&zeroByte, 1);
		DWORD bufLen = buffer.length() + 1;
		WCHAR* conversionBuffer = new WCHAR[bufLen];
		ZeroMemory(conversionBuffer, bufLen * sizeof(WCHAR));
		MultiByteToWideChar(CP_UTF8, 0, (const char*)buffer.get_buffer(), -1, conversionBuffer, bufLen);
		out = conversionBuffer;

		result = TRUE;

		InternetCloseHandle(hInet);
	}

	return result;
}

std::wstring CAveHTTP::Encode(const std::wstring& str)
{
	WCHAR* textUrl = new WCHAR[str.length()+1];
	DWORD bufLen = str.length() + 1;
	DWORD flags = 0;
	if(!InternetCombineUrlW(L"", str.c_str(), textUrl, &bufLen, flags))
	{
		delete[] textUrl;
		textUrl = new WCHAR[bufLen+1];
		textUrl[0] = L'\0';
		bufLen++;

		InternetCombineUrlW(L"", str.c_str(), textUrl, &bufLen, flags);

	}

	std::wstring ret(textUrl);
	delete[] textUrl;

	return ret;
}
