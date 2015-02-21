// AveInstFontAction.h: interface for the CAveInstFontAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTFONTACTION_H__FF4FA518_F1B3_49EB_ABCB_73C1F5396B34__INCLUDED_)
#define AFX_AVEINSTFONTACTION_H__FF4FA518_F1B3_49EB_ABCB_73C1F5396B34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstFileDirOperation.h"

class CAveInstFontAction :
	public CAveInstAction,
	public CAveInstFileDirOperation
{
protected:
	CAveInstCopyItemDirectory m_Dir;
public:
	CAveInstFontAction();
	virtual ~CAveInstFontAction();

	virtual bool FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);
	virtual bool DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath);

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTFONTACTION_H__FF4FA518_F1B3_49EB_ABCB_73C1F5396B34__INCLUDED_)
