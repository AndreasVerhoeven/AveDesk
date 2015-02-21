// AveInstSuspendedDesklets.cpp: implementation of the CAveInstSuspendedDesklets class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstSuspendedDesklets.h"
#include "Docklet.h"
#include "XMLSaver.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstSuspendedDesklets::CAveInstSuspendedDesklets() :
	m_Count(0)
{

}

CAveInstSuspendedDesklets::~CAveInstSuspendedDesklets()
{
	if(0 == m_Count)
		ResumeDesklets();
		
}

long CAveInstSuspendedDesklets::AddCount()
{
	return ++m_Count;
}

void CAveInstSuspendedDesklets::ResumeDesklets()
{
	for(std::list<XMLNodeWrapper>::iterator iter = m_Desklets.begin(); iter != m_Desklets.end(); ++iter)
	{
		XMLNodeWrapper& nodeWrapper = *iter;
		Docklet::LoadFromXMLNode(nodeWrapper.node,NULL,TRUE);
	}
	m_Desklets.clear();
}

long CAveInstSuspendedDesklets::ReleaseCount()
{
	--m_Count;
	if(m_Count <= 0)
	{
		ResumeDesklets();
		m_Count = 0;
	}

	return m_Count;
}

HRESULT CAveInstSuspendedDesklets::Add(Docklet* d)
{
	HRESULT hRes = S_OK;
	if(NULL == d)
		return (hRes = E_FAIL);

	CXMLSaver saver(L"Desklet");
	saver.SaveString(d->GetRelativeDLLFile(),L"DLL");

	if(FAILED( hRes = d->SaveAsync(saver)))
		return hRes;

	d->Remove();
	CComPtr<IXMLDOMDocument> doc(saver.GetDOMDocument());

	XMLNode node;
	doc.QueryInterface(&node);
	if(node.p != NULL)
		m_Desklets.push_back(XMLNodeWrapper(node));

	return (hRes = S_OK);
}

int CAveInstSuspendedDesklets::GetNumberOfSuspendedDesklets()const
{
	return m_Desklets.size();
}