// AveInstFontAction.cpp: implementation of the CAveInstFontAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstFontAction.h"
#include "AveInstallerEnvironment.h"
#include "pidl.h"
#include "AveInstSuspendedDesklets.h"
#include "AveInstSuspendedDeskletsList.h"
#include "Docklet.h"

extern GdiplusStartupInput g_gdiplusStartupInput;
extern ULONG_PTR g_gdiplusToken;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstFontAction::CAveInstFontAction()
{

}

CAveInstFontAction::~CAveInstFontAction()
{

}

HRESULT CAveInstFontAction::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_INVALIDARG);
		
	hRes = m_Dir.LoadFromNode(node);

	return hRes;
}

bool CAveInstFontAction::FileOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	if(PathFileExists(dstPath.c_str()))
	{
		CPidl fontsPidl;
		fontsPidl.SetFromSpecialFolderLocation(CSIDL_FONTS);
		if(fontsPidl.IsValid())
		{
			WCHAR fontsPath[MAX_PATH] = {0};
			wcscpy(fontsPath,fontsPidl.GetFullParseDisplayName().c_str());
			const WCHAR* fileName = PathFindFileNameW(srcPath.c_str());
			PathAppendW(fontsPath,fileName);

			CopyFile(srcPath.c_str(), fontsPath, FALSE);

			SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0L, 0L);


			// suspend all desklets so we can shutdown and restart GDI+
			CAveInstSuspendedDeskletsList* sdl = env.GetSuspendedDeskletsList();
			if(NULL == sdl)
				return true;

			std::vector<Docklet*> docklets;

			EnterCriticalSection(&Docklet::cs);
			for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
			{
				Docklet* d = iter->second;
				if(d != NULL)
					docklets.push_back(iter->second);
			}
			LeaveCriticalSection(&Docklet::cs);

			std::vector<HANDLE> handles;
			for(std::vector<Docklet*>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
			{
				Docklet* d = *d_iter;
				if(d != NULL)
				{
					CAveInstSuspendedDesklets& sd = sdl->GetSuspendedDesklets(d->GetRelativeDLLFile());
					//sd.AddCount();
					handles.push_back(d->GetThreadHandle());
					sd.Add(d);
				}
			}

			if(handles.size() > 0)
				DWORD dw = WaitForMultipleObjects(handles.size(), &handles[0], TRUE, 30000);

			// shut down GDI+ and restart it to make GDI+ reconize the newly installed font
			GdiplusShutdown(g_gdiplusToken);
			GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);

		}
	}

	return true;
}

bool CAveInstFontAction::DirectoryOperation(CAveInstallerEnvironment& env, const std::wstring& srcPath, const std::wstring& dstPath)
{
	return PathFileExists(srcPath.c_str()) == TRUE;
}

HRESULT CAveInstFontAction::DoAction(CAveInstallerEnvironment& env)
{
	CAveInstallerEnvironment tmpEnv = env; // use a temporary environment
	m_Dir.PerformOperation(*this,tmpEnv,tmpEnv.GetAveDeskEnvironment().GetInstallPath(),tmpEnv.GetAveDeskEnvironment().GetInstallPath());

	return S_OK;
}