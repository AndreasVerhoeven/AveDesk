#ifndef DATAOBJECTIMPL_HPP_2302939426539436939393
#define DATAOBJECTIMPL_HPP_2302939426539436939393


#include <windows.h>
#include <oleidl.h>
#include <shlobj.h>
#include <prsht.h>
#include <atlcom.h>

#include "EnumDeskletDragDropFormats.h"
#include <vector>

template<typename T> class ATL_NO_VTABLE IDataObjectImpl : 
	public IDataObject
{
	struct DATAENTRY
	{
        FORMATETC   fe;
        STGMEDIUM   stgm;
	};

protected:
	std::vector<FORMATETC> m_DragFormats;		// OLE D&D dragformats

	std::vector<DATAENTRY>m_SetDataList;		// OLE Set-data list.

	HRESULT AddRefStgMedium(STGMEDIUM *pstgmIn, STGMEDIUM *pstgmOut, BOOL fCopyIn)
	{
		HRESULT hres = S_OK;
		STGMEDIUM stgmOut = *pstgmIn;

		if (pstgmIn->pUnkForRelease == NULL && !(pstgmIn->tymed & (TYMED_ISTREAM | TYMED_ISTORAGE)))
		{
			if (fCopyIn)
			{
				if (pstgmIn->tymed == TYMED_HGLOBAL)
				{
					stgmOut.hGlobal = GlobalClone(pstgmIn->hGlobal);
					if (!stgmOut.hGlobal)
						hres = E_OUTOFMEMORY;
					else
						hres = DV_E_TYMED;
				}
			}
			else
			{
				stgmOut.pUnkForRelease = static_cast<IDataObject*>(this);
			}
		}

		if (SUCCEEDED(hres))
		{
			switch (stgmOut.tymed)
			{
				case TYMED_ISTREAM:
					stgmOut.pstm->AddRef();
				break;

				case TYMED_ISTORAGE:
					stgmOut.pstg->AddRef();
				break;
			}

			if (stgmOut.pUnkForRelease)
			{
				stgmOut.pUnkForRelease->AddRef();
			}
	
		    *pstgmOut = stgmOut;
		}

		return hres;
	}

	IUnknown *GetCanonicalIUnknown(IUnknown* punk)
	{
		IUnknown *punkCanonical = NULL;
		if (punk && SUCCEEDED(punk->QueryInterface(IID_IUnknown,
                                               (void**)&punkCanonical))) 
		{
			punkCanonical->Release();
		}
		else
		{
			punkCanonical = punk;
		}

		return punkCanonical;
	}

public:
	STDMETHOD(GetData)(FORMATETC *pFormatetc, STGMEDIUM *pmedium)
	{
		T* t = static_cast<T*>(this);

		if(pFormatetc == NULL || pmedium == NULL)
			return E_INVALIDARG;

	    if (!(DVASPECT_CONTENT & pFormatetc->dwAspect))
			return DV_E_DVASPECT;
	
		for(std::vector<FORMATETC>::iterator iter = m_DragFormats.begin(); iter != m_DragFormats.end(); ++iter)
		{
			FORMATETC& f = *iter;
			if(f.cfFormat == pFormatetc->cfFormat && (f.tymed & pFormatetc->tymed) != 0)
			{
				return t->DataRequest(pFormatetc,pmedium);
			}
		}

		for(std::vector<DATAENTRY>::iterator iter2 = m_SetDataList.begin(); iter2 != m_SetDataList.end(); ++iter2)
		{
			DATAENTRY &de = *iter2;
			if (de.fe.cfFormat == pFormatetc->cfFormat &&
				de.fe.dwAspect == pFormatetc->dwAspect &&
				de.fe.lindex == pFormatetc->lindex)
			{
				if ((de.fe.tymed & pFormatetc->tymed)) 
				{
					DATAENTRY* pde = &de;
					AddRefStgMedium(&pde->stgm, pmedium, FALSE);
					return S_OK;
				}
				else
					return DV_E_TYMED;
			}
		}

		return DV_E_FORMATETC;
	}

    STDMETHOD(GetDataHere)(FORMATETC *pFormatetc, STGMEDIUM *pmedium)
	{
		return E_NOTIMPL;	
	}



    STDMETHOD(QueryGetData)(FORMATETC *pFormatetc)
	{
		if(pFormatetc == NULL)
			return E_INVALIDARG;

		if (!(DVASPECT_CONTENT & pFormatetc->dwAspect))
				return DV_E_DVASPECT;

		for(std::vector<FORMATETC>::iterator iter = m_DragFormats.begin(); iter != m_DragFormats.end(); ++iter)
		{
			FORMATETC& f = *iter;
			if(f.cfFormat == pFormatetc->cfFormat && (f.tymed & pFormatetc->tymed) != 0)
				return S_OK;
		}

		for(std::vector<DATAENTRY>::iterator iter2 = m_SetDataList.begin(); iter2 != m_SetDataList.end(); ++iter2)
		{
			DATAENTRY& de = *iter2;
			if (de.fe.cfFormat == pFormatetc->cfFormat &&
				de.fe.dwAspect == pFormatetc->dwAspect &&
				de.fe.lindex == pFormatetc->lindex)
			{
				if ((de.fe.tymed & pFormatetc->tymed)) 
					return S_OK;
				else
					return DV_E_TYMED;
			}
		}		
		return DV_E_FORMATETC;
	}



    STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *pFormatetcIn, FORMATETC *pFormatetcInOut)
	{
		return E_NOTIMPL;
	}


    STDMETHOD(SetData)(FORMATETC *pFormatetc, STGMEDIUM *pmedium, BOOL fRelease)
	{
		std::vector<DATAENTRY>::iterator hit = m_SetDataList.end();
		for(std::vector<DATAENTRY>::iterator iter2 = m_SetDataList.begin(); iter2 != m_SetDataList.end(); ++iter2)
		{
			DATAENTRY& de = *iter2;
			if (de.fe.cfFormat == pFormatetc->cfFormat &&
				de.fe.dwAspect == pFormatetc->dwAspect &&
				de.fe.lindex == pFormatetc->lindex)
			{
				if ((de.fe.tymed & pFormatetc->tymed)) 
				{
					hit = iter2;
					break;
				}
			}
		}
		if(hit == m_SetDataList.end())
		{
			DATAENTRY tmp = {0};
			tmp.fe = *pFormatetc;
			m_SetDataList.push_back(tmp);
			hit = m_SetDataList.end();
			--hit;
		}
		else
		{
			DATAENTRY& de = *hit;
			ReleaseStgMedium(&de.stgm);
			ZeroMemory(&de.stgm, sizeof(STGMEDIUM));
	
		}
		DATAENTRY& de = *hit;

		HRESULT hres = S_OK;

		if (fRelease)
		{
			de.stgm = *pmedium;
			hres = S_OK;
		}
		else
		{	
			hres = AddRefStgMedium(pmedium, &de.stgm, TRUE);
		}

		de.fe.tymed = de.stgm.tymed;
		if (GetCanonicalIUnknown(de.stgm.pUnkForRelease) ==
			    GetCanonicalIUnknown(static_cast<IDataObject*>(this)))
		{
		de.stgm.pUnkForRelease->Release();
		de.stgm.pUnkForRelease = NULL;
		}
		return hres;
	}


    STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ppenumFormatetc)
	{
	    if (ppenumFormatetc == NULL)
		{
	        return E_INVALIDARG;
		}
		else
		{
			EnumDeskletDragDropFormats* enumerator = new EnumDeskletDragDropFormats(m_DragFormats);
			for(std::vector<DATAENTRY>::iterator iter = m_SetDataList.begin(); iter != m_SetDataList.end(); ++iter)
				enumerator->AddFormat(iter->fe);

			(*ppenumFormatetc) = enumerator;

			return S_OK;
		}
	}


    STDMETHOD(DAdvise)(FORMATETC *pFormatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
	{
		return OLE_E_ADVISENOTSUPPORTED;
	}

    STDMETHOD(DUnadvise)(DWORD dwConnection)
	{
		return OLE_E_ADVISENOTSUPPORTED;
	}

    STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppenumAdvise)
	{
		return OLE_E_ADVISENOTSUPPORTED;
	}

	void AddFormat(FORMATETC& format)
	{
		m_DragFormats.push_back(format);
	}

	// FIX: made format parameter constant
	bool RemoveFormat(const FORMATETC& format)
	{
		for(std::vector<FORMATETC>::iterator iter = m_DragFormats.begin(); iter != m_DragFormats.end(); ++iter)
		{
			FORMATETC& f = *iter;
			if(f.cfFormat == format.cfFormat && f.tymed == format.tymed)
			{
				m_DragFormats.erase(iter);
				return true;
			}
		}

		return false;
	}

	void ClearSetDataList()
	{
		for (UINT ide = 0; ide < m_SetDataList.size(); ide++)
		{
			CoTaskMemFree(m_SetDataList[ide].fe.ptd);
			ReleaseStgMedium(&m_SetDataList[ide].stgm);
		}
		m_SetDataList.clear();
	}

	HRESULT DataRequest(FORMATETC *pFormatetc, STGMEDIUM *pmedium)
	{
		return E_UNEXPECTED;
	}
};

#endif//DATAOBJECTIMPL_HPP_2302939426539436939393