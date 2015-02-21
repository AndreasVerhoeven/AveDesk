// AveInstRequiredInstallerVersion.cpp: implementation of the CAveInstRequiredInstallerVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRequiredInstallerVersion.h"
#include "AveInstallerEnvironment.h"
#include <sstream>
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequiredInstallerVersion::CAveInstRequiredInstallerVersion() : 
	m_MajorVersion(0),
	m_MinorVersion(0)
{

}

CAveInstRequiredInstallerVersion::~CAveInstRequiredInstallerVersion()
{

}

bool CAveInstRequiredInstallerVersion::DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error)
{
	bool versionIsHigher = (m_MajorVersion < env.GetMajorVersion())  || (m_MajorVersion == env.GetMajorVersion() &&	m_MinorVersion <= env.GetMinorVersion());
	if(versionIsHigher)
	{
		return true;
	}
	else
	{
		std::wstringstream wsCur;
		wsCur << env.GetMajorVersion() << L"." << env.GetMinorVersion();
		std::wstringstream wsWanted;
		wsWanted << m_MajorVersion << L"." << m_MinorVersion;
		CString str;
		str.LoadString(IDS_INSTALLERVERSIONERROR);
		str.Replace(_T("$1"),wsCur.str().c_str());
		str.Replace(_T("$2"),wsWanted.str().c_str());
		error.SetMessage( str.operator LPCTSTR() );
		error.SetHomePage(L"http://www.avedesk.com/AveDeskSetup.exe");

		return false;
	}
}

HRESULT CAveInstRequiredInstallerVersion::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	if( NULL == node.p)
		return (hRes = E_FAIL);

	CComBSTR bstrText;
	if(FAILED( hRes = node->get_text(&bstrText)))
		return hRes;

	std::wstringstream ws;
	if(bstrText != NULL)
		ws << static_cast<WCHAR*>(bstrText);

	ws >> m_MajorVersion;
	ws.ignore(1);
	ws >> m_MinorVersion;

	return S_OK;
}