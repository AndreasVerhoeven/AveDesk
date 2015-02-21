// DeskletFilter.h: interface for the CDeskletFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETFILTER_H__98DD4699_2EFA_4C74_86D6_B80BB7787E32__INCLUDED_)
#define AFX_DESKLETFILTER_H__98DD4699_2EFA_4C74_86D6_B80BB7787E32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
class CDeskletItem;

class CDeskletFilter  
{
public:
		enum FilterType {FilterAny = 0, FilterName = 1, FilterDescription = 2, 
						FilterAuthor = 4, FilterTypeName = 8, FilterCategory = 16};

protected:
	FilterType filterType;
	std::wstring keyword;
public:
	CDeskletFilter::CDeskletFilter(FilterType filterType = FilterAny, const std::wstring& keyword = L"");
	virtual ~CDeskletFilter();

	
	virtual bool Matches(CDeskletItem* item)const;
	virtual FilterType GetType()const;
	virtual std::wstring GetKeyword()const;

};

#endif // !defined(AFX_DESKLETFILTER_H__98DD4699_2EFA_4C74_86D6_B80BB7787E32__INCLUDED_)
