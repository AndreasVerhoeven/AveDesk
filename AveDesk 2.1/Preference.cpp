// Preference.cpp : Implementierung von CPreference
#include "stdafx.h"
#include "AveDesk.h"
#include "Preference.h"

/////////////////////////////////////////////////////////////////////////////
// CPreference

#define CStringToW(n) T2W(const_cast<TCHAR*>(n.operator LPCTSTR()))

STDMETHODIMP CPreference::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IPreference
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
    if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPreference::get_Value(VARIANT *pVal)
{
	VariantCopy(pVal, &value);

	return S_OK;
}

STDMETHODIMP CPreference::put_Value(VARIANT newVal)
{
	value = newVal;
	return S_OK;
}
/*
#define MAPXML(t) \
  if( \
    SUCCEEDED( nodeElem->getAttribute(CComBSTR(A2W(""#t)), &val)) && \
    SUCCEEDED( val.ChangeType(VT_BSTR) ) \
    ) \
  { \
    t = val.bstrVal; \
    val.Clear(); \
  }
*/

void MapXml(CComBSTR nam, CComBSTR & tar, CComVariant & val, CComPtr<IXMLDOMNamedNodeMap> & list)
{
  CComPtr<IXMLDOMNode> pAttr;
  list->getNamedItem(nam, &pAttr.p);
  if( pAttr )
  {
    val.Clear();
    pAttr->get_nodeValue(&val);
    if( SUCCEEDED( val.ChangeType(VT_BSTR) ) )
      tar = val.bstrVal;
  }
}
#define MAPXML(t) MapXml(A2W(""#t), t, val, list)
void CPreference::LoadXML(IXMLDOMNode *nod)
{
  USES_CONVERSION;
  
  CComQIPtr<IXMLDOMNode> nodeElem = nod; // simplified attribute access
  CComPtr<IXMLDOMNamedNodeMap> list;
  nodeElem->get_attributes(&list.p);
  CComVariant val;
  
  MAPXML(name);  
  MAPXML(type); 
  CComBSTR defvalstr;
  MapXml(L"defval", defvalstr, val, list);
  defval = defvalstr;
  if( IsValid() )
    defval.ChangeType(vt);
  //MAPXML(defval); 
  MAPXML(min); 
  MAPXML(max); 
  MAPXML(items); 
  editval = defval;
  value = defval;
  nodeElem->get_text(&description.m_str);  
}

BOOL CPreference::IsValid()
{
  /*
  valid types of 1.1 are:
  range   - a slider
  boolean - a checkbox
  combo   - a combobox
  text    - a textbox 
  */
  vt = VT_BSTR;
  if( type == L"range" )
  { 
    ptype = pt_range;
    vt = VT_I4;
  }
  else if( type == L"boolean" )
  {
    ptype = pt_boolean;
    vt = VT_BOOL;
  }
  else if( type == L"combo" )
    ptype = pt_combo;
  else if( type == L"text" )
    ptype = pt_text;
  return ( 
    type == L"range" || 
    type == L"boolean" ||
    type == L"combo" ||
    type == L"text" 
    );
}

STDMETHODIMP CPreference::get_DefVal(VARIANT *pVal)
{
  ::VariantCopy(pVal, &defval);

	return S_OK;
}

STDMETHODIMP CPreference::put_DefVal(VARIANT newVal)
{
	defval = newVal;

	return S_OK;
}

STDMETHODIMP CPreference::get_Description(BSTR *pVal)
{
	description.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CPreference::put_Description(BSTR newVal)
{
	description = newVal;

	return S_OK;
}

STDMETHODIMP CPreference::get_Name(BSTR *pVal)
{
	name.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CPreference::put_Name(BSTR newVal)
{
	name = newVal;

	return S_OK;
}

STDMETHODIMP CPreference::get_Max(long *pVal)
{
	BSTR dummy = NULL;
  *pVal = wcstol(max, &dummy, 10);

	return S_OK;
}

STDMETHODIMP CPreference::put_Max(long newVal)
{
	max = newVal;

	return S_OK;
}

STDMETHODIMP CPreference::get_Min(long *pVal)
{
	BSTR dummy = NULL;
  *pVal = wcstol(min, &dummy, 10);

	return S_OK;
}

STDMETHODIMP CPreference::put_Min(long newVal)
{
	min = newVal;

	return S_OK;
}

STDMETHODIMP CPreference::get_Type(BSTR *pVal)
{
	type.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CPreference::put_Type(BSTR newVal)
{
  CComBSTR oldVal = type;
  type = newVal;
  if( IsValid() )
  {
	  return S_OK;
  }
  type = oldVal;
  return E_INVALIDARG;
}

void CPreference::AttachFrame(HWND wnd)
{
  USES_CONVERSION;

  if( !IsValid() )
    return;
  
  //Attach(wnd);
  SubclassWindow(wnd);
  HFONT hf = GetFont();
  RECT rc = {0};
  GetClientRect(&rc);
  HWND par = GetParent();


  rc.top = rc.bottom / 4;

  w_desc.Create( m_hWnd, rc, _T("Description"), 
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_LEFT| ES_READONLY,
    WS_EX_TRANSPARENT
    );
  w_desc.SetFont(hf);
  CString txt = W2T(description);
  txt.Replace(_T("\n"), _T("\r\n"));
  w_desc.SetWindowText(txt);
  w_desc.SetTabStops(8);

  GetClientRect(&rc);
  rc.bottom /= 4;

  switch( ptype )
  {
  case pt_range:
    {
      rc.bottom /= 2;
      w_track.Create(m_hWnd, rc, _T("Slider"), 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | TBS_NOTICKS | TBS_HORZ | TBS_BOTH, 
        0 
        );
      w_track.SetDlgCtrlID(1);
      OffsetRect(&rc, 0, rc.bottom);

      if( editval.vt != VT_I4 )
        editval.ChangeType(VT_I4);
      

      BSTR tmp = NULL;
      int 
        iMin = wcstol(min, &tmp, 10), 
        iMax = wcstol(max, &tmp, 10), 
        iVal = editval.lVal;
      w_track.SetRangeMin(iMin);
      w_track.SetRangeMax(iMax);
     
	  // FIX: If you set the ranges of a slider control to -200 and +200 and set the value to 0,
	  //      the slider will show at -200. You'll need to move the slider around to get it correctly.
	  w_track.SetPos(iMin);
      w_track.SetPos(iVal);

      w_track.SetFont(hf);
      CString txtVal;
      txtVal.Format(_T("%ld"), editval.lVal);
      //int iAlign[3] = {SS_LEFT, SS_CENTER, SS_RIGHT};
      CString str[3] = { W2T(min), txtVal, W2T(max) };
      int w = rc.right / 3;
      for( int i = 0; i < 3; i++ )
      {
        RECT rcl = { i*w, rc.top, (i+1)*w, rc.bottom };
        w_tracklabel[i].Create(m_hWnd, rcl, _T("SliderLabel"), WS_VISIBLE | WS_CHILD | i );
        w_tracklabel[i].SetFont(hf);
        w_tracklabel[i].SetWindowText(str[i]);
      }
    }
    break;
  case pt_boolean:
    {
      w_check.Create(m_hWnd, rc, _T("Boolean"), 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX );
      w_check.SetWindowText(W2T(name));
      w_check.SetDlgCtrlID(2);
      w_check.SetFont(hf);

      CComVariant var = editval;
      if( SUCCEEDED( var.ChangeType(VT_BOOL) ) )
      { 
        w_check.SetCheck((var.boolVal)?BST_CHECKED:BST_UNCHECKED);
      }
    }
    break;
  case pt_combo:
    {
      rc.bottom *= 4;
      w_combo.Create(m_hWnd, rc, _T("Combo"), 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST );
      w_combo.SetDlgCtrlID(3);
      w_combo.SetFont(hf);
      
      CComBSTR tmp = items;
      BSTR it = wcstok(tmp, L"|");
      while( it )
      {
        w_combo.AddString(W2T(it));
        it = wcstok(NULL, L"|");
      }
      int iSel = w_combo.FindStringExact(0, W2T(editval.bstrVal));
      if( CB_ERR != iSel )
        w_combo.SetCurSel(iSel);
    }
    break;
  case pt_text:
    {
      rc.bottom /= 2;
      w_text.Create(m_hWnd, rc, _T("Text"), 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_WANTRETURN);
      w_text.SetWindowText(W2T(editval.bstrVal));
      w_text.SetDlgCtrlID(4);
      w_text.SetFont(hf);
      SHAutoComplete(w_text.m_hWnd, SHACF_DEFAULT);
    }
    break;
  }
}
#define DETWND(t)   if(::IsWindow(t.m_hWnd)) t.DestroyWindow()
void CPreference::DetachFrame()
{
  DETWND(w_check);
  DETWND(w_combo);
  DETWND(w_text);
  DETWND(w_desc);
  DETWND(w_track);
  DETWND(w_tracklabel[0]);
  DETWND(w_tracklabel[1]);
  DETWND(w_tracklabel[2]);
  UnsubclassWindow();
  //Detach();
}

LRESULT CPreference::OnPrefChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  USES_CONVERSION;
  bHandled = FALSE;
  switch( wID )
  {
  case 2: // checkbox
    {
      editval = (w_check.GetCheck() == BST_CHECKED)?L"True":L"False";
    }
    break;
  case 3: // combo
    {
      CString str;
      w_combo.GetWindowText(str.GetBuffer(MAX_PATH), MAX_PATH);
      str.ReleaseBuffer();
      editval = CStringToW(str);
    }
    break;
  case 4: // text
    {
      CString str;
      w_text.GetWindowText(str.GetBuffer(MAX_PATH), MAX_PATH);
      str.ReleaseBuffer();
      editval = CStringToW(str);
    }
    break;

  }
  return 0;
}

LRESULT CPreference::OnRangeChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  USES_CONVERSION;
  bHandled = FALSE;
  if( w_track.IsWindow() ) // avoid any H-Scroll on future items
  {
    int newPos = w_track.GetPos();
    CString newVal;
    newVal.Format(_T("%ld"), newPos);
    if( w_tracklabel[1].IsWindow() )
      w_tracklabel[1].SetWindowText(newVal);
    editval = CStringToW(newVal);
  }
  return 0;
}

STDMETHODIMP CPreference::Load(BSTR IniFile, BSTR IniSection)
{
  USES_CONVERSION;
  
  CComBSTR valname;
  CComVariant val;
  parent->get_Name(&valname.m_str);
  valname += L".";
  valname += name;

  CString newVal;
  

  if( VT_BOOL == vt )
    val = (defval.boolVal)?L"True":L"False";
  else if( VT_BSTR == vt )
    val = defval;
  else
    val.ChangeType(VT_BSTR, &defval);

  GetPrivateProfileString(
    W2T(IniSection),
    W2T(valname),
    W2T(val.bstrVal),
    newVal.GetBuffer(MAX_PATH+1),
    MAX_PATH,
    W2T(IniFile)
    );

  newVal.ReleaseBuffer();
  value = CStringToW(newVal);
  value.ChangeType(vt);
  editval=value;
	return S_OK;
}

STDMETHODIMP CPreference::Save(BSTR IniFile, BSTR IniSection)
{
  USES_CONVERSION;
  
  CComBSTR valname;
  CComVariant val;
  parent->get_Name(&valname.m_str);
  valname += L".";
  valname += name;

  if( VT_BOOL == vt )
    val = (value.boolVal)?L"True":L"False";
  else if( VT_BSTR == vt )
    val = value;
  else
    val.ChangeType(VT_BSTR, &value);

  WritePrivateProfileString(
    W2T(IniSection),
    W2T(valname),
    W2T(val.bstrVal),
    W2T(IniFile)
    );
	return S_OK;
}
