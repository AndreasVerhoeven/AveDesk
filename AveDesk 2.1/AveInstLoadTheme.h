// AveInstLoadTheme.h: interface for the CAveInstLoadTheme class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTLOADTHEME_H__9FE70FEE_24C3_4730_9BC4_6C07BEF3C583__INCLUDED_)
#define AFX_AVEINSTLOADTHEME_H__9FE70FEE_24C3_4730_9BC4_6C07BEF3C583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"


class CAveInstLoadTheme :
	public CAveInstAction,
	public CAveInstFileDirOperation
{
protected:
	CAveInstCopyItemDirectory m_Dir;
	bool m_UseAlias;
	std::wstring m_FileName;
public:
	CAveInstLoadTheme();
	virtual ~CAveInstLoadTheme();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);

};

#endif // !defined(AFX_AVEINSTLOADTHEME_H__9FE70FEE_24C3_4730_9BC4_6C07BEF3C583__INCLUDED_)
