// AveInstDeskletUsedRequirement.cpp: implementation of the CAveInstDeskletUsedRequirement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstDeskletUsedRequirement.h"
#include "AveInstallerEnvironment.h"
#include "AveInstError.h"
#include "AveInstSuspendedDeskletsList.h"
#include "Docklet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstDeskletUsedRequirement::CAveInstDeskletUsedRequirement()
{

}

CAveInstDeskletUsedRequirement::~CAveInstDeskletUsedRequirement()
{

}

bool CAveInstDeskletUsedRequirement::FileOperation(CAveInstallerEnvironment& env,
												const std::wstring& srcPath,
												const std::wstring& dstPath)
{
	if(PathFileExists(dstPath.c_str()))
	{
		if(env.GetSuspendedDeskletsList() != NULL && 
			env.GetSuspendedDeskletsList()->GetSuspendedDesklets(dstPath).GetNumberOfSuspendedDesklets() != 0)
				return true;

		bool found=false;
		EnterCriticalSection(&Docklet::cs);
		Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin();
		while(iter != Docklet::Docklets.end() && !found)
		{
			Docklet* d = iter->second;

			WCHAR path[MAX_PATH] = {0};
			GetModuleFileName(d->GetDocklet()->GetModule(),path,MAX_PATH);

			found = (_wcsicmp(path, dstPath.c_str()) == 0);

			++iter;
		};
		LeaveCriticalSection(&Docklet::cs);

		return found;
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
bool CAveInstDeskletUsedRequirement::DirectoryOperation(CAveInstallerEnvironment& env,
														const std::wstring& srcPath,
														const std::wstring& dstPath)
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
			str.Format(IDS_DIRECTORYDOESNOTEXIST,dstPath.c_str());
			m_Error.SetMessage(str.operator LPCTSTR() );
		}

		return false;
	}
}

bool CAveInstDeskletUsedRequirement::DoesMeetRequirement(CAveInstallerEnvironment& env,
														 CAveInstError& error)
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

HRESULT CAveInstDeskletUsedRequirement::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	hRes = CAveInstReqItem::LoadFromNode(node);

	hRes = m_Dir.LoadFromNode(node);

	return hRes;
}