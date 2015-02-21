// MainRootItemBehaviour.cpp: implementation of the CMainRootItemBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#include "MainRootItemBehaviour.h"

#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainRootItemBehaviour::CMainRootItemBehaviour(IShortcutDesklet* desklet) :
	CShellItemBehaviour(desklet),
	m_Desklet(desklet)
{

}

CMainRootItemBehaviour::~CMainRootItemBehaviour()
{

}

bool CMainRootItemBehaviour::CanHandlePidl(CPidl& pidl, std::wstring)
{
	CPidl myPCPidl;
	myPCPidl.SetFromSpecialFolderLocation(CSIDL_DRIVES);

	CPidl dtopPidl;
	dtopPidl.SetFromSpecialFolderLocation(CSIDL_DESKTOP);

	return myPCPidl == pidl || dtopPidl == pidl;
}

DWORD CMainRootItemBehaviour::OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter)
{
	return 0;
}
	
std::vector<std::wstring> CMainRootItemBehaviour::GetCustomSublabelItems()
{
	std::vector<std::wstring> items;
	items.push_back( L"%%\tpercentage sign");
	items.push_back( L"%#\tnewline");
	items.push_back( L"%u\tusername");
	items.push_back( L"%c\tcomputername");

	return items;
}

std::wstring CMainRootItemBehaviour::ParseSubLabel(CPidl& pidl,const std::wstring& format)
{
	std::wstringstream lbl;

	int count = format.length();
	int i(0);
	while(i<count)
	{
		// the next character indicates that the user wants to have
		// some data inserted here
		if(format[i]==L'%')
		{
			i++;

		
			// % -> percentage sign
			if(format[i]==L'%')
			{
				lbl << L"%";
			}
			// # -> newline character
			else if(format[i]==L'#')
			{
				lbl << L"\n"; 
			}
			// %number. Refers to the shelldetails id.
			else if(format[i] ==  L'u' || format[i] == L'U')
			{
				WCHAR buf[10024] = {0};
				DWORD dwSize(10024);
				GetUserNameW(buf,&dwSize);

				lbl << buf;
			}
			else if(format[i] ==  L'c' || format[i] == L'C')
			{
				WCHAR buf[10024] = {0};
				DWORD dwSize(10024);
				GetComputerNameW(buf,&dwSize);

				lbl << buf;
			}

		}
		// this character just needs to be copied to the output label
		else
		{
			lbl << format[i];
		}
		i++;
	}

	return lbl.str();
}