// CloseButtonMouseOverPolicy.h: interface for the CCloseButtonMouseOverPolicy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLOSEBUTTONMOUSEOVERPOLICY_H__B4689107_C435_4212_8F9A_93BA2BA3A02F__INCLUDED_)
#define AFX_CLOSEBUTTONMOUSEOVERPOLICY_H__B4689107_C435_4212_8F9A_93BA2BA3A02F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CloseButtonPolicy.h"

class CCloseButtonMouseOverPolicy : public CCloseButtonPolicy  
{
public:
	virtual bool shouldShow(Docklet* d);
	virtual bool shouldHide(Docklet* d);

	CCloseButtonMouseOverPolicy();
	virtual ~CCloseButtonMouseOverPolicy();

};

#endif // !defined(AFX_CLOSEBUTTONMOUSEOVERPOLICY_H__B4689107_C435_4212_8F9A_93BA2BA3A02F__INCLUDED_)
