// FolderBehaviour.cpp: implementation of the CFolderBehaviour class.
//
//////////////////////////////////////////////////////////////////////

#include "FolderBehaviour.h"

#include "ShellFolder.h"
#include "Shellhelper.h"

#include <vector>
#include <string>
#include <memory>
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFolderBehaviour::CFolderBehaviour(IShortcutDesklet* desklet) :
	CShellItemBehaviour(desklet),
	m_Desklet(desklet),
	m_IsDirty(true),
	m_Stop(false),
	m_ThreadHandle(NULL),
	m_WasRecursive(false),
	m_WasCountSize(false),
	m_TempUpdate(false),
	m_DropType(DropNone),
	m_DropOnSameDrive(false)
{
	m_NumOfFiles.QuadPart = 0;
	m_NumOfFolders.QuadPart = 0;
	m_TotalFileSize.QuadPart = 0;
}

CFolderBehaviour::~CFolderBehaviour()
{
	EndUpdating();
}

void CFolderBehaviour::UpdateItems(bool countFileSize, bool recursive, std::wstring path)
{
	DWORD dwStart = GetTickCount();

	WIN32_FIND_DATAW data = {0};

	if(path[path.length()-1] != L'\\' || path[path.length()-1] != L'/')
		path += L"\\";

	HANDLE h = FindFirstFileW(std::wstring(path + L"*.*").c_str(),&data);
	if( h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if( wcscmp(data.cFileName,L".") != 0 && wcscmp(data.cFileName,L"..") != 0 )
			{
				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					m_NumOfFolders.QuadPart++;
					
					if(recursive)
						UpdateItems(countFileSize,recursive,path + data.cFileName + L"\\");
				}
				else
				{
					m_NumOfFiles.QuadPart++;

					if(countFileSize)
					{
							DWORD fhw = data.nFileSizeHigh;
							DWORD f = data.nFileSizeLow;
							if(f != 0xFFFFFFFF)
							{
								ULARGE_INTEGER uiSize =  { f + float(fhw)*(float(0xFFFFFFFF)+1) };
								m_TotalFileSize.QuadPart+= uiSize.QuadPart;
							}
					}
				}

				DWORD dwNew = GetTickCount();
				if(dwNew - dwStart < 0 || dwNew - dwStart > 5000 )
				{
					dwStart = dwNew;
					m_TempUpdate = true;
					m_Desklet->NotifySublabelChange();
					m_TempUpdate = false;
				}
			}

		}while(!m_Stop && FindNextFileW(h,&data) );

		FindClose(h);
	}
}

DWORD __stdcall CFolderBehaviour::UpdateThread(LPVOID lpData)
{
	std::auto_ptr<UpdateParams> params( reinterpret_cast<UpdateParams*>(lpData) );
	
	if(params.get() && params->obj)
	{
		CFolderBehaviour* obj = params->obj;
		obj->UpdateItems(params->countFileSize,params->recursive,params->path);

		CloseHandle(obj->m_ThreadHandle);
		obj->m_ThreadHandle = NULL;

		if(obj->m_Stop == false)
		{
			obj->m_IsDirty = false;
			obj->m_TempUpdate = true;
			obj->m_Desklet->NotifySublabelChange();
			obj->m_TempUpdate = false;
		}

	}

	ExitThread(0);

	return TRUE;
}

void CFolderBehaviour::BeginUpdating(bool countFileSize, bool recursive, std::wstring path)
{
	if(m_ThreadHandle != NULL)
		EndUpdating();

	m_WasRecursive = recursive;
	m_WasCountSize = countFileSize;

	UpdateParams* params = new UpdateParams;
	params->countFileSize = countFileSize;
	params->obj = this;
	params->path = path;
	params->recursive = recursive;

	m_NumOfFiles.QuadPart = 0;
	m_NumOfFolders.QuadPart = 0;
	m_TotalFileSize.QuadPart = 0;

	m_Stop = false;
	DWORD dummy(0);
	m_ThreadHandle = CreateThread(NULL,0,UpdateThread,reinterpret_cast<void*>(params),0,&dummy);
	SetThreadPriority(m_ThreadHandle,THREAD_PRIORITY_IDLE);
}

void CFolderBehaviour::EndUpdating()
{
	if(m_ThreadHandle)
	{
		m_Stop = true;
		WaitForSingleObject(m_ThreadHandle,INFINITE);
		
		m_ThreadHandle = NULL;
	}
}

DWORD CFolderBehaviour::OnMonitorMessage(CPidl& pidl, LPARAM event, LPITEMIDLIST pidlBefore, LPITEMIDLIST pidlAfter)
{
	switch (event)
    {
		case SHCNE_CREATE              : //0x00000002L
		case SHCNE_DELETE              : //0x00000004L
		case SHCNE_MKDIR               : //0x00000008L
		case SHCNE_RMDIR               : //0x00000010L
			m_IsDirty = true;

			m_Desklet->NotifySublabelChange();
		break;

		/*case SHCNE_UPDATEIMAGE         : //0x00008000L
			m_Desklet->NotifyIconChange();
        break;
		*/
    }

	return 0;
}

std::wstring CFolderBehaviour::GetCustomKeyName()
{
	return L"Normal Image";
}

std::map<std::wstring,std::wstring> CFolderBehaviour::GetMapOfImageNames()
{
	std::map<std::wstring,std::wstring> map;
	map[L"Normal Image"] = L"";
	map[L"Opened Folder"] = L"";

	return map;
}

std::vector<std::wstring> CFolderBehaviour::GetCustomSublabelItems()
{
	std::vector<std::wstring> items;
	items.push_back( L"%%\tpercentage sign");
	items.push_back( L"%#\tnewline");
	items.push_back( L"%u\tusername");
	items.push_back( L"%c\tcomputername");
	items.push_back( L"%r\talso count files in subfolders");
	items.push_back( L"%s\ttotal size of all files");
	items.push_back( L"%n\ttotal number of items");
	items.push_back( L"%t\ttotal number of items");
	items.push_back( L"%f\ttotal number of files");
	items.push_back( L"%d\ttotal number of directories");

	return items;
}

std::wstring CFolderBehaviour::ParseSubLabel(CPidl& pidl,const std::wstring& format)
{
	std::wstringstream lbl;

	if(!m_TempUpdate)
	{
		if( (m_IsDirty && (	format.find(L"%s") != std::wstring::npos || 
							format.find(L"%d") != std::wstring::npos ||
							format.find(L"%f") != std::wstring::npos ||
							format.find(L"%t") != std::wstring::npos
						)
	
			) || (m_WasRecursive != (format.find(L"%r") != std::wstring::npos)) ||
			(m_WasCountSize != (format.find(L"%s") != std::wstring::npos) )
		)
		{
			BeginUpdating(format.find(L"%s") != std::wstring::npos,format.find(L"%r") != std::wstring::npos,pidl.GetFullParseDisplayName() );
		}
	}

	int i=0;
	int count( format.length() );
	while(i<count)
	{
		// the next character indicates that the user wants to have
		// some data inserted here
		if(format[i]==L'%')
		{
			i++;

			if(format[i]=='s')
			{
				ULARGE_INTEGER size;
				size.QuadPart = m_TotalFileSize.QuadPart;

				WCHAR conv[100] = {0};
				StrFormatByteSizeW(size.QuadPart,conv,sizeof(conv)/sizeof(conv[0]));
				lbl << conv;
				//std::wstring postfix =L" bytes";
				//if(size.QuadPart >= 1024){size.QuadPart/=1024;postfix=L" kB";};
				//if(size.QuadPart >= 1024){size.QuadPart/=1024;postfix=L" MB";};
				//if(size.QuadPart >= 1024){size.QuadPart/=1024;postfix=L" GB";};

				//lbl << static_cast<DWORD>(size.QuadPart) << postfix;
			}
			else if(format[i]=='d')
			{
				lbl << static_cast<DWORD>(m_NumOfFolders.QuadPart);
			}
			else if(format[i]=='f')
			{
				lbl << static_cast<DWORD>(m_NumOfFiles.QuadPart);
			}
			else if(format[i]=='t' || format[i]==L'n')
			{
				lbl << static_cast<DWORD>(m_NumOfFolders.QuadPart + m_NumOfFiles.QuadPart);
			}
			else
			{
				CShellItemBehaviour::ParseTokenAt(pidl,format,lbl,i);
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

bool CFolderBehaviour::CanHandlePidl(CPidl& pidl, std::wstring)
{
	CPidl cp;
	cp.SetFromPIDL(pidl);

	ULONG attr(0);//SFGAO_FOLDER);
	if( SUCCEEDED(cp.GetAtrributesOf(&attr)) )
	{
		cp.Detach();
		return ( (attr & SFGAO_FOLDER) != 0 );
	}

	cp.Detach();

	return false;

}


BOOL CFolderBehaviour::DragEnter(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect)
{
	std::vector<std::wstring> filenames;
	int length(0);

	if(SUCCEEDED(CShellHelper::GetListOfFileNamesFromIDataObject(obj,filenames,length)) )
	{
		if(filenames.size() > 0)
		{
			m_DropOnSameDrive = filenames[0][0] == m_Desklet->GetPidl().GetFullParseDisplayName()[0];
			if(m_DropOnSameDrive )
				m_DropType = GetKeyState(VK_CONTROL) < 0 ? DropMove : DropCopy ;
			else
				m_DropType = GetKeyState(VK_SHIFT) < 0 ? DropCopy : DropMove;
		}
		else
		{
			m_DropType = DropNone;
		}
	}
	else
	{
		m_DropType = DropNone;
	}
	
	if(m_DropType != DropNone)
		m_Desklet->ImageFadeIn(L"Opened Folder");

	return FALSE;
}

BOOL CFolderBehaviour::DragOver(DWORD keys, POINTL pnt, DWORD *effect)
{
	if(m_DropType != DropNone)
	{
		if(m_DropOnSameDrive )
			m_DropType = GetKeyState(VK_CONTROL) < 0 ? DropMove : DropCopy ;
		else
			m_DropType = GetKeyState(VK_SHIFT) < 0 ? DropCopy : DropMove;
	}

	return FALSE;
}

BOOL CFolderBehaviour::Drop(IDataObject *obj, DWORD keys, POINTL pnt, DWORD *effect)
{
	if(m_DropType != DropNone)
		m_Desklet->ImageFadeOut();

	return FALSE;
}

BOOL CFolderBehaviour::DragLeave()
{
	if(m_DropType != DropNone)
		m_Desklet->ImageFadeOut();

	m_DropType = DropNone;

	return FALSE;
}