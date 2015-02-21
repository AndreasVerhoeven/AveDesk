// AveInstUnloadModuleAction.h: interface for the CAveInstUnloadModuleAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTUNLOADMODULEACTION_H__3CC5BBC9_DB3C_4DF4_9EC7_DF0B50C8A078__INCLUDED_)
#define AFX_AVEINSTUNLOADMODULEACTION_H__3CC5BBC9_DB3C_4DF4_9EC7_DF0B50C8A078__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

#include <string>

class CAveInstUnloadModuleAction :
	public CAveInstAction,
	public CAveInstFileDirOperation
{

protected:
	CAveInstCopyItemDirectory m_Dir;
	bool m_UseAlias;
	std::wstring m_FileName;

	void UnloadModuleByFileName(const std::wstring& fname);

public:
	CAveInstUnloadModuleAction();
	virtual ~CAveInstUnloadModuleAction();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTUNLOADMODULEACTION_H__3CC5BBC9_DB3C_4DF4_9EC7_DF0B50C8A078__INCLUDED_)
