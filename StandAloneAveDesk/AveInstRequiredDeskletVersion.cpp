// AveInstRequiredDeskletVersion.cpp: implementation of the CAveInstRequiredDeskletVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRequiredDeskletVersion.h"
#include "AveInstallerEnvironment.h"
#include "AveInstError.h"
#include "DockletDLL.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequiredDeskletVersion::CAveInstRequiredDeskletVersion() :
	m_MajorVersion(0),
	m_MinorVersion(0)
{

}

CAveInstRequiredDeskletVersion::~CAveInstRequiredDeskletVersion()
{

}

bool CAveInstRequiredDeskletVersion::FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	if(PathFileExists(dstPath.c_str()))
	{
		DockletDLL* dll = DockletDLL::LoadDockletDLL(dstPath.c_str());
		if(dll)
		{
			int major = dll->GetVersion() / 100;
			int minor = dll->GetVersion() - major * 100;

			dll->Release();
			bool versionIsHigher = (m_MajorVersion < major) || (m_MajorVersion == major && m_MinorVersion <= minor);

			if(versionIsHigher)
			{
				return true;
			}
			else
			{
		
				if(m_Error.GetMessage().empty())
				{
					std::wstringstream wsCur;
					wsCur << major << L"." << minor;
					std::wstringstream wsWanted;
					wsWanted << m_MajorVersion << L"." << m_MinorVersion;
					CString str;
					str.LoadString(IDS_DESKLETVERSIONERROR);
					str.Replace(_T("$1"),dstPath.c_str());
					str.Replace(_T("$2"),wsCur.str().c_str());
					str.Replace(_T("$3"),wsWanted.str().c_str());
					m_Error.SetMessage( str.operator LPCTSTR() );
				}

				return false;
			}
		}
		else
		{
			CString str;
			str.Format(IDS_FILEDOESNOTEXIST,dstPath.c_str());
			m_Error.SetMessage(str.operator LPCTSTR() );
		}
		

		return true;
	}
	else
	{
		if(m_Error.GetMessage().empty())
		{
			CString str;
			str.Format(IDS_FILEDOESNOTEXIST,dstPath.c_str());
			m_Error.SetMessage(str.operator LPCTSTR() );
		}

		return false;
	}
}

bool CAveInstRequiredDeskletVersion::DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	if(PathFileExists(dstPath.c_str()))
	{
		return true;
	}
	else
	{
		if(m_Error.GetMessage().empty())
		{
			CString str;
			str.Format(IDS_DIRECTORYDOESNOTEXIST,dstPath.c_str() );
			m_Error.SetMessage(str.operator LPCTSTR() );
		}

		return false;
	}
}

bool CAveInstRequiredDeskletVersion::DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error)
{
	// by copying the environment, we make the aliases in the current Node local.
	CAveInstallerEnvironment tmpEnv = env;

	if(m_Dir.PerformOperation(*this,tmpEnv,env.GetTempPath(), env.GetAveDeskEnvironment().GetInstallPath()) )
	{
		return true;
	}
	else
	{

		error = m_Error;

		return false;
	}
}

HRESULT CAveInstRequiredDeskletVersion::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	
	CComBSTR bstrVersion(L"Version");
	XMLNode versionNode;
	if(FAILED( hRes = node->selectSingleNode(bstrVersion,&versionNode)))
		return hRes;

	CComBSTR bstrText;
	if(FAILED( hRes = versionNode->get_text(&bstrText)))
		return hRes;

	std::wstringstream ws;
	if(bstrText != NULL)
		ws << static_cast<WCHAR*>(bstrText);

	ws >> m_MajorVersion;
	ws.ignore(1);
	ws >> m_MinorVersion;

	hRes = m_Dir.LoadFromNode(node);

	return hRes;
}
