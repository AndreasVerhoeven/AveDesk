// MainDlg.cpp : implementation of the CVersionDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "VersionDlg.h"



LRESULT CVersionDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_list.SubclassWindow(GetDlgItem(IDC_VERSIONDATA));

	std::vector<pair_t<std::wstring, std::wstring> > info = ts->GetDefaultVersionInfo();
	int counter=0;
	for(std::vector<pair_t<std::wstring, std::wstring> >::iterator iter = info.begin(); iter != info.end(); ++iter)
	{
		m_list.AddItem( PropCreateSimple(iter->key.c_str(), iter->val.c_str()) );
	}

	return TRUE;
}

BOOL CVersionDlg::OnSetActive() 
{    
	SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
   return TRUE;
}


int CVersionDlg::OnWizardNext()
{
	int count = m_list.GetCount();
	ts->versionInfo.clear();
	for(int i = 0; i < count; ++i)
	{
		WCHAR key[MAX_PATH] = {0};
		CComVariant value;
		HPROPERTY prop = m_list.GetProperty(i);
		m_list.GetItemName(prop, key, MAX_PATH);
		m_list.GetItemValue(prop, &value);
		const WCHAR* str = value.bstrVal;
		pair_t<std::wstring, std::wstring> pair(key, (str != NULL) ? str : L"");
		ts->versionInfo.push_back(pair);
	}

	ts->DoBuild();

	return 0;
}
