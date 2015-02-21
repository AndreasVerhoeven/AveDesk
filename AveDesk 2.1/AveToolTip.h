#pragma once

class Docklet;

class CAveToolTip
{
protected:
	Docklet* m_Owner;
	HWND m_hWnd;
	SIZE m_parentSize;
	UINT m_CurrentLayerID;
	WCHAR* currentBuffer;

protected:
	virtual void UpdateTooltip(INT x, INT y);
public:
	CAveToolTip();
	virtual ~CAveToolTip();

	virtual BOOL Create(HWND hwndParent, Docklet *owner);
	virtual BOOL Destroy();
	virtual void SetText(const WCHAR* txt);
	virtual void RelayEvent(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	virtual void RelayNotify(HWND hwnd, WPARAM wParam, LPARAM lParam);
	virtual void Hide();

	virtual void OnResizeParent(HWND hwndParent, const SIZE& newSize);
	virtual void OnMoving(HWND hwndParent, const POINT& newPoint);

};
