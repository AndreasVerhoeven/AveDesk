// AveInstRequiredToAcceptText.cpp: implementation of the CAveInstRequiredToAcceptText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstRequiredToAcceptText.h"
#include "AveInstallerEnvironment.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstRequiredToAcceptText::CAveInstRequiredToAcceptText()
{

}

CAveInstRequiredToAcceptText::~CAveInstRequiredToAcceptText()
{

}

bool CAveInstRequiredToAcceptText::DoesMeetRequirement(CAveInstallerEnvironment& env, CAveInstError& error)
{
	CString installerCaption;
	installerCaption.LoadString(IDS_INSTALLERCAPTION);

	INT res = MessageBoxW(env.GetHWND(),m_Text.c_str(),installerCaption,MB_ICONQUESTION | MB_YESNO);
	if(res == IDYES)
	{
		return true;
	}
	else
	{
		error = m_Error;
		return false;
	}
}

HRESULT CAveInstRequiredToAcceptText::LoadFromNode(XMLNode node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_INVALIDARG);

	hRes = CAveInstReqItem::LoadFromNode(node);

	XMLNode textNode;
	CComBSTR bstrNode(L"Text");
	if(FAILED(hRes = node->selectSingleNode(bstrNode,&textNode)))
		return hRes;

	if(NULL == textNode.p)
		return (hRes = E_FAIL);

	CComBSTR bstrText;
	if(FAILED(hRes = textNode->get_text(&bstrText)))
		return hRes;

	if(bstrText != NULL)
		m_Text = static_cast<WCHAR*>(bstrText);
	else
		m_Text = L"";

	return (hRes = S_OK);
}
