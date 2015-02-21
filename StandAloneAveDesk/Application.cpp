// Application.cpp : Implementation of CApplication
#include "stdafx.h"
#include "AveDesk.h"
#include "Application.h"
#include "AveImage.h"
#include "helper_funcs.h"

#include "docklet.h" // for Docklet::ReleaseAll()

#include <string>

#include "controldlg.h"
#include "AveInstaller.h" 
#include "XMLSaver.h"
#include "XMLLoader.h"
#include "debug.h"
#include "ThemeBrowserDlg.h"
#include "Modules.h"
#include "Module.h"
#include "ThemeIniToXMLMapper.h"
#include "TrayIconAnimation.h"
#include "DeskletBrowserDlg.h"

class CShowCase;
extern CComObject<CShowCase> *_ShowCase;
extern CModules* _Modules;

#include "AveDeskGlobalSettings.hpp"
extern AveDeskGlobalSettings _globalSettings;
extern int CALLBACK DeskletGetAveDeskVersion();
extern CComObject<CDebug> *_Dbg;

#include "Policies.h"
extern CPolicies _Policies;

/////////////////////////////////////////////////////////////////////////////
// CApplication

// some symbols from AveDesk.cpp
//old extern bool curentDockletsDlgIsVisible;
//old extern HWND cpHwnd;
//old extern int CALLBACK CurrentDockletsDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);

CApplication::CApplication() : 
	m_desklets(NULL),
	m_dlls(NULL),
	m_showCase(NULL),
	m_MainThreadId(0),
	numInitialDocklets(0)
{
}

void CApplication::SetupMainWindow()
{
	//HWND hwnd = OdDockletInstance::CreateDockletWindow(IconWinProc,_Module.GetModuleInstance());
	RECT r = {0};
	m_IconWindow.Create(NULL,r,_T("AveDeskMainWindow"),WS_POPUP | WS_OVERLAPPED,WS_EX_TOOLWINDOW);
}

HWND CApplication::GetMainWindowHandle()const
{
	return m_IconWindow.m_hWnd;
}
void CApplication::InitSafeInstance()
{
	HRESULT hr = CoCreateInstance(CLSID_StdGlobalInterfaceTable,
					 NULL,
					 CLSCTX_INPROC_SERVER,
					 IID_IGlobalInterfaceTable,
					 (void **)&pGIT.p);
	hr = pGIT->RegisterInterfaceInGlobal(
		(IApplication*)this, IID_IApplication, &dwCookie);
}

bool CApplication::GetSafeInstance(IApplication **typedRet)
{
	/*HRESULT hr = */pGIT->GetInterfaceFromGlobal(
		dwCookie,
		IID_IApplication,
		(void**)typedRet);
	return true;
}

void CApplication::ExitSafeInstance()
{
	pGIT->RevokeInterfaceFromGlobal(dwCookie);
}

CApplication::~CApplication()
{
	SaveRelease(m_desklets);
	SaveRelease(m_dlls);
	SaveRelease(m_showCase);
}

std::wstring CApplication::GetAveDeskThemeDir()const
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path,GetAveDeskUserPath().c_str() );
	std::wstring folderName = _Policies.GetStringValue(L"paths/themesFolderName", L"Themes");
	PathAppend(path,folderName.c_str());

	if(!PathFileExistsW(path))
		CreateDirectoryW(path,NULL);

	return path;
}


void CApplication::SetAveDeskDataDir(const std::wstring& dataDir)
{
	m_DataDir = dataDir;
}

std::wstring CApplication::GetAveDeskDataDir()const
{
	if(m_DataDir.length() == 0)
	{
		WCHAR path[MAX_PATH] = {0};
		GetDLLPathW(NULL, path, MAX_PATH);
	
		std::wstring dir = _Policies.GetStringValue(std::wstring(L"paths//dataDir"), std::wstring(path));
		if(dir.length() == 0)
			dir = path;

		return dir;
	}

	return m_DataDir;
}

std::wstring CApplication::GetAveDeskUserPath()const
{

	WCHAR path[MAX_PATH+1] = {0};
	wcscpy(path, GetAveDeskDataDir().c_str());


	if( _globalSettings.IsNoMultipleUserAwareness() )
	{
		wcscpy(path, GetAveDeskDataDir().c_str());
		PathAppendW(path,L"Settings");
		if(!PathFileExistsW(path))
			CreateDirectoryW(path,NULL);

	}
	// moved out the CSIDL_FLAG_CREATE, so it doesn't auto-create when NoMultipleUserAwareness is active.
	else if(SUCCEEDED(SHGetFolderPathW(NULL,CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,path)))
	{
		std::wstring folderName = _Policies.GetStringValue(L"paths//settingsFolderName", L"AveDesk");
		if(folderName.length() == 0) folderName = L"AveDesk";

		PathAppendW(path,folderName.c_str());
		if(!PathFileExistsW(path))
			CreateDirectoryW(path,NULL);
	}
	
	return path;
}

void CApplication::SetThemeFileName(const std::wstring& path)
{
	// post: the themeFile has been set to v. V can be a full or a relative path.
	_globalSettings.SetThemeFileName(path);
}

std::wstring CApplication::GetThemeFileName()const
{
	// post: the current ,full path, to the theme file has been stored

	std::wstring theme = _Policies.GetStringValue(L"files//themeFileName", _globalSettings.GetThemeFileName());

	if( !PathFileExistsW(theme.c_str()) )
	{
		WCHAR path[MAX_PATH+1] = {0};
		// BUGFIX:
		wcscpy(path,GetAveDeskThemeDir().c_str());
		PathAppendW(path,theme.c_str());
			
		theme = path;
	}
	
	return theme;
}

STDMETHODIMP CApplication::Close()
{
	if( ::IsWindow(::GetConsoleWindow()) )
	{
		_Dbg->HideConsole();
	}
	if( _Dlg.IsWindow() )
		_Dlg.DestroyWindow();
	::PostQuitMessage(0);
	return S_OK;
}

STDMETHODIMP CApplication::ShowControlPanel()
{
	
	if(0 == m_MainThreadId)
		_Dlg.ShowOrActivate();
	else
	{
		if( _Dlg.IsWindow() )
			_Dlg.ShowOrActivate();
		else
			PostThreadMessage(m_MainThreadId,WM_AVEACTION_SHOWCP,0,0);
	}

	return S_OK;
}


template<typename M, typename P> inline void get_Prop(M*& m, P **pVal)
{
	if( NULL == m )
	{
		M::CreateInstance(&m);
		m->AddRef();
	}
	if(m)
		m->QueryInterface(pVal);
}
void CApplication::SetMainThreadId(const DWORD& id)
{
	m_MainThreadId = id;
}

DWORD CApplication::GetMainThreadID()const
{
	return m_MainThreadId;
}

STDMETHODIMP CApplication::get_AvailableDesklets(IDeskletInfos **pVal)
{
	get_Prop(m_dlls,pVal);
	return S_OK;
}

STDMETHODIMP CApplication::get_RunningDesklets(IDesklets **pVal)
{
	get_Prop(m_desklets,pVal);
	return S_OK;
}

STDMETHODIMP CApplication::LoadImage(BSTR FileName, IAveImage **pVal)
{
	CComObject<CImage> *img = NULL;
	CComObject<CImage>::CreateInstance(&img);
	if( img )
	{
		img->AddRef();
		VARIANT_BOOL bSucc = VARIANT_FALSE;
		img->LoadImage(FileName, &bSucc);
		if( bSucc )
			img->QueryInterface(pVal);
		else
		{
			*pVal = NULL;
			img->Release();
		}
	}
	return S_OK;
}


STDMETHODIMP CApplication::Sleep(int ms)
{
	::Sleep(ms);
	return S_OK;
}

UINT extern _themePreviewSizePercentage;

STDMETHODIMP CApplication::SaveTheme(BSTR Name)
{
	CTrayIconAnimation trayAnimation(m_IconWindow._ncd, TRUE);

	HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
	HCURSOR oldCursor = SetCursor(hCursor);

	DWORD startTime = GetTickCount();
	USES_CONVERSION;

	boost::shared_ptr<Bitmap> previewBitmap;
	boost::shared_ptr<Graphics> previewGraphics;

	if(_globalSettings.GetAddPreviewInThemeFiles())
	{
		RECT r = {0};
		::GetWindowRect(::GetDesktopWindow(),&r);
		SIZE s = SIZEFROMRECT(r);
		previewBitmap = boost::shared_ptr<Bitmap>(new Bitmap(s.cx, s.cy));
		previewGraphics = boost::shared_ptr<Graphics>(new Graphics(previewBitmap.get()));
	}

	CXMLSaver xmlSaver(L"AveDesk");

	xmlSaver.AddNode(L"theme");

	// save version information
	xmlSaver.AddNode(L"information");
	std::wstringstream ws;

	// number of desklets
	ws << Docklet::Docklets.size();
	xmlSaver.SaveString(ws.str(),L"NumberOfDesklet");
	ws.str(L"");

	// username
	WCHAR userName[0x1024] = {0};
	DWORD bufSize(0x1024);
	GetUserNameW(userName, &bufSize);
	xmlSaver.SaveString(userName, L"Author");

	// avedesk version
	long version(0);
	get_Version(&version);
	ws << version;
	xmlSaver.SaveString(ws.str(), L"AveDeskVersion");
	ws.str();

	xmlSaver.GoToParentNode();

	for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
	{
		Docklet* d=  iter->second;
		if(d->GetIsVisible() || d->GetIsShowCased())
		{
			if(previewGraphics.get() != NULL)
			{
				SIZE dockletSize = d->GetDocklet()->GetSize();
				POINT dockletPos = d->GetDocklet()->GetPos();
				d->GetDocklet()->DrawCachedBitmap(*previewGraphics,dockletSize, dockletPos);
			
				if(d->GetLabelType() == LABEL_REGULAR || (d->GetLabelOnlyOnMouseOver() && d->GetMouseIsOver()))
				{
					Bitmap* bmp2 = d->GetLabel().GetCachedBitmap();
					if(bmp2 != NULL)
						previewGraphics->DrawImage(bmp2,Rect(d->GetLabelPos().x,d->GetLabelPos().y,bmp2->GetWidth(),bmp2->GetHeight()),0,0,bmp2->GetWidth(),bmp2->GetHeight(),UnitPixel,NULL,0,0);
				}
			}
		}

		xmlSaver.AddNode(L"desklet");
		
		xmlSaver.SaveString(iter->second->GetRelativeDLLFile(),L"DLL");
		d->SaveAsync(xmlSaver);

		xmlSaver.GoToParentNode();
	}
	xmlSaver.GoToParentNode();

	std::wstring wsName = static_cast<WCHAR*>(Name);

	if( wsName.find(L":") == std::wstring::npos )
	{
		WCHAR path[MAX_PATH+1] = {0};
		wcscpy(path,GetAveDeskThemeDir().c_str());
		PathAppendW(path,wsName.c_str());
		wsName = path;
	}
	

	//xmlSaver.Save(wsName.c_str());

	if(_globalSettings.GetAddPreviewInThemeFiles() && previewBitmap.get() != NULL)
	{
		DeleteFile(wsName.c_str());
		try
		{
			HRESULT hRes = S_OK;
			CComPtr<IStorage> storage;
			hRes = StgCreateStorageEx(wsName.c_str(),	STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_TRANSACTED,
						STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, (void**)&storage);

			if(SUCCEEDED(hRes) && storage.p != NULL)
			{
				CComPtr<IStream> themeStream;
				if(FAILED(hRes = storage->CreateStream(L"Theme", STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, NULL, &themeStream)))
					throw hRes;

				if(NULL == themeStream.p)
					throw (hRes = E_FAIL);

				if(FAILED(hRes = xmlSaver.SaveToStream(themeStream)))
					throw hRes;

				CComPtr<IStream> previewStream;
				if(FAILED(hRes = storage->CreateStream(L"Preview", STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, NULL, &previewStream)))
					throw hRes;

				if(NULL == previewStream.p)
					throw (hRes = E_FAIL);

				CLSID encoderCLSID = {0};
				GetEncoderClsid(L"image/png",&encoderCLSID);

				boost::shared_ptr<Image> previewImage( 
					previewBitmap->GetThumbnailImage( 
						(_themePreviewSizePercentage * previewBitmap->GetWidth() ) / 100,
						(_themePreviewSizePercentage * previewBitmap->GetHeight() ) / 100,
						NULL, NULL)
					);
				if(previewImage.get() != NULL)
					previewImage->Save(previewStream,&encoderCLSID, NULL);

				if(FAILED(hRes = storage->Commit(STGC_DEFAULT)))
					throw hRes;
				
			}
		}
		catch(HRESULT&)
		{
			xmlSaver.Save(wsName.c_str());
		}
	}
	else
	{
		xmlSaver.Save(wsName.c_str());
	}
	
	SetCursor(oldCursor);
	// FIX: only call DestroyCursor(hCursor); when  the cursor is created with CreateCursor()


	return S_OK;
}

STDMETHODIMP CApplication::LoadTheme(BSTR Name)
{
	// TODO: FIX

	USES_CONVERSION;

	CTrayIconAnimation trayAnim(m_IconWindow._ncd, TRUE);

	std::wstring wsName = static_cast<WCHAR*>(Name);

	if( !PathFileExistsW(wsName.c_str()) )
	{
		WCHAR path[MAX_PATH+1] = {0};
		wcscpy(path,GetAveDeskThemeDir().c_str());
		PathAppendW(path,wsName.c_str());	
		wsName = path;

		
		if(!PathFileExistsW(wsName.c_str()) )
		{
			WCHAR path[MAX_PATH+1] = {0};
			wcscpy(path,GetAveDeskDataDir().c_str());
			PathAppendW(path,(WCHAR*)Name);	
			wsName = path;
		}

		if(!PathFileExistsW(wsName.c_str()) )
		{
			return Error(L"Theme File not found");	
		}

	}
	
	// we have a liftoff

	// drop old desklets - maybe further excerpts from EndApp
	Docklet::FadeAllOut();
	Docklet::ReleaseAll();

	// load theme - maybe further excerpts from StartApp
 
	boost::shared_ptr<CXMLLoader> xmlLoader;

	if(StgIsStorageFile(wsName.c_str()) == S_OK) // explicit check for S_OK
	{
		HRESULT hRes = S_OK;
		CComPtr<IStorage> storage;
		hRes = StgOpenStorageEx(wsName.c_str(), STGM_READ | STGM_DIRECT | STGM_SHARE_DENY_WRITE, STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, reinterpret_cast<void**>(&storage));
		if(SUCCEEDED(hRes) && storage.p != NULL)
		{
			CComPtr<IStream> stream;
			hRes = storage->OpenStream(L"Theme", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, &stream);
			if(SUCCEEDED(hRes) && stream.p != NULL)
				xmlLoader = boost::shared_ptr<CXMLLoader>(new CXMLLoader(stream));
		}
	}
	else
	{
		xmlLoader = boost::shared_ptr<CXMLLoader>(new CXMLLoader(wsName.c_str()));
	}

	if(xmlLoader.get() != NULL)
	{
		try
		{
			xmlLoader->SelectChildNode(L"AveDesk/theme");
			xmlLoader->SelectNodes(L"desklet");

			std::vector<HANDLE> loadEvents;

			long deskletsCount = xmlLoader->NumberOfNodesInList();
			numInitialDocklets = (int)deskletsCount;
			for(int i=0;i<deskletsCount;++i)
			{
				xmlLoader->SelectNodeFromList(i);

				std::wstring dockletDLLFile;
				xmlLoader->LoadString(dockletDLLFile,L"DLL");

				WCHAR path[MAX_PATH];
				wcscpy(path, _App->GetAveDeskDataDir().c_str());
				wcscat(path,dockletDLLFile.c_str());

				Docklet* d = Docklet::LoadDocklet(path,xmlLoader.get(),NULL,NULL,true);
				if(!d)
				{
					CString msg;
					msg.LoadString(IDS_ERRORLOADINGDOCKLET);

					TCHAR errMsg[1024+MAX_PATH+2];;
					DWORD_PTR args[1] = { (DWORD_PTR)(path) };

					FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,msg,0,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),errMsg,
						sizeof(errMsg)/sizeof(errMsg[0]),(va_list*)args);

					MessageBox(0,errMsg,0,MB_ICONERROR);
				}
				else
				{
					loadEvents.push_back(d->GetIsInitializedEventHandle() );
				}
			}

			if(loadEvents.size() > 0)
				WaitForMultipleObjects(loadEvents.size(),&loadEvents[0],TRUE,4000);

			trayAnim.Stop();
			Docklet::FadeAllLoadedIn();



		}
		catch(HRESULT&)
		{
			return Error(L"Theme File not valid");
		}
	}

	return S_OK;
}

STDMETHODIMP CApplication::get_ShowCase(IShowCase **pVal)
{
	if(NULL == pVal)
		return E_POINTER;

	if(_ShowCase)
		_ShowCase->QueryInterface(pVal);

	return S_OK;
}

STDMETHODIMP CApplication::get_Version(long *pVal)
{
	if(NULL == pVal)
		return E_POINTER;

	*pVal = 200;

	return S_OK;
}

STDMETHODIMP CApplication::OpenFile(BSTR FileName)
{
	if(FileName != NULL)
	{
		WCHAR* path = new WCHAR[SysStringLen(FileName) + 1];
		wcscpy(path, static_cast<WCHAR*>(FileName));
		PathUnquoteSpaces(path);
		WCHAR* ext = PathFindExtensionW(path);

		if(_wcsicmp(ext,L".zip") == 0 || _wcsicmp(ext,L".aveinst") == 0)
			CAveInstaller::InstallAsync(path, NULL, FALSE);
		else if(_wcsicmp(ext,L".avedesklet") == 0)
			Docklet::LoadFromSingleFile(path,NULL,TRUE);
		else if(_wcsicmp(ext,L".avetheme") == 0)
			_App->LoadTheme(CComBSTR(path));

		delete[] path;
	}

	return S_OK;
}

STDMETHODIMP CApplication::get_ExePath(BSTR *pVal)
{
	if( NULL == pVal )
		return E_POINTER;
	
	WCHAR ts[1024];
	DWORD dwRes = GetModuleFileName(NULL, ts, 1024);
	*pVal = ::SysAllocStringLen(ts, dwRes);
	return S_OK;
}

STDMETHODIMP CApplication::get_ThemePath(BSTR *pVal)
{
	if( NULL == pVal )
		return E_POINTER;
	
	std::wstring ws = GetAveDeskThemeDir();
	*pVal = ::SysAllocString(ws.c_str());

	return S_OK;
}

STDMETHODIMP CApplication::get_UserPath(BSTR *pVal)
{
	if( NULL == pVal )
		return E_POINTER;
	
	std::wstring ws = GetAveDeskUserPath();
	*pVal = ::SysAllocString(ws.c_str());

	return S_OK;
}

STDMETHODIMP CApplication::get_Debug(IDebugHelperClient **pVal)
{
	if( _Dbg )
		_Dbg->QueryInterface(pVal);

	return S_OK;
}

STDMETHODIMP CApplication::ShowThemeBrowserDialog()
{
	CThemeBrowserDlg themeBrowser;
	themeBrowser.DoModal();

	return S_OK;
}

STDMETHODIMP CApplication::SaveGlobalSettings()
{
	CXMLSaver xmlSaver(L"AveDesk");

	xmlSaver.AddNode(L"GlobalSettings");
	_globalSettings.Save(xmlSaver);

	xmlSaver.GoToParentNode();

	if(_Policies.GetBoolValue(L"saving//saveDefaultDeskletSettings", TRUE))
	{
		if(Docklet::defaultDocklet != NULL)
		{
			xmlSaver.AddNode(L"DefaultDeskletSettings");
			Docklet::defaultDocklet->Save(xmlSaver);
			xmlSaver.GoToParentNode();
		}
	}
	
	if(_Policies.GetBoolValue(L"saving//saveModules", TRUE))
	{
		xmlSaver.AddNode(L"Modules");

		WCHAR adPath[MAX_PATH+1] = {0};
		wcscpy(adPath, GetAveDeskDataDir().c_str());
		lowercaseW(adPath);

		std::vector<CModule*> modules = _Modules->GetCurrentModules();
		for(std::vector<CModule*>::iterator iter = modules.begin(); iter != modules.end(); ++iter)
		{
			CModule* m = *iter;
			xmlSaver.AddNode(L"Module");

			WCHAR path[MAX_PATH] = {0};
			WCHAR* pathPtr = path;
			GetModuleFileNameW(m->GetHMODULE(),path,MAX_PATH);

			lowercaseW(path);

			if(wcsstr(path, adPath) != NULL)
			{
				pathPtr = (path + wcslen(adPath));	
			}

			xmlSaver.SaveString(pathPtr,L"Path");

			xmlSaver.GoToParentNode();
		}

		xmlSaver.GoToParentNode();
	}


	WCHAR settingsPath[MAX_PATH+1] = {0};
	wcscpy(settingsPath,_App->GetAveDeskUserPath().c_str());
	PathAppend(settingsPath,L"settings.xml");
	xmlSaver.Save(settingsPath);


	return S_OK;
}

bool CApplication::LoadGlobalSettings()
{
	//CheckAndDoSettingsOverrides();

	WCHAR themePath[MAX_PATH+1] = {0};
	wcscpy(themePath,_App->GetAveDeskUserPath().c_str());
	PathAppendW(themePath,L"settings.xml");
	if(!PathFileExists(themePath))
	{
		WCHAR oldThemePath[MAX_PATH] = {0};
		wcscpy(oldThemePath, GetAveDeskDataDir().c_str());
		PathAppendW(oldThemePath,L"theme.ini");

		if(PathFileExists(oldThemePath))
		{
			CThemeIniToXMLMapper mapper;
			mapper.MapSettings(oldThemePath,themePath);


			WCHAR convertedThemePath[MAX_PATH] = {0};
			wcscpy(convertedThemePath,_App->GetAveDeskThemeDir().c_str());
			PathAppendW(convertedThemePath,L"AveDesk1.1.avetheme");
			mapper.MapOldThemeToNewTheme(oldThemePath,convertedThemePath);
			_globalSettings.SetThemeFileName(convertedThemePath);
			
			WCHAR backupPath[MAX_PATH] = {0};
			wcscpy(backupPath, GetAveDeskDataDir().c_str());
			PathAppendW(backupPath,L"theme.ini.bak");

			if(CopyFile(oldThemePath,backupPath,FALSE))
				DeleteFile(oldThemePath);

			_App->SaveGlobalSettings();

			try
			{
				CXMLLoader xmlLoader(themePath,L"AveDesk");
				xmlLoader.Reset();
				if( SUCCEEDED(xmlLoader.SelectChildNode(L"AveDesk")) && SUCCEEDED(xmlLoader.SelectChildNode(L"DefaultDeskletSettings") ))
				{
					Docklet::defaultDocklet->Load(xmlLoader);
				}
			}
			catch(HRESULT&)
			{
				// bleh
			}

			return true;
		}
	}

	if(!PathFileExists(themePath))
	{
		wcscpy(themePath,_App->GetAveDeskDataDir().c_str());
		PathAppendW(themePath,L"settings.xml");
	}

	try
	{	

		CXMLLoader xmlLoader(themePath,L"AveDesk");
		if( SUCCEEDED(xmlLoader.SelectChildNode(L"GlobalSettings")) )
			_globalSettings.Load(xmlLoader);
		
		xmlLoader.Reset();

		if(_Policies.GetBoolValue(L"loading//loadDefaultDeskletSettings", TRUE))
		{
			if( SUCCEEDED(xmlLoader.SelectChildNode(L"AveDesk")) && SUCCEEDED(xmlLoader.SelectChildNode(L"DefaultDeskletSettings") ))
				Docklet::defaultDocklet->Load(xmlLoader);
		}

		if(_Policies.GetBoolValue(L"loading//loadModules", TRUE))
		{
			xmlLoader.Reset();
			if( SUCCEEDED(xmlLoader.SelectChildNode(L"AveDesk")) && SUCCEEDED(xmlLoader.SelectChildNode(L"Modules")) && SUCCEEDED(xmlLoader.SelectNodes(L"Module")))
			{
				long count = xmlLoader.NumberOfNodesInList();	
				for(int i=0; i < count; ++i)
				{
					std::wstring modFileName;
					if(SUCCEEDED(xmlLoader.SelectNodeFromList(i)) && SUCCEEDED(	xmlLoader.LoadString(modFileName,L"Path") ))
					{
						_Modules->LoadModule(modFileName);
					}
				}
			}
		}

		return true;


	}
	catch(HRESULT&)
	{
		return false;	
	}
}

STDMETHODIMP CApplication::ShowDeskletBrowserDialog()
{
	CDeskletBrowserDlg dlg;
	dlg.DoModal();

	return S_OK;
}
