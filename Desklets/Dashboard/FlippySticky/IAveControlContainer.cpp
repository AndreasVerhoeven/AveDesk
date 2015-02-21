// IAveControlContainer.cpp: implementation of the IAveControlContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "IAveControlContainer.h"
#include "IAveControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IAveControlContainer::IAveControlContainer(HWND hwndDesklet, IAveResources* resources) :
	hwndDesklet(hwndDesklet),
	resources(resources)
{

}

IAveControlContainer::~IAveControlContainer()
{
	Empty();
}

HWND IAveControlContainer::GetDeskletHandle()const
{
	return hwndDesklet;
}

IAveResources* IAveControlContainer::GetResources()
{
	return resources;
}

BOOL IAveControlContainer::Add(IAveControl* ctrl)
{
	if(NULL == ctrl)
		return FALSE;

	std::map<std::string, IAveControl*>::iterator iter = controls.find(ctrl->GetName());
	if(iter != controls.end())
		return FALSE;

	controls[ctrl->GetName()] = ctrl;

	return TRUE;
}

IAveControl* IAveControlContainer::Remove(std::string name)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(name);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		controls.erase(ctrl->GetName());

		return ctrl;
	}

	return NULL;
}

IAveControl* IAveControlContainer::Get(std::string name)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(name);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		return ctrl;
	}

	return NULL;
}

void IAveControlContainer::Empty()
{
	for(std::map<std::string, IAveControl*>::iterator iter = controls.begin();
		iter != controls.end();
		++iter)
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
		{
			UINT id = DeskletLayerGetLayer(GetDeskletHandle(), iter->first.c_str(), NULL);
			DeskletLayerRemove(GetDeskletHandle(), id, FALSE);
			delete ctrl;
		}
	}

	controls.clear();

	if(GetRedraw(TRUE))
		DeskletRedraw(hwndDesklet);
}


BOOL IAveControlContainer::Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(WM_KILLFOCUS == msg && focusLayer.length() > 0)
	{
		std::map<std::string, IAveControl*>::iterator iter = controls.find(focusLayer);
		if(iter != controls.end())
		{
			IAveControl* ctrl = iter->second;
			if(ctrl != NULL)
				ctrl->KillFocus(NULL);
		}

		focusLayer = "";
	}

	for(std::map<std::string, IAveControl*>::iterator iter = controls.begin();
		iter != controls.end();
		++iter)
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
		{
			if(ctrl->Message(msg, wParam, lParam))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL IAveControlContainer::MouseDown(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	BOOL handled = FALSE;
	std::string oldFocus = focusLayer;

	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
		{
			if(focusLayer != layerName)
			{
				if(oldFocus != layerName)
				{
				std::map<std::string, IAveControl*>::iterator iter = controls.find(oldFocus);
				if(iter != controls.end())
				{
					IAveControl* ctrl = iter->second;
					if(ctrl != NULL)
						ctrl->KillFocus(focusLayer.c_str());
					}
				}

				ctrl->SetFocus();
				focusLayer = layerName;
			}

			handled = ctrl->MouseDown(pt, s, keys);
		}
	}
	else
	{
		focusLayer = layerName;

		if(oldFocus != focusLayer)
		{
			std::map<std::string, IAveControl*>::iterator iter = controls.find(oldFocus);
			if(iter != controls.end())
			{
				IAveControl* ctrl = iter->second;
				if(ctrl != NULL)
					ctrl->KillFocus(focusLayer.c_str());
			}
		}
	}

	return handled;
}

BOOL IAveControlContainer::MouseUp(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
			return ctrl->MouseUp(pt, s, keys);
	}
	return FALSE;
}

BOOL IAveControlContainer::MouseWheelForward(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
			return ctrl->MouseWheelForward(pt, s, keys, numOfScrolls);
	}
	return FALSE;
}

BOOL IAveControlContainer::MouseWheelBackward(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
			return ctrl->MouseWheelBackward(pt, s, keys, numOfScrolls);
	}
	return FALSE;
}

BOOL IAveControlContainer::MouseClick(UINT id, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
			return ctrl->MouseClick(pt, s, keys);
	}
	return FALSE;
}

BOOL IAveControlContainer::MouseOn(UINT id, const char* layerName, const POINT* pt, const SIZE* s)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
			return ctrl->MouseOn(pt, s);
	}
	return FALSE;
}
BOOL IAveControlContainer::MouseMove(UINT id, const char* layerName, const POINT* pt, const SIZE* s)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
			return ctrl->MouseMove(pt, s);
	}
	return FALSE;
}

BOOL IAveControlContainer::MouseOut(UINT id, const char* layerName)
{
	std::map<std::string, IAveControl*>::iterator iter = controls.find(layerName);
	if(iter != controls.end())
	{
		IAveControl* ctrl = iter->second;
		if(ctrl != NULL)
			return ctrl->MouseOut();
	}
	return FALSE;
}
