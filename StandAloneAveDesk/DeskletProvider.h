// DeskletProvider.h: interface for the CDeskletProvider class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETPROVIDER_H__8D9E10CE_C56E_4739_847B_1D28EA122853__INCLUDED_)
#define AFX_DESKLETPROVIDER_H__8D9E10CE_C56E_4739_847B_1D28EA122853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DeskletItem.h"
#include "IObservable.h"

class CDeskletProvider : public IObservable 
{
protected:
	std::vector<CDeskletItem*> items;

public:
	CDeskletProvider();
	virtual ~CDeskletProvider();

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual UINT GetCount();
	virtual CDeskletItem* GetItem(UINT index);
	virtual void Clear();

};

#endif // !defined(AFX_DESKLETPROVIDER_H__8D9E10CE_C56E_4739_847B_1D28EA122853__INCLUDED_)
