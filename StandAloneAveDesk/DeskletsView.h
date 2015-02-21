// DeskletsView.h: interface for the CDeskletsView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETSVIEW_H__9F212C53_A403_4636_AF99_DCFE472C6C63__INCLUDED_)
#define AFX_DESKLETSVIEW_H__9F212C53_A403_4636_AF99_DCFE472C6C63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DeskletItem.h"
#include "IObserver.h"
#include "DeskletCollection.h"
#include "DeskletFilter.h"

class CDeskletListCtrl;

class CDeskletsView : public IObserver
{
protected:
	std::vector<CDeskletItem*> m_Items;
	std::vector<CDeskletItem*> m_FilterItems;

	std::wstring m_SearchText;
	int m_SelItem;
	int m_TopItem;

	std::wstring m_Name;

	CDeskletCollection* m_Collection;
	CDeskletListCtrl* m_Control;
	CDeskletFilter* m_SearchFilter;

public:
	CDeskletsView();
	virtual ~CDeskletsView();

	virtual void SetCollection(CDeskletCollection* collection);
	virtual CDeskletCollection* GetCollection();
	virtual void Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2);
	virtual void SetControl(CDeskletListCtrl* control);


	virtual std::wstring GetName();
	virtual void SetName(const std::wstring& name);

	virtual void SetSearchText(const std::wstring& text);
	virtual std::wstring GetSearchText();

	virtual void ClearItems();
	virtual void AddItem(CDeskletItem* item);
	virtual CDeskletItem* GetItem(UINT index);
	virtual UINT GetCount();

	virtual void SetSel(int item);
	virtual int GetSel();
	virtual void SetTopItem(int item);
	virtual int GetTopItem();

};

#endif // !defined(AFX_DESKLETSVIEW_H__9F212C53_A403_4636_AF99_DCFE472C6C63__INCLUDED_)
