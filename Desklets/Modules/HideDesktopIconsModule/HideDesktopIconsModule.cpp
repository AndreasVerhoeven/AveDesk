#define UNICODE
#include <windows.h>

struct MODULE_STRUCT
{
	BOOL wasVisible;
	HWND listView;
};


HWND FindIconsListView()
{
	HWND progMan = FindWindow(TEXT("Progman"),NULL);
	if(NULL == progMan)
		return NULL;

	HWND defView = FindWindowEx(progMan,NULL,TEXT("SHELLDLL_DefView"),NULL);
	if(NULL == defView)
		return NULL;

	HWND listView = FindWindowEx(defView,NULL,TEXT("SysListView32"),NULL);

	return listView;
}

MODULE_STRUCT* __stdcall OnCreate()
{
	MODULE_STRUCT* ms = new MODULE_STRUCT;
	ms->wasVisible = FALSE;

	ms->listView = FindIconsListView();
	if(ms->listView != NULL)
	{
		ms->wasVisible = IsWindowVisible(ms->listView);
		ShowWindow(ms->listView,SW_HIDE);
	}


	return ms;
}

void __stdcall OnDestroy(MODULE_STRUCT* ms)
{
	if(NULL == ms)
		return;

	if(ms->wasVisible && ms->listView != NULL)
	{
		ShowWindow(ms->listView,SW_SHOW);
	}

	delete ms;
};

void __stdcall OnGetInformation(char* name, char* author, int* version, char* descr)
{
	strcpy(name,"Hide Desktop Icons");
	strcpy(author,"Andreas Verhoeven");
	strcpy(descr,"This module will hide your desktop icons when it\'s loaded.");
	*version = 100;
}
