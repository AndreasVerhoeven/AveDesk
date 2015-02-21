#if !defined(AFX_DeskletListBox_H__20011106_AF04_B15C_D1F1_0080AD509054__INCLUDED_)
#define AFX_DeskletListBox_H__20011106_AF04_B15C_D1F1_0080AD509054__INCLUDED_

#pragma once

#include "ImageListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDeskletListBox - A ListBox control with that extra touch
//
// For this control to work, you must add the
//   REFLECT_NOTIFICATIONS()
// macro to the parent's message map.
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
  #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
  #error DeskletListBox.h requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error DeskletListBox.h requires atlctrls.h to be included first
#endif


#define WM_STARTS_DRAGGING WM_USER + 8067

template< class T, class TBase = CListBox, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CDeskletListBoxImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public COwnerDraw< CDeskletListBoxImpl<T> >
	//public CImageListBoxImpl<T, CListBox, TWinTraits>
{
public:
   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

   BOOL noLButtonHandling;

   CDeskletListBoxImpl() : m_hSubFont(NULL), useDoubleBuffering(TRUE)
   { 
	   noLButtonHandling = FALSE;

      ::ZeroMemory(&m_st, sizeof(m_st));      
      m_st.ptArc.x = m_st.ptArc.y = 8;

	  iconSize.cx = -1; iconSize.cy = -1;
   }

   ILBSETTINGS m_st;
   CImageList m_imgNormal;
   CImageList m_imgSelected;
   HFONT m_hSubFont;

   SIZE iconSize;
   void GetIconSize(int& w, int& h, BOOL forceReal=FALSE)
   {
	   if(NULL == m_imgNormal.m_hImageList)
		   return;

	   if(forceReal)
	   {
		   m_imgNormal.GetIconSize(w,h);
		   return;
	   }

		if(iconSize.cx != -1 && iconSize.cy != -1)
		{
			w = iconSize.cx;
			h = iconSize.cy;
		}
		else
		{
			m_imgNormal.GetIconSize(w,h);
		}
   }

   BOOL useDoubleBuffering;

   // Message map
   
   BEGIN_MSG_MAP(CDeskletListBoxImpl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
      CHAIN_MSG_MAP_ALT(COwnerDraw<CDeskletListBoxImpl>, 1)
      DEFAULT_REFLECTION_HANDLER()
   END_MSG_MAP()

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd == NULL);
      ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
      TCHAR szBuffer[16] = { 0 };
      if( ::GetClassName(hWnd, szBuffer, 15) ) {
         ATLASSERT(::lstrcmpi(szBuffer, CListBox::GetWndClassName())==0);
      }
#endif
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
      if( bRet ) _Init();
      return bRet;
   }

   int AddString(LPCTSTR lpszItem)
   {
      ATLASSERT(false);
   }
   int InsertString(int nIndex, LPCTSTR lpszItem)
   {
      ATLASSERT(false);
   }
   void SetItemData(int nIndex, LPARAM lParam)
   {
      ATLASSERT(false);
   }

   LPARAM GetItemData(int iIndex)
   {
		ATLASSERT(::IsWindow(m_hWnd));

		LPARAM itemData = static_cast<LPARAM>(NULL);

		if(iIndex >= 0 && iIndex < GetCount())
		{
			PILBITEM pItem = reinterpret_cast<PILBITEM>(CListBox::GetItemData(iIndex));
			if(pItem != NULL)
			{
				itemData = pItem->lParam;
			}
		}

		return itemData;
   }

   int AddItem(const ILBITEM* pItem)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      // Create a copy of the ITEM structure
      PILBITEM pNewItem = NULL;      
      ATLTRY(pNewItem = new ILBITEM);
      ATLASSERT(pNewItem);
      ::ZeroMemory(pNewItem, sizeof(ILBITEM));
      UINT mask = pItem->mask;
      pNewItem->mask = mask;
      if( mask & ILBIF_TEXT ) {
         ATLTRY(pNewItem->pszText = new TCHAR[::lstrlen(pItem->pszText)+1]);
         ::lstrcpy( pNewItem->pszText, pItem->pszText );
      }
      if( mask & ILBIF_SUBTEXT ) {
         ATLTRY(pNewItem->pszSubText = new TCHAR[::lstrlen(pItem->pszSubText )+1]);
         ::lstrcpy( pNewItem->pszSubText, pItem->pszSubText );
      }
      if( mask & ILBIF_STYLE ) pNewItem->style = pItem->style;
      if( mask & ILBIF_FORMAT ) pNewItem->format = pItem->format;
      if( mask & ILBIF_IMAGE ) pNewItem->iImage = pItem->iImage;
      if( mask & ILBIF_SELIMAGE ) pNewItem->iSelImage = pItem->iSelImage;
      if( mask & ILBIF_PARAM ) pNewItem->lParam = pItem->lParam;
      // Add item to listbox
      int iItem = CListBox::AddString(pNewItem->pszText != NULL ? pNewItem->pszText : _T("") );
	  pNewItem->iItem = iItem;
      if( iItem>=0 ) CListBox::SetItemDataPtr(iItem, pNewItem);
      return iItem;
   }

   int InsertItem(const ILBITEM* pItem)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      // Create a copy of the ITEM structure
      PILBITEM pNewItem;      
      ATLTRY(pNewItem = new ILBITEM);
      ATLASSERT(pNewItem);
      ::ZeroMemory(pNewItem, sizeof(ILBITEM));
      UINT mask = pItem->mask;
      pNewItem->mask = mask;
      if( mask & ILBIF_TEXT ) {
         ATLTRY(pNewItem->pszText = new TCHAR[::lstrlen(pItem->pszText)+1]);
         ::lstrcpy( pNewItem->pszText, pItem->pszText );
      }
      if( mask & ILBIF_SUBTEXT ) {
         ATLTRY(pNewItem->pszSubText = new TCHAR[::lstrlen(pItem->pszSubText )+1]);
         ::lstrcpy( pNewItem->pszSubText, pItem->pszSubText );
      }
      if( mask & ILBIF_STYLE ) pNewItem->style = pItem->style;
      if( mask & ILBIF_FORMAT ) pNewItem->format = pItem->format;
      if( mask & ILBIF_IMAGE ) pNewItem->iImage = pItem->iImage;
      if( mask & ILBIF_SELIMAGE ) pNewItem->iSelImage = pItem->iSelImage;
      if( mask & ILBIF_PARAM ) pNewItem->lParam = pItem->lParam;
      // Add item to listbox
      int iItem = CListBox::InsertString(pItem->iItem, pNewItem->pszText != NULL ? pNewItem->pszText : _T("") );
      if( iItem>=0 ) CListBox::SetItemDataPtr(iItem, pNewItem);
      return iItem;
   }

   BOOL GetItem(ILBITEM *pItem) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(pItem);
      PILBITEM pOrgItem = (PILBITEM) CListBox::GetItemData(pItem->iItem);
      if( pOrgItem==NULL ) return FALSE;
      // Copy attributes
      UINT mask = pItem->mask;
      if( mask & ILBIF_TEXT ) ::lstrcpyn( pItem->pszText, pOrgItem->pszText, pItem->cchMaxText );
      if( mask & ILBIF_SUBTEXT ) ::lstrcpyn( pItem->pszSubText, pOrgItem->pszSubText, pItem->cchMaxSubText );
      if( mask & ILBIF_STYLE ) pItem->style = pOrgItem->style;
      if( mask & ILBIF_FORMAT ) pItem->format = pOrgItem->format;
      if( mask & ILBIF_IMAGE ) pItem->iImage = pOrgItem->iImage;
      if( mask & ILBIF_SELIMAGE ) pItem->iSelImage = pOrgItem->iSelImage;
      if( mask & ILBIF_PARAM ) pItem->lParam = pOrgItem->lParam;
      return TRUE;
   }
   BOOL SetItem(const ILBITEM *pItem)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(pItem);     
      // Get original item data and set attributes
      PILBITEM pOrgItem = (PILBITEM) CListBox::GetItemData(pItem->iItem);
      if( pOrgItem==NULL ) return FALSE;
      UINT mask = pItem->mask;
      if( mask & ILBIF_TEXT ) {
         if( pOrgItem->mask & ILBIF_TEXT ) delete [] pOrgItem->pszText;
         ATLTRY(pOrgItem->pszText = new TCHAR[::lstrlen(pItem->pszText)+1]);
         ::lstrcpy( pOrgItem->pszText, pItem->pszText );
      }
      if( mask & ILBIF_SUBTEXT ) {
         if( pOrgItem->mask & ILBIF_SUBTEXT ) delete [] pOrgItem->pszSubText;
         ATLTRY(pOrgItem->pszSubText = new TCHAR[::lstrlen(pItem->pszSubText)+1]);
         ::lstrcpy( pOrgItem->pszSubText, pItem->pszSubText );
      }
      if( mask & ILBIF_STYLE ) pOrgItem->style = pItem->style;
      if( mask & ILBIF_FORMAT ) pOrgItem->format = pItem->format;
      if( mask & ILBIF_IMAGE ) pOrgItem->iImage = pItem->iImage;
      if( mask & ILBIF_SELIMAGE ) pOrgItem->iSelImage = pItem->iSelImage;
      if( mask & ILBIF_PARAM ) pOrgItem->lParam = pItem->lParam;
      pOrgItem->mask |= pItem->mask;
      // Repaint item
      RECT rc;
      GetItemRect(pItem->iItem, &rc);
      InvalidateRect(&rc, TRUE);
      return TRUE;
   }
   CImageList SetImageList(HIMAGELIST hImageList, int nImageList)
   {
      HIMAGELIST hOldList = NULL;
      switch( nImageList ) {
      case ILSIL_NORMAL:
         hOldList = m_imgNormal;
         m_imgNormal = hImageList;
         break;
      case ILSIL_SELECTED:
         hOldList = m_imgSelected;
         m_imgSelected = hImageList;
         break;
      default:
         ATLASSERT(false);
      }    
      return hOldList;
   }
   HFONT SetSmallFont(HFONT hFont)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      HFONT hOldFont = m_hSubFont;
      m_hSubFont = hFont;
      Invalidate();
      return hOldFont;
   }
   void GetPreferences(PILBSETTINGS pPrefs) const
   {
      ATLASSERT(pPrefs);
      *pPrefs = m_st;
   }
   void SetPreferences(ILBSETTINGS Prefs)
   {
      m_st = Prefs;
   }

   // Implementation

   void _Init()
   {
      ATLASSERT(::IsWindow(m_hWnd));

      ModifyStyle(LBS_SORT | LBS_HASSTRINGS | LBS_MULTIPLESEL, LBS_NOTIFY);

      ATLASSERT(GetStyle() & LBS_OWNERDRAWFIXED); // Needs "OWNER DRAW" to be "FIXED"

      T* pT = static_cast<T*>(this);
      pT->_InitSettings();
   }
   void _InitSettings()
   {
      m_st.clrText = ::GetSysColor(COLOR_WINDOWTEXT);
      m_st.clrBackground = ::GetSysColor(COLOR_WINDOW);
      m_st.clrHighliteText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
      m_st.clrHighlite = 
      m_st.clrHighliteBorder = ::GetSysColor(COLOR_HIGHLIGHT);
   }

   // Message handlers
   
   LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
	   int imgW = 0, imgH = 0;
	   GetIconSize(imgW, imgH);
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

   LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
      _Init();
      return lRes;
   }
   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      ResetContent(); // Make sure to delete item-data memory
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {

		CDCHandle dc((HDC)wParam);
		RECT rc;
		GetClientRect(&rc);

		HRGN origClipRegion = NULL;
		if(useDoubleBuffering)
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

		dc.FillSolidRect(&rc, m_st.clrBackground);

		if(useDoubleBuffering)
		{
			SelectClipRgn(dc, origClipRegion);
			DeleteObject(origClipRegion);
		}

		return 1;   // no background needed
   }

   // Owner draw methods

   void DeleteItem(LPDELETEITEMSTRUCT lpdis)
   {
      if( lpdis->itemData ) {
         PILBITEM pItem = reinterpret_cast<PILBITEM>(lpdis->itemData);
         if( pItem->mask & ILBIF_TEXT ) delete [] pItem->pszText;
         if( pItem->mask & ILBIF_SUBTEXT ) delete [] pItem->pszSubText;
         delete pItem;
      }
   }

	POINT GetImagePt(RECT& rcItem, PILBITEM pItem, bool bSelected, UINT style)
	{
		// Get image information
      int iImageWidth = 0, iImageHeight = 0;
      if(pItem->mask & ILBIF_IMAGE )
	  {
		  GetIconSize(iImageWidth, iImageHeight);
		// m_imgNormal.GetIconSize(iImageWidth, iImageHeight);
	  }

		POINT pt = {0};
		// Draw image (may be aligned left/right/top)
      if( pItem->mask & ILBIF_IMAGE )
	  {
         if( (style & ILBS_IMGRIGHT) == 0 )
		 {
            // Left- or top-aligned image
            if( style & ILBS_IMGTOP )
			{
               pt.x = rcItem.left + ((rcItem.right - rcItem.left) / 2) - (iImageWidth / 2);
               pt.y = rcItem.top;
            }
            else
			{
               pt.x = rcItem.left;
               pt.y = rcItem.top + ((rcItem.bottom - rcItem.top) / 2) - (iImageHeight / 2);
            }


            if( style & ILBS_IMGTOP)
               rcItem.top += iImageHeight;
            else 
               rcItem.left += iImageWidth;
            
         }
         else 
		 {
            // Right-aligned image
            pt.x = rcItem.right - iImageWidth;
            pt.y = rcItem.top + ((rcItem.bottom - rcItem.top) / 2) - (iImageHeight / 2);
            rcItem.right -= iImageWidth;
         }
	  }

	  return pt;
	}

   void DrawImageInternal(CDCHandle& dc, RECT& rcItem, PILBITEM pItem, bool bSelected, UINT style)
   {
	  	// Get image information
      int iImageWidth = 0, iImageHeight = 0;
      if( pItem->mask & ILBIF_IMAGE )
	  {
		  GetIconSize(iImageWidth, iImageHeight, TRUE);
		  //m_imgNormal.GetIconSize(iImageWidth, iImageHeight);
	  }

	// Draw image (may be aligned left/right/top)
      if( pItem->mask & ILBIF_IMAGE ) {
         if( (style & ILBS_IMGRIGHT) == 0 ) {
            // Left- or top-aligned image
            POINT pt;
            if( style & ILBS_IMGTOP ) {
               pt.x = rcItem.left + ((rcItem.right - rcItem.left) / 2) - (iImageWidth / 2);
               pt.y = rcItem.top;
            }
            else {
               pt.x = rcItem.left;
               pt.y = rcItem.top + ((rcItem.bottom - rcItem.top) / 2) - (iImageHeight / 2);
            }
            if( bSelected && (pItem->mask & ILBIF_SELIMAGE) ) {
               m_imgSelected.Draw(dc, pItem->iSelImage, pt, ILD_NORMAL);
            }
            else {
               m_imgNormal.Draw(dc, pItem->iImage, pt, ILD_NORMAL);
            }
            if( style & ILBS_IMGTOP ) {
               rcItem.top += iImageHeight;
            }
            else {
               rcItem.left += iImageWidth;
            }
         }
         else {
            // Right-aligned image
            int x = rcItem.right - iImageWidth;
            int y = rcItem.top + ((rcItem.bottom - rcItem.top) / 2) - (iImageHeight / 2);
            if( bSelected && (pItem->mask & ILBIF_SELIMAGE) ) {
               m_imgSelected.Draw(dc, pItem->iSelImage, x,y, ILD_NORMAL);
            }
            else {
               m_imgNormal.Draw(dc, pItem->iImage, x,y, ILD_NORMAL);
            }
            rcItem.right -= iImageWidth;
         }
      }
   }

   void DrawItem(LPDRAWITEMSTRUCT lpdis)
   {
      if( lpdis->itemID==-1 ) return; // If there are no list box items, skip this message. 

      CDCHandle dc(lpdis->hDC);
      PILBITEM pItem = (PILBITEM)lpdis->itemData;
      ATLASSERT(pItem);
      RECT rcItem = lpdis->rcItem;
      bool bSelected = lpdis->itemState & ODS_SELECTED;
      
      UINT style = (pItem->mask & ILBIF_STYLE) != 0 ? pItem->style : ILBS_IMGLEFT|ILBS_SELNORMAL;

      COLORREF clrFront = bSelected ? m_st.clrHighliteText : m_st.clrText;
      COLORREF clrBack = bSelected ? m_st.clrHighlite : m_st.clrBackground;

	  if(useDoubleBuffering && pItem->mask & ILBIF_IMAGE && m_imgNormal.m_hImageList != NULL)
	  {
		CDC iconDC;
		CBitmapHandle oldIconHBitmap;
		CBitmap iconBmp;

		int imgWidth = 0;
		int imgHeight = 0;
		if(m_imgNormal.m_hImageList != NULL)
			m_imgNormal.GetIconSize(imgWidth, imgHeight);


		CWindowDC winDC(NULL);
		iconDC.CreateCompatibleDC(winDC);
		iconBmp.CreateCompatibleBitmap(winDC, imgWidth, imgHeight);
		oldIconHBitmap = iconDC.SelectBitmap(iconBmp);
		
		RECT rcItemCopy = rcItem;
		::InflateRect(&rcItemCopy, -m_st.sizeMargin.cx, -m_st.sizeMargin.cy);
		POINT iconPos = GetImagePt(rcItemCopy, pItem, bSelected, style);

		RECT fillRect = {0, 0, imgWidth, imgHeight};

		iconDC.FillSolidRect(&fillRect, clrBack);

		if( bSelected && (pItem->mask & ILBIF_SELIMAGE))
			m_imgSelected.Draw(iconDC, pItem->iSelImage, 0, 0, ILD_NORMAL);
        else
			m_imgNormal.Draw(iconDC, pItem->iImage, 0, 0, ILD_NORMAL);

		if(iconSize.cx != -1 && iconSize.cy != -1 &&
			iconSize.cx != imgWidth && iconSize.cy != imgHeight)
		{
			dc.SetStretchBltMode(HALFTONE);
			dc.StretchBlt(iconPos.x, iconPos.y, iconSize.cx, iconSize.cy, iconDC, 0, 0, imgWidth, imgHeight, SRCCOPY);
			imgWidth = iconSize.cx;
			imgHeight = iconSize.cy;
		}
		else
		{
			dc.BitBlt(iconPos.x, iconPos.y, imgWidth, imgHeight, iconDC, 0, 0, SRCCOPY);
		}

		ExcludeClipRect(dc, iconPos.x, iconPos.y, imgWidth + iconPos.x, imgHeight + iconPos.y);
		  
		iconDC.SelectBitmap(oldIconHBitmap);

	  }

	  // Prepare draw
      HFONT hOldFont = dc.SelectFont(GetFont());

      if( bSelected && (style & ILBS_SELROUND) ) {
         // Draw round-rect selection
         dc.FillSolidRect(&rcItem, m_st.clrBackground);
         ::InflateRect(&rcItem, -m_st.sizeMargin.cx, -m_st.sizeMargin.cy);
         CPen pen;
         pen.CreatePen(PS_SOLID, 1, m_st.clrHighliteBorder);
         CBrush brush;
         brush.CreateSolidBrush(m_st.clrHighlite);
         HPEN hOldPen = dc.SelectPen(pen);
         HBRUSH hOldBrush = dc.SelectBrush(brush);
         dc.RoundRect(&rcItem, m_st.ptArc);
         dc.SelectBrush(hOldBrush);
         dc.SelectPen(hOldPen);
      }
      else
	  {
        dc.FillSolidRect(&rcItem, clrBack);

        ::InflateRect(&rcItem, -m_st.sizeMargin.cx, -m_st.sizeMargin.cy);
      }
      if( style & ILBS_SELROUND ) ::InflateRect(&rcItem, -m_st.ptArc.x, -m_st.ptArc.y);

      // Get image information
      int iImageWidth = 0, iImageHeight = 0;
      if( pItem->mask & ILBIF_IMAGE && m_imgNormal.m_hImageList ) m_imgNormal.GetIconSize(iImageWidth, iImageHeight);

	  dc.SelectFont(GetFont());
	  if(!useDoubleBuffering)
		dc.SetBkMode(TRANSPARENT);
      dc.SetBkColor(clrBack);
      dc.SetTextColor(clrFront);

	  if(pItem->mask & ILBIF_IMAGE)
	  {
		if(!useDoubleBuffering)
			DrawImageInternal(dc, rcItem, pItem, bSelected, style);
		else
			GetImagePt(rcItem, pItem, bSelected, style);
	  }

      // Prepare text drawing
      UINT format = DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_END_ELLIPSIS;
      if( pItem->mask & ILBIF_FORMAT ) format = pItem->format;

      // Draw text
      if( pItem->mask & ILBIF_TEXT ) {
         ::InflateRect(&rcItem, -m_st.sizeIndent.cx, -m_st.sizeIndent.cy);
         RECT rcText = { 0 };
         dc.DrawText(pItem->pszText, -1, &rcText, format | DT_CALCRECT);
		 rcItem.left += 3; // OFFSET from the image
		 if(pItem->style & ILBS_VCENTERTEXT)
		 {
			 int height = rcItem.bottom - rcItem.top;
			 int mid = height / 2 + rcItem.top;
			 int textHeight = rcText.bottom - rcText.top;
			 if(pItem->pszSubText != NULL && wcslen(pItem->pszSubText) > 0)
				 textHeight = textHeight * 2 + 4;
			 rcItem.top = mid - textHeight / 2;
		 }
         dc.DrawText(pItem->pszText, -1, &rcItem, format);
         rcItem.top += rcText.bottom-rcText.top;
      }
      // Draw subtext
      if( pItem->mask & ILBIF_SUBTEXT ) {
         if( m_hSubFont != NULL ) dc.SelectFont(m_hSubFont);
         ::InflateRect(&rcItem, -m_st.sizeSubIndent.cx, 0);
         rcItem.top += m_st.sizeSubIndent.cy;
         RECT rcText = { 0 };
         dc.DrawText(pItem->pszSubText, -1, &rcText, format | DT_CALCRECT | DT_WORDBREAK | DT_END_ELLIPSIS);
         dc.DrawText(pItem->pszSubText, -1, &rcItem, format | DT_WORDBREAK | DT_END_ELLIPSIS);
         rcItem.top += rcText.bottom-rcText.top;
      }

      dc.SelectFont(hOldFont);
   }
};

class CDeskletListBoxCtrl : public CDeskletListBoxImpl<CDeskletListBoxCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_DeskletListBox"), GetWndClassName())  
};


#endif // !defined(AFX_DeskletListBox_H__20011106_AF04_B15C_D1F1_0080AD509054__INCLUDED_)
