// AveInstReloadLanguage.cpp: implementation of the CAveInstReloadLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstReloadLanguage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstReloadLanguage::CAveInstReloadLanguage()
{

}

CAveInstReloadLanguage::~CAveInstReloadLanguage()
{

}

HRESULT CAveInstReloadLanguage::LoadFromNode(XMLNode& node)
{
	return S_OK;
}


extern void LoadLanguage();
HRESULT CAveInstReloadLanguage::DoAction(CAveInstallerEnvironment& env)
{
	LoadLanguage();
	return S_OK;
}