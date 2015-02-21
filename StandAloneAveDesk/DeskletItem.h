// DeskletItem.h: interface for the CDeskletItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETITEM_H__3EBFC275_2DEB_4FDA_BC90_75FB894DAFE3__INCLUDED_)
#define AFX_DESKLETITEM_H__3EBFC275_2DEB_4FDA_BC90_75FB894DAFE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "IProgressIndicator.h"


class CDeskletItem  
{
protected:
	ULONG m_Ref;
	HBITMAP m_SmallIcon;
	HBITMAP m_BigIcon;
	SIZE m_Size;
	std::wstring m_Name;
	std::wstring m_Author;
	std::wstring m_Description;
	std::wstring m_GUID;
	DWORD m_Flags;
	IProgressIndicator* m_Indicator;

public:
	CDeskletItem();
	virtual ~CDeskletItem() = 0;


	virtual ULONG AddRef();
	virtual ULONG Release();

	virtual HBITMAP GetSmallIcon() = 0;
	virtual HBITMAP GetBigIcon() = 0;
	virtual SIZE GetSize() = 0;
	virtual BOOL AreSmallAndBigIconTheSame() = 0;

	virtual std::wstring GetName() = 0;
	virtual std::wstring GetAuthor() = 0;
	virtual std::wstring GetDescription() = 0;
	virtual std::wstring GetTypeName() = 0;
	virtual std::wstring GetCategoryName() = 0;

	virtual DWORD GetFlags() = 0;
	virtual std::wstring GetGUID() = 0;

	virtual void StartCreatingSmallIcon(const SIZE& s, HWND hwnd, UINT msg) = 0;
	virtual void StopCreatingSmallIcon() = 0;
	virtual void StartCreatingBigIcon(HWND hwnd, UINT msg) = 0;
	virtual void StopCreatingBigIcon() = 0;


	virtual BOOL StartingToInstantiate(IProgressIndicator* progress) = 0;
	virtual void StopToInstantiate() = 0;

	virtual HWND Instantiate(BOOL deskletInitHidden, POINT* pt, BOOL async, IProgressIndicator* progress) = 0;

	virtual bool Equals(CDeskletItem* item) = 0;

};

#endif // !defined(AFX_DESKLETITEM_H__3EBFC275_2DEB_4FDA_BC90_75FB894DAFE3__INCLUDED_)
