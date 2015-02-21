#pragma once

#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT 0x0501
#define WINVER 0x0501
#define _WIN32_IE 0x0501
#pragma warning(disable:4503)
#pragma warning(disable:4786)

#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#include "skin.h"

#include <vector>
#include <string>
#include <memory>

#include "../DeskletImpl.hpp"

const timerId=494818;

class CFilmStripDesklet : 
	public CDeskletImpl<CFilmStripDesklet>
{
protected:
	std::vector<std::string> m_wallpapers;

	std::string m_skinDir;
	CSkin m_skin;
	
	std::auto_ptr<Bitmap> overlayBuffer;

	int  scrollingDelta;
	bool isScrollingLeft;
	int scrollingSpeed;

	void RebuildOverlayBuffer();
	void MakeOverlay(BOOL redraw);
	void MakeBackground(BOOL redraw);
	std::string GetAbsoluteSkinFolder();
	BOOL InvokeAddImagesDialog();

public:
	//USE_DEFAULT_DESKLET_CONSTRUCTOR;
	// uncomment the above line if don't want to provide code for the constructor.

	DEFAULT_DESKLET_CONSTRUCTOR_PROTOTYPE(CFilmStripDesklet);

	// implemented events handlers
	BOOL LeftButtonClick(POINT* p, SIZE* s);
	BOOL RightButtonClick(POINT* p, SIZE* s);

	// implemented drop handlers
	BOOL FilesDrop(std::vector<std::string>& files,DWORD /*keys*/,POINTL /*pnt*/, DWORD* /*effect*/);

	void SaveWallpaperList();
	void SaveWallpaperList(const char* iniFile,const char* iniSection);
	void ReadWallpaperList(const char* iniFile, const char* iniSection);
	void ReadFromIni(const char* iniFile, const char* iniSection);
	void SaveToIni(const char* iniFile, const char* iniSection);

	void SetSkin(std::string dir, BOOL noResize=FALSE);

	int GetNumberOfFramesThatCanBeVisible();

	void OnTimer(UINT msg, WPARAM w, LPARAM l);

	int GetWallpaperIndexFromPoint(POINT& rp);

	BOOL MouseOut();
	BOOL MouseMove(int x, int y);

	void SetWindowsWallpaper(int i);

	PROCESSMESSAGE_BEGIN()
		HANDLE_MESSAGE(WM_TIMER,OnTimer)
	PROCESSMESSAGE_END()
};

// this line is always required, otherwise the host application (AveDesk) won't reconize
// this as a valid desklet. Required information (in order): name, author, comments, version.
DESKLET_INFORMATION("Wallpaper FilmStrip","Andreas Verhoeven","This desklet let you change your wallpaper in style.",100)

// these events are always required. They make sure the desklet is created, destroyed and
// identified by the host application (AveDesk).
REQUIRED_EVENT_ONCREATE(CFilmStripDesklet)
REQUIRED_EVENT_ONDESTROY(CFilmStripDesklet)

// these events are optional, uncomment the ones you want to use and replace t with your class name.
// Also, you have to remove the the semicolon in front of the events-name in the .DEF file.
// If you want to respond to an event in your class, you should override the function tha
// matches it.

EVENT_ONLEFTBUTTONCLICK(CFilmStripDesklet)		// invokes LeftButtonClick
EVENT_ONRIGHTBUTTONCLICK(CFilmStripDesklet)		// invokes RightButtonClick
EVENT_ONMOUSEMOVE(CFilmStripDesklet)			// invokes MouseMove
//EVENT_ONMOUSEON(CFilmStripDesklet)				// invokes MouseOn
EVENT_ONMOUSEOUT(CFilmStripDesklet)				// invokes MouseOut
//EVENT_ONDRAGENTER(CFilmStripDesklet)			// invokes DragEnter
//EVENT_ONDRAGLEAVE(CFilmStripDesklet)			// invokes DragLeaver
EVENT_ONDROP(CFilmStripDesklet)					// invokes 3 drop handlers: FilesDrop(), StringDrop(), Drop()
//EVENT_ONDRAGOVER(CFilmStripDesklet)				// invokes DragOver
EVENT_ONSAVE(CFilmStripDesklet)					// invokes SaveToIni
EVENT_ONPROCESSMESSAGE(CFilmStripDesklet)		// invokes ProcessMessage
//EVENT_ONADDPROPERTYSHEETS(CFilmStripDesklet)	// invokes AddPropertySheets
//EVENT_ONDRAGDROPDATAREQUEST(CFilmStripDesklet)	// invokes DragDropDataRequest iff extended drag drop is enabled
//EVENT_ONSELECT(CFilmStripDesklet)				// invokes Select
//EVENT_ONDESELECT(CFilmStripDesklet)				// invokes Deselect
//EVENT_ONMOUSEWHEELFORWARD(CFilmStripDesklet)	// invokes MouseWheelForward
//EVENT_ONMOUSEWHEELBACKWARD(CFilmStripDesklet)	// invokes MouseWheelBackward