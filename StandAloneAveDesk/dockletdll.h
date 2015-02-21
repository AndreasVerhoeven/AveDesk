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

#ifndef DOCKLET_DLL_H__590683292
#define DOCKLET_DLL_H__590683292

#include "avedesk.h"
#include <map>
#include <string>
#include <windows.h>
#include "od_docklet.hpp"
#include "helper_funcs.h"
#include "deskletinfo.h"

class DockletDLL : 
  public CoElement<DockletDLL, CDeskletInfo, IDeskletInfo>
{
public:
		typedef std::map<std::wstring,DockletDLL*> DockletsDLLMap;
private:
	HMODULE hMod;
	docklets::OdDocklet* docklet;
	LONG ref_count;

	std::wstring wFilename;
	std::wstring wName;
	std::wstring wAuthor;
	std::wstring wComments;
	int version;

  DockletDLL(docklets::OdDocklet* docklet, HMODULE hMod);
	~DockletDLL();

public:
  //!herd: made map public for access from CDeskletInfo
	static DockletsDLLMap DockletDLLs;

	static DockletDLL* LoadDockletDLL(const WCHAR* filename);
	static void ReleaseAll();

	static void ScanDirectoryForDesklets(std::wstring dir, bool recursive);
	static DockletsDLLMap::iterator begin();
	static DockletsDLLMap::iterator end();
	static DockletsDLLMap::iterator find(const WCHAR* filename);
	static ULONG  size();

	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	docklets::OdDocklet* GetDocklet();


	const WCHAR* GetFileName()const	{return wFilename.c_str();};
	const WCHAR* GetName()const		{return wName.c_str();};
	const WCHAR* GetAuthor()const	{return wAuthor.c_str();};
	const WCHAR* GetComments()const	{return wComments.c_str();};
	int			GetVersion()const	{return version;};
};

#endif//DOCKLET_DLL_H__590683292