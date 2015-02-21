// filename:		DefaultDeskletLabelSheet.cpp
// Description:		Implementation of class CDefaultDeskletLabelSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "DefaultDeskletLabelSheet.hpp"
#include "Docklet.h"
#include "AveDesk.h"
#include "Label.h"
#include "ConfigShadowWindow.hpp"
#include "ConfigSubShadowWindow.hpp"

CDefaultDeskletLabelSheet::CDefaultDeskletLabelSheet(_U_STRINGorID title) :
	CPropertyPageImpl<CDefaultDeskletLabelSheet>(title), initMode(true)
{
}

CDefaultDeskletLabelSheet::~CDefaultDeskletLabelSheet()
{

}

void CDefaultDeskletLabelSheet::FillLabelBackgroundComboBox()
{
	WCHAR path[MAX_PATH];
	GetDLLPathW(_Module.GetModuleInstance(),path,sizeof(path)/sizeof(path[0]));
	wcscat(path,L"Labels\\*.*");

	WIN32_FIND_DATA data;

	HANDLE h = FindFirstFileW(path,&data);

	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(wcscmp(data.cFileName,L".")!=0 && wcscmp(data.cFileName,L"..")!=0)
				m_comboBackground.AddString(data.cFileName);
		}while(FindNextFileW(h,&data));

		FindClose(h);
	}
}

BOOL CDefaultDeskletLabelSheet::OnInitDialog (HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	m_comboLabelType				= GetDlgItem(IDC_LABELTYPE);
	m_checkOnlyVisibleOnMouseOver	= GetDlgItem(IDC_LABELONLYONMOUSEOVER);
	m_checkSubLabel					= GetDlgItem(IDC_SUBLABEL);
	m_checkShadow					= GetDlgItem(IDC_SHADOW);
	m_checkShadow2					= GetDlgItem(IDC_SHADOW2);
	m_comboBackground				= GetDlgItem(IDC_LABELBACKGROUND);
	m_comboHorAlign					= GetDlgItem(IDC_HALIGN2);
	m_comboVerAlign					= GetDlgItem(IDC_VALIGN2);
	m_editRotation					= GetDlgItem(IDC_ROTATION);
	m_checkTopDown					= GetDlgItem(IDC_TOPDOWN);
	m_comboPosition					= GetDlgItem(IDC_POSITION);
	m_editMargin					= GetDlgItem(IDC_MARGIN);
	m_editMaxWidth					= GetDlgItem(IDC_WIDTH);
	m_editMaxHeight					= GetDlgItem(IDC_HEIGHT);
	m_spinRotation					= GetDlgItem(IDC_ROTATIONSPINNER);
	m_spinRotation.SetRange(0,360);
	m_spinRotation.SetPos(Docklet::defaultLabel.GetRotation() );

	CString buf;
	buf.LoadString(IDS_LABELTYPE0);
	m_comboLabelType.AddString(buf);
	buf.LoadString(IDS_LABELTYPE2);
	m_comboLabelType.AddString(buf);
	m_comboLabelType.SetCurSel(Docklet::defaultLabelType);

	buf.LoadString(IDS_HALIGN0);
	m_comboHorAlign.AddString(buf);
	buf.LoadString(IDS_HALIGN1);
	m_comboHorAlign.AddString(buf);
	buf.LoadString(IDS_HALIGN2);
	m_comboHorAlign.AddString(buf);
	m_comboHorAlign.SetCurSel(Docklet::defaultLabel.GetHAlign() );

	buf.LoadString(IDS_VALIGN0);
	m_comboVerAlign.AddString(buf);
	buf.LoadString(IDS_VALIGN1);
	m_comboVerAlign.AddString(buf);
	buf.LoadString(IDS_VALIGN2);
	m_comboVerAlign.AddString(buf);
	m_comboVerAlign.SetCurSel(Docklet::defaultLabel.GetVAlign() );

	buf.LoadString(IDS_LABELPOSITION0);
	m_comboPosition.AddString(buf);
	buf.LoadString(IDS_LABELPOSITION1);
	m_comboPosition.AddString(buf);
	buf.LoadString(IDS_LABELPOSITION2);
	m_comboPosition.AddString(buf);
	buf.LoadString(IDS_LABELPOSITION3);
	m_comboPosition.AddString(buf);
	m_comboPosition.SetCurSel(Docklet::defaultLabel.GetPosition() );

	SetDlgItemInt(IDC_ROTATION,Docklet::defaultLabel.GetRotation(),TRUE);
	SetDlgItemInt(IDC_MARGIN,Docklet::defaultLabel.GetMargin(),TRUE);
	SetDlgItemInt(IDC_WIDTH,Docklet::defaultLabel.GetWidth(),FALSE);
	SetDlgItemInt(IDC_HEIGHT,Docklet::defaultLabel.GetHeight(),FALSE);
	m_checkTopDown.SetCheck(Docklet::defaultLabel.GetDrawTopDown() );

	m_checkOnlyVisibleOnMouseOver.SetCheck(Docklet::defaultLabelOnlyOnMouseOver);
	m_checkSubLabel.SetCheck(Docklet::defaultLabel.GetShowSubLabel() );
	m_checkShadow.SetCheck(Docklet::defaultLabel.GetMainLabelProperties().GetHasShadow() );
	m_checkShadow2.SetCheck(Docklet::defaultLabel.GetSubLabelProperties().GetHasShadow() );

	m_butNormalColor.SubclassWindow(GetDlgItem(IDSELECTCOLOR));
	COLORREF cr = Docklet::defaultLabel.GetMainLabelProperties().GetColor().ToCOLORREF();
	//RGB(Docklet::defaultLabel.ColorR,Docklet::defaultLabel.ColorG,Docklet::defaultLabel.ColorB);
	m_butNormalColor.SetColor(cr);
	m_butNormalColor.SetDefaultColor(cr);
	m_butNormalColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_butNormalColor.SetCustomText(IDS_MORECOLORS);

	m_comboBackground.AddString(L"");
	FillLabelBackgroundComboBox();
	m_comboBackground.SelectString(0,StringToWString(Docklet::defaultLabelBg).c_str());


	m_butSubColor.SubclassWindow(GetDlgItem(IDSELECTCOLOR2));
	COLORREF subCr = Docklet::defaultLabel.GetSubLabelProperties().GetColor().ToCOLORREF();;
	//RGB(Docklet::defaultLabel.subColorR,Docklet::defaultLabel.subColorG,Docklet::defaultLabel.subColorB);
	m_butSubColor.SetColor(subCr);
	m_butSubColor.SetDefaultColor(subCr);
	m_butSubColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_butSubColor.SetCustomText(IDS_MORECOLORS);

	initMode = false;

	return TRUE;
}

void CDefaultDeskletLabelSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && (wNotifyCode == BN_CLICKED || wNotifyCode == EN_CHANGE || wNotifyCode ==CBN_SELCHANGE))
	{
		SetModified();
	}

}

BOOL CDefaultDeskletLabelSheet::OnEditColor(LPNMHDR notifyData)
{
	if(notifyData->code == CPN_SELCHANGE)
	{
		SetModified();
	}
	return TRUE;
}

bool CDefaultDeskletLabelSheet::SelectFontProps(CLabelProperties& lp)
{
	CWindowDC screenDC(NULL);

	LOGFONTW lf = {0};
	wcscpy(lf.lfFaceName,lp.GetFont().c_str() );
	lf.lfHeight = lp.GetSize() * GetDeviceCaps(screenDC, LOGPIXELSY) / 72;
	lf.lfItalic = lp.GetStyle() & FontStyleItalic;
	lf.lfStrikeOut = lp.GetStyle() & FontStyleStrikeout;
	lf.lfUnderline = lp.GetStyle() & FontStyleUnderline;
	lf.lfWeight = (lp.GetStyle() & FontStyleBold) ? FW_BOLD : FW_REGULAR;
	CHOOSEFONTW cf = {0};
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &lf;
	cf.hwndOwner = ::GetActiveWindow();
	cf.hInstance = _Module.GetModuleInstance();
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.iPointSize = lp.GetSize()*10;

	if(!ChooseFontW(&cf))return false;

	int style = FontStyleRegular;
	if(lf.lfItalic) style |= FontStyleItalic;
	if(lf.lfStrikeOut) style |= FontStyleStrikeout;
	if(lf.lfUnderline) style |= FontStyleUnderline;
	if(lf.lfWeight >= FW_BOLD) style |= FontStyleBold;

	lp.SetStyle(style);
	lp.SetFont(lf.lfFaceName);
	lp.SetSize(cf.iPointSize/10 );

	return true;
}

void CDefaultDeskletLabelSheet::OnSelectFont(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if( SelectFontProps(Docklet::defaultLabel.GetMainLabelProperties() ) )
		SetModified();

/*	char tempfont[1024] = {0};
	WideCharToMultiByte(CP_ACP,0,Docklet::defaultLabel.Font_,-1,tempfont,sizeof(tempfont)/sizeof(tempfont[0]),0,0);
	LOGFONT lf = {0};
	strcpy(lf.lfFaceName,tempfont);
	lf.lfHeight = Docklet::defaultLabel.Size_ * GetDeviceCaps(::GetDC(0), LOGPIXELSY) / 72;
	lf.lfItalic = Docklet::defaultLabel.Style & FontStyleItalic;
	lf.lfStrikeOut = Docklet::defaultLabel.Style & FontStyleStrikeout;
	lf.lfUnderline = Docklet::defaultLabel.Style & FontStyleUnderline;
	lf.lfWeight = (Docklet::defaultLabel.Style & FontStyleBold) ? FW_BOLD : FW_REGULAR;
	CHOOSEFONT cf = {0};
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &lf;
	cf.hwndOwner = ::GetActiveWindow();
	cf.hInstance = _Module.GetModuleInstance();
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.iPointSize = Docklet::defaultLabel.Size_*10;
	if(!ChooseFont(&cf))return;

	SetModified();

	Docklet::defaultLabel.Style = FontStyleRegular;
	if(lf.lfItalic) Docklet::defaultLabel.Style |= FontStyleItalic;
	if(lf.lfStrikeOut) Docklet::defaultLabel.Style |= FontStyleStrikeout;
	if(lf.lfUnderline) Docklet::defaultLabel.Style |= FontStyleUnderline;
	if(lf.lfWeight >= FW_BOLD) Docklet::defaultLabel.Style |= FontStyleBold;

	MultiByteToWideChar(CP_ACP,0,lf.lfFaceName,-1,Docklet::defaultLabel.Font_,sizeof(Docklet::defaultLabel.Font_)/sizeof(Docklet::defaultLabel.Font_[0]));
	Docklet::defaultLabel.Size_ = cf.iPointSize/10;
*/
}

void CDefaultDeskletLabelSheet::OnSelectSubFont(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if( SelectFontProps(Docklet::defaultLabel.GetSubLabelProperties() ) )
		SetModified();

/*	char tempfont[1024] = {0};
	WideCharToMultiByte(CP_ACP,0,Docklet::defaultLabel.subFont,-1,tempfont,sizeof(tempfont)/sizeof(tempfont[0]),0,0);
	LOGFONT lf = {0};
	strcpy(lf.lfFaceName,tempfont);
	lf.lfHeight = Docklet::defaultLabel.subSize * GetDeviceCaps(::GetDC(0), LOGPIXELSY) / 72;
	lf.lfItalic = Docklet::defaultLabel.subStyle & FontStyleItalic;
	lf.lfStrikeOut = Docklet::defaultLabel.subStyle & FontStyleStrikeout;
	lf.lfUnderline = Docklet::defaultLabel.subStyle & FontStyleUnderline;
	lf.lfWeight = (Docklet::defaultLabel.subStyle & FontStyleBold) ? FW_BOLD : FW_REGULAR;

	CHOOSEFONT cf = {0};
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &lf;
	cf.hwndOwner = ::GetActiveWindow();
	cf.hInstance = _Module.GetModuleInstance();
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.iPointSize = Docklet::defaultLabel.subSize*10;
	if(!ChooseFont(&cf))return;

	SetModified();

	Docklet::defaultLabel.subStyle = FontStyleRegular;
	if(lf.lfItalic) Docklet::defaultLabel.subStyle |= FontStyleItalic;
	if(lf.lfStrikeOut) Docklet::defaultLabel.subStyle |= FontStyleStrikeout;
	if(lf.lfUnderline) Docklet::defaultLabel.subStyle |= FontStyleUnderline;
	if(lf.lfWeight >= FW_BOLD) Docklet::defaultLabel.subStyle |= FontStyleBold;

	MultiByteToWideChar(CP_ACP,0,lf.lfFaceName,-1,Docklet::defaultLabel.subFont,sizeof(Docklet::defaultLabel.subFont)/sizeof(Docklet::defaultLabel.subFont[0]));
	Docklet::defaultLabel.subSize = cf.iPointSize/10;
*/
}

void CDefaultDeskletLabelSheet::OnShadowSettings(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	CConfigShadowWindow csw(Docklet::defaultLabel);
	csw.DoModal();
	SetModified();
}

void CDefaultDeskletLabelSheet::OnSubShadowSettings(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	CConfigSubShadowWindow csw(Docklet::defaultLabel);
	csw.DoModal();
	SetModified();
}

int CDefaultDeskletLabelSheet::OnApply()
{
	Label& label = Docklet::defaultLabel;

	Docklet::defaultLabelType = m_comboLabelType.GetCurSel();
	Docklet::defaultLabelOnlyOnMouseOver = m_checkOnlyVisibleOnMouseOver.GetCheck() == TRUE;
	///Docklet::defaultLabel.drawTopDown = m_checkTopDown.GetCheck() == TRUE;
	//Docklet::defaultLabel.showSublabel = m_checkSubLabel.GetCheck() == TRUE;

	label.SetDrawTopDown( m_checkTopDown.GetCheck() == TRUE );
	label.SetShowSubLabel( m_checkSubLabel.GetCheck() == TRUE);

	
	SendDlgItemMessage(IDC_LABELBACKGROUND,WM_GETTEXT,MAX_PATH,(LPARAM)Docklet::defaultLabelBg);
	
	label.SetVAlign( m_comboVerAlign.GetCurSel() );
	label.SetHAlign( m_comboHorAlign.GetCurSel() );
	label.SetPosition( m_comboPosition.GetCurSel() );

	label.GetMainLabelProperties().SetHasShadow( m_checkShadow.GetCheck() == TRUE );
	label.GetSubLabelProperties().SetHasShadow( m_checkShadow2.GetCheck() == TRUE  );

	label.SetAlpha( GetDlgItemInt(IDC_ALPHA,0,FALSE) );
	label.SetRotation( GetDlgItemInt(IDC_ROTATION,0,FALSE) );
	label.SetWidth( GetDlgItemInt(IDC_WIDTH,0,FALSE) );
	label.SetHeight( GetDlgItemInt(IDC_HEIGHT,0,FALSE) );
	label.SetMargin( GetDlgItemInt(IDC_MARGIN,0,TRUE) );

/*
	Docklet::defaultLabel.VAlign		= m_comboVerAlign.GetCurSel();
	Docklet::defaultLabel.HAlign		= m_comboHorAlign.GetCurSel();
	Docklet::defaultLabel.Position	= m_comboPosition.GetCurSel();
					
	Docklet::defaultLabel.Shadow		= m_checkShadow.GetCheck();
	Docklet::defaultLabel.subShadow	= m_checkShadow2.GetCheck();

	Docklet::defaultLabel.Alpha		= GetDlgItemInt(IDC_ALPHA,0,FALSE);
	Docklet::defaultLabel.Rotation	= GetDlgItemInt(IDC_ROTATION,0,FALSE);

	Docklet::defaultLabel.Width		= GetDlgItemInt(IDC_WIDTH,0,FALSE);
	Docklet::defaultLabel.Height		= GetDlgItemInt(IDC_HEIGHT,0,FALSE);
	Docklet::defaultLabel.Margin		= GetDlgItemInt(IDC_MARGIN,0,TRUE);
*/

	COLORREF cr = m_butNormalColor.GetColor();
	m_butNormalColor.SetDefaultColor(cr);
	//Docklet::defaultLabel.ColorR = GetRValue(cr);
	//Docklet::defaultLabel.ColorG = GetGValue(cr);
	//Docklet::defaultLabel.ColorB = GetBValue(cr);
	Color c;
	c.SetFromCOLORREF(cr);
	label.GetMainLabelProperties().SetColor(c);

	COLORREF crSub = m_butSubColor.GetColor();
	m_butSubColor.SetDefaultColor(crSub);
	//Docklet::defaultLabel.subColorR = GetRValue(crSub);
	//Docklet::defaultLabel.subColorG = GetGValue(crSub);
	//Docklet::defaultLabel.subColorB = GetBValue(crSub);
	Color cSub;
	cSub.SetFromCOLORREF(crSub);
	label.GetMainLabelProperties().SetColor(cSub);

	return TRUE;
}
