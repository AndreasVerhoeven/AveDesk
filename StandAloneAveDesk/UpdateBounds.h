// UpdateBounds.h: interface for the CUpdateBounds class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATEBOUNDS_H__F47BAA5D_6007_4B3E_857D_3A70964BC8AC__INCLUDED_)
#define AFX_UPDATEBOUNDS_H__F47BAA5D_6007_4B3E_857D_3A70964BC8AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <gdiplus.h>

class CUpdateBounds  
{
protected:
	Region region;
	bool isEmpty;

public:
	CUpdateBounds();
	~CUpdateBounds();

	void Clear();
	bool IsEmpty()const;

	void Add(const RECT& r);

	std::list<RECT> GetList();

	Region* GetRegion(){return &region;};
};

#endif // !defined(AFX_UPDATEBOUNDS_H__F47BAA5D_6007_4B3E_857D_3A70964BC8AC__INCLUDED_)
