// AveDeskletFile.cpp : Implementation of CAveDeskApp and DLL registration.

#include "stdafx.h"
#include "AveDesk.h"
#include "AveDeskletFile.h"
#include "shellhelper.h"
#include "Docklet.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CAveDeskletFile::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAveDeskletFile,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


HRESULT __stdcall CAveDeskletFile::DragEnter(IDataObject* obj, DWORD dwKeys, POINTL ptl, DWORD* pEffect)
{
	return S_OK;
}

HRESULT __stdcall CAveDeskletFile::DragOver(DWORD dwKeys, POINTL ptl, DWORD * pEffect)
{
	return S_OK;
}

HRESULT __stdcall CAveDeskletFile::DragLeave()
{
	return S_OK;
}

HRESULT __stdcall CAveDeskletFile::Drop(IDataObject * obj, DWORD dwKeys, POINTL ptl, DWORD *pEffect)
{
	// this is where the cool stuff goes!
	std::vector<std::wstring> files;
	int totalLength(0);

	HRESULT hRes = CShellHelper::GetListOfFileNamesFromIDataObject(obj,files,totalLength);
	if(FAILED(hRes))
		return E_FAIL;

	for(std::vector<std::wstring>::iterator iter = files.begin(); iter != files.end(); ++iter)
	{
		std::wstring& fileName = *iter;
		hRes = Docklet::LoadFromSingleFile(fileName.c_str(),NULL,TRUE);
	}

	return S_OK;
}