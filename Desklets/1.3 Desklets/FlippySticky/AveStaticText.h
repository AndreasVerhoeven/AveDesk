// AveStaticText.h: interface for the CAveStaticText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVESTATICTEXT_H__E724E17F_6F26_442B_9852_D9E3613B4F92__INCLUDED_)
#define AFX_AVESTATICTEXT_H__E724E17F_6F26_442B_9852_D9E3613B4F92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"

class CAveStaticText : public IAveControl  
{
protected:
	std::wstring fontName;
	float        fontSize;
	StringFormat format;
	Color        color;
	FontStyle    fontStyle;

	std::wstring text;

public:
	CAveStaticText(IAveControlContainer* container, std::string name, BOOL noMessages = FALSE);
	virtual ~CAveStaticText();

	virtual void Update()const;

	virtual  void SetText(const std::wstring& str);
	virtual std::wstring GetText()const;

};

#endif // !defined(AFX_AVESTATICTEXT_H__E724E17F_6F26_442B_9852_D9E3613B4F92__INCLUDED_)
