// ShellFolder.cpp: implementation of the CShellFolder class.
//
//////////////////////////////////////////////////////////////////////

#include "ShellFolder.h"
#include "pidl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShellFolder::CShellFolder()
{

}

CShellFolder::~CShellFolder()
{

}

HRESULT CShellFolder::SetToDesktopFolder()
{
	if(m_ShellFolder.p) m_ShellFolder.Release();
	if(m_ShellFolder2.p) m_ShellFolder2.Release();

	return SHGetDesktopFolder(&m_ShellFolder);
}

HRESULT CShellFolder::SetFromPIDL(LPITEMIDLIST pidl)
{
	CPidl p;
	p.SetFromPIDL(pidl);

	LPCITEMIDLIST relPidl = {0};

	HRESULT hRes = S_OK;

	CShellFolder parentFolder;

	hRes = p.GetParentShellFolder(&parentFolder,&relPidl);
	if(FAILED(hRes))
	{
		p.Detach();
		return hRes;
	}

	hRes =  parentFolder.m_ShellFolder->BindToObject(relPidl, NULL, IID_IShellFolder, reinterpret_cast<void**>(&m_ShellFolder));

	p.Detach();

	return hRes;
}

bool CShellFolder::GetPIDLFromRelativePath(const std::wstring& path, LPITEMIDLIST* pidl)
{
	if(!m_ShellFolder.p)
		return false;

	WCHAR* wPath = new WCHAR[path.length()+2];
	memset(static_cast<void*>(wPath),0,(path.length()+2)*sizeof(WCHAR));
	wcscpy(wPath,path.c_str() );

	HRESULT _hRes = m_ShellFolder->ParseDisplayName(NULL,	NULL, wPath,	NULL, pidl, NULL);

	delete[] wPath;

	return SUCCEEDED(_hRes) ? true : false;
}

HRESULT	CShellFolder::SetFromShellFolder(LPSHELLFOLDER shellFolder)
{
	if(m_ShellFolder.p) m_ShellFolder.Release();
	if(m_ShellFolder2.p) m_ShellFolder2.Release();

	if(shellFolder == NULL)
		return E_FAIL;

	HRESULT _hRes =  shellFolder->QueryInterface(IID_IShellFolder,reinterpret_cast<void**>(&m_ShellFolder));

	if( SUCCEEDED(_hRes) )
		shellFolder->QueryInterface(IID_IShellFolder2,reinterpret_cast<void**>(&m_ShellFolder2));


	return _hRes;
}

HRESULT CShellFolder::GetShellDetailString(LPCITEMIDLIST relativePidl, std::wstring& wStr, UINT num)
{

	if(m_ShellFolder2.p == NULL)
	{
		if(m_ShellFolder.p == NULL)
			return E_FAIL;

		HRESULT _hRes = m_ShellFolder.p->QueryInterface(IID_IShellFolder2,reinterpret_cast<void**>(&m_ShellFolder2));
		if( FAILED(_hRes) )
			return _hRes;
	}

	USES_CONVERSION;

	SHELLDETAILS details = {0};

	if( !m_ShellFolder2.p || FAILED( m_ShellFolder2->GetDetailsOf(relativePidl,num,&details)) )
		return E_FAIL;

	STRRET str = {0};
	TCHAR szDisplayName[MAX_PATH+2] = {0};

	StrRetToBuf(&details.str, 
	            relativePidl, 
		        szDisplayName, 
				sizeof(szDisplayName)/sizeof((szDisplayName)[0]));

	wStr = T2W(szDisplayName);

	return S_OK;
}