// EffectLet.h: Definition der Klasse CEffectLet
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTLET_H__5969C808_4D45_41A8_A607_0FB026886E3D__INCLUDED_)
#define AFX_EFFECTLET_H__5969C808_4D45_41A8_A607_0FB026886E3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // Hauptsymbole
#include "drawplugin.h"
#include "ACTIVSCP.h"
#include "timer.h"
#include "colormatrix.h"
#include "matrix.h"
#include "preferences.h"
#include "images.h"
#include <map>
#include "activdbg.h"
#include "debug.h"


/////////////////////////////////////////////////////////////////////////////
// CEffectLet
class CEffectLet;
typedef CComObject<CEffectLet>* vdat;
class CEffectLet : 
	public IDispatchImpl<IEffectLet, &IID_IEffectLet, &LIBID_AveDeskLib>,
	public IActiveScriptSite,
	public IActiveScriptSiteWindow,
	public IDebugDocumentHost,
	public IActiveScriptSiteDebug,
	public IActiveScriptSiteInterruptPoll,
	public CComObjectRoot,
	public IProvideClassInfo2Impl<&CLSID_EffectLet, NULL, &LIBID_AveDeskLib>,
	public CComCoClass<CEffectLet,&CLSID_EffectLet>,
	public IDispatchImpl<IRenderCanvas, &IID_IEffectLet, &LIBID_AveDeskLib>
{
public:
	// these are unintentionally static - compiler won't cast void* to this...
	static vdat __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, char* iniFile, char* iniGroup);
	static void __stdcall OnSave(vdat inst, char* iniFile, char* iniGroup);
	static void __stdcall OnDestroy(vdat inst);
	static void __stdcall OnConfigure(vdat inst, HWND owner);
	static void __stdcall OnEvent(vdat inst, UINT event);
	static void __stdcall OnBeforeDraw(vdat inst, HWND hwnd,
	Graphics* g,
	UINT left, UINT top,UINT width, UINT height, 
	ColorMatrix* m, Matrix* matrix,
	BOOL* modified, BOOL isSelected, BOOL isMouseOn);
	static void __stdcall OnAfterDraw(vdat inst, HWND hwnd,
	Graphics* g, Bitmap *bmp, 
	UINT left, UINT top,UINT width, UINT height, 
	ColorMatrix* m, Matrix* matrix,
	BOOL* modified, BOOL isSelected, BOOL isMouseOn);
	static void __stdcall OnDrawPluginInfo(char*,char*,char*,int*);
	static void __stdcall OnSetParent(vdat inst, DrawPlugin *parent);

	// variables
	DrawPlugin *parent;
	CComPtr<IDesklet> desklet;
	char *inifile;
	char *inisection;
	BOOL isPreview, isDebug;
	CComQIPtr<IActiveScript> script;
	CComDispatchDriver scriptDisp;
	std::map<int, DISPID> dispMap;
	int addOffset;
	HWND hwnd;
	SCRIPTSTATE scriptstate;
	BOOL inscript, abort;
	CComObject<CTimer> *timer;
	CComObject<CColorMatrix> *colorMatrix;
	CComObject<CMatrix> *matrix;
	CComObject<CPreferences> *preferences;
	CComObject<CImages> *images;
	CComBSTR path;

	// script debugging
    CComPtr<IDebugDocumentHelper> pDebugDocumentHelper;
	CComPtr<IDebugHelperClient> pDbg;
	CComPtr<IApplication> pApp;
    DWORD dwDocCookie;
    CComQIPtr<IDebugDocumentContext> pDebugDocumentContext;




	// ave: possible extra members
	/*
	std::map<std::wstring,Bitmap*> bitmaps;
	*/
	Graphics* graphics;

	CEffectLet()  /* ave: possible init. of extra members
					graphics(0)

					*/
	{
		parent = NULL;
		inifile = NULL;
		inisection = NULL;
		isPreview = FALSE;
		hwnd = NULL;
		scriptstate = SCRIPTSTATE_UNINITIALIZED;
		inscript = FALSE;
		abort = FALSE;
		timer = NULL;

		colorMatrix = NULL;
		matrix = NULL;
		preferences = NULL;
		images = NULL;

		graphics = NULL;

		pDbg = NULL;
	}

	~CEffectLet();


	BEGIN_COM_MAP(CEffectLet)
		COM_INTERFACE_ENTRY(IEffectLet)
		COM_INTERFACE_ENTRY2(IDispatch, IEffectLet)
		COM_INTERFACE_ENTRY(IActiveScriptSite)
		COM_INTERFACE_ENTRY(IActiveScriptSiteWindow)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		COM_INTERFACE_ENTRY(IRenderCanvas)
		COM_INTERFACE_ENTRY(IDebugDocumentHost)
		COM_INTERFACE_ENTRY(IActiveScriptSiteDebug)
		COM_INTERFACE_ENTRY(IActiveScriptSiteInterruptPoll)
	END_COM_MAP()
	//DECLARE_NOT_AGGREGATABLE(CEffectLet) 

	//DECLARE_REGISTRY_RESOURCEID(IDR_EffectLet)

// IEffectLet
public:
	STDMETHOD(get_Canvas)(/*[out, retval]*/ int *pVal);
	STDMETHOD(get_Canvas)(/*[out, retval]*/ IRenderCanvas* *pVal);
	STDMETHOD(get_Application)(/*[out, retval]*/ IApplication* *pVal);
	STDMETHOD(get_Images)(/*[out, retval]*/ IAveImages* *pVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Author)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	/* ave: prototypes of possible functions
	STDMETHOD(DrawImage)(BSTR imageFileName,int left, int top, int width, int height);
	STDMETHOD(UnloadImage)(BSTR imageFileName);
	STDMETHOD(LoadImage)(BSTR imageFileName);
	*/
	STDMETHOD(get_Matrix)(/*[out, retval]*/ IMatrix **pVal);
	STDMETHOD(get_ColorMatrix)(/*[out, retval]*/ IColorMatrix* *pVal);
	STDMETHOD(get_Ticker)(/*[out, retval]*/ ITimer* *pVal);
	STDMETHOD(get_Desklet)(/*[out, retval]*/ IDesklet **pVal);
	STDMETHOD(get_Preferences)(/*[out, retval]*/ IPreferences* *pVal);
	CComVariant CallScript0(UINT event);
	void CreateScript();
	CComVariant CallScript(BSTR Func, int numArg, CComVariant *vararg);

// IActiveScriptSite
	STDMETHOD(GetLCID)(LCID * plcid)
	{
		if (plcid == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(GetItemInfo)(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown * * ppiunkItem, ITypeInfo * * ppti);
	STDMETHOD(GetDocVersionString)(BSTR * pbstrVersion)
	{
	    if (pbstrVersion == NULL)
			return E_POINTER;
			
		if( parent )
		{
			CComBSTR nm(parent->GetName());
		    nm.CopyTo(pbstrVersion);
			return S_OK;
		}
		return E_FAIL;
	}
	STDMETHOD(OnScriptTerminate)(const VARIANT * pvarResult, const EXCEPINFO * pexcepinfo)
	{
		return S_OK;
	}
	STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState)
	{
		scriptstate = ssScriptState;
		return S_OK;
	}
	STDMETHOD(OnScriptError)(IActiveScriptError * pscripterror);
	STDMETHOD(OnEnterScript)()
	{
		inscript = TRUE;
		return S_OK;
	}
	STDMETHOD(OnLeaveScript)()
	{
		inscript = FALSE;
		return S_OK;
	}
	// IActiveScriptSiteWindow
	STDMETHOD(GetWindow)(HWND * phwnd)
	{
		if (phwnd == NULL)
			return E_POINTER;
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

// IActiveScriptSiteInterruptPoll
	STDMETHOD(QueryContinue)()
	{
		if( abort )
			return S_FALSE;
		return S_OK;
	}


// IRenderCanvas
	STDMETHOD(GetGraphics)(int *pVal)
	{
		if (pVal == NULL)
			return E_POINTER;

		*pVal = (int)graphics;	
		return S_OK;
	}
	STDMETHOD(GetHDC)(int *pVal)
	{
		if (pVal == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}

// IActiveScriptSiteDebug
	STDMETHOD(GetDocumentContextFromPosition)( 
		DWORD dwSourceContext,
		ULONG uCharacterOffset,
		ULONG uNumChars,
		IDebugDocumentContext  **ppsc);

	STDMETHOD(GetApplication)( 
		IDebugApplication  * *ppda);

	STDMETHOD(GetRootApplicationNode)( 
		IDebugApplicationNode  * *ppdanRoot);

	STDMETHOD(OnScriptErrorDebug)( 
		IActiveScriptErrorDebug  *pErrorDebug,
		BOOL __RPC_FAR *pfEnterDebugger,
		BOOL __RPC_FAR *pfCallOnScriptErrorWhenContinuing);
		CComPtr<IDebugDocumentHelper> m_pDebugDocumentHelper;
		DWORD m_dwDocCookie;
		CComQIPtr<IDebugDocumentContext> m_pDebugDocumentContext;

// IDebugDocumentHost
	STDMETHOD(GetDeferredText)( 
		DWORD dwTextStartCookie,
		WCHAR  *pcharText,
		SOURCE_TEXT_ATTR  *pstaTextAttr,
		ULONG  *pcNumChars,
		ULONG cMaxChars);

	STDMETHOD(GetScriptTextAttributes)( 
		LPCOLESTR pstrCode,
		ULONG uNumCodeChars,
		LPCOLESTR pstrDelimiter,
		DWORD dwFlags,
		SOURCE_TEXT_ATTR  *pattr);

	STDMETHOD(OnCreateDocumentContext)( 
		IUnknown  * *ppunkOuter);

	STDMETHOD(GetPathName)( 
		BSTR  *pbstrLongName,
		BOOL  *pfIsOriginalFile);

	STDMETHOD(GetFileName)( 
		BSTR  *pbstrShortName);

	STDMETHOD(NotifyChanged)( void);

};

#endif // !defined(AFX_EFFECTLET_H__5969C808_4D45_41A8_A607_0FB026886E3D__INCLUDED_)
