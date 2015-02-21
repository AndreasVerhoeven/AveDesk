// AveThemeFile.cpp : Implementation of CAveDeskApp and DLL registration.


#include "stdafx.h"
#include "AveDesk.h"
#include "AveThemeFile.h"
#include "shellhelper.h"
#include "Docklet.h"

/////////////////////////////////////////////////////////////////////////////
//

extern BOOL _noLoadThemeOnStartUp;

STDMETHODIMP CAveThemeFile::InterfaceSupportsErrorInfo(REFIID riid)
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


HRESULT __stdcall CAveThemeFile::DragEnter(IDataObject* obj, DWORD dwKeys, POINTL ptl, DWORD* pEffect)
{
	_noLoadThemeOnStartUp = TRUE;
	return S_OK;
}

HRESULT __stdcall CAveThemeFile::DragOver(DWORD dwKeys, POINTL ptl, DWORD * pEffect)
{
	return S_OK;
}

HRESULT __stdcall CAveThemeFile::DragLeave()
{
	return S_OK;
}

HRESULT __stdcall CAveThemeFile::Drop(IDataObject * obj, DWORD dwKeys, POINTL ptl, DWORD *pEffect)
{
	// this is where the cool stuff goes!
	std::vector<std::wstring> files;
	int totalLength(0);

	HRESULT hRes = CShellHelper::GetListOfFileNamesFromIDataObject(obj,files,totalLength);
	if(FAILED(hRes))
		return E_FAIL;

	if(files.size() > 0)
	{
		std::wstring themeFile = files[files.size()-1];
		CComBSTR bstrThemeFile(themeFile.c_str());
		if(_App)
			_App->LoadTheme(bstrThemeFile);
	}

	return S_OK;
}