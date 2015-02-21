// AveInstDeskletUsedRequirement.h: interface for the CAveInstDeskletUsedRequirement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTDESKLETUSEDREQUIREMENT_H__D83F787A_1690_47A9_B024_207C9B1AB8E0__INCLUDED_)
#define AFX_AVEINSTDESKLETUSEDREQUIREMENT_H__D83F787A_1690_47A9_B024_207C9B1AB8E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstReqItem.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

class CAveInstDeskletUsedRequirement : 
	public CAveInstReqItem,
	public CAveInstFileDirOperation
{
protected:
	CAveInstCopyItemDirectory m_Dir;
public:
	CAveInstDeskletUsedRequirement();
	virtual ~CAveInstDeskletUsedRequirement();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error);
	virtual HRESULT LoadFromNode(XMLNode node);
};

#endif // !defined(AFX_AVEINSTDESKLETUSEDREQUIREMENT_H__D83F787A_1690_47A9_B024_207C9B1AB8E0__INCLUDED_)
