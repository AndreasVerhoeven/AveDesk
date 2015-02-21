// Unzip.h : main header file for the UNZIP application
//

#if !defined(AFX_UNZIP_H__4B4F37C1_F685_437A_8E94_982876E72653__INCLUDED_)
#define AFX_UNZIP_H__4B4F37C1_F685_437A_8E94_982876E72653__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnzipApp:
// See Unzip.cpp for the implementation of this class
//

class CUnzipApp : public CWinApp
{
public:
	CUnzipApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnzipApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUnzipApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNZIP_H__4B4F37C1_F685_437A_8E94_982876E72653__INCLUDED_)
