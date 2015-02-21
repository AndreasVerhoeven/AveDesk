// AveFont.h: interface for the CAveFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEFONT_H__890631BA_6B03_4326_8C7F_E8F0FA704631__INCLUDED_)
#define AFX_AVEFONT_H__890631BA_6B03_4326_8C7F_E8F0FA704631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "graphics_funcs/image_funcs.h"

#include "IAveUsingFontImpl.h"

class CAveFont : public IAveUsingFontImpl  
{
private:
	CShadowProperties shadowProperties;
	BOOL drawShadow;

	StringFormat strFormat;

public:
	CAveFont();
	virtual ~CAveFont();

	virtual void Draw(Graphics& g, Rect& rc, const std::wstring& ws);

	virtual BOOL GetDrawShadow()const;
	virtual void SetDrawShadow(BOOL val);

	virtual INT GetVerticalAlignment()const;
	virtual INT GetHorizontalAligment()const;

	virtual void SetAlignment(INT vert, INT hor);

	virtual void SetShadowProperties(const CShadowProperties& props);
	virtual CShadowProperties GetShadowProperties();

	void SetFromFont(const CAveFont& font);
};

#endif // !defined(AFX_AVEFONT_H__890631BA_6B03_4326_8C7F_E8F0FA704631__INCLUDED_)
