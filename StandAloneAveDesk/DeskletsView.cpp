// DeskletsView.cpp: implementation of the CDeskletsView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletsView.h"
#include <algorithm>
#include <functional>
#include "DeskletListCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletsView::CDeskletsView() : 
	m_Collection(NULL),
	m_Control(NULL),
	m_SearchFilter(NULL)
{

}

CDeskletsView::~CDeskletsView()
{
	if(m_Collection != NULL)
		m_Collection->Deregister(this);

	ClearItems();

	delete m_SearchFilter;
}

void CDeskletsView::SetControl(CDeskletListCtrl* ctrl)
{
	m_Control = ctrl;
}

void CDeskletsView::SetCollection(CDeskletCollection* collection)
{
	if(m_Collection != NULL)
		m_Collection->Deregister(this);

	m_Collection = collection;
	ClearItems();
	if(m_Collection != NULL)
	{
		if(m_Control != NULL)
			m_Control->ResetContent();

		for(UINT i = 0; i < m_Collection->GetCount(); ++i)
		{
			CDeskletItem* item = m_Collection->GetItem(i);
			if(item != NULL)
			{
				AddItem(item);
				item->Release();
			}
		}

		m_Collection->Register(this);
	}
}

CDeskletCollection* CDeskletsView::GetCollection()
{
	return m_Collection;
}

void CDeskletsView::Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2)
{
	if(paramsAreValid)
	{
		CDeskletItem* item = reinterpret_cast<CDeskletItem*>(val1);
		if(item != NULL)
			AddItem(item);
	}
}

std::wstring CDeskletsView::GetName()
{
	return m_Name;
}


void CDeskletsView::SetName(const std::wstring& name)
{
	m_Name = name;
}

void CDeskletsView::SetSearchText(const std::wstring& text)
{
	m_SearchText = text;
	delete m_SearchFilter;
	m_SearchFilter = NULL;

	CDeskletItem* selItem = NULL;
	if(m_SearchText.length() > 0)
	{
		m_SearchFilter = new CDeskletFilter(CDeskletFilter::FilterAny, m_SearchText);
	}

	if(m_Control != NULL)
	{
		::LockWindowUpdate(m_Control->m_hWnd);
		::SendMessage(m_Control->m_hWnd, WM_SETREDRAW, (WPARAM)FALSE, 0L);

		int curSel = m_Control->GetCurSel();
		selItem = GetItem(curSel);
		int curTop = m_Control->GetTopIndex();

		m_Control->ResetContent();
	}

	m_FilterItems.clear();

	for(UINT i = 0; i < m_Items.size(); ++i)
	{
		CDeskletItem* item = m_Items[i];
		if(item != NULL)
		{
			if(NULL == m_SearchFilter || m_SearchFilter->Matches(item))
			{
				m_FilterItems.push_back(item);
				if(m_Control != NULL)
				{
					m_Control->AddItem(item);
					if(item == selItem)
						m_Control->SetCurSel(m_Control->GetCount()-1);
				}
			}
		}
	}

	if(m_Control != NULL)
	{
		m_Control->LockWindowUpdate(NULL);
		::SendMessage(m_Control->m_hWnd, WM_SETREDRAW, (WPARAM)TRUE, 0L);
	}
}

std::wstring CDeskletsView::GetSearchText()
{
	return m_SearchText;
}

void CDeskletsView::ClearItems()
{
	std::for_each(m_Items.begin(), m_Items.end(), std::mem_fun(&CDeskletItem::Release));
	m_Items.clear();
	m_FilterItems.clear();
}

void CDeskletsView::AddItem(CDeskletItem* item)
{
	if(item != NULL)
	{
		item->AddRef();
		m_Items.push_back(item);

		if(NULL == m_SearchFilter || m_SearchFilter->Matches(item))
		{
			m_FilterItems.push_back(item);
			if(m_Control != NULL)
				m_Control->AddItem(item);
		}
	}
}


CDeskletItem* CDeskletsView::GetItem(UINT index)
{
	CDeskletItem* ret = NULL;
	if(index < GetCount())
	{
		ret = m_FilterItems[index];
		ret->AddRef();
	}

	return ret;
}


UINT CDeskletsView::GetCount()
{
	return m_FilterItems.size();
}

void CDeskletsView::SetSel(int item)
{
	m_SelItem = item;
	if(m_Control != NULL)
		m_Control->SetCurSel(m_SelItem);
}

int CDeskletsView::GetSel()
{
	if(m_Control != NULL)
		return m_Control->GetCurSel();
	else
		return m_SelItem;
}

void CDeskletsView::SetTopItem(int item)
{
	m_TopItem = item;
	if(m_Control != NULL)
		m_Control->SetTopIndex(m_TopItem);
}

int CDeskletsView::GetTopItem()
{
	if(m_Control != NULL)
		return m_Control->GetTopIndex();
	else
		return m_TopItem;
}