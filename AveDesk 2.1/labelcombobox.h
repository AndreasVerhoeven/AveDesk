#ifndef LABEL_COMBOBOX_AVEDESK__13__
#define LABEL_COMBOBOX_AVEDESK__13__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CImageListBox - A ListBox control with that extra touch
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
  #error ImageListBox.h requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error ImageListBox.h requires atlctrls.h to be included first
#endif
   
#define FADE_IN_TIMER_LABELCOMBOBOX 4349485

template< class T, class TBase = CComboBox, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CLabelComboBoxImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public COwnerDraw< CLabelComboBoxImpl<T> >
{
protected:
	CImageList m_ImageList;
	bool m_IsLoading;
	UINT m_LoadingProgress;
	CString m_LoadingMessage;
	UINT m_FadeInTicker;
	bool m_IsFadingIn;

public:
   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

	BEGIN_MSG_MAP(CLabelComboBoxImpl)
      //MESSAGE_HANDLER(WM_CREATE, OnCreate)
      //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      //MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
	  MESSAGE_HANDLER(WM_TIMER,OnTimer);
      CHAIN_MSG_MAP_ALT(COwnerDraw<CLabelComboBoxImpl>, 1)
      DEFAULT_REFLECTION_HANDLER()
   END_MSG_MAP()

   CLabelComboBoxImpl() :
	m_IsLoading(false),
	m_LoadingProgress(0),
	m_LoadingMessage(_T("loading...")),
	m_FadeInTicker(0),
	m_IsFadingIn(false)
   {

   }


   void StopLoadingProgressAndFadeIn()
   {
	   m_FadeInTicker = 0;
	   m_IsFadingIn   = true;
	   m_IsLoading = false;
	   SetTimer(FADE_IN_TIMER_LABELCOMBOBOX, 10);
   }

   LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
		//if(wParam == FADE_IN_TIMER_LABELCOMBOBOX)
		{
			m_FadeInTicker += 10;

			if(m_FadeInTicker >= 255)
			{
				KillTimer(FADE_IN_TIMER_LABELCOMBOBOX);
				m_IsFadingIn = false;
				EnableWindow(TRUE);
			}
			InvalidateRect(NULL,FALSE);
		}

		return 0;
   }

   void SetImageList(HIMAGELIST imageList)
   {
		m_ImageList = imageList;
   }

   void SetIsLoading(bool isLoading)
   {
	   if(isLoading != m_IsLoading)
	   {
			m_IsLoading = isLoading;
			InvalidateRect(NULL,FALSE);
	   }
   }

   void SetLoadingMessage(const CString& message)
   {
		m_LoadingMessage = message;
		if(m_isLoading)
			InvalidateRect(NULL,FALSE);;
   }

   void SetLoadingProgress(UINT progress)
   {
		m_LoadingProgress = progress;
		if(m_LoadingProgress > 100)
			m_LoadingProgress = 100;

		if(m_IsLoading)
			InvalidateRect(NULL,FALSE);
   }

   void DrawItem(LPDRAWITEMSTRUCT lpdis)
   {
	    RECT rcItem = lpdis->rcItem;
		SIZE itemSize = { rcItem.right - rcItem.left, rcItem.bottom - rcItem.top };

        bool bSelected = lpdis->itemState & ODS_SELECTED;

		COLORREF clrText = ::GetSysColor(COLOR_WINDOWTEXT);
        COLORREF clrBackground = ::GetSysColor(COLOR_WINDOW);
        COLORREF clrHighliteText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
        COLORREF clrHighlite = ::GetSysColor(COLOR_HIGHLIGHT);
        COLORREF clrHighliteBorder = ::GetSysColor(COLOR_HIGHLIGHT);

		COLORREF clrFront = bSelected ? clrHighliteText : clrText;
		COLORREF clrBack = bSelected ? clrHighlite : clrBackground;

		CDCHandle dc(lpdis->hDC);

		dc.SetBkMode(TRANSPARENT);
		dc.SetBkColor(clrBack);
		dc.SetTextColor(clrFront);

		if(m_IsFadingIn)
		{
			
			SIZE iconSize = {0};
			m_ImageList.GetIconSize(iconSize);

			CWindowDC windowDC(NULL);

			CBitmap bitmap = CreateCompatibleBitmap(windowDC, iconSize.cx, iconSize.cy);
			CDC alphaDc;
			alphaDc.CreateCompatibleDC(windowDC);
			HBITMAP oldBitmap = alphaDc.SelectBitmap(bitmap);
			RECT rc = {0,0, iconSize.cx, iconSize.cy};
			//alphaDc.FillSolidRect(&rc,clrBack);

			int index =lpdis->itemID -1;

			index = (index * 2);
			if(bSelected)
				index += 1;

			POINT pt = {0,0};
			m_ImageList.Draw(alphaDc, index, pt, ILD_NORMAL);

			POINT pt2 = {rcItem.left, (itemSize.cy - iconSize.cy) / 2 + rcItem.top};
			

			BLENDFUNCTION blendFunction = {0};
			blendFunction.BlendOp = AC_SRC_OVER;
			blendFunction.BlendFlags = 0;
			blendFunction.SourceConstantAlpha = (BYTE)m_FadeInTicker;
			blendFunction.AlphaFormat = AC_SRC_ALPHA;

			BOOL bRes = AlphaBlend(dc, pt2.x, pt2.y, iconSize.cx, iconSize.cy, alphaDc, 0, 0, iconSize.cx, iconSize.cy,blendFunction);
			
			//dc.FillSolidRect(&rcItem, RGB(255-m_FadeInTicker,0,0));
			//BitBlt(dc, pt.x, pt.y, iconSize.cx, iconSize.cy, alphaDc, 0, 0, SRCPAINT);



			alphaDc.SelectBitmap(oldBitmap);
			
		}
		else if(m_IsLoading)
		{

			/*
			rcItem.top += 2;
			//rcItem.bottom -= 2;
			rcItem.bottom = rcItem.top + 1;
			int progressWidth = (itemSize.cx * m_LoadingProgress) / 100;
			RECT rect = rcItem;
			if(m_LoadingProgress != 100)
				rect.right  = progressWidth;
			dc.FillSolidRect(&rect, clrHighlite);
			*/

			rcItem.right = rcItem.left + 1;
			int progressHeight = (itemSize.cy * m_LoadingProgress) /100;
			RECT rect = rcItem;
			//rect.bottom = progressHeight;
			rect.top = rect.bottom - progressHeight;
			dc.FillSolidRect(&rect, clrHighlite);

			dc.SetTextColor(clrHighliteText);
			dc.SetROP2(R2_MERGEPEN);


		}
		else
		{

			if(lpdis->itemID <= 0)
			{
				dc.FillSolidRect(&rcItem,clrBack);
			}
			else
			{
				SIZE iconSize = {0};
				m_ImageList.GetIconSize(iconSize);


				dc.FillSolidRect(&rcItem,clrBack);
				POINT pt = {rcItem.left, (itemSize.cy - iconSize.cy) / 2 + rcItem.top};
				
				int index =lpdis->itemID -1;

				index = (index * 2);
				if(bSelected)
					index += 1;

				m_ImageList.Draw(dc, index, pt, ILD_NORMAL);
			}
		}
   }

};

class CLabelComboBoxCtrl : public CLabelComboBoxImpl<CLabelComboBoxCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_AveDeskLabelComboBox"), GetWndClassName())  
};


#endif