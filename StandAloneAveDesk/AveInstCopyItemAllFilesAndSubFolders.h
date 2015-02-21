// AveInstCopyItemAllFilesAndSubFolders.h: interface for the CAveInstCopyItemAllFilesAndSubFolders class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTCOPYITEMALLFILESANDSUBFOLDERS_H__C5CEF63D_6EDA_403E_AD84_9E4744229B4D__INCLUDED_)
#define AFX_AVEINSTCOPYITEMALLFILESANDSUBFOLDERS_H__C5CEF63D_6EDA_403E_AD84_9E4744229B4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstCopyItem.h"

class CAveInstCopyItemAllFilesAndSubFolders  :
	public CAveInstCopyItem
{
protected:
	bool m_Recursive;
public:
	CAveInstCopyItemAllFilesAndSubFolders();
	virtual ~CAveInstCopyItemAllFilesAndSubFolders();

	virtual HRESULT LoadFromNode(XMLNode& node);

	virtual std::wstring GetNewInputPath(CAveInstallerEnvironment& env, const std::wstring& inputPath);
	virtual std::wstring GetNewOutputPath(CAveInstallerEnvironment& env, const std::wstring& outputPath);
	virtual bool PerformOperation(CAveInstFileDirOperation& op, CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath);

};

#endif // !defined(AFX_AVEINSTCOPYITEMALLFILESANDSUBFOLDERS_H__C5CEF63D_6EDA_403E_AD84_9E4744229B4D__INCLUDED_)
