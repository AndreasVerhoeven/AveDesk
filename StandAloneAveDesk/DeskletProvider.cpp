// DeskletProvider.cpp: implementation of the CDeskletProvider class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletProvider.h"

#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletProvider::CDeskletProvider()
{

}

CDeskletProvider::~CDeskletProvider()
{
	Clear();
}

void CDeskletProvider::Clear()
{
	std::for_each(items.begin(), items.end(), std::mem_fun(&CDeskletItem::Release));	
	items.clear();
}

UINT CDeskletProvider::GetCount()
{
	return items.size();
}

CDeskletItem* CDeskletProvider::GetItem(UINT index)
{
	CDeskletItem* item = NULL;
	if(index < GetCount())
	{
		item = items[index];
		item->AddRef();
	}
	return item;
}