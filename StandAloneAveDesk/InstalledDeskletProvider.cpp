#include "stdafx.h"
#include "InstalledDeskletProvider.h"
#include "InstalledDeskletItem.h"

CInstalledDeskletProvider::CInstalledDeskletProvider(void) : isRunning(FALSE)
{
	AddDirectory(L"Desklets");
	AddDirectory(L"Docklets");
}

CInstalledDeskletProvider::~CInstalledDeskletProvider(void)
{
	Stop();
	DestroyWindow();
	Clear();
}


void CInstalledDeskletProvider::Start()
{
	if(NULL == m_hWnd)
		Create(HWND_MESSAGE);

	isRunning = TRUE;

	for(std::vector<FinderPtrType>::iterator iter = finders.begin(); iter != finders.end(); ++iter)
	{
		FinderPtrType& finder = *iter;
		finder->Start(m_hWnd, WM_USER_FIND_DESKLET_CALLBACK_MSG);
	}
}

void CInstalledDeskletProvider::AddDirectory(const std::wstring& dir)
{
	FinderPtrType finder(new CDeskletFinder(dir));
	finders.push_back(finder);	
	if(isRunning)
		finder->Start(m_hWnd, WM_USER_FIND_DESKLET_CALLBACK_MSG);

}

void CInstalledDeskletProvider::Stop()
{
	isRunning = FALSE;

	// notify to stop first
	for(std::vector<FinderPtrType>::iterator iter = finders.begin(); iter != finders.end(); ++iter)
	{
		FinderPtrType& finder = *iter;
		finder->Stop(0);
	}

	// wait until stopped
	for(std::vector<FinderPtrType>::iterator iter = finders.begin(); iter != finders.end(); ++iter)
	{
		FinderPtrType& finder = *iter;
		finder->Stop(INFINITE);
	}
}

LRESULT CInstalledDeskletProvider::OnFind(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT index = static_cast<UINT>(wParam);
	CDeskletFinder* finder = reinterpret_cast<CDeskletFinder*>(lParam);
	if(finder != NULL)
	{
		CFoundDeskletEntry* entry = finder->GetEntry(index);
		if(entry != NULL)
		{
			CDeskletItem* item = new CInstalledDeskletItem(entry);
			items.push_back(item);
			NotifyAll(true, reinterpret_cast<LPARAM>(item), 0L);
		}
	}

	return 0;
}