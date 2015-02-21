// IAveResources.h: interface for the IAveResources class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVERESOURCES_H__C6DCEE4A_73D4_4A52_BCB8_5DC2B49BB376__INCLUDED_)
#define AFX_IAVERESOURCES_H__C6DCEE4A_73D4_4A52_BCB8_5DC2B49BB376__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "controls_header.h"

class IAveResources  
{
protected:
	std::wstring dirName;

	static std::wstring StringToWString(const std::string& str);
	static std::string WStringToString(const std::wstring& str);

public:
	IAveResources(const char* dirName);
	IAveResources(const WCHAR* dirName);

	virtual ~IAveResources();

	virtual Bitmap* GetBitmap(const char* resName);
	virtual std::string GetFilename(const char* resName);
	virtual std::string GetDirName()const;

	virtual Bitmap* GetBitmapW(const WCHAR* resName);
	virtual std::wstring GetFilenameW(const WCHAR* resName);
	virtual std::wstring GetDirNameW()const;

	virtual void SetPath(const char* path);
	virtual void SetPathW(const WCHAR* path);

};

#endif // !defined(AFX_IAVERESOURCES_H__C6DCEE4A_73D4_4A52_BCB8_5DC2B49BB376__INCLUDED_)
