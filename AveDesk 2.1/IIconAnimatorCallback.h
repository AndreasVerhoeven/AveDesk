// IIconAnimatorCallback.h: interface for the IIconAnimatorCallback class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IICONANIMATORCALLBACK_H__E9954DF1_E3C2_470D_8FAB_02B44E67D58A__INCLUDED_)
#define AFX_IICONANIMATORCALLBACK_H__E9954DF1_E3C2_470D_8FAB_02B44E67D58A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IIconAnimatorCallback  
{
public:
	virtual bool Render(HICON icon, DWORD callbackValue) = 0;

};

#endif // !defined(AFX_IICONANIMATORCALLBACK_H__E9954DF1_E3C2_470D_8FAB_02B44E67D58A__INCLUDED_)
