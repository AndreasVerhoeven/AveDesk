#include "../DeskletSDK.h"

#include "../AveControls/avebutton.h"
#include "../AveControls/avetextfield.h"
#include "../AveControls/avestatictext.h"
#include "../AveControls/aveverticalscrollbar.h"
#include "../AveControls/avedropdownlist.h"
#include "../AveControls/AveEditBox.h"

#include <cstring>
#include<string>
#include <map>
#include <list>
#include <algorithm>


SIZE collapsedLeft = {198, 90};
SIZE collapsedRight = {297, 90};
SIZE collapsedMid = {153, 90};

RECT searchBoxRectRight = {118, 31, 243, 51};

RECT MultiplyRect(const RECT& r, int fac)
{
	RECT ret = r;
	ret.left *= fac;
	ret.right *= fac;
	ret.top *= fac;
	ret.bottom *= fac;

	return ret;
}

SIZE MultiplySize(const SIZE& s, int fac)
{
	SIZE ret = s;
	ret.cx *= fac;
	ret.cy *= fac;

	return ret;
}


class Teletext
{
public:
	HWND hwnd;
	HINSTANCE dll;

	IAveResources* resources;
	IAveControlContainer* container;

	CAveEditBox* editBox;

	enum Modes {COLLAPSED_FULL = 0, COLLAPSED_MINI = 1};
	Modes mode;

	Teletext(HWND hwnd, HINSTANCE dll) : hwnd(hwnd), dll(dll),
		mode(COLLAPSED_MINI)
	{
		ClearAllLayers();
		SetupBackgroundLayers();
		SetupControls();
	}


	~Teletext()
	{
		container->Empty();
		delete container;
		delete resources;
	}

	void ClearAllLayers()
	{
		while(DeskletLayerCount(hwnd) > 0)
			DeskletLayerRemove(hwnd, 0, FALSE);
	}

	void SetLayerImage(const char* name, const char* file)
	{
		char relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(hwnd, relativePath);

		char img[MAX_PATH] = {0};
		strcpy(img, relativePath);
		strcat(img, "images\\");
		strcat(img, file);
		strcat(img, ".png");

		BOOL success = FALSE;
		UINT lid = DeskletLayerGetLayer(hwnd, name, &success);
		if(success)
			DeskletLayerSetImageFile(hwnd, lid, FALSE, img);	
	}

	UINT AddLayer(const char* name)
	{
		char relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(hwnd, relativePath);

		char bgPath[MAX_PATH] = {0};
		strcpy(bgPath, relativePath);
		strcat(bgPath, "images\\");
		strcat(bgPath, name);
		strcat(bgPath, ".png");

		UINT bgLayer = DeskletLayerAdd(hwnd, name);
		DeskletLayerSetImageFile(hwnd, bgLayer, FALSE, bgPath);

		return bgLayer;
	}

	void SetupBackgroundLayers()
	{
		DeskletLockInvalidating(hwnd);

		SetupCollapsedLayers();

		DeskletUnlockInvalidating(hwnd, FALSE);
	}

	void SetupControls()
	{
		char resPath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(hwnd, resPath);
		strcat(resPath, "images\\controls\\");

		resources = new IAveResources(resPath);
		container = new IAveControlContainer(hwnd, resources);

		container->Lock();


		
		editBox = new CAveEditBox(container, "test_edit");
		editBox->Lock();

		RECT r2 = MultiplyRect(searchBoxRectRight,2);
		SIZE s2 = MultiplySize(collapsedRight,2);
		editBox->SetRect(&r2, &s2);
		editBox->Unlock();
		container->Add(editBox);
		editBox->Update();
		

		container->Unlock();
	}


	void SetupCollapsedLayers()
	{
		UINT collapsed_left = AddLayer("collapsed_left");
		DeskletLayerSetVisibility(hwnd, collapsed_left, FALSE, FALSE);

		UINT collapsed_right = AddLayer("collapsed_right");
		DeskletLayerSetVisibility(hwnd, collapsed_right, FALSE, FALSE);
	}

	void SetCollapsedMiniMode(BOOL doRedraw)
	{
		UINT collapsed_left  = DeskletLayerGetLayer(hwnd,"collapsed_left", NULL);
		UINT collapsed_right = DeskletLayerGetLayer(hwnd,"collapsed_right", NULL);

		DeskletLockInvalidating(hwnd);

		if(mode != COLLAPSED_MINI || mode != COLLAPSED_FULL)
		{
			DeskletLayerSetVisibility(hwnd, collapsed_left, FALSE, TRUE);
			DeskletLayerSetVisibility(hwnd, collapsed_right, FALSE, TRUE);
		}

		SIZE newSize = { collapsedRight.cx + (collapsedLeft.cx - collapsedMid.cx), collapsedRight.cy };

		editBox->Lock();

		RECT newEditBoxRect = searchBoxRectRight;
		OffsetRect(&newEditBoxRect, collapsedLeft.cx - collapsedMid.cx, 0);
		RECT r2 = MultiplyRect(newEditBoxRect,2);
		SIZE s2= MultiplySize(newSize, 2);		
		editBox->SetRect(&r2, &s2);

		editBox->Unlock();

		RECT rightRect = {newSize.cx - collapsedRight.cx, 0, newSize.cx, newSize.cy };
		DeskletLayerSetPlacement(hwnd, collapsed_right, FALSE, TRUE, &rightRect, newSize.cx, newSize.cy);

		RECT leftRect = {0, 0, collapsedLeft.cx - collapsedMid.cx, newSize.cy};
		DeskletLayerSetPlacement(hwnd, collapsed_left, FALSE, TRUE, &leftRect, newSize.cx, newSize.cy);

		RECT leftBounds = {0, 0, collapsedLeft.cx - collapsedMid.cx, collapsedLeft.cy};
		DeskletLayerSetImageSourceBounds(hwnd, collapsed_left, FALSE, TRUE, &leftBounds, collapsedLeft.cx, collapsedLeft.cy);

		POINT curPos = {0,0};
		DeskletGetPosition(hwnd, &curPos);
		RECT newRect = {curPos.x, curPos.y, curPos.x + newSize.cx, curPos.y + newSize.cy};
		if(mode == COLLAPSED_FULL)
		{
			newRect.left += collapsedMid.cx;
			newRect.right += collapsedMid.cx;
		}
		DeskletMove(hwnd, &newRect, FALSE);
		

		DeskletUnlockInvalidating(hwnd,doRedraw);

		mode = COLLAPSED_MINI;
	}

	int animStep, prevStep;


	void AnimToMini()
	{
		animStep = collapsedMid.cx;
		prevStep = animStep;

		int speed = 8;
		if(GetKeyState(VK_SHIFT) < 0)
			speed = 1;


		while(animStep > 0)
		{
			AnimToFullStep(animStep, prevStep, TRUE);
			prevStep = animStep;
			animStep -= speed;
		}

		// we always repaint totally because of a resize, so no final check needed.
		SetCollapsedMiniMode(TRUE);
	}

	void AnimToFull()
	{
		animStep = 0;
		prevStep = animStep;

		int speed = 8;
		if(GetKeyState(VK_SHIFT) < 0)
			speed = 1;

		SetCollapsedFullMode(FALSE);

		while(animStep < collapsedMid.cx)
		{
			AnimToFullStep(animStep, prevStep, TRUE);
			prevStep = animStep;
			animStep += speed;
		}

		if(animStep != collapsedMid.cx)
			AnimToFullStep(collapsedMid.cx, prevStep, TRUE);

	}

	void AnimToFullStep(int step, int prevStep, BOOL doRedraw)
	{
		UINT collapsed_left  = DeskletLayerGetLayer(hwnd,"collapsed_left", NULL);
		UINT collapsed_right = DeskletLayerGetLayer(hwnd,"collapsed_right", NULL);

		SIZE newSize = { collapsedRight.cx + collapsedLeft.cx, collapsedRight.cy };

		int oldOffset = collapsedMid.cx - prevStep;
		int oldInvalidateWidth = collapsedLeft.cx - collapsedMid.cx;
		RECT oldnvalidateRect = {oldOffset, 0, oldOffset + oldInvalidateWidth, newSize.cy};
		DeskletInvalidateRect(hwnd, &oldnvalidateRect, &newSize, FALSE);

		DeskletLockInvalidating(hwnd);

		if(mode != COLLAPSED_MINI || mode != COLLAPSED_FULL)
		{
			DeskletLayerSetVisibility(hwnd, collapsed_left, FALSE, TRUE);
			DeskletLayerSetVisibility(hwnd, collapsed_right, FALSE, TRUE);
		}

		int offset = collapsedMid.cx - step;

		RECT leftRect = {offset, 0, collapsedLeft.cx , newSize.cy};
		DeskletLayerSetPlacement(hwnd, collapsed_left, FALSE, TRUE, &leftRect, newSize.cx, newSize.cy);

		RECT leftBounds = {0, 0, collapsedLeft.cx - offset, collapsedLeft.cy};
		DeskletLayerSetImageSourceBounds(hwnd, collapsed_left, FALSE, TRUE, &leftBounds, collapsedLeft.cx, collapsedLeft.cy);	

		DeskletUnlockInvalidating(hwnd,FALSE);

		int invalidateWidth = collapsedLeft.cx - collapsedMid.cx;
		RECT invalidateRect = {offset, 0, offset + invalidateWidth, newSize.cy};
		DeskletInvalidateRect(hwnd, &invalidateRect, &newSize, doRedraw);

		mode = COLLAPSED_FULL;		
	}

	void SetCollapsedFullMode(BOOL doRedraw)
	{
		UINT collapsed_left  = DeskletLayerGetLayer(hwnd,"collapsed_left", NULL);
		UINT collapsed_right = DeskletLayerGetLayer(hwnd,"collapsed_right", NULL);

		DeskletLockInvalidating(hwnd);

		if(mode != COLLAPSED_MINI || mode != COLLAPSED_FULL)
		{
			DeskletLayerSetVisibility(hwnd, collapsed_left, FALSE, TRUE);
			DeskletLayerSetVisibility(hwnd, collapsed_right, FALSE, TRUE);
		}

		SIZE newSize = { collapsedRight.cx + collapsedLeft.cx, collapsedRight.cy };

		editBox->Lock();

		RECT newEditBoxRect = searchBoxRectRight;
		OffsetRect(&newEditBoxRect, collapsedLeft.cx, 0);
		RECT r2 = MultiplyRect(newEditBoxRect,2);
		SIZE s2= MultiplySize(newSize, 2);		
		editBox->SetRect(&r2, &s2);

		editBox->Unlock();


		RECT rightRect = {newSize.cx - collapsedRight.cx, 0, newSize.cx, newSize.cy };
		DeskletLayerSetPlacement(hwnd, collapsed_right, FALSE, TRUE, &rightRect, newSize.cx, newSize.cy);

		RECT leftRect = {0, 0, collapsedLeft.cx , newSize.cy};
		DeskletLayerSetPlacement(hwnd, collapsed_left, FALSE, TRUE, &leftRect, newSize.cx, newSize.cy);

		RECT leftBounds = {0, 0, collapsedLeft.cx, collapsedLeft.cy};
		DeskletLayerSetImageSourceBounds(hwnd, collapsed_left, FALSE, TRUE, &leftBounds, collapsedLeft.cx, collapsedLeft.cy);

		POINT curPos = {0,0};
		DeskletGetPosition(hwnd, &curPos);
		RECT newRect = {curPos.x, curPos.y, curPos.x + newSize.cx, curPos.y + newSize.cy};
		if(mode == COLLAPSED_MINI)
		{
			newRect.left -= collapsedMid.cx;
			newRect.right -= collapsedMid.cx;
		}
		DeskletMove(hwnd, &newRect, FALSE);
		

		DeskletUnlockInvalidating(hwnd,doRedraw);

		mode = COLLAPSED_FULL;
	}

		/*

	/*
	void Collapse()
	{
		SIZE s = {528, 91 + 254 + 121};
		DeskletSetSize(hwnd, &s);

		DeskletLockInvalidating(hwnd);

		DeskletLayerSetVisibility(hwnd, DeskletLayerGetLayer(hwnd, "bg", NULL), FALSE, FALSE);

		int bottom = DeskletLayerGetLayer(hwnd, "bottom", NULL);
		int top = DeskletLayerGetLayer(hwnd, "top", NULL);
		int mid = DeskletLayerGetLayer(hwnd, "middle", NULL);

		DeskletLayerSetVisibility(hwnd, bottom, FALSE, TRUE);
		DeskletLayerSetVisibility(hwnd, top, FALSE, TRUE);
		DeskletLayerSetVisibility(hwnd, mid, FALSE, TRUE);

		RECT rTop = {0,0, 528, 121};
		DeskletLayerSetPlacement(hwnd, top, FALSE, TRUE, &rTop, scale.cx, scale.cy);

		RECT rMid = {0, rTop.bottom, 528, rTop.bottom + 0};
		RECT rBottom = {0,rMid.bottom, 528, rMid.bottom + 91};

		DeskletLayerSetPlacement(hwnd, bottom, FALSE, TRUE, &rBottom, scale.cx, scale.cy);
		DeskletLayerSetPlacement(hwnd, mid, FALSE, TRUE, &rMid, scale.cx, scale.cy);


		DeskletUnlockInvalidating(hwnd, FALSE);

		DeskletRedraw(hwnd);

		int diff = 18;
		int midHeight = 254;
		for(int i=0; i < midHeight; i += diff)
		{
			RECT rMid = {0, rTop.bottom, 528, rTop.bottom + i};
			RECT rBottom = {0,rMid.bottom, 528, rMid.bottom + 91};

			DeskletLockInvalidating(hwnd);

			DeskletLayerSetPlacement(hwnd, bottom, FALSE, TRUE, &rBottom, scale.cx, scale.cy);
			DeskletLayerSetPlacement(hwnd, mid, FALSE, TRUE, &rMid, scale.cx, scale.cy);

			RECT bounds = {0, 0, 528, i};
			SIZE midScale = {528, 254};
			DeskletLayerSetImageSourceBounds(hwnd, mid, FALSE, TRUE, &bounds, midScale.cx, midScale.cy);

			DeskletUnlockInvalidating(hwnd, FALSE);

			RECT invalRect = {0, rMid.bottom-diff, 528, rBottom.bottom};
			SIZE scale = {528, 91 + 254 + 121};
			DeskletInvalidateRect(hwnd, &invalRect, &scale, TRUE);
			SleepEx(10, FALSE);
		}

	}*/
};



void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"Teletext");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Translates");
	*iVersion = 130;
}


Teletext* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, const char* ini, const char* group)
{
	Teletext* t = new Teletext(hwndDesklet, hInstanceDll);
	if(!ini || !group)
	{
		DeskletSetLabel(hwndDesklet, "");
		DeskletSetSubLabel(hwndDesklet, "");
		//DeskletSetSize(hwndDesklet, &scale);
	}
	else
	{

	}

	t->SetCollapsedMiniMode(FALSE);
	DeskletRedraw(hwndDesklet);

	return t;
}


void __stdcall OnDestroy(Teletext* t, HWND hwndDesklet)
{
	delete t;
}


BOOL __stdcall OnLayerClick(Teletext* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(t->container->MouseClick(layerId, layerName, pt, s, keys))
		return TRUE;

	if(t->mode == Teletext::Modes::COLLAPSED_FULL)
		t->AnimToMini();
	else
		t->AnimToFull();

	return FALSE;
}

BOOL __stdcall OnLayerMouseDown(Teletext* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(t->container->MouseDown(layerId, layerName, pt, s, keys))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelForward(Teletext* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(t->container->MouseWheelForward(layerId, layerName, pt, s, keys, numOfScrolls))
		return FALSE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelBackward(Teletext* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(t->container->MouseWheelBackward(layerId, layerName, pt, s, keys, numOfScrolls))
		return FALSE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseUp(Teletext* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(t->container->MouseUp(layerId, layerName, pt, s, keys))
		return FALSE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseOn(Teletext* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(t->container->MouseOn(layerId, layerName, pt, s))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseMove(Teletext* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(t->container->MouseMove(layerId, layerName, pt, s))
		return TRUE;

	return FALSE;
}


BOOL __stdcall OnLayerMouseOut(Teletext* t, UINT layerId, const char* layerName)
{
	if(t->container->MouseOut(layerId, layerName))
		return TRUE;

	return FALSE;
}


void __stdcall OnSave(Teletext* t, const char* ini, const char* group, BOOL bIsForExport)
{

}

void __stdcall OnProcessMessage(Teletext* t, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!t)
		return;

	if(t->container != NULL)
	{
		if(t->container->Message(msg, wParam, lParam))
			return;
	}
}

BOOL __stdcall OnMouseMove(Teletext* t, int x, int y)
{
	//if(!t->flipVisible && !t->isInBackMode)
	//{
	//	t->flipVisible = TRUE;
	//	DeskletLayerSetVisibility(t->hwnd, DeskletLayerGetLayer(t->hwnd, "flip", NULL), TRUE, TRUE);
	//}

	return TRUE;
}

BOOL __stdcall OnMouseOut(Teletext* t)
{
	//if(t->flipVisible)
	//{
	//	t->flipVisible = FALSE;
	//	DeskletLayerSetVisibility(t->hwnd, DeskletLayerGetLayer(t->hwnd, "flip", NULL), TRUE, FALSE);
	//}

	return TRUE;
}