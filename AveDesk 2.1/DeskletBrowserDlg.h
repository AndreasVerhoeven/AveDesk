// DeskletBrowserDlg.h: interface for the CDeskletBrowserDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETBROWSERDLG_H__E3873886_A76B_4EDB_8912_D81CA0FD7945__INCLUDED_)
#define AFX_DESKLETBROWSERDLG_H__E3873886_A76B_4EDB_8912_D81CA0FD7945__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#include <vector>
#include <string>

#include "DeskletFinder.h"
#include "ToolTipImpl.h"
#include "ListBoxCategories.h"
#include "Splitterbar.h"

#include "DeskletProvider.h"
#include "DeskletProvider.h"
#include "DeskletCollection.h"
#include "DeskletListCtrl.h"
#include "DeskletsView.h"
#include "DeskletProviderFactory.h"
#include "AveScripterWarningDlg.h"

#define WM_FINDER_NOTIFY WM_USER + 1028
#define WM_PREVIEWBITMAP_NOTIFY WM_USER + 1029


class CDeskletBrowserDlg :
	public CDialogImpl<CDeskletBrowserDlg>,
    public CDialogResize<CDeskletBrowserDlg>
{
public:

	struct DeskletsViewState
	{
		DeskletsViewState() : top(0), curSel(0){}
		int top;
		int curSel;
	};

	static BOOL AveScripterInstalled();

	CAveScripterWarningDlg* warningDlg;

	CImageList m_ImageList;
	CImageList imgListCats;
	//CDeskletListBoxCtrl lb;
	CComboBox searchBox;
	CFont bigFont, smallFont;
	CTrackBarCtrl iconSlider;
	CVertSplitterCtrl splitter;

	CDeskletProviderFactory* providerFactory;
	std::map<std::wstring,CDeskletProvider*> providers;
	std::vector<CDeskletCollection*> collections;
	std::vector<DeskletsViewState> viewStates;
	CDeskletsView* view;
	CDeskletListCtrl lb;
	int curCollectionSel;


	//CDeskletFinder m_Finder;
	//CDeskletFinder m_Finder2;
	CListBoxCategoriesCtrl cats;

	//UINT m_IndexedA;

	CDeskletBrowserDlg();
	virtual ~CDeskletBrowserDlg();

	enum { IDD = IDD_DESKLETBROWSER };


	BEGIN_TOOLTIP_MAP()
		DEF_TOOLTIP(IDC_ICONSLIDER, IDS_TOOLTIP_ICONSLIDER)
		DEF_TOOLTIP(IDC_CLOSE, IDS_TOOLTIP_DESKLETBROWSE_CLOSE)
		DEF_TOOLTIP(IDC_ADDDESKLET, IDS_TOOLTIP_DESKLETBROWSE_ADDDESKLET)
		DEF_TOOLTIP(IDC_DESKLETS, IDS_TOOLTIP_DESKLETBROWSE_DESKLETS)
		DEF_TOOLTIP(IDC_SEARCHBOX, IDS_TOOLTIP_DESKLETBROWSE_SEARCHBOX)
		DEF_TOOLTIP(IDC_CATEGORIES, IDS_TOOLTIP_DESKLETBROWSE_CATEGORIES)
		DEF_TOOLTIP(IDC_ADDFEED, IDS_TOOLTIP_DESKLETBROWSE_ADDFEED)
		DEF_TOOLTIP(IDC_DELETEFEED, IDS_TOOLTIP_DESKLETBROWSE_DELETEFEED)
	END_TOOLTIP_MAP()

BEGIN_DLGRESIZE_MAP(CDeskletBrowserDlg)
    DLGRESIZE_CONTROL(IDC_DESKLETS, DLSZ_SIZE_X|DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_SEARCHBOX, DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_ADDDESKLET, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_CLOSE, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_ICONSLIDER, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_CATEGORIES, DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_SPLITTER, DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_ADDFEED, DLSZ_MOVE_Y)
	DLGRESIZE_CONTROL(IDC_DELETEFEED, DLSZ_MOVE_Y)
	//DLGRESIZE_CONTROL(IDC_CLOSEWINDOW, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	
END_DLGRESIZE_MAP()


BEGIN_MSG_MAP(CDeskletBrowserDlg)

	// dialog stuff
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	MESSAGE_HANDLER(WM_STARTS_DRAGGING, OnListBoxStartsDragging)
	MESSAGE_HANDLER(WM_UPDATED_SPLITTERPOS, OnUpdatedSplitterPos)
	MESSAGE_HANDLER(WM_USER_NOTIFY_ADD_DIR, OnUserNotifyAddDir)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	
	COMMAND_HANDLER(IDC_SEARCHBOX, CBN_EDITUPDATE, OnSearchBoxTextChange)
	COMMAND_HANDLER(IDC_SEARCHBOX, CBN_SELCHANGE, OnSearchBoxTextChange)
	COMMAND_HANDLER(IDC_DESKLETS, LBN_DBLCLK, OnListDoubleClick)
    COMMAND_HANDLER(IDC_DESKLETS, LBN_SELCHANGE, OnListSelChange)
	COMMAND_ID_HANDLER(IDC_ADDFEED, OnNewFeedButtonClick)
	COMMAND_ID_HANDLER(IDC_DELETEFEED, OnDeleteFeedButtonClick)

	COMMAND_HANDLER(IDC_CATEGORIES, LBN_SELCHANGE, OnListSelChangeCat)

    //MESSAGE_HANDLER(WM_HELP, OnHelp)

	//COMMAND_ID_HANDLER(IDC_LOADTHEME, OnLoadThemeButton)
	//COMMAND_ID_HANDLER(IDC_CLOSEWINDOW, OnCloseButton)
    
	
    // list box stuff
    //COMMAND_HANDLER(IDC_THEMES, LBN_DBLCLK, OnListDoubleClick)
    //COMMAND_HANDLER(IDC_THEMES, LBN_SELCHANGE, OnListSelChange)
    
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
	CHAIN_MSG_MAP(CDialogResize<CDeskletBrowserDlg>)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	int ChangeIconSize(int newSize);
	void Cleanup();
	void DeleteCategory(int index);
	BOOL LoadCategories();
	BOOL SaveCategories();

	LRESULT OnDeleteFeedButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNewFeedButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSearchBoxTextChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnListSelChangeCat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnListBoxStartsDragging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdatedSplitterPos(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUserNotifyAddDir(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif // !defined(AFX_DESKLETBROWSERDLG_H__E3873886_A76B_4EDB_8912_D81CA0FD7945__INCLUDED_)
