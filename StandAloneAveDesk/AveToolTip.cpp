#include "stdafx.h"
#include "AveToolTip.h"
#include "Docklet.h"

CAveToolTip::CAveToolTip() :
	m_hWnd(NULL),
	m_CurrentLayerID((UINT)-1),
	currentBuffer(NULL)
{
	m_parentSize.cx = 0;
	m_parentSize.cy = 0;
}

CAveToolTip::~CAveToolTip()
{
	Destroy();
	delete[] currentBuffer;
	currentBuffer = NULL;
}

BOOL CAveToolTip::Create(HWND hwndParent, Docklet* owner)
{
	m_Owner = owner;

	m_hWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW, TOOLTIPS_CLASS, NULL,
                           TTS_NOPREFIX | TTS_ALWAYSTIP,
                           0, 0, 0, 0,
                           hwndParent, NULL, _Module.GetModuleInstance(), NULL);

	if(m_hWnd != NULL)
	{
		TOOLINFO ti = { sizeof(ti) };
		ti.uFlags = TTF_TRANSPARENT;
		ti.hwnd = hwndParent;
		ti.uId = 0;
		//ti.lpszText = TEXT("Placeholder tooltip");
		ti.lpszText = LPSTR_TEXTCALLBACK;
		SetRectEmpty(&ti.rect);
		::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
	}
	
	return m_hWnd != NULL;
}

BOOL CAveToolTip::Destroy()
{
	if(::IsWindow(m_hWnd))
		::DestroyWindow(m_hWnd);

	m_hWnd = NULL;

	return TRUE;
}

void CAveToolTip::SetText(const WCHAR* txt)
{
// GetToolTipTextForLayer
}

void CAveToolTip::UpdateTooltip(INT x, INT y)
{
	if(NULL == m_Owner)
		return;

	docklets::OdDockletInstance* inst = m_Owner->GetDocklet();
	if(NULL == inst)
		return;

	POINT pt = {x,y};
	UINT layerID = inst->HitTestLayers(pt, inst->GetSize(), true, NULL);
	if(layerID != m_CurrentLayerID)
		Hide();

	m_CurrentLayerID = layerID;
}

void CAveToolTip::RelayNotify(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	int idCtrl = (int) wParam; 
	LPNMHDR pnmh = (LPNMHDR) lParam; 

 if (pnmh->hwndFrom == m_hWnd)
 {
	switch (pnmh->code)
	{
		case TTN_GETDISPINFO:
		{
			NMTTDISPINFO *pdi = (NMTTDISPINFO *)pnmh;

			::SendMessage(pnmh->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);

			if(NULL == m_Owner)
				pdi->szText[0] = TEXT('\0');
			else
			{
				docklets::OdDockletInstance* inst = m_Owner->GetDocklet();
				if(NULL == inst)
					pdi->szText[0] = TEXT('\0');
				else
				{
					std::wstring ttText = inst->GetToolTipTextForLayer(m_CurrentLayerID);
					delete[] currentBuffer;
					currentBuffer = new WCHAR[ttText.length() + 2];
					wcscpy_s(currentBuffer, ttText.length()+2, ttText.c_str());
					pdi->lpszText = currentBuffer;
				}
			}
		}
		break;
	}
  }
}

void CAveToolTip::RelayEvent(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	UpdateTooltip(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	MSG msg;
	msg.hwnd = hwnd;
	msg.message = uiMsg;
	msg.wParam = wParam;
	msg.lParam = lParam;
	SendMessage(m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)&msg);
}

void CAveToolTip::Hide()
{
	SendMessage(m_hWnd, TTM_POP, 0, 0);
}

void CAveToolTip::OnResizeParent(HWND hwndParent, const SIZE& newSize)
{
	if(m_parentSize.cx != newSize.cx || m_parentSize.cy != newSize.cy)
	{
		TOOLINFO ti ={ sizeof(ti) };
		ti.hwnd = hwndParent;
		ti.uId = 0;
		::SetRect(&ti.rect, 0, 0, newSize.cx, newSize.cy);
		//::GetClientRect(hwnd, &ti.rect);
		SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);

		m_parentSize = newSize;
	}
}

void CAveToolTip::OnMoving(HWND hwndParent, const POINT& newPoint)
{

}
