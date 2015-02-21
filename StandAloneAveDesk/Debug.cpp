// Debug.cpp : Implementierung von CDebug
#include "stdafx.h"
#include "helper_funcs.h"
#include "AveDesk.h"
#include "Debug.h"
#include "Application.h"


//IID_IDebugDocumentHelper {51973C26-CB0C-11d0-B5C9-00A0244A0E7A}
const IID IID_IDebugDocumentHelper = {0x51973C26,0xCB0C,0x11d0,{0xB5,0xC9,0x00,0xA0,0x24,0x4A,0x0E,0x7A}};
extern CComObject<CApplication> *_App;
/////////////////////////////////////////////////////////////////////////////
// CDebug
CRITICAL_SECTION CDebug::pDebugAppLock = {0};


STDMETHODIMP CDebug::Print(BSTR Text)
{
	USES_CONVERSION;
	if( pDebugApplication.p )
    {
        HRESULT hr = pDebugApplication->DebugOutput(Text);
	}
	else if( _Dbg->bConsole )
	{
		echo(Text);
		echo("\n");
	}
	else
	{
		OutputDebugString(Text);
		OutputDebugString(L"\r\n");
    }
	return S_OK;
}

STDMETHODIMP CDebug::Assert(VARIANT Expression)
{
	CComVariant var = Expression;
	if( SUCCEEDED( var.ChangeType(VT_BOOL) ) && !var.boolVal )
		this->Break();
	return S_OK;
}

STDMETHODIMP CDebug::Break()
{
	if( pDebugApplication.p )
	{
		if( SUCCEEDED( pDebugApplication->StartDebugSession() ) )
			pDebugApplication->CauseBreak();
	}
	else
	{
		OutputDebugString(L"Nondebugged script has issued a break. Run it in debug mode.\n");
	}
	return S_OK;
}

STDMETHODIMP CDebug::AttachDebugClient(IUnknown *Document, IDebugHelperClient **pVal)
{
	if( NULL == pVal )
		return E_POINTER;
	
	CComObject<CDebugClient> *ret = NULL;
	CComObject<CDebugClient>::CreateInstance(&ret);
	ret->QueryInterface(pVal);
	ret->pParent = this;
	ret->pDebugDocumentHelper = Document;
	return S_OK;
}

STDMETHODIMP CDebug::get_DebugDocument(IUnknown **pVal)
{
	pDebugDocumentHelper.QueryInterface(pVal);

	return S_OK;
}

STDMETHODIMP CDebug::putref_DebugDocument(IUnknown *newVal)
{
	if( NULL == newVal)
	{
		pDebugDocumentHelper.Release();
		return S_OK;
	}
	IDebugDocumentHelper *hlp = NULL;
	HRESULT hr = newVal->QueryInterface(IID_IDebugDocumentHelper, (void**)&hlp);

	if( SUCCEEDED(hr) )
	{
		pDebugDocumentHelper = hlp;
		hlp->Release();
	}

	return hr;
}

STDMETHODIMP CDebug::get_DebugApplication(IUnknown **pVal)
{
	HRESULT hr = S_OK;
	
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
	if( _Dbg->pDebugApplication.p )
		_Dbg->pDebugApplication.QueryInterface(pVal);
	LeaveCriticalSection(&_Dbg->pDebugAppLock);
	return hr;
}

STDMETHODIMP CDebug::get_DebugApplicationContext(DWORD *pVal)
{
	if( NULL == pVal )
		return E_POINTER;

	*pVal = pDebugApplicationContext;

	return S_OK;
}

STDMETHODIMP CDebug::get_Console(VARIANT_BOOL *pVal)
{
	if( NULL == pVal )
		return E_POINTER;
	*pVal = (::IsWindow(::GetConsoleWindow()))?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CDebug::put_Console(VARIANT_BOOL IsVisible)
{
	HRESULT hr = S_OK;
	HWND hw = ::GetConsoleWindow();
	if( IsVisible )
	{
		if( !hw )
			hr = ShowConsole();
		else
			SetForegroundWindow(hw);
	}
	else
	{
		if( hw )
			hr = HideConsole();
	}
	return hr;
}

DWORD CDebug::GetThreadID()const
{
	return _App->GetMainThreadID();
}

STDMETHODIMP CDebug::ShowConsole()
{
	if( hThread )
		HideConsole();
	
	hEventIn = CreateEvent(NULL, FALSE, FALSE, NULL);
	hEventOut = CreateEvent(NULL, FALSE, FALSE, NULL);
	hThread = CreateThread(
		NULL, 0L, (LPTHREAD_START_ROUTINE)ConsoleThread, this, 0L, NULL);
	return S_OK;
}

STDMETHODIMP CDebug::HideConsole()
{
	if( hThread )
	{
		FreeConsole();
		SetEvent(hEventOut);
		if( !bInScript && !bInExec )
			WaitForSingleObject(hThread, INFINITE);
		hThread = NULL;
	}
	return S_OK;
}

void CDebug::echo(LPCWSTR text)
{
	int iLen = 0;
	if(text != NULL)
		iLen = wcslen(text);
	if(text != NULL && iLen > 0)
	{
		int iCount = WideCharToMultiByte(
			CP_OEMCP, 0, text, iLen, NULL, 0, NULL, NULL);

		char *oem = (char*)alloca(iCount);

		WideCharToMultiByte(
			CP_OEMCP, 0, text, iLen, oem, iCount, NULL, NULL);
		oem[iLen] = 0;

		echo(oem);
	}
}
void CDebug::echo(char* text)
{
	DWORD dwWritten = 0;
	::WriteFile(
		_Dbg->hStdout, text, strlen(text), &dwWritten, NULL);
}

BOOL CDebug::CtrlHandler(DWORD fdwCtrlType) 
{ 
	int iRet;
	switch (fdwCtrlType) 
	{ 
		// Handle the CTRL+C signal. 

		case CTRL_C_EVENT: 

			Beep(1000, 1000); 
			return TRUE; 

		// CTRL+CLOSE: confirm that the user wants to exit. 

		case CTRL_CLOSE_EVENT: 
			iRet = ::MessageBox(
						NULL, 
						L"Quit AveDesk?", L"AveDesk", MB_YESNO|MB_SYSTEMMODAL);
			if( iRet == IDYES )
				_Dbg->HideConsole();
			return TRUE; 

		// Pass other signals to the next handler. 

		case CTRL_BREAK_EVENT:

		case CTRL_LOGOFF_EVENT: 

		case CTRL_SHUTDOWN_EVENT: 

		default: 

			return FALSE; 
	} 
} 

CString LogCrackHR( HRESULT hr )
{
	LPVOID  lpMsgBuf;
	CString strTmp;

	::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
		   FORMAT_MESSAGE_FROM_SYSTEM,
		   NULL,
		   hr,
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		   (LPTSTR) &lpMsgBuf,
		   0,
		   NULL );

	// STR_TMP is defined within LOG.CPP to provide safe format string
	// for both ANSI and UNICODE
	strTmp.Format( _T("%s"), (TCHAR *) lpMsgBuf );


	// Free the buffer.
	::LocalFree( lpMsgBuf );

	return strTmp;
}



STDMETHODIMP CDebug::OnScriptError(IActiveScriptError * pscripterror)
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

		if( bInExec )
		{
			if(ep.bstrDescription) 
				echo(ep.bstrDescription);
			else if( ep.scode )
			{
				echo(LogCrackHR(ep.scode));	
			}
			echo("\n");
		}
		else
		{
			CString tmp;
			tmp.Format(
				L"Syntax Error '%s' in Line %d, Column %d: '%s'\n",
				ep.bstrDescription,
				Line,
				CharPos,
				errLin);

			echo(tmp);
		}
	}
	return S_OK;
}

STDMETHODIMP CDebug::GetItemInfo(
  LPCOLESTR pstrName, 
  DWORD dwReturnMask, 
  IUnknown * * ppiunkItem, 
  ITypeInfo * * ppti)
{
	HRESULT hr = S_OK;
	// global application object
	if( !_wcsicmp(pstrName, L"Application" ) )
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			_App->GetTypeInfo(0, 0, ppti);
		}
		if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
		{
			// hurg
			//safeCreatableImpl<CApplication, CApplication, IApplication>* ptr = 
			//	reinterpret_cast< safeCreatableImpl<CApplication, CApplication, IApplication>* >
			//		(_App);
			hr = _App->QueryInterface(ppiunkItem);
		}
	}
	// global debug object
	else if( !_wcsicmp(pstrName, L"Debug" ) )
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			_Dbg->GetTypeInfo(0, 0, ppti);
		}
		if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
		{
			hr = _Dbg->QueryInterface(ppiunkItem);
		}
	}
	else if( !_wcsicmp(pstrName, L"Console" ) )
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			pDebugConsole->GetClassInfo(ppti);
			(*ppti)->AddRef();	 // because returning
		}
		if( dwReturnMask & SCRIPTINFO_IUNKNOWN )
		{
			hr = pDebugConsole->QueryInterface(IID_IUnknown, (void**) ppiunkItem);
		}
	}
	else
		return TYPE_E_ELEMENTNOTFOUND;

	return hr;
}


DWORD CDebug::ConsoleThread(CComObject<CDebug>* ths)
{
	ths->DoConsoleLoop();
	return 0;
}

void CDebug::DoConsoleLoop()
{
	USES_CONVERSION;
	HRESULT hr = S_OK;
	
	AllocConsole();
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	SetConsoleTitle(L"AveDesk immediate Debug Console");
	SetConsoleCP(850);
    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	bConsole = true;
    SetConsoleMode(hStdin, 
		ENABLE_PROCESSED_INPUT | ENABLE_WINDOW_INPUT | 
		ENABLE_MOUSE_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);

	SetConsoleMode(hStdout,
		ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT);

	DWORD dwRead = 0;
	HWND wnd = GetConsoleWindow();

	echo("Welcome to the AveDesk immediate console.\nEnter ? for help.\n>");
	while( ReadFile(hStdin, buf, 1024, &dwRead, NULL) )
	{
		if( dwRead )
		{
			buf[dwRead] = 0;
			SetEvent(hEventIn);
			PostThreadMessage(
				_App->GetThreadID(), WM_NULL, 0,0);
			WaitForSingleObject(hEventOut, INFINITE);
		}
		echo(">");
	}

	FreeConsole();
	bConsole = false;
}


void CDebug::exec(char* cmd)
{
	USES_CONVERSION;
	switch(cmd[0])
	{
	default:
	case '?':
		{
			CComQIPtr<IActiveScriptParse> parser = pScript;
			if( parser.p )
			{
				CComVariant varRes;
				EXCEPINFO ei = {0};

				DWORD dwFlags = 0;
				if( '?' == cmd[0] )
				{
					dwFlags = SCRIPTTEXT_ISEXPRESSION;
					cmd++;
				}
				bInExec = true;

				HRESULT hr = parser->ParseScriptText(
					A2W(cmd), NULL, NULL, NULL, 0L, 0,
					dwFlags, &varRes, &ei);

				if( SUCCEEDED( hr ) )
				{
					if( SUCCEEDED( varRes.ChangeType(VT_BSTR) ) )
						echo( W2A(varRes.bstrVal) );
					echo("\n");
				}

				bInExec = false;
			}
		}
		break;
	case 0:
		break;
	case '!':
		system(cmd+1);
	}
}

STDMETHODIMP CDebugClient::Assert(VARIANT Expression)
{
	CComVariant var = Expression;
	if( SUCCEEDED( var.ChangeType(VT_BOOL) ) && !var.boolVal )
		this->Break();
	return S_OK;
}

STDMETHODIMP CDebugClient::Break()
{
	if( pDebugDocumentHelper.p )
	{
		if( SUCCEEDED( pDebugDocumentHelper->BringDocumentToTop() )	)
			_Dbg->Break();
		else
			_Dbg->Print(L"Nondebugged script has issued a break. Run it in debug mode.\n");
	}
	return S_OK;
}
STDMETHODIMP CDebugClient::get_Console(VARIANT_BOOL *pVal)
{
	return _Dbg->get_Console(pVal);
}

STDMETHODIMP CDebugClient::put_Console(VARIANT_BOOL IsVisible)
{
	return _Dbg->put_Console(IsVisible);
}

STDMETHODIMP CDebugClient::Print(BSTR Text)
{
	return _Dbg->Print(Text);
}

CDebugConsole::CDebugConsole()
{

}

CDebugConsole::~CDebugConsole()
{

}

void CDebug::Open()
{
	//create our console commands
	CComObject<CDebugConsole>::CreateInstance(&pDebugConsole);
	pDebugConsole->AddRef();

	//create our immediate mode engine
	pScript.p = NULL;
	HRESULT hr = pScript.CoCreateInstance(L"vbScript");
	hr = pScript->SetScriptSite(this);
	// add script scope local variables
	hr = pScript->AddTypeLib(LIBID_AveDeskLib, 1, 0, 0);
	hr = pScript->AddNamedItem(CComBSTR(L"Application"), 
		SCRIPTITEM_GLOBALMEMBERS | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_ISVISIBLE);
	hr = pScript->AddNamedItem(CComBSTR(L"Debug"), 
		SCRIPTITEM_GLOBALMEMBERS | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_ISVISIBLE);
	hr = pScript->AddNamedItem(CComBSTR(L"Console"), 
		SCRIPTITEM_GLOBALMEMBERS | SCRIPTITEM_ISPERSISTENT );

	CComBSTR pth;
	_App->get_UserPath(&pth);
	pth += L"\\start.vbs";

	if( PathFileExists(pth) )
	{
			
	}

	pScript->SetScriptState(SCRIPTSTATE_STARTED);


}

void CDebug::Close()
{
	pScript->SetScriptState(SCRIPTSTATE_DISCONNECTED);
	pScript->Close();
	pScript.Release();
	pDebugConsole->Release();
	pDebugConsole = NULL;
}

void CDebug::HandleConsoleInput()
{
	if( hThread && hEventIn && WaitForSingleObject(hEventIn, 0)==WAIT_OBJECT_0 )
	{
		exec(buf);
		SetEvent(hEventOut);
	}
}
