// IProgressIndicator.h: interface for the IProgressIndicator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPROGRESSINDICATOR_H__8095F5DD_C389_4F68_8400_F7B676F66553__INCLUDED_)
#define AFX_IPROGRESSINDICATOR_H__8095F5DD_C389_4F68_8400_F7B676F66553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class IProgressIndicator  
{
public:
	virtual ~IProgressIndicator() = 0;

	virtual void SetMessage(const std::wstring& msg) = 0;
	virtual void SetProgress(UINT procent) = 0;
	virtual void NotifyDone() = 0;
	virtual void NotifyBlocked() = 0;
	virtual void NotifyError(const std::wstring& error) = 0;


};

#endif // !defined(AFX_IPROGRESSINDICATOR_H__8095F5DD_C389_4F68_8400_F7B676F66553__INCLUDED_)
