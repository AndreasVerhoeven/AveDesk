// Preferences.h : Deklaration von CPreferences

#ifndef __PREFERENCES_H_
#define __PREFERENCES_H_

#include "resource.h"       // Hauptsymbole
#include <map>
#include "preference.h"
#include "helper_funcs.h"


typedef std::map<CComBSTR, CComObject<CPreference>*, CComBSTRLessIgnCase> PreferencesMap;

/////////////////////////////////////////////////////////////////////////////
// CPreferences
class ATL_NO_VTABLE CPreferences : 
  public CDialogImpl<CPreferences>,
  public CThemeImpl<CPreferences>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPreferences, &CLSID_Preferences>,
	public ISupportErrorInfo,
	public IDispatchImpl<IPreferences, &IID_IPreferences, &LIBID_AveDeskLib>
{
public:

  enum { IDD = IDD_PREFERENCES };


  BEGIN_MSG_MAP(CPreferences)
    CHAIN_MSG_MAP(CThemeImpl<CPreferences>)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_LIST, LBN_SELCHANGE, OnSelchange)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

    COMMAND_HANDLER(IDOK, BN_CLICKED, OnOk)
    COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnCancel)
    COMMAND_HANDLER(IDC_ABOUTEFFECTLET, BN_CLICKED, OnAbout)
  END_MSG_MAP()

  CPreferences() : 
    actPref(NULL),
    parentHwnd(NULL)
	{
	}
  ~CPreferences();

DECLARE_REGISTRY_RESOURCEID(IDR_PREFERENCES)
DECLARE_NOT_AGGREGATABLE(CPreferences)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPreferences)
	COM_INTERFACE_ENTRY(IPreferences)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


// Variables
PreferencesMap prefs;
CComPtr<IEffectLet> parent;
CWindow wndPrefFrame;
CListBox box;
CComObject<CPreference> *actPref;

CComBSTR IniSection, IniFile;

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPreferences
public:
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(get_IniSection)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IniSection)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IniFile)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IniFile)(/*[in]*/ BSTR newVal);
	STDMETHOD(Show)();
	bool IsKnownPreference(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown * * ppiunkItem, ITypeInfo * * ppti);
	void LoadXML( IXMLDOMNode *nod);
	STDMETHOD(SaveAll)();
	STDMETHOD(ReadAll)();
	
  
// Dialog  
  HWND parentHwnd;
  LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EndDialog(0);
		return 0;
	}
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSelchange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	  if( actPref )
    {
      actPref->DetachFrame();
      actPref = NULL;
    }
		return 0;
	}
};

#endif //__PREFERENCES_H_
