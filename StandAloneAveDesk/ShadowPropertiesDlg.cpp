// ShadowPropertiesDlg.cpp: implementation of the CShadowPropertiesDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShadowPropertiesDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadowPropertiesDlg::CShadowPropertiesDlg(CShadowProperties& shadowProperties,_U_STRINGorID title) :
	CDialogImpl<CShadowPropertiesDlg>(),
	m_initialProperties(shadowProperties),
	m_Properties(shadowProperties)
{
	CString str;
	str.LoadString(IDS_MAINLABELSHADOWDLGTITLE);
	//SetWindowText(str);
}

CShadowPropertiesDlg::~CShadowPropertiesDlg()
{

}

BOOL CShadowPropertiesDlg::OnInitDialog (HWND hwndFocus, LPARAM lParam)
{
	CenterWindow();


	m_butColor.SubclassWindow(GetDlgItem(IDSETCOLOR));
	m_butColor.SetCustomText(IDS_MORECOLORS);
	m_butColor.SetDefaultText(IDS_CURRENTCOLOR);

	COLORREF cr = m_Properties.GetColor(). ToCOLORREF();
	m_butColor.SetDefaultColor(cr );
	m_butColor.SetColor(cr );

	m_spinAlpha = GetDlgItem(IDC_ALPHASPIN);
	m_spinAlpha.SetRange(0,255);
	m_spinAlpha.SetPos(m_Properties.GetColor().GetA() );

	m_spinBlur = GetDlgItem(IDC_BLURSPIN);
	m_spinBlur.SetRange(0,40);
	m_spinBlur.SetPos((int)(m_Properties.GetBlurRatio() * 10.0f) );

	m_spinX = GetDlgItem(IDC_XSPIN);
	m_spinX.SetRange(-100,100);
	m_spinX.SetPos(m_Properties.GetXOffset() );

	m_spinY = GetDlgItem(IDC_YSPIN);
	m_spinY.SetRange(-100,100);
	m_spinY.SetPos(m_Properties.GetYOffset() );

	m_spinWidth = GetDlgItem(IDC_WIDTHSPIN);
	m_spinWidth.SetRange(-100,100);
	m_spinWidth.SetPos(m_Properties.GetExtraWidth() );


	m_spinHeight = GetDlgItem(IDC_HEIGHTSPIN);
	m_spinHeight.SetRange(-100,100);
	m_spinHeight.SetPos(m_Properties.GetExtraHeight() );

	m_spinIntensity = GetDlgItem(IDC_SPININTENSITY);
	m_spinIntensity.SetRange(0,100);
	m_spinIntensity.SetPos( m_Properties.GetIntensity() );

	return TRUE;
}

void CShadowPropertiesDlg::OnCancel(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	OnClose();
}

BOOL CShadowPropertiesDlg::OnClose ()
{
	// on cancel or close, reset the original properties back, so we only
	// actually change options if the [OK] button is pressed.
	m_Properties = m_initialProperties;

	EndDialog(IDCANCEL);
	return TRUE;
}

void CShadowPropertiesDlg::OnOK(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	COLORREF cr = m_butColor.GetColor();
	Color c((BYTE)m_spinAlpha.GetPos(), GetRValue(cr), GetGValue(cr), GetBValue(cr) );
	m_Properties.SetColor(c);

	m_Properties.SetBlurRatio( static_cast<float>(m_spinBlur.GetPos() ) / 10.0f );

	m_Properties.SetXOffset( m_spinX.GetPos() );
	m_Properties.SetYOffset( m_spinY.GetPos() );
	m_Properties.SetExtraWidth( m_spinWidth.GetPos() );
	m_Properties.SetExtraHeight( m_spinHeight.GetPos() );
	m_Properties.SetIntensity( m_spinIntensity.GetPos() );

	EndDialog(IDOK);
}
