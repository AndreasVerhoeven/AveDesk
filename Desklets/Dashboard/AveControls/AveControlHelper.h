// AveControlHelper.h: interface for the CAveControlHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVECONTROLHELPER_H__FAC9A572_6CA5_4BB6_8BF8_550BE487561C__INCLUDED_)
#define AFX_AVECONTROLHELPER_H__FAC9A572_6CA5_4BB6_8BF8_550BE487561C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <string>

struct AVE_SHADOWPROPERTIES;
class CShadowProperties;

class CAveControlHelper  
{
public:
	CAveControlHelper();
	virtual ~CAveControlHelper();

	static void ConvertToCShadowProperties(const AVE_SHADOWPROPERTIES* in, CShadowProperties& out);
	static void ConvertToShadowProperties(const CShadowProperties& in, AVE_SHADOWPROPERTIES* out);

	static size_t CopyWStringToWCHARPtr(const std::wstring& ws, WCHAR* output, size_t max);
	static size_t CopyStringToCHARPtr(const std::string& ws, char* output, size_t max);

};

#endif // !defined(AFX_AVECONTROLHELPER_H__FAC9A572_6CA5_4BB6_8BF8_550BE487561C__INCLUDED_)
