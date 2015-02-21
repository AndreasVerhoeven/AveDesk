// ModulesDlg.h: interface for the CModulesDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULESDLG_H__92686101_4622_454C_BADC_8213A05BE32F__INCLUDED_)
#define AFX_MODULESDLG_H__92686101_4622_454C_BADC_8213A05BE32F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "ImageListBox.h"

#include "Modules.h"
#include "Module.h"

#include "ToolTipImpl.h"

class CModulesDlg :
	public CDialogImpl<CModulesDlg>
{
protected:
	CModules* m_Modules;
	bool AddModuleToList(CModule* m);
public:
	CModulesDlg(CModules* modules);
	~CModulesDlg();

	CImageListBoxCtrl  m_ModulesListBox;
	CImageList m_ImageList;

	CFont bigFont, smallFont;

	enum { IDD = IDD_MODULES };

	BEGIN_TOOLTIP_MAP()
		DEF_TOOLTIP(IDC_UNINSTALL, IDS_TOOLTIP_MODULES_UNINSTALL)
		DEF_TOOLTIP(IDC_CLOSE, IDS_TOOLTIP_MODULES_CLOSE)
		DEF_TOOLTIP(IDC_INSTALL, IDS_TOOLTIP_MODULES_INSTALL)
		DEF_TOOLTIP(IDC_MODULESLIST, IDS_TOOLTIP_MODULES_LIST);
	END_TOOLTIP_MAP()

BEGIN_MSG_MAP(CModulesDlg)
	// dialog stuff
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	//MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    //MESSAGE_HANDLER(WM_HELP, OnHelp)
	MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles)
    

    // list box stuff
    COMMAND_HANDLER(IDC_MODULESLIST, LBN_SELCHANGE, OnListSelChange)
	

	 COMMAND_ID_HANDLER(IDC_UNINSTALL, OnUninstallButton)
	 COMMAND_ID_HANDLER(IDC_INSTALL, OnInstallButton)
	 COMMAND_ID_HANDLER(IDC_CLOSE, OnCloseButton)

	// other stuff
    REFLECT_NOTIFICATIONS()
	//CHAIN_MSG_MAP(CDialogResize<CControlDlg>)
END_MSG_MAP()
// Prototypen von Behandlungsroutinen:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnUninstallButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnInstallButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// dialog stuff
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // listbox stuff
	LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // !defined(AFX_MODULESDLG_H__92686101_4622_454C_BADC_8213A05BE32F__INCLUDED_)
