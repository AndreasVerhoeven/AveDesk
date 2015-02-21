// AveInstCancelable.h: interface for the CAveInstCancelable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTCANCELABLE_H__C23BA829_39FC_4031_A0DC_EAB3BCC49990__INCLUDED_)
#define AFX_AVEINSTCANCELABLE_H__C23BA829_39FC_4031_A0DC_EAB3BCC49990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAveInstCancelable  
{
public:
	CAveInstCancelable();
	virtual ~CAveInstCancelable();

	virtual bool Cancel() = 0;
};

#endif // !defined(AFX_AVEINSTCANCELABLE_H__C23BA829_39FC_4031_A0DC_EAB3BCC49990__INCLUDED_)
