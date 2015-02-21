// Preference.h : Deklaration von CPreference

#ifndef __PREFERENCE_H_
#define __PREFERENCE_H_

#include "resource.h"       // Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CPreference
class ATL_NO_VTABLE CPreference : 
  public CWindowImpl<CPreference>,
  public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPreference, &CLSID_Preference>,
	public ISupportErrorInfo,
  public IProvideClassInfo2Impl<&CLSID_Preference, NULL, &LIBID_AveDeskLib>,
	public IDispatchImpl<IPreference, &IID_IPreference, &LIBID_AveDeskLib>
{
public:
	CPreference()
	{
    vt = VT_EMPTY;
	}
  typedef enum tagKnownPrefTypes
  {
    pt_range,
    pt_boolean,
    pt_combo,
    pt_text
  }KnownPrefTypes;

  BEGIN_MSG_MAP(CPreference)
    //COMMAND_HANDLER(1, WM_HSCROLL, OnPrefChange)
    MESSAGE_HANDLER(WM_HSCROLL, OnRangeChange)
    COMMAND_HANDLER(2, BN_CLICKED, OnPrefChange)
    COMMAND_HANDLER(3, LBN_SELCHANGE, OnPrefChange)
    COMMAND_HANDLER(4, EN_CHANGE, OnPrefChange)
    //REFLECT_NOTIFICATIONS()
  END_MSG_MAP()
  
  //DECLARE_REGISTRY_RESOURCEID(IDR_PREFERENCE)
  DECLARE_NOT_AGGREGATABLE(CPreference)

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(CPreference)
	  COM_INTERFACE_ENTRY(IPreference)
	  COM_INTERFACE_ENTRY(IDispatch)
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Variables
  CComPtr<IEffectLet> parent;
  CComVariant value, defval, editval;
  CComBSTR name, type, min, max, items, description;
  KnownPrefTypes ptype;
  VARTYPE vt;

  CButton       w_check;
  CEdit         w_desc;
  CTrackBarCtrl w_track;
  CComboBox     w_combo;
  CEdit         w_text;
  CStatic       w_tracklabel[3];

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPreference
public:
	STDMETHOD(Save)(BSTR IniFile, BSTR IniSection);
	STDMETHOD(Load)(BSTR IniFile, BSTR IniSection);
	void DetachFrame();
	void AttachFrame(HWND wnd);
  LRESULT OnPrefChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnRangeChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	STDMETHOD(get_Type)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Type)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Min)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Min)(/*[in]*/ long newVal);
	STDMETHOD(get_Max)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Max)(/*[in]*/ long newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_DefVal)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_DefVal)(/*[in]*/ VARIANT newVal);
	BOOL IsValid();
	void LoadXML(IXMLDOMNode *nod);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
};

#endif //__PREFERENCE_H_
