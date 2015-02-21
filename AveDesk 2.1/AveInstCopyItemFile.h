// AveInstCopyItemFile.h: interface for the CAveInstCopyItemFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTCOPYITEMFILE_H__9A19DF2E_D37A_4E3B_A984_041DF0E17477__INCLUDED_)
#define AFX_AVEINSTCOPYITEMFILE_H__9A19DF2E_D37A_4E3B_A984_041DF0E17477__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstCopyItem.h"
#include <string>

class CAveInstCopyItemFile : public CAveInstCopyItem  
{
protected:
	std::wstring m_FileName;

public:
	typedef CComPtr<IXMLDOMNode> XMLNode;


	CAveInstCopyItemFile();
	virtual ~CAveInstCopyItemFile();

	virtual HRESULT LoadFromNode(XMLNode& node);

	virtual std::wstring GetNewInputPath(CAveInstallerEnvironment& env, const std::wstring& inputPath);
	virtual std::wstring GetNewOutputPath(CAveInstallerEnvironment& env, const std::wstring& outputPath);
	virtual bool PerformOperation(CAveInstFileDirOperation& op, CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath);
};

#endif // !defined(AFX_AVEINSTCOPYITEMFILE_H__9A19DF2E_D37A_4E3B_A984_041DF0E17477__INCLUDED_)
