// AveInstPackageManager.cpp: implementation of the CAveInstPackageManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstPackageManager.h"
#include "AveInstaller.h"
#include "AveInstAbout.h"
#include "helper_funcs.h"

#include "Application.h"
extern CComObject<CApplication>* _App;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRITICAL_SECTION CAveInstPackageManager::cs = {0};

CAveInstPackageManager::CAveInstPackageManager()
{

}

CAveInstPackageManager::~CAveInstPackageManager()
{

}

HRESULT CAveInstPackageManager::AddPackageFromInstaller(CAveInstaller& installer)
{
	EnterCriticalSection(&cs);

	HRESULT hRes = S_OK;

	if(NULL == installer.m_About.get() || installer.m_About->GetGUID().empty())
	{
		LeaveCriticalSection(&cs);
		return (hRes = E_FAIL);
	}

	XMLNode node;
	std::wstring nodePath = L"InstalledPackages/" + installer.m_About->GetGUID();
	CComBSTR bstrPackage( nodePath.c_str() );

	CComPtr<IXMLDOMDocument> packDoc;

	if(FAILED( hRes = packDoc.CoCreateInstance(CLSID_DOMDocument)))
	{
		LeaveCriticalSection(&cs);
		return hRes;
	}

	packDoc->put_preserveWhiteSpace(VARIANT_TRUE);


	WCHAR packPath[MAX_PATH] = {0};
	wcscpy(packPath, _App->GetAveDeskDataDir().c_str());

	PathAppendW(packPath,L"Data\\packages.xml");

	CComBSTR bstrFileName( packPath );
	CComVariant varFileName( bstrFileName );
	varFileName.vt = VT_BSTR;
	VARIANT_BOOL vbSuccess = { VARIANT_FALSE };
	if( SUCCEEDED( hRes = packDoc->load(varFileName,&vbSuccess)) && VARIANT_FALSE != vbSuccess)
	{
		CComBSTR bstrPackagesStr(L"Packages");
		XMLNode packagesNode;
		if(SUCCEEDED(hRes = packDoc->selectSingleNode(bstrPackagesStr,&packagesNode)) && packagesNode.p != NULL)
		{
			std::wstring xpath = L"//*[@GUID='" + installer.m_About->GetGUID() + L"']";
			CComBSTR bstrXPath(xpath.c_str());
	
			hRes = packDoc->selectSingleNode(bstrXPath,&node);
			if(node.p != NULL)
			{
				LeaveCriticalSection(&cs);
				return S_OK;
			}
		}
	}

	// fetch about and copyinformation for this package
	CComBSTR bstrAbout(L"AveInstaller/About");
	XMLNode aboutNode;
	hRes = installer.m_Doc->selectSingleNode(bstrAbout,&aboutNode);

	CComBSTR bstrCopyInfo(L"AveInstaller/CopyInformation");
	XMLNode copyInfoNode;
	hRes = installer.m_Doc->selectSingleNode(bstrCopyInfo,&copyInfoNode);

	CComBSTR bstrDep(L"AveInstaller/Dependencies");
	XMLNode depNode;
	hRes = installer.m_Doc->selectSingleNode(bstrDep,&depNode);

	XMLNode retNode;

	XMLNode packagesNode;
	CComBSTR bstrPackages(L"Packages");
	if(FAILED(hRes = packDoc->selectSingleNode(bstrPackages,&packagesNode)) || NULL == packagesNode.p)
	{
		CComPtr<IXMLDOMElement> packagesElement;
		if( FAILED(hRes = packDoc->createElement(bstrPackages,&packagesElement)) || NULL == packagesElement.p)
		{
			LeaveCriticalSection(&cs);
			return hRes;
		}

		if(FAILED(hRes = packDoc->appendChild(packagesElement,&packagesNode)) || NULL == packagesNode.p)
		{
			LeaveCriticalSection(&cs);
			return hRes;
		}
	}

	CComBSTR bstrGUID(L"Package");
	CComPtr<IXMLDOMElement> element;
	if( FAILED(hRes = packDoc->createElement(bstrGUID,&element)) || NULL == element.p  )
	{
		LeaveCriticalSection(&cs);
		return hRes;
	}

	CComVariant varT( installer.m_About->GetGUID().c_str() );
	CComBSTR bstrValueStr(L"GUID");

	if(FAILED( hRes = element->setAttribute(bstrValueStr,varT)) )
	{
		LeaveCriticalSection(&cs);
		return hRes;
	}

	if(FAILED(hRes = packagesNode->appendChild(element,&retNode)) || NULL == retNode.p )
	{
		LeaveCriticalSection(&cs);
		return hRes;
	}


	if(aboutNode.p != NULL)
	{
		XMLNode dummyNode;
		if(FAILED(hRes = retNode->appendChild(aboutNode,&dummyNode)) || NULL == dummyNode.p)
		{
			LeaveCriticalSection(&cs);
			return hRes;
		}
	}

	if(copyInfoNode.p != NULL)
	{
		XMLNode dummyNode;
		if(FAILED(hRes = retNode->appendChild(copyInfoNode,&dummyNode)) || NULL == dummyNode.p)
		{
			LeaveCriticalSection(&cs);
			return hRes;
		}
	}

	if(depNode.p != NULL)
	{
		XMLNode dummyNode;
		if(FAILED(hRes = retNode->appendChild(depNode,&dummyNode)) || NULL == dummyNode.p)
		{
			LeaveCriticalSection(&cs);
			return hRes;
		}
	}


	CComVariant outputFileName(bstrFileName);
	hRes = packDoc->save(outputFileName);

	LeaveCriticalSection(&cs);

	return hRes;
}

bool CAveInstPackageManager::IsPackageInstalled(const std::wstring& guid)
{
	EnterCriticalSection(&cs);

	HRESULT hRes = S_OK;

	if(guid.empty())
	{
		LeaveCriticalSection(&cs);
		return false;
	}

	CComPtr<IXMLDOMDocument> packDoc;

	if(FAILED( hRes = packDoc.CoCreateInstance(CLSID_DOMDocument)))
	{
		LeaveCriticalSection(&cs);
		return false;
	}

	packDoc->put_preserveWhiteSpace(VARIANT_TRUE);


	WCHAR packPath[MAX_PATH] = {0};
	wcscpy(packPath, _App->GetAveDeskDataDir().c_str());
	PathAppendW(packPath,L"Data\\packages.xml");

	CComBSTR bstrFileName( packPath );
	CComVariant varFileName( bstrFileName );
	varFileName.vt = VT_BSTR;
	VARIANT_BOOL vbSuccess = { VARIANT_FALSE };
	if( SUCCEEDED( hRes = packDoc->load(varFileName,&vbSuccess)) && VARIANT_FALSE != vbSuccess)
	{
		CComBSTR bstrPackagesStr(L"Packages");
		XMLNode packagesNode;
		if(SUCCEEDED(hRes = packDoc->selectSingleNode(bstrPackagesStr,&packagesNode)) && packagesNode.p != NULL)
		{
			std::wstring xpath = L"//*[@GUID='" + guid + L"']";
			CComBSTR bstrXPath(xpath.c_str());
	
			XMLNode node;
			hRes = packDoc->selectSingleNode(bstrXPath,&node);
			if(FAILED(hRes) || node.p != NULL)
			{
				LeaveCriticalSection(&cs);
				return true;
			}
		}
	}

	LeaveCriticalSection(&cs);

	return false;
}