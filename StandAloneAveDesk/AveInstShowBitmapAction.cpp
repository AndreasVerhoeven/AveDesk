// AveInstShowBitmapAction.cpp: implementation of the CAveInstShowBitmapAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstShowBitmapAction.h"
#include "AveInstallerEnvironment.h"

#include "AveInstBitmapWindow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstShowBitmapAction::CAveInstShowBitmapAction()
{

}

CAveInstShowBitmapAction::~CAveInstShowBitmapAction()
{

}

HRESULT CAveInstShowBitmapAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if( NULL == node.p)
		return (hRes = E_FAIL);

	CComBSTR bstrText;
	if(FAILED( hRes = node->get_text(&bstrText)))
		return hRes;

	if(bstrText != NULL)
		m_BitmapFileName = static_cast<WCHAR*>(bstrText);


	return S_OK;
}

HRESULT CAveInstShowBitmapAction::DoAction(CAveInstallerEnvironment& env)
{
	// showing a bitmap is not silent!
	if(!env.IsSilentInstall())
	{
		WCHAR path[MAX_PATH] = {0};
		wcscpy(path,env.GetTempPath().c_str() );
		PathAppendW(path,m_BitmapFileName.c_str() );

		CAveInstBitmapWindow::CreateBitmapWindow(path);
	}

	return S_OK;	
}