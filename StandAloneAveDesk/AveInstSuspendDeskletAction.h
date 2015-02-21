// AveInstSuspendDeskletAction.h: interface for the CAveInstSuspendDeskletAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTSUSPENDDESKLETACTION_H__3C3AF74F_E0DC_424B_AD6C_2F96FFA9D589__INCLUDED_)
#define AFX_AVEINSTSUSPENDDESKLETACTION_H__3C3AF74F_E0DC_424B_AD6C_2F96FFA9D589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

class CAveInstSuspendDeskletAction :
	public CAveInstAction,
	public CAveInstFileDirOperation
{

protected:

	CAveInstCopyItemDirectory m_Dir;
	bool m_UseAlias;
	std::wstring m_FileName;

	void UnloadDeskletByFileName(CAveInstallerEnvironment& env, const std::wstring& fname);


public:
	CAveInstSuspendDeskletAction();
	virtual ~CAveInstSuspendDeskletAction();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTSUSPENDDESKLETACTION_H__3C3AF74F_E0DC_424B_AD6C_2F96FFA9D589__INCLUDED_)
