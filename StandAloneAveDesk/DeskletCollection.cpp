// DeskletCollection.cpp: implementation of the CDeskletCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletCollection.h"
#include "DeskletProvider.h"
#include "DeskletItem.h"
#include <functional>
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletCollection::CDeskletCollection() : provider(NULL)
{

}

std::vector<CDeskletFilter> CDeskletCollection::GetFilters()
{
	return filters;
}

void CDeskletCollection::ClearFilters()
{
	filters.clear();
}

void CDeskletCollection::SetName(const std::wstring& newName)
{
	name = newName;
}

void CDeskletCollection::Refresh()
{
	Clear();
	if(provider != NULL)
	{
		for(UINT i = 0; i < provider->GetCount(); ++i)
		{
			CDeskletItem* item = provider->GetItem(i);
			if(item != NULL)
			{
				if(MatchesFilter(item))
				{
					item->AddRef();
					items.push_back(item);
					NotifyAll(true, (LPARAM)item, 0L);
				}
				item->Release();
			}
		}
	}
}

void CDeskletCollection::AddFilter(const CDeskletFilter& filter)
{
	filters.push_back(filter);
}

CDeskletCollection::~CDeskletCollection()
{
	Clear();
	if(provider != NULL)
		provider->Deregister(this);
}

UINT CDeskletCollection::GetCount()
{
	return items.size();
}

CDeskletItem* CDeskletCollection::GetItem(UINT index)
{
	CDeskletItem* item = NULL;
	if(index < GetCount())
	{
		item = items[index];
		item->AddRef();
	}

	return item;
}

void CDeskletCollection::SetProvider(CDeskletProvider* prov)
{
	Clear();
	if(provider != NULL)
		provider->Deregister(this);

	provider = prov;
	if(prov != NULL)
	{
		for(UINT i = 0; i < prov->GetCount(); ++i)
		{
			CDeskletItem* item = prov->GetItem(i);
			if(item != NULL)
			{
				if(MatchesFilter(item))
				{
					item->AddRef();
					items.push_back(item);
					NotifyAll(true, (LPARAM)item, 0L);
				}
				item->Release();
			}
		}
		prov->Register(this);
	}
}

void CDeskletCollection::Clear()
{
	std::for_each(items.begin(), items.end(), std::mem_fun(&CDeskletItem::Release));
	items.clear();
	NotifyAll(true, NULL, 1);
}

std::wstring CDeskletCollection::GetIconFileName()
{
	return icon;
}

void CDeskletCollection::SetIconFileName(const std::wstring& fname)
{
	icon = fname;
}

bool CDeskletCollection::MatchesFilter(CDeskletItem* item)
{
	return std::find_if( filters.begin(),
						 filters.end(), 
						 std::bind2nd(std::mem_fun_ref(&CDeskletFilter::Matches), item)
						)
						!= filters.end() || filters.size() == 0;
}


std::wstring CDeskletCollection::GetName()
{
	return name;
}


HBITMAP CDeskletCollection::GetIcon()
{
	// TODO: load icon code here
	return NULL;
}

void CDeskletCollection::Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2)
{
	if(paramsAreValid)
	{
		CDeskletItem* item = reinterpret_cast<CDeskletItem*>(val1);
		if(item != NULL)
		{
			if(MatchesFilter(item))
			{
				item->AddRef();
				items.push_back(item);
				NotifyAll(true, (LPARAM)item, val2);
			}
		}
	}
}