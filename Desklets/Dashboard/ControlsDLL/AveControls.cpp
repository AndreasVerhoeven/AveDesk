#include "AveControls.h"

#include "../DeskletSDK.h"

#include "../AveControls/avetextfield.h"
#include "../AveControls/avestatictext.h"
#include "../AveControls/aveverticalscrollbar.h"
#include "../AveControls/avedropdownlist.h"
#include "../AveControls/aveeditbox.h"
#include "../AveControls/aveflags.h"
#include "../AveControls/aveimagebutton.h"
#include "../AveControls/avelabel.h"
#include "../AveControls/avehyperlink.h"


BOOL __stdcall AveGetVersion(AVE_VERSION* version)
{
	if(NULL == version)
		return FALSE;

	version->major = 1;
	version->minor = 3;
	version->revision = 0;
	version->intendedAveDeskVersion = 130;

	return TRUE;
}


AVEHANDLE __stdcall AveCreateResources(const CHAR* baseFolder)
{
	if(NULL == baseFolder)
		return NULL;

	return (AVEHANDLE) new IAveResources(baseFolder);	
}

BOOL __stdcall AveFreeResources(AVEHANDLE handle)
{
	IAveResources* res = reinterpret_cast<IAveResources*>(handle);
	if(NULL == res)
		return FALSE;

	delete res;
	return TRUE;
}

AVEHANDLE __stdcall AveCreateResourcesW(const WCHAR* baseFolder)
{
	if(NULL == baseFolder)
		return NULL;

	return (AVEHANDLE) new IAveResources(baseFolder);
}

BOOL __stdcall AveSetResourcesPath(AVEHANDLE resourcesHandle, const CHAR* baseFolder)
{
	IAveResources* res = reinterpret_cast<IAveResources*>(resourcesHandle);
	if(NULL == res)
		return FALSE;

	res->SetPath(baseFolder);

	return TRUE;
}
BOOL __stdcall AveSetResourcesPathW(AVEHANDLE resourcesHandle, const WCHAR* baseFolder)
{
	IAveResources* res = reinterpret_cast<IAveResources*>(resourcesHandle);
	if(NULL == res)
		return FALSE;

	res->SetPathW(baseFolder);

	return TRUE;
}


AVEHANDLE __stdcall AveCreateControlContainer(HWND deskletHwnd, AVEHANDLE resources)
{
	if(NULL == deskletHwnd || NULL == resources)
		return NULL;

	IAveResources* res = reinterpret_cast<IAveResources*>(resources);
	if(NULL == res)
		return NULL;

	return (AVEHANDLE) new IAveControlContainer(deskletHwnd, res);

}

BOOL __stdcall AveFreeControlContainer(AVEHANDLE container)
{
	IAveControlContainer* c = reinterpret_cast<IAveControlContainer*>(container);
	if(NULL == c)
		return FALSE;

	delete c;
	return TRUE;
}

BOOL __stdcall AveAddControlToContainer(AVEHANDLE containerHandle, AVEHANDLE controlHandle)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	IAveControl* control = reinterpret_cast<IAveControl*>(controlHandle);

	if(NULL == container || NULL == control)
		return FALSE;

	
	container->Add(control);

	return TRUE;
}

BOOL __stdcall AveRemoveControlFromContainer(AVEHANDLE containerHandle, const CHAR* name)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(NULL == container || NULL == name)
		return FALSE;

	container->Remove(name);

	return TRUE;
}

AVEHANDLE __stdcall AveGetControlFromContainer(AVEHANDLE containerHandle, const CHAR* name)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(NULL == container || NULL == name)
		return NULL;

	return (AVEHANDLE)container->Get(name);
}

BOOL __stdcall AveEmptyContainer(AVEHANDLE containerHandle)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(NULL == container)
		return FALSE;

	container->Empty();
	return TRUE;
}

INT __stdcall AveLockContainer(AVEHANDLE containerHandle)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(NULL == container)
		return 0;

	return container->Lock();
}

INT __stdcall AveUnlockContainer(AVEHANDLE containerHandle)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(NULL == container)
		return 0;

	return container->Unlock();
}

BOOL __stdcall AveIsContainerLocked(AVEHANDLE containerHandle)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(NULL == container)
		return FALSE;

	return container->IsLocked();
}

BOOL __stdcall AveSetFocusToControl(AVEHANDLE containerHandle, const char* name)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(NULL == container)
		return FALSE;

	container->SetFocusToControl(name);
	return TRUE;
}




BOOL __stdcall AveForwardMessage(AVEHANDLE containerHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->Message(msg, wParam, lParam);
}


BOOL __stdcall AveForwardMouseWheelForward(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys, int scrolls)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseWheelForward(id, layerName, pt, s, keys, scrolls);
}

BOOL __stdcall AveForwardMouseWheelBackward(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys, int scrolls)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseWheelBackward(id, layerName, pt, s, keys, scrolls);
}


BOOL __stdcall AveForwardMouseDown(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;

	return container->MouseDown(id, layerName, pt, s, keys);
}

BOOL __stdcall AveForwardMouseUp(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseUp(id, layerName, pt, s, keys);
}

BOOL __stdcall AveForwardMouseClick(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseClick(id, layerName, pt, s, keys);
}

BOOL __stdcall AveForwardMouseRightClick(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseRightClick(id, layerName, pt, s, keys);
}

BOOL __stdcall AveForwardMouseOut(AVEHANDLE containerHandle, UINT id, const CHAR* layerName)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseOut(id, layerName);
}

BOOL __stdcall AveForwardMouseOn(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseOn(id, layerName, pt, s);
}

BOOL __stdcall AveForwardMouseMove(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s)
{
	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);
	if(NULL == container)
		return FALSE;	

	return container->MouseMove(id, layerName, pt, s);
}

AVEHANDLE __stdcall AveCreateControl(AVEHANDLE containerHandle, const CHAR* name, const CHAR* type, DWORD flags, DWORD_PTR* reserved)
{
	if(NULL == containerHandle || NULL == name || NULL == type)
		return NULL;

	IAveControlContainer* container = reinterpret_cast<IAveControlContainer*>(containerHandle);

	if(stricmp(type, "TEXTFIELD") == 0)
	{
		return (AVEHANDLE) new CAveTextField(container, name, flags & TEXTFIELD_NORETURNS);
	}
	else if(stricmp(type, "VERTICAL_SCROLLBAR") == 0)
	{
		return (AVEHANDLE) new CAveVerticalScrollBar(container, name);
	}
	else if(stricmp(type, "DROPDOWNLIST") == 0)
	{
		return (AVEHANDLE) new CAveDropDownList(container, name);
	}
	else if(stricmp(type, "EDITBOX") == 0)
	{
		return (AVEHANDLE) new CAveEditBox(container, name);
	}
	else if(stricmp(type, "IMAGEBUTTON") == 0)
	{
		return (AVEHANDLE) new CAveImageButton(container, name);
	}
	else if(stricmp(type, "LABEL") == 0)
	{
		return (AVEHANDLE) new CAveLabel(container, name);
	}
	else if(stricmp(type, "HYPERLINK") == 0)
	{
		return (AVEHANDLE) new CAveHyperLink(container, name);
	}
	

	return NULL;
}

BOOL __stdcall AveFreeControl(AVEHANDLE control)
{
	IAveControl* c = reinterpret_cast<IAveControl*>(control);
	if(NULL == c)
		return FALSE;

	delete c;

	return TRUE;
}

INT __stdcall AveLockControl(AVEHANDLE controlHandle)
{
	IAveControl* control = reinterpret_cast<IAveControl*>(controlHandle);

	if(NULL == control)
		return 0;

	return control->Lock();
}

INT __stdcall AveUnlockControl(AVEHANDLE controlHandle)
{
	IAveControl* control = reinterpret_cast<IAveControl*>(controlHandle);

	if(NULL == control)
		return 0;

	return control->Unlock();
}

BOOL __stdcall AveIsControlLocked(AVEHANDLE controlHandle)
{
	IAveControl* control = reinterpret_cast<IAveControl*>(controlHandle);

	if(NULL == control)
		return FALSE;

	return control->IsLocked();
}


DWORD_PTR __stdcall AveSendMessage(AVEHANDLE controlHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	IAveControl* control = reinterpret_cast<IAveControl*>(controlHandle);

	if(NULL == control)
		return FALSE;

	BOOL handled = FALSE;
	DWORD res  = control->InternalMessage(msg, wParam, lParam, handled);

	return res;
}

/* example

OnCreate(...)
{
	// initialize our resources
	char resPath[MAX_PATH] = {0};
	DeskletGetRelativeFolder(hwndDesklet, resPath);
	strcpy(resPath, "resources//");
	AVEHANDLE resources = AveCreateResources(resPath);

	// create a container that hold or controls
	AVEHANDLE container = AveCreateControlContainer(hwndDesklet, resources);

	// create an textfield control
	AVEHANDLE textField = AveCreateControl(container, "textfield_A", "TEXTFIELD", 0, NULL);
	AveLockControl(textField); // lock redrawing when we are setting multiple properties
	RECT r = {0, 0, 100, 100};
	SCALE s = {200, 200};
	AveSendMessage(textField, AV_SETRECT, (WPARAM)&r, (LPARAM)&s);
	AveSendMessage(textField, AV_SET_TEXT, 0L, (LPARAM)L"a sample text"); // wide string - just for the records
	AveAddControlToContainer(container, textField);
	AveUnlockControl(textField);

}

OnProcessMessage(UINT msg, WPARAM w, LPARAM l)
{
	BOOL handled = AveForwardMessage(container, msg, w, l);
	if(!handled)
	{
		// see what we can do with it ourself
	}
}

OnLayerMouseDown(UINT lid, const char* name, ....)
{
	BOOL handled = AveForwardMouseDown(container, id, name, ....);
	if(!handled)
	{
		// might be one of our own layers we want to handle
	}
}


OnDestroy(...)
{
	// free up - the controls are freed by the container
	AveFreeControlContainer(container);
	AveFreeResources(resources);
}


*/