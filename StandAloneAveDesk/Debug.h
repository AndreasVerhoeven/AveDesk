// Debug.h : Deklaration von CDebug

#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "resource.h"       // Hauptsymbole
#include "activdbg.h"
const BSTR AVEPIPE = L"\\\\.\\pipe\\AveDeskConsole";

/////////////////////////////////////////////////////////////////////////////
// CDebugConsole
class ATL_NO_VTABLE CDebugConsole : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IDebugConsole, &IID_IDebugConsole, &LIBID_AveDeskLib>,
	public IProvideClassInfo2Impl<&CLSID_DebugConsole, NULL, &LIBID_AveDeskLib>
{
public:
	CDebugConsole();
	~CDebugConsole();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDebugConsole)
		COM_INTERFACE_ENTRY(IDebugConsole)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CDebug
class ATL_NO_VTABLE CDebug : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDebug, &CLSID_DebugHelperServer>,
	public safeCreatableImpl<CDebug, CDebug, IDebugHelperClient>,
	public IProvideClassInfo2Impl<&CLSID_DebugHelperServer, NULL, &LIBID_AveDeskLib>,
	public IDispatchImpl<IDebugHelperClient, &IID_IDebugHelperClient, &LIBID_AveDeskLib>,
	public IActiveScriptSite,
	public IActiveScriptSiteWindow,
	public IActiveScriptSiteDebug,
	public IDebugHelperServer
{
public:
	CDebug()
	{
		InitializeCriticalSection(&pDebugAppLock);
		pDebugAppCount=0L;
		pDebugApplicationContext = 0L;
		bConsole = false;
		bInExec = false;
		bInScript = false;
		pDebugConsole = NULL;
		hThread = NULL;
		hEventIn = NULL;
		hEventOut = NULL;
	}
	~CDebug()
	{
		if( bConsole )
			FreeConsole();
		if( pDebugConsole )
			pDebugConsole->Release();
	}

	//DECLARE_REGISTRY_RESOURCEID(IDR_DEBUG)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDebug)
		COM_INTERFACE_ENTRY(IDebugHelperClient)
		COM_INTERFACE_ENTRY(IDebugHelperServer)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IActiveScriptSite)
		COM_INTERFACE_ENTRY(IActiveScriptSiteWindow)
		COM_INTERFACE_ENTRY(IActiveScriptSiteDebug)
	END_COM_MAP()

// IDebug
public:
	void HandleConsoleInput();
	void Close();
	void Open();
	void exec(char* cmd);
	void echo(LPCWSTR text);
	void echo(char* text);
	STDMETHOD(get_Console)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Console)(VARIANT_BOOL IsVisible);
	STDMETHOD(HideConsole)();
	STDMETHOD(ShowConsole)();
	STDMETHOD(get_DebugApplicationContext)(/*[out, retval]*/ DWORD *pVal);
	STDMETHOD(get_DebugApplication)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(get_DebugDocument)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(putref_DebugDocument)(/*[in]*/ IUnknown* newVal);
	STDMETHOD(AttachDebugClient)(IUnknown *Document,/*[out, retval]*/ IDebugHelperClient ** pVal);
	STDMETHOD(Break)();
	STDMETHOD(Assert)(/*[in]*/ VARIANT Expression);
	STDMETHOD(Print)(/*[in]*/ BSTR Text);

// CoElement thread safe COM access
	DWORD GetThreadID()const;


// IActiveScriptSite
	STDMETHOD(GetLCID)(LCID * plcid)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetItemInfo)(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown * * ppiunkItem, ITypeInfo * * ppti);
	STDMETHOD(GetDocVersionString)(BSTR * pbstrVersion)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnScriptTerminate)(const VARIANT * pvarResult, const EXCEPINFO * pexcepinfo)
	{
		return S_OK;
	}
	STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState)
	{
		return S_OK;
	}
	STDMETHOD(OnScriptError)(IActiveScriptError * pscripterror);
	STDMETHOD(OnEnterScript)()
	{
		bInScript = true;
		return S_OK;
	}
	STDMETHOD(OnLeaveScript)()
	{
		bInScript = false;
		return S_OK;
	}
	// IActiveScriptSiteWindow
	STDMETHOD(GetWindow)(HWND * phwnd)
	{
		if (phwnd == NULL)
			return E_POINTER;
		HWND hwnd = GetConsoleWindow();
		if( IsWindow(hwnd) )
		{
			*phwnd = hwnd;
			return S_OK;
		}
		*phwnd = GetDesktopWindow();
		return S_OK;
	}
	STDMETHOD(EnableModeless)(BOOL fEnable)
	{
		return S_OK;
	}

	// IActiveScriptSiteDebug

	STDMETHOD(GetDocumentContextFromPosition)( 
            DWORD dwSourceContext,
            ULONG uCharacterOffset,
            ULONG uNumChars,
            IDebugDocumentContext **ppsc)
	{
		return E_NOTIMPL;
	}
        
	STDMETHOD(GetApplication)( 
            IDebugApplication **ppda)
	{
		return E_NOTIMPL;
	}
        
	STDMETHOD(GetRootApplicationNode)( 
            IDebugApplicationNode **ppdanRoot)
	{
		return E_NOTIMPL;
	}
        
	STDMETHOD(OnScriptErrorDebug)( 
            IActiveScriptErrorDebug *pErrorDebug,
            BOOL *pfEnterDebugger,
            BOOL *pfCallOnScriptErrorWhenContinuing)
	{
        *pfEnterDebugger = FALSE;
        *pfCallOnScriptErrorWhenContinuing = FALSE;
		return S_OK;
	}
        




	CComPtr<IDebugDocumentHelper> pDebugDocumentHelper;

// debugger globals
	static CRITICAL_SECTION pDebugAppLock;
	CComPtr<IDebugApplication> pDebugApplication;
	DWORD pDebugApplicationContext;
	long pDebugAppCount;
	CComObject<CDebugConsole>* pDebugConsole;

protected:
	void DoConsoleLoop();
	HANDLE hStdin, hStdout, hEventIn, hEventOut, hThread;
	bool bConsole;
	bool bInExec;
	bool bInScript;
	char buf[0x10000];
	CComPtr<IActiveScript> pScript;

	static BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
	static DWORD WINAPI ConsoleThread(CComObject<CDebug>* ths);
};
extern CComObject<CDebug> *_Dbg;

/////////////////////////////////////////////////////////////////////////////
// CDebugClient
class ATL_NO_VTABLE CDebugClient : 
	public CComCoClass<CDebugClient, &CLSID_DebugHelperClient>,
	public IProvideClassInfo2Impl<&CLSID_DebugHelperClient, NULL, &LIBID_AveDeskLib>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IDebugHelperClient, &IID_IDebugHelperClient, &LIBID_AveDeskLib>
{
public:
friend class CDebug;
	CDebugClient()
	{
	}
	~CDebugClient()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDebugClient)
	COM_INTERFACE_ENTRY(IDebugHelperClient)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDebug
public:
	STDMETHOD(get_Console)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Console)(VARIANT_BOOL IsVisible);
	STDMETHOD(Break)();
	STDMETHOD(Assert)(/*[in]*/ VARIANT Expression);
	STDMETHOD(Print)(/*[in]*/ BSTR Text);

protected:
	CComQIPtr<IDebugDocumentHelper> pDebugDocumentHelper;
	CComPtr<IDebugHelperClient> pParent;
};


#endif //__DEBUG_H_
