// CAveDeskImageShellExt.h : Declaration of the CCAveDeskImageShellExt

#ifndef __CAVEDESKIMAGESHELLEXT_H_
#define __CAVEDESKIMAGESHELLEXT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCAveDeskImageShellExt
class ATL_NO_VTABLE CCAveDeskImageShellExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCAveDeskImageShellExt, &CLSID_CAveDeskImageShellExt>,
	public IDispatchImpl<ICAveDeskImageShellExt, &IID_ICAveDeskImageShellExt, &LIBID_AVEDESKSHELLEXTLib>,
	public IPersistFile,
	public IExtractImage2
{
public:
	CCAveDeskImageShellExt()
	{
		// initialize with some default values
		m_FileName[0] = '0';
		m_Flags = 0;
		m_Size.cx = 48;
		m_Size.cy = 48;
		m_ColorDepth = 32;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CAVEDESKIMAGESHELLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCAveDeskImageShellExt)
	COM_INTERFACE_ENTRY(ICAveDeskImageShellExt)
	COM_INTERFACE_ENTRY(IPersistFile)
	COM_INTERFACE_ENTRY(IExtractImage)
	COM_INTERFACE_ENTRY(IExtractImage2)
END_COM_MAP()

// ICAveDeskImageShellExt
public:

	STDMETHOD(GetClassID)( CLSID* )       { return E_NOTIMPL; }
    STDMETHOD(IsDirty)()                  { return E_NOTIMPL; }
    STDMETHOD(Save)( LPCOLESTR, BOOL )    { return E_NOTIMPL; }
    STDMETHOD(SaveCompleted)( LPCOLESTR ) { return E_NOTIMPL; }
    STDMETHOD(GetCurFile)( LPOLESTR* )    { return E_NOTIMPL; }
    STDMETHOD(Load)( LPCOLESTR wszFile, DWORD /*dwMode*/ )
    { 
		// the shell gives the filename for which the image must be extracted
		// thru this function, so it needs to be saved in the object.
		USES_CONVERSION;
        lstrcpyn ( m_FileName, OLE2CT(wszFile), MAX_PATH );
        return S_OK;
    }

	STDMETHOD(Extract)    (HBITMAP *phBmpImage);
	STDMETHOD(GetLocation)(LPWSTR pszPathBuffer,DWORD cchMax,DWORD *pdwPriority, const SIZE *prgSize,DWORD dwRecClrDepth,DWORD *pdwFlags);

	STDMETHOD(GetDateStamp)(FILETIME *pDateStamp);

	HRESULT InternalDraw(HDC dc, IStream* stream);
	HRESULT DrawPreviewBitmapOnHDC(HDC dc, IStream* stream);

protected:
	TCHAR m_FileName[MAX_PATH+1];
	SIZE  m_Size;
	DWORD m_Flags;
	DWORD m_ColorDepth;
};

#endif //__CAVEDESKIMAGESHELLEXT_H_
