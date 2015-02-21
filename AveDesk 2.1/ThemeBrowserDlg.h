// ThemeBrowserDlg.h: interface for the CThemeBrowserDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THEMEBROWSERDLG_H__41DD6F58_6417_4045_9244_4820A724CFE7__INCLUDED_)
#define AFX_THEMEBROWSERDLG_H__41DD6F58_6417_4045_9244_4820A724CFE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#include "ThemeListBox.h"

#include <vector>
#include <string>


class CThemeBrowserDlg :
	public CDialogImpl<CThemeBrowserDlg>,
    public CDialogResize<CThemeBrowserDlg>
{
private:
	CImageListBoxCtrl lb;
	CFont bigFont, smallFont;
	CImageList m_ImageList;

	void PopulateList();

	std::vector<std::wstring> m_Filenames;

public:
	CThemeBrowserDlg();
	virtual ~CThemeBrowserDlg();
	
	enum { IDD = IDD_THEMEBROWSER };

BEGIN_DLGRESIZE_MAP(CThemeBrowserDlg)
    DLGRESIZE_CONTROL(IDC_THEMES, DLSZ_SIZE_X|DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_LOADTHEME, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_DELETETHEME, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_CLOSEWINDOW, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	
END_DLGRESIZE_MAP()


BEGIN_MSG_MAP(CThemeBrowserDlg)

	// dialog stuff
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	//MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    //MESSAGE_HANDLER(WM_HELP, OnHelp)

	COMMAND_ID_HANDLER(IDC_LOADTHEME, OnLoadThemeButton)
	COMMAND_ID_HANDLER(IDC_DELETETHEME, OnDeleteThemeButton)
	COMMAND_ID_HANDLER(IDC_CLOSEWINDOW, OnCloseButton)
    
	
    // list box stuff
    COMMAND_HANDLER(IDC_THEMES, LBN_DBLCLK, OnListDoubleClick)
    COMMAND_HANDLER(IDC_THEMES, LBN_SELCHANGE, OnListSelChange)
    
	/*
    // menu stuff
	COMMAND_ID_HANDLER(IDM_IMPORTDESKLET, OnImportDesklet)
	COMMAND_ID_HANDLER(IDM_EXPORTDESKLET, OnExportDesklet)
	COMMAND_ID_HANDLER(IDM_CREATEEMPTYTHEME, OnCreateEmptyTheme)
    COMMAND_ID_HANDLER(ID_SETTINGS_GLOBALSETTINGS, OnGlobalSettings)
    COMMAND_ID_HANDLER(ID_DESKLETS_DUPLICATE, OnDuplicate)
    COMMAND_ID_HANDLER(IDM_DESKLETS_ADD, OnAdd)
    COMMAND_ID_HANDLER(IDM_DESKLETS_REMOVE, OnRemove)
	COMMAND_ID_HANDLER(IDM_SAVECONFIGAS, OnSaveConfigAs)
	COMMAND_ID_HANDLER(IDM_LOADCONFIG, OnLoadConfig)
    COMMAND_ID_HANDLER(IDM_DESKLETS_MINIMAL_PROPERTIES, OnMinimalProperties)
    COMMAND_ID_HANDLER(IDM_APPLICATION_EXIT, OnExit)
    COMMAND_ID_HANDLER(IDM_DESKLETS_AUTOALIGN, OnAutoAlign)
    COMMAND_ID_HANDLER(IDM_DESKLETS_PROPERTIES, OnDeskletProperties)
    COMMAND_ID_HANDLER(IDM_SETTINGS_DEFAULTDESKLETPROPERTIES, OnDefaultDeskletProperties)
    COMMAND_ID_HANDLER(IDM_HELP_INDEX, OnHelpIndex)
    COMMAND_ID_HANDLER(IDM_HELP_ABOUT, OnHelpAbout)
    COMMAND_ID_HANDLER(IDM_APPLICATION_SAVECONFIGURATION, OnSaveConfiguration)
#if 1 // remove when AveDeskGlobalSettings is done
    COMMAND_ID_HANDLER(ID_EXTRAS_MANAGETOOLS, OnExtrasManageTools)
#endif
    COMMAND_RANGE_HANDLER(ID_EXTRAS_TOOL0, ID_EXTRAS_TOOL9, OnExtrasTools)
    COMMAND_ID_HANDLER(ID_EXTRAS_MODULES_MODULE0, OnExtrasModulesEdit)
    COMMAND_RANGE_HANDLER(ID_EXTRAS_MODULES_MODULE, ID_EXTRAS_MODULES_MODULE_MAX, OnExtrasModuleShow)

	// other stuff
	*/
	CHAIN_MSG_MAP(CDialogResize<CThemeBrowserDlg>)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()


    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnLoadThemeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDeleteThemeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


};

#endif // !defined(AFX_THEMEBROWSERDLG_H__41DD6F58_6417_4045_9244_4820A724CFE7__INCLUDED_)
