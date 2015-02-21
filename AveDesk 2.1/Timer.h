// Timer.h : Deklaration von CTimer

#ifndef __TIMER_H_
#define __TIMER_H_

#include "resource.h"       // Hauptsymbole
/////////////////////////////////////////////////////////////////////////////
// CTimer
class ATL_NO_VTABLE CTimer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ITimer, &IID_ITimer, &LIBID_AveDeskLib>,
  public CWindowImpl<CTimer, CWindow, CControlWinTraits>
{
public:
  CTimer() : 
      enabled(VARIANT_FALSE),
      interval(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TIMER)
DECLARE_NOT_AGGREGATABLE(CTimer)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTimer)
	COM_INTERFACE_ENTRY(ITimer)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_MSG_MAP(CTimer)
MESSAGE_HANDLER(WM_TIMER, OnTimer)
END_MSG_MAP()


// ITimer
public:
	void Detach();
	void Attach(HWND hwnd, DISPID id, IDispatch *target);
	STDMETHOD(get_Interval)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Interval)(/*[in]*/ long newVal);
	STDMETHOD(get_Enabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Enabled)(/*[in]*/ VARIANT_BOOL newVal);

private:
  VARIANT_BOOL enabled;
  long interval;
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  CComDispatchDriver disp;
  DISPID dispID;
};

#endif //__TIMER_H_
