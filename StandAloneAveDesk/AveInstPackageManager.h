// AveInstPackageManager.h: interface for the CAveInstPackageManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTPACKAGEMANAGER_H__835E61C2_F293_449D_87FA_14E04C08D3C2__INCLUDED_)
#define AFX_AVEINSTPACKAGEMANAGER_H__835E61C2_F293_449D_87FA_14E04C08D3C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CAveInstaller;

class CAveInstPackageManager  
{
private:
	CAveInstPackageManager();
	virtual ~CAveInstPackageManager();

public:
	static CRITICAL_SECTION cs;
	typedef CComPtr<IXMLDOMNode> XMLNode;

	static HRESULT AddPackageFromInstaller(CAveInstaller& installer);
	static bool IsPackageInstalled(const std::wstring&  guid);

};

#endif // !defined(AFX_AVEINSTPACKAGEMANAGER_H__835E61C2_F293_449D_87FA_14E04C08D3C2__INCLUDED_)
