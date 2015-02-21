// ShadowPropertiesDlg.h: interface for the CShadowPropertiesDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADOWPROPERTIESDLG_H__3A44F303_75F8_4A41_B775_AD4527BBE195__INCLUDED_)
#define AFX_SHADOWPROPERTIESDLG_H__3A44F303_75F8_4A41_B775_AD4527BBE195__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // Hauptsymbole
#include <atlframe.h>

#include "ColorButton.h"

#include "ShadowProperties.h"

class CShadowPropertiesDlg : public CDialogImpl<CShadowPropertiesDlg>  
{
protected:
	CShadowProperties	m_initialProperties;
	CShadowProperties&	m_Properties;

	CColorButton		m_butColor;
	CUpDownCtrl			m_spinAlpha;
	CUpDownCtrl			m_spinBlur;
	CUpDownCtrl			m_spinX;
	CUpDownCtrl			m_spinY;
	CUpDownCtrl			m_spinWidth;
	CUpDownCtrl			m_spinHeight;
	CUpDownCtrl			m_spinIntensity;

public:
	CShadowPropertiesDlg(CShadowProperties& shadowProperties,_U_STRINGorID title);
	~CShadowPropertiesDlg();

	enum { IDD = IDD_NEWSHADOWPROPERTIES };

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	BOOL OnClose ();
	void OnOK(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnCancel(UINT wNotifyCode, int wID, HWND hwndCtrl);

	BEGIN_MSG_MAP_EX(CShadowPropertiesDlg)
		COMMAND_ID_HANDLER_EX(IDOK,OnOK)
		COMMAND_ID_HANDLER_EX(IDCANCEL,OnCancel)
		
        MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
        //CHAIN_MSG_MAP(CDialogImpl<CShadowPropertiesDlg>)
		REFLECT_NOTIFICATIONS()
    END_MSG_MAP();

};

#endif // !defined(AFX_SHADOWPROPERTIESDLG_H__3A44F303_75F8_4A41_B775_AD4527BBE195__INCLUDED_)
