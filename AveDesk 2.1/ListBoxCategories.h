// ListBoxCategories.h: interface for the CListBoxCategories class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTBOXCATEGORIES_H__A1947970_F080_4015_BF01_36EAF668AAB1__INCLUDED_)
#define AFX_LISTBOXCATEGORIES_H__A1947970_F080_4015_BF01_36EAF668AAB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template< class T, class TBase = CListBox, class TWinTraits = CControlWinTraits > 
class ATL_NO_VTABLE CListBoxCategoriesImpl : 
	public CWindowImpl< T, TBase, TWinTraits >,
	public COwnerDraw< CListBoxCategoriesImpl<T> >
{
public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	CImageList imgList;

	CListBoxCategoriesImpl(){}
	~CListBoxCategoriesImpl(){}

	 BEGIN_MSG_MAP(CImageListBoxImpl)
      //MESSAGE_HANDLER(WM_CREATE, OnCreate)
      //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CListBoxCategoriesImpl>, 1)
      DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
		//if( bRet ) _Init();
		return bRet;
	}

	 LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {

		CDCHandle dc((HDC)wParam);
		RECT rc;
		GetClientRect(&rc);

		HRGN origClipRegion = NULL;
		{
			GetClipRgn(dc, origClipRegion);
			int numItems = GetCount();
			for(int i = 0; i < numItems; ++i)
			{
				RECT itemRect = {0};
				GetItemRect(i, &itemRect);
				ExcludeClipRect(dc, itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);
			}

		}

		//CBrush backBrush;
		//backBrush.CreateSysColorBrush(COLOR_WINDOW);
		dc.FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));

		SelectClipRgn(dc, origClipRegion);
		DeleteObject(origClipRegion);

		return 1;   // no background needed
   }


	void SetImageList(HIMAGELIST hImageList)
	{
		imgList = hImageList;	
	}

	void DrawItem(LPDRAWITEMSTRUCT lpdis)
	{
		if( lpdis->itemID==-1 ) return; // If there are no list box items, skip this message. 
		

		CDCHandle dc(lpdis->hDC);
		RECT rcItem = lpdis->rcItem;
		bool bSelected = lpdis->itemState & ODS_SELECTED;
		int rcW = rcItem.right - rcItem.left;
		int rcH = rcItem.bottom - rcItem.top;

		CWindowDC winDC(NULL);
		CDC bufDC;
		bufDC.CreateCompatibleDC(winDC);
		CBitmap bufBmp;
		bufBmp.CreateCompatibleBitmap(winDC, rcW, rcH);
		bufDC.SelectBitmap(bufBmp);


		CBrush backBrush;
		COLORREF frontColor;
		bool hasFocus = m_hWnd == ::GetFocus();
		int highLightIndex = hasFocus ? COLOR_HIGHLIGHT : COLOR_BTNSHADOW;
		frontColor = GetSysColor((bSelected) ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT);
		backBrush.CreateSysColorBrush(bSelected ? highLightIndex : COLOR_WINDOW);


		SIZE iconSize = {0};
		imgList.GetIconSize(iconSize);
		RECT fillRect={0,0,rcW,rcH};
		bufDC.FillRect(&fillRect, backBrush);
		if(rcH < iconSize.cy || rcW < iconSize.cx)
		{
			CDC iconDC;
			CBitmap iconBmp;
			iconDC.CreateCompatibleDC(winDC);
			iconBmp.CreateCompatibleBitmap(winDC, iconSize.cx, iconSize.cy);
			iconDC.SelectBitmap(iconBmp);
			RECT fillRect = {0,0,iconSize.cx, iconSize.cy};
			iconDC.FillRect(&fillRect, backBrush);
			imgList.Draw(iconDC, lpdis->itemID, 0, 0, ILD_NORMAL);
			bufDC.SetStretchBltMode(HALFTONE);
			bufDC.StretchBlt(0,0, rcW, rcW, iconDC, 0, 0, iconSize.cy, iconSize.cy, SRCCOPY);
		}
		else
		{
			imgList.Draw(bufDC, lpdis->itemID, rcW/2 - iconSize.cx/2, 2, ILD_NORMAL);
		}



		RECT textRect={0,iconSize.cy + 2 + 2, rcW, rcH};
		bufDC.SetBkMode(TRANSPARENT);
		bufDC.SetTextColor(frontColor);
		CFontHandle oldFont = bufDC.SelectFont(dc.GetCurrentFont());
		CString str;
		CListBox::GetText(lpdis->itemID, str.GetBuffer(MAX_PATH));
		RECT rcMeasure = textRect;
		bufDC.DrawText(str, -1, &rcMeasure, DT_CENTER |  DT_SINGLELINE	| DT_END_ELLIPSIS | DT_CALCRECT); 
		int textH = rcMeasure.bottom - rcMeasure.top;
		if(iconSize.cy + 2 + textH <= rcH)
			bufDC.DrawText(str, -1, &textRect, DT_CENTER |  DT_SINGLELINE	| DT_END_ELLIPSIS); 
		str.ReleaseBuffer();
		bufDC.SelectFont(oldFont);

		dc.BitBlt(rcItem.left, rcItem.top, rcW, rcH, bufDC, 0, 0, SRCCOPY);
	}

	void AddString(LPCTSTR str)
	{
		CListBox::AddString(str);
	}

};

class CListBoxCategoriesCtrl : public CListBoxCategoriesImpl<CListBoxCategoriesCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_CListBoxCategoriesCtrl"), GetWndClassName())  
};


#endif // !defined(AFX_LISTBOXCATEGORIES_H__A1947970_F080_4015_BF01_36EAF668AAB1__INCLUDED_)
