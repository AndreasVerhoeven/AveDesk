// GUIVersion.cpp : main source file for GUIVersion.exe
//

#include "stdafx.h"

#include "resource.h"

#include "SelectDeskletFilesDlg.h"
#include "MainDlg.h"
#include "TrayIconDlg.h"
#include "BootDlg.h"
#include "VersionDlg.h"
#include "ToolSettings.h"

CMyAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	_Module.m_Allocator.Init();

	int nRet = 0;
	// BLOCK: Run application
	{
		ToolSettings ts;

		HPROPSHEETPAGE pages[5] = {0};
		CMainDlg mainDlg;
		mainDlg.ts = &ts;
		CSelectDeskletFilesDlg dlgSelectDeskletFilesDlg;
		dlgSelectDeskletFilesDlg.ts = &ts;
		CTrayIconDlg trayIconDlg;
		trayIconDlg.ts = &ts;
		CBootDlg bootDlg;
		bootDlg.ts = &ts;
		CVersionDlg versionDlg;
		versionDlg.ts = &ts;

		pages[0] = CreatePropertySheetPage(mainDlg);
		pages[1] = CreatePropertySheetPage(dlgSelectDeskletFilesDlg);
		pages[2] = CreatePropertySheetPage(trayIconDlg);
		pages[3] = CreatePropertySheetPage(bootDlg);
		pages[4] = CreatePropertySheetPage(versionDlg);

		PROPSHEETHEADER hdr = {0};
		hdr.dwSize = sizeof(PROPSHEETHEADER);
		hdr.dwFlags = PSH_WIZARD;
		hdr.nStartPage = 0;
		hdr.nPages = sizeof(pages) / sizeof(pages[0]);
		hdr.phpage = pages;
		hdr.hwndParent = 0;
		hdr.pszCaption = _T("Intro");
		hdr.pfnCallback = 0;

		int res =PropertySheet(&hdr);
	}

	_Module.m_Allocator.Term();
	_Module.Term();
	::CoUninitialize();

	return nRet;
}
