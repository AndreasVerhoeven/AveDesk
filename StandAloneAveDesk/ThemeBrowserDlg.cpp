// ThemeBrowserDlg.cpp: implementation of the CThemeBrowserDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThemeBrowserDlg.h"
#include "Application.h"
#include "helper_funcs.h"
#include "XMLLoader.h"


extern CComObject<CApplication> *_App;

#include "AveDeskGlobalSettings.hpp"
extern AveDeskGlobalSettings _globalSettings;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// {75048700-EF1F-11D0-9888-006097DEACF9}
DEFINE_GUID( CLSID_ActiveDesktop, 0x75048700L, 0xEF1F, 0x11D0, 0x98, 0x88, 0x00, 0x60, 0x97, 0xDE, 0xAC, 0xF9);

// {F490EB00-1240-11D1-9888-006097DEACF9}
DEFINE_GUID(IID_IActiveDesktop, 0xF490EB00L, 0x1240, 0x11D1, 0x98, 0x88, 0x00, 0x60, 0x97, 0xDE, 0xAC, 0xF9);


CThemeBrowserDlg::CThemeBrowserDlg()
{

}

CThemeBrowserDlg::~CThemeBrowserDlg()
{

}

LRESULT CThemeBrowserDlg::OnCloseButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(TRUE);
	return 0;
}

LRESULT CThemeBrowserDlg::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int curSel = lb.GetCurSel();
	BOOL hasSel = curSel != LB_ERR;

	::EnableWindow(GetDlgItem(IDC_LOADTHEME), hasSel);
	::EnableWindow(GetDlgItem(IDC_DELETETHEME), hasSel);

    return 0;
}

LRESULT CThemeBrowserDlg::OnListDoubleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return OnLoadThemeButton(wNotifyCode, wID, hWndCtl, bHandled);
}

LRESULT CThemeBrowserDlg::OnLoadThemeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int curSel = lb.GetCurSel();
	if(curSel != LB_ERR)
	{
		CComBSTR themeStr(m_Filenames[curSel].c_str());
		_App->LoadTheme(themeStr);
	}

	return 0;
}

LRESULT CThemeBrowserDlg::OnDeleteThemeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int curSel = lb.GetCurSel();
	if(curSel != LB_ERR)
	{
		CString strDeleteTheme;
		strDeleteTheme.LoadString(IDS_DELETETHEME);
		CString strDeleteThemeCaption;
		strDeleteThemeCaption.LoadString(IDS_DELETETHEMECAPTION);
		if(!_globalSettings.GetAsk() || ::MessageBoxW(m_hWnd, strDeleteTheme, strDeleteThemeCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			WCHAR path[MAX_PATH] = {0};
			wcscpy(path,_App->GetAveDeskThemeDir().c_str());
			PathAppendW(path,m_Filenames[curSel].c_str());

			DeleteFile(path);
			m_Filenames.erase(m_Filenames.begin() + curSel);
			lb.DeleteString(curSel);

			::EnableWindow(GetDlgItem(IDC_LOADTHEME), FALSE);
			::EnableWindow(GetDlgItem(IDC_DELETETHEME), FALSE);
		}
	}

	return 0;
}

LRESULT CThemeBrowserDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ModifyStyleEx(0,WS_EX_APPWINDOW); // for appearance in task bar
    HICON hIco = LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE( IDI_ICON2 ));
    SetIcon( hIco, TRUE );
    SetIcon( hIco, FALSE );
    CenterWindow();

	LOGFONTW lfBig = {0};
	lfBig.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfBig.lfFaceName,L"Tahoma");
	lfBig.lfWeight = FW_BOLD;
	lfBig.lfHeight = 14;

	LOGFONTW lfSmall = {0};
	lfSmall.lfCharSet = DEFAULT_CHARSET;
	wcscpy(lfSmall.lfFaceName,L"Tahoma");
	lfSmall.lfWeight = FW_NORMAL;
	lfSmall.lfHeight = 14;

	bigFont.CreateFontIndirect(&lfBig);
	smallFont.CreateFontIndirect(&lfSmall);


	const int previewWidth  = 1024 / 6;
	const int previewHeight = 768 /  6;
	m_ImageList.Create(previewWidth,previewHeight,ILC_COLOR32,1, 1);

	lb.SubclassWindow(GetDlgItem(IDC_THEMES));
	lb.SetItemHeight(0, previewHeight + 2);
	lb.SetImageList(m_ImageList, ILSIL_NORMAL);

	lb.SetFont(bigFont);
	DeleteObject(lb.SetSmallFont(smallFont));

    PopulateList();

    DlgResize_Init();
    return 1;
;
}

void CThemeBrowserDlg::PopulateList()
{
	SIZE iconSize = {0};
	m_ImageList.GetIconSize(iconSize);

	
	/*CWindowDC screenDC(NULL);
	CWindowDC desktopDC(::FindWindow(_T("Progman"),NULL));
	CDC dc;
	dc.CreateCompatibleDC(screenDC);

	RECT r = {0};
	::GetWindowRect(GetDesktopWindow(), &r);
	SIZE desktopSize = SIZEFROMRECT(r);

	BITMAPINFO bi = {0};
	bi.bmiHeader.biSize = 40;//sizeof(bi);
	bi.bmiHeader.biWidth = desktopSize.cx;
	bi.bmiHeader.biHeight = desktopSize.cy;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = 	BI_RGB;

	void* bitsPtr = NULL;

	CBitmap hDesktopBitmap;
	hDesktopBitmap.CreateDIBSection(screenDC, &bi, DIB_RGB_COLORS, &bitsPtr, NULL, 0);
	HBITMAP oldBitmap = dc.SelectBitmap(hDesktopBitmap);
	dc.BitBlt(0,0, desktopSize.cx, desktopSize.cy, desktopDC, 0, 0, SRCPAINT);


	Bitmap* bmp = new Bitmap(desktopSize.cx, desktopSize.cy);
	BitmapData bmpData = {0};
	Rect rect(0,0,bmp->GetWidth(),bmp->GetHeight());
	bmp->LockBits(&rect,ImageLockModeWrite,PixelFormat32bppRGB,&bmpData);
	GetBitmapBits(hDesktopBitmap,bmpData.Stride * bmpData.Height, bmpData.Scan0);
	bmp->UnlockBits(&bmpData);
	*/

	

	Bitmap desktopBackground(iconSize.cx, iconSize.cy);
	Graphics g(&desktopBackground);
	//g.Clear(Color(255,255,0,0));
//	g.DrawImage(bmp, RectF(0, 0, iconSize.cx, iconSize.cy), 0, 0, bmp->GetWidth(), bmp->GetHeight(), UnitPixel, NULL, NULL, NULL);

	
	IActiveDesktop* iad = NULL;
	HRESULT hRes = ::CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER,
                      IID_IActiveDesktop, (void**)&iad);
	if(iad != NULL)
	{
		WCHAR wallpaperPath[MAX_PATH] = {0};
		hRes = iad->GetWallpaper(wallpaperPath, MAX_PATH, 0);
		if(SUCCEEDED(hRes))
		{
			Bitmap wallpaper(wallpaperPath);
			g.DrawImage(&wallpaper, RectF(0.0f, 0.0f, (REAL)iconSize.cx, (REAL)iconSize.cy), 0.0f, 0.0f, (REAL)wallpaper.GetWidth(), (REAL)wallpaper.GetHeight(), UnitPixel, NULL, NULL, NULL);
		}
		iad->Release();
	}

	//dc.Destroy();

	WIN32_FIND_DATAW data = {0};

	WCHAR path[MAX_PATH] = {0};
	wcscpy(path,_App->GetAveDeskThemeDir().c_str());
	PathAppendW(path,L"*.avetheme");
	HANDLE h = FindFirstFileW(path,&data);
	if( h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(wcscmp(data.cFileName,L".") != 0 && wcscmp(data.cFileName,L"..") != 0)
			{
				m_Filenames.push_back(data.cFileName);

				boost::shared_ptr<CXMLLoader> loader;
				CString extraInfo;
				extraInfo.LoadString(IDS_NOINFO);
				
				WCHAR absolutePath[MAX_PATH] = {0};
				wcscpy(absolutePath,_App->GetAveDeskThemeDir().c_str());
				PathAppendW(absolutePath, data.cFileName);

				Bitmap previewBitmap(iconSize.cx, iconSize.cy);
				previewBitmap.SetPixel(1,1, Color(10,255,255,255));
				Graphics g(&previewBitmap);

				if(StgIsStorageFile(absolutePath) == S_OK) // explicit check for S_OK
				{
					HRESULT hRes = S_OK;
					CComPtr<IStorage> storage;
					hRes = StgOpenStorageEx(absolutePath, STGM_READ | STGM_DIRECT | STGM_SHARE_DENY_WRITE, STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, reinterpret_cast<void**>(&storage));
					if(SUCCEEDED(hRes) && storage.p != NULL)
					{
						CComPtr<IStream> stream;
						hRes = storage->OpenStream(L"Preview", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, &stream);
						if(SUCCEEDED(hRes) && stream.p != NULL)
						{
							boost::shared_ptr<Bitmap> extractedBitmap(new Bitmap(stream, FALSE));

							g.DrawImage(&desktopBackground,0,0,iconSize.cx, iconSize.cy);
							g.DrawImage(extractedBitmap.get(), RectF(0.0f,0.0f,(REAL)iconSize.cx, (REAL)iconSize.cy), 0.0f,0.0f, (REAL)extractedBitmap->GetWidth(), (REAL)extractedBitmap->GetHeight(),UnitPixel, NULL, NULL, NULL);
						}

						CComPtr<IStream> themeStream;
						hRes = storage->OpenStream(L"Theme", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE | STGM_DIRECT, NULL, &themeStream);
						if(SUCCEEDED(hRes) && stream.p != NULL)
							loader = boost::shared_ptr<CXMLLoader>(new CXMLLoader(themeStream));
					}
				}
				else
				{
					g.Clear(Color(255,255,255,255));
					loader = boost::shared_ptr<CXMLLoader>(new CXMLLoader(absolutePath));
				}


				if(loader.get() != NULL)
				{
					loader->SelectChildNode(L"AveDesk");
					loader->SelectChildNode(L"theme");
					if(SUCCEEDED(hRes = loader->SelectChildNode(L"information")))
					{
						// load number of desklets info
						std::wstring wsNumOfDesklets;
						loader->LoadString(wsNumOfDesklets, L"NumberOfDesklet");

						// load author info
						std::wstring wsAuthor;
						loader->LoadString(wsAuthor, L"Author");

						// load avedeskversion info
						std::wstring wsAveDeskVersion;
						loader->LoadString(wsAveDeskVersion, L"AveDeskVersion");

						extraInfo.LoadString(IDS_INFOSTRING);
						extraInfo.Replace(L"%1", wsNumOfDesklets.c_str());
						extraInfo.Replace(L"%2", wsAuthor.c_str());
						extraInfo.Replace(L"%3", wsAveDeskVersion.c_str());
					}
				}

				
				CBitmap hBmp;
				previewBitmap.GetHBITMAP(NULL,&hBmp.m_hBitmap);

				ImageList_Add(m_ImageList.m_hImageList, hBmp, NULL);

				ILBITEM item = {0};
				item.mask = ILBIF_TEXT | ILBIF_SUBTEXT | ILBIF_IMAGE |   ILBIF_STYLE | ILBIF_FORMAT | ILBIF_PARAM;
				item.iItem = lb.GetCount() - 1;
				item.pszText = data.cFileName;
				item.pszSubText = const_cast<LPTSTR>(extraInfo.operator LPCTSTR());
				item.style = ILBS_IMGLEFT;
				item.format = DT_LEFT;
				item.iImage = m_Filenames.size() -1;
				item.lParam = static_cast<LPARAM>(m_Filenames.size()-1);
				int index = lb.AddItem(&item);
				

			}
		}while(FindNextFileW(h,&data));
		FindClose(h);
	}
}

LRESULT CThemeBrowserDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

    EndDialog(FALSE);

	return 0;
}
LRESULT CThemeBrowserDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return 0;
}