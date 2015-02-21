// ShellHelper.cpp: implementation of the CShellHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "ShellHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShellHelper::CShellHelper()
{

}

CShellHelper::~CShellHelper()
{

}

#define HIDA_GetPIDLFolder(pida) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[0])
#define HIDA_GetPIDLItem(pida, i) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[i+1])

void CShellHelper::GetListOfFileNamesFromShellIdList(LPIDA pida, std::vector<std::wstring>& filenames, int& length)
{
	length = 0;
	filenames.clear();

	if(pida)
	{
		LPCITEMIDLIST	pidlFolder= HIDA_GetPIDLFolder(pida);

		for(int i=0; i < pida->cidl; ++i)
		{
			LPCITEMIDLIST pidl = HIDA_GetPIDLItem(pida,i);
			LPITEMIDLIST pidlFull = ILCombine(pidlFolder,pidl);

			CPidl p;
			p.SetFromPIDL(pidlFull);

			std::wstring fname = p.GetFullParseDisplayName();

			filenames.push_back( fname );

			length += fname.length() + 1;
		}
	}
}

void CShellHelper::GetListOfFileNamesFromHDROP(HDROP hDrop, std::vector<std::wstring>& filenames, int& length)
{
	length = 0;
	filenames.clear();

	UINT num = DragQueryFileW(hDrop,(UINT)-1,NULL,0);

	for(int i=0;i< num; ++i)
	{
		UINT bufLength = DragQueryFileW(hDrop,i,NULL,0) + 1;
		WCHAR* buf = new WCHAR[bufLength+1];
		memset(buf,0,(bufLength+1)*sizeof(WCHAR) );

		DragQueryFileW(hDrop,i,buf,bufLength);
		
		std::wstring fname = buf;

		filenames.push_back( fname );

		length += fname.length() + 1;

		delete[] buf;
	}
}

HRESULT CShellHelper::GetListOfFileNamesFromIDataObject(IDataObject *obj, std::vector<std::wstring>& filenames, int& length)
{
	FORMATETC etc = {0};
	etc.cfFormat = RegisterClipboardFormat(CFSTR_SHELLIDLIST);
	etc.dwAspect = DVASPECT_CONTENT;
	etc.lindex = -1;
	etc.tymed = TYMED_HGLOBAL;
	
	STGMEDIUM medium = {0};
	if( SUCCEEDED(obj->GetData(&etc,&medium)) )
	{
		LPIDA pida = reinterpret_cast<LPIDA>( GlobalLock(medium.hGlobal) );

		GetListOfFileNamesFromShellIdList(pida,filenames,length);

		GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);

		return S_OK;
	}
	
	etc.cfFormat = CF_HDROP;
	etc.dwAspect = DVASPECT_CONTENT;
	etc.lindex = -1;
	etc.tymed = TYMED_HGLOBAL;
	
	if( SUCCEEDED(obj->GetData(&etc,&medium)) )
	{
		HDROP hDrop = reinterpret_cast<HDROP>( GlobalLock(medium.hGlobal) );

		GetListOfFileNamesFromHDROP(hDrop,filenames,length);

		GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);

		return S_OK;
	}

	return E_FAIL;
}