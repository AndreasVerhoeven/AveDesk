// filename:		EnumDeskletDragDropFormats.h
// Description:		Definition of class EnumDeskletDragDropFormats
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					This class is used to enumerate all the d-'n-d formats
//					that are supported by a desklet.
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#ifndef EnumDeskletDragDropFormats_H_20202019
#define EnumDeskletDragDropFormats_H_20202019

#include <windows.h>
#include <oleidl.h>
#include <prsht.h>
#include <vector>

class EnumDeskletDragDropFormats : public IEnumFORMATETC
{

private:
	long m_nRefCnt;
	std::vector<FORMATETC> m_DragFormats;
	long m_Current;

public:
	EnumDeskletDragDropFormats(std::vector<FORMATETC>& formats);
	void AddFormat(FORMATETC& format);

	// IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

	// IEnumFORMATETC
	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched);
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);

	virtual HRESULT STDMETHODCALLTYPE Reset(void);
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumFORMATETC ** ppenum);
};

#endif//EnumDeskletDragDropFormats_H_20202019