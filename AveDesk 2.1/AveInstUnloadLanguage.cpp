// AveInstUnloadLanguage.cpp: implementation of the CAveInstUnloadLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstUnloadLanguage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstUnloadLanguage::CAveInstUnloadLanguage()
{

}

CAveInstUnloadLanguage::~CAveInstUnloadLanguage()
{

}

HRESULT CAveInstUnloadLanguage::LoadFromNode(XMLNode& node)
{
	return S_OK;
}

HRESULT CAveInstUnloadLanguage::DoAction(CAveInstallerEnvironment& env)
{
	_Module.ChangeResourceInstance(_T(""));
	return S_OK;
}