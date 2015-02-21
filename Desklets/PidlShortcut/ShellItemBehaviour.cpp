// ShellItemBehaviour.cpp: implementation of the CShellItemBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#include "ShellItemBehaviour.h"

#include <sstream>

#include <windows.h>

#include "resource.h"

#include "../DeskletSDK.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShellItemBehaviour::CShellItemBehaviour(IShortcutDesklet* desklet) :
	m_Desklet(desklet)
{

}

CShellItemBehaviour::~CShellItemBehaviour()
{

}

DWORD CShellItemBehaviour::OnMonitorMessage(CPidl& pidl,LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter)
{
	switch (event)
    {
		case SHCNE_CREATE              : //0x00000002L
		case SHCNE_DELETE              : //0x00000004L
		case SHCNE_MKDIR               : //0x00000008L
		case SHCNE_RMDIR               : //0x00000010L
		//case SHCNE_MEDIAINSERTED       : //0x00000020L
		//case SHCNE_MEDIAREMOVED        : //0x00000040L
		//case SHCNE_DRIVEREMOVED        : //0x00000080L
		//case SHCNE_DRIVEADD            : //0x00000100L
		//case SHCNE_NETSHARE            : //0x00000200L
		//case SHCNE_NETUNSHARE          : //0x00000400L
		case SHCNE_ATTRIBUTES          : //0x00000800L
		case SHCNE_UPDATEDIR           : //0x00001000L
		case SHCNE_UPDATEITEM          : //0x00002000L
		//case SHCNE_SERVERDISCONNECT    : //0x00004000L
		//case SHCNE_UPDATEIMAGE         : //0x00008000L
		//case SHCNE_DRIVEADDGUI         : //0x00010000L
		case SHCNE_RENAMEFOLDER        : //0x00020000L
		case SHCNE_FREESPACE           : //0x00040000L
		//UpdateItems();
			
			//if(oldNumOfItems.QuadPart == 0 || m_NumItems.QuadPart == NULL)
				m_Desklet->NotifySublabelChange();
		break;

		case SHCNE_UPDATEIMAGE         : //0x00008000L
			m_Desklet->NotifyIconChange();

        break;
    }

	return 0;
}

bool CShellItemBehaviour::CanHandlePidl(CPidl &pidl, std::wstring)
{
	return true;
}

std::vector<std::wstring> CShellItemBehaviour::GetCustomSublabelItems()
{
	std::vector<std::wstring> items;
	items.push_back( L"%%\tpercentage sign");
	items.push_back( L"%#\tnewline");
	items.push_back( L"%u\tusername");
	items.push_back( L"%c\tcomputername");

	return items;
}

bool CShellItemBehaviour::ParseTokenAt(CPidl &pidl, const std::wstring &format, std::wstringstream& output, int& index)
{
	// % -> percentage sign
	if(format[index]==L'%')
	{
		output << L"%";
	}
	// # -> newline character
	else if(format[index]==L'#')
	{
		output << L"\n"; 
	}
	// %number. Refers to the shelldetails id.
	else if(format[index] ==  L'u' || format[index] == L'U')
	{
		WCHAR buf[10024] = {0};
		DWORD dwSize(10024);
		GetUserNameW(buf,&dwSize);

		output << buf;
	}
	else if(format[index] ==  L'c' || format[index] == L'C')
	{
		WCHAR buf[10024] = {0};
		DWORD dwSize(10024);
		GetComputerNameW(buf,&dwSize);

		output << buf;
	}
	// %number. Refers to the shelldetails id.
	else if(format[index] >= L'0' && format[index] <= L'9')
	{
		// get the number first
		WCHAR c = format[index];
		WCHAR c2 = 0;

		int count = format.length();


		if(index<count-1 && format[index+1] >= L'0' && format[index+1] <= L'9')
		{
			index++;
			c2 = format[index];
		}

		int num(0);

		if(c2!=0)
			num = int(c - L'0') * 10 + int(c2 - L'0');
		else
			num = int(c - L'0');


		std::wstring str;

		if( SUCCEEDED(pidl.GetShellDetailString(str,num)) )
			output << str;
	}
	else
	{
		return false; // we couldn't parse it
	}

	return true;
}

std::wstring CShellItemBehaviour::ParseSubLabel(CPidl &pidl, const std::wstring &format)
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

			std::wstring parsed;

			ParseTokenAt(pidl,format,lbl,i);
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

std::wstring CShellItemBehaviour::GetCustomKeyName()
{
	return std::wstring(L"Normal Image");
}

std::map<std::wstring,std::wstring> CShellItemBehaviour::GetMapOfImageNames()
{
	std::map<std::wstring,std::wstring> map;

	map[L"Normal Image"] = L"";

	return map;
}