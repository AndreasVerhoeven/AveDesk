// AveInstSuspendedDeskletsList.cpp: implementation of the CAveInstSuspendedDeskletsList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstSuspendedDeskletsList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstSuspendedDeskletsList::CAveInstSuspendedDeskletsList()
{

}

CAveInstSuspendedDeskletsList::~CAveInstSuspendedDeskletsList()
{

}

ULONG CAveInstSuspendedDeskletsList::AddRef()
{
	return ++m_RefCount;
}

ULONG CAveInstSuspendedDeskletsList::Release()
{
	m_RefCount--;
	if(0 == m_RefCount)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}

CAveInstSuspendedDesklets& CAveInstSuspendedDeskletsList::GetSuspendedDesklets(std::wstring fileName)
{
	return m_List[fileName];
}

void CAveInstSuspendedDeskletsList::ReduceCountForAll()
{
	for(std::map<std::wstring,CAveInstSuspendedDesklets>::iterator iter = m_List.begin(); iter != m_List.end(); ++iter)
	{
		CAveInstSuspendedDesklets& sd = iter->second;
		sd.ReleaseCount();
	}
	
}