// GlobalSettingsEffectSheet.h: interface for the CGlobalSettingsEffectSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALSETTINGSEFFECTSHEET_H__06C65307_DC02_4EA3_B266_1699791C09EA__INCLUDED_)
#define AFX_GLOBALSETTINGSEFFECTSHEET_H__06C65307_DC02_4EA3_B266_1699791C09EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "resource.h"

class AveDeskGlobalSettings;

class CGlobalSettingsEffectSheet : public CPropertyPageImpl<CGlobalSettingsEffectSheet>
{
private:
	AveDeskGlobalSettings& globalSettings;
	bool initMode;
	CTrackBarCtrl m_FlipSpeed, m_CloseSpeed;

	void ChangeStatusOfFlipControls(BOOL bEnabled);
	void ChangeStatusOfCloseControls(BOOL bEnabled);

public:
    enum { IDD = IDD_EFFECTSCONFIG };
 
	CGlobalSettingsEffectSheet(AveDeskGlobalSettings& globalSettings);
    ~CGlobalSettingsEffectSheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditFlipValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnEditCloseValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnFlipResetButton(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnCloseResetButton(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar); 
 
	BEGIN_MSG_MAP_EX(CGlobalSettingsEffectSheet)
		COMMAND_ID_HANDLER_EX(IDBUTRESETFLIP,OnFlipResetButton)
		COMMAND_ID_HANDLER_EX(IDBUTRESETCLOSE,OnCloseResetButton)
		COMMAND_ID_HANDLER_EX(IDC_ENABLEFLIP,OnEditFlipValue)
		COMMAND_ID_HANDLER_EX(IDC_ENABLECLOSE,OnEditCloseValue)
		COMMAND_ID_HANDLER_EX(IDC_CLOSEFADEOUT,OnEditCloseValue)
        MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_HSCROLL(OnHScroll)
        CHAIN_MSG_MAP(CPropertyPageImpl<CGlobalSettingsEffectSheet>)
    END_MSG_MAP()

    int OnApply();

};

#endif // !defined(AFX_GLOBALSETTINGSEFFECTSHEET_H__06C65307_DC02_4EA3_B266_1699791C09EA__INCLUDED_)
