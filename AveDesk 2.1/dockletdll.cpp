// filename:		dockletdll.h
// Description:		Definition of class DockletDLL
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					This class is a wrapper class for the OdDockletInstance class
//					and handles additional settings only used by the GUI.
//					In some way, this class is a decorator.
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "dockletdll.h"
#include "helper_funcs.h"
#include "od_docklet.hpp"

DockletDLL::DockletsDLLMap DockletDLL::DockletDLLs;

DockletDLL::DockletDLL(docklets::OdDocklet* docklet, HMODULE hMod) :
	docklet(docklet),
	hMod(hMod),
	ref_count(1),
	version(0)
{
	// post: a new DockletDLL-object has been created

	WCHAR tmpFilename[MAX_PATH+1] = {0};
	GetModuleFileNameW(hMod,tmpFilename,sizeof(tmpFilename)/sizeof(tmpFilename[0]));
	lowercaseW(tmpFilename);

	wFilename = tmpFilename;

	if(docklet)
	{
		char name[1024] = {0};
		char author[1024] = {0};
		char comments[1024] ={0};

		docklet->GetInformation(name,author,&version,comments);

		wName = StringToWString(name);
		wAuthor = StringToWString(author);
		wComments = StringToWString(comments);
	}
}

DockletDLL::~DockletDLL()
{
	// post: this DockletDLL-object has been destructed
	if( m_wrp )
	{
		m_wrp->Release();
		m_wrp = NULL;
	}
	delete docklet;
	docklet = 0;
	FreeLibrary(hMod);
}

DockletDLL* DockletDLL::LoadDockletDLL(const WCHAR* filename)
{
	// pre:  filename is not 0 and filename is not bigger than MAX_PATH (not checked)
	// post: A pointer to the right DockletDLL has been returned if succesfull,
	//		 otherwise 0 has been returned
	if(!filename)return 0;

	// get an all lowercase filepath first
	WCHAR tmp[MAX_PATH+1] = {0};
	wcscpy(tmp,filename);
	lowercaseW(tmp);
	std::wstring dockletFile(tmp);

	// check if this dockletDLL is already in the list, if so return 
	// it immediately after adding a ref
	DockletsDLLMap::iterator iter = DockletDLLs.find(dockletFile);

	if(iter != DockletDLLs.end())
	{
		iter->second->AddRef();
		return iter->second;
	}
	else
	{
		// load the DLL as a module
		HMODULE mod = LoadLibrary(filename);
	
		// load the docklets functions
		docklets::OdDocklet* docklet = docklets::OdDocklet::LoadDockletFromDLL(mod);
	
		if(docklet)
		{
			// wrap the docklet up in a DockletDLL struct
			DockletDLL* dDLL = new DockletDLL(docklet,mod);
	
			// add the docklet to the Docklet DLL map
			DockletDLLs.insert(std::make_pair(dockletFile,dDLL));
			return dDLL;
		}
		else
		{
			return 0;
		}
	}
}

void DockletDLL::ReleaseAll()
{
	// post: all DockletDLLs are released
	for(DockletsDLLMap::iterator iter = DockletDLLs.begin(); iter != DockletDLLs.end(); ++iter)
	{
		delete iter->second;
	}
	DockletDLLs.clear();
}

ULONG __stdcall DockletDLL::AddRef()
{
	// post: this objects refcount is increased by 1 ans has been returned
	ref_count++;
	return ref_count;
}

ULONG __stdcall DockletDLL::Release()
{
	// post: this objects refcount is decreased by 1 and it is freed if
	//		 refcount is equal to 0. the ref_count has been returned.
	ref_count--;

	if(ref_count < 1)
	{
		DockletsDLLMap::iterator iter = DockletDLLs.find(wFilename);
		if(iter != DockletDLLs.end())
		{
			delete iter->second;
			DockletDLLs.erase(iter);
		}
		return 0;
	}
	else
		return ref_count;
}

docklets::OdDocklet* DockletDLL::GetDocklet()
{
	// post: docklet has been returned
	return docklet;
}

void DockletDLL::ScanDirectoryForDesklets(std::wstring dir, bool recursive)
// pre:  dir points to a valid directory
// post: a ref. for every desklet in folder dir has been added.
{
	WIN32_FIND_DATAW data;
	
	HANDLE h = FindFirstFileW(std::wstring(dir+L"*.dll").c_str(),&data);

	if(h!=INVALID_HANDLE_VALUE)
	{
		do
		{
			LoadDockletDLL(std::wstring(dir+data.cFileName).c_str());

		}while(FindNextFileW(h,&data));
	}


	FindClose(h);

	if(recursive)
	{
		h = FindFirstFileW(std::wstring(dir+L"*.*").c_str(),&data);
	
		if(h!=INVALID_HANDLE_VALUE)
		{
			do
			{	
				if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
				{
					if(wcscmp(data.cFileName,L".")!=0 && wcscmp(data.cFileName,L"..")!=0)
						ScanDirectoryForDesklets(std::wstring((dir+data.cFileName)+L"\\").c_str(),recursive);
				}
			}while(FindNextFileW(h,&data));
			FindClose(h);
		}
	}
}

DockletDLL::DockletsDLLMap::iterator DockletDLL::begin()
{
	return DockletDLLs.begin();
}

DockletDLL::DockletsDLLMap::iterator DockletDLL::end()
{
	return DockletDLLs.end();
}

ULONG DockletDLL::size()
{
	return DockletDLLs.size();
}

DockletDLL::DockletsDLLMap::iterator DockletDLL::find(const WCHAR* filename)
{
  return DockletDLLs.find(filename);
}
