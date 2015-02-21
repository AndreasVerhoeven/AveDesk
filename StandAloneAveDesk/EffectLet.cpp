// EffectLet.cpp : Implementierung von CAveDeskApp und DLL-Registrierung.

#include "stdafx.h"
#include "AveDesk.h"
#include "constants.h"
#include "docklet.h"

#include "Application.h"
#include "debug.h"
#include "Desklet.h"
#include "EffectLet.h"

#include <sstream>
#include <iomanip>

// ave: for possible use of boost::shared_ptr
/* #include<memory> */



/////////////////////////////////////////////////////////////////////////////
//

CComObject<CEffectLet>* __stdcall CEffectLet::OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, char* iniFile, char* iniGroup)
{
	CComObject<CEffectLet>* ptr = NULL;

	CComObject<CEffectLet>::CreateInstance(&ptr);
	if( ptr )
	{
		ptr->AddRef();
		ptr->inifile = iniFile;
		ptr->inisection = iniGroup;
		ptr->hwnd = hwndDesklet;
		// ini file handling and real creation is in onsetparent

	}

	return ptr;
}

void __stdcall CEffectLet::OnSave(vdat inst, char* iniFile, char* iniGroup)
{
	USES_CONVERSION;
	if( inst )
	{
		if( iniFile && iniGroup )
		{
			inst->inifile = iniFile;
			inst->inisection = iniGroup;
			CComPtr<IPreferences> dummy;
			inst->get_Preferences(&dummy.p);
			dummy->put_IniFile(A2W(inst->inifile));
			dummy->put_IniSection(A2W(inst->inisection));
			inst->CallScript0(EVENT_ONSAVE);
		}
	}
}

void __stdcall CEffectLet::OnDestroy(vdat inst)
{
	if( inst )
	{
		if( inst->timer )
			inst->timer->Detach();
		OnEvent(inst, EVENT_ONDESTROY);
		if( inst->script )
		{
			inst->pDebugDocumentContext.Release();
			if( inst->pDebugDocumentHelper )
			{
				inst->pDebugDocumentHelper->Detach();
				inst->pDebugDocumentHelper->SetDebugDocumentHost(NULL);
			}
			inst->pDebugDocumentHelper.Release();
			if( inst->pDbg.p )
			{
				inst->pDbg.Release();
			}
			inst->script->SetScriptState(SCRIPTSTATE_DISCONNECTED);
			inst->script->Close();
			inst->scriptDisp.Release();
			inst->script.Release();
		}
		if( inst->preferences )
			inst->preferences->parent = NULL;
		if( inst->images )
			inst->images->parent = NULL;
		inst->Release();
	}
}


void __stdcall CEffectLet::OnConfigure(vdat inst, HWND owner)
{
	if( inst )
	{
		inst->isPreview = TRUE;
		inst->CallScript0(EVENT_ONCONFIGURE);
		inst->preferences->parentHwnd = owner;
		inst->preferences->Show();
		inst->isPreview = FALSE;
	}
}

CComVariant CEffectLet::CallScript0(UINT event)
{
	CComVariant var;
	std::map<int, DISPID>::iterator it;
	if( script.p && (it = dispMap.find(event)) != dispMap.end() )
	{
		HRESULT hr = scriptDisp.Invoke0((*it).second, &var);
	}
	return var;
}

void __stdcall CEffectLet::OnEvent(vdat inst, UINT event)
{
	if( inst )
		inst->CallScript0(event);
}


void __stdcall CEffectLet::OnBeforeDraw(vdat inst, HWND hwnd,
	Graphics* g,UINT left, UINT top,UINT width, UINT height, 
	ColorMatrix* m, Matrix* matrix,BOOL* modified,
	BOOL isSelected, BOOL isMouseOn)
{
	// ave: bleeeh
	inst->graphics = g;


	// on demand creation of matrices' wrappers
	if( NULL == inst->colorMatrix )
	{
		CComPtr<IColorMatrix> dummy;
		inst->get_ColorMatrix(&dummy.p);
	}
	if( NULL == inst->matrix )
	{
		CComPtr<IMatrix> dummy;
		inst->get_Matrix(&dummy.p);
	}

	inst->colorMatrix->Set(*m);
	inst->matrix->CopyFrom(matrix);

	// dispatch arguments need to be passed upside down
	CComVariant vararg[3] = {inst->isPreview, isSelected, isMouseOn};
	CComVariant ret = inst->CallScript(L"OnBeforeDraw", 3, vararg);
	if( SUCCEEDED( ret.ChangeType(VT_BOOL) ) && VARIANT_TRUE == ret.boolVal )
	{
		*modified = TRUE;
		ColorMatrix cm = inst->colorMatrix->Get();
		CopyMemory(m, &cm, sizeof(ColorMatrix));
		inst->matrix->CopyTo(matrix);
	}
	else
		*modified = FALSE;

	// ave: hmm
	inst->graphics = NULL;
}

//(void*,HWND,Graphics* g,Bitmap* bmp,UINT left, UINT top, UINT width, UINT height, ColorMatrix* m, Matrix* matrix,BOOL*,BOOL,BOOL);
void __stdcall CEffectLet::OnAfterDraw(vdat inst, 
  HWND hwnd,
  Graphics* g, Bitmap *bmp, 
  UINT left, UINT top,UINT width, UINT height, 
  ColorMatrix* m, Matrix* matrix,
  BOOL* modified, BOOL isSelected, BOOL isMouseOn)
{
	// ave: bleeeh
	inst->graphics = g;

	// on demand creation of matrices' wrappers
	if( NULL == inst->colorMatrix )
	{
		CComPtr<IColorMatrix> dummy;
		inst->get_ColorMatrix(&dummy.p);
	}
	if( NULL == inst->matrix )
	{
		CComPtr<IMatrix> dummy;
		inst->get_Matrix(&dummy.p);
	}

	inst->colorMatrix->Set(*m);
	inst->matrix->CopyFrom(matrix);

	// dispatch arguments need to be passed upside down
	CComVariant vararg[3] = {inst->isPreview, isSelected, isMouseOn};
	CComVariant ret = inst->CallScript(L"OnAfterDraw", 3, vararg);
	if( SUCCEEDED( ret.ChangeType(VT_BOOL) ) && VARIANT_TRUE == ret.boolVal )
	{
		*modified = TRUE;
		ColorMatrix cm = inst->colorMatrix->Get();
		CopyMemory(m, &cm, sizeof(ColorMatrix));
		inst->matrix->CopyTo(matrix);
	}
	else
		*modified = FALSE;

	// ave: bdfsdf
	inst->graphics = NULL;
}


void __stdcall CEffectLet::OnDrawPluginInfo(char*,char*,char*,int*)
{
  // don't know what to do
}



void CEffectLet::OnSetParent(vdat inst, DrawPlugin *parent)
{
	USES_CONVERSION;
	inst->parent = parent;
	inst->path = A2W(inst->parent->GetFilename().c_str());
	// create script and stuff.
	CComPtr<IPreferences> dummy;
	inst->get_Preferences(&dummy.p);
	dummy->put_IniFile(A2W(inst->inifile));
	dummy->put_IniSection(A2W(inst->inisection));

	// get the script text
	inst->CreateScript();

}

CComVariant CEffectLet::CallScript(BSTR Func, int numArg, CComVariant *vararg)
{
	CComVariant var;

	if( scriptDisp.p )
		scriptDisp.InvokeN(Func, vararg, numArg, &var);
	return var;
}

#define MAPDISP(t, i) \
	if( SUCCEEDED( scriptDisp.GetIDOfName(A2W(""#t), &dsp ) ) ) \
		dispMap[i] = dsp; \
	else AtlTrace("%s not mapped\n", ""#t);
void CEffectLet::CreateScript()
{
	USES_CONVERSION;

	CComPtr<IXMLDOMDocument> doc = parent->GetXML();
	CComPtr<IXMLDOMNode> nod, eng, nodp, namp;
	CComBSTR scrText, scrEngine, scrName;
	CComPtr<IXMLDOMNamedNodeMap> attr;
	CLSID iid = {0};
	HRESULT hr = S_OK;
	isDebug = 0;
	dwDocCookie = 0;

	// monster if
	if( doc.p && 
		SUCCEEDED( doc->selectSingleNode(L"effectlet/script", &nod.p) ) && 
		SUCCEEDED( nod->get_attributes(&attr.p) ) &&
		SUCCEEDED( attr->getNamedItem(CComBSTR("engine"), &eng.p) ) &&
		SUCCEEDED( eng->get_text(&scrEngine) ) && scrEngine.Length() &&
		SUCCEEDED( CLSIDFromProgID(scrEngine, &iid) ) &&
		SUCCEEDED( nod->get_text(&scrText) ) && scrText.Length() &&
		SUCCEEDED( script.CoCreateInstance(iid) )
	)
	{
		
		CComBSTR effText = NULL;
		doc->get_xml(&effText);
		/*
		CString str = effText;
		str.Replace(L"\r", L"");
		effText = str;
		*/
		// houston, we almost have a liftoff
		CComQIPtr<IActiveScriptParse> parser = script;
		parser->InitNew();

		// read the preferences
		if( SUCCEEDED( doc->selectSingleNode(L"effectlet/preferences", &nodp.p) ) )
		{
			CComPtr<IPreferences> dummy;
			get_Preferences(&dummy.p);
			preferences->parent = this;
			preferences->LoadXML(nodp);
		}


		// bother with script debugging?
		PreferencesMap::iterator it = preferences->prefs.find(L"Debug");
		if( it != preferences->prefs.end() )
		{
			CString strDebug, strKey;
			strKey = A2W(parent->GetName());
			strKey += L".";
			strKey += L"Debug";
			GetPrivateProfileString(
				A2W(inisection), strKey, L"False", strDebug.GetBuffer(MAX_PATH), 
				MAX_PATH, A2W(inifile));
			strDebug.ReleaseBuffer();

			isDebug = !_wcsicmp(strDebug, L"True");

		}

		// calculate the script line offset for
		// easier debugging
		BSTR start, stop, seeker;
		int lineOffset = 0;
		start = wcsstr(effText, L"<script");
		start = wcsstr(start, L"<![CDATA[");
		start += 9;
		while( *start == L'\n' )
			start++;
		stop = wcsstr(start, L"]]");
		seeker = effText;
		while(seeker < start)
		{
			if( *seeker == L'\n' )
				lineOffset++;
			seeker++;
		}

		AtlTrace(L"%s(%d) : loaded effectlet with %s-engine\n",
			path, lineOffset, scrEngine);

		// attach script to debugger instance
		if(isDebug && NULL != _Dbg) 
		{
			EnterCriticalSection(&_Dbg->pDebugAppLock);
			if( !_Dbg->pDebugApplication.p )
			{
				CComPtr<IProcessDebugManager> ipdm;
				hr = ipdm.CoCreateInstance(CLSID_ProcessDebugManager);
				// Attach to PDM if not already done (kinda delayload)				
				if( ipdm.p )
				{
					hr = ipdm->CreateApplication(&_Dbg->pDebugApplication.p);
					if( _Dbg->pDebugApplication.p )
					{
						TCHAR fNam[MAX_PATH] = L"AveDesk";
						GetModuleFileName(NULL, fNam, MAX_PATH);
						hr = _Dbg->pDebugApplication->SetName(fNam);
						hr = ipdm->AddApplication(
							_Dbg->pDebugApplication, 
							&_Dbg->pDebugApplicationContext);
						InterlockedIncrement(&_Dbg->pDebugAppCount);
					}
				}
			}
			LeaveCriticalSection(&_Dbg->pDebugAppLock);
			
			if( !pDebugDocumentHelper.p )
			{
				CComPtr<IProcessDebugManager> ipdm;
				hr = ipdm.CoCreateInstance(CLSID_ProcessDebugManager);
				if( ipdm.p )
					ipdm->CreateDebugDocumentHelper(NULL, &pDebugDocumentHelper.p);
			}
			if( pDebugDocumentHelper.p )
			{
				HRESULT hr = 0;
				hr = pDebugDocumentHelper->Init(
					_Dbg->pDebugApplication, 
					A2W(parent->GetName()), 
					path, 
					TEXT_DOC_ATTR_READONLY); 
				hr = pDebugDocumentHelper->Attach(NULL);
				//hr = pDebugDocumentHelper->SetDocumentAttr(TEXT_DOC_ATTR_READONLY);
				hr = pDebugDocumentHelper->SetDebugDocumentHost(this);
				//hr = pDebugDocumentHelper->AddDBCSText("AveDesk EffectLet");
				hr = pDebugDocumentHelper->AddUnicodeText(effText);
				hr = pDebugDocumentHelper->DefineScriptBlock(
						start-effText, 
						stop-start, 
						script, 
						FALSE, 
						&dwDocCookie);
				hr = pDebugDocumentHelper->CreateDebugDocumentContext(
						start-effText, 
						stop-start, 
						&pDebugDocumentContext);
				hr = _Dbg->AttachDebugClient(pDebugDocumentHelper, &pDbg.p);
			}
		}
		else
			_Dbg->AttachDebugClient(NULL, &pDbg.p);

		script->SetScriptSite(this);

		// add script scope local variables
		hr = script->AddTypeLib(LIBID_AveDeskLib, 1, 0, 0);
		hr = script->AddNamedItem(CComBSTR(L"Me"), 
			SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_GLOBALMEMBERS | SCRIPTITEM_ISVISIBLE );
		hr = script->AddNamedItem(CComBSTR(L"EffectLet"), 
			SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_ISVISIBLE );
		hr = script->AddNamedItem(CComBSTR(L"Application"), 
			SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_ISVISIBLE);
		hr = script->AddNamedItem(CComBSTR(L"Debug"), 
			SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_ISVISIBLE);

		// add preferences (predefined items to script scope)
		
		for(it = preferences->prefs.begin(); 
			it != preferences->prefs.end(); it++)
		{
			script->AddNamedItem(
			(*it).second->name, SCRIPTITEM_ISVISIBLE);
		}

		// preferences itself is global, although its also a property of effectlet
		nodp = NULL;

		// add images
		if( SUCCEEDED( doc->selectSingleNode(L"effectlet/images", &nodp.p) ) && nodp.p)
		{
			CComObject<CImages>::CreateInstance(&images);
			images->AddRef();
			images->parent = this;
			images->LoadXML(nodp);

			ImagesMap::iterator it = images->imgs.begin();
			for( ; it != images->imgs.end(); it++ )
			{
				script->AddNamedItem(
				(*it).second->name, SCRIPTITEM_ISVISIBLE);
			}
		}

		// and parse the script text
		EXCEPINFO ep = {0};
		*stop = 0;
		hr = parser->ParseScriptText(
			start, 
			NULL, //L"Effectlet", 
			pDebugDocumentContext, 
			NULL, //L"]]"
			dwDocCookie,
			0,//lineOffset, // one for CR aft. CDATA, one for VC
			0,//SCRIPTTEXT_ISPERSISTENT|SCRIPTTEXT_ISVISIBLE, 
			NULL, 
			&ep);

		//script->SetScriptState(SCRIPTSTATE_INITIALIZED);
		script->SetScriptState(SCRIPTSTATE_STARTED);
		//script->SetScriptState(SCRIPTSTATE_CONNECTED);
		if( SUCCEEDED( hr ) && SUCCEEDED( script->GetScriptDispatch(NULL, &scriptDisp.p) ) )
		{
			DISPID dsp;
			//drv.GetIDOfName(L"", &dsp);
			MAPDISP(OnMouseOn,    EVENT_ONMOUSEON);
			MAPDISP(OnMouseOut,   EVENT_ONMOUSEOUT);  
			MAPDISP(OnSelect,     EVENT_ONSELECT);	
			MAPDISP(OnDeselect,   EVENT_ONDESELECT);	
			MAPDISP(OnShow,       EVENT_ONSHOW); 
			MAPDISP(OnHide,       EVENT_ONHIDE);
			MAPDISP(OnConfigure,  EVENT_ONCONFIGURE);
			MAPDISP(OnStartMove,  EVENT_ONSTARTMOVE);
			MAPDISP(OnEndMove,    EVENT_ONENDMOVE);
			MAPDISP(OnLeftClick,  EVENT_ONLEFTCLICK);
			MAPDISP(OnRightClick, EVENT_ONRIGHTCLICK);
			MAPDISP(OnDestroy,    EVENT_ONDESTROY);
			MAPDISP(OnLoad,       EVENT_ONLOAD);
			MAPDISP(OnSave,       EVENT_ONSAVE);
			MAPDISP(OnTimer,      EVENT_ONTIMER);
		}


		if( SUCCEEDED( hr ) )
		{
			CComVariant var, var2;
			//hr = scriptDisp.GetPropertyByName(L"Me", &var); 
			//var.Clear();
			hr = scriptDisp.GetPropertyByName(L"Application", &var); 
			var.Clear();

			hr = scriptDisp.Invoke0(L"OnCreate");
		}
	}
}

STDMETHODIMP CEffectLet::OnScriptError(IActiveScriptError * pscripterror)
{
	USES_CONVERSION;
	EXCEPINFO ep = {0};
	CComBSTR errLin;
	DWORD Context; ULONG Line; LONG CharPos;
	CComPtr<IActiveScriptError> pErr = pscripterror;
	if( pErr )
	{
		pErr->GetExceptionInfo(&ep);
		pErr->GetSourceLineText(&errLin);
		pErr->GetSourcePosition(&Context, &Line, &CharPos);
		AtlTrace(L"CEffectLet::OnScriptError [%s] '%s', %ld:%ld\n",
		errLin, ep.bstrDescription, Line, CharPos);
		AtlTrace(L"%s(%ld) : error 0x%08x : '%s'\n",
		path, Line, ep.scode, ep.bstrDescription);

		std::wstringstream wss;
		wss << path.operator BSTR() << L" (" << Line << L") error ";

		// fix, NULL is a valid BSTR (it's the empty string ""), but the STL doesn't like to get
		// NULL passed in as a string, so check the ep.bstrDescription's value.
		wss << std::hex << ep.scode << L":\n" << (NULL != ep.bstrDescription ? ep.bstrDescription : L"");
		MessageBoxW(0,wss.str().c_str(),0,0);
	}
	return S_OK;
}

STDMETHODIMP CEffectLet::GetItemInfo(
  LPCOLESTR pstrName, 
  DWORD dwReturnMask, 
  IUnknown * * ppiunkItem, 
  ITypeInfo * * ppti)
{
	HRESULT hr = S_OK;
	// effectlet accessor object
	if( !_wcsicmp(pstrName, L"Me" ) || !_wcsicmp(pstrName, L"EffectLet" ) )
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			GetClassInfo(ppti);
			(*ppti)->AddRef();	 // because returning
		}
		if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
		{
			hr = QueryInterface(IID_IUnknown, (void**) ppiunkItem);
		}
	}
	// global application object
	else if( !_wcsicmp(pstrName, L"Application" ) )
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			_App->GetTypeInfo(0,0,ppti);
		}
		if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
		{
			hr = _App->QueryInterface(IID_IUnknown, (void**) ppiunkItem);
		}
	}
	// local debugger object
	else if( !_wcsicmp(pstrName, L"Debug" ) )
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			pDbg->GetTypeInfo(0,0,ppti);
		}
		if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
		{
			hr = pDbg->QueryInterface(IID_IUnknown, (void**) ppiunkItem);
		}
	}
	// preferences from XML
	else if( preferences->IsKnownPreference(
				pstrName, dwReturnMask, 
				ppiunkItem, ppti) )
	{
		return S_OK;
	}
	// named images from XML
	else if( images && images->IsKnownImage(
				pstrName, dwReturnMask,
				ppiunkItem, ppti) )
	{
		return S_OK;
	}
	else
		return TYPE_E_ELEMENTNOTFOUND;

	return hr;
}


STDMETHODIMP CEffectLet::get_Preferences(IPreferences **pVal)
{
	if( NULL == preferences )
	{
		CComObject<CPreferences>::CreateInstance(&preferences);
		preferences->AddRef();
	}
	preferences->QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Desklet(IDesklet **pVal)
{
	if( NULL == desklet.p )
	{
		Docklet::DockletsMap::iterator it = Docklet::Docklets.find(hwnd);
		if( it != Docklet::Docklets.end() )
		{
			Docklet *ptr = (*it).second;
			if( ptr )
			{
				ptr->GetWrapper(&desklet.p);
			}
		}
	}
	if( NULL != desklet.p )
		desklet.QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Ticker(ITimer **pVal)
{
	if( NULL == timer )
	{
		CComObject<CTimer>::CreateInstance(&timer);
		timer->AddRef();
		if( dispMap.end() != dispMap.find(EVENT_ONTIMER) ) 
		timer->Attach(hwnd, dispMap[EVENT_ONTIMER], scriptDisp);
	}
	timer->QueryInterface(pVal);
	return S_OK;
}

CEffectLet::~CEffectLet()
{
	if( timer )
		timer->Release();

	if( colorMatrix )
		colorMatrix->Release();

	if( matrix )
		matrix->Release();

	if( preferences )
		preferences->Release();

	if( images )
		images->Release();

	if( pDbg )
		pDbg.Release();

	if( isDebug && _Dbg != NULL )
	{
		// detach from PDM (ProcessDebugManager)
		EnterCriticalSection(&_Dbg->pDebugAppLock);
		if( _Dbg->pDebugApplication.p )
		{
			InterlockedDecrement(&_Dbg->pDebugAppCount);
			if( !_Dbg->pDebugAppCount )
				_Dbg->pDebugApplication.Release();
		}
		LeaveCriticalSection(&_Dbg->pDebugAppLock);
	}

}

STDMETHODIMP CEffectLet::get_ColorMatrix(IColorMatrix **pVal)
{
	if( NULL == colorMatrix )
	{
		CComObject<CColorMatrix>::CreateInstance(&colorMatrix);
		colorMatrix->Set(ColorMatrixHelper::GetIdentityMatrix());
		colorMatrix->AddRef();
	}

	colorMatrix->QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Matrix(IMatrix **pVal)
{
	if( NULL == matrix )
	{
		CComObject<CMatrix>::CreateInstance(&matrix);
		matrix->AddRef();
	}
	matrix->QueryInterface(pVal);
	return S_OK;
}

// ave: possible functions.

/*
STDMETHODIMP CEffectLet::LoadImage(BSTR imageFileName)
{
	char tmp[MAX_PATH] = {0};
	WideCharToMultiByte(GetACP(),0,imageFileName,-1,tmp,sizeof(tmp)/sizeof(tmp[0]),0,0);
	Bitmap* bmp = DeskletLoadGDIPlusImage(tmp);

	std::wstring wFName(imageFileName);
	std::map<std::wstring,Bitmap*>::iterator iter = bitmaps.find(wFName);
	if( iter != bitmaps.end() )
	{
		delete iter->second;
		iter->second = bmp;
	}
	else
	{
		bitmaps.insert( std::make_pair(wFName,bmp));
	}

	return S_OK;
}

STDMETHODIMP CEffectLet::UnloadImage(BSTR imageFileName)
{
	std::wstring wFName(imageFileName);
	std::map<std::wstring,Bitmap*>::iterator iter = bitmaps.find(wFName);
	if( iter != bitmaps.end() )
	{
		delete iter->second;
		bitmaps.erase(iter);
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

STDMETHODIMP CEffectLet::DrawImage(BSTR imageFileName,int left, int top, int width, int height)
{
	if(graphics)
	{
		std::wstring wFName(imageFileName);
		std::map<std::wstring,Bitmap*>::iterator iter = bitmaps.find(wFName);		
		if( iter != bitmaps.end() )
		{
			graphics->DrawImage(iter->second,left, top, width, height);
		}
		else
		{
			char tmp[MAX_PATH] = {0};
			WideCharToMultiByte(GetACP(),0,imageFileName,-1,tmp,sizeof(tmp)/sizeof(tmp[0]),0,0);
			boost::shared_ptr<Bitmap> bmp( DeskletLoadGDIPlusImage(tmp) );
			graphics->DrawImage(&*bmp,left, top, width, height);
		}
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}
*/

STDMETHODIMP CEffectLet::get_Name(BSTR *pVal)
{
	CComBSTR str = parent->GetName();
	str.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Author(BSTR *pVal)
{
	CComBSTR str = parent->GetAuthor();
	str.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Description(BSTR *pVal)
{
	CComBSTR str = parent->GetNotes();
	str.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Version(BSTR *pVal)
{
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Images(IAveImages **pVal)
{
	if( images )
		images->QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Application(IApplication **pVal)
{
	desklet->get_Application(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Canvas(IRenderCanvas **pVal)
{
	CComQIPtr<IRenderCanvas> ptr = this;
	ptr.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CEffectLet::get_Canvas(int *pVal)
{
	CComPtr<IRenderCanvas> ths = this;
	*pVal = (int)ths.p;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// IDebugDocumentHost 
STDMETHODIMP CEffectLet::GetDeferredText( 
				DWORD dwTextStartCookie,
				WCHAR  *pcharText,
				SOURCE_TEXT_ATTR  *pstaTextAttr,
				ULONG  *pcNumChars,
				ULONG cMaxChars)
{
	return E_NOTIMPL;
}

STDMETHODIMP CEffectLet::GetScriptTextAttributes( 
				LPCOLESTR pstrCode,
				ULONG uNumCodeChars,
				LPCOLESTR pstrDelimiter,
				DWORD dwFlags,
				SOURCE_TEXT_ATTR  *pattr)
{
	return E_NOTIMPL;
}

STDMETHODIMP CEffectLet::OnCreateDocumentContext( 
  IUnknown  * *ppunkOuter)
{
	return E_NOTIMPL;
}

STDMETHODIMP CEffectLet::GetPathName( 
  BSTR  *pbstrLongName,
  BOOL  *pfIsOriginalFile)
{
	path.CopyTo(pbstrLongName);
	*pfIsOriginalFile = TRUE;
	return S_OK;
}


STDMETHODIMP CEffectLet::GetFileName( 
  BSTR  *pbstrShortName)
{
  CString BaseName = path;
  BaseName.GetBuffer(MAX_PATH);
  PathStripPath(BaseName.GetBuffer(MAX_PATH+1));
  BaseName.ReleaseBuffer();

  CComBSTR bn = BaseName;
  bn.CopyTo(pbstrShortName);
  return S_OK;
}

STDMETHODIMP CEffectLet::NotifyChanged( void)
{
  return S_OK;
}

///////////////////////////////////////////////////////////////////////////
// IActiveScriptSiteDebug
STDMETHODIMP CEffectLet::GetDocumentContextFromPosition( 
	DWORD dwSourceContext,
	ULONG uCharacterOffset,
	ULONG uNumChars,
	IDebugDocumentContext  **ppsc)
{
	if (!ppsc) 
		return E_POINTER; 

	*ppsc = NULL; 

	// every running document has a special "cookie" associated with it. 
	// this code assumes only 1 document with a cookie value stored in 
	// m_dwDocCookie. It then asks the helper interface IDebugDocumentHelper 
	// to convert from a character offset to a document context interface. 
	ULONG ulStartPos=0; 
	HRESULT hr = pDebugDocumentHelper->GetScriptBlockInfo(
		dwDocCookie, NULL, &ulStartPos, NULL); 
	if( SUCCEEDED(hr) ) 
		return pDebugDocumentHelper->CreateDebugDocumentContext( 
					ulStartPos + uCharacterOffset, 
					uNumChars, 
					ppsc);
	return hr; 
}

STDMETHODIMP CEffectLet::GetApplication( 
	IDebugApplication  **ppda)
{
	//return E_NOTIMPL;
	if (!ppda) 
		return E_POINTER; 
	*ppda = NULL; 

	if (!isDebug) 
		return E_FAIL;
	
	if( NULL == _Dbg || !_Dbg->pDebugApplication.p )
		return E_UNEXPECTED;



	// return the IDebugApplication interface we created earlier.
	_Dbg->pDebugApplication.CopyTo( ppda ); 
	return S_OK; 
}

STDMETHODIMP CEffectLet::GetRootApplicationNode( 
  IDebugApplicationNode  **ppdanRoot)
{
	
	//return E_NOTIMPL;
	// since we only have 1 document, we can safely return NULL here. 
	if ( !ppdanRoot ) 
		return E_INVALIDARG; 
	//*ppdanRoot = NULL; 
	//return E_NOTIMPL; 
	if( pDebugDocumentHelper )
		return pDebugDocumentHelper->GetDebugApplicationNode(ppdanRoot);
	else
		return E_NOTIMPL;
}

STDMETHODIMP CEffectLet::OnScriptErrorDebug( 
  IActiveScriptErrorDebug  *pErrorDebug,
  BOOL  *pfEnterDebugger,
  BOOL  *pfCallOnScriptErrorWhenContinuing)
{
	*pfEnterDebugger=(isDebug)?TRUE:FALSE; 
	*pfCallOnScriptErrorWhenContinuing=TRUE;
	return S_OK;
}
