// WinampPlugin.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


typedef struct {
	int version;
	char *description;
	int (*init)();
	void (*config)();
	void (*quit)();
	HWND hwndParent;
	HINSTANCE hDllInstance;
} winampGeneralPurposePlugin;

#define GPPHDR_VER 0x10

extern winampGeneralPurposePlugin *gen_plugins[256];
typedef winampGeneralPurposePlugin * (*winampGeneralPurposePluginGetter)();

int init();
void config();
void quit();

// winamp stuff
winampGeneralPurposePlugin plugin =
{
	GPPHDR_VER,
	"AveTunes Notification Plugin v1.0",
	init,
	config,
	quit,
};

UINT broadcastMessage = 0;

int init()
{
	broadcastMessage = ::RegisterWindowMessageW(L"AveTunesWinampNotification_Close");
	return 0;
}

void config()
{
	::MessageBoxW(plugin.hwndParent, L"Plugin used by AveTunes to detect WinAmp closing.", L"AveTunes", MB_ICONINFORMATION);
}

void quit()
{
	::SendMessage(HWND_BROADCAST, broadcastMessage, 0, 1L);
}

extern "C" __declspec( dllexport ) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin()
{
	return &plugin;
}