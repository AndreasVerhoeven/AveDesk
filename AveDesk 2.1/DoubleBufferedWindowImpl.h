#ifndef DoubleBufferedWindowImpl_349494949_H__
#define DoubleBufferedWindowImpl_349494949_H__

#pragma once

#include "atlgdix.h"

template<typename Base> CDoubleBufferedWindowImpl :
	public CWindow<Base>
{
public:
	BEGIN_MSG_MAP(CDoubleBufferedWindowImpl)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP_ALT(CWindow<CDoubleBufferedWindowImpl>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	CMemDC CreateSuitableDC(HDC hdc)
	{
		return dc(hdc);
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1; // we deal with this one
	}


	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{

		BOOL bHandled = FALSE;
		T* pT = static_cast<T*>(this);
		if( wParam != NULL )
		{
			 CMemDC memdc( (HDC) wParam, NULL );
			 t->OnEraseBackground(WM_ERASEBKGND, (WPARAM)memDc, 0L);
			pT->DoPaint(memdc.m_hDC);
		}
		else
		{
			RECT rc;
			pT->GetClientRect(&rc);
			CPaintDC dc(pT->m_hWnd);
			CMemDC memdc(dc.m_hDC, &rc);
			pT->DoPaint(memdc.m_hDC);
		}
		return 0;
	}

};

#endif//DoubleBufferedWindowImpl_349494949_H__