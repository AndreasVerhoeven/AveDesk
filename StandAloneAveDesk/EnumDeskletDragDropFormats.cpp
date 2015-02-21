// filename:		EnumDeskletDragDropFormats.cpp
// Description:		Implementation of class EnumDeskletDragDropFormats
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

#include "stdafx.h"
#include "EnumDeskletDragDropFormats.h"
#include "docklet.h"

EnumDeskletDragDropFormats::EnumDeskletDragDropFormats(std::vector<FORMATETC>& formats) :
	m_nRefCnt(1),
	m_Current(0)

{
	// post: An EnumDeskletDragDropFormats object has been created

	m_DragFormats = formats;

	// don't forget to add the default format for desklets
	FORMATETC f;
	f.cfFormat = (CLIPFORMAT)Docklet::clipFormat;
	f.dwAspect = DVASPECT_CONTENT;
	f.lindex = -1;
	f.ptd = 0;
	f.tymed = TYMED_HGLOBAL;

	m_DragFormats.push_back(f);
}

void EnumDeskletDragDropFormats::AddFormat(FORMATETC& format)
{
	m_DragFormats.push_back(format);
}

	// IUnknown
HRESULT STDMETHODCALLTYPE EnumDeskletDragDropFormats::QueryInterface(REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IEnumFORMATETC))
	{
		*ppvObject = static_cast<IEnumFORMATETC *>(this);
	}
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    AddRef();

    return S_OK;
}

ULONG STDMETHODCALLTYPE EnumDeskletDragDropFormats::AddRef(void)
{
	return ++m_nRefCnt;
}

ULONG STDMETHODCALLTYPE EnumDeskletDragDropFormats::Release(void)
{
	m_nRefCnt--;
	if(m_nRefCnt > 0)
		return m_nRefCnt;

	delete this;

	return 0;
}

	// IEnumFORMATETC
HRESULT STDMETHODCALLTYPE EnumDeskletDragDropFormats::Next(ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched)
{
	ULONG i(0);
	while(i < celt && m_Current < (int)m_DragFormats.size() )
	{
		rgelt[i++] = m_DragFormats[m_Current++];
	}
	if(pceltFetched)
		*pceltFetched = i;

	return (i == celt) ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE EnumDeskletDragDropFormats::Skip(ULONG celt)
{
	ULONG i(0);
	while(i < celt && m_Current < (int)m_DragFormats.size() )
	{
		m_Current++;
		i++;
	}

	return (i == celt) ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE EnumDeskletDragDropFormats::Reset(void)
{
	m_Current = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE EnumDeskletDragDropFormats::Clone(IEnumFORMATETC ** ppenum)
{
	if(!ppenum)
	{
		return E_INVALIDARG;
	}
	else
	{
		EnumDeskletDragDropFormats* edddf = new EnumDeskletDragDropFormats(m_DragFormats);
		edddf->m_Current = m_Current;
		edddf->QueryInterface(IID_IEnumFORMATETC,(void**)ppenum);
		edddf->Release();
		return S_OK;
	}
}