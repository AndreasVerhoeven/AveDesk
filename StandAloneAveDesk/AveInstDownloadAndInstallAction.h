// AveInstDownloadAndInstallAction.h: interface for the CAveInstDownloadAndInstallAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTDOWNLOADANDINSTALLACTION_H__727A5EA5_CA45_4092_BAA6_954011F18130__INCLUDED_)
#define AFX_AVEINSTDOWNLOADANDINSTALLACTION_H__727A5EA5_CA45_4092_BAA6_954011F18130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"
#include "DownloadableFileDescription.h"

class CAveInstDownloadAndInstallAction : public CAveInstAction  
{
protected:
	CDownloadableFileDescription m_FileDescr;
public:
	CAveInstDownloadAndInstallAction();
	virtual ~CAveInstDownloadAndInstallAction();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTDOWNLOADANDINSTALLACTION_H__727A5EA5_CA45_4092_BAA6_954011F18130__INCLUDED_)
