// DeskletFinder.cpp: implementation of the CDeskletFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletFinder.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

#include "DockletDLL.h"
#include "Od_Docklet.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletFinder::CDeskletFinder(const std::wstring& mainDirName) :
	m_ThreadHandle(NULL),
	m_CallbackHwnd(NULL),
	m_Stop(FALSE),
	m_MainDirName(mainDirName),
	m_Parent(NULL)
{
//	m_Data.reserve(800);
	InitializeCriticalSection(&cs);
}

void CDeskletFinder::Lock()
{
	if(m_Parent != NULL)
		m_Parent->Lock();
	else
		EnterCriticalSection(&cs);
}

void CDeskletFinder::Unlock()
{
	if(m_Parent != NULL)
		m_Parent->Unlock();
	else 
		LeaveCriticalSection(&cs);
}

std::vector<CFoundDeskletEntry*>& CDeskletFinder::GetData()
{
	if(m_Parent != NULL)
		return m_Parent->GetData();
	else
		return m_Data;
}

CFoundDeskletEntry* CDeskletFinder::GetEntry(UINT index)
{
	CFoundDeskletEntry* entry = NULL;

	Lock();
	if(index < GetData().size())
		entry = GetData()[index];
	Unlock();

	return entry;
}


CDeskletFinder::~CDeskletFinder()
{
	Stop(INFINITE);
	Lock();
	for(size_t i = 0; i < m_Data.size(); ++i)
	//for(std::vector<CFoundDeskletEntry*>::iterator iter = m_Data.begin(); iter != m_Data.end(); ++iter)
	{
		CFoundDeskletEntry* entry = m_Data[i];//*iter;
		if(entry != NULL)
			delete entry;
	}

	m_Data.clear();

	Unlock();

	DeleteCriticalSection(&cs);
}

DWORD __stdcall CDeskletFinder::ThisThreadFunc(LPVOID lpData)
{
	CDeskletFinder* f = reinterpret_cast<CDeskletFinder*>(lpData);
	if(NULL == f)
		return 0;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	OleInitialize(NULL);

	DWORD dwRes = f->OnRun();

	CloseHandle(f->m_ThreadHandle);
	f->m_ThreadHandle = NULL;

	CoUninitialize();
	OleUninitialize();
	
	return dwRes;
}

void CDeskletFinder::CheckSkins(DockletDLL* dll)
{
	if(dll != NULL)
	{
		DeskletSkinEnumerator* se = DeskletSkinEnumerator::Create(dll);

		DeskletSkinInfo info;
		while(se->Next(&info))
		{
			se->AddRef();
			DeskletSkinInfo* si = new DeskletSkinInfo;
			*si = info;
			dll->AddRef();
			CFoundDeskletEntry* entry = new CFoundDeskletEntry(dll, se, si);
			Lock();
			UINT index = GetData().size();
			GetData().push_back(entry);
			Unlock();
			::PostMessageW(m_CallbackHwnd, m_Msg, (WPARAM)index, (LPARAM)this);
		}

		se->Release();
	}
}

void CDeskletFinder::CheckFolder(const std::wstring baseFolder)
{
	WCHAR findPath[MAX_PATH] = {0};
	wcscpy(findPath, baseFolder.c_str());
	PathAppendW(findPath, L"*.dll");

	WIN32_FIND_DATAW data = {0};
	HANDLE hFind = FindFirstFileW(findPath, &data);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			WCHAR file[MAX_PATH] = {0};
			wcscpy(file, baseFolder.c_str());
			PathAppendW(file, data.cFileName);
			//MessageBox(NULL, file, L"test", MB_ICONINFORMATION | MB_OK);
			DockletDLL* dll = DockletDLL::LoadDockletDLL(file);
			if(dll != NULL)
			{
			
				DWORD flags(0);
				dll->GetDocklet()->GetFlags(NULL, &flags);
				dll->AddRef();
				if(!(flags & DESKLET_ONLY_INSTANTIATE_SKINS))
				{
					CFoundDeskletEntry* entry = new CFoundDeskletEntry(dll, NULL, NULL);
					Lock();
					UINT index = GetData().size();
					GetData().push_back(entry);
					Unlock();
					::PostMessageW(m_CallbackHwnd, m_Msg, (WPARAM)index, (LPARAM)this);
				}
				else
				{
					dll->Release();
				}

				CheckSkins(dll);
				dll->Release();
			}
			
		}while(FindNextFileW(hFind, &data) && !m_Stop);

		FindClose(hFind);
	}
}


DWORD CDeskletFinder::OnRun()
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path, _App->GetAveDeskDataDir().c_str());
	PathAppendW(path, m_MainDirName.c_str());

	WCHAR findPath[MAX_PATH] = {0};
	wcscpy(findPath, path);
	PathAppendW(findPath, L"*.*");

	CheckFolder(path);

	WIN32_FIND_DATAW data = {0};
	HANDLE hFind = FindFirstFileW(findPath, &data);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(data.cFileName[0] != L'.' && data.cFileName[1] != L'.')
			{
				if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
				{
					WCHAR file[MAX_PATH] = {0};
					wcscpy(file, path);
					PathAppendW(file, data.cFileName);
					CheckFolder(file);
				}
			}
			
		}while(FindNextFileW(hFind, &data) && !m_Stop);

		FindClose(hFind);
	}

	return 0;
}

void CDeskletFinder::Start(HWND callbackHwnd, UINT msg)
{
	m_CallbackHwnd = callbackHwnd;
	m_Stop = FALSE;
	m_Msg = msg;
	DWORD dummy(0);
	m_ThreadHandle = CreateThread(NULL,0,ThisThreadFunc,reinterpret_cast<LPVOID>(this),0,&dummy);
}

void CDeskletFinder::Stop(DWORD timeOut)
{
	m_Stop = TRUE;
	WaitForSingleObject(m_ThreadHandle, timeOut);
}
