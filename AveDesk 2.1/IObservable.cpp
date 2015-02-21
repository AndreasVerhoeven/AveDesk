// IObservable.cpp: implementation of the IObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IObservable.h"
#include <algorithm>

#include "IObserver.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool IObservable::IsRegistered(const IObserver* observer) const
{
	// pre: observer is not NULL
	// post: true has been returned iff observer is registered

	if( NULL == observer)
		return false;

	return std::find(m_Observers.begin(),m_Observers.end(),observer) != m_Observers.end();
}


bool IObservable::Register(IObserver* observer)
{
	// pre:  observer is not NULL, IsRegistered(observered) == false
	// post: observer has been registered for this IObservable

	if(NULL == observer)
		return false;

	if(IsRegistered(observer) == true)
		return false;

	m_Observers.push_back(observer);

	return true;
}


bool IObservable::Deregister(IObserver* observer)
{
	// pre: observer IsRegistered(observer) == true
	// post: observer has been registered for this IObservable and thus
	//		 doesn't get Notify(...)s anymore.

	if(NULL == observer)
		return false;

	if(IsRegistered(observer) == false)
		return false;

	m_Observers.remove(observer);

	return true;
}

bool IObservable::NotifyAll(bool paramsAreValid, LPARAM val1, LPARAM val2)
{
	// post: all observers are notified with paramsAreValid, val1, val2 as the notify data.

	std::list<IObserver*> oberserverToBeNotified = m_Observers;

	for(std::list<IObserver*>::iterator iter = oberserverToBeNotified.begin(); iter != oberserverToBeNotified.end(); iter++)
	{
		IObserver* observer = *iter;
		if(observer != NULL)
			observer->Notify(this, paramsAreValid, val1, val2);
	}

	return true;
}