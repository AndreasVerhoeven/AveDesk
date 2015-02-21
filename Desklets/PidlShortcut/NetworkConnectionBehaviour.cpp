// NetworkConnectionBehaviour.cpp: implementation of the CNetworkConnectionBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#include "NetworkConnectionBehaviour.h"
#include "shellfolder.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetworkConnectionBehaviour::CNetworkConnectionBehaviour(IShortcutDesklet* desklet) :
	CShellItemBehaviour(desklet)

{

}

CNetworkConnectionBehaviour::~CNetworkConnectionBehaviour()
{

}

DWORD CNetworkConnectionBehaviour::OnMonitorMessage(CPidl& pidl,LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter)
{
	switch (event)
    {
		/*
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
		*/

		case SHCNE_UPDATEIMAGE         : //0x00008000L
			m_Desklet->NotifyIconChange();
			m_Desklet->NotifySublabelChange();

        break;
    }

	return 0;
}

std::wstring CNetworkConnectionBehaviour::ParseSubLabel(CPidl &pidl, const std::wstring &format)
{
	CPidl updatedPidl;
	if( SUCCEEDED(pidl.CopyItemID(updatedPidl.m_Pidl)) )
	{
		CanHandlePidl(updatedPidl,L"");
		return CShellItemBehaviour::ParseSubLabel(updatedPidl,format);
	}
	else
	{
		return CShellItemBehaviour::ParseSubLabel(pidl,format);
	}

}

bool CNetworkConnectionBehaviour::CanHandlePidl(CPidl& pidl, std::wstring path)
{
	CPidl connPidl;

	/*
	std::wstring testPath = L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\::{21EC2020-3AEA-1069-A2DD-08002B30309D}\\::{7007ACC7-3202-11D1-AAD2-00805FC1270E}\\";
	testPath += path;

	bool res = connPidl.SetFromPath(testPath);

	if(res)
		pidl.SetFromPath(testPath);
	*/
	
	
	if( FAILED(connPidl.SetFromSpecialFolderLocation(CSIDL_CONNECTIONS)) )
		return false;

	bool ret = ILIsParent(connPidl,pidl,TRUE) == TRUE;

	//return ILIsParent(connPidl,pidl,TRUE) ? true : false;
	

	if(ret)
	{

		CShellFolder folder;
		if( FAILED(folder.SetFromPIDL(connPidl)) )
			return false;

		CComPtr<IEnumIDList> enumerator;
		if( FAILED(folder.m_ShellFolder->EnumObjects(NULL,SHCONTF_NONFOLDERS,&enumerator)) )
			return false;

		LPITEMIDLIST p = {0};
		while( SUCCEEDED(enumerator->Next(1,&p,NULL)) )
		{	
			USES_CONVERSION;
	
			CPidl cp;
			cp.SetFromPIDL(p);
	
			STRRET str = {0};
			TCHAR szDisplayName[MAX_PATH+2] = {0};
	
			if( FAILED(folder.m_ShellFolder->GetDisplayNameOf(p, SHGDN_NORMAL, &str))  )
				return false;

			StrRetToBuf(&str, 
				    p, 
					szDisplayName, 
					sizeof(szDisplayName)/sizeof((szDisplayName)[0]));

			std::wstring name = T2W(szDisplayName);

			if (pidl.GetFriendlyName() == name)
			{
				CPidl connPidl2;
				connPidl2.SetFromSpecialFolderLocation(CSIDL_CONNECTIONS);
				LPITEMIDLIST rawPidl = CPidl::Append(connPidl2,p);
				pidl.SetFromPIDL( rawPidl );

				return true;
			}
			/*else
			{
				cp.Detach();
			}*/

			//MessageBoxW(NULL,cp.ToString().c_str(),0,0);
			//if( name == path )
			//{
			//	
			//}
			//if(name == path)
			//{	
				//MessageBoxW(NULL,name.c_str(),0,0);
				//pidl.SetFromPIDL( ILCombine(connPidl,cp) );

				//	return true;
			//}
		
		}
	}
	return ret;
}