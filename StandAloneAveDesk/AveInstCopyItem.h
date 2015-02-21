// AveInstCopyItem.h: interface for the CAveInstCopyItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTCOPYITEM_H__7C5EC729_39B9_4DB1_AC95_6317803BB121__INCLUDED_)
#define AFX_AVEINSTCOPYITEM_H__7C5EC729_39B9_4DB1_AC95_6317803BB121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CAveInstallerEnvironment;
class CAveInstFileDirOperation;


class CAveInstCopyItem  
{
protected:
	std::wstring m_Alias;

	virtual bool CopyIntern(CAveInstallerEnvironment& env, const std::wstring& source, const std::wstring dest);
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

	CAveInstCopyItem();
	virtual ~CAveInstCopyItem();

	virtual HRESULT LoadFromNode(XMLNode& node);

	virtual std::wstring GetNewInputPath(CAveInstallerEnvironment& env, const std::wstring& inputPath) = 0;
	virtual std::wstring GetNewOutputPath(CAveInstallerEnvironment& env, const std::wstring& outputPath) = 0;
	virtual bool PerformOperation(CAveInstFileDirOperation& op, CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath) = 0;

};

#endif // !defined(AFX_AVEINSTCOPYITEM_H__7C5EC729_39B9_4DB1_AC95_6317803BB121__INCLUDED_)
