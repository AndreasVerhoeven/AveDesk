// Policies.h: interface for the CPolicies class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLICIES_H__518E2FFD_3419_465D_8D4D_6DE016554757__INCLUDED_)
#define AFX_POLICIES_H__518E2FFD_3419_465D_8D4D_6DE016554757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>


class CPolicies  
{
protected:
	typedef CComPtr<IXMLDOMNode> XMLNode;
	CComPtr<IXMLDOMDocument2> m_Doc;
	std::wstring m_CurrentPolicyName;

public:
	CPolicies();
	virtual ~CPolicies();

	virtual void Free();
	virtual bool AreEnabled() const;
	virtual HRESULT LoadFromStream(CComPtr<IStream> stream);
	virtual HRESULT LoadFromFile(const std::wstring& filename);
	virtual void SetCurrentPolicyName(const std::wstring& name);
	virtual std::wstring GetCurrentPolicyName()const;
	virtual HRESULT LoadFromString(const std::wstring& xmlStr);
	virtual std::wstring GetStringValue(const std::wstring& path, const std::wstring& defVal)const;
	virtual BOOL GetBoolValue(const std::wstring& path, BOOL defVal)const;

};

#endif // !defined(AFX_POLICIES_H__518E2FFD_3419_465D_8D4D_6DE016554757__INCLUDED_)
