// AveInstSuspendedDeskletsList.h: interface for the CAveInstSuspendedDeskletsList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTSUSPENDEDDESKLETSLIST_H__FF85F2C7_BE30_4084_85F0_86B6479006B1__INCLUDED_)
#define AFX_AVEINSTSUSPENDEDDESKLETSLIST_H__FF85F2C7_BE30_4084_85F0_86B6479006B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include "AveInstSuspendedDesklets.h"

class CAveInstSuspendedDeskletsList  
{
private:
	std::map<std::wstring, CAveInstSuspendedDesklets> m_List;
	ULONG m_RefCount;

public:
	CAveInstSuspendedDeskletsList();
	virtual ~CAveInstSuspendedDeskletsList();

	CAveInstSuspendedDesklets& GetSuspendedDesklets(std::wstring fileName);

	void ReduceCountForAll();

	virtual ULONG AddRef();
	virtual ULONG Release();
};

#endif // !defined(AFX_AVEINSTSUSPENDEDDESKLETSLIST_H__FF85F2C7_BE30_4084_85F0_86B6479006B1__INCLUDED_)
