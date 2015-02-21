// AveInstRunScriptAction.cpp: implementation of the CAveInstRunScriptAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRunScriptAction.h"
#include "AveInstallerEnvironment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRunScriptAction::CAveInstRunScriptAction()
{

}

CAveInstRunScriptAction::~CAveInstRunScriptAction()
{

}


HRESULT CAveInstRunScriptAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if( NULL == node.p)
		return (hRes = E_FAIL);

	CComBSTR bstrText;
	if(FAILED( hRes = node->get_text(&bstrText)))
		return hRes;

	if(bstrText != NULL)
		m_ScriptFileName = static_cast<WCHAR*>(bstrText);


	return S_OK;
}

HRESULT CAveInstRunScriptAction::DoAction(CAveInstallerEnvironment& env)
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path,env.GetTempPath().c_str() );
	PathAppendW(path,m_ScriptFileName.c_str() );

	// TODO: make it run the script itself actually, not just launch it!
	ShellExecuteW(NULL,L"open",path,NULL,NULL,SW_SHOW);

	return S_OK;	
}