#include "ExampleDesklet.h"

CExampleDesklet::DEFAULT_DESKLET_CONSTRUCTOR_BODY(CExampleDesklet)
{
	// parameters: HWND hwnd, HINSTANCE hInstance (not generally needed) and bNotLoaded.
	// bNotLoaded is true iff the desklet is created for the first time.

	// we want to be able to receive drops.
	ActivateExtendedDragDrop();

	// set a default image if this is the first time this desklet is loaded
	if(bNotLoaded)
		SetImageFile(GetRelativeFolder() + "\\Impl_test.png");
}

BOOL CExampleDesklet::LeftButtonClick(POINT* p, SIZE* s)
{
	// we just show a simple messagebox on left click.
	MessageBox(GetWindow(),"Test Message","Blah Blah",MB_ICONINFORMATION);

	return TRUE;
}

BOOL CExampleDesklet::RightButtonClick(POINT* p, SIZE* s)
{
	// we show the default right click menu from the host application (AveDesk)
	// and add one "reset image" entry to it.

	// get the default menu. It's a good idea to pass an startId that's higher than
	// the number of entries you add, so feature versions, with a possibility different
	// menu, won't break this functionality.
	HMENU menu = GetDefaultMenu(100);
	AppendMenu(menu,MF_STRING | MF_ENABLED,1,"Reset Image");

	// show the menu at the current cursor position.
	POINT cursorPos = {0};
	GetCursorPos(&cursorPos);
	int id = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, cursorPos.x, cursorPos.y, 0, GetWindow(),0);

	// handle the action. If the entry-id is bigger or equal to 100, than the user invoked an menu-item
	// from the avedesk menu, so we let avedesk handle it. Otherwise, if it's our own added entry, we 
	// simply reset the icon of the desklet to the default image.
	if(id >= 100)
		DoDefaultMenuCommand(100,id);
	else if(id == 1)
	{
		SetImageFile(GetRelativeFolder() + "\\Impl_test.png");
	}

	return TRUE;
}

BOOL CExampleDesklet::FilesDrop(std::vector<std::string>& files,DWORD /*keys*/,POINTL /*pnt*/, DWORD* /*effect*/)
{
	// enumurate thru all dropped files.
	for(std::vector<std::string>::iterator iter = files.begin(); iter != files.end(); ++ iter)
		MessageBox(GetWindow(),iter->c_str(),"Files Dropped...",MB_ICONINFORMATION);

	return TRUE;
}

BOOL CExampleDesklet::StringDrop(std::string& s,DWORD /*keys*/,POINTL /*pnt*/, DWORD* /*effect*/)
{
	// show the dropped string in a messagebox.
	MessageBox(GetWindow(),s.c_str(),"String Dropped...",MB_ICONINFORMATION);

	return TRUE;
}
