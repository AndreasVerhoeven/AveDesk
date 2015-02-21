// AveDeskFileAssociation.h: interface for the CAveDeskFileAssociation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEDESKFILEASSOCIATION_H__B4AB96CF_B2AB_4414_96FE_CF41C7E08B46__INCLUDED_)
#define AFX_AVEDESKFILEASSOCIATION_H__B4AB96CF_B2AB_4414_96FE_CF41C7E08B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CAveDeskFileAssociation  
{
private:
	std::wstring m_Extension;
	std::wstring m_Class;
	std::wstring m_Icon;
	long		 m_IconIndex;
public:
	CAveDeskFileAssociation(const std::wstring& ext, 
													const std::wstring& cls,
													const std::wstring& icon,
													long iconIndex);
	virtual ~CAveDeskFileAssociation();

	void SetExtension(const std::wstring& ext);
	void SetClass(const std::wstring& cls);
	void SetIcon(const std::wstring& fileName, long index);

	bool IsSet()const;
	bool SetAssociation()const;

};

#endif // !defined(AFX_AVEDESKFILEASSOCIATION_H__B4AB96CF_B2AB_4414_96FE_CF41C7E08B46__INCLUDED_)
