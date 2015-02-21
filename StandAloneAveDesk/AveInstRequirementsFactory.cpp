// AveInstRequirementsFactory.cpp: implementation of the CAveInstRequirementsFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRequirementsFactory.h"

#include "AveInstRequiredAveDeskVersion.h"
#include "AveInstRequiredInstallerVersion.h"
#include "AveInstRequiredFile.h"
#include "AveInstNotRequired.h"
#include "AveInstRequiredToAcceptText.h"
#include "AveInstDeskletUsedRequirement.h"
#include "AveInstRequiredPackage.h"
#include "AveInstRequiredDeskletVersion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequirementsFactory::CAveInstRequirementsFactory()
{

}

CAveInstRequirementsFactory::~CAveInstRequirementsFactory()
{

}

HRESULT CAveInstRequirementsFactory::GetTypeFromNode(XMLNode node,std::wstring& type)
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


#define CHECKREQUIREDITEM(T,name)						\
																\
	if(_wcsicmp(type.c_str(),name) == 0)							\
	{															\
		boost::shared_ptr<CAveInstReqItem> ptr(new T);				\
		if(FAILED(hRes = ptr->LoadFromNode(node)))				\
			return hRes;										\
																\
		reqItem = ptr;											\
																\
		return (hRes = S_OK);									\
																\
	}

HRESULT CAveInstRequirementsFactory::CreateRequirementItem(XMLNode node, boost::shared_ptr<CAveInstReqItem>& reqItem)
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


	// NOTE: ADD HERE YOUR REQUIREMENTS!
	CHECKREQUIREDITEM(CAveInstRequiredAveDeskVersion, L"RequiredAveDeskVersion");
	CHECKREQUIREDITEM(CAveInstRequiredInstallerVersion, L"RequiredInstallerVersion");
	CHECKREQUIREDITEM(CAveInstRequiredFile, L"FileExists");
	CHECKREQUIREDITEM(CAveInstNotRequired, L"Not");
	CHECKREQUIREDITEM(CAveInstRequiredToAcceptText, L"AcceptText");
	CHECKREQUIREDITEM(CAveInstDeskletUsedRequirement, L"DeskletIsUsed");
	CHECKREQUIREDITEM(CAveInstRequiredPackage,L"PackageIsInstalled");
	CHECKREQUIREDITEM(CAveInstRequiredDeskletVersion,L"RequiredDeskletVersion");

	return (hRes = E_FAIL);
}
