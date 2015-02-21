// AboutDlg.h: interface for the CAboutDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__0939E47E_A2FC_4008_92DC_4C6A69535E99__INCLUDED_)
#define AFX_ABOUTDLG_H__0939E47E_A2FC_4008_92DC_4C6A69535E99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <memory>
#include "WaterAnimation.h"

class CAboutDlg : public CDialogImpl<CAboutDlg>  
{
private:
	boost::shared_ptr<Bitmap> m_Image;
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUT };

	CHyperLink m_Ave, m_Herd, m_KoL;
	CStatic m_Icon;
	boost::shared_ptr<CWaterAnimation> m_Anim;
	boost::shared_ptr<Bitmap> m_Bmp;
	BitmapData m_Data;

BEGIN_MSG_MAP(CAboutDlg)
	// dialog stuff
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_CLOSE, OnCloseDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLeftButtonDoubleClick)
    

	COMMAND_ID_HANDLER(IDOK, OnCloseButtonClick)

	// other stuff
    REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCloseDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonDoubleClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnCloseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);



};

#endif // !defined(AFX_ABOUTDLG_H__0939E47E_A2FC_4008_92DC_4C6A69535E99__INCLUDED_)
