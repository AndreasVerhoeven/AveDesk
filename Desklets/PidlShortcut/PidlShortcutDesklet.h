// PidlShortcutDesklet.h: interface for the CPidlShortcutDesklet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIDLSHORTCUTDESKLET_H__81893815_5EA2_4BCA_8618_6863E272B497__INCLUDED_)
#define AFX_PIDLSHORTCUTDESKLET_H__81893815_5EA2_4BCA_8618_6863E272B497__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER		0x0501
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0100

#include <windows.h>
#include <wininet.h>
#include <shlobj.h>

#include <map>
#include <string>
#include <memory>
#include <algorithm>

#include "../DeskletImpl.hpp"

#include "Pidl.h"

#include <windows.h>

#include "IShortcutDesklet.h"

#include "BehaviourFactory.h"

#define TIMER_FADER 234904

//#define DEBUG_MSG(s) MessageBoxA(0,#s,"debug",0)
#define DEBUG_MSG(s)

class CPidlShortcutDesklet : 
	public CDeskletImpl<CPidlShortcutDesklet>,
	IShortcutDesklet
{
private:
	CPidl m_Pidl;
	bool  m_AutoSetCaption;

	std::wstring m_UserGivenPath;
	std::wstring m_SubLabelFormat;

	std::wstring m_StartIn;
	std::wstring m_Params;
	//CPidl m_StartIn;
	UINT  m_ExecuteAs;

	bool m_RemoveIfItemIsDeleted;
	bool m_RemoveIfItemIsUnmounted;

	std::auto_ptr<CBehaviour> m_Behaviour;

	UINT m_ShellIdListClipFormat;

	bool m_UseCustomImages;

	bool m_CanDrop;

	bool m_IsInFadingState;

	std::wstring m_FadingImage;
	bool m_IsFadingIn;
	int m_FadingCounter;
	bool m_ShouldFade;

	DWORD m_ThreadId;

	std::map<std::wstring,std::wstring> m_CustomImages;

	// property pages notify handlers:
	HWND m_HwndLabelSheet;
	HWND m_HwndIconSheet;

	bool m_IsAnimating;

	bool m_OverlayIsFadingIn;
	int  m_OverlayFadingCounter;
	bool m_OverlayShouldFade;

	void FillCMINVOKECOMMANDINFOEX(CMINVOKECOMMANDINFOEX& cmi)const;

	void ClearOverlayImage();
	void SetOverlayImage(DWORD effect);

	void DoAnimationFrame();

	CComPtr<IDropTarget> m_DropTarget;

	CPidl m_MonitoringPidl;

	void RemoveDragFormats();

public:
	DEFAULT_DESKLET_CONSTRUCTOR_PROTOTYPE(CPidlShortcutDesklet);

	virtual ~CPidlShortcutDesklet();

	BOOL LeftButtonClick(POINT *ptCursor, SIZE *sizeDesklet);	
	BOOL RightButtonClick(POINT *ptCursor, SIZE *sizeDesklet);	

	void ReadFromIni(const char* ini, const char* group);
	void SaveToIni(const char* ini, const char* group);

	virtual CPidl& GetPidl(){ return m_Pidl;};

	void UpdatePath(const std::wstring& path, const std::wstring& alternativePidlString=L"", bool checkMountedState=true);
	void UpdateData(bool checkMountedState);

	std::wstring GetPath()const{return m_UserGivenPath;};

	bool GetUseCustomImages()const { return m_UseCustomImages;};

	void SetUseCustomImages(bool val)
	{
		if(m_UseCustomImages != val)
		{
			m_UseCustomImages = val;
			UpdateIcon();
		}
	}

	CBehaviour* GetBehaviour(){return m_Behaviour.get();};

	int  AddPropertySheets(int numPresent);

	bool GetAutoSetCaption()const{ return m_AutoSetCaption;};
	void SetAutoSetCaption(bool val);

	std::wstring  GetSubLabelFormat()const{ return m_SubLabelFormat;};

	void SetSubLabelFormat(const std::wstring& str);

	void UpdateSubLabel();

	void UpdateIcon();

	std::wstring GetParams()const{return m_Params;};
	void SetParams(const std::wstring& v){m_Params = v;};

	std::wstring GetStartIn()const{return m_StartIn;};
	void SetStartIn(const std::wstring& path){m_StartIn = path;};

	UINT GetExecuteAs()const{return m_ExecuteAs;};
	void SetExecuteAs(UINT v){m_ExecuteAs = v;};

	HWND GetHwndIconSheet()const {return m_HwndIconSheet;};
	HWND GetHwndLabelSheet()const { return m_HwndLabelSheet;};

	void SetHwndIconSheet(HWND hwnd) { m_HwndIconSheet = hwnd;};
	void SetHwndLabelSheet(HWND hwnd){ m_HwndLabelSheet = hwnd;};

	bool GetRemoveIfItemIsDeleted()const{return m_RemoveIfItemIsDeleted;};
	void SetRemoveIfItemIsDeleted(bool val){ m_RemoveIfItemIsDeleted = val;};

	bool GetRemoveIfItemIsUnmounted()const{return m_RemoveIfItemIsUnmounted;};
	void SetRemoveIfItemIsUnmounted(bool val){ m_RemoveIfItemIsUnmounted = val;};

	void  ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	
	BOOL  DragDropDataRequest(FORMATETC* pFetc, STGMEDIUM* pMedium);


	virtual void NotifySublabelChange();
	virtual void NotifyIconChange();

	virtual HWND GetHWnd();

	virtual HINSTANCE GetInstanceHandle() { return GetHInstance() ;};

	std::map<std::wstring,std::wstring>& GetCustomImages(){ return m_CustomImages;};

	BOOL DragEnter(IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect);
	BOOL DragLeave();
	BOOL DragOver(DWORD keys ,POINTL pnt ,DWORD* effect);
	BOOL Drop(IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect);

	virtual void ImageFadeIn(std::wstring keyName);
	virtual void ImageFadeOut();

	void RemoveAndSave();
};

#endif // !defined(AFX_PIDLSHORTCUTDESKLET_H__81893815_5EA2_4BCA_8618_6863E272B497__INCLUDED_)
