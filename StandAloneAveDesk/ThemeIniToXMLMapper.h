// ThemeIniToXMLMapper.h: interface for the CThemeIniToXMLMapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THEMEINITOXMLMAPPER_H__2EFED6CC_C24B_4831_9797_CF0B4A9058D3__INCLUDED_)
#define AFX_THEMEINITOXMLMAPPER_H__2EFED6CC_C24B_4831_9797_CF0B4A9058D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPersistable.h"

class CThemeIniToXMLMapper  
{
public:
	CThemeIniToXMLMapper();
	virtual ~CThemeIniToXMLMapper();

	void MapSettings(const WCHAR* ini, const WCHAR* xml);
	void ReadDeskletAndMapItToXML(ISaver& saver, const char* ini, const char* section);
	void ReadDefaultDeskletAndMapItToXML(ISaver& saver, const char* ini, const char* section);
	void ReadLabelAndMapItToXML(ISaver& saver, const char* ini, const char* section);
	void MapOldThemeToNewTheme(const WCHAR* ini, const WCHAR* theme);
};

#endif // !defined(AFX_THEMEINITOXMLMAPPER_H__2EFED6CC_C24B_4831_9797_CF0B4A9058D3__INCLUDED_)
