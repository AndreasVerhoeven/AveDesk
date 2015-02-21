// filename:		CSpecializedShowCaseTrackBar.hpp
// Description:		Definition of class CSpecializedShowCaseTrackBar
// Author:			Andreas Verhoeven
// Last modified:	31 July 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#pragma once

#include "stdafx.h"

class CSpecializedShowCaseTrackBar : public CWindowImpl<CSpecializedShowCaseTrackBar, CTrackBarCtrl>
//<CSpecializedShowCaseTrackBar>
{
public:
	LRESULT MouseDown(UINT msg, WPARAM w, LPARAM l);
	LRESULT MouseUp(UINT msg, WPARAM w, LPARAM l);

     BEGIN_MSG_MAP_EX(CMyTrackBarCtrl)
		MESSAGE_HANDLER_EX(WM_LBUTTONDOWN,MouseDown)
		MESSAGE_HANDLER_EX(WM_LBUTTONUP,MouseUp)
     END_MSG_MAP()
};
