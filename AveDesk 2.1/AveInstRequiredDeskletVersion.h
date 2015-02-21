// AveInstRequiredDeskletVersion.h: interface for the CAveInstRequiredDeskletVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTREQUIREDDESKLETVERSION_H__A67ED93B_E63A_4258_ACB4_C943593796C6__INCLUDED_)
#define AFX_AVEINSTREQUIREDDESKLETVERSION_H__A67ED93B_E63A_4258_ACB4_C943593796C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstReqItem.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

class CAveInstRequiredDeskletVersion :
	public CAveInstReqItem,
	public CAveInstFileDirOperation
{
protected:
	CAveInstCopyItemDirectory m_Dir;
	int m_MajorVersion;
	int m_MinorVersion;
public:
	CAveInstRequiredDeskletVersion();
	virtual ~CAveInstRequiredDeskletVersion();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual bool DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error);
	virtual HRESULT LoadFromNode(XMLNode node);
};

#endif // !defined(AFX_AVEINSTREQUIREDDESKLETVERSION_H__A67ED93B_E63A_4258_ACB4_C943593796C6__INCLUDED_)
