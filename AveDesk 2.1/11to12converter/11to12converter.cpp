// 11to12converter.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "../XMLSaver.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


void IniToXML(ISaver& saver, const WCHAR* ini, const WCHAR* group, const WCHAR* iniKey, const WCHAR* defVal, const WCHAR* xmlKey)
{
	WCHAR buf[1024] = {0};
	GetPrivateProfileString(group,iniKey,defVal,buf,1024,ini);
	saver.SaveString(buf,xmlKey);
}

#define INI2XML(i,d,x) \
	IniToXML(saver,iniFile,iniGroup,i,d,x);

void ConvertSettings()
{
	WCHAR iniFile[MAX_PATH] = {0};
	WCHAR iniGroup[] = L"Settings");

	CXMLSaver saver(L"AveDesk");
	saver.AddNode("GlobalSettings");

	INI2XML(L"MoveInDelayMs",L"200",L"MoveDelayInMs");
	INI2XML(L"ClickMode",L"0",L"ClickMode");
	INI2XML(L"GridX",L"128",L"GridX");
	INI2XML(L"GridY",L"128",L"GridY");
	INI2XML(L"GridXMargin",L"0",L"GridXMargin");
	INI2XML(L"GridYMargin",L"0",L"GridYMargin");
	INI2XML(L"HoverSelection",L"0",L"HoverSelection");
	INI2XML(L"HoverSelectionDelayInMs",L"500",L"HoverSelectionDelay");
	INI2XML(L"ShowTrayIcon",L"1",L"ShowTrayIcon");
	INI2XML(L"IconFile",L"",L"IconFileName");
	INI2XML(L"IconIndex",L"0",L"IconIndex");

	saver.GoToParentNode();

	saver.AddNode(L"DefaultDeskletSettings");

}