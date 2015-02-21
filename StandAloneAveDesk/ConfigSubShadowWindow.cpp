// filename:		ConfigSubShadowWindow.cpp
// Description:		Implementation of class CConfigSubShadowWindow
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
#include "ConfigSubShadowWindow.hpp"
#include "Label.h"

CConfigSubShadowWindow::CConfigSubShadowWindow(Label& l) :
	label(l)
{


}

CConfigSubShadowWindow::~CConfigSubShadowWindow()
{

}

LRESULT CConfigSubShadowWindow::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
/*	CenterWindow();
	m_butColor.SubclassWindow(GetDlgItem(IDSETCOLOR));
	m_butColor.SetCustomText(IDS_MORECOLORS);
	m_butColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_butColor.SetDefaultColor(label.subShadowColor);
	m_butColor.SetColor(label.subShadowColor);
	SetDlgItemInt(IDC_ALPHA,label.subShadowAlpha,FALSE);
	SetDlgItemInt(IDC_XOFFSET,label.subShadowOffsetX,TRUE);
	SetDlgItemInt(IDC_YOFFSET,label.subShadowOffsetY,TRUE);
*/
	return TRUE;
}

void CConfigSubShadowWindow::OnOK(UINT wNotifyCode, int wId, HWND hwndCtrl)
{
/*	label.subShadowColor = m_butColor.GetColor();
	label.subShadowAlpha = GetDlgItemInt(IDC_ALPHA,0,FALSE);
	label.subShadowOffsetX = GetDlgItemInt(IDC_XOFFSET,0,TRUE);
	label.subShadowOffsetY = GetDlgItemInt(IDC_YOFFSET,0,TRUE);
*/
	EndDialog(wId);
}

LRESULT CConfigSubShadowWindow::OnClose ()
{
	EndDialog(IDCANCEL);
	return TRUE;
}