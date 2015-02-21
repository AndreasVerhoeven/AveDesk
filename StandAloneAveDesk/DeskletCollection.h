// DeskletCollection.h: interface for the CDeskletCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETCOLLECTION_H__90E9BE03_649B_43D6_9D60_A8254840987D__INCLUDED_)
#define AFX_DESKLETCOLLECTION_H__90E9BE03_649B_43D6_9D60_A8254840987D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include "DeskletFilter.h"
#include "IObservable.h"
#include "IObserver.h"

class CDeskletItem;
class CDeskletProvider;

class CDeskletCollection : public IObservable, public IObserver
{
protected:
	std::vector<CDeskletItem*> items;
	std::vector<CDeskletFilter> filters;
	std::wstring name;
	std::wstring icon;
	CDeskletProvider* provider;

public:
	CDeskletCollection();
	virtual ~CDeskletCollection();
	virtual void SetProvider(CDeskletProvider* prov);

	virtual void AddFilter(const CDeskletFilter& filter);
	virtual std::vector<CDeskletFilter> GetFilters();
	virtual void ClearFilters();

	virtual void Clear();
	virtual UINT GetCount();
	virtual CDeskletItem* GetItem(UINT index);

	virtual bool MatchesFilter(CDeskletItem* item);
	virtual std::wstring GetName();
	virtual void SetName(const std::wstring& name);
	virtual HBITMAP GetIcon();
	virtual std::wstring GetIconFileName();
	virtual void SetIconFileName(const std::wstring& fname);
	virtual void Refresh();

	virtual void Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2);

};

#endif // !defined(AFX_DESKLETCOLLECTION_H__90E9BE03_649B_43D6_9D60_A8254840987D__INCLUDED_)
