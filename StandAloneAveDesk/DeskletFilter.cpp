// DeskletFilter.cpp: implementation of the CDeskletFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletFilter.h"
#include "DeskletItem.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletFilter::CDeskletFilter(FilterType filterType, const std::wstring& keyword) :
		filterType(filterType),
		keyword(keyword)
{

}

CDeskletFilter::~CDeskletFilter()
{

}

CDeskletFilter::FilterType CDeskletFilter::GetType()const
{
	return filterType;
}
std::wstring CDeskletFilter::GetKeyword()const
{
	return keyword;
}

bool CDeskletFilter::Matches(CDeskletItem* item)const
{
	if(NULL == item)
		return false;

	std::wstring search(keyword);
	std::transform(search.begin(),search.end(), search.begin(), tolower);

	if(FilterAny == filterType || FilterName == filterType)
	{
		std::wstring ws(item->GetName());
		std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
		if(wcsstr(ws.c_str(), search.c_str()) != NULL)
			return true;
	}

	if(FilterAny == filterType || FilterDescription == filterType)
	{
		std::wstring ws(item->GetDescription());
		std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
		if(wcsstr(ws.c_str(), search.c_str()) != NULL)
			return true;
	}

	if(FilterAny == filterType || FilterAuthor == filterType)
	{
		std::wstring ws(item->GetAuthor());
		std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
		if(wcsstr(ws.c_str(), search.c_str()) != NULL)
			return true;
	}

	if(FilterAny == filterType || FilterTypeName == filterType)
	{
		std::wstring ws(item->GetTypeName());
		std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
		if(wcsstr(ws.c_str(), search.c_str()) != NULL)
			return true;
	}

	if(FilterAny == filterType || FilterCategory == filterType)
	{
		std::wstring ws(item->GetCategoryName());
		std::transform(ws.begin(), ws.end(), ws.begin(), tolower);
		if(wcsstr(ws.c_str(), search.c_str()) != NULL)
			return true;
	}


	return false;
}