// UpdateBounds.cpp: implementation of the CUpdateBounds class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdateBounds.h"
#include "helper_funcs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUpdateBounds::CUpdateBounds() : isEmpty(true)
{

}

CUpdateBounds::~CUpdateBounds()
{

}

void CUpdateBounds::Clear()
{
	isEmpty = true;
	region.MakeEmpty();
}

bool CUpdateBounds::IsEmpty()const
{
	Matrix id;
	return region. GetRegionScansCount(&id) == 0;
}

void CUpdateBounds::Add(const RECT& r)
{
	isEmpty = false;
	SIZE s = SIZEFROMRECT(r);
	Rect rect(r.left, r.top, s.cx, s.cy);
	region.Union(rect);
}

std::list<RECT> CUpdateBounds::GetList()
{
	std::list<RECT> rectList;

	Matrix id;
	INT count = region.GetRegionScansCount(&id);
	if(count != 0)
	{
		Rect* rects = new Rect[count];
		region.GetRegionScans(&id, rects, &count);
		for(int i =0; i < count; ++i)
		{
			RECT r = {rects[i].X, rects[i].Y, + rects[i].X + rects[i].Width, rects[i].Y + rects[i].Height};
			rectList.push_back(r);
		}

		delete[] rects;
	}

	return rectList;
}