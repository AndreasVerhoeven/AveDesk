// DeskletBrowserDlg.cpp: implementation of the CDeskletBrowserDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletBrowserDlg.h"

#include "helper_funcs.h"
#include "AveDeskGlobalSettings.hpp"
//#include "DragAnimationWindow.h"
#include "DeskletItemDragAnimation.h"
#include "Pidl.h"
//#include "NewFeedDlg.h"
#include "NewListDlg.h"
#include "AveScripterWarningDlg.h"
#include "InstalledDeskletProvider.h"
#include "XMLLoader.h"
#include "XMLSaver.h"
#include <sstream>

extern AveDeskGlobalSettings _globalSettings;

#include "Application.h"
extern CComObject<CApplication>* _app;

void TryAndThrow_(HRESULT hr)
{
	if(FAILED(hr))
		throw hr;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletBrowserDlg::CDeskletBrowserDlg() :
	view(NULL),
	providerFactory(NULL),
	curCollectionSel(-1),
	warningDlg(NULL)
{
	providerFactory = new CDeskletProviderFactory;
}

BOOL CDeskletBrowserDlg::AveScripterInstalled()
{
	if(NULL == _App)
		return FALSE;

	WCHAR path[MAX_PATH] = {0};
	wcscpy_s(path, MAX_PATH, _App->GetAveDeskDataDir().c_str());
	PathAppendW(path, L"Desklets\\AveScripter\\AveScripter.dll");
	return PathFileExists(path);
}

LRESULT CDeskletBrowserDlg::OnNewFeedButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//CNewFeedDlg dlg;
	CNewListDlg dlg;
	BOOL res = dlg.DoModal();
	if(res != FALSE)
	{
		CDeskletCollection* col = new CDeskletCollection();
		for(std::vector<CDeskletFilter>::iterator iter = dlg.filterData.begin(); iter != dlg.filterData.end(); ++iter)
			col->AddFilter(*iter);

		DeskletsViewState vs;
		vs.curSel = -1;
		vs.top = 0;
		viewStates.push_back(vs);
		collections.push_back(col);
		col->SetProvider(providers[L"installed"]);
		col->SetName(dlg.name);
		col->SetIconFileName(dlg.iconFileName);
		CPidl pidl;
		pidl.SetFromPath(dlg.iconFileName.c_str());
		HICON hIcon = pidl.ExtractIcon(48);
		ImageList_AddIcon(imgListCats.m_hImageList, hIcon);
		::DestroyIcon(hIcon);

		cats.AddString(dlg.name.c_str());
	}

	return 0;
}

void CDeskletBrowserDlg::DeleteCategory(int index)
{
	// CHECK FOR sel > 0, because the first item (with index 0), cannot be deleted
	if(index != LB_ERR && index > 0 && index < (int)collections.size())
	{
		cats.DeleteString(index);
		ImageList_Remove(imgListCats.m_hImageList, index);

		CDeskletCollection* col = collections[index];
		col->SetProvider(NULL);

		lb.SetCurSel(0);
		view->SetCollection(collections[0]);

		delete col;
		collections.erase(collections.begin() + index);
	}
}

LRESULT CDeskletBrowserDlg::OnDeleteFeedButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int sel = cats.GetCurSel();
	if(sel != LB_ERR && sel > 0 && sel < (int)collections.size())
		DeleteCategory(sel);

	return 0;
}

LRESULT CDeskletBrowserDlg::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	const HWND hwndCtrl = reinterpret_cast<const HWND>(wParam);
	POINT ptScreen = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	if(hwndCtrl == cats.m_hWnd)
	{
		POINT pt = ptScreen;
		cats.ScreenToClient(&pt);
		BOOL outside(FALSE);
		int itemIndex = cats.ItemFromPoint(pt, outside);
		if(itemIndex != LB_ERR)
		{
			cats.SetCurSel(itemIndex);

			// the first item cannot be edited
			if(itemIndex > 0)
			{
				CMenu menuBar;
				menuBar.LoadMenu(IDR_POPUP);

				CMenu popupMenu = menuBar.GetSubMenu(3); // fourth sub-menu of the menuBar is our menu
				int cmd = static_cast<int>(popupMenu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, ptScreen.x, ptScreen.y, m_hWnd,0));
				if(ID_CATEGORYPOPUPMENU_EDIT == cmd)
				{
					CDeskletCollection* col = collections[itemIndex];
					CNewListDlg dlg;
					dlg.filterData = col->GetFilters();
					dlg.iconFileName = col->GetIconFileName();
					dlg.name = col->GetName();

					BOOL res = dlg.DoModal();
					if(res != FALSE)
					{
						col->ClearFilters();
						col->SetIconFileName(dlg.iconFileName);
						col->SetName(dlg.name);
						for(std::vector<CDeskletFilter>::iterator iter = dlg.filterData.begin(); iter != dlg.filterData.end(); ++iter)
							col->AddFilter(*iter);

						col->SetProvider(providers[L"installed"]);

						cats.SetRedraw(FALSE);
						CPidl pidl;
						pidl.SetFromPath(dlg.iconFileName.c_str());
						HICON hIcon = pidl.ExtractIcon(48);
						ImageList_ReplaceIcon(imgListCats.m_hImageList, itemIndex, hIcon);
						::DestroyIcon(hIcon);

						cats.DeleteString(itemIndex);
						cats.InsertString(itemIndex, col->GetName().c_str());
						cats.SetCurSel(itemIndex);
						cats.SetRedraw(TRUE);

						lb.SetRedraw(FALSE);
						view->ClearItems();
						col->Refresh();
						lb.SetTopIndex(viewStates[itemIndex].top);
						lb.SetCurSel(viewStates[itemIndex].curSel);
						
						lb.SetRedraw(TRUE);

					}
				}
				else if(ID_CATEGORYPOPUPMENU_DELETE == cmd)
				{
					DeleteCategory(itemIndex);
				}
			}
		}

	}

	return 0;
}

LRESULT CDeskletBrowserDlg::OnListBoxStartsDragging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(view != NULL)
	{
		CDeskletItem* item = view->GetItem(static_cast<UINT>(wParam));
		if(item != NULL)
		{
			CDeskletItemDragAnimation* anim = new CDeskletItemDragAnimation;
			SIZE s = {80, 80};
			anim->StartAnimation(item, s, m_hWnd);
			item->Release();
		}
	}
	return 0;
}

LRESULT CDeskletBrowserDlg::OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	
	int curSel = lb.GetCurSel();
	if(curSel != LB_ERR)
	{
		CDeskletItem* item = view->GetItem(curSel);
		if(item != NULL)
		{
			item->Instantiate(FALSE, NULL, TRUE, NULL);
			item->Release();
		}
	}
	return 0;
}

LRESULT CDeskletBrowserDlg::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int curSel = lb.GetCurSel();
	::EnableWindow(GetDlgItem(IDC_ADDDESKLET), curSel != LB_ERR && curSel != 0);
	return 0;
}


LRESULT CDeskletBrowserDlg::OnSearchBoxTextChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	WCHAR searchString[MAX_PATH] = {0};
	if(wNotifyCode != CBN_SELCHANGE)
		GetDlgItemText(IDC_SEARCHBOX, searchString, MAX_PATH);
	else if(searchBox.GetLBTextLen(searchBox.GetCurSel()) < MAX_PATH)
	{
		CString searchDefault;
		searchDefault.LoadString(IDS_SEARCHDEFAULT);
		CString searchAllDesklets;
		searchAllDesklets.LoadString(IDS_SEARCHALLDESKLETS);

		searchBox.GetLBText(searchBox.GetCurSel(), searchString);
		if(_wcsicmp(searchString, searchAllDesklets) == 0)
		{
			SetDlgItemText(IDC_SEARCHBOX, L"");
			wcscpy(searchString, L"");
		}
	}
	lowercaseW(searchString);

	view->SetSearchText(searchString);
	return 0;
}

CDeskletBrowserDlg::~CDeskletBrowserDlg()
{
	Cleanup();
	delete providerFactory;
	delete warningDlg;
}

int CDeskletBrowserDlg::ChangeIconSize(int newSize)
{
	
	int pos = newSize;

	if(pos < 28)
	{
		//lb.SetImageList(NULL, ILSIL_NORMAL);
		//iconSlider.SetPos(0);
		pos = 0;
	}
	//else
		//lb.SetImageList(m_ImageList, ILSIL_NORMAL);

//	m_ImageList.SetIconSize(pos, pos);
	lb.iconSize.cx = pos;
	lb.iconSize.cy = pos;
	lb.SetItemHeight(0, max(pos + 4, 16));

		

	::ShowScrollBar(lb.m_hWnd, SB_VERT, FALSE);
	RECT r = {0};
	lb.GetWindowRect(&r);
	if(max(pos + 4, 20) * lb.GetCount() > r.bottom - r.top)
	{
		::ShowScrollBar(lb.m_hWnd, SB_VERT, TRUE);
	}

	_globalSettings.SetIconPreviewSize(static_cast<UINT>(pos));

	lb.InvalidateRect(NULL, TRUE);

	return pos;
}

LRESULT CDeskletBrowserDlg::OnListSelChangeCat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(NULL == view)
		return 0;

	lb.SetRedraw(FALSE);
	int curSel = cats.GetCurSel();
	if(curCollectionSel != -1)
	{
		if(curCollectionSel < static_cast<int>(viewStates.size()))
		{
			viewStates[curCollectionSel].curSel = view->GetSel();
			viewStates[curCollectionSel].top = view->GetTopItem();
		}
	}

	CString str;
	str.GetBuffer(cats.GetTextLen(curSel)+1);
	cats.GetText(curSel, str.LockBuffer());
	str.UnlockBuffer();
	str.MakeLower();

	if(str == L"avescripter" && !AveScripterInstalled())
	{
		if(NULL == warningDlg)
		{
			warningDlg = new CAveScripterWarningDlg;
			warningDlg->hwndListBox = lb.m_hWnd;
			warningDlg->Create(m_hWnd);
			RECT rc = {0};
			lb.GetWindowRect(&rc);
			::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
			rc.left += 0;
			rc.right -= 0;
			rc.top += 0;
			rc.bottom -= 0;
			warningDlg->SetWindowPos(NULL, &rc, SWP_NOZORDER);
		}
		warningDlg->ShowWindow(SW_SHOW);
		lb.ShowWindow(SW_HIDE);
	}
	else
	{
		if(warningDlg != NULL)
		{
			warningDlg->ShowWindow(SW_HIDE);
			lb.ShowWindow(SW_SHOW);
		}
	}

	::EnableWindow(GetDlgItem(IDC_DELETEFEED), curSel != LB_ERR && curSel != 0);
	if(curSel != LB_ERR)
	{
		if(curSel < static_cast<int>(collections.size()))
		{
			curCollectionSel = curSel;
			view->SetCollection(collections[curSel]);
			if(curCollectionSel < static_cast<int>(viewStates.size()))
			{
				view->SetSel(viewStates[curCollectionSel].curSel);
				view->SetTopItem(viewStates[curCollectionSel].top);
			}
		}
	}
	lb.SetRedraw(TRUE);

	return 0;
}

LRESULT CDeskletBrowserDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hwndScrollBar = (HWND) lParam;  
	if(GetDlgItem(IDC_ICONSLIDER)  == hwndScrollBar)
	{
		int pos = iconSlider.GetPos();
		ChangeIconSize(pos);
	}



	return 0;
}

LRESULT CDeskletBrowserDlg::OnUpdatedSplitterPos(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(::IsWindow(cats.m_hWnd))
	{
		RECT rc ={0};
		cats.GetWindowRect(&rc);
		int w = rc.right - rc.left;
		int h = min(70,w);
		if(h < 64) h = min(50, h);
		cats.SetItemHeight(0, h);
		cats.Invalidate(TRUE);
		RECT rcSplitter = {0};
		splitter.GetWindowRect(&rcSplitter);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcSplitter, 2);
		_globalSettings.SetDeskletBrowserSplitterPos((UINT)rcSplitter.left);
	}
	return 0;
}

BOOL CDeskletBrowserDlg::LoadCategories()
{
	// LOAD COLLECTIONS

	// make the default "all desklets" collection
	CDeskletCollection* allCollection = new CDeskletCollection();
	CString allDeskletsStr;
	allDeskletsStr.LoadStringW(IDS_ALLDESKLETS);
	allCollection->SetName(allDeskletsStr.operator LPCTSTR());
	allCollection->SetIconFileName(_T("c:\\"));
	collections.push_back(allCollection);


	// load previously persisted collections
	BOOL hasBeenSavedBefore = FALSE;
	try
	{
		WCHAR catFile[MAX_PATH] = {0};
		if(_App != NULL)
			wcscpy_s(catFile, MAX_PATH, _App->GetAveDeskUserPath().c_str());
		PathAppendW(catFile, L"categories.xml");
		CXMLLoader loader(catFile, L"AveDesk");
		TryAndThrow_(loader.SelectChildNode(L"categories"));
		std::wstring token(L"2.0");
		loader.LoadStringW(token, L"token");
		hasBeenSavedBefore = (token == L"2.0");

		TryAndThrow_(loader.SelectNodes(L"category"));
		long num = loader.NumberOfNodesInList();
		for(long i = 0; i < num; ++i)
		{
			bool bRes = loader.SelectNodeFromList(i);
			if(bRes)
			{
				std::wstring provider(L"installed");
				std::wstring name;
				std::wstring icon;
				
				loader.LoadStringW(provider, L"provider");
				loader.LoadStringW(name, L"name");
				loader.LoadStringW(icon, L"icon");

				CDeskletCollection* col = new CDeskletCollection;
				collections.push_back(col);
				col->SetName(name);
				col->SetIconFileName(icon);

				TryAndThrow_(loader.SelectChildNode(L"filters"));
				TryAndThrow_(loader.SelectNodes(L"filter"));
				long numFilters = loader.NumberOfNodesInList();
				for(long j = 0; j < numFilters; ++j)
				{
					bool bRes = loader.SelectNodeFromList(j);
					if(bRes)
					{
						std::wstring type(L"0");
						std::wstring keyword;

						loader.LoadStringW(type, L"type");
						loader.LoadStringW(keyword, L"keyword");

						CDeskletFilter filter((CDeskletFilter::FilterType)_wtoi(type.c_str()), keyword);
						col->AddFilter(filter);
						
						TryAndThrow_(loader.GoToParentNode());
					}
				}

				TryAndThrow_(loader.GoToParentNode());
				TryAndThrow_(loader.GoToParentNode());
				TryAndThrow_(loader.SelectNodes(L"category"));
			}
			
		}
	}
	catch(HRESULT)
	{
		// uhoh
	}

	return hasBeenSavedBefore;
}

BOOL CDeskletBrowserDlg::SaveCategories()
{
	if(collections.size() < 0)
		return FALSE;


	CXMLSaver saver(L"AveDesk");
	saver.AddNode(L"categories");
	saver.SaveString(L"2.0", L"token");

	// skip the first item, which is always "all desklets"
	for(std::vector<CDeskletCollection*>::iterator iter = collections.begin()+1; iter != collections.end(); ++iter)
	{
		CDeskletCollection* col = *iter;
		if(col != NULL)
		{
			saver.AddNode(L"category");
			saver.SaveString(L"installed", L"provider");
			saver.SaveString(col->GetName(), L"name");
			saver.SaveString(col->GetIconFileName(), L"icon");
			
			saver.AddNode(L"filters");
			std::vector<CDeskletFilter> filters(col->GetFilters());
			for(std::vector<CDeskletFilter>::iterator iter = filters.begin(); iter != filters.end(); ++iter)
			{
				CDeskletFilter& f = *iter;
				int type = static_cast<int>(f.GetType());
				std::wstringstream ws;
				ws << type;
				std::wstring typeStr = ws.str();

				saver.AddNode(L"filter");
				saver.SaveString(typeStr, L"type");
				saver.SaveString(f.GetKeyword(), L"keyword");

				saver.GoToParentNode();
			}
			saver.GoToParentNode();

			saver.GoToParentNode();
		}
	}	

	WCHAR catFile[MAX_PATH] = {0};
	if(_App != NULL)
		wcscpy_s(catFile, MAX_PATH, _App->GetAveDeskUserPath().c_str());
	PathAppendW(catFile, L"categories.xml");

	return SUCCEEDED(saver.Save(catFile));
}

LRESULT CDeskletBrowserDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if(GetParent() == NULL)
	ModifyStyleEx(0,WS_EX_APPWINDOW); // for appearance in task bar
    HICON hIco = LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE( IDI_ICON2 ));
    SetIcon( hIco, TRUE );
    SetIcon( hIco, FALSE );
    CenterWindow();

	splitter.SubclassWindow(GetDlgItem(IDC_SPLITTER));
	UINT splitterPos = _globalSettings.GetDeskletBrowserSplitterPos();
	splitter.SetSplitterPanes(GetDlgItem(IDC_CATEGORIES), GetDlgItem(IDC_DESKLETS));
	if(splitterPos > 0)
	{
		CWindow wnd = GetDlgItem(IDC_SPLITTER);
		RECT rc = {0};
		wnd.GetWindowRect(&rc);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
		int w = rc.right - rc.left;
		rc.left = splitterPos;
		rc.right = rc.left + w;
		wnd.MoveWindow(&rc, TRUE);
	}
	splitter.UpdateSplitterLayout();

	iconSlider = GetDlgItem(IDC_ICONSLIDER);
	iconSlider.SetRange(27,80);


	searchBox = GetDlgItem(IDC_SEARCHBOX);

	LOGFONTW lfBig = {0};
	lfBig.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfBig.lfFaceName,L"Tahoma");
	lfBig.lfWeight = FW_BOLD;
	lfBig.lfHeight = 14;

	LOGFONTW lfSmall = {0};
	lfSmall.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfSmall.lfFaceName,L"Tahoma");
	lfSmall.lfWeight = FW_NORMAL;
	lfSmall.lfHeight = 12;

	bigFont.CreateFontIndirect(&lfBig);
	smallFont.CreateFontIndirect(&lfSmall);


	cats.SubclassWindow(GetDlgItem(IDC_CATEGORIES));
	imgListCats.Create(48,48,ILC_COLOR32,1, 1);
	cats.SetImageList(imgListCats);
	cats.SetItemHeight(0, 70);
	RECT rc ={0};
	cats.GetWindowRect(&rc);
	int w = rc.right - rc.left;
	int h = min(70,w);
	if(h < 64) h = min(50, h);
	cats.SetItemHeight(0, h);

	m_ImageList.Create(80,80,ILC_COLOR32,1, 1);
	ImageList_AddIcon(m_ImageList.m_hImageList, (HICON)LoadIcon(_Module.GetModuleInstance(),MAKEINTRESOURCE(IDI_ICON3)));
	int iconSize = static_cast<int>(_globalSettings.GetIconPreviewSize());
	lb.SubclassWindow(GetDlgItem(IDC_DESKLETS));
	iconSize = ChangeIconSize(iconSize);
	iconSlider.SetPos(iconSize);

	::EnableWindow(::GetDlgItem(m_hWnd, IDC_ADDDESKLET), FALSE);

	DlgResize_Init();
	ToolTips_Init();


	BOOL hasBeenSavedBefore = LoadCategories();


	CDeskletProvider* installedProvider = providerFactory->CreateProvider(L"installed");
	if(installedProvider != NULL)
		providers.insert(std::make_pair(L"installed", installedProvider));


	if(!hasBeenSavedBefore)
	{
		CDeskletCollection* scripterCollection = new CDeskletCollection();
		scripterCollection->AddFilter(CDeskletFilter(CDeskletFilter::FilterTypeName, L"AveScripter"));
		scripterCollection->SetName(L"AveScripter");
		scripterCollection->SetIconFileName(L"Images\\Icons\\bar\\avescripter.ico");
		collections.push_back(scripterCollection);

		CDeskletCollection* sysstatsCollection = new CDeskletCollection();
		sysstatsCollection->AddFilter(CDeskletFilter(CDeskletFilter::FilterTypeName, L"SysStats"));
		sysstatsCollection->SetName(L"SysStats");
		sysstatsCollection->SetIconFileName(L"Images\\Icons\\bar\\sysstats.ico");
		collections.push_back(sysstatsCollection);
	}

	// load icons
	for(std::vector<CDeskletCollection*>::iterator iter = collections.begin(); iter != collections.end(); ++iter)
	{
		CDeskletCollection* coll = *iter;
		if(coll != NULL)
		{
			WCHAR iconFName[MAX_PATH] = {0};
			if(!PathFileExists(coll->GetIconFileName().c_str()) || coll->GetIconFileName().find(L':') == std::wstring::npos)
			{
				wcscpy_s(iconFName,MAX_PATH,_App->GetAveDeskDataDir().c_str());
				PathAppendW(iconFName, coll->GetIconFileName().c_str());
			}
			else
			{
				wcscpy_s(iconFName, MAX_PATH, coll->GetIconFileName().c_str());
			}

			if(!PathFileExists(iconFName) || iconFName[0] == L'\0')
			{
				// file not valid, just use avedesk.exe's icon
				GetModuleFileName(NULL, iconFName, MAX_PATH);
			}

			CPidl pidl;
			pidl.SetFromPath(iconFName);
			HICON hIcon = pidl.ExtractIcon(48);
			ImageList_AddIcon(imgListCats.m_hImageList, hIcon);
			::DestroyIcon(hIcon);
		}
	}

	viewStates.resize(collections.size());

	cats.SetRedraw(FALSE);
	view = new CDeskletsView();
	view->SetControl(&lb);
	lb.view = view;
	view->SetCollection(collections[0]);
	curCollectionSel = 0;
	for(std::vector<CDeskletCollection*>::iterator iter = collections.begin(); iter != collections.end(); ++iter)
	{
		CDeskletCollection* coll = *iter;
		if(coll != NULL)
			coll->SetProvider(providers[L"installed"]);

		cats.AddString(coll->GetName().c_str());
	}
	cats.SetRedraw(TRUE);

	installedProvider->Start();

	CString searchDefault;
	searchDefault.LoadString(IDS_SEARCHDEFAULT);
	CString searchAllDesklets;
	searchAllDesklets.LoadString(IDS_SEARCHALLDESKLETS);
	SetDlgItemTextW(IDC_SEARCHBOX,  searchDefault);
	searchBox.AddString(searchAllDesklets);
	return 1;
}

LRESULT CDeskletBrowserDlg::OnUserNotifyAddDir(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	const WCHAR* str = reinterpret_cast<const WCHAR*>(lParam);
	if(str != NULL)
	{
		CInstalledDeskletProvider* prov = dynamic_cast<CInstalledDeskletProvider*>(providers[L"installed"]);
		if(prov != NULL)
		{
			prov->AddDirectory(str);
			if(warningDlg != NULL)
			{
				if(::IsWindowVisible(warningDlg->m_hWnd) && AveScripterInstalled())
				{
					warningDlg->ShowWindow(SW_HIDE);
					lb.ShowWindow(SW_SHOW);
				}
			}
		}
	}

	return 0;
}

void CDeskletBrowserDlg::Cleanup()
{
	if(view != NULL)
	{
		view->SetControl(NULL);
		view->SetCollection(NULL);
	}
	viewStates.clear();
	curCollectionSel = -1;
	delete view;
	view = NULL;


	for(std::vector<CDeskletCollection*>::iterator iter = collections.begin(); iter != collections.end(); ++iter)
	{
		CDeskletCollection* coll = *iter;
		if(coll != NULL)
			coll->SetProvider(NULL);
		
		delete coll;
	}
	collections.clear();

	for(std::map<std::wstring, CDeskletProvider*>::iterator iter = providers.begin(); iter != providers.end(); ++iter)
	{
		CDeskletProvider* prov = iter->second;
		if(prov != NULL)
			prov->Stop();

		delete prov;
	}
	providers.clear();
}

LRESULT CDeskletBrowserDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(warningDlg != NULL)
	{
		if(::IsWindow(*warningDlg))
			warningDlg->DestroyWindow();
	}
	delete warningDlg;
	warningDlg = NULL;

	SaveCategories();
	Cleanup();

	m_ImageList.Destroy();
	//m_Finder.Stop(INFINITE);
	//m_Finder2.Stop(INFINITE);

	return 0;
}

LRESULT CDeskletBrowserDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog(0);
	return 0;
}