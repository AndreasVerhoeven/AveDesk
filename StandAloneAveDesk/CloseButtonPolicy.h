// CloseButtonPolicy.h: interface for the CCloseButtonPolicy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLOSEBUTTONPOLICY_H__BFA0A3BB_C41D_402D_AA95_33691D2FEE85__INCLUDED_)
#define AFX_CLOSEBUTTONPOLICY_H__BFA0A3BB_C41D_402D_AA95_33691D2FEE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Docklet;

class CCloseButtonPolicy  
{
public:
	virtual bool shouldShow(Docklet* d) = 0;
	virtual bool shouldHide(Docklet* d) = 0;
	virtual ~CCloseButtonPolicy(){};

};

#endif // !defined(AFX_CLOSEBUTTONPOLICY_H__BFA0A3BB_C41D_402D_AA95_33691D2FEE85__INCLUDED_)
