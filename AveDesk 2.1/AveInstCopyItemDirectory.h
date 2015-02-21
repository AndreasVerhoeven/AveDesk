// AveInstCopyItemDirectory.h: interface for the CAveInstCopyItemDirectory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTCOPYITEMDIRECTORY_H__7F7E6004_6E62_48AC_AF20_A4470D06BE86__INCLUDED_)
#define AFX_AVEINSTCOPYITEMDIRECTORY_H__7F7E6004_6E62_48AC_AF20_A4470D06BE86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstCopyItem.h"

#include <list>
#include <memory>
#include <string>

class CAveInstCopyItemDirectory : public CAveInstCopyItem  
{
protected:
	std::list<boost::shared_ptr<CAveInstCopyItem> > m_Items;
	std::wstring m_BaseFolder;
	std::wstring m_DirName;

public:
	CAveInstCopyItemDirectory();
	virtual ~CAveInstCopyItemDirectory();

	virtual HRESULT LoadFromNode(XMLNode& node);

	virtual std::wstring GetNewInputPath(CAveInstallerEnvironment& env, const std::wstring& inputPath);
	virtual std::wstring GetNewOutputPath(CAveInstallerEnvironment& env, const std::wstring& outputPath);
	virtual bool PerformOperation(CAveInstFileDirOperation& op, CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath);
};

#endif // !defined(AFX_AVEINSTCOPYITEMDIRECTORY_H__7F7E6004_6E62_48AC_AF20_A4470D06BE86__INCLUDED_)
