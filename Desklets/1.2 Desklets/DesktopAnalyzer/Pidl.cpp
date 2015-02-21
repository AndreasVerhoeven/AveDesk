// Pidl.cpp: implementation of the CPidl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPidl::CPidl() :
	m_Pidl(NULL),
	m_RelativePidl(NULL),
	m_NotifyId(0)
{
	CoInitialize(NULL);
	CoGetMalloc(1,&m_Malloc);
}

CPidl::~CPidl()
{
	ClearCache();
	FreePidl(m_Pidl);
	CoUninitialize();
}

/*
void CPidl::FreePidl(LPITEMIDLIST& pidl) const
{
	if(pidl)
	{
		CComPtr<IMalloc> malloc = m_Malloc;

		if(malloc.p == NULL)
			CoGetMalloc(1,&malloc);

		if(malloc)
			malloc->Free( static_cast<void*>(pidl) );

		pidl = NULL;
	}
}
*/
void CPidl::FreePidl(LPITEMIDLIST& pidl)const
{
	if(!pidl)return;

	LPITEMIDLIST p=pidl;
	pidl=NULL;
	LPMALLOC lpMalloc=NULL;
	if(FAILED(SHGetMalloc(&lpMalloc)))
		return;

	if(lpMalloc->DidAlloc(p))
		lpMalloc->Free(p);
	if(lpMalloc)lpMalloc->Release();
}

void CPidl::ClearCache()
{
	if(m_ShellFolder.p)
		m_ShellFolder.Release();

	if(m_ShellFolder2.p)
		m_ShellFolder2.Release();

	//FreePidl(m_RelativePidl);
}

bool CPidl::SetFromPath(const std::wstring& path)
{
	ClearCache();
	FreePidl(m_Pidl);

	CComPtr<IShellFolder> desktopFolder;
	if( FAILED(SHGetDesktopFolder(&desktopFolder)) )
		return false;

	WCHAR* wPath = new WCHAR[path.length()+2];
	memset(static_cast<void*>(wPath),0,(path.length()+2)*sizeof(WCHAR));
	wcscpy(wPath,path.c_str() );

	HRESULT _hRes = desktopFolder->ParseDisplayName(NULL,	NULL, wPath,	NULL, &m_Pidl, NULL);

	delete[] wPath;

	return SUCCEEDED(_hRes) ? true : false;
}

void CPidl::SetFromPIDL(LPITEMIDLIST pidl)
{
	ClearCache();
	FreePidl(m_Pidl);
	m_Pidl = pidl;
}


HRESULT CPidl::CopyItemID(LPITEMIDLIST& pidl) const
{ 
	pidl = NULL;

	if( m_Pidl == NULL )
		return E_FAIL;

	CComPtr<IMalloc> malloc;

	if( FAILED(SHGetMalloc(&malloc)) )
		return E_FAIL;

	if( malloc.p == NULL)
		return E_FAIL;

	UINT cb = GetItemIDSize(); 
	pidl = static_cast<LPITEMIDLIST>(malloc->Alloc(cb+sizeof(USHORT))); 

	if( pidl == NULL )
		return E_FAIL;

	memcpy(static_cast<void*>(pidl),static_cast<void*>(m_Pidl),cb);

	*(reinterpret_cast<USHORT*>( reinterpret_cast<BYTE*>(pidl) + cb ) ) = 0;

	return S_OK;
}

HRESULT CPidl::GetRelativePidl(LPITEMIDLIST& pidl)const
{
	pidl = NULL;

	if( m_Pidl == NULL )
		return E_FAIL;

	CComPtr<IMalloc> malloc;

	if( FAILED(SHGetMalloc(&malloc)) )
		return E_FAIL;

	if( malloc.p == NULL)
		return E_FAIL;

	UINT nCount= GetItemIDCount();

	if(nCount>1)
	{
		ITEMIDLIST* temp = m_Pidl;

		UINT Index = nCount - 1;
		while(Index)
		{
			Index--;
			BYTE* b = reinterpret_cast<BYTE*>(temp);
			b += temp->mkid.cb;
			temp= reinterpret_cast<ITEMIDLIST*>(b);
		}

		UINT uSize = temp->mkid.cb;

		pidl = reinterpret_cast<ITEMIDLIST*>(malloc->Alloc( uSize + sizeof(temp->mkid.cb) ));
		memset(pidl,0,uSize+ sizeof(temp->mkid.cb) );
		memcpy(pidl,temp,uSize);
	}

	return S_OK;

}

UINT CPidl::GetItemIDCount() const 
{
	if( m_Pidl == NULL )
		return 0;


	UINT nCount(0);


	BYTE* pCur = reinterpret_cast<BYTE*>(m_Pidl);

	while( (reinterpret_cast<LPCITEMIDLIST>(pCur))->mkid.cb )
	{
		nCount++;
		pCur += (reinterpret_cast<LPCITEMIDLIST>(pCur))->mkid.cb;
	}

	return nCount;
}

UINT CPidl::GetItemIDSize()  const 
{
	if( m_Pidl == NULL )
		return 0;

	LPITEMIDLIST pidl = m_Pidl;

	int nSize(0);

	while( pidl->mkid.cb ) 
	{
		nSize += pidl->mkid.cb;
		pidl = reinterpret_cast<LPITEMIDLIST>( (reinterpret_cast<BYTE*>(pidl)) + pidl->mkid.cb );
	}

	return nSize;
}

HRESULT CPidl::GetParentShellFolder(IShellFolder** folder,LPCITEMIDLIST* relativePidl, IShellFolder2** folder2)const
{
	if(m_Pidl == NULL)
	{
		*folder = NULL;

		if(m_ShellFolder.p)
		{
			m_ShellFolder.p->QueryInterface(IID_IShellFolder,reinterpret_cast<void**>(folder));

			relativePidl = NULL;

			if( folder2)
			{
				*folder2 = NULL;

				if(m_ShellFolder2)
					m_ShellFolder2.p->QueryInterface(IID_IShellFolder2,reinterpret_cast<void**>(folder2));
			}

			return S_OK;
		}
		else
			return E_FAIL;
	}

	*folder = NULL;

	if(m_ShellFolder.p)
	{
		m_ShellFolder.p->QueryInterface(IID_IShellFolder,reinterpret_cast<void**>(folder));

		//if(relativePidl)
		//	CopyItemID(*relativePidl);

		if( folder2)
		{
			*folder2 = NULL;

			if(m_ShellFolder2)
				m_ShellFolder2.p->QueryInterface(IID_IShellFolder2,reinterpret_cast<void**>(folder2));
		}

		return S_OK;
	}
	else
	{
		//if(relativePidl)
		//	GetRelativePidl(*relativePidl);

		LPCITEMIDLIST pidlChild;

		HRESULT _hRes =  SHBindToParent(m_Pidl,
                              IID_IShellFolder, 
                              (void**) folder, 
                              const_cast<LPCITEMIDLIST*>(&pidlChild));

		if(relativePidl)
		{
			*relativePidl = pidlChild;
		}

		if( folder2 )
		{
			*folder2 = NULL;

			if( SUCCEEDED( _hRes) )
				(*folder)->QueryInterface(IID_IShellFolder2,reinterpret_cast<void**>(folder2));
		}

		return _hRes;
	}
}

HRESULT CPidl::CacheParentShellFolder()
{
	ClearCache();

	HRESULT _hRes = E_FAIL;

	if(m_Pidl != NULL)
	{	
		GetRelativePidl(m_RelativePidl);
		_hRes =  SHBindToParent(m_Pidl,
			                      IID_IShellFolder, 
				                  reinterpret_cast<void**>(&m_ShellFolder), 
					              const_cast<LPCITEMIDLIST*>(&m_RelativePidl));
	}

	if( FAILED(_hRes) )
		_hRes = SHGetDesktopFolder(&m_ShellFolder);

	if( SUCCEEDED(_hRes) )
	{
		m_ShellFolder.p->QueryInterface(IID_IShellFolder2,reinterpret_cast<void**>(&m_ShellFolder2));

	}

	return _hRes;

}

HRESULT CPidl::GetShellDetailString(std::wstring& wStr, UINT num)
{
	// pre:  d is not 0
	// post: the listbox with HWND hwndList has been filled with the strings
	//		 that can be parsed for the sublabel.


	USES_CONVERSION;

	CComPtr<IShellFolder>	shellFolder;
	CComPtr<IShellFolder2>	shellFolder2;
	LPCITEMIDLIST relativePidl = {0};

	GetParentShellFolder(&shellFolder,&relativePidl,&shellFolder2);

	SHELLDETAILS details = {0};

	if( !shellFolder2.p || FAILED( shellFolder2->GetDetailsOf(relativePidl,num,&details)) )
		return E_FAIL;



	STRRET str = {0};
	TCHAR szDisplayName[MAX_PATH+2] = {0};

	StrRetToBuf(&details.str, 
	            m_Pidl, 
		        szDisplayName, 
				sizeof(szDisplayName)/sizeof((szDisplayName)[0]));

	wStr = T2W(szDisplayName);

	return S_OK;
}

std::wstring CPidl::GetFullParseDisplayName() const
{
	USES_CONVERSION;

	if(m_Pidl == NULL)
		return L"";

	CComPtr<IShellFolder> parentFolder;
	LPCITEMIDLIST relativePidl = {0};

	HRESULT _hRes = GetParentShellFolder(&parentFolder,&relativePidl);

	if(FAILED(_hRes))
		return L"";

	STRRET str = {0};
	TCHAR szDisplayName[MAX_PATH+2] = {0};

	if( FAILED(parentFolder->GetDisplayNameOf(relativePidl, SHGDN_FORPARSING, &str))  )
		return L"";

	StrRetToBuf(&str, 
	            m_Pidl, 
		        szDisplayName, 
				sizeof(szDisplayName)/sizeof((szDisplayName)[0]));


	return std::wstring(T2W(szDisplayName));
}

HRESULT CPidl::GetIContextMenu(IContextMenu** menu, HWND hWnd)
{
	if(m_Pidl == NULL)
		return E_FAIL;

	CComPtr<IShellFolder> parentFolder;
	LPCITEMIDLIST relativePidl = {0};
	if( FAILED(GetParentShellFolder(&parentFolder,&relativePidl)) ) 
		return E_FAIL;

	return parentFolder->GetUIObjectOf(hWnd,1,const_cast<LPCITEMIDLIST*>(&relativePidl),IID_IContextMenu,NULL,(void**)menu);
}

HRESULT CPidl::GetAtrributesOf(ULONG* attr)
{
	if(m_Pidl == NULL)
		return E_FAIL;

	CComPtr<IShellFolder> parentFolder;
	LPCITEMIDLIST relativePidl = {0};
	if( FAILED(GetParentShellFolder(&parentFolder,&relativePidl)) ) 
		return E_FAIL;

	return parentFolder->GetAttributesOf(1,&relativePidl,attr);
}

bool CPidl::InvokeDefaultVerb(HWND hWnd, CMINVOKECOMMANDINFOEX& cmi)
{
	const UINT QCM_FIRST	= 1;
	const UINT QCM_LAST		= 0x7FF;

	if(m_Pidl == NULL)
		return false;

	CComPtr<IContextMenu> cm;
	if( FAILED( GetIContextMenu(&cm,hWnd)) )
		return false;

	HMENU hMenu = CreatePopupMenu();
	if(!hMenu)
		return false;

	bool bRet = false;

	if( SUCCEEDED(cm->QueryContextMenu(hMenu,0,QCM_FIRST,QCM_LAST,CMF_DEFAULTONLY)) )
	{
		UINT id = GetMenuDefaultItem(hMenu,FALSE,0);
		if( id != (UINT)-1)
		{
			//CMINVOKECOMMANDINFOEX cmi = {0};
			cmi.cbSize  = sizeof(cmi);
			cmi.hwnd    = hWnd;

			cmi.fMask	= CMIC_MASK_UNICODE | CMIC_MASK_PTINVOKE;
			if (GetKeyState(VK_CONTROL) < 0)
				cmi.fMask |= CMIC_MASK_CONTROL_DOWN;
			if (GetKeyState(VK_SHIFT) < 0)
				cmi.fMask |= CMIC_MASK_SHIFT_DOWN;

			cmi.lpVerb  = MAKEINTRESOURCEA(id - QCM_FIRST);
			cmi.lpVerbW = MAKEINTRESOURCEW(id - QCM_FIRST);
			//cmi.nShow   = SW_SHOWNORMAL;
			//cmi.ptInvoke= point;
			if( SUCCEEDED(cm->InvokeCommand(reinterpret_cast<LPCMINVOKECOMMANDINFO>(&cmi))) )
				bRet = true;
		}
	}


	DestroyMenu(hMenu);

	return bRet;
}


std::wstring CPidl::GetFriendlyName() const
{
	USES_CONVERSION;

	if(m_Pidl == NULL)
		return L"";

	CComPtr<IShellFolder> parentFolder;
	LPCITEMIDLIST relativePidl = NULL;

	HRESULT _hRes = GetParentShellFolder(&parentFolder,&relativePidl);

	if(FAILED(_hRes))
		return L"";

	STRRET str = {0};
	TCHAR szDisplayName[MAX_PATH+2] = {0};

	if( FAILED(parentFolder->GetDisplayNameOf(relativePidl, SHGDN_FOREDITING, &str)) )
		return L"";

	StrRetToBuf(&str, 
	            NULL, 
		        szDisplayName, 
				sizeof(szDisplayName)/sizeof((szDisplayName)[0]));

	return std::wstring(T2W(szDisplayName));
}


LRESULT CALLBACK CPidl::SubclassMenuWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	AVECMSUBCLASSDATA *pSubclassData = reinterpret_cast<AVECMSUBCLASSDATA*>(::GetProp(hWnd, _T("AVECMSUBCLASSDATA")) );

	if( pSubclassData )
	{
		/*switch( uMsg )
		{
			case WM_INITMENUPOPUP:
			case WM_DRAWITEM:
			case WM_MENUCHAR:
			case WM_MEASUREITEM:
				if( pSubclassData->pContextMenu2 && 
					SUCCEEDED(pSubclassData->pContextMenu2->HandleMenuMsg(uMsg, wParam, lParam)) )
					return TRUE;
				else
					return FALSE;
			}
		*/

		if( pSubclassData->pContextMenu3 )
		{
			LRESULT lres(0);
			if( SUCCEEDED(pSubclassData->pContextMenu3->HandleMenuMsg2(uMsg, wParam, lParam, &lres)) )
			  return lres;
		}
		else if( pSubclassData->pContextMenu2 )
		{
			if( SUCCEEDED(pSubclassData->pContextMenu2->HandleMenuMsg(uMsg, wParam, lParam)) )
			  return 0;
		}
	
		return CallWindowProc(pSubclassData->pOldWndProc,
			hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

UINT CPidl::TrackItemIDContextMenu(HMENU hMenu, UINT nFlags, const POINT& point, HWND hWnd, CMINVOKECOMMANDINFOEX& cmi)
{
	if( !IsMenu(hMenu) )
		return 0;


	UINT QCM_FIRST = GetMenuItemCount(hMenu)+1;
	UINT QCM_LAST	= 0x7FF;

	if( m_Pidl == NULL || !IsWindow(hWnd) )
		return FALSE;

	
	CComPtr<IContextMenu> cm;
	if( FAILED( GetIContextMenu(&cm,hWnd)) )
		return FALSE;

	if( FAILED(cm->QueryContextMenu(hMenu,0,QCM_FIRST,QCM_LAST,CMF_NORMAL)) )
		return FALSE;


	bool shouldSubclass(false);

	CComPtr<IContextMenu2> cm2;
	if( SUCCEEDED( cm.p->QueryInterface(IID_IContextMenu2,(void**)&cm2)) )
		shouldSubclass = true;

	CComPtr<IContextMenu3> cm3;
	if( SUCCEEDED( cm.p->QueryInterface(IID_IContextMenu3,(void**)&cm3)) )
		shouldSubclass = true;

	std::auto_ptr<AVECMSUBCLASSDATA>scData (new AVECMSUBCLASSDATA);

	if( shouldSubclass )
	{
		::SetProp(hWnd,_T("AVECMSUBCLASSDATA"),static_cast<HANDLE>(scData.get()) );
		scData->pContextMenu2 = cm2.p;
		scData->pContextMenu3 = cm3.p;
		scData->pOldWndProc = reinterpret_cast<WNDPROC>(::SetWindowLong(hWnd,GWL_WNDPROC, reinterpret_cast<LONG>(SubclassMenuWndProc)) );
	}

	nFlags &= ~TPM_NONOTIFY;
	nFlags |= TPM_RETURNCMD;

	UINT cmd = TrackPopupMenuEx(hMenu,nFlags,point.x,point.y,hWnd,NULL);

	
	if( shouldSubclass )
	{
		::SetWindowLong(hWnd,GWL_WNDPROC,reinterpret_cast<LONG>(scData->pOldWndProc) );
		::RemoveProp(hWnd,_T("AVECMSUBCLASSDATA"));
	}

	if( cmd >=  QCM_FIRST && cmd <= QCM_LAST)
	{
		//CMINVOKECOMMANDINFOEX cmi = {0};
		cmi.cbSize  = sizeof(cmi);
		cmi.hwnd = hWnd;

		cmi.fMask	= CMIC_MASK_UNICODE | CMIC_MASK_PTINVOKE;
		if (GetKeyState(VK_CONTROL) < 0)
			cmi.fMask |= CMIC_MASK_CONTROL_DOWN;
		if (GetKeyState(VK_SHIFT) < 0)
			cmi.fMask |= CMIC_MASK_SHIFT_DOWN;

		cmi.lpVerb  = MAKEINTRESOURCEA(cmd - QCM_FIRST);
		cmi.lpVerbW = MAKEINTRESOURCEW(cmd - QCM_FIRST);
		cmi.ptInvoke= point;
		cm->InvokeCommand(reinterpret_cast<LPCMINVOKECOMMANDINFO>(&cmi));
		return FALSE;
	}
	else
	{
		return cmd;
	}
}

HICON CPidl::ExtractIcon(UINT size, UINT flags)
{
	if(m_Pidl == NULL)
		return NULL;

	/*
	SHFILEINFO info = {0};
	SHGetFileInfo(reinterpret_cast<LPCTSTR>(m_Pidl),0, &info,sizeof(info),SHGFI_PIDL | SHGFI_ICON);

	return info.hIcon;*/

	CComPtr<IShellFolder> parentFolder;
	LPCITEMIDLIST relativePidl = NULL;

	HRESULT _hRes = GetParentShellFolder(&parentFolder,&relativePidl);

	if( FAILED(_hRes) )
		return NULL;

	CComPtr<IExtractIcon> extractIcon;
	_hRes = parentFolder->GetUIObjectOf(NULL,1,const_cast<LPCITEMIDLIST*>(&relativePidl),IID_IExtractIcon,NULL,reinterpret_cast<void**>(&extractIcon) );

	if( FAILED(_hRes) )
		return NULL;

	TCHAR location[MAX_PATH+2] = {0};
	int	  index(0);
	UINT outFlags(0);
	_hRes = extractIcon->GetIconLocation(0,location,MAX_PATH,&index,&outFlags);

	if( FAILED(_hRes) )

		return NULL;

	HICON smallIcon = {0};
	HICON largeIcon = {0};

	size = MAKELONG (size,size);

	_hRes = extractIcon->Extract(location,index,&smallIcon,&largeIcon,size);

	if( FAILED(_hRes) )
	{
		return NULL;
	}



	if(largeIcon)
	{
		DestroyIcon(smallIcon);
		return largeIcon;
	}
	else
	{
		return smallIcon;
	}

}

void CPidl::StopMonitoring()
{

	if(m_NotifyId)
		SHChangeNotifyDeregister(m_NotifyId);

	m_NotifyId = 0;
}

bool CPidl::StartMonitoring(HWND hWnd, UINT msg, DWORD events, BOOL recursive )
{
	if( !IsValid() )
		return false;

	StopMonitoring();

	SHChangeNotifyEntry shCNE;
	shCNE.pidl = m_Pidl;
	shCNE.fRecursive = recursive;

	m_NotifyId = SHChangeNotifyRegister(hWnd,0x0001 | 0x0002 | 0x1000, events,msg, 1,&shCNE);

	return m_NotifyId != 0;
}

HRESULT CPidl::SetFromSpecialFolderLocation(int nFolder)
{
	ClearCache();
	FreePidl(m_Pidl);

	return SHGetSpecialFolderLocation(NULL,nFolder,&m_Pidl);
}

HBITMAP CPidl::ExtractThumb(const SIZE* prgSize, DWORD dwRecClrDepth)
{	


	if(m_Pidl == NULL)
		return NULL;

	CComPtr<IShellFolder> parentFolder;
	LPCITEMIDLIST relativePidl = NULL;

	HRESULT _hRes = GetParentShellFolder(&parentFolder,&relativePidl);

	if( FAILED(_hRes) )
		return NULL;

	CComPtr<IExtractImage> extract;
	_hRes = parentFolder->GetUIObjectOf(NULL,1,const_cast<LPCITEMIDLIST*>(&relativePidl),__uuidof(IExtractImage),NULL,reinterpret_cast<void**>(&extract) );

	if( FAILED(_hRes) || extract.p == NULL )
		return NULL;

	WCHAR wBuf[MAX_PATH+2] = {0};
	DWORD dwPriority(0);
	DWORD dwFlags = IEIFLAG_SCREEN;

	HBITMAP hBmp = {0};

	_hRes = extract->GetLocation(wBuf,MAX_PATH,&dwPriority, prgSize, dwRecClrDepth,&dwFlags);

	if( FAILED(_hRes) )
		return NULL;

	extract->Extract(&hBmp);

	return hBmp;
}
