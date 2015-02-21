// IconWindow.cpp: implementation of the CIconWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IconWindow.h"
#include "Docklet.h"
#include "Modules.h"
#include "Showcase1.h"
#include "AveDeskGlobalSettings.hpp"
#include "Application.h"

extern CComObject<CShowCase> *_ShowCase;
extern CModules* _Modules;
extern AveDeskGlobalSettings _globalSettings;
extern CComObject<CApplication> *_App;
extern bool _noMemCleaner;

#include "Policies.h"
extern CPolicies _Policies;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconWindow::CIconWindow() : themesAlreadyFilled(false)
{

}

CIconWindow::~CIconWindow()
{

}

void CIconWindow::CleanUpPhysicalMemUsage()
{
	SetProcessWorkingSetSize(GetCurrentProcess(),(SIZE_T)-1,(SIZE_T)-1);
}

void CIconWindow::MakeIcon()
{
	std::wstring iconFilename = _Policies.GetStringValue(L"ui//trayicon//iconfile", L"");
	_ncd.cbSize = sizeof(_ncd);
	_ncd.hWnd = m_hWnd;
	if(iconFilename.length() == 0)
		_ncd.hIcon = ExtractIconW(_Module.GetModuleInstance(),_globalSettings.GetIconFileName().c_str(),_globalSettings.GetIconIndex());
	else
	{
		WCHAR iconPath[MAX_PATH] = {0};
		wcscpy_s(iconPath, MAX_PATH, _App->GetAveDeskDataDir().c_str());
		PathAppendW(iconPath, iconFilename.c_str());
		_ncd.hIcon = (HICON)LoadImage(NULL, iconPath,IMAGE_ICON,16, 16,LR_LOADFROMFILE);
	}
	std::wstring alternateTip(_Policies.GetStringValue(L"ui//trayicon//tooltip", L"AveDesk 2.0"));
	if(alternateTip.length() > 0)
		wcscpy(_ncd.szTip, alternateTip.c_str());
	else
		wcscpy(_ncd.szTip,L"AveDesk 2.0");
	_ncd.uCallbackMessage = ICON_CALLBACK;
	_ncd.uID = 99;
	_ncd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// create memory cleaner timer
	if(!::_noMemCleaner)
		SetTimer(CLEANER_TIMER,30000);
	
	if(_globalSettings.GetShowTrayIcon() )
		Shell_NotifyIconW(NIM_ADD,&_ncd);

	DestroyIcon(_ncd.hIcon);
}

void CIconWindow::KillIcon()
{
	Shell_NotifyIconW(NIM_DELETE,&_ncd);

	// kill cleaner timer
	KillTimer(CLEANER_TIMER);

	DestroyWindow();
}

	
LRESULT CIconWindow::OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MakeIcon();
	Docklet::RepairDesktopWindows();

	return 0;
}

LRESULT CIconWindow::OnAveLoadModule(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// thread-marshalling modules
	return reinterpret_cast<LONG>(_Modules->LoadModule(reinterpret_cast<WCHAR*>(lParam)));
}

LRESULT CIconWindow::OnAveRemoveModule(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// thread-marshalling modules
	_Modules->RemoveModule(static_cast<int>(lParam));
	return 0;
}

LRESULT CIconWindow::OnAveClearModules(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// thread-marshalling modules
	_Modules->ClearCurrentModules();
	return 0;
}

LRESULT CIconWindow::OnQueryEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	/*
	if(_Policies.GetBoolValue(L"closing//saveSettings", TRUE))
		_App->SaveGlobalSettings();

	if(_Policies.GetBoolValue(L"closing//saveTheme", TRUE) && _globalSettings.GetAutoSaveThemes() )
	{
		CComBSTR bstrThemeFile(_App->GetThemeFileName().c_str());
		_App->SaveTheme(bstrThemeFile);
	}*/
	PostQuitMessage(0);
	return 1;
}

LRESULT CIconWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(CLEANER_TIMER == wParam)
	{
		CleanUpPhysicalMemUsage();
		CoFreeUnusedLibraries();
	}
	return 0;
}

LRESULT CIconWindow::OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CMenu menu((HMENU)wParam);

	if(themesAlreadyFilled)
	{
		menu.Detach();
		return 0;
	}

	// there are always 3 default menu items in the theme-submenu
	if(menu.m_hMenu != popupMenu.m_hMenu)
	{
		if(_Policies.GetBoolValue(L"ui//trayicon//menu//ownmenu", FALSE))
			return 0;

		if(!_Policies.GetBoolValue(L"ui//trayicon//themesMenuItem//allowCustomBrowsing", TRUE))
		{
			RemoveMenu(menu, 2, MF_BYPOSITION);
			RemoveMenu(menu, 1, MF_BYPOSITION);
			RemoveMenu(menu, 0, MF_BYPOSITION);
		}

		// search for themes in the theme-directory
		WIN32_FIND_DATAW data = {0};
		WCHAR path[MAX_PATH] = {0};
		wcscpy(path,_App->GetAveDeskThemeDir().c_str());
		PathAppendW(path,L"*.avetheme");
		HANDLE h = FindFirstFileW(path,&data);
		if(h != INVALID_HANDLE_VALUE)
		{
			int i(0);
			do
			{
				if(wcscmp(data.cFileName,L".") != 0 && wcscmp(data.cFileName,L"..") != 0)
				{
					m_filenames.push_back(data.cFileName);
					menu.AppendMenu(MF_STRING | MF_ENABLED,i+1,data.cFileName);
					i++;
				}
				
			}while(FindNextFileW(h, &data));
			FindClose(h);
		}

		themesAlreadyFilled = true;
	}

	menu.Detach();

	return 0;
}

void CIconWindow::OnTrayIconLeftButtonDoubleClick()
{
	_App->ShowControlPanel();
}

void CIconWindow::OnTrayIconRightButtonClick()
{
	if(!_Policies.GetBoolValue(L"ui//trayicon//menu//allow", TRUE))
		return;

	POINT p = {0};
	// didn't work properly (wrong coordinates): POINT p ={GET_X_LPARAM(GetMessagePos()), GET_Y_LPARAM(GetMessagePos())};
	GetCursorPos(&p);

	SetForegroundWindow(m_hWnd);

	if(_Policies.GetBoolValue(L"ui//trayicon//menu//ownmenu", FALSE))
	{
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenuW(MF_STRING | MF_ENABLED, 1, L"Close");
		int cmd = TrackPopupMenuEx(menu, TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, p.x, p.y, m_hWnd,0);
		if(1 == cmd)
			PostQuitMessage(0);

		
		return;
	}

	CMenu menuBar;
	menuBar.LoadMenu(IDR_POPUP);

	popupMenu = menuBar.GetSubMenu(0); // first sub-menu of the menuBar is our menu
	HMENU menu = popupMenu.m_hMenu;

	if(!_Policies.GetBoolValue(L"ui//trayicon//menu//themesMenuItem//allow", TRUE))
		RemoveMenu(menu, 4, MF_BYPOSITION);
	
	MENUITEMINFO info = {0};
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	info.fState = MFS_DEFAULT;
	SetMenuItemInfo(menu,ID_DESKLETSCONTROLPANEL,FALSE,&info);

	VARIANT_BOOL showCaseActive = VARIANT_FALSE;
	_ShowCase->get_Active(&showCaseActive);
	RemoveMenu(menu, showCaseActive ? ID_OPENSHOWCASE : ID_CLOSESHOWCASE,MF_BYCOMMAND);

	if(!_Policies.GetBoolValue(L"ui//controlPanel//allow", TRUE))
		RemoveMenu(menu, ID_DESKLETSCONTROLPANEL, MF_BYCOMMAND);

	if(!_Policies.GetBoolValue(L"ui//trayicon//menu//showAddNewDeskletItem", TRUE))
		RemoveMenu(menu, ID_ADDADESKLET, MF_BYCOMMAND);

	if(!_Policies.GetBoolValue(L"ui//trayicon//menu//showShowCaseItem", TRUE))
		RemoveMenu(menu, !showCaseActive ? ID_OPENSHOWCASE : ID_CLOSESHOWCASE,MF_BYCOMMAND);

	if(!_Policies.GetBoolValue(L"ui//trayicon//menu//showCloseItem", TRUE))
		RemoveMenu(menu, ID_CLOSE,MF_BYCOMMAND);


	m_filenames.clear();

	themesAlreadyFilled = false;

	int cmd = TrackPopupMenuEx(menu, TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, p.x, p.y, m_hWnd,0);

	if(cmd > 0 && cmd <= (int)m_filenames.size())
	{
		// first, save old theme
		if(_globalSettings.GetAutoSaveThemes() )
			_App->SaveTheme( CComBSTR(_App->GetThemeFileName().c_str()) );

		std::wstring theme = m_filenames[cmd-1];
		_App->SetThemeFileName(theme);
		_App->LoadTheme( CComBSTR(_App->GetThemeFileName().c_str()) );
		::SetFocus(FindWindow(_T("Progman"),NULL));
	}
	else
	{
		switch(cmd)
		{
			case ID_OPENBROWSER:
			{
				_App->ShowThemeBrowserDialog();
			}
			break;

			case ID_LOADTHEME:
			{
				USES_CONVERSION;

				CFileDialog fd(TRUE,												// TRUE for OpenDialog
								_T("avetheme"),										// default extension
								W2T(const_cast<WCHAR*>(_App->GetThemeFileName().c_str())),	// filename
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,				// flags
								_T("AveDesk Themes(*.avetheme)\0*.avetheme\0All Files(*.*)\0*.*\0\0"), // filter
								NULL);

				if( fd.DoModal() == TRUE)
				{
										
					// first, save the old theme silently!
					if(_Policies.GetBoolValue(L"closing//saveTheme", TRUE) && _globalSettings.GetAutoSaveThemes() )
						_App->SaveTheme(CComBSTR(_App->GetThemeFileName().c_str()) );
					
					_App->SetThemeFileName(T2W(fd.m_szFileName) );
					_App->LoadTheme(CComBSTR(_App->GetThemeFileName().c_str()) );
				}		
			}
			break;

			case ID_ADDADESKLET:
				_App->ShowDeskletBrowserDialog();
			break;

			case ID_DESKLETSCONTROLPANEL:
				_App->ShowControlPanel(); 
			break;

			case ID_CLOSE:
			{
				CString str;
				str.LoadString(IDS_CLOSE);
				if(_globalSettings.GetAsk() && ::MessageBox(m_hWnd,str,_T("AveDesk"),MB_ICONQUESTION| MB_YESNO)==IDNO)
					break;
				else
					PostQuitMessage(0);
			}
			break;

			case ID_OPENSHOWCASE:
				_ShowCase->Activate();
			break;

			case ID_CLOSESHOWCASE:
				_ShowCase->Deactivate();
			break;
			break;
		}
	}

	popupMenu.DestroyMenu();
}

LRESULT CIconWindow::OnIconCallback(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(lParam)
	{
		case WM_LBUTTONDBLCLK: OnTrayIconLeftButtonDoubleClick(); break;
		case WM_RBUTTONUP:	   OnTrayIconRightButtonClick();      break;
	}

	return 0;
}