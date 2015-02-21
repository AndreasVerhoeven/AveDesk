// IconAnimator.h: interface for the CIconAnimator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONANIMATOR_H__B29AED72_9C4A_49D7_8596_E549D790D952__INCLUDED_)
#define AFX_ICONANIMATOR_H__B29AED72_9C4A_49D7_8596_E549D790D952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IIconAnimatorCallback.h"
#include "TrayAnimationData.h"

class CIconAnimator  
{
private:
	CTrayAnimationData* m_AnimData;
	IIconAnimatorCallback* m_Callback;
	DWORD  m_CallbackValue;

	HANDLE m_Handle;

	BOOL   m_Stop;
	BOOL   m_StopDirectly;

	CRITICAL_SECTION m_CriticalSection;

public:
	CIconAnimator(CTrayAnimationData* animData, IIconAnimatorCallback* callback, DWORD callbackValue);
	~CIconAnimator();

	static DWORD __stdcall TrayIconAnimationThread(LPVOID lpData);

	void Start();
	void Stop(DWORD maxWaitTime=INFINITE);
	void StopDirectly();

	void RemoveCallback();

};

#endif // !defined(AFX_ICONANIMATOR_H__B29AED72_9C4A_49D7_8596_E549D790D952__INCLUDED_)
