// ControlDlg.cpp : Implementierung von CControlDlg
#include "stdafx.h"
#include "Tools.hpp"
#include "ControlDlg.h"
#include "docklet.h"
#include "globalsettingstoolssheet.hpp"
#include "AddDeskletDlg.h"
#include "Test.h" // for additionalModules
#include "aboutdlg.h"
#include "Pidl.h"
#include "AveInstaller.h"
#include "ModulesDlg.h"
#include "commctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CControlDlg

// these are still (commented out) in avedesk.cpp
//old extern HWND cpHwnd;
//old extern bool curentDockletsDlgIsVisible;
extern void AutoAlignAll();
//extern int CALLBACK DefaultPropertySheet(HWND owner);

#include "AveDeskGlobalSettings.hpp"
extern AveDeskGlobalSettings _globalSettings;

#include "Modules.h"
#include "Module.h"
extern CModules* _Modules;

Docklet::DockletsMap::iterator iter;

#include "Policies.h"
extern CPolicies _Policies;


//////////////////////////////////////////////////////////////////////////////
// dialog stuff
//////////////////////////////////////////////////////////////////////////////

extern UINT _ControlPanelPreviewIconWidth;
extern UINT _ControlPanelPreviewIconHeight;

LRESULT CControlDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if(_Module.m_MainLoop != NULL)
		_Module.m_MainLoop->AddMessageFilter(this);

	HWND butAddDeskletHwnd = GetDlgItem(IDC_ADDDESKLET);
	tooltipHwnd = ::CreateWindowEx(WS_EX_TOOLWINDOW,
        TOOLTIPS_CLASS,
        NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_BALLOON,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_hWnd,
        NULL,
        _Module.GetModuleInstance(),
        NULL
        );


	::SetWindowPos(tooltipHwnd,
        HWND_TOP,
        0,
        0,
        0,
        0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);


    ModifyStyleEx(0,WS_EX_APPWINDOW); // for appearance in task bar
    HICON hIco = LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE( IDI_ICON2 ));
    SetIcon( hIco, TRUE );
    SetIcon( hIco, FALSE );
    CenterWindow();

	if(_Policies.GetBoolValue(L"runtime//controlPanel//acceptDrags", TRUE))
		DragAcceptFiles(TRUE);

	LOGFONTW lfBig = {0};
	lfBig.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfBig.lfFaceName,L"Tahoma");
	lfBig.lfWeight = FW_BOLD;
	lfBig.lfHeight = 14;

	LOGFONTW lfSmall = {0};
	lfSmall.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfSmall.lfFaceName,L"Tahoma");
	lfSmall.lfWeight = FW_NORMAL;
	lfSmall.lfHeight = 14;

	bigFont.CreateFontIndirect(&lfBig);
	smallFont.CreateFontIndirect(&lfSmall);


	m_ImageList.Create(_ControlPanelPreviewIconWidth,_ControlPanelPreviewIconHeight,ILC_COLOR32,1, 1);

	lb.SubclassWindow(GetDlgItem(IDC_LIST));
	lb.SetItemHeight(0, _ControlPanelPreviewIconHeight + 2);
	lb.SetImageList(m_ImageList, ILSIL_NORMAL);

	lb.SetFont(bigFont);
	DeleteObject(lb.SetSmallFont(smallFont));

	if(!_Policies.GetBoolValue(L"runtime//controlPanel//menu//allow", TRUE))
		SetMenu(NULL);
		

	std::wstring customTitle = _Policies.GetStringValue(L"runtime//controlPanel//title", L"");
	if(customTitle.length() > 0)
		SetWindowTextW(customTitle.c_str());

	std::wstring customCopyRight = _Policies.GetStringValue(L"runtime//controlPanel//copyright", L"");
	if(customCopyRight.length() > 0)
		SetDlgItemText(IDC_COPYRIGHT, customCopyRight.c_str());

	std::wstring customLinkText = _Policies.GetStringValue(L"runtime//controlPanel//linkText", L"");
	if(customLinkText.length() > 0)
		SetDlgItemText(IDC_LINK, customLinkText.c_str());

	
	m_Link.SubclassWindow(GetDlgItem(IDC_LINK));

	std::wstring customLink = _Policies.GetStringValue(L"runtime//controlPanel//linkURL", L"");
	if(customLink.length() > 0)
		m_Link.SetHyperLink(customLink.c_str());
	else
		m_Link.SetHyperLink(_T("http://home.hccnet.nl/averhoev.2/support.html"));



	IObservable* obs = Docklet::GetGlobalDockletsEventNotifier();
	obs->Register(this);

    UpdateList();

    // loading accelerators is done in WinMain

	if(_Policies.GetBoolValue(L"runtime//controlPanel//menu//allow", TRUE))
		UpdateMenu(true); // modules init only once, tools init arbitrary times
    DlgResize_Init();
    OnListSelChange(0,0,0,bHandled);
    return 1;
}

LRESULT CControlDlg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HideBalloon();
	return 1;
}


void CControlDlg::HideBalloon()
{
	HWND butAddDeskletHwnd = GetDlgItem(IDC_ADDDESKLET);

	RECT rect = {0};
	::GetClientRect(butAddDeskletHwnd, &rect);

	TOOLINFO ti = {0};
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_TRACK | TTF_TRANSPARENT;
    ti.hwnd = m_hWnd;
    ti.hinst = _Module.GetModuleInstance();
    ti.uId = (UINT_PTR) 0; // uid
        // ToolTip control will cover the whole window
    ti.rect.left = rect.left;    
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

	::SendMessage(tooltipHwnd, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&ti);
}

LRESULT CControlDlg::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(IsIconic())
	{
		HideBalloon();
	}
	else
	{
		HWND butAddDeskletHwnd = GetDlgItem(IDC_ADDDESKLET);

		RECT rect = {0};
		::GetClientRect(butAddDeskletHwnd, &rect);

		TOOLINFO ti = {0};
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = TTF_TRACK | TTF_TRANSPARENT;
		ti.hwnd = m_hWnd;
		ti.hinst = _Module.GetModuleInstance();
		ti.uId = (UINT_PTR) 0; // uid
			// ToolTip control will cover the whole window
		ti.rect.left = rect.left;    
		ti.rect.top = rect.top;
		ti.rect.right = rect.right;
			ti.rect.bottom = rect.bottom;

		POINT pt = {0};
		::ClientToScreen(butAddDeskletHwnd, &pt);
		pt.x += 12; pt.y += 12; // offset by 10
		LRESULT res = ::SendMessage(tooltipHwnd, TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
	}

	return 0;
}

LRESULT CControlDlg::OnShow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND butAddDeskletHwnd = GetDlgItem(IDC_ADDDESKLET);

	RECT rect = {0};
	::GetClientRect(butAddDeskletHwnd, &rect);

	TOOLINFO ti = {0};
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_TRACK | TTF_TRANSPARENT;
    ti.hwnd = m_hWnd;
    ti.hinst = _Module.GetModuleInstance();
    ti.uId =  (UINT_PTR)0; // uid
    ti.lpszText = _T("You can find more desklet to add to\n your desktop in the Desklet Browser Dialog.\n\nAfter you have installed a new desklet,\nit will show up in the Desklet Browser Dialog also.");
        // ToolTip control will cover the whole window
    ti.rect.left = rect.left;    
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

    LRESULT res = ::SendMessage(tooltipHwnd, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	res = ::SendMessage(tooltipHwnd,  TTM_SETTITLE, (WPARAM)1, (LPARAM)_T("More Desklets"));

	POINT pt = {0};
	::ClientToScreen(butAddDeskletHwnd, &pt);
	pt.x += 12; pt.y += 12; // offset by 10
	res = ::SendMessage(tooltipHwnd, TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
	res = ::SendMessage(tooltipHwnd, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&ti);

	return 0;
}

LRESULT CControlDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(GetKeyState(VK_SHIFT) < 0)
		SetDlgItemText(IDC_ADDDESKLET, _T("Desklet Menu"));
	return 0;
}


LRESULT CControlDlg::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(!(GetKeyState(VK_SHIFT) < 0))
		SetDlgItemText(IDC_ADDDESKLET, _T("Add a Desklet"));

	return 0;
}

LRESULT CControlDlg::OnAddDesklet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HideBalloon();
	if(GetKeyState(VK_SHIFT) < 0)
	{
		foundDesklets.clear();
		CMenu menu;
		menu.CreatePopupMenu();

		HCURSOR prevCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT	));

		WCHAR appDir[MAX_PATH] = {0};
		wcscpy(appDir, _App->GetAveDeskDataDir().c_str());
		std::wstring dir = lowercaseW(appDir);
		DockletDLL::ScanDirectoryForDesklets(dir+L"Docklets\\",true);
		DockletDLL::ScanDirectoryForDesklets(dir+L"Desklets\\",true);

		int counter(2);
		for(DockletDLL::DockletsDLLMap::iterator iter = DockletDLL::DockletDLLs.begin();  iter != DockletDLL::DockletDLLs.end(); ++iter)
		{
			DockletDLL* dll = iter->second;
			FoundDesklet fd(dll);

			DWORD flags = MF_STRING | MF_ENABLED;
			UINT_PTR id = (UINT_PTR)counter;
			if(dll->GetDocklet()->SupportsSkinEnumerator())
			{
				fd.hMenu = ::CreatePopupMenu();
				id = (UINT_PTR)fd.hMenu;
				flags |= MF_POPUP;
			}
			menu.AppendMenu(flags, id, dll->GetName());
			foundDesklets.push_back(fd);

			counter++;
		}

		CString str(_T("Open dialog..."));
		menu.AppendMenu(MF_SEPARATOR, (UINT_PTR)0, (LPCTSTR)NULL);
		menu.AppendMenu(MF_STRING | MF_ENABLED | MF_BYCOMMAND, (UINT_PTR)1, str);
		POINT p = {0};
		::ClientToScreen(GetDlgItem(IDC_ADDDESKLET), &p);
		::SetCursor(prevCursor);
		int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, p.x, p.y, m_hWnd, NULL);


		if(cmd > 1 && cmd < static_cast<int>(foundDesklets.size() + 2))
		{
			FoundDesklet& fd = foundDesklets[cmd-2];
			POINT pt = {0};
			SkinInfoWrapper* wrapper = NULL;
			if(fd.skinInfo != NULL)
				wrapper = fd.skinInfo->GetSkinInfoWrapper();
			Docklet::LoadDocklet(fd.dll->GetFileName(), NULL, fd.skinEnum, wrapper, false, pt);

			delete wrapper;
		}

		for(std::vector<FoundDesklet>::iterator fd_iter = foundDesklets.begin(); fd_iter != foundDesklets.end(); ++fd_iter)
		{
			FoundDesklet& fd = *fd_iter;
			if(NULL == fd.skinEnum)
				fd.dll->Release();
			else
			{
				fd.skinEnum->Release();
				delete fd.skinInfo;
			}
		}


		foundDesklets.clear();

		BOOL handled = FALSE;
		OnKeyUp(WM_KEYUP, 0L, 0L, handled);

		if(1 == cmd)
		{
			_App->ShowDeskletBrowserDialog();
		}

	}
	else
	{
		_App->ShowDeskletBrowserDialog();
	}
	return 0;
}

LRESULT CControlDlg::OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT uPos = static_cast<UINT>(LOWORD(lParam));
	HMENU hMenuPopup = reinterpret_cast<HMENU>(wParam);
	for(std::vector<FoundDesklet>::iterator fd_iter = foundDesklets.begin(); fd_iter != foundDesklets.end(); ++fd_iter)
	{
		FoundDesklet& fd = *fd_iter;
		if(fd.hMenu == hMenuPopup)
		{
			if(::GetMenuItemCount(hMenuPopup) == 0)
			{
				HCURSOR prevCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

				DeskletSkinEnumerator* skinEnum = DeskletSkinEnumerator::Create(fd.dll);
				DeskletSkinInfo info;

				int counter(foundDesklets.size()+2);
				while(skinEnum->Next(&info))
				{
					DeskletSkinInfo* si = new DeskletSkinInfo(info);
					skinEnum->AddRef();
					foundDesklets.push_back(FoundDesklet(fd.dll, skinEnum, si));

					::AppendMenuW(hMenuPopup, MF_STRING | MF_ENABLED | MF_BYCOMMAND, (UINT_PTR)counter, (LPCTSTR)info.GetName().c_str());
					counter++;
				}

				::SetCursor(prevCursor);
			}

			return 0;
		}
	}

	return 0;
}

LRESULT CControlDlg::OnBrowseThemes(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HideBalloon();
	_App->ShowThemeBrowserDialog();
	return 0;
}

LRESULT CControlDlg::OnPidlDropNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	CPidl pidl;
	pidl.SetFromPIDL( reinterpret_cast<LPITEMIDLIST>(lParam) );

	std::wstring path = pidl.GetFullParseDisplayName();

	// first, save the old theme silently!
	if(_globalSettings.GetAutoSaveThemes() )
		_App->SaveTheme(CComBSTR(_App->GetThemeFileName().c_str()) );

	_App->SetThemeFileName(path);
	_App->LoadTheme(CComBSTR(_App->GetThemeFileName().c_str()) );

	pidl.Detach();

	return FALSE; // indicate that no more enumeration is necessary!
}

LRESULT CControlDlg::OnCreateEmptyTheme(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// first, save the old theme silently!
	if(_globalSettings.GetAutoSaveThemes() )
		_App->SaveTheme(CComBSTR(_App->GetThemeFileName().c_str()) );

	std::wstring themeDir = _App->GetAveDeskThemeDir();
	std::wstring newThemeFileName;
	int index(1);
	do
	{
		WCHAR path[MAX_PATH] = {0};
		wcscpy(path,themeDir.c_str());

		CString newTheme;
		newTheme.Format(IDS_NEWTHEMEFILENAME, index);

		PathAppend(path, newTheme);
		newThemeFileName = path;

		index++;

	}while(PathFileExists(newThemeFileName.c_str()));

	Docklet::FadeAllOut();
	Docklet::ReleaseAll();

	_globalSettings.SetThemeFileName(newThemeFileName);

	return 0;
}

LRESULT CControlDlg::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT p;
	p.x = GET_X_LPARAM(lParam);
	p.y = GET_Y_LPARAM(lParam);
	lb.ScreenToClient(&p);
	BOOL outside = FALSE;
    int curSel = lb.ItemFromPoint(p, outside);
	if(curSel != LB_ERR && curSel > -1 && !outside)
	{
		if( !lb.GetSel(curSel) )
        {
            // right click will unselect all
            // but shift right click will add to selection
            lb.SetRedraw(FALSE);
            if(lb.GetSelCount() && !(GetKeyState(VK_SHIFT) & 0x8000))
            {
                for( int i = 0; i < lb.GetCount(); i++ )
                {
                    lb.SetSel(i, FALSE);
                }
            }
            lb.SetSel(curSel);
            lb.SetRedraw(TRUE);
            UpdateMenu();
        }
	}

	if(_Policies.GetBoolValue(L"runtime//controlPanel//list//allowRightClickMenu", TRUE))
	{
		GetCursorPos(&p);
		HMENU menu = GetSubMenu(GetMenu(),2);
		int	cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN, p.x, p.y, 0, m_hWnd,0);
	}
	return 0;
}

LRESULT CControlDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDrop = reinterpret_cast<HDROP>(wParam);
	if(NULL == hDrop)
		return 0;


	int numFiles = DragQueryFile(hDrop,(UINT)-1,NULL,0);
	for(int i = 0; i < numFiles; ++i)
	{
		WCHAR path[MAX_PATH] = {0};
		DragQueryFileW(hDrop,i,path,MAX_PATH);

		if(path[0] != L'\0')
		{
			WCHAR* ext = PathFindExtensionW(path);

			if(_wcsicmp(ext,L".zip") == 0 || _wcsicmp(ext,L".aveinst") == 0)
				CAveInstaller::InstallAsync(path, NULL, FALSE);
			else if(_wcsicmp(ext,L".avedesklet") == 0)
				Docklet::LoadFromSingleFile(path,NULL,TRUE);
			else if(_wcsicmp(ext,L".avetheme") == 0)
				_App->LoadTheme(CComBSTR(path));
		}
	}

	return 0;
}

LRESULT CControlDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	// FIX: was originally in OnDestroy(...)
	int count = lb.GetCount();
	for(int i = 0; i < count; ++i)
	{
		HWND hwnd = reinterpret_cast<HWND>( lb.GetItemData(i) );
		Docklet* d = Docklet::FindDocklet(hwnd);
		if(d != NULL)
			d->Deregister(this);
	}

	IObservable* obs = Docklet::GetGlobalDockletsEventNotifier();
	obs->Deregister(this);

	//lb.Detach();
    //EndDialog(FALSE);
	DestroyWindow();

	return 0;
}
LRESULT CControlDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //old curentDockletsDlgIsVisible = false;
    //old cpHwnd = 0;

	if(::IsWindow(tooltipHwnd))
	{
		::DestroyWindow(tooltipHwnd);
		tooltipHwnd = NULL;
	}

	lb.ResetContent();
	//lb.Detach();

	//lb.UnSubclassWindow();
	
	//m_Link.UnsubclassWindow();
	//m_Link.m_hWnd = NULL;
	//m_Link.DestroyWindow();
	if(::IsWindow(m_Link.m_tip.m_hWnd))
		m_Link.m_tip.DestroyWindow();
	m_Link.m_tip.m_hWnd = NULL;

	DeleteObject( bigFont.Detach());
	DeleteObject( smallFont.Detach());

	EnterCriticalSection(&m_CriticalSection);
	m_ImageList.RemoveAll();
	m_ImageList.Destroy();
	m_DockletToImageListIndexMapping.clear();
	LeaveCriticalSection(&m_CriticalSection);

	if(_Module.m_MainLoop != NULL)
		_Module.m_MainLoop->RemoveMessageFilter(this);

	return 0;
}

LRESULT CControlDlg::OnSaveConfigAs(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	bHandled= TRUE;

	CString themesFilter;
	themesFilter.LoadString(IDS_THEMESFILTER);
	for(int i = themesFilter.GetLength() - 1; i >= 0; --i)
	{
		if(themesFilter[i] == _T('\n'))
			themesFilter.SetAt(i,_T('\0'));
	}

	//themesFilter.Replace(_T("\n"),_T("\0"));

	CFileDialog fd(FALSE,												// FALSE for SaveDialog
					_T("avetheme"),										// default extension
					W2T(const_cast<WCHAR*>(_App->GetThemeFileName().c_str())),	// filename
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,				// flags
					themesFilter,//_T("AveDesk Themes(*.avetheme)\0*.avetheme\0All Files(*.*)\0*.*\0\0"), // filter
					m_hWnd);

	if( fd.DoModal() == TRUE)
	{
		_App->SetThemeFileName(T2W(fd.m_szFileName) );
		_App->SaveTheme(CComBSTR(_App->GetThemeFileName().c_str()) );
	}

	return 0;
}

LRESULT CControlDlg::OnImportDesklet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CString filter;
	filter.LoadString(IDS_DESKLETSFILTER);
	for(int i = filter.GetLength() - 1; i >= 0; --i)
	{
		if(filter[i] == _T('\n'))
			filter.SetAt(i,_T('\0'));
	}
	
	CFileDialog fd(TRUE,												// TRUE for OpenDialog
					_T("avedesklet"),									// default extension
					NULL,												// filename
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,				// flags
					filter,//_T("AveDesk Desklets(*.avedesklet)\0*.avedesklet\0All Files(*.*)\0*.*\0\0"), // filter
					m_hWnd);

	if( fd.DoModal() == TRUE)
	{
		Docklet::LoadFromSingleFile(fd.m_szFileName,NULL,TRUE);
	}

	return 0;
}

LRESULT CControlDlg::OnExportDesklet(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CString filter;
	filter.LoadString(IDS_DESKLETSFILTER);
	for(int i = filter.GetLength() - 1; i >= 0; --i)
	{
		if(filter[i] == _T('\n'))
			filter.SetAt(i,_T('\0'));
	}
	

	for( int curSel = lb.GetCount() - 1; curSel >= 0; curSel-- ) 
    {
        if( lb.GetSel(curSel) )
        {
		    HWND hwnd = (HWND)lb.GetItemData(curSel);
			Docklet* d= Docklet::FindDocklet(hwnd);
			if(d)
			{
				CFileDialog fd(TRUE,												// TRUE for OpenDialog
								_T("avedesklet"),									// default extension
								d->GetLabel().GetMainCaption().c_str(),				// filename
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,				// flags
								filter,//_T("AveDesk Desklets(*.avedesklet)\0*.avedesklet\0All Files(*.*)\0*.*\0\0"), // filter
								m_hWnd);

				if( fd.DoModal() == TRUE)
				{
					d->SaveToSingleFile(fd.m_szFileName);
				}
			}
		}
	}

	return 0;
}

LRESULT CControlDlg::OnLoadConfig(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	bHandled= TRUE;

	CString themesFilter;
	themesFilter.LoadString(IDS_THEMESFILTER);
	for(int i = themesFilter.GetLength() - 1; i >= 0; --i)
	{
		if(themesFilter[i] == _T('\n'))
			themesFilter.SetAt(i,_T('\0'));
	}

	CFileDialog fd(TRUE,												// TRUE for OpenDialog
					_T("avetheme"),										// default extension
					W2T(const_cast<WCHAR*>(_App->GetThemeFileName().c_str())),	// filename
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,				// flags
					themesFilter,//_T("AveDesk Themes(*.avetheme)\0*.avetheme\0All Files(*.*)\0*.*\0\0"), // filter
					m_hWnd);

	if( fd.DoModal() == TRUE)
	{
		// first, save the old theme silently!
		if(_globalSettings.GetAutoSaveThemes() )
			_App->SaveTheme(CComBSTR(_App->GetThemeFileName().c_str()) );

		_App->SetThemeFileName(T2W(fd.m_szFileName) );
		_App->LoadTheme(CComBSTR(_App->GetThemeFileName().c_str()) );
	}

	return 0;
}

LRESULT CControlDlg::OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // to do: add context help
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// listbox stuff
//////////////////////////////////////////////////////////////////////////////

LRESULT CControlDlg::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HMENU menu = GetSubMenu(GetMenu(),2);
    int selCount = lb.GetSelCount();
    int oneSel = (selCount == 1) ? MF_ENABLED : MF_GRAYED;
    EnableMenuItem(menu, IDM_DESKLETS_PROPERTIES, MF_BYCOMMAND|oneSel);
    EnableMenuItem(menu, IDM_SETTINGS_DEFAULTDESKLETPROPERTIES, MF_BYCOMMAND|oneSel);
    int noSel = (selCount == 0) ? MF_GRAYED : MF_ENABLED;
    EnableMenuItem(menu, IDM_DESKLETS_REMOVE, MF_BYCOMMAND|noSel);
    EnableMenuItem(menu, ID_DESKLETS_DUPLICATE, MF_BYCOMMAND|noSel);
	EnableMenuItem(menu, IDM_EXPORTDESKLET, MF_BYCOMMAND|noSel);
    return 0;
}
LRESULT CControlDlg::OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    int curSel = lb.GetSelCount();
    if(curSel == 1)
    {
	    lb.GetSelItems(1, &curSel);
        HWND hwnd = (HWND)lb.GetItemData(curSel);
	    Docklet * d = Docklet::FindDocklet(hwnd);
	    if(d)
	    {
		    if(!d->Configure())
			    d->AsyncDeskletPropertySheet(NULL,NULL,false,TRUE);
	    }
    }
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// menu handlers
//////////////////////////////////////////////////////////////////////////////

LRESULT CControlDlg::OnGlobalSettings(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	_globalSettings.ShowDialog();

	//GlobalSettingSheets(m_hWnd);
    return 0;
}

LRESULT CControlDlg::OnDuplicate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for( int curSel = 0; curSel < lb.GetCount(); curSel++ ) 
    {
        if( lb.GetSel(curSel) )
        {
		    HWND hwnd = (HWND)lb.GetItemData(curSel);
		    DuplicateDesklet(hwnd,true);
	    }
    }
    return 0;
}

LRESULT CControlDlg::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAddDeskletDlg dlg;
	dlg.DoModal();
	return 0;
}

extern BOOL RemoveDeskletAndSaveConfig(HWND,BOOL,BOOL);
LRESULT CControlDlg::OnRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	BOOL saveConfig = !(GetKeyState(VK_SHIFT) < 0);
	std::vector<HWND> hwnds;
	for( int curSel = lb.GetCount() - 1; curSel >= 0; curSel-- ) 
    {
        if( lb.GetSel(curSel) )
        {
		    HWND hwnd = (HWND)lb.GetItemData(curSel);
			hwnds.push_back(hwnd);
		    //RemoveDeskletAndSaveConfig(hwnd,TRUE,TRUE);
			//lb.DeleteString(curSel);
	    }
    }

	if(hwnds.size() == 1)
	{
		RemoveDeskletAndSaveConfig(hwnds[0],TRUE,saveConfig);	
	}
	else if(hwnds.size() > 1)
	{
		CString question;
		question.LoadString(IDS_REMOVEDESKLETS);
		CString caption;
		caption.LoadString(IDS_REMOVEDESKLETSCAPTION);
		if(!_globalSettings.GetAsk() || :: MessageBox(m_hWnd, question, caption, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			for(std::vector<HWND>::iterator iter = hwnds.begin(); iter != hwnds.end(); ++iter)
				RemoveDeskletAndSaveConfig(*iter, FALSE, saveConfig);
		}
	}

    return 0;
}

LRESULT CControlDlg::OnMinimalProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for( int curSel = 0; curSel < lb.GetCount(); curSel++ ) 
    {
        if( lb.GetSel(curSel) )
        {
	        HWND hwnd = (HWND)lb.GetItemData(curSel);
	        Docklet * d = Docklet::FindDocklet(hwnd);
	        if(d)
	        {
				// async is not needed, since no other propertysheets are added!
			    d->DeskletPropertySheet(NULL,NULL,true);
	        }
        }
    }
    return 0;
}

LRESULT CControlDlg::OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	PostQuitMessage(0);
    return 0;
}

LRESULT CControlDlg::OnAutoAlign(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AutoAlignAll();
    return 0;
}

LRESULT CControlDlg::OnDeskletProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    int curSel = lb.GetSelCount();
    if(curSel == 1)
    {
	    lb.GetSelItems(1, &curSel);
        HWND hwnd = (HWND)lb.GetItemData(curSel);
	    Docklet * d = Docklet::FindDocklet(hwnd);
	    if(d)
	    {
		    if(!d->Configure())
				d->AsyncDeskletPropertySheet(NULL,NULL,false,TRUE);
			    //DeskletDefaultConfigDialog(hwnd,0);
	    }
    }
	return 0;
}

LRESULT CControlDlg::OnDefaultDeskletProperties(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	Docklet::defaultDocklet->DeskletPropertySheet(m_hWnd);
    //DefaultPropertySheet(m_hWnd);
	return 0;
}

LRESULT CControlDlg::OnHelpIndex(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	wcscat(path,L"help\\readme.html");
	ShellExecute(0,L"open",path,0,0,SW_NORMAL);
	return 0;
}

LRESULT CControlDlg::OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//DialogBoxParam(_Module.m_hInstResource, MAKEINTRESOURCE(IDD_ABOUT), m_hWnd, AboutDlgProc, (LPARAM)0);
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CControlDlg::OnSaveConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	_App->SaveTheme(CComBSTR(_App->GetThemeFileName().c_str()) );
    return 0;
}

#if 1 // remove if AveDeskGlobalSettings in charge, also remove menu entry
LRESULT CControlDlg::OnExtrasManageTools(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CPropertySheet ps(IDS_GLOBALSETTINGSTITLE);
	ps.AddPage ( CGlobalSettingsToolsSheet() );
	int res = ps.DoModal();
	return 0;
}
#endif

LRESULT CControlDlg::OnExtrasTools(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int iMax = lb.GetCount();
    CString cmd, aStr, sStr, it;
    for( int i = 0; i < iMax; i++ )
    {
        int hwnd = (DWORD)lb.GetItemData(i);
        it.Format(_T("%ld "), hwnd);
        aStr += it;

        if( lb.GetSel(i) )
        {
            sStr += it;
        }

    }

    int idx = wID - ID_EXTRAS_TOOL0;

    cmd = _Tools.tools[idx].Arguments;
    cmd.Replace(_T("$S"), sStr);    // selected ones
    cmd.Replace(_T("$A"), aStr);    // all running
    cmd.Replace(_T("$$"), _T("$"));     // in case you need to pass a '$'

    _Tools.tools[idx].m_hWnd = m_hWnd;
    _Tools.tools[idx].Call(cmd);
    return 0;
}


LRESULT CControlDlg::OnExtrasModuleShow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if( wID >= ID_EXTRAS_MODULES_MODULE )
    {
        int index = wID - ID_EXTRAS_MODULES_MODULE;
		std::vector<CModule*> modules = _Modules->GetCurrentModules();
        if( index < static_cast<int>(modules.size()) )
        {
            typedef void (__stdcall *HOnConfigure)(void *lpData);

            CModule *mod = modules[index];
            HOnConfigure OnConfigure = (HOnConfigure)
                GetProcAddress(mod->GetHMODULE(), "OnConfigure");

            if( OnConfigure )
                OnConfigure(mod->GetPointer());
            else
            {
                CString str, ver, cap;
                ver.Format(_T("%d.%d"), mod->GetVersion() / 10, mod->GetVersion() % 10);
                str.LoadString(IDS_MODULENOCONFIG);
                cap.LoadString(IDS_PROJNAME);
                str.Replace(_T("$N"), mod->GetName());
                str.Replace(_T("$A"), mod->GetAuthor());
                str.Replace(_T("$V"), ver);
                str.Replace(_T("$D"), mod->GetDescription());
                this->MessageBox(str, NULL, MB_OK|MB_ICONINFORMATION);
            }
        }
    }

    return 0;
}

LRESULT CControlDlg::OnExtrasModulesEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	/*
    // so far this is it:
    CString modini;
    TCHAR *cModIni = modini.GetBuffer(MAX_PATH+1);
	GetModuleFileName(NULL,cModIni,MAX_PATH);
    PathRemoveFileSpec(cModIni);
    PathAppend(cModIni, _T("Modules.ini"));
    modini.ReleaseBuffer();
    ShellExecute(
        m_hWnd,
        _T("open"),
        modini, NULL, NULL, SW_SHOW);
	*/

	CModulesDlg dlg(_Modules);
	dlg.DoModal();
    return 0;
}

int CControlDlg::GetLBIndexFromHWND(HWND hwnd)
{
	int count(lb.GetCount() );
	int i(0);
	while(i < count && hwnd != reinterpret_cast<HWND>(lb.GetItemData(i)))
		i++;

	return (i < count) ? i : LB_ERR;
}

void CControlDlg::AddDeskletToList(Docklet* d)
{
	lb.LockWindowUpdate(TRUE);



	std::wstring label = d->GetDLL()->GetName();
	CString mainLbl = label.c_str();
	mainLbl.Replace('\t', ' ');
	mainLbl.Replace(_T("\r\n"), _T(" / "));
	
	std::wstring label2;
	std::wstring lbl(d->GetLabel().GetMainCaption());
	if(d->GetSkinInfo().IsAssigned())
	{
		if(lbl.length() > 0)
			label2 += L"("+ d->GetSkinInfo().GetName() + L"): ";
		else
			label2 += d->GetSkinInfo().GetName();
	}

	label2 += lbl;
	CString subLbl = label2.c_str();

	subLbl.Replace('\t', ' ');
	subLbl.Replace(_T("\r\n"), _T(" / "));

	// make AveScripter desklet show they title of their skin first, since
	// it looks prettier :-)
	if(mainLbl == L"AveScripter")
	{
		CString tmp(mainLbl);
		mainLbl = subLbl;
		subLbl = tmp;
	}


	ILBITEM item = {0};
	item.mask = ILBIF_STYLE | ILBIF_FORMAT | ILBIF_PARAM;

	if(_Policies.GetBoolValue(L"runtime//controlPanel//list//showPreviewImage", TRUE))
		item.mask |= ILBIF_IMAGE;	

	if(_Policies.GetBoolValue(L"runtime//controlPanel//list//showTitle", TRUE))
		item.mask |= ILBIF_TEXT;	

	if(_Policies.GetBoolValue(L"runtime//controlPanel//list//showSubTitle", TRUE))
		item.mask |= ILBIF_SUBTEXT;	

	item.iItem = lb.GetCount();
	item.pszText = const_cast<WCHAR*>(mainLbl.operator LPCTSTR());
	item.pszSubText = const_cast<WCHAR*>(subLbl.operator LPCTSTR());
	item.style = ILBS_IMGLEFT | ILBS_VCENTERTEXT;
	item.format |= DT_LEFT;
	item.iImage = m_DockletToImageListIndexMapping.size();
	item.lParam = reinterpret_cast<LPARAM>(d->GetHWnd());

	EnterCriticalSection(&m_CriticalSection);
	m_DockletToImageListIndexMapping[d->GetHWnd()] = item.iImage + 1;
	LeaveCriticalSection(&m_CriticalSection);

	SIZE s = {0};
	m_ImageList.GetIconSize(s);
	SIZE drawSize = {s.cx, s.cy};
	POINT drawPoint = {0, 0};
	SIZE curSize = d->GetDocklet()->GetSize();
	if(curSize.cx > curSize.cy && curSize.cy != 0)
	{
		float ratio = float(curSize.cx) / float(curSize.cy);
		drawSize.cy = int( float(drawSize.cy) / ratio );
		if(drawSize.cy % 2 == 1)
			drawSize.cy += 1;
		drawPoint.y = (s.cy - drawSize.cy) / 2;
	}
	else if(curSize.cy > curSize.cx && curSize.cx != 0)
	{
		float ratio = float(curSize.cy) / float(curSize.cx);
		drawSize.cx = int( float(drawSize.cx) / ratio );
		if(drawSize.cx % 2 == 1)
			drawSize.cx += 1;
		drawPoint.x = (s.cx - drawSize.cx) / 2;
	}

	Bitmap bmp(s.cx, s.cy);
	Graphics g(&bmp);

	if(GetIsRunningWin2k() == TRUE)
		g.Clear(Color(::GetSysColor(COLOR_WINDOW)));

	if(d->GetDocklet()->GetIsDirty())// && d->GetStyle() == STYLE_SHOWCASE)
		d->GetDocklet()->Draw();

	d->GetDocklet()->DrawCachedBitmap(g,drawSize,drawPoint);
	//d->GetDocklet()->DrawDragDropImage(g,drawSize, xMargin, yMargin);

	CBitmap hBmp;
	bmp.GetHBITMAP(NULL,&hBmp.m_hBitmap);

	ImageList_Add(m_ImageList.m_hImageList, hBmp, NULL);


	int index = lb.AddItem(&item);//lb.InsertItem(&item);

	/*if(index != LB_ERR)
	{
		lb.SetItemData(index, reinterpret_cast<DWORD>(d->GetHWnd()));
	}*/

	d->Register(this);

	lb.LockWindowUpdate(FALSE);
}

void CControlDlg::OnDeskletAdded(HWND hwnd)
{
	if(hwnd != NULL && GetLBIndexFromHWND(hwnd) == LB_ERR)
	{
		Docklet* d = Docklet::FindDocklet(hwnd);
		if(d != NULL)
		{
			AddDeskletToList(d);
			/*.LockWindowUpdate(TRUE);

			std::wstring label = d->GetDLL()->GetName();
			if(d->GetSkinInfo().IsAssigned())
				label += L" ("+ d->GetSkinInfo().GetName() + L")";
			label += L" : " + d->GetLabel().GetMainCaption();

			CString lbl = label.c_str();
			lbl.Replace('\t', ' ');
			lbl.Replace(_T("\r\n"), _T(" / "));
			int index = lb.AddString(lbl);
			if(index != LB_ERR)
			{
				lb.SetItemData(index, (DWORD)hwnd);
			}

			d->Register(this);
			lb.LockWindowUpdate(FALSE);
			*/
		}	
	}
}

void CControlDlg::OnAllDeskletsReleased()
{
	lb.LockWindowUpdate(TRUE);
	lb.ResetContent();
	lb.LockWindowUpdate(FALSE);
}

LRESULT CControlDlg::OnGlobalDockletsNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Docklet::OM_Event event = static_cast<Docklet::OM_Event>(wParam);
	HWND hwnd = reinterpret_cast<HWND>(lParam);
	int index = GetLBIndexFromHWND(hwnd);

	switch(event)
	{
		case Docklet::OM_DeskletAdded:
			OnDeskletAdded(hwnd);
		break;

		case Docklet::OM_AllDeskletsAboutToBeReleased:
			OnAllDeskletsReleased();
		break;
	}

	return 0;
}

LRESULT CControlDlg::OnSingleDockletNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Docklet::OM_Event event = static_cast<Docklet::OM_Event>(wParam);
	HWND hwnd = reinterpret_cast<HWND>(lParam);
	int index = GetLBIndexFromHWND(hwnd);

	switch(event)
	{
		case Docklet::OM_Repaint:
		{
			//REDRAWNOTIFICATIONPARAMS* params = reinterpret_cast<REDRAWNOTIFICATIONPARAMS*>(lParam);
			if(index != -1)
			{
				EnterCriticalSection(&m_CriticalSection);
				Docklet* d = Docklet::FindDocklet(hwnd);
				LeaveCriticalSection(&m_CriticalSection);

				if(d != NULL)
				{
					SIZE s = {0};
					m_ImageList.GetIconSize(s);
					SIZE drawSize = {s.cx, s.cy};
					POINT drawPoint = {0, 0};

					SIZE curSize = {0};
					EnterCriticalSection(&m_CriticalSection);
					d = Docklet::FindDocklet(hwnd);
					if(d != NULL)
						curSize = d->GetDocklet()->GetSize();
					LeaveCriticalSection(&m_CriticalSection);

					if(curSize.cx > curSize.cy && curSize.cy != 0)
					{
						float ratio = float(curSize.cx) / float(curSize.cy);
						drawSize.cy = int( float(drawSize.cy) / ratio );
						if(drawSize.cy % 2 == 1)
							drawSize.cy += 1;
						drawPoint.y = (s.cy - drawSize.cy) / 2;
					}
					else if(curSize.cy > curSize.cx && curSize.cx != 0)
					{
						float ratio = float(curSize.cy) / float(curSize.cx);
						drawSize.cx = int( float(drawSize.cx) / ratio );
						if(drawSize.cx % 2 == 1)
							drawSize.cx += 1;
						drawPoint.x = (s.cx - drawSize.cx) / 2;
					}

					Bitmap bmp(s.cx, s.cy);
					Graphics g(&bmp);

					if(GetIsRunningWin2k()  == TRUE)
						g.Clear(Color(::GetSysColor(COLOR_WINDOW)));

					EnterCriticalSection(&m_CriticalSection);
					d = Docklet::FindDocklet(hwnd);
					if(d != NULL)
						d->GetDocklet()->DrawCachedBitmap(g,drawSize, drawPoint);
					LeaveCriticalSection(&m_CriticalSection);

						//d->GetDocklet()->DrawInSimpleForm(g,drawSize,drawPoint);
						//d->GetDocklet()->DrawDragDropImage(g,drawSize, xMargin, yMargin);

					CBitmap hBmp;

					bmp.GetHBITMAP(NULL,&hBmp.m_hBitmap);

					EnterCriticalSection(&m_CriticalSection);
					int index = GetLBIndexFromHWND(hwnd);
					int imageListIndex = m_DockletToImageListIndexMapping[hwnd] - 1;
					if(imageListIndex != -1)
						m_ImageList.Replace(imageListIndex, hBmp, NULL);
								
					RECT boundingRect = {0};
					::SendDlgItemMessage(m_hWnd, IDC_LIST, LB_GETITEMRECT, index, reinterpret_cast<LPARAM>(&boundingRect));
					boundingRect.right = s.cx + 3;
	
					lb.InvalidateRect(&boundingRect,FALSE);
					LeaveCriticalSection(&m_CriticalSection);
				}
			}
		}
		break;
		case Docklet::OM_AboutToBeRemoved:
			if(index != LB_ERR)
			{
				lb.LockWindowUpdate(TRUE);
				lb.DeleteString(index);

				// TODO NO REMOVAL FROM THE IMAGE LIST YET, NEED TO BE MADE THREAD-SAFE, PLUS
				// ALL other indeces need to be updated.
				// int imageListIndex = m_DockletToImageListIndexMapping[hwnd] - 1;
				// ImageList_Remove(m_ImageList.m_hImageList, imageListIndex);
				// m_DockletToImageListIndexMapping.erase(hwnd);
				lb.LockWindowUpdate(FALSE);
			}
		break;

		case Docklet::OM_CaptionChanged:
		case Docklet::OM_SkinChanged:
			if(index != LB_ERR)
			{
				lb.LockWindowUpdate(TRUE);
				Docklet* d = Docklet::FindDocklet(hwnd);

				std::wstring label = d->GetDLL()->GetName();
				CString mainLbl = label.c_str();
				mainLbl.Replace('\t', ' ');
				mainLbl.Replace(_T("\r\n"), _T(" / "));
	
				std::wstring label2;
				if(d->GetSkinInfo().IsAssigned())
					label2 += L"("+ d->GetSkinInfo().GetName() + L"): ";

				label2 += d->GetLabel().GetMainCaption();
				CString subLbl = label2.c_str();

				subLbl.Replace('\t', ' ');
				subLbl.Replace(_T("\r\n"), _T(" / "));

				

				ILBITEM item = {0};
				if(_Policies.GetBoolValue(L"runtime//controlPanel//list//showSubTitle", TRUE))
					item.mask |= ILBIF_SUBTEXT;
				item.iItem = index;
				//item.pszText = const_cast<WCHAR*>(mainLbl.operator LPCTSTR());
				item.pszSubText = const_cast<WCHAR*>(subLbl.operator LPCTSTR());
				//item.iImage = 0;
				//item.lParam = reinterpret_cast<LPARAM>(d->GetHWnd());

				lb.SetItem(&item);

				/*std::wstring label = d->GetDLL()->GetName();
				if(d->GetSkinInfo().IsAssigned())
					label += L" ("+ d->GetSkinInfo().GetName() + L")";
				label += L" : " + d->GetLabel().GetMainCaption();

				CString lbl = label.c_str();
				lbl.Replace('\t', ' ');
				lbl.Replace(_T("\r\n"), _T(" / "));
				int index = lb.AddString(lbl);
				if(index != LB_ERR)
				{
					lb.SetItemData(index, (DWORD)hwnd);
				}*/

				lb.LockWindowUpdate(FALSE);
			}
		break;
	}

	return 0;
}

void CControlDlg::Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2)
{
	if( !IsWindow() )
		return;

	if(Docklet::GetGlobalDockletsEventNotifier() == source )
	{
		EnterCriticalSection(&m_CriticalSection);
		PostMessage(WM_GLOBALDOCKLETSNOTIFY,static_cast<WPARAM>(val1),val2);
		LeaveCriticalSection(&m_CriticalSection);
	}
	else
	{
		PostMessage(WM_SINGLEDOCKLETNOTIFY,static_cast<WPARAM>(val1),val2);
	}
}

//////////////////////////////////////////////////////////////////////////////
// reusable stuff
//////////////////////////////////////////////////////////////////////////////

void CControlDlg::UpdateList()
{
	std::vector<Docklet*> docklets;
	EnterCriticalSection(&Docklet::cs);
	for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin();iter!=Docklet::Docklets.end();++iter)
		docklets.push_back(iter->second);
	LeaveCriticalSection(&Docklet::cs);

    for(std::vector<Docklet*>::iterator d_iter = docklets.begin();d_iter!=docklets.end();++d_iter)
	{
		Docklet* d = *d_iter;
		if(d != NULL)
			AddDeskletToList(d);
	}

}

void CControlDlg::ShowOrActivate()
{
	if(!_Policies.GetBoolValue(L"runtime//controlPanel//allow", TRUE))
		return;

    if( IsWindow() )
	{
		if( IsIconic() )
			ShowWindow(SW_RESTORE);
		// SetFocus() didn't seem to work, so we cheat a bit:
		HMODULE hMod = LoadLibrary(_T("user32.dll"));
		VOID (__stdcall *SwitchToThisWindow)(HWND,BOOL) = (VOID (__stdcall *)(HWND,BOOL)) GetProcAddress(hMod,"SwitchToThisWindow");
		SwitchToThisWindow(m_hWnd,FALSE);
		FreeLibrary(hMod);
	}
    else
	{
        Create(GetDesktopWindow());
		ShowWindow(SW_SHOW);
	}

}

void CControlDlg::UpdateMenu(bool DoModules)
{
    // adding tools
    CMenu root, extras;
    root.LoadMenu(IDR_MENUACTIVE);
    extras = root.GetSubMenu(3);

    CString str;
    char c = '0';
    for( int i = ID_EXTRAS_TOOL0; i < ID_EXTRAS_TOOL9+1; i++ )
    {
        int ii = i - ID_EXTRAS_TOOL0;
        MENUITEMINFO mif = 
        {
            sizeof(MENUITEMINFO), 
            MIIM_DATA|MIIM_STATE|MIIM_STRING, 
            0
        };
        mif.dwTypeData = str.GetBuffer(MAX_PATH+1);
        mif.cch = MAX_PATH;
        if( extras.GetMenuItemInfo(i, FALSE, &mif) )
        {
            SetLastError(0);
            if( ii < static_cast<int>(_Tools.tools.size()) )
            {
                str.ReleaseBuffer();
                str.Format(IDS_TOOL, _Tools.tools[ ii ].Display, c++);
                mif.dwTypeData = str.GetBuffer(str.GetLength());
                mif.fState = MF_ENABLED;
                mif.fMask = MIIM_DATA|MIIM_STATE|MIIM_STRING;
                mif.cch = str.GetLength();
                mif.dwItemData = i - ID_EXTRAS_TOOL0;
            }
            else
            {
                str.ReleaseBuffer();
                str.Format(IDS_EMPTYTOOL, ii, c++ );
                mif.dwTypeData = str.GetBuffer(str.GetLength());
                mif.fState = MF_GRAYED;
                mif.fMask = MIIM_DATA|MIIM_STATE|MIIM_STRING;
                mif.cch = str.GetLength();
                mif.dwItemData = (ULONG_PTR)-1;
            }
            str.ReleaseBuffer();
            extras.SetMenuItemInfo(i, FALSE, &mif);
        }
    }

    // adding modules from additionalModules, now only on startup
    if( DoModules )
    {
		// BUGFIX: Module Menu should always be enabled!
		 extras.EnableMenuItem(0, MF_BYPOSITION|MF_ENABLED);

		std::vector<CModule*> modules = _Modules->GetCurrentModules();
        //if( modules.size() )
        {
            CMenu mod = extras.GetSubMenu(0);
            int iMenus = mod.GetMenuItemCount();
            for(int i = 2; i < iMenus; i++ )
            {
                mod.RemoveMenu(i, MF_BYPOSITION);
            }
            for(UINT i = 0; i < modules.size(); i++ )
            {
                CModule *md  = modules[i];
                mod.AppendMenu(MF_STRING, ID_EXTRAS_MODULES_MODULE + i, md->GetName());
            }
            mod.Detach();
        } 
    }
    ::SetMenu(m_hWnd, root);
    DrawMenuBar();
    extras.Detach();
    root.Detach();


}
