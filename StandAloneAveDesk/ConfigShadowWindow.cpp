// filename:		ConfigShadowWindow.cpp
// Description:		Implementation of class CConfigShadowWindow
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
#include "ConfigShadowWindow.hpp"
#include "Label.h"

CConfigShadowWindow::CConfigShadowWindow(Label& l) :
	label(l)
{


}

CConfigShadowWindow::~CConfigShadowWindow()
{

}

LRESULT CConfigShadowWindow::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
/*	CenterWindow();
	m_butColor.SubclassWindow(GetDlgItem(IDSETCOLOR));
	m_butColor.SetCustomText(IDS_MORECOLORS);
	m_butColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_butColor.SetDefaultColor(label.shadowColor);
	m_butColor.SetColor(label.shadowColor);
	SetDlgItemInt(IDC_ALPHA,label.shadowAlpha,FALSE);
	SetDlgItemInt(IDC_XOFFSET,label.shadowOffsetX,TRUE);
	SetDlgItemInt(IDC_YOFFSET,label.shadowOffsetY,TRUE);
*/
	return TRUE;
}

void CConfigShadowWindow::OnOK(UINT wNotifyCode, int wId, HWND hwndCtrl)
{
/*	label.shadowColor = m_butColor.GetColor();
	label.shadowAlpha = GetDlgItemInt(IDC_ALPHA,0,FALSE);
	label.shadowOffsetX = GetDlgItemInt(IDC_XOFFSET,0,TRUE);
	label.shadowOffsetY = GetDlgItemInt(IDC_YOFFSET,0,TRUE);
*/
	EndDialog(wId);
}

LRESULT CConfigShadowWindow::OnClose ()
{
	EndDialog(IDCANCEL);
	return TRUE;
}