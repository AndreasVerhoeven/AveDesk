// Preferences.cpp : Implementierung von CPreferences
#include "stdafx.h"
#include "AveDesk.h"
#include "preference.h"
#include "Preferences.h"
#include "activscp.h"
#include "effectlet.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferences
CFont fnt;

STDMETHODIMP CPreferences::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IPreferences
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
    if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPreferences::ReadAll()
{
  if( IniFile.Length() && IniSection.Length() )
  {
    PreferencesMap::iterator it = prefs.begin();
    for( ; it != prefs.end(); it++)
    {
      (*it).second->parent = parent;
      (*it).second->Load(IniFile, IniSection);
      (*it).second->parent = NULL;
    }
  }
	return S_OK;
}

STDMETHODIMP CPreferences::SaveAll()
{
  if( IniFile.Length() && IniSection.Length() )
  {
    PreferencesMap::iterator it = prefs.begin();
    for( ; it != prefs.end(); it++)
    {
      (*it).second->parent = parent;
      (*it).second->Save(IniFile, IniSection);
      (*it).second->parent = NULL;
    }
  }
	return S_OK;
}

void CPreferences::LoadXML(IXMLDOMNode *nod)
{
  long count = 0;
  CComPtr<IXMLDOMNodeList> list;
  nod->selectNodes(L"preference", &list.p);
  if( list.p )
  {
    list->get_length(&count);  
    for( long i = 0; i < count; i++ )
    {
      CComPtr<IXMLDOMNode> nodeChild;
      CComObject<CPreference> *pr = NULL;

      list->get_item(i, &nodeChild.p);
      CComObject<CPreference>::CreateInstance(&pr);
      pr->AddRef();
      pr->LoadXML(nodeChild);
      prefs[pr->name] = pr;
      // this shmug is not reference counted
      //pr->Release();
    }
  }
}

bool CPreferences::IsKnownPreference(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti)
{
  PreferencesMap::iterator it = prefs.find(pstrName);
  if( it != prefs.end() )
  {
    CComObject<CPreference> *ptr = (*it).second;
    if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
    {
      ptr->GetClassInfo(ppti);
      (*ppti)->AddRef();	 // because returning
    }
    if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
    {
      return SUCCEEDED(ptr->QueryInterface(IID_IUnknown, (void**) ppiunkItem));
    }
  }
  return false;
}

CPreferences::~CPreferences()
{
  PreferencesMap::iterator it = prefs.begin();
  for( ; it != prefs.end(); it++)
  {
    (*it).second->Release();
  }
}

STDMETHODIMP CPreferences::Show()
{
  USES_CONVERSION;
  
  HWND hWnd = ::GetActiveWindow();
  if( ::IsWindow( parentHwnd ) )
    hWnd = parentHwnd;
  PreferencesMap::iterator it = prefs.begin();
  int iRet = DoModal(parentHwnd);

  CComObject<CEffectLet> *pp = 
    dynamic_cast< CComObject<CEffectLet> *>(parent.p);

  for( ; it != prefs.end(); it++)
  {
    CComObject<CPreference> *ptr = (*it).second;
    if( iRet )
    {
      CComVariant oldVal = ptr->value;
      oldVal.ChangeType(VT_BSTR);
      if( pp && ptr->value != CComVariant(ptr->editval) )
      {
        CComVariant varArg[3] = 
        {
          CComVariant(ptr->editval),
          CComVariant(ptr->value),
          CComVariant((IDispatch*)ptr)
        };
        CComVariant vRet = 
          pp->CallScript(L"OnPreferenceChange", 3, varArg);

        if( SUCCEEDED( vRet.ChangeType(VT_BSTR) ) )
        {
          if( wcslen(vRet.bstrVal) )
          {
            MessageBox(
              W2T(vRet.bstrVal), W2T(ptr->name));
          }
          else
          {
            VARTYPE vt = ptr->value.vt;
            ptr->value = ptr->editval;
            ptr->value.ChangeType(vt);
          }
        }
      }
    }
    else
    {
      CComVariant v = ptr->value;
      if( SUCCEEDED( v.ChangeType(VT_BSTR) ) )
        ptr->editval = v.bstrVal;
    }
  }
	return S_OK;
}

LRESULT CPreferences::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  USES_CONVERSION;
  EnableThemeDialogTexture(ETDT_ENABLETAB);
  int iCount = prefs.size();
  CComBSTR nam;
  parent->get_Name(&nam.m_str);
  nam += L" EffectLet Preferences";
  SetWindowText(W2T(nam));
  if( iCount )
  {
    wndPrefFrame.Attach(GetDlgItem(IDC_PREFFRAME));
    if( wndPrefFrame.IsWindow() )
    {
      wndPrefFrame.SetWindowText(_T(""));
      box.Attach(GetDlgItem(IDC_LIST));
      box.ResetContent();
      PreferencesMap::iterator it = prefs.begin();
      
      for( int i=0; it != prefs.end(); it++)
      {
        CComObject<CPreference> *ptr = (*it).second;
        box.InsertString(i++, W2T(ptr->name));
        box.SetItemDataPtr(box.GetCount()-1, ptr);
      }

      box.SetCurSel(0);
      actPref = (*prefs.begin()).second;
      actPref->AttachFrame(wndPrefFrame.m_hWnd);
    }
  }
  fnt.m_hFont = GetFont();
	return 0;
}

LRESULT CPreferences::OnSelchange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( actPref )
  {
    actPref->DetachFrame();
    actPref = NULL;
  }
  int iSel = box.GetCurSel();
  if( LB_ERR != iSel )
  {
    fnt.m_hFont = GetFont();
    actPref = (CComObject<CPreference>*)box.GetItemDataPtr(iSel);
    actPref->AttachFrame(wndPrefFrame.m_hWnd);
  }
  return 0;
}

LRESULT CPreferences::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  EndDialog(1);
  return 0;
}
LRESULT CPreferences::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  EndDialog(0);
  return 0;
}

INT_PTR CALLBACK AboutProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
  
  switch(msg)
	{
		case WM_INITDIALOG:
      {
        CWindow par, btn;
        CEdit wnd;
        wnd.Attach( ::GetDlgItem(hDlg, 101) );
        btn.Attach( ::GetDlgItem(hDlg, IDOK) );
        par.Attach(wnd.GetParent());
        //fnt.CreatePointFont(100, "MS Dlg");
        wnd.SetFont(fnt.m_hFont);
        btn.SetFont(fnt.m_hFont);
        //wnd.SetTabStops(8);
        par.Detach();
        if( CTheme::IsThemingSupported() )
        {
          ::EnableThemeDialogTexture(hDlg, ETDT_ENABLETAB);
        }
      }
		break;

		case WM_PAINT:
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hDlg,TRUE);
				break;
			}
		break;

		case WM_CLOSE:
			EndDialog(hDlg,FALSE);
		break;

		case WM_DESTROY:
      fnt.Detach();
		break;
	}
  return FALSE;
}

LPWORD lpwAlign ( LPWORD lpIn)
{
    ULONG ul;

    ul = (ULONG) lpIn;
    ul +=3;
    ul >>=2;
    ul <<=2;
    return (LPWORD) ul;
}

LRESULT CPreferences::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    
  // small modified snippet from MSDN - testcase for custom dialogs
  USES_CONVERSION;
  
  HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    LRESULT ret;
    CComBSTR title, name, author, version, description, wface;

    parent->get_Name(&name.m_str);
    parent->get_Author(&author.m_str);
    parent->get_Description(&description.m_str);
    parent->get_Version(&version.m_str);

    title = L"About ";
    title += name;

    CString face;
    face.Format(
      _T("Author:\t%s\n")
      _T("Version:\t%s\n")
      _T("\n")
      _T("%s"),
      W2T(author),
      W2T(version),
      W2T(description)
      );
    face.Replace(_T("\n"), _T("\r\n"));

    RECT rct = {0};
    DrawText(GetDC(), face, -1, &rct, 
      DT_CALCRECT | DT_LEFT | DT_TOP);
    long base = GetDialogBaseUnits(), baseX, baseY;
    
    //MapDialogRect(&rct);
    baseX = MulDiv(rct.right, 4, LOWORD(base));
    baseY = MulDiv(rct.bottom, 8, HIWORD(base));

    wface = face;

    int nSize = 2 * (
      sizeof(DLGTEMPLATE) + 
      3 * sizeof(DLGITEMTEMPLATE) +
      2 * title.Length() +
      2 * name.Length() +
      2 * author.Length() +
      2 * description.Length()
      );

    int nchar;

    hgbl = GlobalAlloc(GMEM_ZEROINIT, nSize);
    if (!hgbl)
        return -1;
 
    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
 
    // Define a dialog box.
 
    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU
                   | DS_MODALFRAME | WS_CAPTION;
    lpdt->cdit = 2;  // number of controls
    lpdt->x  = 10;  lpdt->y  = 10;
    lpdt->cx = (short)baseX + 20; lpdt->cy = (short)baseY + 35;

    lpw = (LPWORD) (lpdt + 1);
    *lpw++ = 0;   // no menu
    *lpw++ = 0;   // predefined dialog box class (by default)

    lpwsz = (LPWSTR) lpw;
    nchar = 1+ title.Length();
    wcscpy(lpwsz, title.m_str);
    lpw   += nchar;

    //-----------------------
    // Define an OK button.
    //-----------------------
    lpw = lpwAlign (lpw); // align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE) lpw;
    lpdit->x  = (short)baseX / 2 - 5; lpdit->y  = (short)baseY + 10;
    lpdit->cx = 35; lpdit->cy = 13;
    lpdit->id = IDOK;  // OK button identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

    lpw = (LPWORD) (lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;    // button class

    lpwsz = (LPWSTR) lpw;
    CComBSTR okStr = L"OK";
    wcscpy(lpwsz, okStr.m_str);
    nchar = 1+okStr.Length();
    lpw   += nchar;
    lpw = lpwAlign (lpw); // align creation data on DWORD boundary
    *lpw++ = 0;           // no creation data

    //-----------------------
    // Define a text control.
    //-----------------------
    lpw = lpwAlign (lpw); // align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE) lpw;
    lpdit->x  = 10; lpdit->y  = 10;
    lpdit->cx = (short)baseX; lpdit->cy = (short)baseY;
    lpdit->id = 101;  // text identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_READONLY;

    lpw = (LPWORD) (lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0081;                         // edit class

    lpwsz = (LPWSTR) lpw;
    nchar = 1+ wface.Length();
    wcscpy(lpwsz, wface.m_str);
    lpw   += nchar;
    lpw = lpwAlign (lpw); // align creation data on DWORD boundary
    *lpw++ = 0;           // no creation data

    GlobalUnlock(hgbl); 
    ret = DialogBoxIndirect(_Module.m_hInst, lpdt, 
        m_hWnd, (DLGPROC) AboutProc); 
    GlobalFree(hgbl); 
  return 0;
}

STDMETHODIMP CPreferences::get_IniFile(BSTR *pVal)
{
	IniFile.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CPreferences::put_IniFile(BSTR newVal)
{
	IniFile = newVal;

	return S_OK;
}

STDMETHODIMP CPreferences::get_IniSection(BSTR *pVal)
{
	IniSection.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CPreferences::put_IniSection(BSTR newVal)
{
	IniSection = newVal;

	return S_OK;
}

STDMETHODIMP CPreferences::get__NewEnum(IUnknown **pVal)
{
  return DoMapInplaceEnum<PreferencesMap>(prefs, pVal);
}
