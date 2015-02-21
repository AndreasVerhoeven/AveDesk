// DriveBehaviour.cpp: implementation of the CDriveBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#include "DriveBehaviour.h"
#include <string>
#include <sstream>
#include <iomanip>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveBehaviour::CDriveBehaviour(IShortcutDesklet* desklet) : 
	CShellItemBehaviour(desklet),
	m_IsInit(false)
{

}

CDriveBehaviour::~CDriveBehaviour()
{

}

DWORD CDriveBehaviour::OnMonitorMessage(CPidl& pidl,LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter)
{
	switch (event)
    {
		case SHCNE_FREESPACE           : //0x00040000L
			GetDriveInfo();
			// FALL THROUGH

		//case SHCNE_CREATE              : //0x00000002L
		//case SHCNE_DELETE              : //0x00000004L
		//case SHCNE_MKDIR               : //0x00000008L
		//case SHCNE_RMDIR               : //0x00000010L
		case SHCNE_MEDIAINSERTED       : //0x00000020L
		case SHCNE_MEDIAREMOVED        : //0x00000040L
		case SHCNE_DRIVEREMOVED        : //0x00000080L
		case SHCNE_DRIVEADD            : //0x00000100L
		case SHCNE_NETSHARE            : //0x00000200L
		case SHCNE_NETUNSHARE          : //0x00000400L
		case SHCNE_ATTRIBUTES          : //0x00000800L
		//case SHCNE_UPDATEDIR           : //0x00001000L
		case SHCNE_UPDATEITEM          : //0x00002000L
		case SHCNE_SERVERDISCONNECT    : //0x00004000L
		//case SHCNE_UPDATEIMAGE         : //0x00008000L
		case SHCNE_DRIVEADDGUI         : //0x00010000L
		//case SHCNE_RENAMEFOLDER        : //0x00020000L

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

void CDriveBehaviour::GetDriveInfo()
{
	ULARGE_INTEGER freeCaller;

	SHGetDiskFreeSpaceExW(GetShortcutDesklet()->GetPidl().GetFullParseDisplayName().c_str(),&freeCaller,&m_TotalBytes,&m_FreeBytes);

	m_IsInit = true;	
}

bool CDriveBehaviour::CanHandlePidl(CPidl &pidl, std::wstring)
{
	CPidl drivesPidl;
	drivesPidl.SetFromSpecialFolderLocation(CSIDL_DRIVES);

	return ILIsParent(drivesPidl,pidl,TRUE) == TRUE;
}


std::vector<std::wstring> CDriveBehaviour::GetCustomSublabelItems()
{
	std::vector<std::wstring> items;
	items.push_back( L"%%\tpercentage sign");
	items.push_back( L"%#\tnewline");
	items.push_back( L"%u\tusername");
	items.push_back( L"%c\tcomputername");
	items.push_back( L"");

	items.push_back( L"%t\ttotal diskspace");
	items.push_back( L"%f\tfree diskspace left");
	items.push_back( L"%s\tused diskspace");
	items.push_back( L"%pF\tprocentage of free diskspace");
	items.push_back( L"%pU\tpercentage of used diskspace");

	return items;
}


bool CDriveBehaviour::ParseTokenAt(CPidl &pidl, const std::wstring &format, std::wstringstream& output, int& index)
{
	// f -> total bytes
	if(format[index]==L't' || format[index] == L'T')
	{
		if(!m_IsInit)
			GetDriveInfo();

		WCHAR conv[100] = {0};
		StrFormatByteSizeW(m_TotalBytes.QuadPart,conv,sizeof(conv)/sizeof(conv[0]));
		output << conv;
	}
	// t -> free bytes
	else if(format[index]==L'f' || format[index]==L'F')
	{
		if(!m_IsInit)
			GetDriveInfo();

		WCHAR conv[100] = {0};
		StrFormatByteSizeW(m_FreeBytes.QuadPart,conv,sizeof(conv)/sizeof(conv[0]));
		output << conv;
	}
	// s -> used
	else if(format[index]==L's' || format[index]==L'S')
	{
		if(!m_IsInit)
			GetDriveInfo();

		ULARGE_INTEGER usedBytes;
		usedBytes.QuadPart = m_TotalBytes.QuadPart - m_FreeBytes.QuadPart;

		WCHAR conv[100] = {0};
		StrFormatByteSizeW(usedBytes.QuadPart,conv,sizeof(conv)/sizeof(conv[0]));
		output << conv;
	}
	// pnumber. Refers to the shelldetails id.
	else if(format[index] ==  L'p' || format[index] == L'P' && index < format.length()-1)
	{
		if(!m_IsInit)
			GetDriveInfo();

		index++;

		float percentageFree = 0.0f;

		if(m_TotalBytes.QuadPart > 0)
		{
			if(format[index] == 'u' || format[index] == 'U')
			{
				ULARGE_INTEGER usedBytes;
				usedBytes.QuadPart = m_TotalBytes.QuadPart - m_FreeBytes.QuadPart;

				percentageFree	= (float) (static_cast<float>(static_cast<signed __int64>(usedBytes.QuadPart)) / static_cast<float>(static_cast<signed __int64>(m_TotalBytes.QuadPart)) * 100);
			}
			else if(format[index] == 'f' || format[index] == 'F')
			{
				percentageFree	= (float) (static_cast<float>(static_cast<signed __int64>(m_FreeBytes.QuadPart)) / static_cast<float>(static_cast<signed __int64>(m_TotalBytes.QuadPart)) * 100);
			}
		}
		

		output << std::setprecision(3) << percentageFree;
	}
	else
	{
		return CShellItemBehaviour::ParseTokenAt(pidl,format,output,index);
	}

	return true;
}

std::wstring CDriveBehaviour::ParseSubLabel(CPidl &pidl, const std::wstring &format)
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