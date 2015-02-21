// CloseButton.h: interface for the CCloseButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLOSEBUTTON_H__5A99B04E_67D7_4FFC_81A2_06A77148D1E5__INCLUDED_)
#define AFX_CLOSEBUTTON_H__5A99B04E_67D7_4FFC_81A2_06A77148D1E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "layeredwindowimpl.hpp"

class Docklet;
class CCloseButtonPolicy;

class CCloseButton  : public CLayeredWindowImpl<CCloseButton>
{
protected:
	Docklet* m_Docklet;
	Bitmap* m_Bitmap;
	CCloseButtonPolicy* m_Policy;
	HWND m_HiddenParentHwnd;
	BYTE m_Alpha;
	bool m_ButtonIsVisible;
	bool m_MouseIsOver;
	bool m_IsFadingIn;
	bool m_DonotHide;

	ULONG m_RefCount;
	CCloseButton();
	~CCloseButton();

	void SetupWindow();
public:
	
		BEGIN_MSG_MAP(Docklet)
		// message handlers
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLeftButtonDown)
		MESSAGE_HANDLER(WM_TIMER, OnTimer);

		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED , OnWindowsPosChanged)
		//MESSAGE_HANDLER(WM_NCDESTROY,OnDestroy )
		// other stuff

		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CLayeredWindowImpl<CCloseButton>)
	END_MSG_MAP()

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWindowsPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static CCloseButton* CreateCloseButton(HWND hiddenParent);

	ULONG AddRef();
	ULONG Release();

	void BindDocklet(Docklet* d);
	void UnbindDocklet();
	void NotifyDeskletHide();
	void NotifyChangedClosePoint();
	void ChangeAlpha(BYTE bAlpha);

	void ResetPosition();
	void Show();
	void Hide();

	void NotifyZOrderingChange(HWND zOrder, HWND parent);

	void SetDonotHide(bool val);
	bool GetDonotHide()const;
};

#endif // !defined(AFX_CLOSEBUTTON_H__5A99B04E_67D7_4FFC_81A2_06A77148D1E5__INCLUDED_)
