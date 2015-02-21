// IAveLockable.cpp: implementation of the IAveLockable class.
//
//////////////////////////////////////////////////////////////////////

#include "IAveLockable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IAveLockableImpl::IAveLockableImpl() : lockCount(0)
{

}

IAveLockableImpl::~IAveLockableImpl()
{

}

BOOL IAveLockableImpl::IsLocked()const
{
	return lockCount != 0;
};

BOOL IAveLockableImpl::GetRedraw(BOOL redraw)const
{
	return redraw && !IsLocked();
};

int IAveLockableImpl::Lock()
{
	return ++lockCount;
};
int IAveLockableImpl::Unlock()
{
	return --lockCount;
};
