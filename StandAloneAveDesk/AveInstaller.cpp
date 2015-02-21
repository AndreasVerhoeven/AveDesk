// AveInstaller.cpp: implementation of the CAveInstaller class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstaller.h"

#include "AveInstRequirements.h"
#include "AveInstActions.h"
#include "AveInstCopyInformation.h"
#include "AveInstallerEnvironment.h"
#include "AveInstError.h"
#include "AveInstProgressWindow.h"
#include "AveInstSuspendedDeskletsList.h"
#include "AveInstAbout.h"
#include "AveInstDependencies.h"
#include "Unzip/Unzipper.h"
#include "helper_funcs.h"
#include "AveInstPackageManager.h"
#include "InstallErrorDlg.h"
#include "Application.h"
extern CComObject<CApplication> *_App;

#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstaller::CAveInstaller() :
	m_SuspendedDeskletsList(NULL)
{

}

CAveInstaller::~CAveInstaller()
{
	if(m_SuspendedDeskletsList)
		m_SuspendedDeskletsList->Release();
}

void CAveInstaller::SetSuspendedDeskletsList(CAveInstSuspendedDeskletsList* sdl)
{
	if(m_SuspendedDeskletsList)
		m_SuspendedDeskletsList->AddRef();

	m_SuspendedDeskletsList = sdl;
}

HRESULT CAveInstaller::LoadFromFile(const WCHAR* fileName)
{
	HRESULT hRes = S_OK;

	if(FAILED( hRes = m_Doc.CoCreateInstance(CLSID_DOMDocument)))
		return hRes;

	//m_Doc->put_preserveWhiteSpace(VARIANT_TRUE);


	CComBSTR bstrFileName( fileName );
	CComVariant varFileName( bstrFileName );
	varFileName.vt = VT_BSTR;
	VARIANT_BOOL vbSuccess = { VARIANT_FALSE };
	if( FAILED( hRes = m_Doc->load(varFileName,&vbSuccess)) || VARIANT_FALSE == vbSuccess)
		return hRes;

	CComBSTR bstrXPath(L"AveInstaller");
	if(FAILED( hRes = m_Doc->selectSingleNode(bstrXPath,&m_RootNode)) || NULL == m_RootNode.p)
		return hRes;

	hRes = LoadDependencies(m_RootNode);
	hRes = LoadAbout(m_RootNode);
	hRes = LoadRequirements(m_RootNode);
	hRes = LoadBeforeCopy(m_RootNode);
	hRes = LoadCopyInformation(m_RootNode);
	hRes = LoadAfterCopy(m_RootNode);

	return S_OK;
}

template<typename T> HRESULT LoadChildNode(CAveInstaller::XMLNode& rootNode,
	const WCHAR* rootName,
	boost::shared_ptr<T>& resAutoPtr)
{
	HRESULT hRes = S_OK;

	CComBSTR bstrXPath( rootName );
	CAveInstaller::XMLNode node;

	if(FAILED(hRes = rootNode->selectSingleNode(bstrXPath,&node)))
		return hRes;

	if(node.p)
	{
		boost::shared_ptr<T> t( new T );
		if(FAILED(hRes = t->LoadFromNode(node)))
			return hRes;

		resAutoPtr = t;
	}

	return S_OK;
}

HRESULT CAveInstaller::LoadDependencies(XMLNode rootNode)
{
	return LoadChildNode<CAveInstDependencies>(rootNode,L"Dependencies",m_Dependencies);
}

HRESULT CAveInstaller::LoadAbout(XMLNode rootNode)
{
	return LoadChildNode<CAveInstAbout>(rootNode,L"About",m_About);
}


HRESULT CAveInstaller::LoadRequirements(XMLNode& rootNode)
{
	return LoadChildNode<CAveInstRequirements>(rootNode,L"Requirements",m_Requirements);
}


HRESULT CAveInstaller::LoadBeforeCopy(XMLNode& rootNode)
{
	return LoadChildNode<CAveInstActions>(rootNode,L"BeforeCopy",m_BeforeCopy);
}


HRESULT CAveInstaller::LoadCopyInformation(XMLNode& rootNode)
{
	return LoadChildNode<CAveInstCopyInformation>(rootNode,L"CopyInformation",m_CopyInformation);
}


HRESULT CAveInstaller::LoadAfterCopy(XMLNode rootNode)
{
	HRESULT hRes =  LoadChildNode(rootNode,L"AfterCopySucceeded",m_AfterCopy);

	// backward compatibility with my own test-files... bleeeeeeh... gotta remove this
	// for the final release, but I guess I will forget.
	if(FAILED(hRes) || NULL == m_AfterCopy.get())
		hRes = LoadChildNode(rootNode,L"AfterCopy",m_AfterCopy);

	return hRes;
}

HRESULT CAveInstaller::LoadAfterCopyFailed(XMLNode rootNode)
{
	return LoadChildNode(rootNode,L"AfterCopyFailed",m_AfterCopyFailed);
}

std::wstring CAveInstaller::GetTempFolder()
{
	WCHAR tmpPath[MAX_PATH] = {0};
	GetTempPathW(MAX_PATH,tmpPath);

	std::wstringstream ws;
	ws << this << GetTickCount();

	PathAppendW(tmpPath,ws.str().c_str());
	CreateDirectoryW(tmpPath,NULL);

	return tmpPath;
}

extern bool _alwaysSilentInstall;

CAveInstallerEnvironment CAveInstaller::GetEnvironment(HWND hwnd, BOOL silentEnvironment)
{

	long version(0);
	if(_App != NULL)
		_App->get_Version(&version);

	CAveInstallerEnvironment env;
	env.SetVersion(version / 100 , (version % 100 ) / 10);
	env.SetHWND(hwnd);
	env.SetTempPath(GetTempFolder());

	if(!_alwaysSilentInstall)
		env.SetIsSilentIstall(silentEnvironment != FALSE);
	else
		env.SetIsSilentIstall(true);

	if(NULL == m_SuspendedDeskletsList)
		m_SuspendedDeskletsList = new CAveInstSuspendedDeskletsList;

	env.SetSuspendedDeskletsList(m_SuspendedDeskletsList);

	return env;
}

BOOL CAveInstaller::Unzip(CAveInstallerEnvironment& env, const WCHAR* fileName)
{
	USES_CONVERSION;

	CUnzipper unzipper(W2A(fileName));
	return unzipper.UnzipTo(W2A(env.GetTempPath().c_str()),FALSE);
}

std::wstring CAveInstaller::GetInstallerDataFilePath(CAveInstallerEnvironment& env)
{
	WCHAR installerDataFilePath[MAX_PATH] = {0};
	wcscpy(installerDataFilePath,env.GetTempPath().c_str());
	PathAppendW(installerDataFilePath,L"avedesk_installer.xml");

	return installerDataFilePath;
}

bool CAveInstaller::CheckRequirements(CAveInstallerEnvironment& env)
{
	bool meetsRequirements = true;

	CAveInstError error;
	if(m_Requirements.get())
		meetsRequirements = m_Requirements->DoesMeetRequirements(env,error);

	if(false == meetsRequirements)
	{
		if(!env.IsSilentInstall())
		{
			CInstallErrorDlg dlg(error);
			dlg.DoModal();
		}
	}

	return meetsRequirements;
}


DWORD CAveInstaller::InstallerThread(LPVOID lpData)
{
	InstallerThreadInfo* info = reinterpret_cast<InstallerThreadInfo*>(lpData);
	if(info)
	{
		CoInitialize(NULL);
		OleInitialize(NULL);
		CAveInstaller installer;
		HRESULT hRes = installer.Install(info->fileName.c_str(),info->hwnd,info->silently,info->indicator);
		delete info->indicator;

		HANDLE h = info->threadHandle;
		delete info;
		if(h)
			CloseHandle(h);

		CoUninitialize();
		OleUninitialize();
	}

	ExitThread(0);

	return 0;
}


void CAveInstaller::InstallAsync(const WCHAR* fileName, HWND hwndParent, BOOL silently)
{
	if(NULL == fileName)
		return;

	CAveInstProgressWindow* progressWindow = new CAveInstProgressWindow;
	progressWindow->SetupWindow(hwndParent);

	InstallerThreadInfo* info = new InstallerThreadInfo;
	info->fileName = fileName;
	info->hwnd = hwndParent;
	info->silently = silently;
	info->indicator = progressWindow;
	DWORD dummyId(0);
	info->threadHandle = CreateThread(NULL,0,InstallerThread,reinterpret_cast<LPVOID>(info),0,&dummyId);
}


HRESULT CAveInstaller::Install(const WCHAR* fileName, HWND hwnd, BOOL silently, CAveInstProgressIndicator* indicator)
{
	HRESULT hRes = S_OK;

	CAveInstallerEnvironment env = GetEnvironment(hwnd, silently);
	env.SetProgressIndicator(indicator);

	if(!Unzip(env,fileName))
	{
		return (hRes = E_FAIL);
	}

	std::wstring installerDataFilePath = GetInstallerDataFilePath(env);
	if(!PathFileExistsW(installerDataFilePath.c_str()))
	{

		env.ReportErrorByID(IDS_NOINSTALLERFILE);
		return (hRes = E_FAIL);
	}

	if(FAILED(hRes = LoadFromFile(installerDataFilePath.c_str())))
	{
		env.ReportErrorByID(IDS_INSTALLERFILEINVALID);
		return hRes;
	}

	if(CheckRequirements(env))
	{
		if(m_BeforeCopy.get())
			hRes = m_BeforeCopy->DoActions(env);

		bool copiedFiles = true;
		if(m_CopyInformation.get())
			copiedFiles = m_CopyInformation->Copy(env,env.GetTempPath(),env.GetAveDeskEnvironment().GetInstallPath() );

		if(env.GetInstallerProgressIndicator())
			env.GetInstallerProgressIndicator()->Hide();

		// before actions
		if(m_SuspendedDeskletsList)
			m_SuspendedDeskletsList->ReduceCountForAll();

		if(copiedFiles)
		{
			if(m_AfterCopy.get())
				hRes = m_AfterCopy->DoActions(env);

			
			// register this package in packages.xml
			HRESULT tmpHRes = CAveInstPackageManager::AddPackageFromInstaller(*this);
		}
		else
		{
			if(m_AfterCopyFailed.get())
				hRes = m_AfterCopyFailed->DoActions(env);
		}
	}
	else
	{
		env.ReportErrorByID(IDS_INSTALLERREQUIREMENTSERROR);
	}
	
	
	// clean-up time!
	m_Doc.Release();
	DeleteDirectory(env.GetTempPath());

	return hRes;

}
