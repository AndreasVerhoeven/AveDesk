// ThemePackager.h: interface for the CThemePackager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THEMEPACKAGER_H__E51B93DC_68A8_44EC_92A0_FEEFE6D4BD7D__INCLUDED_)
#define AFX_THEMEPACKAGER_H__E51B93DC_68A8_44EC_92A0_FEEFE6D4BD7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Docklet.h"

class CThemePackager  
{
private:
	Docklet::DockletsMap& m_Docklets;
	CRITICAL_SECTION*     m_CriticalSection;

public:
	CThemePackager(Docklet::DockletsMap& docklets, CRITICAL_SECTION* cs);

//	virtual HRESULT Package(bool createPreviewImage);

	virtual ~CThemePackager();

};

#endif // !defined(AFX_THEMEPACKAGER_H__E51B93DC_68A8_44EC_92A0_FEEFE6D4BD7D__INCLUDED_)
