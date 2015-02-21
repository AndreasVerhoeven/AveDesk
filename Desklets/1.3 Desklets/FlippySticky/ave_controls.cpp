#define WINVER		0x0501
#define _WIN32_WINNT	0x0500
#include <windows.h>

#include <gdiplus.h>
using namespace Gdiplus;

#include <windowsx.h>
#include "osxpopupmenu.h"

#include <map>
#include <string>

class IAveControl;


class IAveControlContainer;
class IAveControl : public IAveVisibilityControl, public IAveLockable
{
protected:
	UINT layerId;
	HWND hwndDesklet;
	IControlContainer* container;

public:
	IAveControl() : layerId(static_cast<UINT>(-1)), hwndDesklet(NULL), container(NULL) {}
	IAveControl(UINT id, HWND hwnd, IAveControlContainer container  = NULL) : 
		layerId(id), hwndDesklet(hwnd), container(container) {}

	virtual ~IAveControl() = 0;

	virtual BOOL OnMouseDown(const POINT* pt, const SIZE* s, DWORD keys)  {return FALSE;};
	virtual BOOL OnMouseUp(const POINT* pt, const SIZE* s, DWORD keys)    {return FALSE;};
	virtual BOOL OnMouseClick(const POINT* pt, const SIZE* s, DWORD keys) {return FALSE;};
	
	virtual BOOL OnMouseOn(const POINT* pt, const SIZE*)    {return FALSE;};
	virtual BOOL OnMouseOut()   {return FALSE;};
	virtual BOOL OnMouseMove(const POINT* pt, const SIZE* )  {return FALSE;};

	virtual void Hide()
	{
		if(IsVisible())
			DeskletLayerSetVisibility(hwndDesklet, layerId, GetRedraw(TRUE), FALSE);
	}
	virtual void Show()
	{
		if(!IsVisible())
			DeskletLayerSetVisibility(hwndDesklet, layerId, GetRedraw(TRUE), TRUE);
	}
};

class IAveControlContainer2 : 
{
protected:
	std::list<std::string, IAveControl*> controls;
	HWND hwndDesklet;

public:
	~IAveControlContainer();

	virtual BOOL OnMouseDown()  {return FALSE;};
	virtual BOOL OnMouseUp()    {return FALSE;};
	virtual BOOL OnMouseClick() {return FALSE;};
	
	virtual BOOL OnMouseOn()    {return FALSE;};
	virtual BOOL OnMouseOut()   {return FALSE;};
	virtual BOOL OnMouseMove()  {return FALSE;};


	void Empty()
	{
		for(std::list<std::string, IAveControl*>::iterator iter = controls.begin(); iter != controls; ++iter)
		{
			delete *iter;
		}

		controls.clear();
	}
};

