// AveInstCopyInformation.cpp: implementation of the CAveInstCopyInformation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstCopyInformation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstCopyInformation::CAveInstCopyInformation()
{

}

CAveInstCopyInformation::~CAveInstCopyInformation()
{

}

bool CAveInstCopyInformation::Copy(CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath)
{
	return PerformOperation(*this,env,inputPath,outputPath);
}

bool CAveInstCopyInformation::FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	BOOL bRes = CopyFile(srcPath.c_str(),dstPath.c_str(),FALSE);
	// TODO: what if copy fails?

	return (bRes == TRUE);	
}

bool CAveInstCopyInformation::DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	if(!PathFileExists(dstPath.c_str()))
	{
		return (CreateDirectory(dstPath.c_str(),NULL) == TRUE);
	}
	else
	{
		return true;
	}
}