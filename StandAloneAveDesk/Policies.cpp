// Policies.cpp: implementation of the CPolicies class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Policies.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline void checkRes(HRESULT hr)
{
	if(FAILED(hr))
		throw hr;
}

CPolicies::CPolicies()
{

}

CPolicies::~CPolicies()
{

}

void CPolicies::Free()
{
	m_Doc.Release();
}

bool CPolicies::AreEnabled() const
{
	return m_Doc.p != NULL;
}

HRESULT CPolicies::LoadFromStream(CComPtr<IStream> stream) 
{
	try
	{
		checkRes( m_Doc.CoCreateInstance(CLSID_DOMDocument2) );
		m_Doc->put_preserveWhiteSpace(VARIANT_TRUE);
		CComBSTR prop(L"SelectionLanguage");
		CComBSTR val(L"XPath");
		CComVariant varVal(val);
		checkRes(m_Doc->setProperty(prop, varVal));


		CComVariant varStream( stream );
		varStream.punkVal = stream;
		varStream.vt = VT_UNKNOWN;
		VARIANT_BOOL vbSuccess = { VARIANT_FALSE };

		checkRes ( m_Doc->load(varStream,&vbSuccess) );
	}
	catch(HRESULT& hr)
	{
		m_Doc.Release();
		return hr;
	}

	return S_OK;	
}

HRESULT CPolicies::LoadFromFile(const std::wstring& filename)
{
	try
	{
		checkRes( m_Doc.CoCreateInstance(CLSID_DOMDocument2) );
		m_Doc->put_preserveWhiteSpace(VARIANT_TRUE);
		CComBSTR prop(L"SelectionLanguage");
		CComBSTR val(L"XPath");
		CComVariant varVal(val);
		checkRes(m_Doc->setProperty(prop, varVal));


		CComBSTR bstrFileName( filename.c_str() );
		CComVariant varFileName( bstrFileName );
		VARIANT_BOOL vbSuccess = { VARIANT_FALSE };

		checkRes ( m_Doc->load(varFileName,&vbSuccess) );
	}
	catch(HRESULT& hr)
	{
		m_Doc.Release();
		return hr;
	}

	return S_OK;
}

HRESULT CPolicies::LoadFromString(const std::wstring& xmlStr)
{
	try
	{
		checkRes( m_Doc.CoCreateInstance(CLSID_DOMDocument) );
		m_Doc->put_preserveWhiteSpace(VARIANT_TRUE);


		CComBSTR bstrXML( xmlStr.c_str() );
		VARIANT_BOOL vbSuccess = { VARIANT_FALSE };

		checkRes ( m_Doc->loadXML(bstrXML,&vbSuccess) );
	}
	catch(HRESULT& hr)
	{
		return hr;
	}

	return S_OK;
}

void CPolicies::SetCurrentPolicyName(const std::wstring& name)
{
	m_CurrentPolicyName = name;
}

std::wstring CPolicies::GetCurrentPolicyName()const
{
	return m_CurrentPolicyName;
}

std::wstring CPolicies::GetStringValue(const std::wstring& path, const std::wstring& defVal)const
{
	if(!AreEnabled())
		return defVal;

	std::wstring retVal(defVal);
	std::wstring xpath(L"//policies//policy");

	if(m_CurrentPolicyName.length() > 0)
		xpath.append(L"[name='" + m_CurrentPolicyName + L"']");

	xpath.append(L"//");
	xpath.append(path);

	std::wstring alternateXPath(xpath);
	alternateXPath.append(L"//@value");
	
	XMLNode retNode;

	CComBSTR bstrXPath( xpath.c_str());
	CComBSTR bstrAlternateXPath( alternateXPath.c_str());
	HRESULT hr = m_Doc->selectSingleNode(bstrXPath,&retNode);
	if(SUCCEEDED(hr) && retNode.p != NULL)
	{
		CComBSTR bstrText;
		if(SUCCEEDED(retNode->get_text(&bstrText)) && bstrText != NULL)
			retVal = static_cast<WCHAR*>(bstrText);
	}

	if(FAILED(hr) || retVal.length() == 0)
	{
		XMLNode retNode;
		hr = m_Doc->selectSingleNode(bstrAlternateXPath,&retNode);
		if(SUCCEEDED(hr) && retNode.p != NULL)
		{
			CComBSTR bstrText;
			if(SUCCEEDED(retNode->get_text(&bstrText)) && bstrText != NULL)
				retVal = static_cast<WCHAR*>(bstrText);
		}
	}

	return retVal;
}

BOOL CPolicies::GetBoolValue(const std::wstring& path, BOOL defVal)const
{
	if(!AreEnabled())
		return defVal;

	std::wstring str(GetStringValue(path, L""));
	if(str.length() == 0)
		return defVal;

	const WCHAR* cs  = str.c_str();
	return	_wcsicmp(cs, L"yes") == 0  ||
			_wcsicmp(cs, L"true") == 0 || 
			_wcsicmp(cs, L"ok") == 0   ||
			_wcsicmp(cs, L"1") == 0    ||
			_wcsicmp(cs, L"do") == 0;
}