// PidlShortcutDesklet.cpp: implementation of the CPidlShortcutDesklet class.
//
//////////////////////////////////////////////////////////////////////

#include "PidlShortcutDesklet.h"
#include <sstream>
#include "resource.h"

#include <shellapi.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <winbase.h>
#include <shobjidl.h>
#include <Ntquery.h>
#include "ShellFolder.h"
#include "PidlDropBox.h"
#include "ShellHelper.h"

//#pragma comment(lib,"Msimg32.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define WM_PIDLCHANGED WM_USER + 69592
#define WM_MONPIDLCHANGED WM_PIDLCHANGED +1
#define WM_USER_PATHCHANGED WM_USER + 20293
#define WM_USER_UPDATEPIDL WM_USER + 1208
#define WM_USER_UPDATEPATH WM_USER + 1209
#define WM_USER_UPDATEPATHFROMAUTOMOUNT WM_USER + 1210
#define WM_USER_GETPATH	WM_USER + 1211
#define WM_USER_NOTIFYSUBLABEL + 1212

#define HIDA_GetPIDLFolder(pida) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[0])
#define HIDA_GetPIDLItem(pida, i) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[i+1])

// this line is always required, otherwise the host application (AveDesk) won't reconize
// this as a valid desklet. Required information (in order): name, author, comments, version.
DESKLET_INFORMATION("PidlShortcut","Andreas Verhoeven","This desklet acts as a shortcut.",110)

// these events are always required. They make sure the desklet is created, destroyed and
// identified by the host application (AveDesk).
REQUIRED_EVENT_ONCREATE(CPidlShortcutDesklet)
REQUIRED_EVENT_ONDESTROY(CPidlShortcutDesklet)

// these events are optional, uncomment the ones you want to use and replace t with your class name.
// Also, you have to remove the the semicolon in front of the events-name in the .DEF file.
// If you want to respond to an event in your class, you should override the function tha
// matches it.

EVENT_ONLEFTBUTTONCLICK(CPidlShortcutDesklet)		// invokes LeftButtonClick
EVENT_ONRIGHTBUTTONCLICK(CPidlShortcutDesklet)		// invokes RightButtonClick
EVENT_ONMOUSEMOVE(CPidlShortcutDesklet)			// invokes MouseMove
//EVENT_ONMOUSEON(CPidlShortcutDesklet)				// invokes MouseOn
EVENT_ONMOUSEOUT(CPidlShortcutDesklet)				// invokes MouseOut
EVENT_ONDRAGENTER(CPidlShortcutDesklet)			// invokes DragEnter
EVENT_ONDRAGLEAVE(CPidlShortcutDesklet)			// invokes DragLeaver
EVENT_ONDROP(CPidlShortcutDesklet)					// invokes 3 drop handlers: FilesDrop(), StringDrop(), Drop()
EVENT_ONDRAGOVER(CPidlShortcutDesklet)				// invokes DragOver
EVENT_ONSAVE(CPidlShortcutDesklet)					// invokes SaveToIni
EVENT_ONPROCESSMESSAGE(CPidlShortcutDesklet)		// invokes ProcessMessage
EVENT_ONADDPROPERTYSHEETS(CPidlShortcutDesklet)	// invokes AddPropertySheets
EVENT_ONDRAGDROPDATAREQUEST(CPidlShortcutDesklet)	// invokes DragDropDataRequest iff extended drag drop is enabled
//EVENT_ONSELECT(CPidlShortcutDesklet)				// invokes Select
//EVENT_ONDESELECT(CPidlShortcutDesklet)				// invokes Deselect
//EVENT_ONMOUSEWHEELFORWARD(CPidlShortcutDesklet)	// invokes MouseWheelForward
//EVENT_ONMOUSEWHEELBACKWARD(CPidlShortcutDesklet)	// invokes MouseWheelBackward

Bitmap* HICONToGDIPlusBitmap(HICON icon)
{
	HDC screenDC = GetDC(NULL);
	int screenBitDepth = GetDeviceCaps(screenDC,BITSPIXEL);
	ReleaseDC(NULL,screenDC);

	if(32 == screenBitDepth)
	{
		ICONINFO iconInfo = {0};
		iconInfo.fIcon = TRUE;
		GetIconInfo(icon,&iconInfo);

		int height = iconInfo.xHotspot*2;
		int width = iconInfo.xHotspot*2;

		Bitmap* bmp = new Bitmap(width, height);

		BitmapData data;
		Rect r(0,0,bmp->GetWidth(),bmp->GetHeight());
		bmp->LockBits(&r,ImageLockModeWrite,PixelFormat32bppPARGB,&data);
		
		// copies the colour icon, but not yet masked.
		GetBitmapBits(iconInfo.hbmColor,height * width *4,data.Scan0);

		std::auto_ptr<Bitmap> mask( Bitmap::FromHBITMAP(iconInfo.hbmMask,NULL) );
		BitmapData maskData;

		// use 32BPP, because that will align nicely on DWORD boundaries.
		mask->LockBits(&r,ImageLockModeRead,PixelFormat32bppARGB,&maskData);


		BYTE* maskPtr = (BYTE*)maskData.Scan0;
		BYTE* dataPtr = (BYTE*)data.Scan0;

		int numScanLines = maskData.Height;
		int numPixelPerScanLine = maskData.Width;

		// roughly mask the bitmap with its mask
		for(int y=0;y<numScanLines; ++y)
		{
			BYTE* dataScanLineStart = dataPtr;
			BYTE* maskScanLineStart = maskPtr;
			for(int x=0;x<numPixelPerScanLine;++x)
			{
				if(*(maskPtr+1) == 0)
				{
					DWORD* dwPtr = (DWORD*) dataPtr;
					if( ((*dwPtr) & 0xFF000000) == 0)
						*dwPtr = *dwPtr | 0xFF000000;
				}

				dataPtr += sizeof(DWORD);
				maskPtr += sizeof(DWORD);
						
			}

			dataPtr = dataScanLineStart + data.Stride;
			maskPtr = maskScanLineStart + maskData.Stride;
		}

		bmp->UnlockBits(&data);
		mask->UnlockBits(&maskData);


		DeleteObject(iconInfo.hbmColor);
		DeleteObject(iconInfo.hbmMask);

		return bmp;
		
	}
	else
	{
		// GDI+ is grown-up enough to convert icons with a smaller bitdepth than 32.
		return Bitmap::FromHICON(icon);
	}

}


CPidlShortcutDesklet::DEFAULT_DESKLET_CONSTRUCTOR_BODY(CPidlShortcutDesklet),
	m_AutoSetCaption(false),
	m_SubLabelFormat(L"%0 %1"),
	m_UseCustomImages(false),
	m_CanDrop(false),
	m_IsInFadingState(false),
	m_FadingCounter(0),
	m_IsFadingIn(true),
	m_HwndLabelSheet(NULL),
	m_HwndIconSheet(NULL),
	m_RemoveIfItemIsDeleted(false),
	m_IsAnimating(false),
	m_OverlayIsFadingIn(false),
	m_OverlayFadingCounter(0),
	m_ShouldFade(false),
	m_OverlayShouldFade(false),
	m_RemoveIfItemIsUnmounted(false),
	m_ExecuteAs(0),
	m_ThreadId(GetCurrentThreadId())
{

	SetNoClosePoint();
	
	m_ShellIdListClipFormat = RegisterClipboardFormat(CFSTR_SHELLIDLIST);

	FORMATETC etc = {0};
	etc.cfFormat = m_ShellIdListClipFormat;
	etc.dwAspect = DVASPECT_CONTENT;
	etc.lindex = -1;
	etc.tymed = TYMED_HGLOBAL;

	ActivateExtendedDragDrop();

	AddDragDropFormat(etc);

	// IMAGE CANNOT BE SET
	SetImageCanBeSet(FALSE);
}

void CPidlShortcutDesklet::RemoveAndSave()
{
	FadeOut(FALSE);

	//WCHAR path[MAX_PATH * 2] = {0};
	//::GetDLLPathW(GetHInstance(),path,MAX_PATH);

	WCHAR path[MAX_PATH * 2 + 2] = {0};
	UINT bufSize(MAX_PATH * 2);
	if(!DeskletGetDirectoryW(AVEDESK_SETTING_SETTINGSPATH,path,&bufSize))
		::GetDLLPathW(GetHInstance(),path,MAX_PATH);

	PathAppendW(path,L"\\AutoMounterDefaults\\");

	if(!PathFileExistsW(path))
		CreateDirectoryW(path,NULL);

	WCHAR add[3] = {0};

	std::wstring pdn = m_Pidl.GetFullParseDisplayName();
	if(pdn.length() > 1 && pdn[1] != L':')
	{
		add[0] = pdn[0];
		add[1] = pdn[1];
		add[2] = L'\0';
	}
	else
	{
		if(pdn.length() > 0)
		{
			add[0] = pdn[0];
			add[1] = L'\0';
		}
	}

	PathAppendW(path,add);
	PathAddExtensionW(path,L".avedesklet");
	SaveW(path);

	Remove(FALSE);
}

void CPidlShortcutDesklet::RemoveDragFormats()
{
	if( m_Pidl.IsValid() )
	{
		CComPtr<IDataObject> dataObj;
		if( SUCCEEDED(m_Pidl.GetUIObject(NULL,IID_IDataObject,reinterpret_cast<void**>(&dataObj))) )
		{
			CComPtr<IEnumFORMATETC> enumerator;
			if(SUCCEEDED(dataObj->EnumFormatEtc(DATADIR_GET,&enumerator)))
			{
				FORMATETC etc = {0};
				while( enumerator->Next(1,&etc,NULL) != S_FALSE )
					RemoveDragDropFormat(etc);
			}

		}
	}
}

void CPidlShortcutDesklet::UpdateData(bool checkMountedState)
{
	USES_CONVERSION;

	RemoveDragFormats();

	m_Behaviour = CBehaviourFactory::CreateBehaviourForPidl(m_Pidl,m_UserGivenPath,this);

	if( m_Pidl.IsValid() )
	{
		CComPtr<IDataObject> dataObj;
		if(SUCCEEDED(m_Pidl.GetUIObject(NULL,IID_IDataObject,reinterpret_cast<void**>(&dataObj))) )
		{
			CComPtr<IEnumFORMATETC> enumerator;
			if(SUCCEEDED(dataObj->EnumFormatEtc(DATADIR_GET,&enumerator)))
			{
				FORMATETC etc = {0};
				while( enumerator->Next(1,&etc,NULL) != S_FALSE )
					AddDragDropFormat(etc);
			}

		}
	}

	if(checkMountedState)
	{
		ULONG attr = { SFGAO_VALIDATE};
		HRESULT hRes;
		if(FAILED(hRes = m_Pidl.GetAtrributesOf(&attr)) && !(attr & SFGAO_VALIDATE))
		{
			if(GetRemoveIfItemIsUnmounted() == false)
			{
				m_MonitoringPidl.SetFromSpecialFolderLocation(CSIDL_DRIVES);
				m_MonitoringPidl.StartMonitoring(GetWindow(), WM_MONPIDLCHANGED,SHCNE_GLOBALEVENTS);
				FadeOut(FALSE);
			}
			else
			{
				RemoveAndSave();
			}
		}
		else
		{
			ULONG attr = { SFGAO_REMOVABLE };
			if(SUCCEEDED(m_Pidl.GetAtrributesOf(&attr)) && attr & SFGAO_REMOVABLE)
			{
				DWORD dwErrorMode = SetErrorMode(0);
				dwErrorMode = dwErrorMode | SEM_NOOPENFILEERRORBOX	| SEM_FAILCRITICALERRORS;
				SetErrorMode(dwErrorMode);

				if(GetVolumeInformationW(m_Pidl.GetFullParseDisplayName().c_str(),NULL,0,NULL,NULL,NULL,NULL,0))
				{
					// mounted
					//m_MonitoringPidl.SetFromSpecialFolderLocation(CSIDL_DRIVES);
					//m_MonitoringPidl.StartMonitoring(GetWindow(), WM_MONPIDLCHANGED,SHCNE_GLOBALEVENTS);
					if(m_MonitoringPidl.IsValid())
					{
						m_MonitoringPidl.StopMonitoring();
						m_MonitoringPidl.SetFromPIDL(NULL);
					}
					FadeIn(FALSE);
				}
				else
				{
					// unmounted
					if(GetRemoveIfItemIsUnmounted() == false)
					{
						m_MonitoringPidl.SetFromSpecialFolderLocation(CSIDL_DRIVES);
						m_MonitoringPidl.StartMonitoring(GetWindow(), WM_MONPIDLCHANGED,SHCNE_GLOBALEVENTS);
						FadeOut(FALSE);
					}
					else
					{
						RemoveAndSave();
					}
				}
			}
			else
			{
				if(m_MonitoringPidl.IsValid() )
				{
					m_MonitoringPidl.StopMonitoring();
					m_MonitoringPidl.SetFromPIDL(NULL);
				}
				FadeIn(FALSE);
			}
		}
	}

	if(m_AutoSetCaption)
	{
		const char* lbl = W2A(m_Pidl.GetFriendlyName().c_str());
		SetLabel(std::string(lbl));
	}

	m_Pidl.StopMonitoring();

	if(m_Pidl.IsValid() )
		m_Pidl.StartMonitoring(GetWindow(),WM_PIDLCHANGED);

	if(m_HwndLabelSheet)
		SendMessage(m_HwndLabelSheet,WM_USER_PATHCHANGED,0,0);

	if(m_HwndIconSheet)
		SendMessage(m_HwndIconSheet,WM_USER_PATHCHANGED,0,0);

	UpdateSubLabel();
	UpdateIcon();
}

void CPidlShortcutDesklet::UpdatePath(const std::wstring& path, const std::wstring& alternativePidlString, bool checkMountedState)
{
	USES_CONVERSION;

	m_UserGivenPath = path;

	CPidl newPidl;
	newPidl.SetFromPath(path);

	if(!newPidl.IsValid()  && alternativePidlString != L"")
		newPidl.FromString(alternativePidlString);

	if(m_Pidl == newPidl)
		return;

	RemoveDragFormats();

	if(!newPidl.IsValid()  && alternativePidlString != L"")
		m_Pidl.SetFromPath(m_UserGivenPath);
	else
		m_Pidl.SetFromPIDL(newPidl.Detach() );

	UpdateData(checkMountedState);
}
INT WritePrivateProfileIntW(const WCHAR* group, const WCHAR* key, INT val, const WCHAR* ini)
{
	std::wstringstream ws;
	ws << val;

	return WritePrivateProfileStringW(group,key,ws.str().c_str(),ini);
}

void CPidlShortcutDesklet::UpdateSubLabel()
{
	if(m_Behaviour.get() == NULL)
		return;

	if( !static_cast<bool>(m_Pidl) )
		SetSubLabel(std::string(""));

	//return;

	USES_CONVERSION;

	std::wstring newLabel(m_Behaviour->ParseSubLabel(m_Pidl,m_SubLabelFormat) );

	const char* sublbl = W2A(newLabel.c_str());
	SetSubLabel(std::string(sublbl));
}

BOOL CPidlShortcutDesklet::DragEnter(IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(m_Behaviour.get())
	{
		if( m_Behaviour->DragEnter(obj,keys,pnt,effect) )
		{
			DWORD e = *effect;
			m_Behaviour->GetSpecialDropEffect(e);
			SetOverlayImage(e);
			return TRUE;
		}
	}


	if(m_DropTarget.p != NULL)
		m_DropTarget.Release();

	if(SUCCEEDED(m_Pidl.GetUIObject(GetWindow(),IID_IDropTarget,reinterpret_cast<void**>(&m_DropTarget))))
		m_DropTarget->DragEnter(obj,keys,pnt,effect);
	
	SetOverlayImage(*effect);
	m_CanDrop = (*effect) != DROPEFFECT_NONE;

	return TRUE;
}

BOOL CPidlShortcutDesklet::DragLeave()
{
	ClearOverlayImage();

	if(m_Behaviour.get())
	{
		if( m_Behaviour->DragLeave() )
			return TRUE;
	}

	if(m_DropTarget.p != NULL)
		m_DropTarget->DragLeave();

	if(m_DropTarget.p != NULL)
		m_DropTarget.Release();

	return TRUE;
}

BOOL CPidlShortcutDesklet::DragOver(DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(m_Behaviour.get())
	{
		if( m_Behaviour->DragOver(keys,pnt,effect) )
		{
			DWORD e = *effect;
			m_Behaviour->GetSpecialDropEffect(e);
			SetOverlayImage(e);
			return TRUE;
		}
	}
	
	if(m_DropTarget.p != NULL)
		m_DropTarget->DragOver(keys,pnt,effect);

	SetOverlayImage(*effect);
	m_CanDrop = (*effect) != DROPEFFECT_NONE;

	return TRUE;
}

BOOL CPidlShortcutDesklet::Drop(IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(m_Behaviour.get())
	{
		if( m_Behaviour->Drop(obj,keys,pnt,effect) )
		{
			ClearOverlayImage();
			return TRUE;
		}
	}

	/*
	std::vector<std::wstring> filenames;
	int length(0);

	if(SUCCEEDED(CShellHelper::GetListOfFileNamesFromIDataObject(obj,filenames,length)) )
	{
		*effect = DROPEFFECT_LINK;

		USES_CONVERSION;

		std::wstring newParams = m_Params;
		for(std::vector<std::wstring>::iterator iter = filenames.begin(); iter != filenames.end(); ++iter)
		{
			newParams = newParams + L" \"" + *iter + L"\"";
		}

		TCHAR tStartIn[MAX_PATH+2] = {0};
		_tcscpy(tStartIn,W2T( m_StartIn.c_str() ));

		WCHAR wStartIn[MAX_PATH+2] = {0};
		wcscpy(wStartIn,m_StartIn.c_str());

		CMINVOKECOMMANDINFOEX cmi = {0};
		cmi.cbSize = sizeof(cmi);
		cmi.hwnd = GetWindow();

		cmi.lpDirectory = tStartIn;
		cmi.lpDirectoryW = wStartIn;

		cmi.lpParameters = W2A(newParams.c_str() );
		cmi.lpParametersW = newParams.c_str();

		FillCMINVOKECOMMANDINFOEX(cmi);

		m_Pidl.InvokeDefaultVerb(GetWindow(),cmi);
	}
	else
	{
		*effect = DROPEFFECT_NONE;
	}
	*/
	
	if(m_DropTarget.p != NULL)
	{
		m_DropTarget->Drop(obj,keys,pnt,effect);
		m_DropTarget.Release();
	}

	ClearOverlayImage();

	return TRUE;
}

void CPidlShortcutDesklet::SetSubLabelFormat(const std::wstring& str)
{
	if( m_SubLabelFormat != str)
	{
		m_SubLabelFormat = str;
		UpdateSubLabel();
	}
}

void CPidlShortcutDesklet::NotifySublabelChange()
{
	if(GetCurrentThreadId() != m_ThreadId)
		PostMessage(GetWindow(), WM_USER_NOTIFYSUBLABEL, 0L, 0L);
	else
		UpdateSubLabel();
}

void CPidlShortcutDesklet::NotifyIconChange()
{
	UpdateIcon();
}

HWND CPidlShortcutDesklet::GetHWnd()
{
	return GetWindow();
}

void CPidlShortcutDesklet::ReadFromIni(const char* ini, const char* group)
{
	USES_CONVERSION;

	int count = GetPrivateProfileIntW(A2W(group),L"CustomImagesCount",FALSE,A2W(ini));
	for(int i =0; i < count; ++i)
	{
		WCHAR key[MAX_PATH+2] = {0};
		WCHAR value[MAX_PATH+2] = {0};


		std::wstringstream ws;
		ws << L"CustomImageKey" << i;

		GetPrivateProfileStringW(A2W(group),ws.str().c_str(),key,key,MAX_PATH+2,A2W(ini));

		ws.str(L"");
		ws << L"CustomImageValue" << i;

		GetPrivateProfileStringW(A2W(group),ws.str().c_str(),value,value,MAX_PATH+2,A2W(ini));

		m_CustomImages[key] = value;
	}

	for(std::map<std::wstring,std::wstring>::iterator iter = m_CustomImages.begin(); iter != m_CustomImages.end(); ++iter, count++)
	{
		std::wstring keyName = iter->first;
		std::wstring value = iter->second;

		std::wstringstream ws;
		ws << L"CustomImageKey" << count;

		WritePrivateProfileStringW( A2W(group),ws.str().c_str(),value.c_str(),A2W(ini) );
	}

	WritePrivateProfileIntW( A2W(group),L"CustomImagesCount",count,A2W(ini) );

	WCHAR wSubLabelFormat[10024] = {0};
	GetPrivateProfileStringW(A2W(group),L"SubLabelFormat",wSubLabelFormat,wSubLabelFormat,10024,A2W(ini));
	m_SubLabelFormat = wSubLabelFormat;

	WCHAR wParams[10024] = {0};
	GetPrivateProfileStringW(A2W(group),L"Params",wParams,wParams,10024,A2W(ini));
	m_Params = wParams;

	WCHAR wStartIn[10024] = {0};
	GetPrivateProfileStringW(A2W(group),L"StartIn",wStartIn,wStartIn,10024,A2W(ini));
	m_StartIn = wStartIn;

	m_ExecuteAs = GetPrivateProfileIntW(A2W(group),L"ExecuteAs",FALSE,A2W(ini));

	m_AutoSetCaption = GetPrivateProfileIntW(A2W(group),L"AutoSetCaption",FALSE,A2W(ini)) == TRUE;

	m_UseCustomImages = GetPrivateProfileIntW(A2W(group),L"UseCustomImages",FALSE,A2W(ini)) == TRUE;

	m_RemoveIfItemIsUnmounted = GetPrivateProfileIntW(A2W(group),L"RemoveIfItemIsUnmounted",FALSE,A2W(ini)) == TRUE;
	m_RemoveIfItemIsDeleted =   GetPrivateProfileIntW(A2W(group),L"RemoveIfItemIsDeleted",FALSE,A2W(ini)) == TRUE;

	WCHAR wPath[MAX_PATH+2] = {0};
	GetPrivateProfileStringW(A2W(group),L"PIDL",wPath,wPath,MAX_PATH,A2W(ini));

	WCHAR wPidlString[MAX_PATH*100] = {0};
	GetPrivateProfileStringW(A2W(group),L"PIDLSTRING",wPidlString,wPidlString,MAX_PATH*100,A2W(ini));

	if(wPath[0] != 0 || wPidlString[0] != 0)
	{
		UpdatePath(wPath,wPidlString);
	}
}

void CPidlShortcutDesklet::SaveToIni(const char* ini, const char* group)
{
	USES_CONVERSION;

	WritePrivateProfileStringW(A2W(group),L"PIDL",m_UserGivenPath.c_str(),A2W(ini));
	WritePrivateProfileIntW( A2W(group),L"AutoSetCaption",m_AutoSetCaption ? TRUE : FALSE,A2W(ini) );
	WritePrivateProfileIntW( A2W(group),L"RemoveIfItemIsUnmounted",GetRemoveIfItemIsUnmounted() ? TRUE : FALSE,A2W(ini) );
	WritePrivateProfileIntW( A2W(group),L"RemoveIfItemIsDeleted",GetRemoveIfItemIsDeleted() ? TRUE : FALSE,A2W(ini) );
	WritePrivateProfileIntW( A2W(group),L"UseCustomImages",m_UseCustomImages ? TRUE : FALSE,A2W(ini) );
	WritePrivateProfileStringW( A2W(group),L"SubLabelFormat",m_SubLabelFormat.c_str(),A2W(ini) );
	WritePrivateProfileStringW( A2W(group),L"Params",m_Params.c_str(),A2W(ini) );
	WritePrivateProfileStringW( A2W(group),L"StartIn",m_StartIn.c_str(),A2W(ini) );
	WritePrivateProfileIntW( A2W(group),L"ExecuteAs",m_ExecuteAs,A2W(ini) );

	if(m_Pidl.IsValid() )
		WritePrivateProfileStringW( A2W(group),L"PIDLSTRING",m_Pidl.ToString().c_str(),A2W(ini) );

	int count = 0;
	for(std::map<std::wstring,std::wstring>::iterator iter = m_CustomImages.begin(); iter != m_CustomImages.end(); ++iter, count++)
	{
		std::wstring keyName = iter->first;
		std::wstring value = iter->second;

		std::wstringstream ws;
		ws << L"CustomImageKey" << count;

		WritePrivateProfileStringW( A2W(group),ws.str().c_str(),keyName.c_str(),A2W(ini) );

		ws.str(L"");
		ws << L"CustomImageValue" << count;

		WritePrivateProfileStringW( A2W(group),ws.str().c_str(),value.c_str(),A2W(ini) );
	}

	WritePrivateProfileIntW( A2W(group),L"CustomImagesCount",count,A2W(ini) );

}

void CPidlShortcutDesklet::FillCMINVOKECOMMANDINFOEX(CMINVOKECOMMANDINFOEX& cmi)const
{
	if(m_ExecuteAs == 0)
		cmi.nShow =  SW_SHOWNORMAL;
	else if(m_ExecuteAs == 1)
		cmi.nShow = SW_SHOWMINIMIZED;
	else if(m_ExecuteAs == 2)
		cmi.nShow = SW_SHOWMAXIMIZED;
}

void CPidlShortcutDesklet::ImageFadeIn(std::wstring keyName)
{
	if(this->m_UseCustomImages)
	{
		m_FadingImage = keyName;
		m_IsFadingIn = true;
		m_IsAnimating = true;
		m_ShouldFade = true;
		//DoAnimationFrame();
		SetTimer(GetWindow(),TIMER_FADER,20,0);
	}
}

void CPidlShortcutDesklet::ImageFadeOut()
{
	if(this->m_UseCustomImages)
	{
		m_IsFadingIn = false;
		m_IsAnimating = true;
		m_ShouldFade = true;
		//DoAnimationFrame();
		SetTimer(GetWindow(),TIMER_FADER,20,0);
	}
}

void CPidlShortcutDesklet::ClearOverlayImage()
{
	CLayers& layers = GetLayers();
	try
	{
		try
		{
			CLayer& layer = layers["dragdropOverlay"];
			if(layer.IsVisible() )
			{
				//layer.SetAlpha(200,TRUE);
				//layer.SetAlpha(150,TRUE);
				//layer.SetAlpha(100,TRUE);
				//layer.SetAlpha(50,TRUE);
				m_OverlayShouldFade = true;
				m_OverlayIsFadingIn = false;
				m_IsAnimating		= true;
				SetTimer(GetWindow(),TIMER_FADER,20,0);
			}
		}catch(...){}

		//layers.Remove("dragdropOverlay",TRUE);
	}catch(...){}
}

void CPidlShortcutDesklet::SetOverlayImage(DWORD effect)
{
	CLayers& layers = GetLayers();

	std::string overlayFile = GetRelativeFolder() + "shortcutOverlays\\";
	if(effect & DROPEFFECT_NONE)
		overlayFile += "none.png";
	if(effect & DROPEFFECT_COPY)
		overlayFile += "add.png";
	else if(effect & DROPEFFECT_MOVE)
		overlayFile += "move.png";
	else if(effect & DROPEFFECT_LINK)
		overlayFile += "shortcut.png";
	else if(effect & PRIVATE_DROPEFFECT_REMOVE)
		overlayFile += "remove.png";
	else if(effect & PRIVATE_DROPEFFECT_DELETE)
		overlayFile += "delete.png";


	if(layers.Exists("dragdropOverlay"))
	{
		try
		{
			CLayer& layer = layers["dragdropOverlay"];
			if(layer.GetImageFile() != overlayFile )
			{
				layer.SetVisibility(effect != DROPEFFECT_NONE,FALSE);
				layer.SetImageFile(overlayFile,(!m_IsAnimating && layer.GetAlpha() == 255) ? TRUE : FALSE);

				if(layer.GetAlpha() != 255)
				{
					m_OverlayShouldFade = true;
					m_OverlayIsFadingIn = true;
					m_IsAnimating		= true;
					SetTimer(GetWindow(),TIMER_FADER,20,0);
				}
			}
		}catch(...){}
	}
	else
	{
		try
		{
			CLayer& layer = layers.Add("dragdropOverlay");
			layer.SetVisibility( effect != DROPEFFECT_NONE,FALSE);
			layer.SetImageFile(overlayFile,FALSE);
			layer.SetAlpha(0);
			if(layer.IsVisible())
			{
				m_OverlayShouldFade = true;
				m_OverlayIsFadingIn = true;
				m_IsAnimating		= true;
				SetTimer(GetWindow(),TIMER_FADER,20,0);
			}
		}catch(...){}
	}
}


BOOL PidlCanShow32BitsThumbnails(CPidl& pidl)
{
	if(false == pidl.IsValid())
		return FALSE;

	std::wstring fullPath = pidl.GetFullParseDisplayName();

	if( PathIsDirectoryW(fullPath.c_str()) )
		return TRUE;

	WCHAR* ext = PathFindExtensionW(fullPath.c_str() );

	if( NULL == ext || NULL == *ext)
		return FALSE;

	const WCHAR* imageExtensions[] = {
		L".bmp",
		L".jpg",
		L".jpeg",
		L".gif",
        L".tif",
		L".tiff",
		L".png",
		L".tga",
		L".emf",
		L".wmf",
		L".bid",
		L".aveinst",
		L".avetheme",
		L".avedesklet"
	};

	const imageExtensionsLength = sizeof(imageExtensions) / sizeof(imageExtensions[0]);

	for(int i = 0; i < imageExtensionsLength; ++i)
	{
		if( StrCmpIW(ext,imageExtensions[i]) == 0)
			return TRUE;
	}

	return FALSE;
}


void CPidlShortcutDesklet::UpdateIcon()
{
	if( !m_Pidl.IsValid() )
		return;


	if(!m_UseCustomImages)
	{
		SIZE curSize = GetSize();
	
		HBITMAP hBmp = m_Pidl.ExtractThumb(&curSize,32);
		if(hBmp)
		{
			BitmapData data;
			Bitmap* bmp = new Bitmap(curSize.cx,curSize.cy);
			
			if( PidlCanShow32BitsThumbnails(m_Pidl) )
			{
				Rect r(0,0,bmp->GetWidth(),bmp->GetHeight());
				bmp->LockBits(&r,ImageLockModeWrite,PixelFormat32bppPARGB,&data);
				GetBitmapBits(hBmp,curSize.cx*curSize.cy*4,data.Scan0);
				bmp->UnlockBits(&data);
			}
			else
			{
				std::auto_ptr<Bitmap> bmp2 ( Bitmap::FromHBITMAP(hBmp,NULL) );
				Graphics g(bmp);
				g.Clear(Color(255,255,255,255));
				g.DrawImage(bmp2.get(),0,0,curSize.cx,curSize.cy);
				bmp2 = std::auto_ptr<Bitmap>(NULL);
			}
	
			DeleteObject(hBmp);

			SetImage(bmp,TRUE);
	
			return;
		}
		else if(hBmp = m_Pidl.ExtractThumb(&curSize,24))
		{
			//BitmapData data;
			Bitmap* bmp = new Bitmap(curSize.cx,curSize.cy);
			std::auto_ptr<Bitmap> bmp2 ( Bitmap::FromHBITMAP(hBmp,NULL) );
			Graphics g(bmp);
			g.Clear(Color(255,255,255,255));
			g.DrawImage(bmp2.get(),0,0,curSize.cx,curSize.cy);

			bmp2 = std::auto_ptr<Bitmap>(NULL);
			DeleteObject(hBmp);

			SetImage(bmp,TRUE);
	
			return;
		}
		else
		{
			HICON icon = m_Pidl.ExtractIcon(curSize.cy);
		
			if( !icon )
				return;
		
			Bitmap* bmp = HICONToGDIPlusBitmap(icon);
	
			DestroyIcon(icon);
	
			SetImage(bmp,TRUE);
		}
	}
	else
	{
		if(m_Behaviour.get() )
		{
			USES_CONVERSION;

			std::wstring keyName = m_Behaviour->GetCustomKeyName();



			SetImageFile(W2A( m_CustomImages[keyName].c_str() ));
		}
	}
}

void CPidlShortcutDesklet::DoAnimationFrame()
{
	USES_CONVERSION;
			//CLayers& layers = GetLayers();

			// first we search for the overlay layer named RecycleBin.EjectOverlay

			if(m_ShouldFade)
			{
				BOOL success(FALSE);
				unsigned int id = DeskletLayerGetLayer(GetWindow(),"Shortcut.Overlay",&success);
	
				if(!success)
				{
					// sucess is false, so the layer was not found. In that case we need to create a layer
	
					// add a layer and store it's id
					id = DeskletLayerAdd(GetWindow(),"Shortcut.Overlay");
					DeskletLayerMoveDown(GetWindow(),id,FALSE);

					DeskletLayerSetImageFile(GetWindow(),id,FALSE,W2A(m_CustomImages[m_FadingImage].c_str()));
	
					try
					{
						CLayer layer = GetLayers()["dragdropOverlay"];
						layer.MoveTop(FALSE);
						id = DeskletLayerGetLayer(GetWindow(),"Shortcut.Overlay",&success);
					}catch(...){}
				}

				DeskletLayerSetAlpha(GetWindow(),id,FALSE,m_FadingCounter*13);
				DeskletLayerSetAlpha(GetWindow(),0,!m_OverlayShouldFade,255 - m_FadingCounter*13);

				if(!m_IsFadingIn)
				{
					// we are fading in, so counter must be decreased
					--m_FadingCounter;
	
					if(m_FadingCounter <= 0)
					{
						// counter reached 0, stop the timer and reset the layers
	
						m_ShouldFade = false;
	
						//KillTimer(GetWindow(),TIMER_FADER);
	
						// we can remove the overlay-layer, cause we no longer need it
						DeskletLayerRemove(GetWindow(),id,FALSE);
						DeskletLayerSetAlpha(GetWindow(),0,!m_OverlayShouldFade,255);
					}
				}
				else
				{
					// etc...

					++m_FadingCounter;
					if(m_FadingCounter >= 15)
					{	
						//KillTimer(GetWindow(),TIMER_FADER);
						m_ShouldFade = false;
						DeskletLayerSetAlpha(GetWindow(),id,!m_OverlayShouldFade,255);	

						m_IsAnimating = false;
					}
				}
			}

			if(m_OverlayShouldFade)
			{
				try
				{
					CLayer& layer = GetLayers()["dragdropOverlay"];
					layer.SetAlpha(m_OverlayFadingCounter*36,TRUE);

					if(!m_OverlayIsFadingIn)
					{
						--m_OverlayFadingCounter;
						if(m_OverlayFadingCounter <= 0)
						{
							m_OverlayShouldFade = false;
							GetLayers().Remove("dragdropOverlay",TRUE);
						}
					}
					else
					{
						++m_OverlayFadingCounter;
						if(m_OverlayFadingCounter >= 7)
						{
							m_OverlayShouldFade = false;
							layer.SetAlpha(255,TRUE);
						}
					}

				}catch(...){}
			}


			if(!m_OverlayShouldFade && !m_ShouldFade)
			{
				m_IsAnimating = false;
				KillTimer(GetWindow(),TIMER_FADER);
			}
}

typedef struct {
    DWORD dwItem1;    // dwItem1 contains the previous PIDL or name of the folder. 
    DWORD dwItem2;    // dwItem2 contains the new PIDL or name of the folder. 
} SHNOTIFYSTRUCT;


void CPidlShortcutDesklet::ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_USER_NOTIFYSUBLABEL)
	{
		NotifySublabelChange();
	}
	if(msg == WM_CHAR)
	{
		TCHAR chCharCode = static_cast<TCHAR>(wParam); 
		if(chCharCode == 3)//_T('c'))
		{
			if(GetKeyState(VK_CONTROL) & 0x8000000)
			{
				SHELLEXECUTEINFO info = {sizeof(info)};
				info.fMask = SEE_MASK_IDLIST;
				info.hwnd = GetWindow();
				info.lpVerb = TEXT("copy");
				info.lpIDList = m_Pidl;
	
				ShellExecuteEx(&info);	
			}
		}
	}
	else if(msg == WM_KEYDOWN)
	{
		int vk = static_cast<int>(wParam);
		if(vk == VK_RETURN)
		{
			POINT p = {-1,-1};
			SIZE  s = GetSize();
			LeftButtonClick(&p,&s);
		}
		else if(vk == VK_DELETE)
		{
			// FIX: unknown verb bypassing
			CComPtr<IContextMenu> tmp;
			if(SUCCEEDED(m_Pidl.GetIContextMenu(&tmp,NULL)) && tmp.p)
			{
				SHELLEXECUTEINFO info = {sizeof(info)};
				info.fMask = SEE_MASK_IDLIST;
				info.hwnd = GetWindow();
				info.lpVerb = TEXT("remove");
				info.lpIDList = m_Pidl;
	
				ShellExecuteEx(&info);				
				
				tmp.Release();
			}
		}
		/*
		else if(vk == VK_WINKEY)
		{
			POINT p = {-1,-1};
			SIZE  s = GetSize();
			RightButtonClick(&p,&s);	
		}*/
	}
	else if(msg == WM_USER_GETPATH)
	{
		WCHAR* path = reinterpret_cast<WCHAR*>(lParam);
		BOOL* flag = reinterpret_cast<BOOL*>(wParam);
		if(path)
		{
			std::wstring fpn = m_Pidl.GetFullParseDisplayName();
			if(fpn.length() < MAX_PATH)
			{
				wcscpy(path,fpn.c_str() );
				if(flag)
					*flag = TRUE;
			}
			else
			{
				if(flag)
					*flag = FALSE;
			}
		}
		else
		{
			if(flag)
				*flag = FALSE;
		}

	}
	else if(msg == WM_USER_UPDATEPATHFROMAUTOMOUNT)
	{
		SetRemoveIfItemIsUnmounted(true);
		SetAutoSetCaption(true);

		const WCHAR* path = reinterpret_cast<const WCHAR*>(lParam);
		UpdatePath(path,L"",false);
	}
	else if(msg == WM_USER_UPDATEPATH)
	{
		if(wParam == 1)
		{
			SetRemoveIfItemIsDeleted(true);
			SetAutoSetCaption(true);
		}

		const WCHAR* path = reinterpret_cast<const WCHAR*>(lParam);
		UpdatePath(path);
	}
	else if(msg == WM_USER_UPDATEPIDL)
	{
		if(wParam == 1)
		{
			SetRemoveIfItemIsDeleted(true);
			SetAutoSetCaption(true);
		}

		LPITEMIDLIST rawPidl = reinterpret_cast<LPITEMIDLIST>(lParam);
		CPidl pidl;
		pidl.SetFromPIDL(rawPidl);
		UpdatePath(pidl.GetFullParseDisplayName().c_str(),pidl.ToString());
		pidl.Detach();
	}
	else if(msg == WM_MONPIDLCHANGED)
	{
		SHNOTIFYSTRUCT shns = {0};
		memcpy((void *)&shns,(void *)wParam,sizeof(SHNOTIFYSTRUCT));

		CPidl pidlBefore, pidlAfter;
		pidlBefore.SetFromPIDL( (LPITEMIDLIST) shns.dwItem1);
		pidlAfter.SetFromPIDL( (LPITEMIDLIST) shns.dwItem2);

		CPidl tempPidl;
		tempPidl.SetFromPath(m_UserGivenPath);

		if((lParam == SHCNE_MEDIAINSERTED || lParam == SHCNE_DRIVEADD || lParam == SHCNE_NETSHARE )  && (pidlBefore == m_Pidl || ILIsParent(pidlBefore,m_Pidl,FALSE) || pidlBefore == tempPidl) )
		{
			if(!m_Pidl.IsValid())
				m_Pidl.SetFromPIDL(tempPidl.Detach());

			m_MonitoringPidl.StopMonitoring();
			m_MonitoringPidl.SetFromPIDL(NULL);
			UpdateData(false);

			FadeIn(FALSE);
		}

		pidlBefore.Detach();
		pidlAfter.Detach();
	}
	else if(msg == WM_PIDLCHANGED)
	{
		SHNOTIFYSTRUCT shns = {0};
		memcpy((void *)&shns,(void *)wParam,sizeof(SHNOTIFYSTRUCT));

		CPidl pidlBefore, pidlAfter;
		pidlBefore.SetFromPIDL( (LPITEMIDLIST) shns.dwItem1);
		pidlAfter.SetFromPIDL( (LPITEMIDLIST) shns.dwItem2);

		if(lParam == SHCNE_RENAMEITEM && m_Pidl == pidlBefore)
		{
			m_Pidl.StopMonitoring();

			LPITEMIDLIST copy = {0};
			pidlAfter.CopyItemID(copy);

			m_Pidl.SetFromPIDL( copy );
			m_Pidl.StartMonitoring(GetWindow(),WM_PIDLCHANGED);
			UpdateData(false);
		}
		else if(lParam == SHCNE_ATTRIBUTES)
		{
			UpdateSubLabel();
		}
		else if(lParam == SHCNE_DELETE && m_Pidl == pidlBefore)
		{
			SetSubLabel(std::string("File has been deleted."));

			pidlBefore.Detach();
			pidlAfter.Detach();

			if(m_RemoveIfItemIsDeleted)
				Remove(FALSE);

			return;
		}
		else if(lParam == SHCNE_UPDATEIMAGE)
		{
			UpdateIcon();
		}
		else if((lParam == SHCNE_MEDIAREMOVED || lParam == SHCNE_DRIVEREMOVED  || lParam == SHCNE_NETUNSHARE ) && (pidlBefore == m_Pidl || ILIsParent(pidlBefore,m_Pidl,FALSE)) )
		{
			if(GetRemoveIfItemIsUnmounted() == false)
			{
				m_MonitoringPidl.SetFromSpecialFolderLocation(CSIDL_DRIVES);
				m_MonitoringPidl.StartMonitoring(GetWindow(), WM_MONPIDLCHANGED,SHCNE_GLOBALEVENTS);
				m_Pidl.StopMonitoring();
				FadeOut(FALSE);
			}
			else
			{
				RemoveAndSave();
			}
		}
		else if(lParam == SHCNE_UPDATEITEM || lParam == SHCNE_SERVERDISCONNECT)
		{
			/*m_Pidl.StopMonitoring();

			LPITEMIDLIST copy = {0};
			pidlAfter.CopyItemID(copy);

			m_Pidl.SetFromPIDL( copy );
			m_Pidl.StartMonitoring(GetWindow(),WM_PIDLCHANGED);
			*/
			

			UpdateIcon();
			UpdateSubLabel();
		}

		m_Behaviour->OnMonitorMessage(m_Pidl,lParam,pidlBefore,pidlAfter);

		pidlBefore.Detach();
		pidlAfter.Detach();
		
	}
	if(msg == WM_TIMER &&  wParam == TIMER_FADER)
	{
		DoAnimationFrame();
	}
}

CPidlShortcutDesklet::~CPidlShortcutDesklet()
{
	m_Pidl.StopMonitoring();
}

BOOL CPidlShortcutDesklet::LeftButtonClick(POINT *ptCursor, SIZE *sizeDesklet)
{
	USES_CONVERSION;

	TCHAR tStartIn[MAX_PATH+2] = {0};
	_tcscpy(tStartIn,W2T( m_StartIn.c_str() ));

	WCHAR wStartIn[MAX_PATH+2] = {0};
	wcscpy(wStartIn,m_StartIn.c_str());

	TCHAR tParams[MAX_PATH+2] = {0};
	_tcscpy(tParams,W2T( m_Params.c_str() ));

	WCHAR wParams[MAX_PATH+2] = {0};
	wcscpy(wParams, m_Params.c_str());


	CMINVOKECOMMANDINFOEX cmi = {0};
	cmi.cbSize = sizeof(cmi);
	cmi.hwnd = GetWindow();

	cmi.lpDirectory = tStartIn;
	cmi.lpDirectoryW = wStartIn;


	cmi.lpParameters = tParams;
	cmi.lpParametersW = wParams;

	FillCMINVOKECOMMANDINFOEX(cmi);

	m_Pidl.InvokeDefaultVerb(GetWindow(),cmi);
	return TRUE;
}

BOOL CPidlShortcutDesklet::RightButtonClick(POINT *ptCursor, SIZE *sizeDesklet)
{
	USES_CONVERSION;

	TCHAR tStartIn[MAX_PATH+2] = {0};
	_tcscpy(tStartIn,W2T( m_StartIn.c_str() ));

	WCHAR wStartIn[MAX_PATH+2] = {0};
	wcscpy(wStartIn,m_StartIn.c_str());

	TCHAR tParams[MAX_PATH+2] = {0};
	_tcscpy(tParams,W2T( m_Params.c_str() ));

	WCHAR wParams[MAX_PATH+2] = {0};
	wcscpy(wParams, m_Params.c_str() );


	CMINVOKECOMMANDINFOEX cmi = {0};
	cmi.cbSize = sizeof(cmi);
	cmi.hwnd = GetWindow();

	cmi.lpDirectory = tStartIn;
	cmi.lpDirectoryW = wStartIn;


	cmi.lpParameters = tParams;
	cmi.lpParametersW = wParams;

	FillCMINVOKECOMMANDINFOEX(cmi);


	POINT screenPt = {0};
	DWORD msgPos = GetMessagePos();
	
	screenPt.x = LOWORD(msgPos);
	screenPt.y = HIWORD(msgPos);

	HMENU menu = GetDefaultMenu(10000);
	//HMENU menu2 = CreatePopupMenu();
	//AppendMenu(menu2,MF_POPUP | MF_STRING | MF_ENABLED | MF_UNCHECKED,(UINT)menu,_T("&AveDesk"));


	//HBITMAP hBitmap = LoadBitmap(GetHInstance(),MAKEINTRESOURCE(IDB_AVEDESK));
	//SetMenuItemBitmaps(menu2,MF_BYPOSITION,0,hBitmap,hBitmap);

	UINT cmd = 0;

	if( m_Pidl.IsValid() )
		cmd = m_Pidl.TrackItemIDContextMenu(menu,0,screenPt,GetWindow(),cmi );
	else
		cmd = TrackPopupMenuEx(menu,TPM_RETURNCMD,screenPt.x,screenPt.y,GetWindow(),NULL);

	//char c[100] = {0};
	//itoa(cmd,c,10);
	//MessageBox(NULL,c,NULL,NULL);

	if(cmd != 0)
		DoDefaultMenuCommand(10000,cmd);

	//DestroyMenu(menu2);
	//DeleteObject(hBitmap);

	DestroyMenu(menu);

	return TRUE;
}

void CPidlShortcutDesklet::SetAutoSetCaption(bool val)
{
	USES_CONVERSION;

	if(val != m_AutoSetCaption)
	{
		m_AutoSetCaption = val;

		if(m_AutoSetCaption)
		{
			const char* lbl = W2A(m_Pidl.GetFriendlyName().c_str());
			SetLabel(std::string(lbl));
		}
	}
}



SIZE inline SIZEFROMRECT(RECT& r)
{
	SIZE s = {r.right - r.left,r.bottom - r.top};
	return s;
}

void inline SIZEFROMRECT(SIZE& s, RECT& r)
{
	s.cx = r.right - r.left;
	s.cy = r.bottom - r.top;
}

void SetImageFromFileNameInStatic(HWND hwnd, const WCHAR* filename, BOOL fEnable)
{
	USES_CONVERSION;

	std::auto_ptr<Bitmap> selBmp(DeskletLoadGDIPlusImage(W2A(filename)));

	RECT r = {0};
	GetClientRect(hwnd,&r);

	SIZE s = {0};
	SIZEFROMRECT(s,r);

	Bitmap bmp(s.cx,s.cy);
	Graphics g(&bmp);
	//ApplyQualityTo(QUALITY_BEST,&g);
	DWORD dCol = GetSysColor(COLOR_BTNFACE) | 0xFF000000;
	Color c(255,GetRValue(dCol), GetGValue(dCol), GetBValue(dCol) );
	g.Clear(c);

	if(selBmp.get())
	{
		g.DrawImage(selBmp.get(),5,5,s.cx-10,s.cy-10);
	
		if(!fEnable)
		{
			Color overlayColor( 200,GetRValue(dCol), GetGValue(dCol), GetBValue(dCol) );
			SolidBrush brush(overlayColor);
			g.FillRectangle(&brush,r.left,r.top,s.cx,s.cy);
		}
		
	}

	HBITMAP newHBMP;
	HBITMAP oldHBMP;

	bmp.GetHBITMAP(NULL,&newHBMP);

	oldHBMP = (HBITMAP) SendMessage(hwnd,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)newHBMP);

	DeleteObject(newHBMP);
	DeleteObject(oldHBMP);
}

#define WM_GETIMAGE_FILENAME WM_USER + 10
#define WM_SETIMAGE_FILENAME WM_USER + 11

#define WM_SETTITLE WM_USER + 12

//	This is a DLGPROC for the Label propertysheetpage.
int __stdcall ImageProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{		
		case WM_INITDIALOG:
		{
			WCHAR* img = new WCHAR[MAX_PATH];	
			memset(img,0,MAX_PATH*sizeof(WCHAR));
			SetProp(hDlg,_T("FileName"),(HANDLE)img);

			SetProp(hDlg,_T("Brush"),(HANDLE)GetSysColorBrush(COLOR_BTNFACE) );

			CPidlDropBox* pidlDropBox = new CPidlDropBox(hDlg);
			LPDROPTARGET dropTarget = {0};
			if (FAILED(pidlDropBox->QueryInterface(IID_IDropTarget,reinterpret_cast<void**>(&dropTarget))) )
				delete pidlDropBox;
			else
			{
				RegisterDragDrop(hDlg/*GetDlgItem(hDlg,IDC_ICONDROP)*/,dropTarget);
				dropTarget->Release();
			}
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDSETIMAGE:
				{
					WCHAR* img = (WCHAR*) GetProp(hDlg,_T("FileName"));
					if(!img)
						return FALSE;

					USES_CONVERSION;

					char fname[MAX_PATH+2] = {0};
					strcpy(fname,W2A(img) );

					if(DeskletBrowseForImage(hDlg,fname))
					{
						SendMessage(hDlg,WM_SETIMAGE_FILENAME,0,(LPARAM)A2W(fname));
						PropSheet_Changed(GetParent(GetParent(hDlg)),GetParent(hDlg));	
					}
				}
				break;


			}
		break;

		case WM_SETTITLE:
		{
			WCHAR* title = (WCHAR*)lParam;

			if(title)
				SetDlgItemTextW(hDlg,IDC_TITLE,title);
			else
				SetDlgItemTextW(hDlg,IDC_TITLE,L"");

		}
		break;


		case WM_GETIMAGE_FILENAME:
		{
			WCHAR* img = (WCHAR*) GetProp(hDlg,_T("FileName"));
			if(!img)
				return FALSE;
			
			WCHAR* w = (WCHAR*)lParam;

			wcscpy(w,img);
			
			return TRUE;
		}
		break;

		case WM_SETIMAGE_FILENAME:
		{
			WCHAR* img = (WCHAR*) GetProp(hDlg,_T("FileName"));
			if(!img)
				return FALSE;
			
			WCHAR* w = (WCHAR*)lParam;

			wcscpy(img,w);

			SetImageFromFileNameInStatic(GetDlgItem(hDlg,IDC_IMG),img,IsWindowEnabled(hDlg));

			
			return TRUE;
		}
		break;

		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORDLG:
		{
			return (INT) GetProp(hDlg,_T("Brush"));
		}
		break;

		case WM_ENABLE:
		{
			BOOL fEnable = (BOOL) wParam;
			EnableWindow(GetDlgItem(hDlg,IDC_TITLE),fEnable);
			EnableWindow(GetDlgItem(hDlg,IDSETIMAGE),fEnable);
			//EnableWindow(GetDlgItem(hDlg,IDC_IMG),fEnable);

			WCHAR* img = (WCHAR*) GetProp(hDlg,_T("FileName"));
			if(!img)
				break;

			SetImageFromFileNameInStatic(GetDlgItem(hDlg,IDC_IMG),img,IsWindowEnabled(hDlg));
			

		}
		break;

		case WM_DESTROY:
		{
			WCHAR* img = (WCHAR*) RemoveProp(hDlg,_T("FileName"));
			delete[] img;

			DeleteObject( (HGDIOBJ) RemoveProp(hDlg,_T("Brush")) );
		}
		break;


		case WM_KEYDOWN:
		{
			TCHAR chCharCode = (TCHAR) wParam;
			if(chCharCode == L'v' || chCharCode == L'V' && GetKeyState(VK_CONTROL) < 0)
			{
				SendMessage(hDlg,WM_PASTE,0,0);	
			}
		}
		break;

		case WM_PASTE:
		{
			if(!OpenClipboard(hDlg))
				break;

			UINT shellIdListClipFormat = RegisterClipboardFormat(CFSTR_SHELLIDLIST);

			if(IsClipboardFormatAvailable(shellIdListClipFormat) == 0)
			{
				CloseClipboard();
				break;
			}
	
			HANDLE h = GetClipboardData(shellIdListClipFormat);
	
			if( h == NULL)
			{
				CloseClipboard();
				break;
			}

			LPIDA pida = reinterpret_cast<LPIDA>( GlobalLock(h) );

			if(pida)
			{
				LPCITEMIDLIST	pidlFolder= HIDA_GetPIDLFolder(pida);

				if(pida->cidl > 0)
				{
					LPCITEMIDLIST pidl = HIDA_GetPIDLItem(pida,0);
					LPITEMIDLIST pidlFull = ILCombine(pidlFolder,pidl);
				
					SendMessage(hDlg,WM_PIDLDROPNOTIFY,0L,reinterpret_cast<LPARAM>(pidlFull) ) ;

					CoTaskMemFree(reinterpret_cast<void*>(pidlFull));
				}
			}

			GlobalUnlock(h);
		}
		break;


		case WM_CONTEXTMENU:
		{
			if(!OpenClipboard(hDlg))
				break;

			UINT shellIdListClipFormat = RegisterClipboardFormat(CFSTR_SHELLIDLIST);

			if(IsClipboardFormatAvailable(shellIdListClipFormat) == 0)
			{
				CloseClipboard();
				break;
			}

			HMENU hMenu = CreatePopupMenu();

			AppendMenu(hMenu,MF_ENABLED | MF_STRING,1,_T("Paste File As Settings"));


			DWORD dwPos = GetMessagePos();

			UINT cmd = TrackPopupMenu(hMenu,TPM_RETURNCMD,LOWORD(dwPos),HIWORD(dwPos),0,hDlg,NULL);

			if(cmd != 1)
			{
				CloseClipboard();
				break;
			}

			SendMessage(hDlg,WM_PASTE,0,0);

			CloseClipboard();

		}
		break;

		case WM_PIDLDROPNOTIFY:
		{
			CPidl pidl;
			pidl.SetFromPIDL( reinterpret_cast<LPITEMIDLIST>(lParam) );

			SendMessage(hDlg,WM_SETIMAGE_FILENAME,0,(LPARAM) pidl.GetFullParseDisplayName().c_str() );

			pidl.Detach();

			PropSheet_Changed(GetParent(GetParent(hDlg)),GetParent(hDlg));	
		}
		break;
	}
	return FALSE;
}

HWND CreateImageSetWindow(HINSTANCE hInstance, HWND hwndParent)
{
	return CreateDialogParam(hInstance,MAKEINTRESOURCE(IDD_IMAGE),hwndParent,ImageProc,NULL);
}

void LoadIconDlgSettings(HWND hDlg,CPidlShortcutDesklet* lpData)
{
	if(!lpData)
		return;

	lpData->SetHwndIconSheet(hDlg);

	if(!lpData->GetUseCustomImages() )
		CheckDlgButton(hDlg,IDC_USESHELLIMAGES,TRUE);
	else
		CheckDlgButton(hDlg,IDC_USECUSTOMIMAGES,TRUE);

	std::map<HWND,std::wstring>* hwndMap = (std::map<HWND,std::wstring>* ) RemoveProp(hDlg,_T("ChildMap"));
	if(hwndMap)
	{
		for(std::map<HWND,std::wstring>::iterator iter = hwndMap->begin(); iter != hwndMap->end(); ++iter)
		{
			HWND hwnd = iter->first;
			DestroyWindow(hwnd);
		}
	}

	delete hwndMap;

	HWND hwndChild = NULL;

	if(lpData->GetBehaviour() )
	{
		std::map<HWND,std::wstring>* hwndMap = new std::map<HWND,std::wstring>;

		int count(0);

		std::map<std::wstring,std::wstring> imagesMap = lpData->GetBehaviour()->GetMapOfImageNames();
		for(std::map<std::wstring,std::wstring>::iterator iter = imagesMap.begin(); iter != imagesMap.end(); ++iter)
		{
			std::wstring keyName = iter->first;
			HWND child = CreateImageSetWindow(lpData->GetHInstance(),hDlg);

			SendMessage(child,WM_SETTITLE,0,(LPARAM)keyName.c_str());
			SendMessage(child,WM_SETIMAGE_FILENAME,0,(LPARAM)lpData->GetCustomImages()[keyName].c_str());

			(*hwndMap)[child] = keyName;

			SetWindowPos(child,HWND_TOP,40 + 150 * count,90,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);

			EnableWindow(child,lpData->GetUseCustomImages() ? TRUE : FALSE);
			count++;

		}
		SetProp(hDlg,_T("ChildMap"),(HANDLE)hwndMap);
	}
}


//	This is a DLGPROC for the Label propertysheetpage.
int __stdcall IconConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			// store the FileDesklet-pointer of this desklet-instance in a WindowProperty, so
			// we can use it in other calls

			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,_T("lpData"),(HANDLE)psp->lParam);
			CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*)GetProp(hDlg,"lpData");
			LoadIconDlgSettings(hDlg,lpData);
		}
		break;

		case WM_USER_PATHCHANGED:
			LoadIconDlgSettings(hDlg,(CPidlShortcutDesklet*)GetProp(hDlg,"lpData"));
		break;


		
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_USESHELLIMAGES: case IDC_USECUSTOMIMAGES:
					if(HIWORD(wParam)==BN_CLICKED)
					{
						PropSheet_Changed(GetParent(hDlg),hDlg);

						std::map<HWND,std::wstring>* hwndMap = (std::map<HWND,std::wstring>* ) GetProp(hDlg,_T("ChildMap"));

						if(hwndMap)
						{
							for(std::map<HWND,std::wstring>::iterator iter = hwndMap->begin(); iter != hwndMap->end(); ++iter)
							{
								HWND child = iter->first;

								EnableWindow(child,IsDlgButtonChecked(hDlg,IDC_USECUSTOMIMAGES) );
							}

						}
					}
				break;


			}
		break;
		

		
		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				// Apply is pressed, so commit the changes.
				case PSN_APPLY:
				{
					CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*)GetProp(hDlg,_T("lpData"));
					if(lpData)
					{
						lpData->SetUseCustomImages( IsDlgButtonChecked(hDlg,IDC_USECUSTOMIMAGES) == BST_CHECKED);

						std::map<HWND,std::wstring>* hwndMap = (std::map<HWND,std::wstring>* ) GetProp(hDlg,_T("ChildMap"));

						if(hwndMap)
						{
							for(std::map<HWND,std::wstring>::iterator iter = hwndMap->begin(); iter != hwndMap->end(); ++iter)
							{
								HWND child = iter->first;
								std::wstring& keyName = iter->second;

								WCHAR fname[MAX_PATH+2] = {0};
								SendMessage(child,WM_GETIMAGE_FILENAME,MAX_PATH+2,(LPARAM)fname);
								lpData->GetCustomImages()[keyName] = fname;

								EnableWindow(child,lpData->GetUseCustomImages() ? TRUE : FALSE);
							}

							lpData->UpdateIcon();
						}
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
		{
			// remove the property from the window
			CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*) RemoveProp(hDlg,"lpData");
			if(lpData)
				lpData->SetHwndIconSheet(NULL);
			//RemoveProp(hDlg,_T("ChildIconHwnd"));
			std::map<HWND,std::wstring>* hwndMap = (std::map<HWND,std::wstring>* ) RemoveProp(hDlg,_T("ChildMap"));
			delete hwndMap;
		}
		break;
	}
	return FALSE;
}


void LoadLabelDlgSettings(HWND hDlg,CPidlShortcutDesklet* lpData)
{
	if(lpData)
	{
		lpData->SetHwndLabelSheet(hDlg);

		if(lpData->GetBehaviour() )
		{
			SendDlgItemMessageW(hDlg, IDC_FORMATS, LB_RESETCONTENT, 0L, 0L );

			std::vector<std::wstring> items = lpData->GetBehaviour()->GetCustomSublabelItems();
			for(std::vector<std::wstring>::iterator iter = items.begin(); iter != items.end(); ++ iter)
			{	
				std::wstring& s = *iter;
				SendDlgItemMessageW(hDlg, IDC_FORMATS, LB_ADDSTRING, 0L, reinterpret_cast<LPARAM>( s.c_str()) );
			}
			SendDlgItemMessageW(hDlg, IDC_FORMATS, LB_ADDSTRING, 0L, reinterpret_cast<LPARAM>( L"") );
		}

		CShellFolder shellFolder;

		if(lpData->GetPidl().IsValid() )
			lpData->GetPidl().GetParentShellFolder(&shellFolder,NULL);
		else
			shellFolder.SetToDesktopFolder();


		std::wstring str;
		int i(0);
		while( SUCCEEDED(shellFolder.GetShellDetailString(NULL,str,i++)) )
		{
			std::wstringstream ws;
			ws << L"%" << i-1 << L"\t" << str;
			SendDlgItemMessageW(hDlg, IDC_FORMATS, LB_ADDSTRING, 0L, reinterpret_cast<LPARAM>( ws.str().c_str()) );
		}
		
		CheckDlgButton(hDlg,IDC_AUTOSETCAPTION,lpData->GetAutoSetCaption() ? TRUE : FALSE);

		SetDlgItemTextW(hDlg,IDC_SUBLABEL,lpData->GetSubLabelFormat().c_str());

	}
}

//	This is a DLGPROC for the Label propertysheetpage.
int __stdcall LabelFormatConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			// store the FileDesklet-pointer of this desklet-instance in a WindowProperty, so
			// we can use it in other calls

			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*)GetProp(hDlg,"lpData");

			LoadLabelDlgSettings(hDlg,lpData);

		}
		break;
		
		case WM_USER_PATHCHANGED:
			LoadLabelDlgSettings(hDlg,(CPidlShortcutDesklet*)GetProp(hDlg,"lpData"));
		break;

		
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_AUTOSETCAPTION: //case IDC_AUTOLABEL:
					// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==BN_CLICKED)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDC_SUBLABEL:
					// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;
		

		
		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				// Apply is pressed, so commit the changes.
				case PSN_APPLY:
				{
					CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*)GetProp(hDlg,"lpData");
					if(lpData)
					{
						lpData->SetAutoSetCaption( IsDlgButtonChecked(hDlg,IDC_AUTOSETCAPTION) == BST_CHECKED);

						WCHAR tmp[10024] = {0};
						GetDlgItemTextW(hDlg,IDC_SUBLABEL,tmp,10024);
						lpData->SetSubLabelFormat(tmp);
					}
				}
				break;
			}
		break;

		case WM_DESTROY:
			// remove the property from the window
			CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*) RemoveProp(hDlg,"lpData");
			if(lpData)
				lpData->SetHwndLabelSheet(NULL);
		break;
	}
	return FALSE;
}

// same except for images sheet
int __stdcall OtherConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*)GetProp(hDlg,"lpData");

			CPidlDropBox* pidlDropBox = new CPidlDropBox(hDlg);
			LPDROPTARGET dropTarget = {0};
			if (FAILED(pidlDropBox->QueryInterface(IID_IDropTarget,reinterpret_cast<void**>(&dropTarget))) )
				delete pidlDropBox;
			else
			{
				RegisterDragDrop(hDlg/*GetDlgItem(hDlg,IDC_ICONDROP)*/,dropTarget);
				dropTarget->Release();
			}
			
		
			if(!lpData)return TRUE;


			SetDlgItemTextW(hDlg,IDC_PATH,lpData->GetPath().c_str() );
			SHAutoComplete(GetDlgItem(hDlg,IDC_PATH),SHACF_DEFAULT);

			if(lpData->GetPidl().IsValid() )
			{
				CPidl p;
				lpData->GetPidl().GetRelativePidl(p.m_Pidl);
				SetDlgItemTextW(hDlg,IDC_PIDLSTRING,p.ToString().c_str() );
			}

			SetDlgItemTextW(hDlg,IDC_STARTIN,lpData->GetStartIn().c_str());

			SHAutoComplete(GetDlgItem(hDlg,IDC_STARTIN),SHACF_FILESYSTEM);

			SendDlgItemMessageW(hDlg,IDC_EXECUTEAS,CB_ADDSTRING,0,(LPARAM)L"Normal Window");
			SendDlgItemMessageW(hDlg,IDC_EXECUTEAS,CB_ADDSTRING,0,(LPARAM)L"Minimized Window");
			SendDlgItemMessageW(hDlg,IDC_EXECUTEAS,CB_ADDSTRING,0,(LPARAM)L"Maximized Window");
			SendDlgItemMessageW(hDlg,IDC_EXECUTEAS,CB_SETCURSEL,lpData->GetExecuteAs(),0);

			SetDlgItemTextW(hDlg,IDC_PARAMS,lpData->GetParams().c_str());

			HICON icon = reinterpret_cast<HICON>( SendDlgItemMessage(hDlg,IDC_ICONDROP,STM_SETIMAGE,IMAGE_ICON,reinterpret_cast<LPARAM>(lpData->GetPidl().ExtractIcon(32))) );
			DestroyIcon(icon);
		}
		break;

		case WM_PIDLDROPNOTIFY:
		{
			CPidl pidl;
			pidl.SetFromPIDL( reinterpret_cast<LPITEMIDLIST>(lParam) );

			//CPidl pidl2;
			//pidl2.FromString( pidl.ToString() );
			//if( ILIsEqual(pidl,pidl2) )
			//	MessageBox(hDlg,TEXT("equal"),0,0);

			SetDlgItemTextW(hDlg,IDC_PATH,pidl.GetFullParseDisplayName().c_str() );
			HICON icon = reinterpret_cast<HICON>( SendDlgItemMessage(hDlg,IDC_ICONDROP,STM_SETIMAGE,IMAGE_ICON,reinterpret_cast<LPARAM>(pidl.ExtractIcon(32))) );
			DestroyIcon(icon);

			if(pidl.IsValid() )
				SetDlgItemTextW(hDlg,IDC_PIDLSTRING,pidl.ToString().c_str() );

			pidl.Detach();
		}
		break;

		case WM_CONTEXTMENU:
		{
			if(!OpenClipboard(hDlg))
				break;

			UINT shellIdListClipFormat = RegisterClipboardFormat(CFSTR_SHELLIDLIST);

			if(IsClipboardFormatAvailable(shellIdListClipFormat) == 0)
			{
				CloseClipboard();
				break;
			}

			HMENU hMenu = CreatePopupMenu();

			AppendMenu(hMenu,MF_ENABLED | MF_STRING,1,_T("Paste File As Settings"));


			DWORD dwPos = GetMessagePos();

			UINT cmd = TrackPopupMenu(hMenu,TPM_RETURNCMD,LOWORD(dwPos),HIWORD(dwPos),0,hDlg,NULL);

			if(cmd != 1)
			{
				CloseClipboard();
				break;
			}

			HANDLE h = GetClipboardData(shellIdListClipFormat);

			if( h == NULL)
			{
				CloseClipboard();
				break;
			}


			LPIDA pida = reinterpret_cast<LPIDA>( GlobalLock(h) );

			if(pida)
			{
				LPCITEMIDLIST pidlFolder= HIDA_GetPIDLFolder(pida);

				if(pida->cidl > 0)
				{
					LPCITEMIDLIST pidl = HIDA_GetPIDLItem(pida,0);
					LPITEMIDLIST pidlFull = ILCombine(pidlFolder,pidl);
				
					SendMessage(hDlg,WM_PIDLDROPNOTIFY,0L,reinterpret_cast<LPARAM>(pidlFull) ) ;

					CoTaskMemFree(reinterpret_cast<void*>(pidlFull));
				}
			}

			GlobalUnlock(h);

		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{				
				case IDC_EXECUTEAS:
				// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==CBN_SELCHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
				

				case IDC_PATH: 
					if(HIWORD(wParam) == EN_CHANGE)
					{
						SetDlgItemText(hDlg,IDC_PIDLSTRING,TEXT(""));
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				break;

				case IDC_PARAMS: case IDC_STARTIN:
					// the user changed something, so notify the propertydialogue that there was a change.
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;

				case IDBROWSE:
				{
					CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*)GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

					WCHAR wPathInEditBox[MAX_PATH+2] = {0};
					GetDlgItemTextW(hDlg,IDC_PATH,wPathInEditBox,MAX_PATH);

					OPENFILENAMEW ofn = {0};
					ofn.hInstance = lpData->GetHInstance();
					ofn.hwndOwner = hDlg;
					ofn.lpstrFile = wPathInEditBox;
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFilter = L"All Files(*.*)\0*.*\0\0";
					ofn.lpstrTitle = L"Select File";
					ofn.nMaxFile = MAX_PATH;


					if (GetOpenFileNameW(&ofn))
					{
						SetDlgItemTextW(hDlg,IDC_PATH,wPathInEditBox);
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				}
				break;

			
				case IDBROWSE2:
				{
					BROWSEINFO bi = { 0 };
					bi.lpszTitle = _T("Pick a Directory");
					LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
					if ( pidl != 0 )
					{
						CPidl cPidl;
						cPidl.SetFromPIDL(pidl);

						SetDlgItemTextW(hDlg,IDC_STARTIN,cPidl.GetFullParseDisplayName().c_str());
						PropSheet_Changed(GetParent(hDlg),hDlg);
					}
				}
	
				break;
			}
		break;


		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
				{
					CPidlShortcutDesklet* lpData = (CPidlShortcutDesklet*)GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

					WCHAR wPathInEditBox[MAX_PATH+2] = {0};
					GetDlgItemTextW(hDlg,IDC_PATH,wPathInEditBox,MAX_PATH);

					CPidl pidl1;
					pidl1.SetFromPath(wPathInEditBox);
					if(!pidl1.IsValid() )
					{
						WCHAR wPidlStringInEditBox[MAX_PATH*100] = {0};
						GetDlgItemTextW(hDlg,IDC_PIDLSTRING,wPidlStringInEditBox,MAX_PATH*100);
						CPidl pidl2;
						pidl2.FromString(wPidlStringInEditBox);
						if( pidl2.IsValid() )
						{
							if(pidl2.GetFullParseDisplayName() == wPathInEditBox)
								lpData->UpdatePath(wPathInEditBox,wPidlStringInEditBox);
							else
								lpData->UpdatePath(wPathInEditBox);
						}
						else
						{
							lpData->UpdatePath(wPathInEditBox);
						}
					}
					else
					{
						lpData->UpdatePath(wPathInEditBox);
					}

					WCHAR wParams[10024] = {0};
					GetDlgItemTextW(hDlg,IDC_PARAMS,wParams,10024);
					lpData->SetParams(wParams);

					lpData->SetExecuteAs( SendDlgItemMessage(hDlg,IDC_EXECUTEAS,CB_GETCURSEL,0,0) );

					WCHAR wPathStartIn[MAX_PATH+2] = {0};
					GetDlgItemTextW(hDlg,IDC_STARTIN,wPathStartIn,MAX_PATH);

					lpData->SetStartIn(wPathStartIn);

					HICON icon = reinterpret_cast<HICON>( SendDlgItemMessage(hDlg,IDC_ICONDROP,STM_SETIMAGE,IMAGE_ICON,reinterpret_cast<LPARAM>(lpData->GetPidl().ExtractIcon(32)) ) );
					DestroyIcon(icon);

				}
				break;
			}
		break;

		case WM_DESTROY:
		{
			HICON icon = reinterpret_cast<HICON>(SendDlgItemMessage(hDlg,IDC_ICONDROP,STM_GETIMAGE,IMAGE_ICON,0L) );
			DestroyIcon(icon);

			RemoveProp(hDlg,"lpData");
		}
		break;
	}
	return FALSE;
}

#define HIDA_GetPIDLFolder(pida) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[0])
#define HIDA_GetPIDLItem(pida, i) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[i+1])

BOOL CPidlShortcutDesklet::DragDropDataRequest(FORMATETC* pFetc, STGMEDIUM* pMedium)
{
	CComPtr<IDataObject> dataObj;
	if( SUCCEEDED(m_Pidl.GetUIObject(NULL,IID_IDataObject, reinterpret_cast<void**>(&dataObj))) )
		return SUCCEEDED(dataObj->GetData(pFetc,pMedium));
	else
		return FALSE;
}

int CPidlShortcutDesklet::AddPropertySheets(int numPresent)
{
	PROPSHEETPAGE psp = {0};
	psp.dwSize = sizeof(psp);


	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = GetHInstance();
	psp.pszTitle = _T("Label Format");
	psp.pszTemplate = MAKEINTRESOURCE(IDD_LABEL);
	psp.pfnDlgProc = LabelFormatConfigProc;
	psp.lParam = reinterpret_cast<LPARAM>(this);

	AddPropertySheet(CreatePropertySheetPage(&psp));

	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = GetHInstance();
	psp.pszTitle = _T("Icon");
	psp.pszTemplate = MAKEINTRESOURCE(IDD_ICON);
	psp.pfnDlgProc = IconConfigProc;
	psp.lParam = reinterpret_cast<LPARAM>(this);

	AddPropertySheet(CreatePropertySheetPage(&psp));
	

	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = GetHInstance();
	psp.pszTitle = _T("Shortcut");
	psp.pszTemplate = MAKEINTRESOURCE(IDD_SHORTCUT);
	psp.pfnDlgProc = OtherConfigProc;
	psp.lParam = reinterpret_cast<LPARAM>(this);


	return AddPropertySheet(CreatePropertySheetPage(&psp));
}