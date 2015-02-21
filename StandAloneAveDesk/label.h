// filename:		label.h
// Description:		Definition of class Label
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!


#ifndef LABEL_H__200101001
#define LABEL_H__200101001

#include <windows.h>
#include <gdiplus.h>
#include "labelbackground.h"

#include <memory>

#include "ShadowProperties.h"

#include "IPersistable.h"

#include "LabelProperties.h"

class Docklet;

#define USE_GDIPLUS
#include "LayeredWindowImpl.hpp"

#define LABELPROPERTY(t,n) private: ##t m_##n; \
public:	void Set##n (##t const & v) { m_##n = v; }; \
		##t& Get##n () {return m_##n;}

class Label : public IPersistable,
	public CLayeredWindowImpl<Label>
{
private:
	Docklet* m_DockletParent;

	boost::shared_ptr<Bitmap> m_CachedBitmap;
	

	LABELPROPERTY(CLabelProperties,MainLabelProperties);
	LABELPROPERTY(CLabelProperties,SubLabelProperties);
	LABELPROPERTY(BYTE,Alpha);
	LABELPROPERTY(UINT,Rotation);
	LABELPROPERTY(int,VAlign);
	LABELPROPERTY(int,HAlign);
	LABELPROPERTY(UINT,Width);
	LABELPROPERTY(UINT,Height);
	LABELPROPERTY(int,Margin);
	LABELPROPERTY(int,Position);
	LABELPROPERTY(bool,DrawTopDown);
	LABELPROPERTY(bool,ShowSubLabel);
	LABELPROPERTY(LabelBackground*,Bg);
	LABELPROPERTY(std::wstring,MainCaption);
	LABELPROPERTY(std::wstring,SubCaption);
	LABELPROPERTY(bool,FadingIn);
	LABELPROPERTY(bool,IsDirty);

public:

	DECLARE_PERSISTANCYNAME(Label);

	virtual HRESULT Load(ILoader& loader);
	virtual HRESULT Save(ISaver& saver);

	Label(Docklet* d=NULL);
	~Label();

	HWND SetupWindow(HWND hWndParent);
	void UpdateLabel(UINT Quality, BYTE Alpha=255, bool sel = false);


	void ReadFromIni(const char* group, const char* ini);
	void WriteToIni(const char* group, const char* ini);

	void Draw(Graphics *graphics, bool sel = false);
	void DrawTopDown(Graphics *graphics);

	void SetDockletParent(Docklet* d)
	{
		m_DockletParent = d;
	};

	Bitmap* GetCachedBitmap();

	void UpdateAlphaOnly(BYTE b);

	BEGIN_MSG_MAP(Label)

		// message handlers
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLeftButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRightButtonUp)

		//MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		//MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLeftButtonDoubleClick)
		//MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		//MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		//MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		//MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		//MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING,OnWindowsPosChanging )
		MESSAGE_HANDLER(WM_TIMER,OnTimer )

		// other stuff
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CLayeredWindowImpl<Label>)
	END_MSG_MAP()

	LRESULT OnWindowsPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRightButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonDoubleClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool NeedsRedrawOnSelect()const;
};

#endif//LABEL_H__200101001
