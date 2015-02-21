// AveInstallFile.cpp : Implementation of CAveDeskApp and DLL registration.

#include "stdafx.h"
#include "AveDesk.h"
#include "AveInstallFile.h"
#include "shellhelper.h"
#include "AveInstaller.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CAveInstallFile::InterfaceSupportsErrorInfo(REFIID riid)
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


HRESULT __stdcall CAveInstallFile::DragEnter(IDataObject* obj, DWORD dwKeys, POINTL ptl, DWORD* pEffect)
{
	return S_OK;
}

HRESULT __stdcall CAveInstallFile::DragOver(DWORD dwKeys, POINTL ptl, DWORD * pEffect)
{
	return S_OK;
}

HRESULT __stdcall CAveInstallFile::DragLeave()
{
	return S_OK;
}

HRESULT __stdcall CAveInstallFile::Drop(IDataObject * obj, DWORD dwKeys, POINTL ptl, DWORD *pEffect)
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
		CAveInstaller inst;
		inst.InstallAsync(fileName.c_str(),NULL,FALSE);
	}

	return S_OK;
}