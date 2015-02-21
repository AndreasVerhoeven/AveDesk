// IObservable.h: interface for the IObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBSERVABLE_H__DADD8B80_DD6D_4D9B_ACE8_BF734B843C47__INCLUDED_)
#define AFX_IOBSERVABLE_H__DADD8B80_DD6D_4D9B_ACE8_BF734B843C47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

// forward decleration
class IObserver;

class IObservable  
{
protected:
	std::list<IObserver*> m_Observers;

public:
	virtual ~IObservable() = 0 {};

	virtual bool IsRegistered(const IObserver* observer)const;
	virtual bool Register(IObserver* observer);
	virtual bool Deregister(IObserver* observer);

	virtual bool NotifyAll(bool paramsValid, LPARAM val1, LPARAM val2);
};

class CObservable : public IObservable 
{
	
};

#endif // !defined(AFX_IOBSERVABLE_H__DADD8B80_DD6D_4D9B_ACE8_BF734B843C47__INCLUDED_)
