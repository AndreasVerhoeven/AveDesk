// DeskletItem.cpp: implementation of the CDeskletItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletItem::CDeskletItem() :
	m_Ref(1),
	m_SmallIcon(NULL),
	m_BigIcon(NULL),
	m_Flags(NULL),
	m_Indicator(NULL)
{
	m_Size.cx = 0;
	m_Size.cy = 0;
}

CDeskletItem::~CDeskletItem()
{

}

ULONG CDeskletItem::AddRef()
{
	return ++m_Ref;
}

ULONG CDeskletItem::Release()
{
	ULONG newRef = --m_Ref;
	if(0 == m_Ref)
		delete this;
	
	return newRef;
}

SIZE CDeskletItem::GetSize()
{
	return m_Size;
}

std::wstring CDeskletItem::GetName()
{
	return m_Name;
}
std::wstring CDeskletItem::GetAuthor()
{
	return m_Author;
}

std::wstring CDeskletItem::GetDescription()
{
	return m_Description;
}

DWORD CDeskletItem::GetFlags()
{
	return m_Flags;
}

std::wstring CDeskletItem::GetGUID()
{
	return m_GUID;
}