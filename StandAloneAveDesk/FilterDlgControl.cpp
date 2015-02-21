#include "stdafx.h"
#include "FilterDlgControl.h"
#include <cmath>

CFilterDlgControl::CFilterDlgControl(void) : boxSel(0)
{
}

CFilterDlgControl::~CFilterDlgControl(void)
{
}


LRESULT CFilterDlgControl::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	box = GetDlgItem(IDC_FILTERTYPE);

	CString str;
	str.LoadStringW(IDS_FILTERANY);
	box.AddString(str);
	str.LoadStringW(IDS_FILTERNAME);
	box.AddString(str);
	str.LoadStringW(IDS_FILTERDESCRIPTION);
	box.AddString(str);
	str.LoadStringW(IDS_FILTERAUTHOR);
	box.AddString(str);
	str.LoadStringW(IDS_FILTERTYPENAME);
	box.AddString(str);
	str.LoadStringW(IDS_FILTERCATEGORY);
	box.AddString(str);

	box.SetCurSel(MapFilterToBox((CDeskletFilter::FilterType)boxSel));
	SetDlgItemText(IDC_KEYWORD, keyword.c_str());

	ToolTips_Init();
	return 0;
}


LRESULT CFilterDlgControl::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

int CFilterDlgControl::MapFilterToBox(CDeskletFilter::FilterType f)
{
	if(CDeskletFilter::FilterAny == f)
		return 0;
	else if(CDeskletFilter::FilterName == f)
		return 1;
	else if(CDeskletFilter::FilterDescription == f)
		return 2;
	else if(CDeskletFilter::FilterAuthor == f)
		return 3;
	else if(CDeskletFilter::FilterTypeName == f)
		return 4;
	else if(CDeskletFilter::FilterCategory == f)
		return 5;
	else
		return 0;
}

CDeskletFilter::FilterType CFilterDlgControl::MapBoxToFilter(int sel)
{
	if(0 == sel)
		return (CDeskletFilter::FilterType)0;
	else
		return (CDeskletFilter::FilterType)(int)std::pow(2.0f,sel-1);
}

LRESULT CFilterDlgControl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CEdit edit = GetDlgItem(IDC_KEYWORD);
	WCHAR str[1024] = {0};
	edit.GetWindowTextW(str, 1024);
	keyword = str;

	boxSel = MapBoxToFilter(box.GetCurSel());

	return 0;
}

LRESULT CFilterDlgControl::OnAddFilter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetParent().SendMessage(WM_USER_ADDFILTER, (WPARAM)m_hWnd, 0L);
	return 0;
}

LRESULT CFilterDlgControl::OnRemoveFilter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	GetParent().SendMessage(WM_USER_REMOVEFILTER, (WPARAM)m_hWnd, 0L);
	return 0;
}