// AddDeskletDlg.h: interface for the CAddDeskletDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDDESKLETDLG_H__479A702A_45AC_41F4_87A1_D93C97D8BAA0__INCLUDED_)
#define AFX_ADDDESKLETDLG_H__479A702A_45AC_41F4_87A1_D93C97D8BAA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DockletDLL.h"

#include "resource.h"
#include <vector>

#include "DeskletSkinEnumerator.h"

struct KnownDeskletStruct
{
	DockletDLL* dll;
	DeskletSkinEnumerator* enumerator;
	std::vector<DeskletSkinInfo> skinInfos;
};

class CAddDeskletDlg : public CDialogImpl<CAddDeskletDlg>  
{
protected:
	std::vector<KnownDeskletStruct> m_KnownDesklets;

	CTreeViewCtrlEx m_DeskletsListBox;

	void FillListBox();
	void SelectItem(UINT item, int childItemId=-1);
	void AddItem(UINT item, const WCHAR* skin=NULL, UINT skinNum=0);

	int GetDeskletSelectionID();

public:
	CAddDeskletDlg();
	~CAddDeskletDlg();

	enum { IDD = IDD_ADDDOCKLET };

BEGIN_MSG_MAP(CAddDeskletDlg)
	// dialog stuff
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CLOSE, OnCloseDialog)
	//MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	//MESSAGE_HANDLER(WM_CLOSE, OnClose)
	//MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    //MESSAGE_HANDLER(WM_HELP, OnHelp)
    
    // list box stuff
    COMMAND_HANDLER(IDC_TREE, LBN_DBLCLK, OnListDoubleClick)
    COMMAND_HANDLER(IDC_TREE, LBN_SELCHANGE, OnListSelChange)
	NOTIFY_HANDLER(IDC_TREE, TVN_SELCHANGED, OnTreeSelChange);
	//NOTIFY_HANDLER(IDC_TREE, NM_RCLICK, OnRClick)

    
	COMMAND_ID_HANDLER(IDADD, OnAddButtonClick)
	COMMAND_ID_HANDLER(IDOK, OnCloseButtonClick)

	// other stuff
    REFLECT_NOTIFICATIONS()
	//CHAIN_MSG_MAP(CDialogImpl<CAddDeskletDlg>)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCloseDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAddButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnTreeSelChange(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
};

#endif // !defined(AFX_ADDDESKLETDLG_H__479A702A_45AC_41F4_87A1_D93C97D8BAA0__INCLUDED_)
