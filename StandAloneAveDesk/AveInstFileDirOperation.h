// AveInstFileDirOperation.h: interface for the CAveInstFileDirOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTFILEDIROPERATION_H__7AE0AE03_8EF6_41ED_B371_D91AFB356687__INCLUDED_)
#define AFX_AVEINSTFILEDIROPERATION_H__7AE0AE03_8EF6_41ED_B371_D91AFB356687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CAveInstallerEnvironment;

class CAveInstFileDirOperation  
{
public:
	CAveInstFileDirOperation();
	virtual ~CAveInstFileDirOperation();
	
	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath) = 0;
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath) = 0;
};

#endif // !defined(AFX_AVEINSTFILEDIROPERATION_H__7AE0AE03_8EF6_41ED_B371_D91AFB356687__INCLUDED_)
