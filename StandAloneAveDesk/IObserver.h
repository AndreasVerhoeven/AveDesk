// IObserver.h: interface for the IObserver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBSERVER_H__BD79A47F_A3A7_4F54_8194_3D6E2A13031F__INCLUDED_)
#define AFX_IOBSERVER_H__BD79A47F_A3A7_4F54_8194_3D6E2A13031F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// forward declaration
class IObservable;

class IObserver  
{
public:
	virtual ~IObserver() = 0 {};

	virtual void Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2) = 0;

};

#endif // !defined(AFX_IOBSERVER_H__BD79A47F_A3A7_4F54_8194_3D6E2A13031F__INCLUDED_)
