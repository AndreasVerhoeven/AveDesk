// AveInstaller.h: interface for the CAveInstaller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTALLER_H__46A8B667_E217_438C_8B0F_7CB340F8FC36__INCLUDED_)
#define AFX_AVEINSTALLER_H__46A8B667_E217_438C_8B0F_7CB340F8FC36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory>
#include <string>

class CAveInstRequirements;
class CAveInstActions;
class CAveInstCopyInformation;
class CAveInstallerEnvironment;
class CAveInstProgressIndicator;
class CAveInstSuspendedDeskletsList;
class CAveInstAbout;
class CAveInstDependencies;
class CAveInstPackageManager;

class CAveInstaller  
{

	friend class CAveInstPackageManager;

public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

	struct InstallerThreadInfo
	{
		public:
			std::wstring fileName;
			HWND hwnd;
			BOOL silently;
			CAveInstProgressIndicator* indicator;
			HANDLE threadHandle;
	};

protected:

	CComPtr<IXMLDOMDocument> m_Doc;
	XMLNode m_RootNode;
	
	int m_VersionMajor;
	int m_VersionMinor;

	boost::shared_ptr<CAveInstRequirements>		m_Requirements;
	boost::shared_ptr<CAveInstActions>			m_BeforeCopy;
	boost::shared_ptr<CAveInstCopyInformation>	m_CopyInformation;
	boost::shared_ptr<CAveInstActions>			m_AfterCopy;
	boost::shared_ptr<CAveInstActions>			m_AfterCopyFailed;
	boost::shared_ptr<CAveInstAbout>			m_About;
	boost::shared_ptr<CAveInstDependencies>		m_Dependencies;
	CAveInstSuspendedDeskletsList*			m_SuspendedDeskletsList;


	HRESULT LoadRequirements(XMLNode& rootNode);
	HRESULT LoadBeforeCopy(XMLNode& rootNode);
	HRESULT LoadCopyInformation(XMLNode& rootNode);
	HRESULT LoadAfterCopy(XMLNode rootNode);
	HRESULT LoadAfterCopyFailed(XMLNode rootNode);
	HRESULT LoadAbout(XMLNode rootNode);
	HRESULT LoadDependencies(XMLNode rootNode);


	std::wstring GetTempFolder();
	CAveInstallerEnvironment GetEnvironment(HWND hwnd, BOOL silentEnvironment);
	BOOL Unzip(CAveInstallerEnvironment& env, const WCHAR* fileName);
	std::wstring GetInstallerDataFilePath(CAveInstallerEnvironment& env);
	bool CheckRequirements(CAveInstallerEnvironment& env);

public:
	//static const int INSTALLER_VERSION_MAJOR = 1;
	//static const int INSTALLER_VERSION_MINOR = 2;

	static DWORD __stdcall InstallerThread(LPVOID lpData);

	CAveInstaller();
	virtual ~CAveInstaller();

	virtual void SetSuspendedDeskletsList(CAveInstSuspendedDeskletsList* sdl);

	virtual HRESULT LoadFromFile(const WCHAR* fileName);
	virtual HRESULT Install(const WCHAR* fileName, HWND hwnd, BOOL silently, CAveInstProgressIndicator* indicator=NULL);
	static void InstallAsync(const WCHAR* fileName, HWND hwndParent, BOOL silently);

};

#endif // !defined(AFX_AVEINSTALLER_H__46A8B667_E217_438C_8B0F_7CB340F8FC36__INCLUDED_)
