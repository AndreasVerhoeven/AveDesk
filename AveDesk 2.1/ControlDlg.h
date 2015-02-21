// ControlDlg.h : Deklaration von CControlDlg

#ifndef __CONTROLDLG_H_
#define __CONTROLDLG_H_

#include "resource.h"       // Hauptsymbole
#include <atlframe.h>

#include "IObserver.h"

#include "PidlDropBox.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////

#define WM_GLOBALDOCKLETSNOTIFY WM_USER + 100
#define WM_SINGLEDOCKLETNOTIFY	WM_USER + 101
#define WM_AVEACTION_SHOWCP WM_USER + 201

#include "ImageListBox.h"

#include <map>

//#include "DockletDll.h"
//#include "DeskletSkinEnumerator.h"
class Docklet;
class DockletDLL;
class DeskletSkinEnumerator;
class DeskletSkinInfo;

// CControlDlg
class CControlDlg : 
	public CDialogImpl<CControlDlg>,
    public CDialogResize<CControlDlg>,
	public CMessageFilter,
	public IObserver
{
public:
	class FoundDesklet
	{
	public:
		FoundDesklet(DockletDLL* dll, DeskletSkinEnumerator* skinEnum=NULL, DeskletSkinInfo* skinInfo=NULL)
			: dll(dll), skinEnum(skinEnum), skinInfo(skinInfo), hMenu(NULL)
		{

		}

		DockletDLL* dll;
		DeskletSkinEnumerator* skinEnum;
		DeskletSkinInfo* skinInfo;
		HMENU hMenu;
	};

	std::vector<FoundDesklet> foundDesklets;
	HMENU deskletsPopupMenu;

	void UpdateMenu(bool DoModules=false);
	void ShowOrActivate();
	void UpdateList();
	CControlDlg() : deskletsPopupMenu(NULL)
	{
		hMenuAccel = AtlLoadAccelerators(MAKEINTRESOURCE(IDR_MENUACTIVE));
		InitializeCriticalSection(&m_CriticalSection);
	}

	~CControlDlg()
	{  
		if( IsWindow() )
			DestroyWindow();
		DeleteCriticalSection(&m_CriticalSection);
	}

	virtual BOOL PreTranslateMessage(  MSG * pMsg)
	{
		if(WM_KEYUP == pMsg->message)
		{
			BOOL handled=FALSE;
			OnKeyUp(pMsg->message, pMsg->wParam, pMsg->lParam, handled);
			return FALSE;
		}
		else if(WM_KEYDOWN == pMsg->message || WM_SYSKEYDOWN == pMsg->message)
		{
			BOOL handled=FALSE;
			OnKeyDown(pMsg->message, pMsg->wParam, pMsg->lParam, handled);
			return TranslateAccelerator(m_hWnd, hMenuAccel, pMsg);
		}
		else if(WM_AVEACTION_SHOWCP == pMsg->message)
		{
			ShowOrActivate();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	enum { IDD = IDD_ACTIVEDOCKLETS };

	HACCEL hMenuAccel;

	CImageListBoxCtrl lb;
	CFont bigFont, smallFont;
	CImageList m_ImageList;
	CPidlDropBox* pPidlDropBox;
	CHyperLink m_Link;

	std::map<HWND,int> m_DockletToImageListIndexMapping;

	CRITICAL_SECTION m_CriticalSection;

	void AddDeskletToList(Docklet* docklet);

	HWND tooltipHwnd;

BEGIN_DLGRESIZE_MAP(CControlDlg)
    DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X|DLSZ_SIZE_Y)
	//DLGRESIZE_CONTROL(IDC_LINK, DLSZ_MOVE_Y|DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_ADDDESKLET, DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_BROWSETHEMES, DLSZ_MOVE_Y)
END_DLGRESIZE_MAP()


BEGIN_MSG_MAP(CControlDlg)
	// dialog stuff
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShow)
	MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_MOVE, OnMove)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_HELP, OnHelp)
	MESSAGE_HANDLER(WM_PIDLDROPNOTIFY, OnPidlDropNotify)
	MESSAGE_HANDLER(WM_GLOBALDOCKLETSNOTIFY,OnGlobalDockletsNotify)
	MESSAGE_HANDLER(WM_SINGLEDOCKLETNOTIFY,OnSingleDockletNotify)
	MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles)
	MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
    
    // list box stuff
    COMMAND_HANDLER(IDC_LIST, LBN_DBLCLK, OnListDoubleClick)
    COMMAND_HANDLER(IDC_LIST, LBN_SELCHANGE, OnListSelChange)
    
	// button stuff
	COMMAND_ID_HANDLER(IDC_ADDDESKLET, OnAddDesklet)
	COMMAND_ID_HANDLER(IDC_BROWSETHEMES, OnBrowseThemes)

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
	CHAIN_MSG_MAP(CDialogResize<CControlDlg>)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()
// Prototypen von Behandlungsroutinen:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	// dialog stuff
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// drag drop stuff
	LRESULT OnPidlDropNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // listbox stuff
	LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// button stuff
	LRESULT OnAddDesklet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBrowseThemes(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    // menu stuff
	LRESULT OnImportDesklet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnExportDesklet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCreateEmptyTheme(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnGlobalSettings(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnDuplicate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSaveConfigAs(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnLoadConfig(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnMinimalProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnAutoAlign(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnDeskletProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnDefaultDeskletProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnHelpIndex(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSaveConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
#if 1 // remove
    LRESULT OnExtrasManageTools(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
# endif
    LRESULT OnExtrasTools(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnExtrasModuleShow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnExtrasModulesEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// notifications
	LRESULT OnGlobalDockletsNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSingleDockletNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2);

	int GetLBIndexFromHWND(HWND hwnd);


private:
	void SingleDockletNotify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2);
	void GlobalDockletsNotify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2);
	void OnAllDeskletsReleased();
	void OnDeskletAdded(HWND hwndDesklet);

	void HideBalloon();
};

extern CControlDlg _Dlg;

#endif //__CONTROLDLG_H_
