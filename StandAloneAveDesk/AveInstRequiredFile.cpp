// AveInstRequiredFile.cpp: implementation of the CAveInstRequiredFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRequiredFile.h"
#include "AveInstallerEnvironment.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequiredFile::CAveInstRequiredFile()
{

}

CAveInstRequiredFile::~CAveInstRequiredFile()
{

}

bool CAveInstRequiredFile::FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
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
			str.Format(IDS_FILEDOESNOTEXIST,dstPath.c_str());
			m_Error.SetMessage(str.operator LPCTSTR() );
		}

		return false;
	}
	
}

bool CAveInstRequiredFile::DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
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

bool CAveInstRequiredFile::DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error)
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

HRESULT CAveInstRequiredFile::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	hRes = CAveInstReqItem::LoadFromNode(node);

	return (hRes = m_Dir.LoadFromNode(node));
}