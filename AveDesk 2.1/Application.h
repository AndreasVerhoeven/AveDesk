// Application.h : Declaration of the CApplication

#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "resource.h"       // main symbols
#include "desklets.h"
#include "deskletinfos.h"
#include "ShowCase1.h"
#include "IconWindow.h"
#include <string>

#define WM_AVEACTION_SHOWCP WM_USER + 201

template<typename T> inline void SaveRelease(T*& t)
{
	if( t )
	{
		t->Release();
		t = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CApplication
class ATL_NO_VTABLE CApplication : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CApplication, &CLSID_Application>,
	public IProvideClassInfo2Impl<&CLSID_Application, NULL, &LIBID_AveDeskLib>,
	public IDispatchImpl<IApplication, &IID_IApplication, &LIBID_AveDeskLib>
{
private:
	DWORD m_MainThreadId;
	CIconWindow m_IconWindow;

	std::wstring m_DataDir;

public:
	CApplication();
	~CApplication();

	DECLARE_REGISTRY_RESOURCEID(IDR_APPLICATION)
	DECLARE_CLASSFACTORY_SINGLETON(CApplication)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CApplication)
		COM_INTERFACE_ENTRY(IApplication)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
	END_COM_MAP()

// IApplication
public:
	STDMETHOD(ShowDeskletBrowserDialog)();
	STDMETHOD(SaveGlobalSettings)();
	STDMETHOD(ShowThemeBrowserDialog)();
	STDMETHOD(get_Debug)(/*[out, retval]*/ IDebugHelperClient* *pVal);
	STDMETHOD(get_UserPath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ThemePath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ExePath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(OpenFile)(BSTR FileName);
	STDMETHOD(get_Version)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_ShowCase)(/*[out, retval]*/ IShowCase* *pVal);
	STDMETHOD(LoadTheme)(/*[in, defaultvalue("Theme.ini")]*/ BSTR Name);
	STDMETHOD(SaveTheme)(/*[in]*/ BSTR Name);
	STDMETHOD(Sleep)(int ms);
	STDMETHOD(get_AvailableDesklets)(/*[out, retval]*/ IDeskletInfos* *pVal);
	STDMETHOD(LoadImage)(/*[in]*/ BSTR FileName, /*[out, retval]*/ IAveImage ** pVal);
	STDMETHOD(get_RunningDesklets)(/*[out, retval]*/ IDesklets * *pVal);
	STDMETHOD(ShowControlPanel)();
	STDMETHOD(Close)();

	CComObject<CDesklets> *m_desklets;
	CComObject<CDeskletInfos> *m_dlls;
	CComObject<CShowCase> *m_showCase;

	// CoElement thread safe COM access
	CComPtr<IGlobalInterfaceTable> pGIT;
	DWORD dwCookie;
	DWORD GetThreadID()const{return GetMainThreadID();}
	void InitSafeInstance();
	void ExitSafeInstance();
	bool GetSafeInstance(IApplication **typedRet);


  
	std::wstring GetAveDeskUserPath()const;
	std::wstring GetAveDeskThemeDir()const;
	std::wstring GetAveDeskDataDir()const;

	void SetAveDeskDataDir(const std::wstring& dataDir);

	void SetThemeFileName(const std::wstring& path);
	std::wstring GetThemeFileName()const;

	void SetMainThreadId(const DWORD& id);
	DWORD GetMainThreadID()const;

	HWND GetMainWindowHandle()const;
	void SetupMainWindow();
	CIconWindow& GetIconWindow(){return m_IconWindow;};

	// loading SHOULD not be accessible by the COM interface
	// because it's a one-time action or user-initiated only.
	bool LoadGlobalSettings();
};

extern CComObject<CApplication> *_App;

#endif //__APPLICATION_H_
