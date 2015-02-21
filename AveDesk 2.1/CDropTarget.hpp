#ifndef CDROPTARGETIMPL_HPP_2302939426539436939393
#define CDROPTARGETIMPL_HPP_2302939426539436939393


#include <windows.h>
#include <oleidl.h>
#include <shlobj.h>
#include <prsht.h>

class template<typename T> CDropTarget : IDropTarget
{
private:
	long m_lRefCount;
	CComPtr<IDropTargetHelper>	dropTargetHelper;
	
public:
	CDropTarget() :
		m_lRefCount(0)
	{

	}

	// IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)
	{
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget))
		{
			*ppvObject = static_cast<IDropTarget*>(t);
			
			t->AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		m_lRefCount++;
		return m_lRefCount;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		m_lRefCount--;
		if(m_lRefCount > 0)
			return m_lRefCount;

		delete this;

		return 0;
	}

	// IDropTarget
    virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject*, DWORD, POINTL, DWORD*)
	{
		return S_NOTIMPLEMENTED;
	}

    virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD dw, POINTL, DWORD *)
	{
		return E_NOTIMPL;
	}

    virtual HRESULT STDMETHODCALLTYPE DragLeave()
	{

	}

    virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *, DWORD, POINTL, DWORD *)
	{

	}
};

#endif//CDROPTARGETIMPL_HPP_2302939426539436939393