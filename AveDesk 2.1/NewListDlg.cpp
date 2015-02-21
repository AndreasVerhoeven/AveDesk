#include "stdafx.h"
#include "NewListDlg.h"
#include "FilterDlgControl.h"
#include "Pidl.h"

#include "Application.h"
extern CComObject<CApplication>* _app;

CNewListDlg::CNewListDlg(void) : threadHandle(NULL)
{
}

CNewListDlg::~CNewListDlg(void)
{
	StopIconEnum();
}

LRESULT CNewListDlg::OnIconFound(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	const WCHAR* iconFName = reinterpret_cast<const WCHAR*>(lParam);

	CPidl pidl;
	pidl.SetFromPath(iconFName);
	HICON hIcon = pidl.ExtractIcon(48);
	imgList.AddIcon(hIcon);
	int imgIndex = iconBox.GetCount();
	iconBox.InsertItem(-1, iconFName, imgIndex, imgIndex, 0);
	::DestroyIcon(hIcon);

	return 0;
}

LRESULT CNewListDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDrop = reinterpret_cast<HDROP>(wParam);
	if(NULL == hDrop)
		return 0;

	UINT filesCount = DragQueryFile(hDrop, (UINT)-1, NULL, 0);
	if(filesCount > 0)
	{
		WCHAR iconFName[MAX_PATH];
		DragQueryFileW(hDrop, 0, iconFName, MAX_PATH);
		CPidl pidl;
		pidl.SetFromPath(iconFName);
		HICON hIcon = pidl.ExtractIcon(48);
		imgList.AddIcon(hIcon);
		int imgIndex = iconBox.GetCount();
		int index = iconBox.InsertItem(-1, iconFName, imgIndex, imgIndex, 0);
		iconBox.SetCurSel(index);

		::DestroyIcon(hIcon);
	}

	::DragFinish(hDrop);

	return 0;
}

DWORD CALLBACK CNewListDlg::EnumIconsThreadFunc(LPVOID lpData)
{
	IconFindData* iconFindData = reinterpret_cast<IconFindData*>(lpData);
	if(NULL == iconFindData)
		return 0;

	WCHAR path[MAX_PATH] = {0};
	GetModuleFileName(NULL, path, MAX_PATH);
	if(::IsWindow(iconFindData->callbackHwnd))
		::SendMessage(iconFindData->callbackHwnd, iconFindData->callbackMsg, 0L, (LPARAM)path);


	WCHAR findPath[MAX_PATH] = {0};
	wcscpy_s(findPath, MAX_PATH, iconFindData->path);
	PathAppendW(findPath, L"*.ico");

	WIN32_FIND_DATAW data = {0};
	HANDLE h = FindFirstFile(findPath, &data);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			WCHAR path[MAX_PATH] = {0};
			wcscpy_s(path, MAX_PATH, iconFindData->path);
			PathAppendW(path, data.cFileName);

			if(::IsWindow(iconFindData->callbackHwnd))
				::SendMessage(iconFindData->callbackHwnd, iconFindData->callbackMsg, 0L, (LPARAM)path);
			

		}while(FindNextFile(h, &data));
		FindClose(h);
	}


	HANDLE threadHandle = iconFindData->threadHandle;
	delete iconFindData;
	CloseHandle(threadHandle);
	ExitThread(0);

	return 0;
}

void CNewListDlg::StartIconEnum()
{
	IconFindData* data = new IconFindData;
	data->callbackHwnd = m_hWnd;
	data->callbackMsg = WM_USER_ICONFOUND;
	if(_App != NULL)
		wcscpy_s(data->path,MAX_PATH,_App->GetAveDeskDataDir().c_str());
	PathAppendW(data->path, L"Images\\Icons\\Bar");

	DWORD threadId(0);
	data->threadHandle = ::CreateThread(NULL, 0, EnumIconsThreadFunc, (LPVOID)data, 0, &threadId);
	threadHandle = data->threadHandle;
}

void CNewListDlg::StopIconEnum()
{
	WaitForSingleObject(threadHandle, INFINITE);
}


void CNewListDlg::AddFilter(int index, int type, const std::wstring& keyword)
{
	CFilterDlgControl* filterDlg = new CFilterDlgControl;
	filterDlg->boxSel = type;
	filterDlg->keyword = keyword;
	filterDlg->Create(m_hWnd);
	RECT rc = {0};
	filterDlg->GetWindowRect(&rc);
	int h = rc.bottom - rc.top;
	int w = rc.right - rc.left;
	filterDlg->MoveWindow(32,76 + (h+4) * filters.size(), w, h);
	filters.push_back(filterDlg);
	filterDlg->ShowWindow(SW_SHOW);

	CWindow okBut = GetDlgItem(IDOK);
	CWindow cancelBut = GetDlgItem(IDCANCEL);
	CWindow rulesBox = GetDlgItem(IDC_RULESBOX);

	RECT rcDlg = {0};
	GetWindowRect(&rcDlg);
	rcDlg.bottom += (h+4);
	SetWindowPos(NULL, &rcDlg, SWP_NOZORDER | SWP_NOMOVE);

	RECT rcRules = {0};
	rulesBox.GetWindowRect(&rcRules);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcRules, 2);
	rcRules.bottom += (h+4);
	rulesBox.SetWindowPos(NULL, &rcRules, SWP_NOZORDER | SWP_NOMOVE);

	RECT rcOk = {0};
	okBut.GetWindowRect(&rcOk);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcOk, 2);
	rcOk.top += (h+4);
	rcOk.bottom += (h+4);
	okBut.SetWindowPos(NULL, &rcOk, 0);

	RECT rcCancel = {0};
	cancelBut.GetWindowRect(&rcCancel);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcCancel, 2);
	rcCancel.top += (h+4);
	rcCancel.bottom += (h+4);
	cancelBut.SetWindowPos(NULL, &rcCancel, SWP_NOZORDER);

	if(filters.size() == 1)
		filters[0]->GetDlgItem(IDC_REMOVEFILTER).EnableWindow(FALSE);
	else if(filters.size() > 1)
		filters[0]->GetDlgItem(IDC_REMOVEFILTER).EnableWindow(TRUE);

}

void CNewListDlg::RemoveFilter(int index)
{

	if(filters.size() == 2)
		filters[0]->GetDlgItem(IDC_REMOVEFILTER).EnableWindow(FALSE);


	RECT rcTemp = {0};
	filters[index]->GetWindowRect(&rcTemp);
	int h = rcTemp.bottom - rcTemp.top;

	for(int i = index+1; i < (int)filters.size(); ++i)
	{
		CFilterDlgControl* filterDlg = filters[i];

		RECT rc = {0};
		filterDlg->GetWindowRect(&rc);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
		rc.top -= (h+4);
		rc.bottom -= (h+4);
		filterDlg->SetWindowPos(NULL, &rc, SWP_NOZORDER);
		//filterDlg->MoveWindow(32,76 + (h+4) * filters.size(), w, h);
	}

	filters[index]->DestroyWindow();
	/// XXX: memory leak!
	//delete filters[index];
	filters.erase(filters.begin() + index);

	CWindow okBut = GetDlgItem(IDOK);
	CWindow cancelBut = GetDlgItem(IDCANCEL);
	CWindow rulesBox = GetDlgItem(IDC_RULESBOX);

	RECT rcDlg = {0};
	GetWindowRect(&rcDlg);
	rcDlg.bottom -= (h+4);
	SetWindowPos(NULL, &rcDlg, SWP_NOZORDER | SWP_NOMOVE);

	RECT rcRules = {0};
	rulesBox.GetWindowRect(&rcRules);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcRules, 2);
	rcRules.bottom -= (h+4);
	rulesBox.SetWindowPos(NULL, &rcRules, SWP_NOZORDER | SWP_NOMOVE);

	RECT rcOk = {0};
	okBut.GetWindowRect(&rcOk);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcOk, 2);
	rcOk.top -= (h+4);
	rcOk.bottom -= (h+4);
	okBut.SetWindowPos(NULL, &rcOk, 0);

	RECT rcCancel = {0};
	cancelBut.GetWindowRect(&rcCancel);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcCancel, 2);
	rcCancel.top -= (h+4);
	rcCancel.bottom -= (h+4);
	cancelBut.SetWindowPos(NULL, &rcCancel, SWP_NOZORDER);
}

LRESULT CNewListDlg::OnAddFilter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AddFilter();
	return 0;
}

LRESULT CNewListDlg::OnRemoveFilter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int index(-1);
	for(int i = 0; i < (int)filters.size(); ++i)
	{
		if(filters[i]->m_hWnd == (HWND)wParam)
		{
			index = i;
			break;
		}
	}

	if(index != -1)
		RemoveFilter(index);

	return 0;
}

LRESULT CNewListDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ToolTips_Init();

	DragAcceptFiles();

	iconBox = GetDlgItem(IDC_COMBOICONS);
	imgList.Create(48,48,ILC_COLOR32,1, 1);
	iconBox.SetImageList(imgList);

	if(filterData.size() == 0)
		AddFilter();
	else
	{
		for(std::vector<CDeskletFilter>::iterator iter = filterData.begin(); iter != filterData.end(); ++iter)
		{
			CDeskletFilter& f = *iter;
			AddFilter(-1, static_cast<int>(f.GetType()), f.GetKeyword());
		}
	}

	if(name.length() == 0)
	{
		CString str;
		str.LoadStringW(IDS_NEWLIST_UNNAMED);
		name = str;
	}

	CEdit edit = GetDlgItem(IDC_NAME);
	edit.SetWindowText(name.c_str());
	::SetFocus(GetDlgItem(IDC_NAME));
	edit.SetSel(0, name.length());


	WCHAR iconFName[MAX_PATH] = {0};
	if(!PathFileExists(iconFileName.c_str()) && iconFileName[0] != L'\0')
	{
		wcscpy_s(iconFName,MAX_PATH,_App->GetAveDeskDataDir().c_str());
		PathAppendW(iconFName, iconFileName.c_str());
	}
	else
	{
		wcscpy_s(iconFName, MAX_PATH, iconFileName.c_str());
	}

	if(!PathFileExists(iconFName) || iconFName[0] == L'\0')
	{
		// file not valid, just use avedesk.exe's icon
		GetModuleFileName(NULL, iconFName, MAX_PATH);
	}

	CPidl pidl;
	pidl.SetFromPath(iconFName);
	HICON hIcon = pidl.ExtractIcon(48);
	imgList.AddIcon(hIcon);
	iconBox.InsertItem(0, iconFName, 0, 0, 0);
	iconBox.SetCurSel(0);
	::DestroyIcon(hIcon);

	StartIconEnum();


	//AddFilter();
	//AddFilter();
	//AddFilter();
	//AddFilter();

	return 0;
}

LRESULT CNewListDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog(FALSE);
	return 0;
}

LRESULT CNewListDlg::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(TRUE);
	return 0;
}

LRESULT CNewListDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(FALSE);
	return 0;
}

LRESULT CNewListDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	filterData.clear();
	for(std::vector<CFilterDlgControl*>::iterator iter = filters.begin(); iter != filters.end(); ++iter)
	{
		CFilterDlgControl* dlg = *iter;
		if(dlg != NULL)
		{
			dlg->DestroyWindow();

			if(dlg->keyword[0] != L'0')
				filterData.push_back(CDeskletFilter((CDeskletFilter::FilterType)dlg->boxSel, dlg->keyword));

			delete dlg;
		}
	}
	filters.clear();

	CEdit edit = GetDlgItem(IDC_NAME);
	WCHAR str[1024] = {0};
	edit.GetWindowTextW(str, 1024);
	name = str;

	WCHAR* iconFName = new WCHAR[iconBox.GetLBTextLen(iconBox.GetCurSel())+1];
	iconBox.GetLBText(iconBox.GetCurSel(), iconFName);
	iconFileName = iconFName;
	delete[] iconFName;
	iconFName = NULL;

	return 0;
}