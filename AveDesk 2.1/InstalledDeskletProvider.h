#pragma once
#include "deskletprovider.h"
#include "DeskletFinder.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

class CInstalledDeskletProvider :
	public CDeskletProvider,
	public CWindowImpl<CInstalledDeskletProvider>
{
public:
	static const UINT WM_USER_FIND_DESKLET_CALLBACK_MSG = WM_USER + 956;
protected:
	BOOL isRunning;

	typedef boost::shared_ptr<CDeskletFinder> FinderPtrType;
	std::vector<FinderPtrType> finders;

public:
	CInstalledDeskletProvider(void);
	virtual ~CInstalledDeskletProvider(void);



	BEGIN_MSG_MAP_EX(CInstalledDeskletProvider)
		MESSAGE_HANDLER(WM_USER_FIND_DESKLET_CALLBACK_MSG,OnFind);
		//CHAIN_MSG_MAP(CWindowImpl<CInstalledDeskletProvider>)
	END_MSG_MAP()

	LRESULT OnFind(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:

	virtual void Start();
	virtual void Stop();
	virtual void AddDirectory(const std::wstring& dir);
};
