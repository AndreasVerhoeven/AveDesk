// IconAnimator.cpp: implementation of the CIconAnimator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IconAnimator.h"

#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconAnimator::CIconAnimator(CTrayAnimationData* animData, IIconAnimatorCallback* callback, DWORD callbackValue) :
	m_Stop(FALSE),
	m_Handle(NULL),
	m_AnimData(animData),
	m_Callback(callback),
	m_CallbackValue(callbackValue),
	m_StopDirectly(FALSE)
{
		InitializeCriticalSection(&m_CriticalSection);
}

CIconAnimator::~CIconAnimator()
{
	if(m_Handle != NULL)
		Stop();

	DeleteCriticalSection(&m_CriticalSection);
}

void CIconAnimator::StopDirectly()
{
	EnterCriticalSection(&m_CriticalSection);
		m_StopDirectly = TRUE;	
	LeaveCriticalSection(&m_CriticalSection);

	Stop(INFINITE);
}

void CIconAnimator::RemoveCallback()
{
	EnterCriticalSection(&m_CriticalSection);
		m_Callback = NULL;	
	LeaveCriticalSection(&m_CriticalSection);
}

DWORD __stdcall CIconAnimator::TrayIconAnimationThread(LPVOID lpData)
{
	CIconAnimator* animator = reinterpret_cast<CIconAnimator*>(lpData);

	if(animator != NULL && animator->m_AnimData->GetNumberOfIcons() > 0)
	{
		UINT iconIndex = 0;
		UINT currentRevolution = 0;

		UINT speedInMs = animator->m_AnimData->GetInitialSpeedInMs();
		

		// spin the tray-icon
		while(animator->m_Stop == FALSE)
		{
			if(animator->m_Stop == TRUE)
			{
				OutputDebugString(_T("THREAD SHOULD STOP!\n"));	
			}

			EnterCriticalSection(&animator->m_CriticalSection);
			if(animator->m_StopDirectly == FALSE && animator->m_Callback != NULL)
			{
				//OutputDebugString(_T("entering render\n"));
				animator->m_Callback->Render(animator->m_AnimData->GetIcon(iconIndex), animator->m_CallbackValue);
				//OutputDebugString(_T("exited render\n"));
			}
			LeaveCriticalSection(&animator->m_CriticalSection);

			//OutputDebugString(_T("entering sleep\n"));
			SleepEx(speedInMs ,TRUE);
			
			//OutputDebugString(_T("exiting sleep\n"));

			iconIndex++;
			if(iconIndex % animator->m_AnimData->GetNumberOfIcons ()  == 0)
			{
				iconIndex = 0;
				currentRevolution += 1;

				if(speedInMs - currentRevolution > 0)
					speedInMs -= currentRevolution;
				else
					speedInMs = animator->m_AnimData->GetMaximumSpeedInMs();

				if(speedInMs < animator->m_AnimData->GetMaximumSpeedInMs())
				{
					speedInMs = animator->m_AnimData->GetMaximumSpeedInMs();
					currentRevolution = 0;
				}		
			}
		}

		// smoothly go back to not spinning
		if(speedInMs == animator->m_AnimData->GetMaximumSpeedInMs() && animator->m_StopDirectly == FALSE)
		{

			currentRevolution = 0;
			while(currentRevolution != animator->m_AnimData->GetNumberOfRevolutionsNeededToRestore() && animator->m_StopDirectly == FALSE)
			{
				EnterCriticalSection(&animator->m_CriticalSection);
				if(animator->m_StopDirectly == FALSE && animator->m_Callback != NULL)
					animator->m_Callback->Render(animator->m_AnimData->GetIcon(iconIndex), animator->m_CallbackValue);
				LeaveCriticalSection(&animator->m_CriticalSection);

				SleepEx(speedInMs ,TRUE);

				iconIndex++;
				if(iconIndex % animator->m_AnimData->GetNumberOfIcons() == 0)
				{
					currentRevolution += 1;

					speedInMs += (currentRevolution);
					iconIndex = 0;
				}
			}
		}

		EnterCriticalSection(&animator->m_CriticalSection);
			if(animator->m_StopDirectly == FALSE && animator->m_Callback != NULL)
			animator->m_Callback->Render(NULL, animator->m_CallbackValue);
		LeaveCriticalSection(&animator->m_CriticalSection);
	}


	OutputDebugString(_T("m_Handle = NULL\n"));
	CloseHandle(animator->m_Handle);
	animator->m_Handle = NULL;


	OutputDebugString(_T(">>> thread destroyed\n"));
	ExitThread(1);

	return 0;
}

void CIconAnimator::Start()
{
	if(m_Handle == NULL && m_Callback != NULL && m_AnimData != NULL)
	{
		DWORD dummy(0);
		m_StopDirectly = FALSE;
		m_Stop = FALSE;
		m_Handle = CreateThread(NULL,0,TrayIconAnimationThread,reinterpret_cast<LPVOID>(this),0L,&dummy);
	}
}

void CIconAnimator::Stop(DWORD maxWaitTime)
{
	HANDLE h = m_Handle;
	if(m_Handle != NULL)
	{
		//EnterCriticalSection(&m_CriticalSection);
		OutputDebugString(_T(">>> m_Stop = TRUE\n"));
		m_Stop = TRUE;

		OutputDebugString(_T(">>> waiting\n"));
		WaitForSingleObject(h, maxWaitTime);
		//LeaveCriticalSection(&m_CriticalSection);
	}
}