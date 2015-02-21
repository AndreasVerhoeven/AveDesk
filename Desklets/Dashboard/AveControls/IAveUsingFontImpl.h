// IAveUsingFontImpl.h: interface for the IAveUsingFontImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVEUSINGFONTIMPL_H__6DBE07BE_2699_4653_8039_B3589F47567A__INCLUDED_)
#define AFX_IAVEUSINGFONTIMPL_H__6DBE07BE_2699_4653_8039_B3589F47567A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <string>
#include <gdiplus.h>
using namespace Gdiplus;

class IAveUsingFontImpl  
{
protected:
	std::wstring fontName;
	FLOAT fontSize;
	INT fontStyle;
	Color fontColor;
public:
	IAveUsingFontImpl();
	virtual ~IAveUsingFontImpl();

	virtual void SetFontName(const std::wstring& name);
	virtual void SetFontSize(FLOAT size);
	virtual void SetFontStyle(INT style);
	virtual void SetFontColor(Color c);

	virtual std::wstring GetFontName()const;
	virtual FLOAT GetFontSize()const;
	virtual INT GetFontStyle()const;
	virtual Color GetFontColor()const;

	virtual DWORD_PTR InternalFontMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

};

#endif // !defined(AFX_IAVEUSINGFONTIMPL_H__6DBE07BE_2699_4653_8039_B3589F47567A__INCLUDED_)
