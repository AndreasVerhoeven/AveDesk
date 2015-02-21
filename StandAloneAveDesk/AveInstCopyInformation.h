// AveInstCopyInformation.h: interface for the CAveInstCopyInformation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTCOPYINFORMATION_H__50E03065_50E9_48BD_BE68_5D97A5941609__INCLUDED_)
#define AFX_AVEINSTCOPYINFORMATION_H__50E03065_50E9_48BD_BE68_5D97A5941609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

class CAveInstCopyInformation : public CAveInstCopyItemDirectory,
	public CAveInstFileDirOperation
{
public:
	CAveInstCopyInformation();
	virtual ~CAveInstCopyInformation();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual bool Copy(CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath);
};

#endif // !defined(AFX_AVEINSTCOPYINFORMATION_H__50E03065_50E9_48BD_BE68_5D97A5941609__INCLUDED_)
