#include "stdafx.h"
#include "DeskletSkins.h"
#include "DeskletSkin.h"
#include "AveInstallFile.h"
#include "AveThemeFile.h"
#include "AveDeskletFile.h"
#include "SkinEnumerator.h"
#include "ShowCase1.h"
#include "resource.h"
#include <initguid.h>
#include "AveDesk.h"
#include "AveDesk_i.c"
//!herd: all COM classes below here
#include "Application.h"
#include "Desklets.h"
#include "Desklet.h"
#include "Matrix.h"
#include "ColorMatrix.h"
#include "EffectLet.h"
#include "Preference.h"
#include "Preferences.h"
#include "Timer.h"
#include "Debug.h"
#include "AveImage.h"
#include "Images.h"
#include "DeskletInfos.h"
#include "DeskletInfo.h"
#include "oLayers.h"
#include "oLayer.h"

#include "AvailableDeskletsAPI.h"

#include "AveInstaller.h"
#include "AveInstPackageManager.h"
#include "ThemeBrowserDlg.h"

#include "FlipAnimation.h"

#include "IPersistable.h"

#include "TrayIconAnimation.h"

#include "controldlg.h"
#include "aboutdlg.h"
//!herd: the class wizard will insert above, but it'll need reordering
#include <crtdbg.h>
#include <shellapi.h>
#include <commctrl.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "prsht.h"
#pragma comment(lib, "comctl32.lib")
#include  <gdiplus.h>
using namespace Gdiplus;				
#pragma comment(lib, "gdiplus.lib")

#pragma comment(lib,"msimg32.lib")

#include "XMLLoader.h"
#include "XMLSaver.h"

// WTL stuff
#include <windows.h>
#include <atlapp.h> //Message loop, interfaces, general app stuff  
#include <atlcrack.h> //Message cracker macros atlctrls.h Standard and common control classes  
#include <atlwin.h>
#include <atlctrls.h> // controls
#include <atlctrlw.h> //Command bar class atlctrlx.h Bitmap button, check list view, and other controls   
#include <atldlgs.h>// property sheet stuff
#include <atlmisc.h>
#include "colorbutton.h"

#include "AddDeskletDlg.h"

#include "GlobalSettingsMoveSheet.hpp"

#define NO_LAYERED_WINDOW
#include "od_docklet.hpp"
#include "od_docklet_instance.hpp"
using namespace docklets;

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "constants.h"
#include "helper_funcs.h"
#include "label.h"
#include "callback_funcs.h"
#include "labelbackground.h"
#include "dockletdll.h"
#include "docklet.h"
#include "test.h"
#include "avedesk.h"
#include "AveDeskGlobalSettings.hpp"
#include "Tools.hpp"
#include "Modules.h"
#include "Module.h"
#include "ThemeIniToXMLMapper.h"

#include "AveDeskFileAssociation.h"

#include "Unzip/Unzipper.h"

AveDeskGlobalSettings _globalSettings;
CModules* _Modules = NULL;

GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken;


// GLOBAL SETTING (OVERRIDES)
bool _noFileAssociation = false;
WCHAR overridenLanguageFile[MAX_PATH] = {0};
bool _noMemCleaner = false;
bool _noAntiAliasFonts = false;
bool _alwaysSilentInstall = false;
bool _noStartAnimation = false;
bool _noCloseAnimation = false;
DWORD _AnimTimeInMS = 500;
BOOL _noLoadThemeOnStartUp = FALSE;
UINT _ControlPanelPreviewIconWidth = 48;
UINT _ControlPanelPreviewIconHeight = 48;
UINT _themePreviewSizePercentage = 25;
BOOL _noCachingOfBitmaps = FALSE;


#include "Policies.h"
CPolicies _Policies;

LRESULT __stdcall FrameSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

const DWORD dwTimeOut = 5000; // time for EXE to be idle before shutting down
const DWORD dwPause = 1000; // time to wait for threads to finish up

// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CExeModule* p = (CExeModule*)pv;
    p->MonitorShutdown();
    return 0;
}

LONG CExeModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0)
    {
        bActivity = true;
        SetEvent(hEventShutdown); // tell monitor that we transitioned to zero
    }
    return l;
}

void CExeModule::ChangeResourceInstance(LPCTSTR file)
// pre:  file is not NULL
// post: changes the resource file used by AveDesk
{
	if(m_hInstResource != GetModuleHandle(NULL) )
		FreeLibrary(m_hInstResource);

	if(PathFileExists(file))
	{
		m_hInstResource = LoadLibrary(file);
		DWORD error = GetLastError();
		if(m_hInstResource)
		{
			// NOTE: this is an explicit check for AveDesk 1.3 resource files
			int (__stdcall *Func)() = (int (__stdcall *)())GetProcAddress(m_hInstResource,"GetAveDeskVersion");
			if(!Func || Func() != 130)
			{
				FreeLibrary(m_hInstResource);
				m_hInstResource = GetModuleHandle(NULL);
			}

		}
	}
	else
		m_hInstResource = GetModuleHandle(NULL);
}

//Monitors the shutdown event
void CExeModule::MonitorShutdown()
{
    while (1)
    {
        WaitForSingleObject(hEventShutdown, INFINITE);
        DWORD dwWait=0;
        do
        {
            bActivity = false;
            dwWait = WaitForSingleObject(hEventShutdown, dwTimeOut);
        } while (dwWait == WAIT_OBJECT_0);
        // timed out
        if (!bActivity && m_nLockCnt == 0) // if no activity let's really bail
        {
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
            CoSuspendClassObjects();
            if (!bActivity && m_nLockCnt == 0)
#endif
                break;
        }
    }
    CloseHandle(hEventShutdown);
    PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
}

bool CExeModule::StartMonitor()
{
    hEventShutdown = CreateEvent(NULL, false, false, NULL);
    if (hEventShutdown == NULL)
        return false;
    DWORD dwThreadID;
    HANDLE h = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
    return (h != NULL);
}


// Global variables, which are just singletons, so they can be global.
CExeModule _Module;
CComObject<CApplication> *_App;
CComObject<CDebug> *_Dbg;
CComObject<CShowCase> *_ShowCase;
CControlDlg _Dlg;
CTools _Tools;


BEGIN_OBJECT_MAP(ObjectMap)
//OBJECT_ENTRY(CLSID_Application, CApplication)
//OBJECT_ENTRY(CLSID_Desklets, CDesklets)
//OBJECT_ENTRY(CLSID_Desklet, CDesklet)
//OBJECT_ENTRY(CLSID_EffectLet, CEffectLet)
//OBJECT_ENTRY(CLSID_Preferences, CPreferences)
//OBJECT_ENTRY(CLSID_Preference, CPreference)
//OBJECT_ENTRY(CLSID_ColorMatrix, CColorMatrix)
//OBJECT_ENTRY(CLSID_Matrix, CMatrix)
//OBJECT_ENTRY(CLSID_Timer, CTimer)
//OBJECT_ENTRY(CLSID_Debug, CDebug)
//OBJECT_ENTRY(CLSID_AveImage, CImage)
//OBJECT_ENTRY(CLSID_AveImages, CImages)
//OBJECT_ENTRY(CLSID_DeskletInfo, CDeskletInfo)
//OBJECT_ENTRY(CLSID_DeskletInfos, CDeskletInfos)
//OBJECT_ENTRY(CLSID_oLayer, CoLayer)
//OBJECT_ENTRY(CLSID_Layers, CoLayers)
//OBJECT_ENTRY(CLSID_ShowCase, CShowCase)
//OBJECT_ENTRY(CLSID_SkinEnumerator, CSkinEnumerator)
//OBJECT_ENTRY(CLSID_AveDeskletFile, CAveDeskletFile)
//OBJECT_ENTRY(CLSID_AveThemeFile, CAveThemeFile)
//OBJECT_ENTRY(CLSID_AveInstallFile, CAveInstallFile)
//OBJECT_ENTRY(CLSID_DeskletSkin, CDeskletSkin)
//OBJECT_ENTRY(CLSID_DeskletSkins, CDeskletSkins)
END_OBJECT_MAP()


LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

//!eof herd

// global mutex to make sure this app is only launched once.
HANDLE hMutex = 0;
//NOTIFYICONDATAW _ncd = {0};

// OD CALLBACK SDK FUNCTIONS
//--------------------------------------------------------------------------------------------
/**
 * Not implemented function.
 *
 */
void CALLBACK DeskletSetHelperDesklet(HWND hwndDesklet, BOOL isHelper)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		// TODO: implement
	}
}
//--------------------------------------------------------------------------------------------
/**
 *	Gets AveDesk's version.
 *	@return the version number of the current AveDesk host-application.
 *  @remark the version number is defined as major * 100 + minor * 10 + subversion;
 */
int CALLBACK DeskletGetAveDeskVersion()
// post: AveDesk's version has been returned.
{
	long version(0);
	if(_App != NULL)
		_App->get_Version(&version);

	return version;
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the sublabel of a desklet.
 * @param hwndDesklet the handle of the desklet to set the sublabel for
 * @param subLabel    the text of the sublabel to set to. Cannot be NULL.
 * @remark the sublabel is not persisted when AveDesk or the current theme is closed.
 */
void CALLBACK DeskletSetSubLabelW(HWND hwndDesklet, const WCHAR* subLabel)
// pre:  desklet(hwndDesklet) != NULL, subLabel is not NULL
// post: the label of the desklet(hwndDesklet) has been set to subLabel
// note: UNICODE and NON-UNICODE version available.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL && subLabel != NULL)
	{
		std::wstring wsSubLabel(subLabel);

		if(d->GetLabel().GetSubCaption() != wsSubLabel)
		{
			d->GetLabel().SetSubCaption(wsSubLabel);

			if(d->GetLabel().GetShowSubLabel())
				d->UpdateLabelWindow();
		}
	}
}

/**
 * Sets the sublabel of a desklet (NONUNICODE)
 * @deprecated
 * @see        DeskletSetSubLabelW
 * @remark     NON-UNICODE function of DeskletSetSubLabel
 */
void CALLBACK DeskletSetSubLabel(HWND hwndDesklet, char* subLabel)
// note: NON-UNICODE version of DeskletSetSubLabelW
{
	DeskletSetSubLabelW(hwndDesklet,StringToWString(subLabel).c_str() );
}
//--------------------------------------------------------------------------------------------
/**
 * Adds a propertysheet to the desklets property-dialog.
 * @param  hwndDesklet the handle of the desklet to add the propertysheet for
 * @param  hpg         the propertysheet to add to dialog. Cannot be NULL.
 * @return the number of propertysheets present
 * @remark This function needs to be called only in the OnAddPropertySheet() callback function.
 *         Everytime the property-dialog is displayed, All old HPROPSHEETPAGE's are removed and
 *         OnAddPropertySheet() is called. In that function, the HPROPSHEETPAGE should be
 *         made again. In other words, HPROPSHEETPAGE are not persisted.
 */
int CALLBACK DeskletAddPropertySheet(HWND hwndDesklet, HPROPSHEETPAGE hpg)
// pre:  desklet(hwndDesklet) != NULL, hpg is not NULL
// post: the new number of propertysheets associated with this desklet has been returned if successfull, 
//       otherwise 0 has been returned.
// note: should be called in response to the OnAddPropertySheet() callback function.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL && hpg != NULL)
	{
		return d->AddPropertyPage(hpg);
	}
	else
	{
		return 0;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gives the default menu for a desklet.
 * @param  hwndDesklet the handle of the desklet to get the default menu for 
 * @param  startId     the id that the first menuitem starts with.
 * @return An HMENU with the default-desklet-menu if successfull. Otherwise NULL.
 * @remark 1) The caller gets ownership of the HMENU and thus must destroy it with DestroyMenu()
 *         2) The default menu for a desklet is the menu a user gets when he right-clicks a desklet
 *            that has no rightclick-handler. The menu contains items to interact with avedesk and the
 *            desklet.
 */
HMENU CALLBACK DeskletGetDefaultMenu(HWND hwndDesklet, int startId)
// pre:  desklet(hwndDesklet) != NULL
// post: A new HMENU has been returned that is the default AveDesk menu for desklets. The first menu-item starts
//       with startId. If not-successfull, NULL has been returned instead.
// note: the caller gets ownership of the allocated HMENU and thus must call DestroyMenu() on it when
//       done using it.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->GetDefaultMenu(startId);
	}
	else
	{
		return NULL;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Performs the command associated with a menuitem for the desklets default-menu.
 * @param  hwndDesklet the handle of the desklet to perform the command for.
 * @param  startId     the id that the first menuitem starts with.
 * @param  cmd         the id of the menuitem to perform the command for.
 * @remark this function is typically used together with <function>DeskletGetDefaultMenu</function>
 */
void CALLBACK DeskletDoDefaultMenuCommand(HWND hwndDesklet,int startId,int cmd)
// post:  the action for the default-menu item that has command-id cmd in a HMENU obtained
//        from DeskletGetDefaultMenu(...,startId) has been performed.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		d->DoDefaultMenuCommand(startId,cmd);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * @visibility INTERNAL
 */
void CALLBACK DuplicateDesklet(HWND hwndDesklet, BOOL Ask, HWND *phwndTarget)
// pre:  desklet(hwndDesklet) != NULL
// post: desklet(hwndDesklet) has been duplicated (instantiated with the same settings).
// parameters:
//       BOOL Ask - if TRUE, the user must confirm the action (can be overriden though by 
//                  global user-settings.
//
//       HWND *phwndTarget - if not NULL, it will be assigned the new HWND that the
//                           duplicated desklet has.

// note: INTERNAL FUNCTION, IT'S EXPORTED BUT NOT IN THE SDK.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	HWND hwndTemp = NULL;
	if(d != NULL)
	{
		if(_globalSettings.GetAsk() && Ask)
		{
			CString str;
			str.LoadString(IDS_ASKDUPLICATE);
			CString caption;
			caption.LoadString(IDS_PROJNAME);
			if(MessageBox(hwndDesklet,str,caption,MB_ICONQUESTION | MB_YESNO)==IDNO)
				return; // bail out, the user decided to not duplicate actually.
		}

		// we duplicate by saving the desklet into an in-memory XML file, and then recreating from that
		// that in-memory XML file.
		CXMLSaver xmlSaver(L"AveDesk");
		xmlSaver.AddNode(L"desklet");
		d->SaveAsync(xmlSaver);

		CXMLLoader xmlLoader(xmlSaver.GetDOMDocument(),L"AveDesk");
		xmlLoader.SelectChildNode(L"desklet");

		POINT offsetPos = {10, 10};
		WCHAR dllFile[MAX_PATH] = {0};
		GetModuleFileNameW(d->GetDocklet()->GetModule(),dllFile,sizeof(dllFile)/sizeof(dllFile[0]));

		Docklet* d = Docklet::LoadDocklet(dllFile,&xmlLoader,NULL,NULL,false, offsetPos);
		/*
		if(d->WaitForDockletBeingInitialized(INFINITE))
		{
			// the new desklet will be moved 10 pixels to the right and 10 pixels down.
			POINT p = d->GetDocklet()->GetPos();
			p.x += 10;
			p.y += 10;
			d->GetDocklet()->SetPos(p.x,p.y);
		}*/
	}

	if(phwndTarget != NULL)
		*phwndTarget = hwndTemp;
}
//--------------------------------------------------------------------------------------------
/**
 * Redraws a desklet completely.
 * @param hwndDesklet the handle of the desklet to redraw
 */
void CALLBACK DeskletRedraw(HWND hwndDesklet)
// pre:  desklet(hwndDesklet) != NULL
// post: the desklet has been redrawn.
{
	// pre:  hwndDesklet is a valid desklet
	// post: the desklet is redrawn

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		d->GetDocklet()->SetIsDirty(true);
		d->GetDocklet()->InvalidateAll();
		d->Redraw();
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Moves a desklet.
 * @param hwndDesklet          the handle of the desklet to move
 * @param newBoundingRectangle a pointer to the RECTangle that holds the coords to move the desklet to.
 * @param redraw               if TRUE, the desklet will be redrawn immediately. Otherwise, the desklet
 *                             won't be redrawn until A redraw command has been issued.
 * @remark This function can be used to scale and move the desklet in the same time, because
 *         the RECTANGLE holds the 4 bounding-points and thus also defines the new width and height.
 * @remark 1.3 changes: added a const-modifier to newBoundingRectangle
 */
void CALLBACK DeskletMove(HWND hwndDesklet, const RECT* newBoundingRectangle, BOOL redraw)
// pre:  desklet(hwndDesklet) newBoundingRectangle != NULL, 
// post: the desklet has been moved and sized according to newBoundingRectangle, and has
//		 been redrawn if redraw is TRUE.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);

	if(d != NULL && newBoundingRectangle != NULL)
	{
		SIZE s = SIZEFROMRECT(*newBoundingRectangle);

		d->GetDocklet()->SetPos(newBoundingRectangle->left, newBoundingRectangle->top);
		d->GetDocklet()->SetSize(s.cx, s.cy);

		if(redraw == TRUE)
		{
			d->Redraw();
		}
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the visibility of the desklet
 * @param hwndDesklet the handle of the desklet to check the visibility of.
 * @return TRUE iff the desklet is visible.
 */
BOOL CALLBACK DeskletIsVisible(HWND hwndDesklet)
// pre:  desklet(hwndDesklet) != NULL
// post: TRUE has been returned iff the desklet is visible.
{
	return IsWindowVisible(hwndDesklet);
}

BOOL CALLBACK DockletIsVisible(HWND hwndDocklet)
// note: ObjectDock compatibility API of DeskletIsVisible()
{
	return DeskletIsVisible(hwndDocklet);
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the rectangle for a desklet
 * @param hwndDesklet the handle of the desklet to get the rect for.
 * @param rcDesklet   A pointer to the rectangle that will receive the rectangle of this desklet.
 *                    cannot be NULL.
 * @return TRUE iff the desklet is visible.
 */
BOOL CALLBACK DeskletGetRect(HWND hwndDesklet,RECT *rcDesklet)
// pre:  hwndDesklet is a valid desklet, rcDesklet is not NULL.
// post: TRUE has been returned iff the desklet is visible, and rcDesklet has been assigned
//		 the current bounding rectangle of the desklet.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL != d)
	{
		if(NULL != rcDesklet)
		{
			POINT p = d->GetDocklet()->GetPos();
			SIZE  s = d->GetDocklet()->GetSize();
			
			rcDesklet->left = p.x;
			rcDesklet->right = p.x + s.cx;
			rcDesklet->bottom = p.y + s.cy;
			rcDesklet->top = p.y;
		}
		return d->GetIsVisible();
	}
	else
		return FALSE;
}

/**
 * Object Dock compatibility function of DeskletGetRect
 * @see DeskletGetRect
 */
BOOL CALLBACK DockletGetRect(HWND hwndDocklet,RECT *rcDocklet)
{
	return DeskletGetRect(hwndDocklet,rcDocklet);
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the label for a desklet.
 * @param hwndDesklet the handle of the desklet to get the label for.
 * @param wLabel      a pointer to the buffer that will receive the label forthis desklet. cannot be NULL.
 * @param maxSize     the maximum size of the buffer pointed to by wLabel.
 * @return -1 if not succesfull (the desklet does not exist). Otherwise, the length of the label + 1 (zero-terminator)
 *               will be returned. If the buffer is not large enough to hold the desklet's label. Nothing will be
 *               copied.
 * @remark Function differs from the NON-UNICODE version
 * @remark The caller should check if maxSize >= the return value to check if there was actually data copied
 *         into the wLabel buffer.
 * @see DeskletGetLabel
 */
int CALLBACK DeskletGetLabelW(HWND hwndDesklet,WCHAR* wLabel, int maxSize)
{

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		if(NULL == wLabel)
			return d->GetLabel().GetMainCaption().size() + 1;
		else
		{
			int lblSize = d->GetLabel().GetMainCaption().size() + 1;

			// only copy if the caller has allocated enough space
			if(lblSize <= maxSize)
				wcscpy(wLabel,d->GetLabel().GetMainCaption().c_str() );

			return lblSize;
		}
	}
	else
	{
		return -1;
	}
}

/**
 * Gets the label of a desklet.
 * @param hwndDesklet the handle of the desklet to get the label for.
 * @param szLabel the buffer that will receive the label. The buffer should be able to hold 1024 characters, + 1 zero-terminator.
 * @return -1 if not successfull (the desklet does not exist), otherwise the number of characters copied or the length of the label
 *         if szLabel is NULL.
 * @remark NON-UNICODE version of DeskletGetLabelW
 * @see DeskletGetLabelW
 */
int CALLBACK DeskletGetLabel(HWND hwndDesklet,char *szLabel)
// pre:  hwndDesklet is a valid desklet, if szLabel is not NULL, szLabel must be large enough
//		 for the label to be copied into it.
// post: if szLabel is NULL, the number of characters in the label has been returned, otherwise
//		 the label has been copied into szLabel. On error -1 has been returned.

{
	const int MAXLABELSIZE = 1024;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL != d)
	{
		if(NULL == szLabel)
			return d->GetLabel().GetMainCaption().size() + 1;
		else
			return WideCharToMultiByte(GetACP(),0,d->GetLabel().GetMainCaption().c_str(),-1,szLabel,MAXLABELSIZE,NULL,NULL);
	}
	else
	{
		return -1;
	}
}


/**
 * ObjectDock compatibility function of DeskletGetLabel
 * @see DeskletGetLabel
 */
int CALLBACK DockletGetLabel(HWND hwndDocklet,char* szLabel)
{
	return DeskletGetLabel(hwndDocklet,szLabel);
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the label of a desklet.
 * @param hwndDesklet the handle of the desklet to set the label for.
 * @param szLabel     the text to set the label to. Cannot be NULL.
 * @return If successfull, the number of copied characters has been returned.
 *         Otherwise -1 has been returned.
 * @remark UNICODE version of DeskletSetLabel
 * @see DeskletSetLabel
 */
int CALLBACK DeskletSetLabelW(HWND hwndDocklet,const WCHAR *szLabel)
{

	Docklet* d = Docklet::FindDocklet(hwndDocklet);
	if(d != NULL && szLabel != NULL)
	{
		return d->SetMainLabel(szLabel);
	}
	else
	{
		return -1;
	}
}

/**
 * Sets the label of a desklet.
 * @param hwndDesklet the handle of the desklet to set the label for.
 * @param szLabel     the text to set the label to. Cannot be NULL.
 * @return If successfull, the number of copied characters has been returned.
 *         Otherwise -1 has been returned.
 * @remark NON-UNICODE version of DeskletSetLabelW
 * @see DeskletSetLabelW
 */
int CALLBACK DeskletSetLabel(HWND hwndDesklet, const char* szLabel)
{
	if(szLabel != NULL)
	{
		return DeskletSetLabelW(hwndDesklet,StringToWString(szLabel).c_str());
	}
	else
	{
		return -1;
	}
}

/**
 * ObjectDock compatibility function of DeskletSetLabel
 * @see DeskletSetLabel
 */
int CALLBACK DockletSetLabel(HWND hwndDocklet,const char *szLabel)
{
	return DeskletSetLabel(hwndDocklet,szLabel);
}

//--------------------------------------------------------------------------------------------

/**
 * Loads an Image File(png, jpg, bmp, ico) as an GDI HBITMAP object
 * @param szImage the path of the image file to load. Can be relative to the AveDesk folder.
 *                cannot be NULL.
 * @return If succesfull, an HBITMAP object of the image. Otherwise, NULL will be returned.
 * @remark The caller gets ownership of the GDI HBITMAP object and is thus responsible
 *         for releasing it.
 * @remark This function actually calls DeskletLoadGDIPlusImageW() and then convert the
 *         GDI+ bitmap to a HBITMAP
 * @version 1.3
 */

HBITMAP CALLBACK DeskletLoadHBITMAPImageW(const WCHAR *szImage)
{
	if(NULL == szImage)
		return NULL;

	HBITMAP hBitmap = NULL;

	boost::shared_ptr<Bitmap> bmp(DeskletLoadGDIPlusImageW(szImage));
	if(bmp.get())
	{
		bmp->GetHBITMAP(NULL, &hBitmap);	
	}

	return hBitmap;
}

/**
 * Loads an Image File (png, jpg, bmp, ico) as a GDI+ Bitmap objects
 * @param szImage the path of the image file to load. Can be relative to the AveDesk folder.
 *                cannot be NULL.
 * @return if successfull, a pointer to an GDI+ Bitmap object will be returned. Otherwise, NULL
 *         will be returned.
 * @remark The caller gets ownership of the returned GDI+ Bitmap object and is thus responsible
 *         for releasing it with C++'s delete function. 
 * @remark ONLY SUPPORTS VISUAL STUDIO 6, see DeskletLoadHBITMAPImageW for a language independant version.
 * @see DeskletLoadHBITMAPImageW
 * @remark UNICODE version of DeskletLoadGDIPlusImage
 * @see DeskletLoadGDIPlusImage 
 */
Bitmap* CALLBACK DeskletLoadGDIPlusImageW(const WCHAR *szImage)
{
	// FIX: added input parameter check
	if(NULL == szImage)
		return NULL;

	const int DEFAULT_WIDTH  = 128;
	const int DEFAULT_HEIGHT = 128;

	WCHAR path[MAX_PATH] = {0};
	if(wcsstr(szImage,L":")==0)wcscpy(path, _App->GetAveDeskDataDir().c_str());
	wcscat(path,szImage);

	if(!FileExistsW(path))
	{
		wcscpy(path, _App->GetAveDeskDataDir().c_str());
		wcscat(path,L"Images\\default.png");
	}

	WCHAR* ext = PathFindExtensionW(path);

	if( _wcsicmp(ext,L".ico")==0)//PathIsContentTypeW(path,L"image//icon") )
	{

		HICON icon = (HICON) LoadImageW(0,path, IMAGE_ICON, DEFAULT_WIDTH, DEFAULT_HEIGHT, LR_LOADFROMFILE);

		Bitmap* bmp = HICONToGDIPlusBitmap(icon);
		
		DestroyIcon(icon);

		return bmp;
		
	}
	else
	{	
		return new Bitmap(path);
	}
}


/**
 * Loads an Image File (png, jpg, bmp, ico) as a GDI+ Bitmap objects
 * @param szImage the path of the image file to load. Can be relative to the AveDesk folder.
 *                cannot be NULL.
 * @return if successfull, a pointer to an GDI+ Bitmap object will be returned. Otherwise, NULL
 *         will be returned.
 * @remark The caller gets ownership of the returned GDI+ Bitmap object and is thus responsible
 *         for releasing it with C++'s delete function. 
 * @remark ONLY SUPPORTS VISUAL STUDIO 6, see DeskletLoadHBITMAPImageW for a language independant version.
 * @see DeskletLoadHBITMAPImageW
 * @remark NON-UNICODE version of DeskletLoadGDIPlusImageW
 * @see DeskletLoadGDIPlusImageW
 */
Bitmap* CALLBACK DeskletLoadGDIPlusImage(const char *szImage)
{
	if(szImage != NULL)
	{
		std::wstring wImagePath( StringToWString(szImage) );
		return DeskletLoadGDIPlusImageW(wImagePath.c_str() );
	}
	else
	{
		return DeskletLoadGDIPlusImageW(NULL);
	}
}

/**
 * ObjectDock compatibility function of DeskletLoadGDIPlusImage
 * @see DeskletLoadGDIPlusImage
 */
Bitmap* CALLBACK DockletLoadGDIPlusImage(const char *szImage)
{
	return DeskletLoadGDIPlusImage(szImage);
}
//-----------------------------------------------------------------------------

/**
 * Sets the image for this desklet.
 * @param hwndDesklet the handle of the desklet to set the image for.
 * @param lpImageNew the image to set.
 * @param bAutomaticallyDeleteImage, if TRUE, AveDesk Host Application gets ownership of
 *        lpImageNew and is responsible for freeing its memory.
 * @remark ONLY COMPATIBLE WITH VISUAL STUDIO 6. USE DeskletSetHBITMAP for a language-
 *         independent function.
 * @see DeskletSetHBITMAP
 */
void CALLBACK DeskletSetImage(HWND hwndDesklet,Image *lpImageNew,BOOL bAutomaticallyDeleteImage)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		d->SetIconFileName("");
		d->GetDocklet()->SetNormalImage(lpImageNew,bAutomaticallyDeleteImage!=TRUE);
		d->Redraw();
	}
	else
	{
		if(bAutomaticallyDeleteImage)
			delete lpImageNew;
	}
}

void CALLBACK DeskletSetHBITMAP(HWND hwndDesklet, HBITMAP hBitmap, BOOL bAutomaticallyDeleteImage)
{
	// TODO: ADD CODE!
}

/**
 * ObjectDock compatibility function of DeskletSetImage
 * @see DeskletSetImage
 */
void CALLBACK DockletSetImage(HWND hwndDocklet, Image *lpImageNew,BOOL bAutomaticallyDeleteImage)
{
	DeskletSetImage(hwndDocklet,lpImageNew,bAutomaticallyDeleteImage);
}
//--------------------------------------------------------------------------------------------
/**
 * Adds a FORMATETC to the list of `dragformats` that are accepted exported
 * by this desklet.
 * @param hwndDesklet the handle of the desklet to add the dragformat to
 * @param format the FORMATETC describing the dragformat to add.
 * @remark for more info on FORMATETC and OLE drag-'n-drop, see the MSDN.
 */
void CALLBACK DeskletAddDragDropFormat(HWND hwndDesklet,FORMATETC* format)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL && format != NULL)
	{
		d->AddFormat(*format);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Removes a FORMATETC from the list of `dragformats` that are accepted exported
 * by this desklet.
 * @param hwndDesklet the handle of the desklet to remove the dragformat from
 * @param format      the FORMATETC describing the dragformat to remove.
 * @remark for more info on FORMATETC and OLE drag-'n-drop, see the MSDN.
 */
void CALLBACK DeskletRemoveDragDropFormat(HWND hwndDesklet,const FORMATETC* format)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL && format != NULL)
	{
		// FIX: bug a drag format was actually added it, while it should have been
		//      removed
		d->RemoveFormat(*format);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets an image for a desklet by using a filename.
 * @param hwndDesklet   the handle of the desklet to set the image for.
 * @param imageFilename the filename of the image to set. If imageFilename is NULL,
 *                      then the current image will be removed.
 * @remark UNICODE version of DeskletSetImageFile
 * @see DeskletSetImageFile
 */
void CALLBACK DeskletSetImageFileW(HWND hwndDesklet, const WCHAR *imageFilename)
{

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		if(NULL == imageFilename)
		{
			d->SetIconFileName("");
			d->GetDocklet()->SetNormalImage(NULL,false);
			d->Redraw();
		}
		else if(strcmp(WStringToString(imageFilename).c_str(),"")==0 || !strcmp_nocase(d->GetIconFileName(),WStringToString(imageFilename).c_str()))
		{
			d->SetIconFileName(WStringToString(imageFilename).c_str());
			d->GetDocklet()->SetNormalImage(DeskletLoadGDIPlusImageW(imageFilename),false);
			d->Redraw();
		}	
	}
}

/**
 * Sets an image for a desklet by using a filename.
 * @param hwndDesklet   the handle of the desklet to set the image for.
 * @param imageFilename the filename of the image to set. If imageFilename is NULL,
 *                      then the current image will be removed.
 * @remark NON-UNICODE version of DeskletSetImageFileW
 * @see DeskletSetImageFileW
 */
void CALLBACK DeskletSetImageFile(HWND hwndDesklet,const char *imageFilename)
{
	if(imageFilename != NULL)
	{
		DeskletSetImageFileW(hwndDesklet, StringToWString(imageFilename).c_str());
	}
	else
	{
		DeskletSetImageFileW(hwndDesklet, NULL);
	}
}

/**
 * Object Dock compatibility function of DeskletSetImageFile
 * @see DeskletSetImageFile
 *
 */
void CALLBACK DockletSetImageFile(HWND hwndDocklet,const char *imageFilename)
{
	DeskletSetImageFile(hwndDocklet,imageFilename);
}
//--------------------------------------------------------------------------------------------
/**
 * Object Dock compatibility function of DeskletSetImageOverlay
 * @see DeskletSetImageOverlay
 */
void CALLBACK DockletSetImageOverlay(HWND hwndDocklet,Image *lpImageNew, BOOL bAutomaticallyDeleteImage)
{
	Docklet* d = Docklet::FindDocklet(hwndDocklet);
	if(d != NULL)
	{
		d->GetDocklet()->SetOverlayImage(lpImageNew,bAutomaticallyDeleteImage!=TRUE);
		d->Redraw();
	}
	else
	{
		if(bAutomaticallyDeleteImage)delete lpImageNew;
	}
}
/*
 * Sets an overlay image for a desklet.
 * @remark Programmers are encouraged to use the DeskletLayer* set of functions.
 * @deprecated
 */
void CALLBACK DeskletSetImageOverlay(HWND hwndDesklet, Image *lpImageNew,BOOL bAutomaticallyDeleteImage)
{
	DockletSetImageOverlay(hwndDesklet,lpImageNew,bAutomaticallyDeleteImage);
}
//--------------------------------------------------------------------------------------------
/**
 * Object Dock compatibility function of DeskletSetImageFile.
 * @see DeskletSetImageFile
 */
void CALLBACK DockletSetImageOverlayFile(HWND hwndDocklet,char *szImage)
{
	Docklet* d = Docklet::FindDocklet(hwndDocklet);
	if(d)
	{
		d->GetDocklet()->SetOverlayImage(DockletLoadGDIPlusImage(szImage),false);
		d->Redraw();
	}
}
/**
 * Sets an overlay imagefile for a desklet.
 * @remark Programmers are encouraged to use the DeskletLayer* set of functions.
 * @deprecated
 */
void CALLBACK DeskletSetImageOverlayFile(HWND hwndDesklet,char *szImage)
{
	DockletSetImageOverlayFile(hwndDesklet,szImage);
}
//--------------------------------------------------------------------------------------------
/**
 * Shows a browse-for-image dialog to the user.
 * @param HWND hwndParent - the parent for the browse-for-image-dialog.
 * @param szImage the buffer to receive the relative path to the selected image file. 
 *        szImage cannot be NULL and must be at least MAX_PATH characters in length.
 *		  The default selected file will also be based on szImage.
 * @return TRUE iff the user selected a new imagefile, otherwise false.
 * @remark UNICODE version of DeskletBrowseForImage
 * @see DeskletBrowseForImage
 */
BOOL CALLBACK DeskletBrowseForImageW(HWND hwndParent, WCHAR *szImage)
{
	if(NULL == szImage)
		return FALSE;

	WCHAR path[MAX_PATH+1] = {0};
	if(szImage[0]!=0 && wcsstr(szImage,L":")==0)
		wcscpy(path, _App->GetAveDeskDataDir().c_str());

	if(szImage[0]==L'\\')
	{
		wcscat(path,szImage+1);
	}
	else
	{
		wcscat(path,szImage);
	}

	WCHAR tmpPath[MAX_PATH] = {0};
	wcscpy(tmpPath,path);

	int i=0,j=0;
	while(tmpPath[i]!=0)
	{
		if(tmpPath[i+1]!=0 && tmpPath[i]==L'\\' && tmpPath[i+1]==L'\\')	i++;
		path[j++]=tmpPath[i];
		i++;
	}
	path[j]=0;
	

	OPENFILENAMEW ofn = {0};			
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndParent;
	ofn.hInstance = _Module.GetModuleInstance();
	ofn.lpstrFilter = L"Graphics files(*.png,*.ico)\0*.png;*.ico;\0\0";
	ofn.lpstrFile = path; 
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.nMaxFileTitle = MAX_PATH;

	INT result =  GetOpenFileNameW(&ofn);

	if(result)
	{
		WCHAR rPath[MAX_PATH] = {0};
		wcscpy(rPath, _App->GetAveDeskDataDir().c_str());
		if(wcsstr(path,rPath)!=0)
		{
			wcscpy(szImage,path+wcslen(rPath));
		}
		else
		{
			wcscpy(szImage,path);
		}
	}

	return result;
}

/**
 * Shows a browse-for-image dialog to the user.
 * @param HWND hwndParent - the parent for the browse-for-image-dialog.
 * @param szImage the buffer to receive the relative path to the selected image file. 
 *        szImage cannot be NULL and must be at least MAX_PATH characters in length.
 *		  The default selected file will also be based on szImage.
 * @return TRUE iff the user selected a new imagefile, otherwise false.
 * @remark NON-UNICODE version of DeskletBrowseForImageW
 * @see DeskletBrowseForImageW
 */
BOOL CALLBACK DeskletBrowseForImage(HWND hwndParent,char *szImage)
{
	if(NULL == szImage)
		return FALSE;

	WCHAR wImage[MAX_PATH] = {0};
	MultiByteToWideChar(GetACP(),0,szImage,-1,wImage,MAX_PATH);

	BOOL res = DeskletBrowseForImageW(hwndParent,wImage);

	if(res)
		WideCharToMultiByte(GetACP(),0,wImage,-1,szImage,MAX_PATH,NULL,NULL);

	return res;
}

/**
 * Object Dock compatibility function of DeskletBrowseForImage.
 * @see DeskletBrowseForImage
 * @remark different set of parameters than DeskletBrowseForImage. Last parameter unused in AveDesk.
 */
BOOL CALLBACK DockletBrowseForImage(HWND hwndParent,char *szImage,char* /*szAlternateRelativeRoot*/)
{

	return DeskletBrowseForImage(hwndParent,szImage);

}
//--------------------------------------------------------------------------------------------
/**
 * Object Dock compatibility function for DockletLockMouseEffect
 * @remark NOT SUPPORTED BY AVEDESK
 */
void CALLBACK  DockletLockMouseEffect(HWND hwndDocklet,BOOL bLock)
{ 
	// not supported
}
//--------------------------------------------------------------------------------------------
/**
 * DEPRECATED
 * @deprecated
 */
void CALLBACK DeskletDoAttentionAnimation(HWND hwndDesklet, INT iAnimationType, UINT speed)
{
	// not supported anymore
}

/**
 * Object Dock compatibility function of DeskletDoAttentionAnimation
 * @remark NOT SUPPORT BY AVEDESK
 */
void CALLBACK DockletDoAttentionAnimation(HWND hwndDocklet)
{
	// not supported anymore
	//DeskletDoAttentionAnimation(hwndDocklet,ANIMATION_FADEOUTANDIN,1);
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the relative folder for a desklet.
 * @param hwndDesklet the desklet to retrieve the relative folder for.
 * @param folder the buffer to receive the relative folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark UNICODE version of DeskletGetRelativeFolder.
 * @see DeskletGetRelativeFolder
 */
void CALLBACK DeskletGetRelativeFolderW(HWND hwndDesklet,WCHAR* folder)
{
	if(NULL == folder)
		return;

	// folder must be at least MAX_PATH as said in the docs.

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		WCHAR path[MAX_PATH] = {0};
		WCHAR pathDocklet[MAX_PATH] = {0};
		wcscpy(path, _App->GetAveDeskDataDir().c_str());
		GetDLLPathW(d->GetDocklet()->GetModule(),pathDocklet,sizeof(pathDocklet)/sizeof(pathDocklet[0]));

		lowercaseW(path);
		lowercaseW(pathDocklet);

		wcscpy(folder,pathDocklet+wcslen(path));

	}
}

/**
 * Gets the relative folder for a desklet.
 * @param hwndDesklet the desklet to retrieve the relative folder for.
 * @param folder the buffer to receive the relative folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark NON-UNICODE version of DeskletGetRelativeFolderW
 * @see DeskletGetRelativeFolderW
 */
void CALLBACK DeskletGetRelativeFolder(HWND hwndDocklet,char* szFolder)
{
	if(szFolder)
	{
		WCHAR tmp[MAX_PATH+1] = {0};
		DeskletGetRelativeFolderW(hwndDocklet,tmp);
		WideCharToMultiByte(GetACP(),0,tmp,-1,szFolder,MAX_PATH,NULL,NULL);
	}
}

/**
 * Object Dock compatibility function of DeskletGetRelativeFolder.
 */
void CALLBACK DockletGetRelativeFolder(HWND hwndDesklet, char* szFolder)
{
	DeskletGetRelativeFolder(hwndDesklet,szFolder);
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the root folder for avedesk.
 * @param hwndDesklet the desklet to retrieve the root folder for.
 * @param folder the buffer to receive the root folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark UNICODE version of DeskletGetRootFolder.
 * @see DeskletGetRootFolder
 */
void CALLBACK DeskletGetRootFolderW(HWND hwndDesklet,WCHAR* folder)
{
	if(folder)
	{
		wcscpy(folder, _App->GetAveDeskDataDir().c_str());
	}
}

/**
 * Gets the root folder for avedesk.
 * @param hwndDesklet the desklet to retrieve the root folder for.
 * @param folder the buffer to receive the root folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark NON-UNICODE version of DeskletGetRootFolderW.
 * @see DeskletGetRootFolderW
 */

void CALLBACK DeskletGetRootFolder(HWND hwndDesklet,char *szFolder)
{
	if(szFolder)
	{
		GetDLLPath(GetModuleHandle(0),szFolder,MAX_PATH);
	}
}

/**
 * Object Dock compatibility function for DeskletGetRootFolder.
 */
void CALLBACK DockletGetRootFolder(HWND hwndDocklet,char *szFolder)
{
	DeskletGetRootFolder(hwndDocklet,szFolder);
}

//--------------------------------------------------------------------------------------------
/**
 * Shows the default property-dialog for a desklet.
 * @deprecated
 * @remark Use the OnAddPropertySheet functionality
 */
void CALLBACK DeskletDefaultConfigDialog(HWND hwndDesklet,PropertyDialogStruct* pds)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);

	if(d != NULL && !d->configDlgIsVisible)
	{
		if(pds != NULL)
		{
			pds->lParam = (LPARAM)d;
			d->AsyncDeskletPropertySheet(NULL,pds);
		}
		else
		{
			d->AsyncDeskletPropertySheet(NULL,NULL);
		}
	}
}

/**
 * Object Dock compatibilty function of DeskletDefaultConfigDialog.
 * @remark different parameters. AveDesk's function uses one parameter more.
 */
void CALLBACK DockletDefaultConfigDialog(HWND hwndDocklet)
{
	DeskletDefaultConfigDialog(hwndDocklet,NULL);
}
//--------------------------------------------------------------------------------------------
/**
 * Object Dock compatibility function.
 * @remark emulates the behaviour of the dock by taking the position of the desklet into account for
 *			emulating the side of the dock, thus per desklets result.
 */
int CALLBACK DockletQueryDockEdge(HWND hwndDocklet)
{
	const int BOTTOM	= 0;
	const int TOP		= 1;
	const int LEFT		= 2;
	const int RIGHT		= 3;
	RECT r = {0};
	GetWindowRect(hwndDocklet,&r);

	// FIX: went from SM_CXSCREEN to SM_CXVIRTUALSCREEN (idem for Y),
	//		to support multimonitor setups.
	UINT topDif		= r.top;
	UINT leftDif	= r.left;
	UINT bottomDif	= GetSystemMetrics(SM_CYVIRTUALSCREEN) - r.bottom;
	UINT rightDif	= GetSystemMetrics(SM_CXVIRTUALSCREEN) - r.right;


	if(topDif <= leftDif && topDif <= leftDif && topDif <= bottomDif)return TOP;
	else if(leftDif <= topDif && leftDif <= bottomDif && leftDif <= rightDif) return LEFT;
	else if(rightDif <= topDif && rightDif <= leftDif && rightDif <= bottomDif)return RIGHT;
	else return BOTTOM;

}
//--------------------------------------------------------------------------------------------
/**
 * Object Dock compatibility function.
 * @remark does nothing, since desklets are not docked to a dock.
 */
int CALLBACK DockletSetDockEdge(HWND hwndDocklet,int iNewEdge)
{
	// empty
	return DockletQueryDockEdge(hwndDocklet);
}
//--------------------------------------------------------------------------------------------
/**
 * Object Dock compatibility function.
 * @remark does nothing, since desklets are not docked to a dock.
 */
int CALLBACK DockletQueryDockAlign(HWND hwndDocklet)
{
	// empty
	return 0;
}
//--------------------------------------------------------------------------------------------
/**
 * Object Dock compatibility function.
 * @remark does nothing, since desklets are not docked to a dock.
 */
int CALLBACK DockletSetDockAlign(HWND hwndDocklet, int iNewAlign)
{
	// empty
	return DockletQueryDockAlign(hwndDocklet);
}
//--------------------------------------------------------------------------------------------
// EXTENDED DESKLET API


// helper function
BOOL RemoveDeskletAndSaveConfig(HWND hwndDesklet, BOOL Ask, BOOL saveConfig)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		if(_globalSettings.GetAsk() && Ask)
		{
			CString question;
			question.LoadString(IDS_REMOVEDESKLETQUESTION);
			CString caption;
			caption.LoadString(IDS_REMOVEDESKLETCAPTION);
			if(MessageBox(hwndDesklet,question,caption,MB_ICONQUESTION | MB_YESNO)==IDNO)
				return FALSE;
		}

		d = Docklet::FindDocklet(hwndDesklet);
		if(d != NULL)
		{
			// save if needed
			if(saveConfig && FALSE) // TODO: make a user setting?
			{
				USES_CONVERSION;
				CString fileName = W2T(const_cast<WCHAR*>(d->GetRelativeDLLFile().c_str()));
				fileName.Replace(TEXT('\\'),TEXT('_'));
				fileName.Replace(TEXT('/'),TEXT('_'));
		
				WCHAR pathName[MAX_PATH] = {0};
				wcscpy(pathName,_App->GetAveDeskUserPath().c_str());
				PathAppendW(pathName,L"\\Defaults\\");

				if(!PathFileExistsW(pathName))
					CreateDirectoryW(pathName,NULL);

				PathAppendW(pathName,T2W(const_cast<TCHAR*>(fileName.operator LPCTSTR())));
				PathRenameExtensionW(pathName,L".avedesklet");
				d->SaveToSingleFile(pathName);
			}

			d->BeforeRemoval();
			d->Remove();
			//d->StartSucking(true);
			
			// there is no guarantee anymore that the docklet is actually removed when this function returns.
			// might consider waiting for the remove to be done:
			//WaitForSingleObject(d->GetIsInitializedEventHandle(),INFINITE);

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/**
 * Removes an desklet instance.
 * @param hwndDesklet the handle of the desklet instance to remove.
 * @param Ask iff TRUE the user will be asked if the desklet should be removed. If FALSE, the
 *				the desklet will be removed without asking the user.
 * @return TRUE iff the desklet will be removed, otherwise FALSE.
 *
 */
BOOL CALLBACK DeskletRemove(HWND hwndDesklet, BOOL Ask)
{
	return RemoveDeskletAndSaveConfig(hwndDesklet,Ask,FALSE);
}
//--------------------------------------------------------------------------------------------
/**
 * Loads and creates a desklet from an INI file.
 * @deprecated
 * @see DeskletLoadFromConfigW
 */
HWND CALLBACK DeskletLoadFromIniW(const WCHAR* filename, const WCHAR* ini, const WCHAR* iniGroup, void** p, BOOL makeVisible)
{
	if(NULL == filename)
		return NULL;

	Docklet* d = Docklet::LoadDocklet(filename,NULL,NULL,NULL,true);
	WaitForSingleObject(d->GetIsInitializedEventHandle(),INFINITE);
	if(d != NULL)
	{
		d->FadeIn(_AnimTimeInMS);

		if(p != NULL)
			*p = static_cast<void*>(d->GetDocklet()->GetPointer() );

		return d->GetHWnd();

	}
	else
	{
		return NULL;
	}
}

/**
 * Loads and creates a desklet.
 * @see DeskletLoadFromDLLW
 */
HWND CALLBACK DeskletLoad(char* filename,char* ini, char* iniGroup,void** p,BOOL makeVisible)
{
	if(NULL == filename)
		return NULL;

	std::wstring wFilename = StringToWString(filename);

	if(ini != NULL && iniGroup != NULL)
		return DeskletLoadFromIniW(wFilename.c_str(),StringToWString(ini).c_str(),StringToWString(iniGroup).c_str(),p,makeVisible);
	else
		return DeskletLoadFromIniW(wFilename.c_str(),NULL,NULL,p,makeVisible);
}
//--------------------------------------------------------------------------------------------
/**
 * Hides a desklet.
 * @param hwndDesklet the handle of the desklet to hide.
 */
void CALLBACK DeskletHide(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		d->SetVisibility(false);
	}

}
//--------------------------------------------------------------------------------------------
/**
 * Shows a desklet.
 * @param hwndDesklet the handle of the desklet to show.
 */
void CALLBACK DeskletShow(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		d->SetVisibility(true);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the position of desklet.
 * @param hwndDesklet the handle of the desklet to set the position of.
 * @param p a pointer to the POINT structure describing the new position of the desklet.
 */
void CALLBACK DeskletSetPosition(HWND hwndDesklet,POINT *p)
{
	if(NULL == p)
		return;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		d->GetDocklet()->SetPos(p->x,p->y,false);
		d->UpdateLabelPosition();
		POINT p2 = *p;
		if(d->GetDocklet()->GetDrawShadow())
		{
			p2.x -= Layer::ratio/10;
			p2.y -= Layer::ratio/10;
		}

		// FIX: multimonitor fix for desktop icon style; use ULW instead of old window-manager APIs.
		UpdateLayeredWindow(hwndDesklet, NULL, &p2, NULL, NULL, NULL, 0, NULL, 0);
		//MoveWindow(hwndDesklet,p2.x,p2.y,s.cx,s.cy,FALSE);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the size of desklet.
 * @param hwndDesklet the handle of the desklet to set the size of.
 * @param s a pointer to the SIZE structure describing the new size of the desklet.
 * @remark this function will always cause a total redraw immediately
 */
void CALLBACK DeskletSetSize(HWND hwndDesklet,SIZE *s)
{
	if(NULL == s)
		return;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		d->GetDocklet()->SetSize(s->cx,s->cy);
		d->Redraw();
		d->UpdateLabelPosition();
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the alpha-level of desklet.
 * @param hwndDesklet the handle of the desklet to set the alpha of.
 * @param alpha the new alpha level for the desklet range [0,255].
 */
void CALLBACK DeskletSetAlpha(HWND hwndDesklet,BYTE alpha)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		d->GetDocklet()->SetAlpha(alpha);
		d->SetAlpha(alpha);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the rotation of desklet.
 * @param hwndDesklet the handle of the desklet to set the rotation of.
 * @param p rotation the new rotation value for the desklet
 * @remark this function will cause a total redraw immediately.
 */
void CALLBACK DeskletSetRotation(HWND hwndDesklet,INT rotation)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		d->GetDocklet()->SetRotation(rotation);
		d->Redraw();
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the position of a desklet.
 * @param hwndDesklet the handle of the desklet to get the position of.
 * @param p a pointer to a POINT structure receiving the position of the desklet. Cannot be NULL.
 */
void CALLBACK DeskletGetPosition(HWND hwndDesklet,POINT *p)
{
	if(NULL == p)
		return;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		*p = d->GetDocklet()->GetPos();
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the size of a desklet.
 * @param hwndDesklet the handle of the desklet to get the size of.
 * @param s a pointer to a SIZE structure receiving the size of the desklet. Cannot be NULL.
 */
void CALLBACK DeskletGetSize(HWND hwndDesklet,SIZE *s)
{
	if(NULL == s)
		return;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		*s = d->GetDocklet()->GetSize();
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the alpha-level of a desklet.
 * @param hwndDesklet the handle of the desklet to get the alpha of.
 * @return the current alpha-level of the desklet, range [0,255].
 */
BYTE CALLBACK DeskletGetAlpha(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->GetDocklet()->GetAlpha();
	}
	else
	{
		return 0;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the rotation of a desklet.
 * @param hwndDesklet the handle of the desklet to get the rotation of.
 * @return the current rotation of the desklet.
 */
INT CALLBACK DeskletGetRotation(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->GetDocklet()->GetRotation();
	}
	else
	{
		return 0;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the ID of the OK button for a desklet propertysheet.
 * @deprecated
 * @remark programmers are encouraged to use the AddOnPropertySheet functionality
 */
UINT CALLBACK DeskletGetParentOK()
{
	return IDOK;
}
//--------------------------------------------------------------------------------------------
/**
 * Performs an avedesk manager function.
 * @param action, the action to perform. must be one of the following:
 *					DM_CONTROLPANEL, opens the control panel
 *					DM_SAVE, saves the current theme
 *					DM_ABOUT, opens the about dialog
 *					DM_HELP, opens the help
 *					DM_EXIT, closes avedesk
 *					DM_GLOBALSETTINGS, opens the global settings dialog 
 *					DM_DEFAULTDESKLETPROPERTIES, opens the default desklet properties dialog
 *					DM_ADDDESKLET, opens the add-desklet dialog.
 */
void CALLBACK DeskletManager(UINT action)
{
	const int DM_CONTROLPANEL = 0;
	const int DM_SAVE = 1;
	const int DM_ABOUT = 2;
	const int DM_HELP = 3;
	const int DM_EXIT = 4;
	const int DM_GLOBALSETTINGS = 5;
	const int DM_DEFAULTDESKLETPROPERTIES = 6;
	const int DM_ADDDESKLET = 7;

	WCHAR helpPath[MAX_PATH] = {0};
	wcscpy(helpPath, _App->GetAveDeskDataDir().c_str());
	wcscat(helpPath,L"Help\\index.html");

	switch(action)
	{
		case DM_CONTROLPANEL:	_App->ShowControlPanel();break;
		case DM_SAVE:			_App->SaveTheme(CComBSTR(_App->GetThemeFileName().c_str()) );break;
		case DM_ABOUT:			{CAboutDlg dlg; dlg.DoModal();};break;
		case DM_HELP:			ShellExecuteW(0,L"open",helpPath,0,helpPath,SW_NORMAL);break;
		case DM_EXIT:			PostQuitMessage(0);break;
		case DM_GLOBALSETTINGS:	_globalSettings.ShowDialog();break;
		case DM_DEFAULTDESKLETPROPERTIES: {Docklet::defaultDocklet->DeskletPropertySheet(NULL);}break;
		case DM_ADDDESKLET:		{ CAddDeskletDlg dlg; dlg.DoModal();};break;	
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets a desklet to be static.
 * @deprecated
 */
void CALLBACK DeskletSetStatic(HWND hwndDesklet, BOOL staticValue)
{
	// unsupported in 1.1
}
//--------------------------------------------------------------------------------------------
/**
 * Activates Extended (OLE) drag&drop for a desklet.
 * @param hwndDesklet the handle of the desklet to activate drag and drop for
 * @remark call this function if you want your desklet to be able to receive drags
 */
void CALLBACK DeskletActivateExtendedDragDrop(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		RegisterDragDrop(hwndDesklet,d);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Deactivates Extended (OLE) drag&drop for a desklet.
 * @param hwndDesklet the handle of the desklet to deactivate drag and drop for.
 */
void CALLBACK DeskletDeactivateExtendedDragDrop(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		RevokeDragDrop(hwndDesklet);
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Gets the COM IDesklet pointer wrapper for a desklet.
 * @param hwndDesklet the handle of the desklet to retrieve the wrapper COM IDesklet pointer for.
 * @param ptr, A pointer to the pointer receiving the IDesklet object.
 * @remark caller must call AddRef()
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletGetWrapper(HWND hwndDesklet, /*out, retval*/ IDesklet **ptr)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL && ptr != NULL && SUCCEEDED( d->GetWrapper(ptr) ) )
	{
		(*ptr)->Release();
		// caller must call AddRef
		return TRUE;
	}

	// FIX: set ptr to NULL if it wasn't assigned.
	if(ptr != NULL)
		*ptr = NULL;

	return FALSE;
}
//--------------------------------------------------------------------------------------------
/**
 * Fades a desklet in.
 * @param hwndDesklet the handle of the desklet to fade in.
 * @param async iff TRUE, the function will return before the fade finished, otherwise
 *				the function will return if the fade is completed.
 * @param durationInMs the time the fade in animation should take, in MS. DEFAULT_DURATION
 *						can be passed to use the default duration.
 * @remark makes a desklet visible if it's hidden, so this function is nice for showing a 
 *			a desklet when it's hidden.
 */
void CALLBACK DeskletFadeIn(HWND hwndDesklet, BOOL async, DWORD durationInMs)
{
	const int DEFAULT_DURATION = -1;

	if(DEFAULT_DURATION == durationInMs)
		durationInMs = ::_AnimTimeInMS;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		if(d->GetIsVisible() == false)
		{
			VARIANT_BOOL scIsActive = {FALSE};
			_ShowCase->get_Active(&scIsActive);
			if(d->GetIsInitialized() && (d->GetStyle() != STYLE_SHOWCASE || scIsActive == VARIANT_TRUE))
			{
		
				if(FALSE == async)
				{
					d->SendMessage(WM_AVEACTION,static_cast<WPARAM>(durationInMs),AVEACTION_FADEIN);
					d->SetVisibility(true);
				}
				else
				{
					d->AsyncFade(true,durationInMs);
				}
			}
			else
			{
				d->SetVisibility(true);
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Fades a desklet out.
 * @param hwndDesklet the handle of the desklet to fade out.
 * @param async iff TRUE, the function will return before the fade finished, otherwise
 *				the function will return if the fade is completed.
 * @param durationInMs the time the fade out animation should take, in MS. DEFAULT_DURATION
 *						can be passed to use the default duration.
 * @remark makes a desklet hidden if it's visible, so this function is nice for hiding a 
 *			a desklet when it's visible.
 */
void CALLBACK DeskletFadeOut(HWND hwndDesklet, BOOL async, DWORD durationInMs)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);

	if(durationInMs == -1)
		durationInMs = ::_AnimTimeInMS;

	if(d)
	{
		if(d->GetIsVisible() == true)
		{

			VARIANT_BOOL scIsActive = {FALSE};
			_ShowCase->get_Active(&scIsActive);
			if(d->GetIsInitialized() && (d->GetStyle() != STYLE_SHOWCASE || scIsActive == VARIANT_TRUE) )
			{
		
				if(FALSE == async)
				{
					d->SendMessage(WM_AVEACTION,static_cast<WPARAM>(durationInMs),AVEACTION_FADEOUT);
					d->SetVisibility(false);
				}
				else
				{
					d->AsyncFade(false,durationInMs);
				}
			}
			else
			{
				d->SetVisibility(false);
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Saves a desklet instance to a file.
 * @param hwndDesklet the handle of the desklet to save.
 * @param fileName the name of the file to save this desklet instance to. Cannot be NULL.
 * @return TRUE iff successfull, FALSE otherwise.
 * @remark only a UNICODE version of this function is available.
 */
BOOL CALLBACK DeskletSaveW(HWND hwndDesklet, const WCHAR* fileName)
{
	if(NULL == fileName)
		return FALSE;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		HRESULT hRes = d->SaveToSingleFile(fileName);
		return SUCCEEDED(hRes);
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Loads and creates a desklet from a DLL.
 * @param dllFileName the filename of the dll of the desklet to create an instance from. Cannot be NULL.
 * @param configFileName optional filename of the configuration file for the newly created desklet. Can be NULL,
 *							in which case no configuration file will be used.
 * @param pointer optional, a pointer to a receiving pointer that is returned by OnCreate(), if successfull. Can
 *				  be NULL if not used.
 * @param makeVisible if TRUE, the desklet will be made visible directly, otherwise not.
 * @return the handle of the new desklet iff successfull, otherwise NULL.
 * @remark only a UNICODE version of this function is available.
 */
HWND CALLBACK DeskletLoadFromDLLW(const WCHAR* dllFileName, const WCHAR* configFileName, void** pointer,
								  BOOL makeVisible)
{
	try
	{
		Docklet* d = NULL;

		if(configFileName)
		{
			CXMLLoader loader(configFileName,L"AveDesk");
			CHECKHRES(loader.SelectChildNode(L"Desklet"));
	
			d = Docklet::LoadDocklet(dllFileName,&loader,NULL,NULL,makeVisible == FALSE);
		}
		else
		{
			d = Docklet::LoadDocklet(dllFileName,NULL,NULL,NULL,makeVisible == FALSE);
		}

		if(NULL == d)
			throw static_cast<HRESULT>(E_FAIL);

			d->WaitForDockletBeingInitialized(INFINITE);
			
		if(NULL != pointer)
			*pointer = reinterpret_cast<void*>(d->GetDocklet()->GetPointer());

		return d->GetHWnd();

	}
	catch(HRESULT&)
	{
		if(NULL != pointer)
			*pointer = NULL;

		return NULL;	
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Loads and creates a desklet from a config file.
 * @param fileName the filename of a configuration file to create a desklet from. Cannot be NULL.
 * @param pointer optional, a pointer to a receiving pointer that is returned by OnCreate(), if successfull. Can
 *				  be NULL if not used.
 * @param makeVisible if TRUE, the desklet will be made visible directly, otherwise not.
 * @return the handle of the new desklet iff successfull, otherwise NULL.
 * @remark only a UNICODE version of this function is available.
 */
HWND CALLBACK DeskletLoadFromConfigW(const WCHAR* fileName, void** pointer, BOOL makeVisible)
{
	Docklet* d = NULL;
	HRESULT hRes = S_OK;

	hRes = Docklet::LoadFromSingleFile(fileName,&d,makeVisible);
	if(FAILED(hRes) || NULL == d)
	{
		if(NULL != pointer)
			*pointer = NULL;

		return NULL;
	}
	else
	{
		d->WaitForDockletBeingInitialized(INFINITE);
			
		if(NULL != pointer)
			*pointer = reinterpret_cast<void*>(d->GetDocklet()->GetPointer());

		return d->GetHWnd();
	}
}
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *DESKLETENUMPROC)(HWND,LPARAM);
/**
 * Enumerates the handles of all running desklet instances.
 * @param enumProc a function pointer to a DESKLETENUMPROC function that will be called for each desklet handle.
 *					cannot be NULL.
 * @param lParam a value that will be passes to each call of enumProc.
 * @remark enumProc will be called as long as there are desklet handles AND enumProc returns TRUE.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletEnumRunningDesklets(DESKLETENUMPROC enumProc, LPARAM lParam)
{
	if(NULL == enumProc)
		return FALSE;

	std::vector<HWND> docklets;

	EnterCriticalSection(&Docklet::cs);
	for(Docklet::DockletsMap::iterator iter = Docklet::Docklets.begin(); iter != Docklet::Docklets.end(); ++iter)
	{
		docklets.push_back(iter->second->GetHWnd());
	}
	LeaveCriticalSection(&Docklet::cs);

	for(std::vector<HWND>::iterator d_iter = docklets.begin(); d_iter != docklets.end(); ++d_iter)
	{
		BOOL res = enumProc(*d_iter,lParam);
		if(FALSE == res)
			break;
	}

	return TRUE;
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the pointer of a desklet asynchronically, before OnCreate() returns.
 * @param hwndDesklet the handle of the desklet to set the pointer of.
 * @param pointer the value of the pointer to set.
 * @remark should be used if OnCreate() will trigger callback On* functions. For example,
 *			a desklet could remove itself in OnCreate() by calling DeskletRemove(). It needs
 *			to set the desklets pointer async then, because OnDestroy will be called with a NULL pointer,
 *			leaving a (possible) memory leak.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletAsyncSetPointer(HWND hwndDesklet, void* pointer)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);	
	if(d != NULL)
	{
		d->GetDocklet()->SetPointer(static_cast<DOCKLET_DATA*>(pointer));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------------------------
#define AVEDESK_SETTING_SETTINGSPATH	1
#define AVEDESK_SETTING_THEMESPATH		2

/**
 * Gets an AveDesk directory path.
 * @param directory the directory to get the path of, can be one of the following
 *					AVEDESK_SETTING_SETTINGSPATH, the directory where all avedesk settings are saved in.
 *					AVEDESK_SETTING_THEMESPATH, the directory where all avedesk themes are saved in.
 * @param wBuf a pointer to a buffer receiving the path. Cannot be NULL.
 * @param maxBufSize a pointer to an UINT giving the length (in characters) of the wBuf buffer. If
 *						the buffer is not large enough to hold the path, maxBufSize will receive
 *						the number of characters the buffer should be able to hold. Cannot be NULL.
 * @param TRUE iff a path was successfully copied into wBuf, otherwise FALSE.
 */
BOOL CALLBACK DeskletGetDirectoryW(DWORD directory, WCHAR* wBuf, UINT* maxBufSize)
{
	if(NULL == maxBufSize)
		return FALSE;

	if(AVEDESK_SETTING_SETTINGSPATH == directory)
	{
		std::wstring wpath = _App->GetAveDeskUserPath();
		if(NULL == wBuf || wpath.length() +1 > *maxBufSize)
		{
			*maxBufSize = wpath.length() + 1;
			return FALSE;
		}
		else
		{
			wcscpy(wBuf,wpath.c_str());
			return TRUE;
		}
	}
	else if(AVEDESK_SETTING_THEMESPATH == directory)
	{
		std::wstring wpath = _App->GetAveDeskThemeDir();
		if(NULL == wBuf || wpath.length() +1 > *maxBufSize)
		{
			*maxBufSize = wpath.length() + 1;
			return FALSE;
		}
		else
		{
			wcscpy(wBuf,wpath.c_str());
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the SkinInfo for a desklet.
 * @param hwndDesklet the handle of the desklet to set the skininfo for.
 * @param info a pointer to a SkinInfo structure that holds the skininfo data. Can be NULL, in case of 
 *				no skin. 
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletSetSkinInfoW(HWND hwndDesklet, SkinInfo* info)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);	
	if(d != NULL)
	{
		d->SetSkinInfo(info);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the Image Status for a desklet.
 * @param hwndDesklet the handle of the deskle to set the image status of.
 * @param imageCanBeSet iff TRUE, the image can be set by the user, otherwise not.
 * @remark if imageCanBeSet, the user can set the image of the desklet on the appearance tab
 *          if !imageCanBeSet, the user cannot set the image of the desklet manually.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletSetImageSetStatus(HWND hwndDesklet, BOOL imageCanBeSet)
// pre:    desklet(hwndDesklet) is not null
// post:   TRUE has been returned iff successfull
// effect: if imageCanBeSet, the user can set the image of the desklet on the appearance tab
//         if !imageCanBeSet, the user cannot set the image of the desklet manually.
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);	
	if(d != NULL)
	{
		bool imageCannotBeSet = !imageCanBeSet;
		d->SetImageCannotBeSet(imageCannotBeSet);
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}
//--------------------------------------------------------------------------------------------
/**
 * Flips a desklet with a visual effect to another set of active layers.
 * @param hwndDesklet the handle of the desklet to flip.
 * @param params a pointer to a FlipParams structure giving the parameters for the flip. Cannot be NULL.
 * @return TRUE iff successfull, otherwise FALSE.
 * @remark this function returns immediately, it doesn't wait 'till the flip is done. To be notified when
 *			the flip is completed, use the callback field of the FlipParams structure.
 */
BOOL CALLBACK DeskletFlip(HWND hwndDesklet, const FlipParams* params)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->StartFlipping(params);
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the close point for a desklet.
 * @param hwndDesklet the handle of the desklet to set the closepoint for.
 * @param closePoint a pointer to a POINT structure giving the closepoint coordinates. Can be NULL
 *					 to disable the usage of a closepoint for this desklet.
 * @param xScale the scale on which closePoint->x is based.
 * @param yScale the scale on which closePoint->y is based.
 * @remark the closepoint is the point on which avedesk will render a closebutton under certain
 *			circumstances.
 * @remark the POINT structure is relative to the xScale and yScale parameters. For example, if xScale and yScale
 *			are both 100, then the POINT structure will give the coordinates in percentages of the desklet.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletSetClosePoint(HWND hwndDesklet, const POINT* closePoint, int xScale, int yScale)
{
	Docklet* d= Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		
		if(closePoint != NULL)
		{
			d->SetUseClosePoint(true);
			d->SetClosePoint(*closePoint, xScale, yScale);
		}
		else
		{
			d->SetUseClosePoint(false);
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Hit tests a desklet for a certain POINT.
 * @param hwndDesklet the handle of the desklet to hittest.
 * @param pt the point to hittest the desklet on. Cannot be NULL.
 * @param s the scale the pt parameter is based on. Cannot be NULL. See remarks.
 * @param onlyTestVisibleLayers if TRUE, only layers that are visible will be hittested, otherwise
 *								all layers will be hittested.
 * @remark the POINT structure is relative to the s->cx, s->cy, s parameter. For example, if s->cx and s->cy
 *			are both 100, then the POINT structure will give the coordinates in percentages of the desklet.
 * @return the id of the layer that is under pt (with scale s) if successfull. Otherwise, (UINT)-1.
 */
unsigned int CALLBACK DeskletHitTest(HWND hwndDesklet, const POINT* pt, const SIZE* s, BOOL onlyTestVisibleLayers)
{
	if(NULL == pt)
		return (UINT) -1;

	if(NULL == s)
		return (UINT) -1;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->GetDocklet()->HitTestLayers(*pt, *s, onlyTestVisibleLayers != FALSE);
	}
	else
	{
		return (UINT) -1;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Locks invalidating for a desklet. If invalidating is locked for
 * a desklet, updating a desklet may be more efficient since no update rectangle
 * is added to the desklet.
 * @param HWND hwndDesklet the handle to the deskle to lock
 * @return the lock-count (can be negative too).
 * @remark must be called together with DeskletUnlockInvalidating in pairs
 */
INT CALLBACK DeskletLockInvalidating(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->GetDocklet()->LockUpdateRegion();
	}
	else
	{
		return (INT)0;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Unlocks invalidating for a desklet. If invalidating is locked for
 * a desklet, updating a desklet may be more efficient since no update rectangle
 * is added to the desklet.
 * @param HWND hwndDesklet the handle to the deskle to unlock
 * @param BOOL redraw iff TRUE, the desklet will be redrawn
 * @return the lock-count (can be negative too).
 * @remark must be called totogether with DeskletLockInvalidating in pairs
 */
INT CALLBACK DeskletUnlockInvalidating(HWND hwndDesklet, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		INT lockCount = d->GetDocklet()->UnlockUpdateRegion();

		if(redraw)
			d->Redraw();

		return lockCount;
	}
	else
	{
		return (INT)0;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Invalidates a portion of the desklet.
 * @param HWND hwndDesklet the handle to to the desklet to invalidate a rect of
 * @param RECT* r, a pointer to rectangle to erase. Cannot be NULL.
 * @param SIZE* scale, a pointer to a SIZE structure defining the scale of r. Cannot be NULL
 * @param BOOL redraw iff TRUE, the desklet will be redrawn
  * @remark the RECT structure is relative to the s->cx, s->cy, s parameter. For example, if s->cx and s->cy
 *			are both 100, then the RECT structure will give the coordinates in percentages of the desklet.
 * @return TRUE iff successfull
 */
BOOL CALLBACK DeskletInvalidateRect(HWND hwndDesklet, RECT* rect, SIZE* scale, BOOL redraw)
{
	if(NULL == rect || NULL == scale)
		return FALSE;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		d->GetDocklet()->AddUpdateRect(*rect, *scale);

		if(redraw)
			d->Redraw();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------------------------
/**
 * Sets the cursor for a desklet.
 * @param HWND hwndDesklet the handle of the desklet to set the cursor of
 * @param HCURSOR handle of the new cursor to set
 * @return the handle of the previous cursor
 */
HCURSOR CALLBACK DeskletSetCursor(HWND hwndDesklet, HCURSOR hCursor)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->ChangeCursor(hCursor);
	}
	else
	{
		return NULL;
	}
}
//--------------------------------------------------------------------------------------------


bool EndApp()
{
	// post:  the application is ended. If it is ended properly, true has
	//			been returned, otherwise false

	if( _Dlg.IsWindow() )
		_Dlg.DestroyWindow();

	if( IsWindow(GetConsoleWindow()) )
		_Dbg->HideConsole();

	if(_Policies.GetBoolValue(L"closing//fadeAllOut", TRUE))
		Docklet::FadeAllOut();


	if(_Policies.GetBoolValue(L"closing//saveSettings", TRUE))
		_App->SaveGlobalSettings();

	delete Docklet::defaultDocklet;
	Docklet::defaultDocklet = NULL;

	if(_Policies.GetBoolValue(L"closing//saveTheme", TRUE) && _globalSettings.GetAutoSaveThemes() )
	{
		CComBSTR bstrThemeFile(_App->GetThemeFileName().c_str());
		_App->SaveTheme(bstrThemeFile);
	}

	Docklet::ReleaseAll();

	DockletDLL::ReleaseAll();

	delete _Modules;
	_Modules = NULL;

	// clear the labels used
	LabelBackground::ReleaseAll();

	// remove the tray icon
	//KillIcon();
	_App->GetIconWindow().KillIcon();

	// release the mutex, thus allowing new instances of the same application to be started.
	ReleaseMutex(hMutex);
	hMutex = 0;


	return true;
}


LONG CALLBACK DummyProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

bool ReadBoolFromIni(const WCHAR* group, const WCHAR* key, bool defVal, const WCHAR* ini)
{
	WCHAR str[MAX_PATH] = {0};
	GetPrivateProfileStringW(group,key,defVal ? L"true" : L"false",str,MAX_PATH,ini);

	return _wcsicmp(str,L"true") == 0 || _wcsicmp(str,L"yes") == 0 || wcscmp(str,L"1") == 0;
}

void CheckAndDoSettingsOverrides()
{
	WCHAR overridesSettingsFilePath[MAX_PATH] = {0};
	if(_App)
		wcscpy(overridesSettingsFilePath, _App->GetAveDeskDataDir().c_str());
	else
		GetDLLPathW(NULL, overridesSettingsFilePath, MAX_PATH);
	GetDLLPathW(NULL,overridesSettingsFilePath,MAX_PATH);
	PathAppend(overridesSettingsFilePath,L"settings_overrides.ini");


	bool noMultipleUserAwareness = ReadBoolFromIni(L"Overrides",L"NoMultipleUserAwareness",false,overridesSettingsFilePath);
	_globalSettings.SetNoMultipleUserAwareness(noMultipleUserAwareness);

	::_noFileAssociation =  ReadBoolFromIni(L"Overrides",L"NoFileAssociations",false,overridesSettingsFilePath);
	::_noMemCleaner	= ReadBoolFromIni(L"Overrides",L"NoMemCleaner",false,overridesSettingsFilePath);
	::_alwaysSilentInstall = ReadBoolFromIni(L"Overrides",L"AlwaysSilentInstall",false,overridesSettingsFilePath);
	::_noAntiAliasFonts = ReadBoolFromIni(L"Overrides",L"NoAntiAliasFonts",false,overridesSettingsFilePath);
	::_noStartAnimation = ReadBoolFromIni(L"Overrides",L"NoStartAnimation",false,overridesSettingsFilePath);
	::_noCloseAnimation = ReadBoolFromIni(L"Overrides",L"NoCloseAnimation",false,overridesSettingsFilePath);

	int animTime = GetPrivateProfileInt(L"Overrides",L"AnimationTime",-1,overridesSettingsFilePath);
	if(animTime != -1)
		_AnimTimeInMS = animTime;


	GetPrivateProfileString(L"Overrides",L"LanguageFile",L"",overridenLanguageFile,MAX_PATH,overridesSettingsFilePath);

	//WCHAR alwaysSilentInstallString[MAX_PATH] = {0};
	/// TODO: hmmm where was this for, I wonder.... 

	_ControlPanelPreviewIconWidth  = static_cast<UINT>(GetPrivateProfileInt(L"overrides", L"ControlPanelPreviewIconWidth", _ControlPanelPreviewIconWidth, overridesSettingsFilePath));
	_ControlPanelPreviewIconHeight = static_cast<UINT>(GetPrivateProfileInt(L"overrides", L"ControlPanelPreviewIconHeight", _ControlPanelPreviewIconHeight, overridesSettingsFilePath));

	_themePreviewSizePercentage = static_cast<UINT>(GetPrivateProfileInt(L"overrides",L"ThemePreviewSizePercentage",_themePreviewSizePercentage, overridesSettingsFilePath));

	_noCachingOfBitmaps = ReadBoolFromIni(L"engine", L"FullRedraws", _noCachingOfBitmaps != FALSE, overridesSettingsFilePath) ? TRUE : FALSE;
}

bool TryToCreateMutex()
{
	// create a mutex, so this application could only be started once
	WCHAR path[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(0),path,sizeof(path)/sizeof(path[0]));
	lowercaseW(path);
	// convert all backslashes to tildes, because mutex names cannot
	// contain any backslashes
	for(int z=wcslen(path)-1;z>=0;--z)
	{
		if(path[z]==L'\\')path[z]=L'~';
	}
	hMutex = CreateMutexW(0,TRUE,path);

	// mutex is not created, so the app is already started
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		// FIX: closehandle must also be done when getlasterror() reports
		// ERROR_ALREADY_EXISTS
		CloseHandle(hMutex);
		return false;
	}

	return true;
}

bool StartApp()
{
	// post: all settings are read from the ini-file and are applied
	//			this includes creating/loading all docklets that are
	//			set up to be started
	  // ctrls
	  InitCommonControls();

	  if(_Policies.GetBoolValue(L"loading//registerAveDeskCOMLibrary", TRUE))
	  {
		  // register on demand
		  CComPtr<ITypeLib> lib;
		  LoadRegTypeLib(LIBID_AveDeskLib, 1, 0, 0L, &lib.p);
		  if( !lib.p )
		  {
			  //_Module.UpdateRegistryFromResource(IDR_AveDesk, TRUE);
			  _Module.RegisterServer(TRUE);
			  lib.Release();
		  }
	  }

	_Modules = new CModules(GetCurrentThreadId(),_App->GetMainWindowHandle());
	if(_Policies.GetBoolValue(L"loading//loadSettings", TRUE))
		_App->LoadGlobalSettings();

	// make a tray icon
	if(_Policies.GetBoolValue(L"ui//trayicon/show", TRUE))
		_App->GetIconWindow().MakeIcon();

	if(_Policies.GetBoolValue(L"loading//registerClipboardFormat", TRUE))
		Docklet::clipFormat = RegisterClipboardFormat(_T("AveDeskDesklet"));

	// register window class
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.hInstance = _Module.GetModuleInstance();
	wc.lpfnWndProc = DummyProc;
	wc.lpszClassName = _T("AveDummyClass");
	RegisterClassEx(&wc);

	if(_Policies.GetBoolValue(L"loading//loadTheme", TRUE) &&
		!_noLoadThemeOnStartUp)
	{
		std::wstring theme(_App->GetThemeFileName());
		std::wstring polTheme(_Policies.GetStringValue(L"files//themeFileName", theme));
		if(polTheme.length() > 0)
			theme = polTheme;
		CComBSTR themeFile(theme.c_str());
		_App->LoadTheme(themeFile);
	}

	if(_Policies.GetBoolValue(L"loading//doFirstBootIntro", TRUE) && _globalSettings.GetFirst13Boot() || 
		(_Policies.GetBoolValue(L"loading//firstBootWhenEmptyTheme", FALSE) && 0 == _App->numInitialDocklets)
		)
	{
		POINT offset = {0,0};
		WCHAR intro[MAX_PATH] = {0};
		wcscpy(intro, _App->GetAveDeskDataDir().c_str());
		std::wstring deskletToLoad = _Policies.GetStringValue(L"loading//deskletForFirstBoot", L"desklets\\AveDesk 1.3 Intro\\intro.stickynotes.avedesklet");
		wcscat(intro, deskletToLoad.c_str());
		WCHAR* ext = PathFindExtension(intro);
	
		if(_wcsicmp(ext, L".dll") == 0)
		{
			Docklet *d = Docklet::LoadDocklet(intro, NULL, NULL, NULL, false, offset);
			if(d != NULL)
			{
				d->WaitForDockletBeingInitialized(INFINITE);
				if(::IsWindow(d->m_hWnd))
					d->ApplyStyle(STYLE_TOP);
				_globalSettings.SetFirst13Boot(false);
			}
		}
		else
		{
			_App->OpenFile(CComBSTR(intro));
			_globalSettings.SetFirst13Boot(false);
		}
	}

	//CAveInstaller inst;
	//inst.Install(L"c:\\testje.zip", _ncd.hWnd);
	
	return true;
}


void LoadLanguage()
{
		if(overridenLanguageFile[0] != L'\0')
		{
			if(PathFileExists(overridenLanguageFile))
			{
				_Module.ChangeResourceInstance(overridenLanguageFile);
			}
			else
			{
				WCHAR langPath[MAX_PATH] = {0};
				wcscpy(langPath, _App->GetAveDeskDataDir().c_str());
				PathAppendW(langPath,overridenLanguageFile);
				if(PathFileExists(overridenLanguageFile))
				{
					_Module.ChangeResourceInstance(langPath);
				}
				else
				{
					wcscpy(langPath, _App->GetAveDeskDataDir().c_str());
					PathAppendW(langPath,L"languages");
					PathAppendW(langPath,overridenLanguageFile);
					_Module.ChangeResourceInstance(langPath);
				}
			}

		}
		else
		{
			WCHAR langPath[MAX_PATH] = {0};
			wcscpy(langPath, _App->GetAveDeskDataDir().c_str());
			PathAppendW(langPath,L"Languages");

			LANGID langId = GetUserDefaultLangID();

			std::wstringstream  ws;
			ws << L"AVE"<< std::hex <<  std::setfill(L'0') <<  std::setw(3) << PRIMARYLANGID(langId) << std::setfill(L'0') <<  std::setw(3) << SUBLANGID(langId) << L".DLL";
			PathAppendW(langPath,ws.str().c_str() );
			if(!PathFileExists(langPath))
			{
				wcscpy(langPath, _App->GetAveDeskDataDir().c_str());
				PathAppendW(langPath,L"Languages");

				std::wstringstream  ws;
				ws << L"AVE"<< std::hex <<  std::setfill(L'0') <<  std::setw(3) << PRIMARYLANGID(langId) <<  L"???" << L".DLL";
				PathAppendW(langPath,ws.str().c_str() );

				WIN32_FIND_DATAW data = {0};
				HANDLE h = FindFirstFile(langPath,&data);
				if(h != INVALID_HANDLE_VALUE)
				{

					wcscpy(langPath, _App->GetAveDeskDataDir().c_str());
					PathAppendW(langPath,L"Languages");
					PathAppendW(langPath,data.cFileName);

					FindClose(h);

					_Module.ChangeResourceInstance(langPath);
				}

			}
			else
			{
				_Module.ChangeResourceInstance(langPath);
			}
		}
}

struct CAveDeskSpecialMessageFilter : public CMessageFilter
{
	virtual BOOL PreTranslateMessage(MSG *pMsg)
	{
		/*
		AtlTrace("FLT:%ld,%ld,%ld\n", 
			pMsg->message,
			pMsg->lParam,
			pMsg->wParam);
			*/

		if(_Dbg)
			_Dbg->HandleConsoleInput();

		if(WM_QUERYENDSESSION == pMsg->message)
		{
			if( _Dbg )
				_Dbg->HideConsole();
			PostQuitMessage(0);
			return TRUE;
		}
		else if(WM_AVEACTION_SHOWCP == pMsg->message)
		{
			_Dlg.ShowOrActivate();
			return TRUE;
		}
		else if(WM_AVEACTION == pMsg->message)
		{
			if(AVEACTION_GETSAFEOBJECT == pMsg->lParam)
			{
				IStream *pStr = NULL;
				IDispatch *pDisp = (IDispatch*)pMsg->wParam;
				HRESULT hr = S_OK;
				if( pDisp )
				{
					hr = CoMarshalInterThreadInterfaceInStream(
						IID_IDispatch, pDisp, &pStr);
					if( pStr )
					{
						pDisp->Release();
						ReplyMessage((LRESULT)pStr);
						return TRUE;
					}
				}
			}
			else if(AVEACTION_GETSAFEELEM == pMsg->lParam)
			{
				safeElem * se = (safeElem*)pMsg->wParam;
				se->Marshal();
				return TRUE;
			}
		}
		return FALSE;
	}
};

BOOL DoCommandLineArgsBeforeInit(LPWSTR* szArglist, INT nArgs)
{
	WCHAR path[MAX_PATH] = {0};
	GetModuleFileNameW(NULL, path, MAX_PATH);
	int nRet = 0;
	BOOL bRun = TRUE;
	if(nArgs > 1)
	{
		LPWSTR arg = szArglist[1];
		if(arg != NULL)
		{
			if(_wcsicmp(arg, L"-RegServer") == 0 || _wcsicmp(arg, L"/RegServer") == 0) // register server
			{
				//_Module.UpdateRegistryFromResource(IDR_AveDesk, TRUE);
				nRet = _Module.RegisterServer(TRUE);
				bRun = FALSE;
			}
			else if(_wcsicmp(arg, L"-AssocFiles") == 0 || _wcsicmp(arg, L"/AssocFiles") == 0) // associate file extensions
			{
				CAveDeskFileAssociation deskletAssoc(L".avedesklet",L"AveDesk.AveDeskletFile",path,2);
				deskletAssoc.SetAssociation();

				CAveDeskFileAssociation themeAssoc(L".avetheme",L"AveDesk.AveThemeFile",path,3);
				themeAssoc.SetAssociation();

				CAveDeskFileAssociation installAssoc(L".aveinst",L"AveDesk.AveInstallFile",path,4);
				installAssoc.SetAssociation();

				bRun = FALSE;
			}
			else if(_wcsicmp(arg, L"-UnassocFiles") == 0 || _wcsicmp(arg, L"-UnassocFiles") == 0) // unassociate file extensions
			{
				CAveDeskFileAssociation deskletAssoc(L".avedesklet",L"",path,2);
				deskletAssoc.SetAssociation();

				CAveDeskFileAssociation themeAssoc(L".avetheme",L"",path,3);
				themeAssoc.SetAssociation();

				CAveDeskFileAssociation installAssoc(L".aveinst",L"",path,4);
				installAssoc.SetAssociation();

				bRun = FALSE;
			}
		}
	}

	return bRun;
}


BOOL DoCommandLineArgs(CComPtr<IApplication> app, LPWSTR* szArglist, INT nArgs, BOOL firstStart = TRUE)
{
	int nRet = 0;
	BOOL bRun = TRUE;
	if(nArgs > 1)
	{
		LPWSTR arg = szArglist[1];
		if(arg != NULL)
		{
			if(_wcsicmp(arg, L"-opensc") == 0) // open showcase
			{
				CComPtr<IShowCase> sc;
				app->get_ShowCase(&sc);
				if(sc != NULL)
					sc->Activate();
			}
			else if(_wcsicmp(arg, L"-closesc") == 0) // close showcase
			{
				CComPtr<IShowCase> sc;
				app->get_ShowCase(&sc);
				if(sc != NULL)
					sc->Deactivate();
			}
			else if(_wcsicmp(arg, L"-cp") == 0) // control panel
			{
				app->ShowControlPanel();
			}
			else
			{
				CComBSTR bstr(arg);
				app->OpenFile(bstr);
			}
		}
	}
	else
	{
		if(!firstStart)
			app->ShowControlPanel();
	}

	return bRun;
}

BOOL CheckFileAssocations()
{
	WCHAR path[MAX_PATH] = {0};
	GetModuleFileNameW(NULL, path, MAX_PATH);

	if(!::_noFileAssociation)
	{
		CAveDeskFileAssociation deskletAssoc(L".avedesklet",L"AveDesk.AveDeskletFile",path,2);
		if(!deskletAssoc.IsSet())
			deskletAssoc.SetAssociation();

		CAveDeskFileAssociation themeAssoc(L".avetheme",L"AveDesk.AveThemeFile",path,3);
		if(!themeAssoc.IsSet())
			themeAssoc.SetAssociation();

		CAveDeskFileAssociation installAssoc(L".aveinst",L"AveDesk.AveInstallFile",path,4);
		if(!installAssoc.IsSet())
			installAssoc.SetAssociation();
	}

	return TRUE;
}


void DoCOMInit(HINSTANCE hInstance)
{
  
	#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
		HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	#else
		HRESULT hRes = CoInitialize(NULL);
	#endif
	_ASSERTE(SUCCEEDED(hRes));
	_Module.Init(ObjectMap, hInstance, &LIBID_AveDeskLib);
	_Module.dwThreadID = GetCurrentThreadId();
	TCHAR szTokens[] = _T("-/");

	OleInitialize(NULL);

}

BOOL HasPackageData()
{
	const int IDR_PACKAGEDATA_ZIP = 181;
	HRSRC res = FindResourceW(NULL, MAKEINTRESOURCE(IDR_PACKAGEDATA_ZIP),L"PackageData");
	return SizeofResource(NULL, res) > 10;
}

BOOL UnpackResources(const std::wstring& path)
{
	USES_CONVERSION;

	WCHAR packagePath[MAX_PATH] = {0};
	wcscpy(packagePath, path.c_str());
	PathAppendW(packagePath, CreateGUIDString().c_str());

	const int IDR_PACKAGEDATA_ZIP = 181;
	HRSRC res = FindResourceW(NULL, MAKEINTRESOURCE(IDR_PACKAGEDATA_ZIP),L"PackageData");
	if(res != NULL)
	{
		HGLOBAL glob = LoadResource(NULL, res);
		if(glob != NULL)
		{
			LPVOID data = LockResource(glob);
			if(data != NULL)
			{
				HANDLE h = CreateFile(packagePath,GENERIC_WRITE, FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if(h != INVALID_HANDLE_VALUE)
				{
					DWORD numWritten(0);
					DWORD dwResSize = SizeofResource(NULL, res);
					WriteFile(h, data,dwResSize,&numWritten,NULL);
					CloseHandle(h);
				}
			}
		}
	}

	CUnzipper unzipper(W2A(packagePath));
	BOOL bRes = unzipper.UnzipTo(W2A(path.c_str()),FALSE);	
	unzipper.CloseZip();
	DeleteFileW(packagePath);
		
	return bRes;
}

#define LOAD_POLICIES
#define LOAD_INTERNAL_POLICY
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

#ifdef DEBUG
	_CrtMemState stat = {0};
	_CrtMemCheckpoint(&stat);
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(0L);
#endif//DEBUG


	// First, get the commandline args.
	lpCmdLine = reinterpret_cast<char*>(GetCommandLine()); //this line necessary for _ATL_MIN_CRT
	LPWSTR *szArglist = NULL;
	int nArgs = 0;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	// initialize the basic come stuff
	DoCOMInit(hInstance);

#ifdef LOAD_POLICIES
	const int IDR_POLICIES1 = 188;
	HRSRC policiesRes = FindResourceW(NULL, MAKEINTRESOURCE(IDR_POLICIES1),L"POLICIES");
	if(policiesRes != NULL && SizeofResource(NULL, policiesRes) > 10)
	{
		HGLOBAL hGlobPolicy = LoadResource(NULL, policiesRes);
		if(hGlobPolicy != NULL)
		{
			LPVOID data = LockResource(hGlobPolicy);
			if(data != NULL)
			{
				SIZE_T len = SizeofResource(NULL, policiesRes);
				CHAR* wPolicyStr = new CHAR[len+1];
				memcpy(wPolicyStr,data, len);
				wPolicyStr[len] = '\0';

				_Policies.LoadFromString(StringToWString(wPolicyStr));
			}
		}
	}
#endif//LOAD_POLICIES


	BOOL runningSelfContained = FALSE;
	std::wstring unpackedResourcesPath;

	if(HasPackageData())//if(_Policies.GetBoolValue(L"loading//unpackResources", FALSE))
	{
		BOOL shouldCache = _Policies.GetBoolValue(L"resources//cache", FALSE);
		if(shouldCache)
		{
			WCHAR path[MAX_PATH+1] = {0};

			// moved out the CSIDL_FLAG_CREATE, so it doesn't auto-create when NoMultipleUserAwareness is active.
			if(SUCCEEDED(SHGetFolderPathW(NULL,CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,path)))
			{
				std::wstring folderName = _Policies.GetStringValue(L"paths//settingsFolderName", L"AveDeskUnknown");
				if(folderName.length() == 0) folderName = L"AveDeskUnknown";

				PathAppendW(path,folderName.c_str());
				if(!PathFileExistsW(path))
					CreateDirectoryW(path,NULL);

				PathAppendW(path,L"cache");
				if(!PathFileExistsW(path))
					CreateDirectoryW(path,NULL);

				wcscat_s(path, MAX_PATH, L"\\");

				unpackedResourcesPath = path;
				
				// only extract when no previously cached data was found!
				PathAppendW(path, L"avecontrols.dll");
				if(!PathFileExistsW(path))
				{
					runningSelfContained = UnpackResources(unpackedResourcesPath);
					if(!runningSelfContained)
						unpackedResourcesPath = L"";
				}
				else
				{
					runningSelfContained = TRUE;
				}
			}
		}
		else
		{
			WCHAR tempPath[MAX_PATH] = {0};
			GetTempPathW(MAX_PATH, tempPath);
			PathAppendW(tempPath, std::wstring(CreateGUIDString()+L"\\").c_str() );
			CreateDirectoryW(tempPath, NULL);
			unpackedResourcesPath = tempPath;
			runningSelfContained = UnpackResources(unpackedResourcesPath);
			if(!runningSelfContained)
				unpackedResourcesPath = L"";
		}
	}

#ifdef LOAD_POLICIES
	if(!_Policies.AreEnabled())
	{
		WCHAR policiesPath[MAX_PATH] = {0};
		if(unpackedResourcesPath.length() > 0)
			wcscpy(policiesPath, unpackedResourcesPath.c_str());
		else
			GetDLLPathW(NULL, policiesPath, MAX_PATH);

		PathAppendW(policiesPath, L"policies.xml");
		_Policies.LoadFromFile(policiesPath);
	}

#endif//LOAD_POLICIES

	// check for the commandline arguments that needs to be run before this instance is
	// fully up -'n- running
	if(_Policies.GetBoolValue(L"loading//parseCommandLine", TRUE))
	{
		if(!DoCommandLineArgsBeforeInit(szArglist, nArgs))
			return 0;
	}

	if(!_Policies.GetBoolValue(L"loading//allowMultipleInstances", FALSE))
	{
		// try to see if this instance is already started actually
		bool alreadyStarted = !TryToCreateMutex();
		if(alreadyStarted)
		{
			CComPtr<IApplication> otherApp;
			otherApp.CoCreateInstance(CLSID_Application, NULL, CLSCTX_ALL);
			if(otherApp.p != NULL)
			{
				if(_Policies.GetBoolValue(L"loading//parseCommandLine", TRUE))
					DoCommandLineArgs(otherApp, szArglist, nArgs, FALSE);
			}
			LocalFree(szArglist);

			return 0;
		}
	}

	// initialize some commonly used critical sections.
	InitializeCriticalSection(&Docklet::cs);
	InitializeCriticalSection(&CXMLLoader::cs);
	InitializeCriticalSection(&CAveInstPackageManager::cs);

	// start up GDI+
	GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);

	if(_Policies.GetBoolValue(L"loading//doCOMServer", TRUE))
	{
		//!herd register COM stuff
		_Module.StartMonitor();
		#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
			HRESULT hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
			REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
			_ASSERTE(SUCCEEDED(hRes));
			hRes = CoResumeClassObjects();
		#else
			HRESULT hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
			REGCLS_MULTIPLEUSE);
		#endif
		_ASSERTE(SUCCEEDED(hRes));
	}
	_Module.Lock();

	// create global application object
	CComObject<CApplication>::CreateInstance(&_App);
	_App->AddRef();
	_App->SetupMainWindow();
	_App->SetMainThreadId(GetCurrentThreadId() );
	_App->InitSafeInstance();
	if(unpackedResourcesPath.length() > 0)
		_App->SetAveDeskDataDir(unpackedResourcesPath);

	// load AveControls.dll, to be sure to always load the right one
	WCHAR controlsPath[MAX_PATH] = {0};
	wcscpy(controlsPath, _App->GetAveDeskDataDir().c_str());
	PathAppendW(controlsPath, L"avecontrols.dll");
	LoadLibraryW(controlsPath);

	// load AveImg
	// load AveControls.dll, to be sure to always load the right one
	WCHAR imgdllPath[MAX_PATH] = {0};
	wcscpy(imgdllPath, _App->GetAveDeskDataDir().c_str());
	PathAppendW(imgdllPath, L"aveimg.dll");
	LoadLibraryW(imgdllPath);

	
	// check for any settings that needs to be overridden
	if(_Policies.GetBoolValue(L"loading//doSettingOverrides", TRUE))
		CheckAndDoSettingsOverrides();


	// load the current language
	if(_Policies.GetBoolValue(L"loading//loadLanguage", TRUE))
		LoadLanguage();

	// create global debug helper object
	CComObject<CDebug>::CreateInstance(&_Dbg);
	_Dbg->AddRef();
	_Dbg->Open();

	// create showcase
	CComObject<CShowCase>::CreateInstance(&_ShowCase);
	_ShowCase->AddRef();
	
	// and finally we are able to handle commandline arguments for this specific instance :-)
	if(_Policies.GetBoolValue(L"loading//parseCommandLine", TRUE))
		DoCommandLineArgs(_App, szArglist, nArgs);

	// start the application
	if(!StartApp())
	{
		_App->Release();
		_ShowCase->Release();

		LocalFree(szArglist);

		DeleteCriticalSection(&CXMLLoader::cs);
		DeleteCriticalSection(&Docklet::cs);
		DeleteCriticalSection(&CAveInstPackageManager::cs);
		return 0;
	}

	// initialize showcase's hotkeys
	if(_Policies.GetBoolValue(L"loading//startShowCase", TRUE))
	{
		VARIANT_BOOL scIsEnabled = {FALSE};
		_ShowCase->get_Enabled(&scIsEnabled);
		if(scIsEnabled == VARIANT_TRUE)
			_ShowCase->RegisterHotKeys();
	}

	// The MessageLoop
	CAveDeskSpecialMessageFilter specialMessageFilter;
	CMessageLoop msgLoop;
	_Module.m_MainLoop = &msgLoop;
	msgLoop.AddMessageFilter(&specialMessageFilter);
	msgLoop.Run();

	// closing down again...
	_ShowCase->UnregisterHotKeys();

	// end the application
	EndApp();

	_ShowCase->Release();

	delete Docklet::GetGlobalDockletsEventNotifier();

	// shut down GDI+
	GdiplusShutdown(g_gdiplusToken);
	//!herd
	_Dbg->Close();
	_Dbg->Release();
	_App->ExitSafeInstance();
	_App->Release();
	_Module.Unlock();
	_Module.RevokeClassObjects();
	Sleep(dwPause); //wait for any threads to finish

	DeleteCriticalSection(&CXMLLoader::cs);
	DeleteCriticalSection(&Docklet::cs);
	DeleteCriticalSection(&CAveInstPackageManager::cs);

	delete Docklet::defaultDocklet;
	Docklet::defaultDocklet = NULL;

	CTrayIconAnimation::DestroyIcons();

	LocalFree(szArglist);

	BOOL shouldCache = _Policies.GetBoolValue(L"resources//cache", FALSE);
	_Policies.Free();

	CoFreeUnusedLibraries();
	_Module.Term();
	CoUninitialize();


	// TODO: enable again, for debug purposes
	if(!shouldCache)
	{
		if(unpackedResourcesPath.length() > 0)
			DeleteDirectory(unpackedResourcesPath);
	}


#ifdef DEBUG
	AtlTrace(">>AveDesk open COM references: %ld\n", _Module.GetLockCount() );
	OutputDebugString(_T(">>AveDesk Leaks\n"));
	OutputDebugString(_T("<<AveDesk Leaks\n"));
#endif//DEBUG

	return 0;
}


void AutoAlignAll()
{
	int xPos = _globalSettings.GetGridXMargin();
	int yPos = _globalSettings.GetGridYMargin();
	int maxWidth = 0;
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	for(Docklet::DockletsMap::iterator iter=Docklet::Docklets.begin();iter!=Docklet::Docklets.end();++iter)
	{
		Docklet* d = iter->second;
		if(d->GetStyle() == STYLE_ICON)
		{
	
			if(yPos + d->GetDocklet()->GetSize().cy + d->GetLabel().GetHeight()+10 > static_cast<UINT>(screenHeight))
			{
				yPos = _globalSettings.GetGridXMargin();
				xPos += maxWidth;
				maxWidth = d->GetLabel().GetWidth()+10;
			}
			d->GetDocklet()->SetPos(xPos,yPos,false);
			d->Redraw();

			d->UpdateLabelPosition();
			if(d->GetDocklet()->GetSize().cx> maxWidth)maxWidth = d->GetDocklet()->GetSize().cx+10;
			yPos = yPos + d->GetDocklet()->GetSize().cy + d->GetLabel().GetHeight()+10;
		}
	}
}



//	This is a wrapper for the old DialogPropStruct. way of showing tabs. (1.0).
INT CALLBACK WrapperProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	WrapperTabPage* wrapperData = (WrapperTabPage*) GetProp(hwnd,_T("AVE_PSHEET_WRAPPER"));

	if(msg == WM_INITDIALOG)
	{
		PROPSHEETPAGE* psp = (PROPSHEETPAGE*)lParam;	

		if(!psp)
			return TRUE;
		else
			SetProp(hwnd,_T("AVE_PSHEET_WRAPPER"),(HANDLE)psp->lParam);

		wrapperData = (WrapperTabPage*) GetProp(hwnd,_T("AVE_PSHEET_WRAPPER"));

		Docklet* d = (Docklet*)wrapperData->lParam;
		return wrapperData->dlgProc(hwnd,msg,wParam,(LPARAM)( d ? d->GetDocklet()->GetPointer() : 0));
	}
	else
	{
		switch(msg)
		{
			case WM_NOTIFY:
			{
				NMHDR* nmhdr;
				nmhdr = (NMHDR*) lParam;
				switch(nmhdr->code)
				{
					case PSN_APPLY:
						if(wrapperData)
						{
							WPARAM newWParam = MAKEWPARAM(IDOK,0);
							wrapperData->dlgProc(hwnd,WM_COMMAND,newWParam,lParam);
						}
					break;
				}
			}
		}
		if(wrapperData)
			return wrapperData->dlgProc(hwnd,msg,wParam,lParam);
		else
			return FALSE;
	}
}
