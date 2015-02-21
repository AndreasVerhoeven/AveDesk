// AveInstActionsFactory.cpp: implementation of the CAveInstActionsFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstActionsFactory.h"
#include "AveInstAction.h"

#include "AveInstRunScriptAction.h"
#include "AveInstShowBitmapAction.h"
#include "AveInstLoadDeskletAction.h"
#include "AveInstRegisterCOMServerAction.h"
#include "AveInstShowTextAction.h"
#include "AveInstInstallModuleAction.h"
#include "AveInstUnloadModuleAction.h"
#include "AveInstDownloadAndInstallAction.h"
#include "AveInstSuspendDeskletAction.h"
#include "AveInstDoIfAction.h"
#include "AveInstFontAction.h"
#include "AveInstReloadLanguage.h"
#include "AveInstUnloadLanguage.h"
#include "AveInstLoadTheme.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstActionsFactory::CAveInstActionsFactory()
{

}

CAveInstActionsFactory::~CAveInstActionsFactory()
{

}

HRESULT CAveInstActionsFactory::GetTypeFromNode(XMLNode node,std::wstring& type)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	CComPtr<IXMLDOMNamedNodeMap> nodeMap;
	if(FAILED(hRes = node->get_attributes(&nodeMap)))
		return hRes;

	if(NULL == nodeMap.p)
		return (hRes = E_FAIL);


	CComBSTR bstrName( L"Type");
	XMLNode namedNode;
	if(FAILED(hRes = nodeMap->getNamedItem(bstrName,&namedNode)))
		return hRes;

	if(NULL == namedNode.p)
		return (hRes = E_FAIL);

	CComBSTR bstrText;
	if(FAILED( hRes = namedNode->get_text(&bstrText)))
		return hRes;

	if(bstrText != NULL)
		type = static_cast<WCHAR*>(bstrText);

	return (hRes = S_OK);
}


#define CHECKACTIONITEM(T,name)									\
																\
	if(_wcsicmp(type.c_str(),name) == 0)							\
	{															\
		boost::shared_ptr<CAveInstAction> ptr(new T);				\
		if(FAILED(hRes = ptr->LoadFromNode(node)))				\
			return hRes;										\
																\
		actItem = ptr;											\
																\
		return (hRes = S_OK);									\
																\
	}

HRESULT CAveInstActionsFactory::CreateActionItem(XMLNode node, boost::shared_ptr<CAveInstAction>& actItem)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	std::wstring type;
	if(FAILED(hRes = GetTypeFromNode(node,type)))
		return hRes;

	// lowercase type
	for(std::wstring::iterator iter = type.begin(); iter != type.end(); ++iter)
		*iter = static_cast<wchar_t>(tolower(*iter));	

	// NOTE: ADD HERE YOUR ACTIONS!
	CHECKACTIONITEM(CAveInstRunScriptAction,L"RunScript");
	CHECKACTIONITEM(CAveInstShowBitmapAction,L"ShowBitmap");
	CHECKACTIONITEM(CAveInstLoadDeskletAction,L"LoadDesklet");
	CHECKACTIONITEM(CAveInstRegisterCOMServerAction,L"RegisterCOMServer");
	CHECKACTIONITEM(CAveInstShowTextAction,L"ShowMessage");
	CHECKACTIONITEM(CAveInstInstallModuleAction,L"InstallModule");
	CHECKACTIONITEM(CAveInstUnloadModuleAction,L"UnloadModule");
	CHECKACTIONITEM(CAveInstDownloadAndInstallAction,L"DownloadAndInstall");
	CHECKACTIONITEM(CAveInstSuspendDeskletAction,L"SuspendDesklet");
	CHECKACTIONITEM(CAveInstDoIfAction,L"DoIf");
	CHECKACTIONITEM(CAveInstFontAction,L"InstallFont");
	CHECKACTIONITEM(CAveInstReloadLanguage,L"ReloadLanguage");
	CHECKACTIONITEM(CAveInstUnloadLanguage,L"UnloadLanguage");
	CHECKACTIONITEM(CAveInstLoadTheme,L"LoadThemeFile");

	return (hRes = E_FAIL);
}