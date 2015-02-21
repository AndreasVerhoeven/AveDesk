// Tools.cpp: Implementierung der Klasse CTools.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Tools.hpp"
#include "Application.h"



extern CComObject<CApplication> *_App;
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////


CTools::CTools()
{
	USES_CONVERSION;

	WCHAR path[MAX_PATH] = {0};
	if(_App)
		wcscpy(path,_App->GetAveDeskUserPath().c_str());
	else
		GetDLLPathW(NULL, path, MAX_PATH);
	PathAppendW(path,L"tools.ini");
	ini = W2T(path);

    ReadData();
}

CTools::~CTools()
{

}

#define READTOOL(t) GetPrivateProfileString(sec, A2T(""#t), _T(""), tl.t.GetBuffer(MAX_PATH), MAX_PATH, ini); tl.t.ReleaseBuffer()
void CTools::ReadData()
{
	USES_CONVERSION;

    int i = 0;
    tools.clear();
    while(1)
    {
        CTool tl;
        CString sec;
        sec.Format(_T("Command%d"), i++);
        READTOOL(Display);
        if( !tl.Display.GetLength() )
            break;
        READTOOL(Execute);
        READTOOL(Arguments);
        READTOOL(Location);
        tools.push_back(tl);
    }
}

#define WRITETOOL(t) WritePrivateProfileString(sec, A2T(""#t), tl->t, ini);
void CTools::SaveAll()
{
	USES_CONVERSION;
    for( UINT i = 0; i < tools.size(); i++ )
    {
        CTool *tl = &tools[i];
        CString sec;
        sec.Format(_T("Command%d"), i);
        WRITETOOL(Display);
        WRITETOOL(Execute);
        WRITETOOL(Arguments);
        WRITETOOL(Location);
    }
}

// CTool
HWND CTool::m_hWnd = NULL;
void CTool::Call()
{
    Call(Arguments);
}

void CTool::Call(CString &newArgs)
{
	USES_CONVERSION;

    CString exec = Execute;
    if( !PathFileExists(exec) )
    {
        TCHAR *cExec = exec.GetBuffer(MAX_PATH+1);
        GetModuleFileName(NULL, cExec, MAX_PATH);
        PathRemoveFileSpec(cExec);
        PathAppend(cExec, Execute);
        exec.ReleaseBuffer();
        if(!PathFileExists(exec) )
        {
            CString msg, cap;
            msg.LoadString(IDS_NOSUCHFILE3);
            msg.Replace(_T("$1"), Execute);
            msg.Replace(_T("$2"), exec);
            msg.Replace(_T("$3"), _Tools.getIni());
            cap.LoadString(IDS_TOOLNOTFOUND);
            MessageBox(
                m_hWnd,
                msg, _T("Tool not found"), MB_OK | MB_ICONERROR );
            return;
        }
    }
    ShellExecute(m_hWnd, _T("open"), exec, newArgs, Location, SW_SHOW);
}

