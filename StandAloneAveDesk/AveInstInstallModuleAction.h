// AveInstInstallModuleAction.h: interface for the CAveInstInstallModuleAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTINSTALLMODULEACTION_H__B59CD275_26F7_4189_96E4_74EFB81C1DF2__INCLUDED_)
#define AFX_AVEINSTINSTALLMODULEACTION_H__B59CD275_26F7_4189_96E4_74EFB81C1DF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

class CAveInstInstallModuleAction : 
	public CAveInstAction,
	public CAveInstFileDirOperation
{
protected:
	CAveInstCopyItemDirectory m_Dir;
	bool m_UseAlias;
	std::wstring m_FileName;
public:
	CAveInstInstallModuleAction();
	virtual ~CAveInstInstallModuleAction();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTINSTALLMODULEACTION_H__B59CD275_26F7_4189_96E4_74EFB81C1DF2__INCLUDED_)
