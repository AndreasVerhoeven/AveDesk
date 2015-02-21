// LabelProperties.h: interface for the CLabelProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LABELPROPERTIES_H__AA27109C_7F74_47B7_AFFA_BF5CBC50A893__INCLUDED_)
#define AFX_LABELPROPERTIES_H__AA27109C_7F74_47B7_AFFA_BF5CBC50A893__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<string>

#include "ShadowProperties.h"

#include "IPersistable.h"

// getting dirty (changing access-modifiers in this macro, beware!), the herd style.
#define CLABELPROPERTY(t,n) private: ##t m_##n; \
public:	void Set##n (##t const & v) { m_##n = v; }; \
		##t& Get##n () {return m_##n;}

class CLabelProperties  : public IPersistable
{
private:
	CLABELPROPERTY(bool,HasShadow);
	CLABELPROPERTY(std::wstring,Font);
	CLABELPROPERTY(Color,Color);
	CLABELPROPERTY(int,Style);
	CLABELPROPERTY(int,Size);
	CLABELPROPERTY(CShadowProperties,ShadowProperties)

public:

	DECLARE_PERSISTANCYNAME(LabelProperties);

	BEGIN_LOADING_MAP()
		LOADABLE_PROP_PRIMITIVE(HasShadow);
		LOADABLE_PROP_PRIMITIVE(Font);
		LOADABLE_PROP_PRIMITIVE(Color);
		LOADABLE_PROP_PRIMITIVE(Style);
		LOADABLE_PROP_PRIMITIVE(Size);
		LOADABLE_PROP_CLASS(ShadowProperties);
	END_LOADING_MAP()

	BEGIN_SAVING_MAP()
		SAVABLE_PROP_PRIMITIVE(HasShadow);
		SAVABLE_PROP_PRIMITIVE(Font);
		SAVABLE_PROP_PRIMITIVE(Color);
		SAVABLE_PROP_PRIMITIVE(Style);
		SAVABLE_PROP_PRIMITIVE(Size);
		SAVABLE_PROP_CLASS(ShadowProperties);
	END_SAVING_MAP()

	CLabelProperties();
	~CLabelProperties();

};

#endif // !defined(AFX_LABELPROPERTIES_H__AA27109C_7F74_47B7_AFFA_BF5CBC50A893__INCLUDED_)
