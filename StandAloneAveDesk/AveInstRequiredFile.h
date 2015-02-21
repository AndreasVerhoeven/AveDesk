// AveInstRequiredFile.h: interface for the CAveInstRequiredFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREDFILE_H__14EDC3ED_A386_4094_B553_C52B888FD3AA__INCLUDED_)
#define AFX_AVEINSTREQUIREDFILE_H__14EDC3ED_A386_4094_B553_C52B888FD3AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstReqItem.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

class CAveInstRequiredFile : 
	public CAveInstReqItem,
	public CAveInstFileDirOperation
{
protected:
	CAveInstCopyItemDirectory m_Dir;
public:
	CAveInstRequiredFile();
	virtual ~CAveInstRequiredFile();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error);
	virtual HRESULT LoadFromNode(XMLNode node);

};

#endif // !defined(AFX_AVEINSTREQUIREDFILE_H__14EDC3ED_A386_4094_B553_C52B888FD3AA__INCLUDED_)
