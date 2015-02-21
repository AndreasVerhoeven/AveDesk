// IAveControlContainer.h: interface for the IAveControlContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVECONTROLCONTAINER_H__94EB2DEB_EBC8_4AEF_BA88_96E7E08AE668__INCLUDED_)
#define AFX_IAVECONTROLCONTAINER_H__94EB2DEB_EBC8_4AEF_BA88_96E7E08AE668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveLockable.h"
#include "IAveResources.h"
#include <map>
#include <string>

#include "controls_header.h"

class IAveControl;

class IAveControlContainer : public IAveLockableImpl
{
protected:
	std::map<std::string, IAveControl*> controls;
	IAveResources* resources;
	HWND hwndDesklet;
	std::string focusLayer;

public:
	IAveControlContainer(HWND hwndDesklet, IAveResources* resources);
	virtual ~IAveControlContainer();

	virtual HWND GetDeskletHandle()const;
	virtual void Empty();
	virtual BOOL Add(IAveControl* ctrl);
	virtual IAveControl* Remove(std::string name);
	virtual IAveControl* Get(std::string name);

	virtual void SetFocusToControl(const char* layerName);

	virtual BOOL MouseDown(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseUp(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseClick(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseRightClick(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys);

	virtual BOOL MouseOn(UINT id, const char* layerName, const POINT* pt, const SIZE* s);
	virtual BOOL MouseMove(UINT id, const char* layerName, const POINT* pt, const SIZE* s);
	virtual BOOL MouseOut(UINT id, const char* layerName);

	virtual BOOL MouseWheelForward(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	virtual BOOL MouseWheelBackward(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);

	virtual BOOL Message(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual IAveResources* GetResources();

};

#endif // !defined(AFX_IAVECONTROLCONTAINER_H__94EB2DEB_EBC8_4AEF_BA88_96E7E08AE668__INCLUDED_)
