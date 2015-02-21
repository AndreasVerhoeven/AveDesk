// filename:		CGlobalSettingsToolsSheet.hpp
// Description:		Definition of class CGlobalSettingsClickSheet
// Author:			Andreas Verhoeven
// Last modified:	29 July 2004, 22:59
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CGLOBALSETTINGSTOOLSSHEET_HPP__CD2C7031_82E5_46E9_BD9B_02580CBABFB5__INCLUDED_)
#define AFX_CGLOBALSETTINGSTOOLSSHEET_HPP__CD2C7031_82E5_46E9_BD9B_02580CBABFB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "resource.h"

class AveDeskGlobalSettings;

class CGlobalSettingsToolsSheet : public CPropertyPageImpl<CGlobalSettingsToolsSheet>
{
private:
	//AveDeskGlobalSettings& globalSettings;
	bool initMode;
    CListBox lb;
    CEdit editDisplay, editExecute, editArguments, editLocation;

public:
	void Invalidate();
	void UpdateView(int curSel);
	int ReadData();
    enum { IDD = IDD_TOOLSCONFIG };
	CGlobalSettingsToolsSheet(_U_STRINGorID title=IDS_TAB5TOOLS);
	virtual ~CGlobalSettingsToolsSheet();
    
    // overrides from CPropertyPageImpl
	BOOL OnSetActive();
	BOOL OnKillActive();
	int OnApply();
	void OnReset();
	BOOL OnQueryCancel();
	void OnHelp();

    BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void OnCommandButton(UINT wNotififyCode, int wID, HWND hwndCtrl);
	void OnStartupButton(UINT wNotififyCode, int wID, HWND hwndCtrl);
	void OnArgumentsButton(UINT wNotififyCode, int wID, HWND hwndCtrl);

    BEGIN_MSG_MAP_EX(CGlobalSettingsSelectSheet)
        MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_CONTEXTMENU,OnContextMenu);
        COMMAND_HANDLER(IDC_LISTTOOLS, LBN_SELCHANGE, OnListSelChange)
        COMMAND_HANDLER(IDC_EDIT_CAPTION, EN_CHANGE, OnEditChange)
        COMMAND_HANDLER(IDC_EDIT_COMMAND, EN_CHANGE, OnEditChange)
        COMMAND_HANDLER(IDC_EDIT_ARGS, EN_CHANGE, OnEditChange)
        COMMAND_HANDLER(IDC_EDIT_STARTUP_FOLDER, EN_CHANGE, OnEditChange)
		COMMAND_ID_HANDLER_EX(IDC_BUTTON_COMMAND,OnCommandButton)
		COMMAND_ID_HANDLER_EX(IDC_BUTTON_STARTUP_FOLDER,OnStartupButton)
		COMMAND_ID_HANDLER_EX(IDC_BUTTON_ARGS,OnArgumentsButton)
        CHAIN_MSG_MAP(CPropertyPageImpl<CGlobalSettingsToolsSheet>)
    END_MSG_MAP()

	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};

#endif // !defined(AFX_CGLOBALSETTINGSTOOLSSHEET_HPP__CD2C7031_82E5_46E9_BD9B_02580CBABFB5__INCLUDED_)
