// IAveLockable.h: interface for the IAveLockable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVELOCKABLE_H__393AC0EB_FE19_4494_88E6_F772767867DC__INCLUDED_)
#define AFX_IAVELOCKABLE_H__393AC0EB_FE19_4494_88E6_F772767867DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "controls_header.h"

class IAveLockable
{
public:
	virtual ~IAveLockable() = 0;
	virtual BOOL IsLocked()const = 0;
	virtual BOOL GetRedraw(BOOL redraw)const = 0;
	virtual int Lock() = 0;
	virtual int Unlock() = 0;
};

class IAveLockableImpl
{
protected:
	int lockCount;

public:
	IAveLockableImpl();
	virtual ~IAveLockableImpl();

	virtual BOOL IsLocked()const;
	virtual BOOL GetRedraw(BOOL redraw)const;

	virtual int Lock();
	virtual int Unlock();


};

#endif // !defined(AFX_IAVELOCKABLE_H__393AC0EB_FE19_4494_88E6_F772767867DC__INCLUDED_)
