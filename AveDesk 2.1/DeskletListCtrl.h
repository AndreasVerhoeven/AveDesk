#pragma once

#include "DeskletsView.h"

#define WM_STARTS_DRAGGING WM_USER + 8067

template< class T, class TBase = CListBox, class TWinTraits = CControlWinTraits > 
class ATL_NO_VTABLE CDeskletListCtrlImpl : 
	public CWindowImpl< T, TBase, TWinTraits >,
	public COwnerDraw< CDeskletListCtrlImpl<T> >
{

public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	CBitmap dummyBmp;
	CDeskletsView* view;
	SIZE iconSize;

	CFont bigFont;
	CFont smallFont;
	CIcon noIcon;

	static const UINT WM_ICON_NOTIFY = WM_USER + 304;

	int val;
	DWORD origTid;

	BOOL noLButtonHandling;

	CDeskletListCtrlImpl() : val(0)
	{
		noLButtonHandling = FALSE;

		origTid = ::GetCurrentThreadId();

		noIcon.LoadIcon(IDI_ICON3);

		LOGFONTW lfBig = {0};
		lfBig.lfCharSet = DEFAULT_CHARSET;
		wcscpy(lfBig.lfFaceName,L"MS Shell Dlg");
		lfBig.lfWeight = FW_BOLD;
		lfBig.lfHeight = 14;

		LOGFONTW lfSmall = {0};
		lfSmall.lfCharSet = DEFAULT_CHARSET;
		wcscpy(lfSmall.lfFaceName,L"MS Shell Dlg");
		lfSmall.lfWeight = FW_NORMAL;
		lfSmall.lfHeight = 12;

		bigFont.CreateFontIndirect(&lfBig);
		smallFont.CreateFontIndirect(&lfSmall);
	}
	~CDeskletListCtrlImpl(){}

	 BEGIN_MSG_MAP(CDeskletListCtrlImpl)
      //MESSAGE_HANDLER(WM_CREATE, OnCreate)
      //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
	  MESSAGE_HANDLER(WM_ICON_NOTIFY, OnIconNotify)
	  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CDeskletListCtrlImpl>, 1)
      DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()


   LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
	   int imgW = iconSize.cx, imgH = iconSize.cy;
	   int xPos = GET_X_LPARAM(lParam);
	   //int yPos = GET_Y_LPARAM(lParam);

	   if(noLButtonHandling || xPos > imgH + 3)
	   {
		   bHandled = FALSE;
		   return 0;
	   }

	   DWORD msgPos = ::GetMessagePos();
	   POINT pt = {GET_X_LPARAM(msgPos), GET_Y_LPARAM(msgPos)};
	   BOOL startsDragging = ::DragDetect(m_hWnd, pt);
	   if(startsDragging)
	   {
		   noLButtonHandling = TRUE;
		   ::SendMessage(m_hWnd, WM_LBUTTONDOWN, wParam, lParam);
		   ::SendMessage(m_hWnd, WM_LBUTTONUP, wParam, lParam);
		   noLButtonHandling = FALSE;

		   //POINT pos = {xPos, yPos};
		   //::LBItemFromPt(m_hWnd, &pos, FALSE);
		   int curSel = GetCurSel();
		   ::SendMessage(GetParent(), WM_STARTS_DRAGGING, (WPARAM) curSel, (LPARAM)0L);

		   //::MessageBox(NULL, _T("starts dragging"), NULL, 0);
	   }
	   else
	   {
		   //LRESULT res = 0;
		   //CWindowImpl< T, TBase, TWinTraits >::ProcessWindowMessage(m_hWnd, uMsg, wParam, lParam, res, 0);
		   bHandled = TRUE;
		   noLButtonHandling = TRUE;
		   ::SendMessage(m_hWnd, WM_LBUTTONDOWN, wParam, lParam);
		   ::SendMessage(m_hWnd, WM_LBUTTONUP, wParam, lParam);
		   noLButtonHandling = FALSE;
	   }

		return 0;
   }

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
		//if( bRet ) _Init();
		return bRet;
	}

	 LRESULT OnIconNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	 {
		 CDeskletItem* iconDoneItem = reinterpret_cast<CDeskletItem*>(wParam);
		 if(view != NULL)
		 {
			 int index(-1);
			 for(UINT i = 0; i < view->GetCount(); ++i)
			 {
				 CDeskletItem* item = view->GetItem(i);
				 if(item != NULL)
				 {
					if(item->Equals(iconDoneItem))
					{
						index = (int)i;
						break;	
					}
					item->Release();
				 }
			 }

			 if(index != -1)
			 {
				RECT r = {0};
				GetItemRect(index, &r);
				InvalidateRect(&r, FALSE);
			 }

		 }

		return 0;
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



	void DrawItem(LPDRAWITEMSTRUCT lpdis)
	{
		DWORD tid = ::GetCurrentThreadId();
		ATLASSERT(tid == origTid);
		//ATLASSERT(val == 0);
		
		if( lpdis->itemID==-1 ) return; // If there are no list box items, skip this message. 

		val++;
		

		CDCHandle dc(lpdis->hDC);
		RECT rcItem = lpdis->rcItem;
		bool bSelected = lpdis->itemState & ODS_SELECTED;
		int rcW = rcItem.right - rcItem.left;
		int rcH = rcItem.bottom - rcItem.top;
		int rcX = 0;//rcItem.left;
		int rcY = 0;//rcItem.top;
		UINT index = lpdis->itemID;

		 //winDC(NULL);
		//CDCHandle bufDC = dc;
		CDC bufDC;
		bufDC.CreateCompatibleDC(CWindowDC(NULL));
		CBitmap bufBmp;
		bufBmp.CreateCompatibleBitmap(CWindowDC(NULL), rcW, rcH);
		if(::GetObjectType(bufBmp) == OBJ_BITMAP)
			bufDC.SelectBitmap(bufBmp);


		CBrush backBrush;
		COLORREF frontColor;
		bool hasFocus = m_hWnd == ::GetFocus();
		int highLightIndex = hasFocus ? COLOR_HIGHLIGHT : COLOR_BTNSHADOW;
		frontColor = GetSysColor((bSelected) ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT);
		backBrush.CreateSysColorBrush(bSelected ? highLightIndex : COLOR_WINDOW);

		CDeskletItem* item = NULL;
		std::wstring str;
		if(view != NULL)
			item = view->GetItem(index);
	
		if(NULL == item)
			return;

		RECT fillRect={rcX,rcY,rcW + rcX,rcH + rcY};
		bufDC.FillRect(&fillRect, backBrush);

		RECT textRect={iconSize.cx + 4 + rcX,2 + rcY, rcW + rcX, rcH + rcY};
		bufDC.SetBkMode(TRANSPARENT);
		bufDC.SetTextColor(frontColor);
		CFontHandle oldFont;
		if(iconSize.cy > 16)
			oldFont = bufDC.SelectFont(bigFont);
		else
			oldFont = bufDC.SelectFont(smallFont);
		bufDC.DrawText(item->GetName().c_str(), -1, &textRect, DT_LEFT |  DT_SINGLELINE	| DT_END_ELLIPSIS);

		std::wstring details;//(item->GetAuthor());
		//details += L"\n";
		details += item->GetDescription();

		RECT detailsTextRect={iconSize.cx + 4 + rcX, 16 + rcY, rcW + rcX, rcH + rcY};
		bufDC.SelectFont(smallFont);
		bufDC.DrawText(details.c_str(), -1, &detailsTextRect, DT_LEFT |  DT_EDITCONTROL | DT_WORDBREAK);

		bufDC.SelectFont(oldFont);

		if(iconSize.cy > 16 && rcItem.left+2 < iconSize.cx)
		{
			SIZE defIconSize = {80,80};
			CBitmapHandle bmp = item->GetSmallIcon();
			if(NULL == bmp.m_hBitmap)
			{
				item->StartCreatingSmallIcon(defIconSize,m_hWnd, WM_ICON_NOTIFY);
			}


			
			if(bmp.m_hBitmap != NULL)
			{
				CBitmapHandle hbmp = bmp;
				CDC tempDC, tempDC2;
				CBitmap tempBmp;
				tempDC.CreateCompatibleDC(CWindowDC(NULL));
				tempDC2.CreateCompatibleDC(CWindowDC(NULL));
				tempBmp.CreateCompatibleBitmap(CWindowDC(NULL), defIconSize.cx, defIconSize.cy);
				if(::GetObjectType(tempBmp) == OBJ_BITMAP)
					tempDC.SelectBitmap(tempBmp);

				RECT fillRect = {0,0,defIconSize.cx, defIconSize.cy};
				tempDC.FillRect(&fillRect, backBrush);

				if(::GetObjectType(hbmp) == OBJ_BITMAP)
					tempDC2.SelectBitmap(hbmp);
				BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
				tempDC.AlphaBlend(0,0, defIconSize.cx, defIconSize.cx, tempDC2, 0, 0, defIconSize.cx, defIconSize.cx, bf);
				bufDC.SetStretchBltMode(HALFTONE);
				bufDC.StretchBlt(2+rcX,2+rcY, iconSize.cx, iconSize.cy, tempDC, 0, 0, defIconSize.cx, defIconSize.cy, SRCCOPY);
			}
			else
			{
				noIcon.DrawIconEx(bufDC, 2+rcX, 2+rcY, iconSize.cx, iconSize.cy);
			}
		}

		item->Release();
		
		dc.BitBlt(rcItem.left, rcItem.top, rcW, rcH, bufDC, 0, 0, SRCCOPY);

		val--;
	}

	void AddItem(CDeskletItem* item)
	{
		CListBox::AddString(item->GetName().c_str());
	}

	void AddString(LPCTSTR str)
	{
		CListBox::AddString(str);
	}

};

class CDeskletListCtrl : public CDeskletListCtrlImpl<CDeskletListCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("AveCDeskletListCtrl"), GetWndClassName())  
};
