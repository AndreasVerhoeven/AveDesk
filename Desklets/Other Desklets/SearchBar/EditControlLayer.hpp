#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include "../DeskletSDK.h"
#include "../DeskletSDK.cpp"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")


#ifndef __ATLBASE_H__
#include <ATLBASE.h>
#endif

#ifndef __ATLCONV_H__
#include <ATLCONV.H>
#endif

extern CComModule _Module;
#include <atlcom.h>

#include <richedit.h>
#include <textserv.h>
#pragma comment(lib,"riched20.lib")

using namespace Gdiplus;

const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
    0x8d33f740,
    0xcf58,
    0x11ce,
    {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
  };

const IID IID_ITextHost = { // c5bdd8d0-d26e-11ce-a89e-00aa006cadc5
    0xc5bdd8d0,
    0xd26e,
    0x11ce,
    {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
  };

class CEditControlLayer : public CComObjectRoot, public ITextHost
{
	BEGIN_COM_MAP(CEditControlLayer)
		COM_INTERFACE_ENTRY(ITextHost)
	END_COM_MAP( )

public:

	//@cmember Get the DC for the host
	virtual HDC 		TxGetDC() = 0;

	//@cmember Release the DC gotten from the host
	virtual INT			TxReleaseDC(HDC hdc) = 0;
	
	//@cmember Show the scroll bar
	virtual BOOL 		TxShowScrollBar(INT fnBar, BOOL fShow) = 0;

	//@cmember Enable the scroll bar
	virtual BOOL 		TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags) = 0;

	//@cmember Set the scroll range
	virtual BOOL 		TxSetScrollRange(
							INT fnBar,
							LONG nMinPos,
							INT nMaxPos,
							BOOL fRedraw) = 0;

	//@cmember Set the scroll position
	virtual BOOL 		TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw) = 0;

	//@cmember InvalidateRect
	virtual void		TxInvalidateRect(LPCRECT prc, BOOL fMode) = 0;

	//@cmember Send a WM_PAINT to the window
	virtual void 		TxViewChange(BOOL fUpdate) = 0;
	
	//@cmember Create the caret
	virtual BOOL		TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight) = 0;

	//@cmember Show the caret
	virtual BOOL		TxShowCaret(BOOL fShow) = 0;

	//@cmember Set the caret position
	virtual BOOL		TxSetCaretPos(INT x, INT y) = 0;

	//@cmember Create a timer with the specified timeout
	virtual BOOL 		TxSetTimer(UINT idTimer, UINT uTimeout) = 0;

	//@cmember Destroy a timer
	virtual void 		TxKillTimer(UINT idTimer) = 0;

	//@cmember Scroll the content of the specified window's client area
	virtual void		TxScrollWindowEx (
							INT dx,
							INT dy,
							LPCRECT lprcScroll,
							LPCRECT lprcClip,
							HRGN hrgnUpdate,
							LPRECT lprcUpdate,
							UINT fuScroll) = 0;

	//@cmember Get mouse capture
	virtual void		TxSetCapture(BOOL fCapture) = 0;

	//@cmember Set the focus to the text window
	virtual void		TxSetFocus() = 0;

	//@cmember Establish a new cursor shape
	virtual void 	TxSetCursor(HCURSOR hcur, BOOL fText) = 0;

	//@cmember Converts screen coordinates of a specified point to the client coordinates
	virtual BOOL 		TxScreenToClient (LPPOINT lppt) = 0;

	//@cmember Converts the client coordinates of a specified point to screen coordinates
	virtual BOOL		TxClientToScreen (LPPOINT lppt) = 0;

	//@cmember Request host to activate text services
	virtual HRESULT		TxActivate( LONG * plOldState ) = 0;

	//@cmember Request host to deactivate text services
   	virtual HRESULT		TxDeactivate( LONG lNewState ) = 0;

	//@cmember Retrieves the coordinates of a window's client area
	virtual HRESULT		TxGetClientRect(LPRECT prc) = 0;

	//@cmember Get the view rectangle relative to the inset
	virtual HRESULT		TxGetViewInset(LPRECT prc) = 0;

	//@cmember Get the default character format for the text
	virtual HRESULT 	TxGetCharFormat(const CHARFORMATW **ppCF ) = 0;

	//@cmember Get the default paragraph format for the text
	virtual HRESULT		TxGetParaFormat(const PARAFORMAT **ppPF) = 0;

	//@cmember Get the background color for the window
	virtual COLORREF	TxGetSysColor(int nIndex) = 0;

	//@cmember Get the background (either opaque or transparent)
	virtual HRESULT		TxGetBackStyle(TXTBACKSTYLE *pstyle) = 0;

	//@cmember Get the maximum length for the text
	virtual HRESULT		TxGetMaxLength(DWORD *plength) = 0;

	//@cmember Get the bits representing requested scroll bars for the window
	virtual HRESULT		TxGetScrollBars(DWORD *pdwScrollBar) = 0;

	//@cmember Get the character to display for password input
	virtual HRESULT		TxGetPasswordChar(TCHAR *pch) = 0;

	//@cmember Get the accelerator character
	virtual HRESULT		TxGetAcceleratorPos(LONG *pcp) = 0;

	//@cmember Get the native size
    virtual HRESULT		TxGetExtent(LPSIZEL lpExtent) = 0;

	//@cmember Notify host that default character format has changed
	virtual HRESULT 	OnTxCharFormatChange (const CHARFORMATW * pcf) = 0;

	//@cmember Notify host that default paragraph format has changed
	virtual HRESULT		OnTxParaFormatChange (const PARAFORMAT * ppf) = 0;

	//@cmember Bulk access to bit properties
	virtual HRESULT		TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) = 0;

	//@cmember Notify host of events
	virtual HRESULT		TxNotify(DWORD iNotify, void *pv) = 0;

	// Far East Methods for getting the Input Context
//#ifdef WIN95_IME
	virtual HIMC		TxImmGetContext() = 0;
	virtual void		TxImmReleaseContext( HIMC himc ) = 0;
//#endif

	//@cmember Returns HIMETRIC size of the control bar.
	virtual HRESULT		TxGetSelectionBarWidth (LONG *lSelBarWidth) = 0;
};