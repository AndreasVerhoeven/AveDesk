// AveInstRequiredAveDeskVersion.cpp: implementation of the CAveInstRequiredAveDeskVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRequiredAveDeskVersion.h"
#include <sstream>
#include "AveInstallerEnvironment.h"
#include "AveInstError.h"
#include  "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequiredAveDeskVersion::CAveInstRequiredAveDeskVersion() :
	m_MajorVersion(0),
	m_MinorVersion(0)
{

}

CAveInstRequiredAveDeskVersion::~CAveInstRequiredAveDeskVersion()
{

}

bool CAveInstRequiredAveDeskVersion::DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error)
{
	CAveEnvironment aveEnv = env.GetAveDeskEnvironment();
	bool versionIsHigher = (aveEnv.GetMajorVersion() > m_MajorVersion) || 
			(m_MajorVersion == aveEnv.GetMajorVersion() &&	m_MinorVersion <= aveEnv.GetMinorVersion()) ;
		
	if(versionIsHigher)
	{
		return true;
	}
	else
	{

		std::wstringstream wsCur;
		wsCur << aveEnv.GetMajorVersion() << L"." << aveEnv.GetMinorVersion();
		std::wstringstream wsWanted;
		wsWanted << m_MajorVersion << L"." << m_MinorVersion;
		CString str;
		str.LoadString(IDS_AVEDESKVERSIONERROR);
		str.Replace(_T("$1"),wsCur.str().c_str());
		str.Replace(_T("$2"),wsWanted.str().c_str());
		error.SetMessage( str.operator LPCTSTR() );
		error.SetHomePage(L"http://www.avedesk.com/AveDeskSetup.exe");

		return false;
	}
}

HRESULT CAveInstRequiredAveDeskVersion::LoadFromNode(XMLNode node)
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