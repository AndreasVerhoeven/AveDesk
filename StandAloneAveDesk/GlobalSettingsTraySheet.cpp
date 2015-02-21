// filename:		CGlobalSettingsTraySheet.hpp
// Description:		Definition of class CGlobalSettingsMoveSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "GlobalSettingsTraySheet.hpp"
#include "AveDeskGlobalSettings.hpp"
#include "resource.h"
#include <atlmisc.h>
#include "helper_funcs.h"

#include "IconAnimator.h"
#include "Application.h"
extern CComObject<CApplication>* _App;

CGlobalSettingsTraySheet::CGlobalSettingsTraySheet(AveDeskGlobalSettings& globalSettings) :
	globalSettings(globalSettings), hIcon(0), initMode(false),
	m_PreviousSelection(LB_ERR), m_IsDestroying(FALSE)
{
}


LRESULT CGlobalSettingsTraySheet::OnAnimationsListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_PreviousSelection != LB_ERR && m_PreviousSelection >= 0 && m_PreviousSelection < (int)m_Animators.size())
	{
		m_Animators[m_PreviousSelection]->Stop(0);
	}

	
	DWORD curSel = m_AnimationsList.GetCurSel();
	if(curSel != LB_ERR && curSel >= 0 && curSel < m_Animators.size())
	{
		//m_IsDestroying = TRUE;
		//m_Animators[curSel]->StopDirectly();
		//m_IsDestroying = FALSE;
		m_Animators[curSel]->Start();
		m_PreviousSelection = curSel;
	}

	if(!initMode)
		SetModified();

	return 0;
}

void CGlobalSettingsTraySheet::LoadAnimations()
{
	WCHAR trayAnimationsPath[MAX_PATH] = {0};
	wcscpy(trayAnimationsPath, _App->GetAveDeskDataDir().c_str());
	PathAppendW(trayAnimationsPath, L"Images\\Animations");

	WCHAR pathToSearch[MAX_PATH] = {0};
	wcscpy(pathToSearch, trayAnimationsPath);
	PathAppendW(pathToSearch, L"*.*");

	WIN32_FIND_DATAW findData = {0};
	HANDLE h = FindFirstFileW(pathToSearch, &findData);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(wcscmp(findData.cFileName,L".")!=0 && wcscmp(findData.cFileName,L"..")!=0 && findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				WCHAR path[MAX_PATH] = {0};
				wcscpy(path, trayAnimationsPath);
				PathAppendW(path, findData.cFileName);

				CTrayAnimationData* trayAnimData = CTrayAnimationData::LoadFromFolder(path, findData.cFileName);

				if(trayAnimData != NULL)
				{
					m_Animations.push_back(trayAnimData);

					ILBITEM item = {0};
					item.mask = ILBIF_TEXT | ILBIF_SUBTEXT | ILBIF_IMAGE |   ILBIF_STYLE | ILBIF_FORMAT;
					item.iItem = m_AnimationsList.GetCount();
					item.pszText = const_cast<WCHAR*>(trayAnimData->GetFolderName().c_str());
					item.pszSubText = const_cast<WCHAR*>(trayAnimData->GetAuthor().c_str());
					item.style = ILBS_IMGLEFT;
					item.format = DT_LEFT;

					if(trayAnimData->GetNumberOfIcons() > 0)
					{
						HICON icon = trayAnimData->GetIcon(0);
						ImageList_AddIcon(m_ImageList.m_hImageList, icon);
						item.iImage = m_AnimationsList.GetCount();
					}

					m_Animators.push_back( new CIconAnimator(trayAnimData, this, m_AnimationsList.GetCount()));
					m_AnimationsList.InsertItem(&item);
				}
			}

		}while(FindNextFileW(h,&findData));
	FindClose(h);
	}
}

bool CGlobalSettingsTraySheet::Render(HICON icon, DWORD callbackValue)
{
	if(::IsWindow(m_hWnd) && ::IsWindow(m_AnimationsList.m_hWnd) && m_IsDestroying == FALSE)
	{
	
		ImageList_ReplaceIcon(m_ImageList.m_hImageList, callbackValue,icon);

		RECT boundingRect = {0};
		::SendDlgItemMessage(m_hWnd, IDC_ANIMATIONS, LB_GETITEMRECT, callbackValue, reinterpret_cast<LPARAM>(&boundingRect));
		boundingRect.right = 16 + 1;
	
		m_AnimationsList.InvalidateRect(&boundingRect,FALSE);
	}

	return true;
}

void CGlobalSettingsTraySheet::UnloadAnimations()
{
}


CGlobalSettingsTraySheet::~CGlobalSettingsTraySheet()
{
	if(hIcon)
		DestroyIcon(hIcon);

	//UnloadAnimations();
}

typedef struct _AnimatorsClearingThreadParams
{
	HANDLE handle;
	std::vector<CIconAnimator*> animators;
	std::vector<CTrayAnimationData*> animations;
} AnimatorsClearingThreadParams;

DWORD __stdcall AnimatorsClearingThread(LPVOID lpData)
{
	AnimatorsClearingThreadParams* params = reinterpret_cast<AnimatorsClearingThreadParams*>(lpData);

	for(std::vector<CIconAnimator*>::iterator iter = params->animators.begin(); iter != params->animators.end(); ++iter)
	{
		CIconAnimator* animator = *iter;
		animator->StopDirectly();
		delete animator;
	}

	for(std::vector<CTrayAnimationData*>::iterator iter2 = params->animations.begin(); iter2 != params->animations.end(); ++iter2)
		delete *iter2;

	CloseHandle(params->handle);
	delete params;

	ExitThread(0);

	return 0;
}

LRESULT CGlobalSettingsTraySheet::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_IsDestroying = TRUE;

	// make sure no more messages are in the queue if we start removing the callback, otherwise
	// the threads will deadlock
	MSG msg = {0};
	while(PeekMessage(&msg,m_hWnd,0,0,PM_REMOVE))
	{
	  TranslateMessage(&msg);
      DispatchMessage(&msg);
	 }

	for(std::vector<CIconAnimator*>::iterator iter = m_Animators.begin(); iter != m_Animators.end(); ++iter)
	{
		CIconAnimator* animator = *iter;
		animator->RemoveCallback();
	}

	
	DWORD dummy(0);
	AnimatorsClearingThreadParams* params = new AnimatorsClearingThreadParams;
	params->animators = m_Animators;
	params->animations = m_Animations;
	params->handle = CreateThread(NULL,0,AnimatorsClearingThread,reinterpret_cast<LPVOID>(params),0L,&dummy);

	return 0;
}

void CGlobalSettingsTraySheet::UpdateIcon()
{
	if(hIcon)
		DestroyIcon(hIcon);

	hIcon = ExtractIconW(GetModuleHandle(0),selIconFile.c_str(),selIconIndex);

	iconControl.SetIcon(hIcon);
}
 
// Message handlers
BOOL CGlobalSettingsTraySheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	iconControl = GetDlgItem(IDC_CURICON2);
	//ATLASSERT(iconControl.IsWindow() );
	
	LOGFONTW lfBig = {0};
	lfBig.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfBig.lfFaceName,L"Tahoma");
	lfBig.lfWeight = FW_BOLD;
	lfBig.lfHeight = 14;

	LOGFONTW lfSmall = {0};
	lfSmall.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfSmall.lfFaceName,L"Tahoma");
	lfSmall.lfWeight = FW_NORMAL;
	lfSmall.lfHeight = 12;

	bigFont.CreateFontIndirect(&lfBig);
	smallFont.CreateFontIndirect(&lfSmall);


	//ILC_COLOR24
	//m_ImageList.Create(16,16,ILC_COLOR24,1, 1);
	m_ImageList.Create(16,16,ILC_COLOR32,1, 1);

	m_AnimationsList.SubclassWindow(GetDlgItem(IDC_ANIMATIONS));
	m_AnimationsList.SetItemHeight(0, 30);
	m_AnimationsList.SetImageList(m_ImageList, ILSIL_NORMAL);

	m_AnimationsList.SetFont(bigFont);
	DeleteObject(m_AnimationsList.SetSmallFont(smallFont));

	DragAcceptFiles(TRUE);

	CheckDlgButton(IDC_SHOWTRAY,globalSettings.GetShowTrayIcon() );

	selIconFile = globalSettings.GetIconFileName();
	selIconIndex = globalSettings.GetIconIndex();
	UpdateIcon();

	CheckDlgButton(IDC_DOTRAYANIMATION, globalSettings.GetDoTrayAnimation());

	LoadAnimations();


	int index(LB_ERR);
	DWORD i(0);
	std::vector<CTrayAnimationData*>::iterator iter = m_Animations.begin();
	while(iter != m_Animations.end() && index == LB_ERR)
	{

		CTrayAnimationData* data = *iter;
		if(data->GetFolderName() == globalSettings.GetTrayAnimationFolder())
			index = i;

		i++;
		iter++;
	}

	if(index != LB_ERR)
	{
		m_AnimationsList.SetCurSel(index);
		if(index >= 0 && index < (int)m_Animators.size())
		{
			//m_Animators[curSel]->StopDirectly();
			m_Animators[index]->Start();
			m_PreviousSelection = index;
		}
	}

	return TRUE;	
}
 
int CGlobalSettingsTraySheet::OnApply()
{
	globalSettings.SetIconFile(selIconFile.c_str(),selIconIndex, IsDlgButtonChecked(IDC_SHOWTRAY) ==BST_CHECKED);
	globalSettings.SetDoTrayAnimation( IsDlgButtonChecked(IDC_DOTRAYANIMATION) == BST_CHECKED);

	DWORD index = m_AnimationsList.GetCurSel();
	if(index != LB_ERR && index >= 0 && index < m_Animations.size())
		globalSettings.SetTrayAnimationFolder ( m_Animations[index]->GetFolderName() );

	return TRUE;
};

LRESULT CGlobalSettingsTraySheet::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDrop = reinterpret_cast<HDROP>(wParam);

	int count = DragQueryFile(hDrop,(UINT)-1,NULL,0);
	if(count > 0)
	{
		WCHAR fileName[MAX_PATH] = {0};
		DragQueryFile(hDrop,0,fileName,MAX_PATH);
		if(fileName[0] != L'\0')
		{
				selIconFile = fileName;
				selIconIndex = 0;
				UpdateIcon();
				SetModified();
		}
	}

	DragFinish(hDrop);

	return 0;
}

void CGlobalSettingsTraySheet::OnEditSettings(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && wNotifyCode == BN_CLICKED)
		SetModified();
}

void CGlobalSettingsTraySheet::OnBrowse(UINT wNotififyCode, int wID, HWND hwndCtrl)
{
	HMODULE mod = LoadLibraryW(L"shell32");
	if(mod)
	{
		LONG (__stdcall *SHChangeIconDialog)(HWND,WCHAR* filename,LONG,LONG*) = (LONG (__stdcall*)(HWND,WCHAR* filename,LONG,LONG*) ) GetProcAddress(mod,(CHAR*)MAKEWORD(62,0));
		if(SHChangeIconDialog)
		{
			WCHAR fname[MAX_PATH] = {0};
			wcscpy(fname,selIconFile.c_str() );
			if (SHChangeIconDialog(*this,fname,MAX_PATH,&selIconIndex) )
			{
				selIconFile = fname;
				UpdateIcon();
				SetModified();
			}
		}
        else
        {
          //!herd:I'm a born pessimist: - this function is now documented 
		// under the name PickIconDlg() ... need a platform SDK update for that too

          MessageBox(
            _T("Ooops - I used an undocumented function and now it has disappeared.\n")
            _T("My apologies for the inconvenience.\n\n")
            _T("Please Drag and Drop an Icon onto the image instead."),
            _T("Unexpected (Longhorn) Error"),
            MB_OK | MB_ICONEXCLAMATION);
        }

		FreeLibrary(mod);
	}
}

void CGlobalSettingsTraySheet::OnDefault(UINT wNotififyCode, int wID, HWND hwndCtrl)
{
	WCHAR tmp[MAX_PATH+1];
	GetModuleFileNameW(NULL,tmp,sizeof(tmp)/sizeof(tmp[0]));
	selIconFile = tmp;
	selIconIndex = 1;
	UpdateIcon();
	SetModified();
}

