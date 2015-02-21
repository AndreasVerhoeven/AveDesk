// Timer.cpp : Implementierung von CTimer
#include "stdafx.h"
#include "AveDesk.h"
#include "Timer.h"
#include "effectlet.h"

/////////////////////////////////////////////////////////////////////////////
// CTimer


STDMETHODIMP CTimer::get_Enabled(VARIANT_BOOL *pVal)
{
	*pVal = enabled;

	return S_OK;
}

STDMETHODIMP CTimer::put_Enabled(VARIANT_BOOL newVal)
{
	if( newVal != enabled )
  {
    enabled = newVal;
    if( enabled )
    {
      SetTimer(0, interval);
    }
    else
    {
      KillTimer(0);
    }
  }
	return S_OK;
}

STDMETHODIMP CTimer::get_Interval(long *pVal)
{
  *pVal = interval;
	return S_OK;
}

STDMETHODIMP CTimer::put_Interval(long newVal)
{
  if( newVal != interval )
  {
    interval = newVal;
    if( enabled )
    {
      KillTimer(0);
      SetTimer(0, interval);
    }
  }
	return S_OK;
}

void CTimer::Attach(HWND hwnd, DISPID id, IDispatch *target)
{
  RECT rc = {0,0,0,0};
  Create(hwnd, rc, _T("AveDeskEffectLetTimerWindow"));
  dispID = id;
  disp = target;
}

void CTimer::Detach()
{
	
  if (IsWindow())
  {
	KillTimer(0);
	DestroyWindow();
  }

  disp.Release();
}

LRESULT CTimer::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  /*
  static bool inTimer = false;
  if( inTimer )
    return 0;
  inTimer = true;
  */
  disp.Invoke0(dispID);  
  //inTimer = false;
	return 0;
}
