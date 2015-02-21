// ShowCase1.h: Definition of the CShowCase class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWCASE1_H__08D6CF7A_C642_46E8_A860_CF57C3EDF387__INCLUDED_)
#define AFX_SHOWCASE1_H__08D6CF7A_C642_46E8_A860_CF57C3EDF387__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include "stdafx.h"

#include "avedesk.h"

#include <memory>

#include <gdiplus.h>
using namespace Gdiplus;

#include <vector>
class Docklet;

#include "IPersistable.h"

/////////////////////////////////////////////////////////////////////////////
// CShowCase

class HotKey : public IPersistable
{
public:
	HotKey() :
	  id(0), modifiers(0), vk(0)
	{

	}

	UINT id;
	LONG modifiers;
	LONG vk;

	DECLARE_PERSISTANCYNAME(HotKey);

	BEGIN_LOADING_MAP()
		LOADABLE_PRIMITIVE(vk);
		LOADABLE_PRIMITIVE(modifiers);
	END_LOADING_MAP()

	BEGIN_SAVING_MAP()
		SAVABLE_PRIMITIVE(vk);
		SAVABLE_PRIMITIVE(modifiers);
	END_SAVING_MAP()
};

class CShowCase : 
	public IDispatchImpl<IShowCase, &IID_IShowCase, &LIBID_AveDeskLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CShowCase,&CLSID_ShowCase>,
	public CLayeredWindowImpl<CShowCase>,
	public IPersistable
{
private:
	//const int HOTKEYID = 304293;

	VARIANT_BOOL	doAnimation;
	LONG			numOfAnimFrames;
	LONG			maxAlpha;
	COLORREF		color;
	VARIANT_BOOL	isActive;
	VARIANT_BOOL	isEnabled;
	VARIANT_BOOL    isClickThru;
	bool            m_IsActiveOnAllDesklets;

	// privately used stuff
	boost::shared_ptr<HotKey> normalHotKey;
	boost::shared_ptr<HotKey> allDeskletsHotKey;
	BOOL hotKeysAreRegistered;
	BOOL isAnimating;

	boost::shared_ptr<Bitmap> backGroundBuffer;

	enum DrawState { DrawNone, DrawShowCaseOnly, DrawAllDesklets, DrawFadeOut };
	DrawState m_DrawState;

	void ShowOverlayWindow(bool doAnimation);
	void HideOverlayWindow(bool doAnimation);

	void RecreateBuffer(DrawState m_DrawState,BYTE bAlpha);


	int animFrame;

	std::vector<HWND>hwnds;
	std::vector<BYTE> alphas;
	std::vector<Docklet*> docklets;

public:
	CShowCase();
	~CShowCase();

	HWND GetHWND()const{return m_hWnd;};

BEGIN_COM_MAP(CShowCase)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShowCase)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


	BOOL OnHotKey(UINT id, UINT modifiers, UINT vk); 

	BEGIN_MSG_MAP_EX(CShowCase)
		MSG_WM_ERASEBKGND(OnEraseBackground);
		MSG_WM_PAINT(OnPaint)
        MSG_WM_HOTKEY(OnHotKey)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowsPosChanging)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
        CHAIN_MSG_MAP(CLayeredWindowImpl<CShowCase>)
    END_MSG_MAP()

	void OnPaint(HDC dc);
	LRESULT OnEraseBackground(HDC hdc);
	LRESULT OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWindowsPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

//DECLARE_NOT_AGGREGATABLE(CShowCase) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

//DECLARE_REGISTRY_RESOURCEID(IDR_ShowCase)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IShowCase
public:
	STDMETHOD(get_IsClickThru)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_IsClickThru)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Enabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Enabled)(/*[in]*/ VARIANT_BOOL newVal);
	BOOL SetNormalHotKey(HotKey& hotKey, BOOL reRegister=FALSE);
	BOOL SetAllDeskletsHotKey(HotKey& hotKey, BOOL reRegister=FALSE);
	HotKey GetNormalHotKey()const;
	HotKey GetAllDeskletsHotKey()const;

	BOOL RegisterHotKeys();
	BOOL UnregisterHotKeys();

	BOOL GetHotKeysAreRegistered();

	bool GetIsActiveOnAllDesklets() { return m_IsActiveOnAllDesklets;};


	STDMETHOD(ActivateOnAllDesklets)();
	STDMETHOD(get_Color)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_Color)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_DoAnimation)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DoAnimation)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_NumberOfAnimationFrames)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NumberOfAnimationFrames)(/*[in]*/ long newVal);
	STDMETHOD(get_MaximumAlpha)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MaximumAlpha)(/*[in]*/ long newVal);
	STDMETHOD(get_Active)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Active)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(Deactivate)();
	STDMETHOD(Activate)();

	DECLARE_PERSISTANCYNAME(ShowCase);

	BEGIN_LOADING_MAP()
		LOADABLE_PRIMITIVE(isEnabled);
		LOADABLE_PRIMITIVE(doAnimation);
		LOADABLE_PRIMITIVE(numOfAnimFrames);
		LOADABLE_PRIMITIVE(maxAlpha);
		LOADABLE_PRIMITIVE(color);
		LOADABLE_PRIMITIVE(isClickThru);
		LOADABLE_CLASS_NAMED(*normalHotKey,normalHotKey);
		LOADABLE_CLASS_NAMED(*allDeskletsHotKey,allDeskletsHotKey);
	END_LOADING_MAP()

	BEGIN_SAVING_MAP()
		SAVABLE_PRIMITIVE(isEnabled);
		SAVABLE_PRIMITIVE(doAnimation);
		SAVABLE_PRIMITIVE(numOfAnimFrames);
		SAVABLE_PRIMITIVE(maxAlpha);
		SAVABLE_PRIMITIVE(color);
		SAVABLE_PRIMITIVE(isClickThru);
		SAVABLE_CLASS_NAMED(*normalHotKey,normalHotKey);
		SAVABLE_CLASS_NAMED(*allDeskletsHotKey,allDeskletsHotKey);
	END_SAVING_MAP()
};

#endif // !defined(AFX_SHOWCASE1_H__08D6CF7A_C642_46E8_A860_CF57C3EDF387__INCLUDED_)
