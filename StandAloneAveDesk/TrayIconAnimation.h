#ifndef TRAY_ICON_ANIMATION_H___
#define TRAY_ICON_ANIMATION_H___

#include "stdafx.h"

#include <vector>

#include "TrayAnimationData.h"

class CTrayIconAnimation
{
public:
	static CTrayAnimationData* animData;
	static void LoadIcons();
	static void DestroyIcons();

private:
	static DWORD __stdcall TrayIconAnimationThread(LPVOID lpData);
	static BOOL isDoingAnimation;

	HANDLE m_Handle;
	BOOL   m_Stop;

	NOTIFYICONDATAW m_Nid;

public:

	CTrayIconAnimation(NOTIFYICONDATAW nid, BOOL start);
	~CTrayIconAnimation();

	void Start();
	void Stop(DWORD maxWaitTime=INFINITE);

};

typedef struct _TRAYICONANIMATIONPARAMS
{
	HANDLE handle;
	BOOL  stop;
} TrayIconAnimationParams;

#endif//TRAY_ICON_ANIMATION_H___