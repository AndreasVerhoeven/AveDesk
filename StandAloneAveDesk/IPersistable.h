// IPersistable.h: interface for the IPersistable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPERSISTABLE_H__17F717C1_D9A4_4169_899F_80C9C4387665__INCLUDED_)
#define AFX_IPERSISTABLE_H__17F717C1_D9A4_4169_899F_80C9C4387665__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <sstream>
#include <vector>


inline void CHECKHRES(HRESULT hRes)
{
	if(FAILED(hRes))
		throw(hRes);
}

enum PersisterStates { psLoading, psSaving };

class ILoader;
class ISaver;

class IPersistable
{
public:
	virtual HRESULT Load(ILoader& loader) = 0;
	virtual HRESULT Save(ISaver& saver) = 0;
};

class ILoader
{
public:
	virtual HRESULT LoadString(std::wstring& str, const WCHAR* name) = 0;
	virtual HRESULT LoadPersistableClass(IPersistable& p, const WCHAR* name) = 0;
	virtual ILoader* Clone() = 0;
};

class ISaver
{
public:
	virtual HRESULT SaveString(const std::wstring& str, const WCHAR* name) = 0;
	virtual HRESULT SavePersistableClass(IPersistable& p, const WCHAR* name) = 0;
};

template<typename T> inline void PrimitiveToWString(const T& t, std::wstring& output)
{
	std::wstringstream ws;
	ws << t;
	output = ws.str();
}

template<> inline void PrimitiveToWString<std::wstring>(const std::wstring& t, std::wstring& output)
{
	output = t;
}

template<> inline void PrimitiveToWString<Color>(const Color& t, std::wstring& output)
{
	PrimitiveToWString(t.GetValue(),output);
}

template<> inline void PrimitiveToWString<BYTE>(const BYTE& t, std::wstring& output)
{
	PrimitiveToWString(static_cast<int>(t),output);
}



template<typename T> inline void WStringToPrimitive(const std::wstring& input, T& t)
{
	if(input.length() != 0)
	{
		std::wstringstream ws;
		ws.str(input);
		ws >> t;
	}
}

template<>  inline void WStringToPrimitive<Color>(const std::wstring& input, Color& t)
{
	ARGB argb = {0};
	WStringToPrimitive(input,argb);
	t.SetValue(argb);
}

template<>  inline void WStringToPrimitive<BYTE>(const std::wstring& input, BYTE& t)
{
	int b=static_cast<int>(t);
	WStringToPrimitive(input,b);
	t = static_cast<BYTE>(b);
}

template<>inline void WStringToPrimitive<std::wstring>(const std::wstring& input, std::wstring& t)
{
	t = input;
}


#define DECLARE_PERSISTANCYNAME(n)				\
virtual std::wstring _GetPersistancyName()const	\
{												\
	USES_CONVERSION;							\
	return std::wstring(A2W(#n));				\
}

#define BEGIN_LOADING_MAP()				\
virtual HRESULT Load(ILoader& loader)	\
{										\
	USES_CONVERSION;					\
	std::wstring _tmp;


#define LOADABLE_PRIMITIVE(t)		PrimitiveToWString(##t,_tmp);		\
									loader.LoadString(_tmp,A2W(#t));	\
									WStringToPrimitive(_tmp,##t);

#define LOADABLE_PROP_PRIMITIVE(t)	PrimitiveToWString(m_##t,_tmp);		\
									loader.LoadString(_tmp,A2W(#t));	\
									WStringToPrimitive(_tmp,m_##t);

#define LOADABLE_PRIMITIVE_NAMED(t,n)PrimitiveToWString(##t,_tmp);		\
									loader.LoadString(_tmp,A2W(#n));	\
									WStringToPrimitive(_tmp,##t);

#define LOADABLE_PRIMITIVE_UNNAMED(t)PrimitiveToWString(##t,_tmp);		\
									loader.LoadString(_tmp,NULL));		\
									WStringToPrimitive(_tmp,##t);


#define LOADABLE_CLASS(t)			loader.LoadPersistableClass(##t,A2W(#t));
#define LOADABLE_PROP_CLASS(t)		loader.LoadPersistableClass(m_##t,A2W(#t));
#define LOADABLE_CLASS_NAMED(t,n)	loader.LoadPersistableClass(##t,A2W(#n));
#define LOADABLE_CLASS_UNNAMED(t)	loader.LoadPersistableClass(##t,NULL);

#define LOADABLE_CLASS_PTR(t)		if(##t)loader.LoadPersistableClass(*##t,A2W(#t));
#define LOADABLE_PROP_CLASS_PTR(t)	if(m_##t)loader.LoadPersistableClass(*m_##t,A2W(#t));
#define LOADABLE_CLASS_PTR_NAMED(t,n)if(##t)loader.LoadPersistableClass(*##t,A2W(#n));
#define LOADABLE_CLASS_PTR_UNNAMED(t)if(##t)loader.LoadPersistableClass(*##t,NULL);


#define END_LOADING_MAP()			\
		return S_OK;				\
}



#define BEGIN_SAVING_MAP()				\
virtual HRESULT Save(ISaver& saver)		\
{										\
	USES_CONVERSION;					\
	std::wstring _tmp;


#define SAVABLE_PRIMITIVE(t)		PrimitiveToWString(##t,_tmp);	\
									saver.SaveString(_tmp,A2W(#t));

#define SAVABLE_PROP_PRIMITIVE(t)	PrimitiveToWString(m_##t,_tmp);	\
									saver.SaveString(_tmp,A2W(#t));

#define SAVABLE_PRIMITIVE_NAMED(t,n)PrimitiveToWString(##t,_tmp);	\
									saver.SaveString(_tmp,A2W(#n)); 

#define SAVABLE_PRIMITIVE_UNNAMED(t)PrimitiveToWString(##t,_tmp);	\
									saver.SaveString(_tmp,NULL); 

#define SAVABLE_CLASS(t)			saver.SavePersistableClass(##t,A2W(#t));
#define SAVABLE_PROP_CLASS(t)		saver.SavePersistableClass(m_##t,A2W(#t));
#define SAVABLE_CLASS_NAMED(t,n)	saver.SavePersistableClass(##t,A2W(#n));
#define SAVABLE_CLASS_NUNAMED(t,n)	saver.SavePersistableClass(##t,NULL));

#define SAVABLE_CLASS_PTR(t)		if(##t)saver.SavePersistableClass(*##t,A2W(#t));
#define SAVABLE_PROP_CLASS_PTR(t)	if(m_##t)saver.SavePersistableClass(*m_##t,A2W(#t));
#define SAVABLE_CLASS_PTR_NAMED(t,n)if(##t)saver.SavePersistableClass(*##t,A2W(#n));
#define SAVABLE_CLASS_PTR_UNNAMED(t)if(##t)saver.SavePersistableClass(*##t,NULL);


#define END_SAVING_MAP()				\
	return S_OK;						\
}

#endif // !defined(AFX_IPERSISTABLE_H__17F717C1_D9A4_4169_899F_80C9C4387665__INCLUDED_)
