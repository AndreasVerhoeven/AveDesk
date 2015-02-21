#include "FilmStrip.h"

#include <sstream>
#include <memory>

#include "../AveDeskUtil.hpp"

#ifndef __ATLBASE_H__
#include <ATLBASE.h>
#endif

#ifndef __ATLCONV_H__
#include <ATLCONV.H>
#endif

#include <shlwapi.h>
#include <shlguid.h>

// {75048700-EF1F-11D0-9888-006097DEACF9}
DEFINE_GUID( CLSID_ActiveDesktop, 0x75048700L, 0xEF1F, 0x11D0, 0x98, 0x88, 0x00, 0x60, 0x97, 0xDE, 0xAC, 0xF9);

// {F490EB00-1240-11D1-9888-006097DEACF9}
DEFINE_GUID(IID_IActiveDesktop, 0xF490EB00L, 0x1240, 0x11D1, 0x98, 0x88, 0x00, 0x60, 0x97, 0xDE, 0xAC, 0xF9);


#include <shlobj.h>
#pragma comment(lib,"shlwapi.lib")

CFilmStripDesklet::DEFAULT_DESKLET_CONSTRUCTOR_BODY(CFilmStripDesklet),
	scrollingDelta(0),
	isScrollingLeft(false),
	scrollingSpeed(3)
{
	ActivateExtendedDragDrop();

	if(bNotLoaded)
		SetSkin("default");
}

void CFilmStripDesklet::SaveWallpaperList()
{
/*	std::stringstream ssIni;
	ssIni << GetDLLDirectory() << "\\Settings\\" << (long)GetWindow() << ".ini";
	std::string ini = ssIni.str();

	SaveWallpaperList(ini.c_str());
*/
}

void CFilmStripDesklet::SaveWallpaperList(const char* iniFile, const char* iniSection)
{
	::WritePrivateProfileInt(iniSection,"Count",m_wallpapers.size(),iniFile);

	for(int i=0;i<m_wallpapers.size();++i)
	{
		std::stringstream ssKey;
		ssKey << "Wallpaper" << i+1;

		::WritePrivateProfileString(iniSection,ssKey.str().c_str(),m_wallpapers[i].c_str(),iniFile);
	}
}

void CFilmStripDesklet::ReadWallpaperList(const char* iniFile, const char* iniSection)
{
	m_wallpapers.clear();

	int count = ::GetPrivateProfileInt(iniSection,"Count",0,iniFile);

	for(int i=0;i<count;++i)
	{
		std::stringstream ssKey;
		ssKey << "Wallpaper" << i+1;

		char buf[MAX_PATH+2];
		memset(buf,NULL,MAX_PATH+2);

		::GetPrivateProfileString(iniSection,ssKey.str().c_str(),buf,buf,MAX_PATH+2,iniFile);

		m_wallpapers.push_back(buf);
	}
}

void CFilmStripDesklet::ReadFromIni(const char* iniFile, const char* iniSection)
{
//	char ini[MAX_PATH+2];
//	memset(ini,0,MAX_PATH+2);

	//::GetPrivateProfileString(iniSection,"WallpaperList",ini,ini,MAX_PATH,iniFile);

	//if(ini[0] != 0)
	ReadWallpaperList(iniFile,iniSection);

	char skinDir[MAX_PATH+2];
	memset(skinDir,0,MAX_PATH+2);
	strcpy(skinDir,"Default");
	::GetPrivateProfileString(iniSection,"Skin",skinDir,skinDir,MAX_PATH,iniFile);
	SetSkin(skinDir,TRUE);
}

void CFilmStripDesklet::SaveToIni(const char* iniFile, const char* iniSection)
{
	//std::stringstream ssIni;
	//ssIni << GetDLLDirectory() << "\\Settings\\" << (long)GetWindow() << ".ini";
	//std::string ini = ssIni.str();

	SaveWallpaperList(iniFile,iniSection);

	//::WritePrivateProfileString(iniSection,"WallpaperList",ini.c_str(),iniFile);

	::WritePrivateProfileString(iniSection,"Skin",m_skinDir.c_str(),iniFile);
}

BOOL CFilmStripDesklet::FilesDrop(std::vector<std::string>& files,DWORD /*keys*/,POINTL /*pnt*/, DWORD* /*effect*/)
{
	for(std::vector<std::string>::iterator iter = files.begin(); iter != files.end();++iter)
		m_wallpapers.push_back(*iter);


	SaveWallpaperList();
	RebuildOverlayBuffer();
	MakeOverlay(TRUE);

	return TRUE;
}

void CFilmStripDesklet::SetSkin(std::string dir, BOOL noResize)
{
	m_skinDir = dir;
	std::string path( GetDLLDirectory() + "\\skins\\" + m_skinDir + "\\skin.ini");
	m_skin =  CSkin(path.c_str());

	if(!noResize)
		Move(GetLeft(),GetTop(),m_skin.GetWidth(),m_skin.GetHeight(),FALSE);

	RebuildOverlayBuffer();
	MakeBackground(FALSE);
	MakeOverlay(TRUE);
}

std::string CFilmStripDesklet::GetAbsoluteSkinFolder()
{
	return GetDLLDirectory() + "\\skins\\" + m_skinDir;
}

void CFilmStripDesklet::MakeBackground(BOOL redraw)
{
	m_layers.RemoveAll();

	CLayer& bgLayer = m_layers.Add("FilmStrip.Background");

	Bitmap* bmpBg = new Bitmap(m_skin.GetWidth(),m_skin.GetHeight() );
	Graphics g(bmpBg);

	std::auto_ptr<Bitmap>bmpLeft ( AveDeskUtil::LoadGDIPlusImage(GetAbsoluteSkinFolder() +"\\images\\left.png") );
	std::auto_ptr<Bitmap>bmpMid ( AveDeskUtil::LoadGDIPlusImage(GetAbsoluteSkinFolder() +"\\images\\mid.png") );
	std::auto_ptr<Bitmap>bmpRight ( AveDeskUtil::LoadGDIPlusImage(GetAbsoluteSkinFolder() +"\\images\\right.png") );

	if(m_skin.IsVertical())
	{
		int h = m_skin.GetHeight();
		int t = bmpLeft->GetHeight();
		g.DrawImage(&(*bmpLeft),0,0,bmpLeft->GetWidth(),bmpLeft->GetHeight() );
		h -= bmpLeft->GetHeight();
		g.DrawImage(&(*bmpRight),0,m_skin.GetHeight() - bmpRight->GetHeight(),bmpRight->GetHeight(),bmpRight->GetHeight());
		h -= bmpRight->GetHeight();

		int num = h / bmpMid->GetHeight();
		for(int i=0;i<num;++i)
		{
			g.DrawImage(&(*bmpMid),0,t,bmpMid->GetWidth(),bmpMid->GetHeight());
			t += bmpMid->GetHeight();
			h -= bmpMid->GetHeight();
		}

		if(h>0)
		{
			Rect rc(0,t,bmpMid->GetWidth(),h);
			g.DrawImage(&(*bmpMid),rc,0,0,bmpMid->GetWidth(),h,UnitPixel);
		}
	}
	else
	{
		int w = m_skin.GetWidth();
		int l = bmpLeft->GetWidth();
		g.DrawImage(&(*bmpLeft),0,0,bmpLeft->GetWidth(),bmpLeft->GetHeight() );
		w -= bmpLeft->GetWidth();
		g.DrawImage(&(*bmpRight),m_skin.GetWidth() - bmpRight->GetWidth(),0,bmpRight->GetWidth(),bmpRight->GetHeight());
		w -= bmpRight->GetWidth();

		TextureBrush b(&(*bmpMid));
		b.TranslateTransform(m_skin.GetLeftMargin(),0);
		g.FillRectangle(&b,l,0,w,bmpMid->GetHeight());

	}

	bgLayer.SetImage(bmpBg,TRUE,redraw);
}

void CFilmStripDesklet::MakeOverlay(BOOL redraw)
{
	Rect rcFrame = m_skin.GetFrameRect();

	Bitmap* bgOverlay = NULL;
	if(m_skin.IsVertical() )
		bgOverlay = new Bitmap(rcFrame.Width,m_skin.GetHeight() - m_skin.GetLeftMargin() - m_skin.GetRightMargin());
	else
		bgOverlay = new Bitmap(m_skin.GetWidth() - m_skin.GetLeftMargin() - m_skin.GetRightMargin(),rcFrame.Height);

	Graphics g(bgOverlay);
	
	if(m_skin.IsVertical())
	{
		Rect rcDraw(0,0,bgOverlay->GetWidth(),bgOverlay->GetHeight());
		g.DrawImage(&(*overlayBuffer),rcDraw,rcDraw.X,scrollingDelta,rcDraw.Width,rcDraw.Height,UnitPixel);
	}
	else
	{
		Rect rcDraw(0,0,bgOverlay->GetWidth(),bgOverlay->GetHeight());
		g.DrawImage(&(*overlayBuffer),rcDraw,scrollingDelta,rcDraw.Y,rcDraw.Width,rcDraw.Height,UnitPixel);
	}
	
	CLayer& overlayLayer = m_layers["FilmStrip.Overlay"];
	if(m_skin.IsVertical() )
		overlayLayer.SetPlacement(TRUE,TRUE,rcFrame.X,m_skin.GetLeftMargin(),bgOverlay->GetWidth(),bgOverlay->GetHeight(),m_skin.GetWidth(),m_skin.GetHeight() );
	else
		overlayLayer.SetPlacement(TRUE,TRUE,m_skin.GetLeftMargin(),rcFrame.Y,bgOverlay->GetWidth(),bgOverlay->GetHeight(),m_skin.GetWidth(),m_skin.GetHeight() );

	overlayLayer.SetImage(bgOverlay,TRUE,redraw);
	//DeskletRedraw(GetWindow());
}

void CFilmStripDesklet::OnTimer(UINT msg, WPARAM w, LPARAM l)
{
	if(w==timerId)
	{
		
		if(isScrollingLeft)
		{
			if(m_skin.IsVertical())
			{
				if( scrollingDelta < overlayBuffer->GetHeight() - (m_skin.GetHeight() - m_skin.GetLeftMargin() - m_skin.GetRightMargin()))
				{
					scrollingDelta += scrollingSpeed;
					MakeOverlay(TRUE);
				}
			}
			else
			{
				if( scrollingDelta < overlayBuffer->GetWidth() - (m_skin.GetWidth() - m_skin.GetLeftMargin() - m_skin.GetRightMargin()))
				{
					scrollingDelta += scrollingSpeed;
					MakeOverlay(TRUE);
				}
			}
		}
		else
		{
			if(scrollingDelta > 0)
			{
				scrollingDelta -= scrollingSpeed;
				if(scrollingDelta<0)
					scrollingDelta = 0;
				MakeOverlay(TRUE);
			}
		}
	}
}

int CFilmStripDesklet::GetNumberOfFramesThatCanBeVisible()
{
	if(m_skin.IsVertical())
	{
		int h = m_skin.GetHeight() - m_skin.GetLeftMargin() - m_skin.GetRightMargin();
		int fh = m_skin.GetFrameRect().Height + m_skin.GetMarginBetweenTwoFrames();
		if(fh>0)
			return h / fh;
		else
			return 0;
	}
	else
	{
		int w = m_skin.GetWidth() - m_skin.GetLeftMargin() - m_skin.GetRightMargin();
		int fw = m_skin.GetFrameRect().Width + m_skin.GetMarginBetweenTwoFrames();
		if(fw>0)
			return w / fw;
		else
			return 0;
	}
}

void CFilmStripDesklet::RebuildOverlayBuffer()
{
	Rect rcFrame = m_skin.GetFrameRect();

	if(m_skin.IsVertical())
		overlayBuffer = std::auto_ptr<Bitmap>( new Bitmap(m_skin.GetFrameRect().Width,(m_skin.GetFrameRect().Height + m_skin.GetMarginBetweenTwoFrames())* m_wallpapers.size() ) );
	else
		overlayBuffer = std::auto_ptr<Bitmap>( new Bitmap((m_skin.GetFrameRect().Width + m_skin.GetMarginBetweenTwoFrames())* m_wallpapers.size(),m_skin.GetFrameRect().Height ) );

	Graphics g(&(*overlayBuffer));
	for(int i=0;i<m_wallpapers.size();++i)
	{
		std::string wallpaper = m_wallpapers[i];

		Rect rcDraw = rcFrame;
		rcDraw.X = rcDraw.Y = 0;
		if(m_skin.IsVertical() )
			rcDraw.Y = rcDraw.Height * i + m_skin.GetMarginBetweenTwoFrames()*i;
		else
			rcDraw.X = rcDraw.Width * i + m_skin.GetMarginBetweenTwoFrames()*i;

		std::auto_ptr<Bitmap>bmp ( AveDeskUtil::LoadGDIPlusImage(wallpaper) );

		g.DrawImage(&(*bmp),rcDraw,0,0,bmp->GetWidth(),bmp->GetHeight(),UnitPixel);
	}
}


BOOL CFilmStripDesklet::MouseOut()
{
	KillTimer(GetWindow(),timerId);
	SetSubLabel(std::string(""));

	return TRUE;
}

template<typename T> T RemapPoint(T& t, SIZE& s, float xScale=100, float yScale=100)
{
	T nT;
	nT.x = nT.y = 0;
	
	if(s.cx == 0 || s.cy == 0)
		return nT;

	nT.x = float(t.x) / float(s.cx) * xScale;
	nT.y = float(t.y) / float(s.cy) * yScale;

	return nT;
}

std::string GetFileName(std::string& filepath)
{
	int i=filepath.size()-1;
	while( i >= 0 && filepath[i]!='\\' && filepath[i]!='/')i--;

	return filepath.substr(i+1,filepath.size()-i-1);
}

int CFilmStripDesklet::GetWallpaperIndexFromPoint(POINT& rp)
{
	if(m_skin.IsVertical() )
	{
		if( rp.y  < m_skin.GetLeftMargin())return -1;
		if(rp.y >  m_skin.GetHeight() - m_skin.GetRightMargin()) return -1;
		rp.y -= m_skin.GetLeftMargin();
		rp.y += scrollingDelta;

		int item = rp.y / (m_skin.GetFrameRect().Height + m_skin.GetMarginBetweenTwoFrames());
		if(item < 0 || item >= m_wallpapers.size())return -1;

		//if(rp.y > item * (m_skin.GetFrameRect().Height + m_skin.GetMarginBetweenTwoFrames())) return -1;
		return item;
	}
	else
	{
		if( rp.x  < m_skin.GetLeftMargin())return -1;
		if(rp.x > m_skin.GetWidth() - m_skin.GetRightMargin()) return -1;

		rp.x -= m_skin.GetLeftMargin();
		rp.x += scrollingDelta;

		int item = rp.x / (m_skin.GetFrameRect().Width + m_skin.GetMarginBetweenTwoFrames());

		if(item < 0 || item >= m_wallpapers.size())return -1;

		//if(rp.x > item * (m_skin.GetFrameRect().Width + m_skin.GetMarginBetweenTwoFrames())) return -1;

		return item;
	}
}

BOOL CFilmStripDesklet::MouseMove(int x, int y)
{
	POINT p = {x,y};
	POINT rp = RemapPoint(p,GetSize(),m_skin.GetWidth(),m_skin.GetHeight() );

	if(m_skin.IsVertical())
	{
		if(rp.y<m_skin.GetLeftMargin() || rp.y>  m_skin.GetHeight() - m_skin.GetRightMargin())
		{
			if(rp.y<m_skin.GetLeftMargin())
			{
				scrollingSpeed = m_skin.GetLeftMargin() - rp.y;
				isScrollingLeft = false;
			}
			else
			{
				scrollingSpeed = rp.y - (  m_skin.GetHeight() - m_skin.GetRightMargin());
				isScrollingLeft = true;
			}

			SetTimer(GetWindow(),timerId,50,NULL);
			SetSubLabel(std::string(""));
			return TRUE;

		}
		else
			KillTimer(GetWindow(),timerId);

		return TRUE;
	}
	else
	{
		if(rp.x<m_skin.GetLeftMargin() || rp.x> m_skin.GetWidth() - m_skin.GetRightMargin())
		{
			if(x<m_skin.GetLeftMargin())
			{
				scrollingSpeed = m_skin.GetLeftMargin() - rp.x;
				isScrollingLeft = false;
			}
			else
			{
				scrollingSpeed = rp.x - (  m_skin.GetWidth() - m_skin.GetRightMargin());
				isScrollingLeft = true;
			}

			SetTimer(GetWindow(),timerId,50,NULL);
			SetSubLabel(std::string(""));
			return TRUE;
		}
		else
			KillTimer(GetWindow(),timerId);
	}

	int selIndex = GetWallpaperIndexFromPoint(rp);

	if(selIndex != -1)
		SetSubLabel(GetFileName(m_wallpapers[selIndex]));

	return TRUE;
}

BOOL CFilmStripDesklet::LeftButtonClick(POINT* p, SIZE* s)
{
	POINT rp = RemapPoint(*p,*s,m_skin.GetWidth(),m_skin.GetHeight() );

	int selIndex = GetWallpaperIndexFromPoint(rp);

	if(selIndex != -1)
	{
		SetWindowsWallpaper(selIndex);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

void CFilmStripDesklet::SetWindowsWallpaper(int i)
{
	if(i < 0 || i>= m_wallpapers.size() ) return;

	std::auto_ptr<Bitmap> original( AveDeskUtil::LoadGDIPlusImage(m_wallpapers[i]) );

	RECT r = {0};
	GetWindowRect(GetDesktopWindow(),&r);
	std::auto_ptr<Bitmap> bmp( new Bitmap(r.right- r.left,r.bottom - r.top) );
	Graphics g(&(*bmp));
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	g.DrawImage(&(*original),0,0,bmp->GetWidth(),bmp->GetHeight());

	CLSID encoderCLSID = {0};
	GetEncoderClsid(L"image/bmp",&encoderCLSID);

	WCHAR saveFileName[MAX_PATH+2] = {0};
	GetWindowsDirectoryW(saveFileName,MAX_PATH);
	PathAppendW(saveFileName,L"AveWallpaper.bmp");

	bmp->Save(saveFileName,&encoderCLSID,NULL);

	//CComQIPtr<IActiveDesktop, &IID_IActiveDesktop>iad;
	IActiveDesktop* iad = {0};
	::CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER,
                      IID_IActiveDesktop, (void**)&iad);
	if(iad)
	{
		//iad.CoCreateInstance(CLSID_ActiveDesktop);
		//MultiByteToWideChar(CP_OEMCP,0,m_wallpapers[i].c_str(),-1,saveFileName,MAX_PATH);
		iad->SetWallpaper(saveFileName,0);
		iad->ApplyChanges(AD_APPLY_ALL);
		iad->Release();
	}

	//HKEY hKey ={0};
	//RegOpenKeyExW(HKEY_CURRENT_USER,L"Control Panel\\Desktop\\",0,KEY_WRITE,&hKey);

	//RegSetValueExW(hKey,L"Wallpaper",0,REG_SZ,(BYTE*)&saveFileName[0],MAX_PATH);

	//RegCloseKey(hKey);

    //SystemParametersInfoW(SPI_SETDESKWALLPAPER,0,&saveFileName,SPIF_SENDWININICHANGE);
	//InvalidateRect(FindWindow("Progman",NULL),NULL,TRUE);

}

BOOL CFilmStripDesklet::RightButtonClick(POINT* p, SIZE* s)
{
	KillTimer(GetWindow(),timerId);

	POINT rp = RemapPoint(*p,*s,m_skin.GetWidth(),m_skin.GetHeight() );
	int selIndex = GetWallpaperIndexFromPoint(rp);

	HMENU menu = GetDefaultMenu(100);

	AppendMenu(menu,MF_SEPARATOR,0,"-");
	AppendMenu(menu,MF_STRING | MF_ENABLED,1,"Add Images...");
	AppendMenu(menu,MF_STRING | MF_ENABLED,2,"Remove All Images");
	if(selIndex != -1)
	AppendMenu(menu,MF_STRING | MF_ENABLED,3,"Remove The Image Under The Mousecursor");

	POINT pt = {0};
	GetCursorPos(&pt);
	int cmd = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, 0,GetWindow(),0);

	DestroyMenu(menu);

	if(cmd >= 100)
	{
		DoDefaultMenuCommand(100,cmd);
	}
	else
	{
		if(cmd==1)
		{
			InvokeAddImagesDialog();
		}
		else if(cmd==2)
		{
			if( MessageBox(GetWindow(),"This will remove all images from the filmstrip and can\'t be undone. Are you sure?","FilmStrip Desklet",MB_ICONQUESTION | MB_YESNO)==IDYES )
			{
				scrollingDelta = 0;
				isScrollingLeft = false;
				scrollingSpeed = 3;

				m_wallpapers.clear();
				SaveWallpaperList();
				RebuildOverlayBuffer();
				MakeOverlay(TRUE);
			}
		}
		else if(cmd==3)
		{
			m_wallpapers.erase(m_wallpapers.begin()+selIndex);
			SaveWallpaperList();
			RebuildOverlayBuffer();
			MakeOverlay(TRUE);
		}
	}


	return TRUE;
}

BOOL CFilmStripDesklet::InvokeAddImagesDialog()
{
	char buf[(MAX_PATH+2) * 100] = {0};

	OPENFILENAME ofn = {0};
	ofn.hwndOwner = GetWindow();
	ofn.hInstance = GetHInstance();

	return TRUE;
}