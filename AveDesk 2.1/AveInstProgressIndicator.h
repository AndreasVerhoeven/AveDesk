// AveInstProgressIndicator.h: interface for the CAveInstProgressIndicator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTPROGRESSINDICATOR_H__7957795A_1583_461E_A61D_CD1988F91896__INCLUDED_)
#define AFX_AVEINSTPROGRESSINDICATOR_H__7957795A_1583_461E_A61D_CD1988F91896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CAveInstCancelable;

class CAveInstProgressIndicator  
{
public:
	CAveInstProgressIndicator();
	virtual ~CAveInstProgressIndicator();

	virtual void SetMessage(std::wstring msg) = 0;
	virtual void SetProgress(long progress, long max) = 0;
	virtual bool RegisterCancelable(CAveInstCancelable* c) = 0;
	virtual bool UnregisterCancelable(CAveInstCancelable* c) = 0;
	virtual void Hide() = 0;

};

#endif // !defined(AFX_AVEINSTPROGRESSINDICATOR_H__7957795A_1583_461E_A61D_CD1988F91896__INCLUDED_)
