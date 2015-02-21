

#include "stdafx.h"

#include <vector>
#include <sstream>

#include "helper_funcs.h"

#include "AveDeskGlobalSettings.hpp"

extern AveDeskGlobalSettings _globalSettings;

#include "Application.h"
extern CComObject<CApplication>* _App;

#include "TrayIconAnimation.h"

#include "Policies.h"
extern CPolicies _Policies;


BOOL CTrayIconAnimation::isDoingAnimation = FALSE;
CTrayAnimationData* CTrayIconAnimation::animData = FALSE;


CTrayIconAnimation::CTrayIconAnimation(NOTIFYICONDATAW nid, BOOL start) :
	m_Handle(NULL),
	m_Stop(FALSE),
	m_Nid(nid)
{
	if(start)
		Start();
}

CTrayIconAnimation::~CTrayIconAnimation()
{
	if(m_Handle != NULL)
		Stop();
}


void CTrayIconAnimation::Start()
{
	if(_Policies.GetBoolValue(L"ui//trayicon//noAnimation", FALSE))
		return;

	if(isDoingAnimation == FALSE && _globalSettings.GetShowTrayIcon() && _globalSettings.GetDoTrayAnimation())
	{
		DWORD dummy(0);
		m_Stop = FALSE;
		m_Handle = CreateThread(NULL,0,TrayIconAnimationThread,reinterpret_cast<LPVOID>(this),0L,&dummy);
	}
}

void CTrayIconAnimation::Stop(DWORD maxWaitTime)
{
	if(m_Handle != NULL)
	{
		m_Stop = TRUE;
		WaitForSingleObject(m_Handle, maxWaitTime);
	}
}

void CTrayIconAnimation::LoadIcons()
{
	if(animData != NULL && _wcsicmp(animData->GetFolderName().c_str(), _globalSettings.GetTrayAnimationFolder().c_str()) != 0)
	{
		delete animData;	
		animData = NULL;
	}


	if(animData == NULL && _globalSettings.GetDoTrayAnimation())
	{
		WCHAR path[MAX_PATH] = {0};
		wcscpy(path, _App->GetAveDeskDataDir().c_str());
		PathAppendW(path, L"Images\\Animations");
		PathAppendW(path, _globalSettings.GetTrayAnimationFolder().c_str());

		animData = CTrayAnimationData::LoadFromFolder(path, _globalSettings.GetTrayAnimationFolder().c_str());
	}
}

void CTrayIconAnimation::DestroyIcons()
{
	delete animData;
	animData = NULL;
}

DWORD __stdcall CTrayIconAnimation::TrayIconAnimationThread(LPVOID lpData)
{
	CTrayIconAnimation* trayAnim = reinterpret_cast<CTrayIconAnimation*>(lpData);

	isDoingAnimation = TRUE;

	LoadIcons();

	if(animData != NULL && animData->GetNumberOfIcons() > 0 && !_Policies.GetBoolValue(L"ui//trayicon//noAnimation", FALSE))
	{
		UINT iconIndex = 0;
		UINT currentRevolution = 0;

		UINT speedInMs = animData->GetInitialSpeedInMs();
		

		// spin the tray-icon
		while(trayAnim->m_Stop == FALSE)
		{
			trayAnim->m_Nid.hIcon = animData->GetIcon(iconIndex);
			Shell_NotifyIconW(NIM_MODIFY,&trayAnim->m_Nid);

			SleepEx(speedInMs ,TRUE);

			iconIndex++;
			if(iconIndex % animData->GetNumberOfIcons ()  == 0)
			{
				iconIndex = 0;
				currentRevolution += 1;

				if(speedInMs - currentRevolution > 0)
					speedInMs -= currentRevolution;
				else
					speedInMs = animData->GetMaximumSpeedInMs();

				if(speedInMs < animData->GetMaximumSpeedInMs())
				{
					speedInMs = animData->GetMaximumSpeedInMs();
					currentRevolution = 0;
				}		
			}
		}

		// smoothly go back to not spinning
		if(speedInMs == animData->GetMaximumSpeedInMs())
		{
			currentRevolution = 0;
			while(currentRevolution != animData->GetNumberOfRevolutionsNeededToRestore())
			{
				trayAnim->m_Nid.hIcon = animData->GetIcon(iconIndex);
				Shell_NotifyIconW(NIM_MODIFY,&trayAnim->m_Nid);

				SleepEx(speedInMs ,TRUE);

				iconIndex++;
				if(iconIndex % animData->GetNumberOfIcons() == 0)
				{
					currentRevolution += 1;

					speedInMs += (currentRevolution);
					iconIndex = 0;
				}
			}
		}

		// restore original icon
		trayAnim->m_Nid.hIcon = ExtractIconW(_Module.GetModuleInstance(),_globalSettings.GetIconFileName().c_str(),_globalSettings.GetIconIndex() );
		Shell_NotifyIconW(NIM_MODIFY,&trayAnim->m_Nid);
		DestroyIcon(trayAnim->m_Nid.hIcon);

	}

	isDoingAnimation = FALSE;

	CloseHandle(trayAnim->m_Handle);
	trayAnim->m_Handle = NULL;

	ExitThread(0);

	return 0;
}