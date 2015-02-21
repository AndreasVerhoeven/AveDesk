// IAveVisibilityControl.h: interface for the IAveVisibilityControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVEVISIBILITYCONTROL_H__3D93B9AB_0F7C_459F_BFB5_A5B49D876338__INCLUDED_)
#define AFX_IAVEVISIBILITYCONTROL_H__3D93B9AB_0F7C_459F_BFB5_A5B49D876338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "controls_header.h"

class IAveVisibilityControl
{
protected:
	BOOL isVisible;
public:
	IAveVisibilityControl();
	~IAveVisibilityControl();

	virtual  BOOL IsVisible()const;

	virtual void SetVisibility(BOOL val);
	virtual void Hide() = 0;
	virtual void Show() = 0;
};

#endif // !defined(AFX_IAVEVISIBILITYCONTROL_H__3D93B9AB_0F7C_459F_BFB5_A5B49D876338__INCLUDED_)
