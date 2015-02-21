// filename:		CGlobalSettingsTraySheet.hpp
// Description:		Definition of class CGlobalSettingsTraySheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#if !defined(AFX_CGlobalSettingsTraySheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_CGlobalSettingsTraySheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#pragma once

#include "stdafx.h"
#include "resource.h"

#include <string>

#include <vector>
#include "TrayAnimationData.h"

#include "IIconAnimatorCallback.h"
#include "IconAnimator.h"

#include "ImageListBox.h"

class AveDeskGlobalSettings;

class CGlobalSettingsTraySheet : 
	public CPropertyPageImpl<CGlobalSettingsTraySheet>,
	public IIconAnimatorCallback
{
private:
	AveDeskGlobalSettings& globalSettings;
	std::wstring selIconFile;
	long selIconIndex;
	HICON hIcon;
	CStatic iconControl;

	CImageListBoxCtrl m_AnimationsList;
	std::vector<CTrayAnimationData*> m_Animations;
	std::vector<CIconAnimator*> m_Animators;
	CImageList m_ImageList;
	int m_PreviousSelection;
	BOOL m_IsDestroying;

	CFont bigFont, smallFont;


	void UpdateIcon();

	bool initMode;

	void LoadAnimations();
	void UnloadAnimations();

public:
    enum { IDD = IDD_TRAYCONFIG };
 
	CGlobalSettingsTraySheet(AveDeskGlobalSettings& globalSettings);
    ~CGlobalSettingsTraySheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnBrowse(UINT wNotififyCode, int wID, HWND hwndCtrl);
	void OnDefault(UINT wNotififyCode, int wID, HWND hwndCtrl);
	void OnEditSettings(UINT wNotififyCode, int wID, HWND hwndCtrl);
 
	BEGIN_MSG_MAP_EX(CGlobalSettingsSelectSheet)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDBROWSEFORICON,OnBrowse)
		COMMAND_ID_HANDLER_EX(IDSETDEFAULTICON,OnDefault)
		COMMAND_ID_HANDLER_EX(IDC_SHOWTRAY,OnEditSettings);
		COMMAND_ID_HANDLER_EX(IDC_DOTRAYANIMATION,OnEditSettings);
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		COMMAND_HANDLER(IDC_ANIMATIONS, LBN_SELCHANGE, OnAnimationsListSelChange)


		MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles);

        CHAIN_MSG_MAP(CPropertyPageImpl<CGlobalSettingsTraySheet>)
			REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnAnimationsListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    int OnApply();

	virtual bool Render(HICON icon, DWORD callbackValue);
};
	
#endif//AFX_CGlobalSettingsTraySheet_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_