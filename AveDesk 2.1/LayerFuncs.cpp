// filename:		LayerFuncs.cpp
// Description:		Implementation of all DeskletLayer* functions
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "docklet.h"
#include <vector>
#include "Layer.h"
#include "ContainerLayer.h"
#include <algorithm>
#include "callback_funcs.h"
#include "helper_funcs.h"

#define SUBLAYER_MASK 0x10000

inline BOOL IsIDSubLayer(UINT id)
{
	return (id & SUBLAYER_MASK) != 0;
}

Layer* GetLayerFromID(Docklet* d, UINT id)
{
	if(NULL == d)
		return NULL;

	if(IsIDSubLayer(id))
	{
		return d->GetDocklet()->GetSubLayers()[id];
	}
	else
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		if(id < layers.size())
		{
			Layer*& l = layers[id];
			return l;
		}
	}

	return NULL;
}


// helper function
RECT GetLayerRectangle(Layer* l, Docklet* d)
{
	RECT r = {0};
	if(NULL == l || d == NULL)
		return r;

	SIZE deskletSize = d->GetDocklet()->GetSize();

	if(l->GetUsePlacement())
	{
		SIZE scale = l->GetPlacementScale();
		RectF rf = RemapRectF(RECTTORECTF(l->GetPlacementRect()), deskletSize, static_cast<float>(scale.cx), static_cast<float>(scale.cy));
		
		r.left   = (LONG)rf.X;
		r.top    = (LONG)rf.Y;
		r.right  = r.left + round_float_greater(rf.Width);
		r.bottom = r.top + round_float_greater(rf.Height);
	}
	else
	{
		// no placement, so the layer spans the whole desklet
		RECT r2 = {0, 0, deskletSize.cx, deskletSize.cy};
		r = r2;
	}

	if(l->GetRotation() % 360 != 0)
	{
		//if(l->GetUsePlacement())
		//{
			SIZE s = SIZEFROMRECT(r);
			POINT p = {r.left, r.top};
			FixRotation(-l->GetRotation() % 360, s, p);
			r.left -= p.x ;
			r.top -= p.y;
			r.right = r.left + s.cx;
			r.bottom = r.top + s.cy;

			RECT r2 = {0, 0, deskletSize.cx, deskletSize.cy};
			r = r2;
		//}
	}

	return r;
}

void InvalidateLayerRect(Docklet* d, Layer* layer, BOOL ignoreVisibility)
{
	RECT rc = GetLayerRectangle(layer, d);
	d->GetDocklet()->AddUpdateRect(rc, d->GetDocklet()->GetSize());

/*	Layer* l = layer;
	ContainerLayer* parent = NULL;

	SIZE deskletSize = d->GetDocklet()->GetSize();

	RECT rc = {0,0, 100, 100};
	SIZE scale = {100,100};
	if(l->GetUsePlacement())
	{
		rc = l->GetPlacementRect();
		scale = l->GetPlacementScale();
	}
	while(l != NULL && l->GetParent() != NULL)
	{
		if(l->GetRotation() % 360 != 0)
		{
			SIZE s = SIZEFROMRECT(rc);
			POINT p = {rc.left, rc.top};
			FixRotation(-l->GetRotation() % 360, s, p);
			rc.left -= p.x ;
			rc.top -= p.y;
			rc.right = rc.left + s.cx;
			rc.bottom = rc.top + s.cy;

			//RECT r2 = {0, 0, deskletSize.cx, deskletSize.cy};
			//rc = r2;
			//scale = deskletSize;
		}

		if(!(ignoreVisibility || l->GetVisibility()))
			return;

		parent = (ContainerLayer*)l->GetParent();

		// convert to procent
		float fX = float(rc.left) / float(scale.cx) * 100;
		float fY = float(rc.top) / float(scale.cy) * 100;

		parent->AddUpdateRect(rc, scale);


		SIZE newScale = {100,100};
		RECT parentRect = {0,0, 100,100};
		if(parent->GetUsePlacement())
		{
			parent->GetPlacementScale();
			parentRect = parent->GetPlacementRect();
		}
		RectF rf = RemapRectF(RECTTORECTF(rc), newScale, static_cast<float>(scale.cx), static_cast<float>(scale.cy));
		rc.left = floor(rf.X) + parentRect.left;
		rc.top  = floor(rf.Y) + parentRect.top;
		rc.right = rc.left + round_float_greater(rf.Width);
		rc.bottom = rc.top + round_float_greater(rf.Height);
		scale = newScale;
		l = parent;
	}

	if(l != NULL)
	{
		if((ignoreVisibility || (l->GetVisibility() && l->CanBeDrawn())))
		{
			SIZE newScale = d->GetDocklet()->GetSize();
			RECT parentRect = {0,0, newScale.cx, newScale.cy};
			RectF rf = RemapRectF(RECTTORECTF(rc), newScale, static_cast<float>(scale.cx), static_cast<float>(scale.cy));
			rc.left   = floor(rf.X);
			rc.top    = floor(rf.Y);
			rc.right  = rc.left + ceil(rf.Width);
			rc.bottom = rc.top  + ceil(rf.Height);

			if(l->GetRotation() % 360 != 0)
			{
				SIZE s = SIZEFROMRECT(rc);
				POINT p = {rc.left, rc.top};
				FixRotation(-l->GetRotation() % 360, s, p);
				rc.left -= p.x ;
				rc.top -= p.y;
				rc.right = rc.left + s.cx;
				rc.bottom = rc.top + s.cy;

				//RECT r2 = {0, 0, deskletSize.cx, deskletSize.cy};
				//rc = r2;
				//scale = deskletSize;
			}

			d->GetDocklet()->AddUpdateRect(rc, newScale);
		}
	}*/
};



/**
 * Captures the mouse to a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer that gets the capture. (UINT)-1 means no layer.
 * @remark 1.3 function uses (UINT)-1 for no-layer, rather than the ambgious 0.
 * @return returns the layer's id that HAD the capture.
 * @remark v2 ready
 */
UINT CALLBACK DeskletLayerSetCapture(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		if(id != (UINT)-1)
			::SetCapture(hwndDesklet);
		else
			::ReleaseCapture();

		UINT oldCapture = d->GetDocklet()->GetCaptureLayer();
		d->GetDocklet()->SetCaptureLayer(id);
		return oldCapture;
	}
	else
	{
		return (UINT)-1;
	}
}

/**
 * Releases the mouse-capture.
 * @param hwndDesklet the handle of the desklet to release the mousecapture for.
 * @remark 1.3 function uses (UINT)-1 for no-layer, rather than the ambgious 0.
 * @return returns the layer's id that HAD the capture.
 * @remark v2 ready
 */
UINT CALLBACK DeskletLayerReleaseCapture(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		::ReleaseCapture();

		UINT oldCapture = d->GetDocklet()->GetCaptureLayer();
		d->GetDocklet()->SetCaptureLayer((UINT)-1);
		return oldCapture;
	}
	else
	{
		return (UINT)-1;
	}
}

/**
 * Gets the id of the layer that has the capture.
 * @param hwndDesklet the handle of the desklet.
 * @remark 1.3 function uses (UINT)-1 for no-layer, rather than the ambgious 0.
 * @return returns the layer's id that HAS the capture.
 * @remark v2 ready
 */
UINT CALLBACK DeskletLayerGetCapture(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		return d->GetDocklet()->GetCaptureLayer();
	}
	else
	{
		return (UINT)-1;
	}
}

/**
 * Hit-tests a layer for a certain point.
 * @param hwndDesklet the handle of the desklet
 * @param id the id of the layer to test
 * @param pt a pointer to a POINT structure describing the point on desklet-scale to test. 
 *				this x and y-values are relative to s. Cannot be NULL.
 * @param s the scale of pt. Cannot be NULL. See remarks.
 * @remark the POINT structure is relative to the s->cx, s->cy, s parameter. For example, if s->cx and s->cy
 *			are both 100, then the POINT structure will give the coordinates in percentages of the desklet.
 * @return TRUE iff pt would hit-test on id, otherwise FALSE.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerHitTest(HWND hwndDesklet, unsigned int id, const POINT* pt, const SIZE* s)
{
	if(NULL == pt)
		return FALSE;

	if(NULL == s)
		return FALSE;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			return l->HitTest(*pt, *s) ? TRUE : FALSE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/**
 * Sets the Image Clipping Rectangle for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image clipping rectangle for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param useClipping iff TRUE, clipping will be used on the layers image.
 * @param clipRect the relative clipping rectangle to be used for the image. Can be NULL if useClipping = FALSE, otherwise must be not NULL.
 * @param xScale the scale on which all x coordinates in clipRect are based
 * @param yScale the scale on which all y coordinates in clipRect are based
 * @remark if xScale and yScale are both 100, clipRect will describe percentages of the images that should be visible.
 *			e.g. if clipRect->left = 0, clipRect->right = 50. 50% of the layers image will be used for drawing.
 *			If you make xScale == image.Width and yScale == image.Height. clipRect maps directly onto the
 *			pixels that should be visible for the layers image.
 * @return TRUE iff sucessfull.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerSetImageClipRect(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL useClipping, const RECT* clipRect, 
										   int xScale, int yScale)
{
	if(useClipping && NULL == clipRect)
		return FALSE;

	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	InvalidateLayerRect(d, l, FALSE);
	//if(l->IsVisible())
	//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());

	SIZE scale = {xScale, yScale};
	l->SetUseImageSourceBounds(useClipping != FALSE, *clipRect, scale);
	InvalidateLayerRect(d, l, FALSE);

	if(redraw)
		d->Redraw();

	return TRUE;
}


/**
 * Gets the Image Clipping Rectangle for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image clipping rectangle for.
 * @param useClipping if not NULL, this parameter will receive TRUE iff clipping is used for the layer's image.
 * @param clipRect if not NULL, this parameter will receive the clipRect of the layer's image.
 * @param xScale if not NULL, this parameter will receive the xScale of the layer's clipRect.
 * @param yScale if not NULL, this parameter will receive the yScale of the layer's clipRect.
 * @remark if xScale and yScale are both 100, clipRect will describe percentages of the images that should be visible.
 *			e.g. if clipRect->left = 0, clipRect->right = 50. 50% of the layers image will be used for drawing.
 *			If you make xScale == image.Width and yScale == image.Height. clipRect maps directly onto the
 *			pixels that should be visible for the layers image.
 * @return TRUE iff sucessfull.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerGetImageClipRect(HWND hwndDesklet, unsigned int id, BOOL* useClipping, RECT* clipRect,
										int* xScale, int* yScale)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	bool use(false);
	RECT r = {0};
	SIZE s = {0};
	l->GetUseImageSourceBounds(use, r, s);
	if(useClipping != NULL)
		*useClipping = use ? TRUE : FALSE;

	if(clipRect != NULL)
		*clipRect = r;

	if(xScale != NULL)
		*xScale = s.cx;

	if(yScale != NULL)
		*yScale = s.cy;

	return TRUE;
}


/**
 * @deprecated
 */
BOOL CALLBACK DeskletLayerSetImageSourceBounds(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL useBounds, RECT* bounds, int xScale, int yScale)
{
	return DeskletLayerSetImageClipRect(hwndDesklet, id, redraw, useBounds, bounds, xScale, yScale);
}

/**
 * Sets the placement for this layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the placement of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param usePlacement iff TRUE, the layer will be placed at a specific point on the desklet. Otherwise, 
 *						the layer will be stretched over the whole desklet.
 * @param placementRect the placement rectangle to use. Cannot be NULL iff usePlacement is TRUE.
 * @param xScale the scale on which all x coordinates in placementRect are based
 * @param yScale the scale on which all y coordinates in placementRect are based
 * @remark if no placement is used, the layer will be stretched over the whole desklet (0,0, deskletWidth, deskletHeight)
 *			when drawn. If placement is used, the layer will be drawn at a specific place.
 *			The coordinates passed to this function are on a relative scale. if xScale and yScale are both 100, 
 *			then placementRect coordinates are in percentages. This means that if the desklet is resized, the layers
 *			will scale accordingly. For example, when xScale = 100, yScale = 100 and
 *			placementRect = {0,0,50,50}, the layer will always be drawn in the left upper quarter of the desklet.
 *			If you make sure xScale = deskletWidth, yScale = deskletHeight, then you map the desklets
 *			pixels directly on the layers placementRect.
 * @remark v2 ready
 */
void CALLBACK DeskletLayerSetPlacement(HWND hwndDesklet, unsigned int id,BOOL redraw, BOOL usePlacement, RECT* s, int xScale, int yScale)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{	
			// invalidate the old place
			InvalidateLayerRect(d, l, FALSE);
			//if(l->GetVisibility() && l->CanBeDrawn())
				//d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());

			l->SetPlacement(usePlacement!=FALSE,s,xScale,yScale);

			// invalidate the new place
			InvalidateLayerRect(d, l, FALSE);
			//if(l->GetVisibility() && l->CanBeDrawn())
			//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());

			d->GetDocklet()->SetIsDirty(true);

		}
		if(redraw)d->Redraw();
	}	
}


/**
 * Gets the placement for this layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the placement of.
 * @param usePlacement if not NULL, usePlacement will receive TRUE if the layer is currently using placement.
 * @param placementRect if not NULL, placementRect will receive the layer's placement rectangle.
 * @param xScale if not NULL, xScale will recieve the x-scale of the layer's placement rectangle.
 * @param yScale if not NULL, yScale will recieve the y-scale of the layer's placement rectangle.
 * @remark if no placement is used, the layer will be stretched over the whole desklet (0,0, deskletWidth, deskletHeight)
 *			when drawn. If placement is used, the layer will be drawn at a specific place.
 *			The coordinates passed to this function are on a relative scale. if xScale and yScale are both 100, 
 *			then placementRect coordinates are in percentages. This means that if the desklet is resized, the layers
 *			will scale accordingly. For example, when xScale = 100, yScale = 100 and
 *			placementRect = {0,0,50,50}, the layer will always be drawn in the left upper quarter of the desklet.
 *			If you make sure xScale = deskletWidth, yScale = deskletHeight, then you map the desklets
 *			pixels directly on the layers placementRect.
 * @return TRUE iff sucessfull.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerGetPlacement(HWND hwndDesklet, unsigned int id, BOOL* usePlacement, RECT* placementRect, 
									int* xScale, int* yScale)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	if(usePlacement != NULL)
		*usePlacement = l->GetUsePlacement() ? TRUE : FALSE;

	if(placementRect != NULL)
		*placementRect = l->GetPlacementRect();
	
	SIZE scale = l->GetPlacementScale();
	if(xScale != NULL)
		*xScale = scale.cx;

	if(yScale != NULL)
		*yScale = scale.cy;

	return TRUE;
}

/**
 * Sets the icon state of a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the iconstate of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param canBeIcon iff FALSE, the layer cannot be an icon (cannot be changed thru setimage, no dropshadow applied).
 * @remark v2 ready
 */
void CALLBACK DeskletLayerSetIconState(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL canBeIcon)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetCanBeIcon(canBeIcon!=FALSE);
			d->GetDocklet()->SetIsDirty(true);
		}
		if(redraw)d->Redraw();
	}	
}

/**
 * Sets the isNotMovable state of this layer.
 * @remark if the isNotMovable state for a desklet is TRUE, the whole desklet cannot be moved when
 *				the mouse is being held down on this particular layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the isNotMovable state of.
 * @param isNotMovable the new value of the isNotMovable state for this layer.
 * @remark v2 ready
 */
void CALLBACK DeskletLayerSetIsNotMovable(HWND hwndDesklet, unsigned int id, BOOL isNotMovable)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetIsNotMovable(isNotMovable != FALSE);
		}
	}
}


/**
 * Fets the isNotMovable state of this layer.
 * @remark if the isNotMovable state for a desklet is TRUE, the whole desklet cannot be moved when
 *				the mouse is being held down on this particular layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the isNotMovable state of.
 * @param the value of the isNotMovable state for this layer. If (hwndDesklet, id) is invalid, FALSE will be returned.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerGetIsNotMovable(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	return l->GetIsNotMovable() ? TRUE : FALSE;
}

/**
 * Sets the isClickThrough state of this layer.
 * @remark if the isClickThrough state for a desklet is TRUE, hittesting on the desklet will always
 *			result in a "no-hit". The layer cannot be click upon thus and will not receive any mouse-messages.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the isClickThrough state of.
 * @param isClickThrough the new value of the isClickThrough state for this layer.
 * @remark v2 ready
 */
void CALLBACK DeskletLayerSetIsClickThrough(HWND hwndDesklet, unsigned int id, BOOL isClickThrough)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetIsClickThrough(isClickThrough != FALSE);
		}
	}
}

/**
 * Gets the isClickThrough state of this layer.
 * @remark if the isClickThrough state for a desklet is TRUE, hittesting on the desklet will always
 *			result in a "no-hit". The layer cannot be click upon thus and will not receive any mouse-messages.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to get the isClickThrough state of.
 * @return the isClickThrough for a layer. If (hwndDesklet, id) is invalid, then FALSE will always be returned.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerGetIsClickThrough(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	return l->GetIsClickThrough() ? TRUE : FALSE;
}

/**
 * Sets the noAlphaBlendedHitTesting state of this layer.
 * @remark by default, layers use the alpha-blended information in their images when hit-testing. Thus,
 *				if the mouse is over a pixel with alpha=0, the layer will hit-test "no-hit". By setting 
 *				the noAlphaBlendedHitTesting to TRUE, alpha information will not be taken into
 *				account when hit-testing; layers will then have rectangular hit-testing areas.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the noAlphaBlendedHitTesting state of.
 * @param noAlphaBlendedHitTesting the new value of the noAlphaBlendedHitTesting state for this layer.
 * @remark v2 ready
 */
void CALLBACK DeskletLayerSetNoAlphaBlendedHitTesting(HWND hwndDesklet, unsigned int id, BOOL noAlphaBlendedHitTesting)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetNoAlphaBlendedHitTesting(noAlphaBlendedHitTesting != FALSE);
		}
	}
}

/**
 * Gets the noAlphaBlendedHitTesting state of this layer.
 * @remark by default, layers use the alpha-blended information in their images when hit-testing. Thus,
 *				if the mouse is over a pixel with alpha=0, the layer will hit-test "no-hit". By setting 
 *				the noAlphaBlendedHitTesting to TRUE, alpha information will not be taken into
 *				account when hit-testing; layers will then have rectangular hit-testing areas.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to get the noAlphaBlendedHitTesting state of.
 * @return the noAlphaBlendedHitTesting state of the layer, or, if (hwndDesklet, id) is invalid, FALSE always.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerGetNoAlphaBlendedHitTesting(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	return l->GetNoAlphaBlendedHitTesting()? TRUE : FALSE;
}

/**
 * Gets the id by the name of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param name the name of the layer to get the id for. Cannot be NULL.
 * @param success optional pointer to a BOOL which value will be set to TRUE iff the a layer with name 'name' was found,
 *			otherwise it's set to FALSE. Can be NULL.
 * @remark layer names are ANSI, since they are only internal to a desklet and avedesk.
 * @return the id of the layer with name 'name' if successfull, or 0 otherwise. use the success parameter for error-checking.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark v2 ready
 */
unsigned int CALLBACK DeskletLayerGetLayer(HWND hwndDesklet, const char* name, BOOL* success)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		int i(0);
		for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
		{
			Layer* l = *iter;
			if(strcmp(l->GetName(),name)==0)
			{
				if(success)
					*success = TRUE;
				return i;
			}
			i++;
		}

		// check sublayers if no top-level layer has been found
		std::map<UINT, Layer*>& subLayers = d->GetDocklet()->GetSubLayers();
		for(std::map<UINT, Layer*>::iterator subIter = subLayers.begin(); subIter != subLayers.end(); ++subIter)
		{
			Layer* l = subIter->second;
			if(l != NULL)
			{
				if(strcmp(l->GetName(), name) == 0)
				{
					if(success)
						*success= TRUE;

					return subIter->first;
				}
			}
		}

		if(success)
			*success = FALSE;
		return 0;
	}
	else
	{
		if(success)
			*success = FALSE;
		return 0;
	}

}


/**
 * Gets the number of TOP-LEVEL layers in a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @return the number of layers this desklet has.
 * @remark v2 ready
 * @see DeskletLayerCountEx
 */
unsigned int CALLBACK DeskletLayerCount(HWND hwndDesklet)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		return d->GetDocklet()->GetLayers().size();
	}
	else
	{
		return 0;
	}
}


// forward declaration:
void  CALLBACK DeskletLayerRemove(HWND hwndDesklet, unsigned int id, BOOL redraw);

/**
 * Removes all layers from a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after all layers have been applied.
 * @return TRUE iff successfull.
 */
BOOL CALLBACK DeskletLayerRemoveAll(HWND hwndDesklet, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	while(DeskletLayerCount(hwndDesklet) > 0)
	{
		d->GetDocklet()->SetIsDirty(true);
		DeskletLayerRemove(hwndDesklet, 0 , FALSE);
	}

	if(redraw)
		d->Redraw();

	return TRUE;
}

/**
 * Gets the number of sub-layers for a layer.
 * @param hwndDesklet the handle of the desklet
 * @param parentLayerId the id of the parent layer to count the sub-layers of. Returns the number of
 *						top-level layers if -1.
 * @return the number of layers the layer with id parentLayerId has.
 * @remark v2 ready
 */
unsigned int CALLBACK DeskletLayerCountEx(HWND hwndDesklet, unsigned int parentLayerId)
{
	if(-1 == parentLayerId)
		return DeskletLayerCount(hwndDesklet);
	else
	{
		Docklet* d = Docklet::FindDocklet(hwndDesklet);
		if(d != NULL)
		{
			Layer* l = GetLayerFromID(d, parentLayerId);
			if(l != NULL)
			{
				return (unsigned int) l->GetSubLayers().size();
			}
		}

		return 0;
	}
}


const int LAYER_IS_CONTAINER = 0x01;
const int LAYER_IS_BUFFERED =  0x02;

/**
 * Adds a layer to a desklet (extended function).
 * @param hwndDesklet the handle of the desklet.
 * @param name the name for the new layer. Cannot be NULL.
 * @param flags the flags for this layer: valid are LAYER_IS_CONTAINER and LAYER_IS_BUFFERED
 * @remark layer names are ANSI, since they are only internal to a desklet and avedesk.
 * @return the id of the layer just added, or the id of the layer that had the name 'name'
 *			already.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark v2 ready
 */

unsigned int CALLBACK DeskletLayerAddEx(HWND hwndDesklet, const char* name, const char* nameOfParent, DWORD flags)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return (UINT)-1;

	ATLASSERT(d->GetThreadID() == GetCurrentThreadId());

	std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
	std::map<UINT, Layer*>& subLayers = d->GetDocklet()->GetSubLayers();

	
	Layer* parentLayer = NULL;
	if(nameOfParent != NULL)
	{
		BOOL foundParentLayer(FALSE);
		UINT parentId = DeskletLayerGetLayer(hwndDesklet, nameOfParent, &foundParentLayer);
		if(foundParentLayer)
			parentLayer = GetLayerFromID(d, parentId);

		// only container layers can be parents, so return with an error instead
		if(NULL == parentLayer || !parentLayer->IsContainer())
			return (UINT)-1;
	}

	UINT newId = (UINT)-1;

	if(NULL == parentLayer)
	{
		Layer* l = NULL;
		if(flags & LAYER_IS_CONTAINER)
			l = new ContainerLayer(name, NULL, (flags & LAYER_IS_BUFFERED) != 0);
		else
			l = new Layer(name);

		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		newId = (UINT) layers.size();
		layers.push_back(l);
		d->GetDocklet()->SetIsDirty(true);	
	}
	else
	{
		// find the last sublayer ID
		UINT lastId = SUBLAYER_MASK;
		for(std::map<UINT, Layer*>::iterator iter = subLayers.begin(); iter != subLayers.end(); ++iter)
			lastId = max(lastId, iter->first);
		newId = lastId + 1;

		Layer* newLayer = NULL;
		if(flags & LAYER_IS_CONTAINER)
			newLayer = new ContainerLayer(name, parentLayer, (flags & LAYER_IS_BUFFERED) != 0);
		else
			newLayer =  new Layer(name, parentLayer);

		parentLayer->AddChild(newLayer);
		subLayers[newId] = newLayer;
	}

	return newId;
}

/**
 * Adds a layer to a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @param name the name for the new layer. Cannot be NULL.
 * @remark layer names are ANSI, since they are only internal to a desklet and avedesk.
 * @return the id of the layer just added, or the id of the layer that had the name 'name'
 *			already.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark v2 ready
 */
unsigned int CALLBACK DeskletLayerAdd(HWND hwndDesklet, const char* name)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = new Layer(name);
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		unsigned int res = layers.size();
		layers.push_back(l);
		d->GetDocklet()->SetIsDirty(true);
		return res;
	}
	else
	{
		return 0;
	}
}

/**
 * Removes a layer from a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to remove.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerRemove(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			InvalidateLayerRect(d, l, FALSE);

			if(!IsIDSubLayer(id))
			{
				std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
				std::vector<Layer*>::iterator iter = std::find(layers.begin(),layers.end(),l);
				{
					if(iter != layers.end() )
					{
						delete l;
						std::vector<Layer*> newLayers;
						for(UINT i = 0; i < layers.size(); ++i)
						{
							if(i != id)
								newLayers.push_back(layers[i]);
						}
						layers = newLayers;
					}
				}
			}
			else
			{
				ContainerLayer* parent = (ContainerLayer*)l->GetParent();
				if(parent != NULL)
				{
					std::vector<Layer*>& subLayers = parent->GetSubLayers();
					subLayers.erase(std::find(subLayers.begin(), subLayers.end(), l));
					d->GetDocklet()->GetSubLayers().erase(id);

					delete l;
				}
			}

			d->GetDocklet()->SetIsDirty(true);
		}
		if(redraw)d->Redraw();
	}
}


/**
 * Merges all layers into one new layer with id 0.
 * @param hwndDesklet the handle of the desklet.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark NOT v2 ready
 */
void  CALLBACK DeskletLayerMergeAll(HWND hwndDesklet, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		if( layers.size() > 0 )
		{
			SIZE s = d->GetDocklet()->GetSize();
			Bitmap* bmp = new Bitmap(s.cx,s.cy);
			Graphics g(bmp);
			d->GetDocklet()->ApplyQualityTo(&g);
			for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
			{
				Layer* l = *iter;
				l->Draw(&g,0,0,s.cx,s.cy,0);

				// FIX: only delete the layer if it's not the first one, because we will use the first
				//      layer to store the result on
				if(iter != layers.begin())
					delete l;
			}
			Layer* first = layers[0];
			first->SetImage(bmp,false);
			layers.clear();
			layers.push_back(first);
			d->GetDocklet()->SetIsDirty(true);

			// update the whole desklet
			RECT updateRect = {0, 0, s.cx, s.cy};
			d->GetDocklet()->AddUpdateRect(updateRect, d->GetDocklet()->GetSize());
			
		}
		if(redraw)d->Redraw();
	}
}


/**
 * Merges only the visible layers into one new layer with the lowest id possible.
 * @param hwndDesklet the handle of the desklet.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect layer ids will be re-assigned
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark NOT v2 ready
 */
void  CALLBACK DeskletLayerMergeVisible(HWND hwndDesklet,  BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		if( layers.size() > 0 )
		{
			SIZE s = d->GetDocklet()->GetSize();
			Bitmap* bmp = new Bitmap(s.cx,s.cy);
			Graphics g(bmp);
			d->GetDocklet()->ApplyQualityTo(&g);

			std::vector<Layer*> notVisibleLayers;

			for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
			{
				Layer* l = *iter;
				if(l->GetVisibility() )
				{
					l->Draw(&g,0,0,s.cx,s.cy,0);
					// FIX: only delete the layer if it's not the first one, because we will use the first
					//      layer to store the result on
					if(iter != layers.begin())
						delete l;
				}
				else
				{
					notVisibleLayers.push_back(l);
				}
			}
			Layer* first = layers[0];
			first->SetImage(bmp,false);
			layers.clear();
			layers.push_back(first);
			layers.insert(layers.end(),notVisibleLayers.begin(),notVisibleLayers.end() );
			d->GetDocklet()->SetIsDirty(true);

			// TODO: calculate better
			// update the whole desklet
			RECT updateRect = {0, 0, s.cx, s.cy};
			d->GetDocklet()->AddUpdateRect(updateRect, d->GetDocklet()->GetSize());
			
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Moves a layer to the bottom.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layers ids will be re-assigned. This layer will get an id of 0.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark not V2 ready
 */
void  CALLBACK DeskletLayerMoveBottom(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		if( id < layers.size() )
		{
			Layer* l = layers[id];
			std::vector<Layer*>::iterator iter = std::find(layers.begin(),layers.end(),l);
			if( iter != layers.end() )
			{
				layers.erase(iter);
				layers.insert(layers.begin(),l);
				d->GetDocklet()->SetIsDirty(true);

				if(l->GetVisibility())
					d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
			}
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Moves a layer down in the layer-order.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark not V2 ready
 */
void  CALLBACK DeskletLayerMoveDown(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		if( id > 0 && id < layers.size() )
		{
			Layer* l = layers[id];
			std::vector<Layer*>::iterator iter = std::find(layers.begin(),layers.end(),l);
			if( iter != layers.end() )
			{
				std::vector<Layer*>::iterator iterDown = iter - 1;

				// FIX: only the iterator-VALUES were swapped, not the actual iterators.
				//      so, rendering this function effectively useless. has been
				//      fixed by using iter_swap
				std::iter_swap(iter,iterDown);
				d->GetDocklet()->SetIsDirty(true);

				if(l->GetVisibility())
					d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
			}
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Moves a layer to the top in the layer-order, thus getting an id of DeskletLayerGetCount() - 1.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark NOT v2 ready
 */
void  CALLBACK DeskletLayerMoveTop(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		if( id < layers.size() )
		{
			Layer* l = layers[id];
			std::vector<Layer*>::iterator iter = std::find(layers.begin(),layers.end(),l);
			if( iter != layers.end() )
			{
				layers.erase(iter);
				layers.push_back(l);
				d->GetDocklet()->SetIsDirty(true);

				if(l->GetVisibility())
					d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
			}
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Moves a layer one up in the layer-order.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 * @remark NOT v2 ready
 */
void  CALLBACK DeskletLayerMoveUp(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		std::vector<Layer*>& layers = d->GetDocklet()->GetLayers();
		if( id < layers.size()-1 )
		{
			Layer* l = layers[id];
			std::vector<Layer*>::iterator iter = std::find(layers.begin(),layers.end(),l);
			if( iter != layers.end() )
			{
				std::vector<Layer*>::iterator iterDown = iter + 1;
				// FIX: only the iterator-VALUES were swapped, not the actual iterators.
				//      so, rendering this function effectively useless. has been
				//      fixed by using iter_swap
				std::iter_swap(iter,iterDown);
				d->GetDocklet()->SetIsDirty(true);

				if(l->GetVisibility())
					d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
			}
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Resets all properties of a layer
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to reset.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerReset(HWND hwndDesklet, unsigned int id, BOOL redraw)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			if(l->GetRotation() % 360 != 0)
			{
				InvalidateLayerRect(d, l, FALSE);
			}

			l->SetRotation(0);
			l->SetAlpha(255);
			l->SetColorMatrix(0);
			l->SetHue(0);
			l->SetSaturation(100);
			d->GetDocklet()->SetIsDirty(true);

			InvalidateLayerRect(d, l, FALSE);
			//if(l->GetVisibility())
			//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Sets the alpha level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the alpha-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param alpha the new alpha value of the layer. range [0,255] with 0 is invisible, 255 fully visible.
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerSetAlpha(HWND hwndDesklet, unsigned int id, BOOL redraw, BYTE alpha)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetAlpha(alpha);
			d->GetDocklet()->SetIsDirty(true);

			InvalidateLayerRect(d, l, FALSE);
			//if(l->GetVisibility())
				//d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
		}
		if(redraw)d->Redraw();
	}
}


/**
 * Gets the alpha level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the alpha-value of.
 * return the alpha value of the layer. range [0,255] with 0 is invisible, 255 fully visible. If 
 *			(hwndDesklet, id) is invalid, 0 will always be returned.
 * @remark v2 ready
 */
BYTE CALLBACK DeskletLayerGetAlpha(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return 0;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return 0;

	return l->GetAlpha();
}

/**
 * Sets a colormatrix for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the colormatrix for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param matrix the colormatrix for the layer. Can be NULL to un-assign it.
 * @remark might get deprecated in the next release
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerSetColorMatrix(HWND hwndDesklet, unsigned int id, BOOL redraw, ColorMatrix* matrix)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetColorMatrix(matrix);
			d->GetDocklet()->SetIsDirty(true);

			InvalidateLayerRect(d, l, FALSE);
			//if(l->GetVisibility())
			//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
		}
		if(redraw)d->Redraw();
	}
}


/**
 * Gets a colormatrix for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the colormatrix for.
 * @param matrix will retrieve the colormatrix for this layer.
 * @return TRUE iff successfull.
 * @remark might get deprecated in the next release
 * @remark v2 ready
 */
BOOL  CALLBACK DeskletLayerGetGolorMatrix(HWND hwndDesklet, unsigned int id, ColorMatrix* matrix)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;


	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	if(matrix != NULL)
		l->GetColorMatrix(*matrix);

	return TRUE;
}



/**
 * Sets the hue level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the hue-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param hue the new hue value of the layer.
 * @remark v2 ready
 */
void CALLBACK DeskletLayerSetHue(HWND hwndDesklet, unsigned int id, BOOL redraw, int hue)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetHue(hue);
			d->GetDocklet()->SetIsDirty(true);

			InvalidateLayerRect(d, l, FALSE);
			//if(l->GetVisibility())
			//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Gets the hue level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the hue-value of.
 * @return the new hue value of the layer. If (hwndDesklet, id) is invalid, 0 will be returned.
 * @remark v2 ready
 */

INT CALLBACK DeskletLayerGetHue(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return 0;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return 0;

	return l->GetHue();
}

/**
 * Sets a GDI+ image for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param img. the image to use for this layer. Can be NULL to specify no image.
 * @param if AutoDel is TRUE, AveDesk takes ownership of this object lifetime and thus the caller
 *						may not free/delete it. If FALSE, AveDesk will directly use img
 *						when redrawing is done, so changes made to the image will result in an
 *						updated desklet when the desklet is redrawn. Threading issues must be taken
 *						into account by the caller then.
 * @remark For performance, AutoDel can be set to FALSE so updating might be less expensive.
 * @remark NON-UNICODE version of DeskletLayerSetImageFileW
 * @see DeskletLayerSetImageFileW
 */
void  CALLBACK DeskletLayerSetImage(HWND hwndDesklet, unsigned int id, BOOL redraw, Image* img, BOOL AutoDel)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetImage(img, !AutoDel);
			d->GetDocklet()->SetIsDirty(true);
	
			InvalidateLayerRect(d, l, FALSE);
		}
	
		if(redraw)d->Redraw();
	}
}


/**
 * Sets an image file for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image file for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param filename the filename of the image file to use for this layer. Can be NULL to specify no image.
 * @remark NON-UNICODE version of DeskletLayerSetImageFileW
 * @see DeskletLayerSetImageFileW
 * @ remark v2 ready
 */
void  CALLBACK DeskletLayerSetImageFile(HWND hwndDesklet, unsigned int id, BOOL redraw, const char* filename)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			//if(id==0 && (layers.size() == 0 || layers[0]->GetVisibility()))
			//	DeskletSetImageFile(hwndDesklet,filename);
			//else
			{
				if(!strcmp_nocase(l->GetFileName(),filename))
				{
					l->SetFileName(filename);
					l->SetImage(DeskletLoadGDIPlusImage(filename),false);
					d->GetDocklet()->SetIsDirty(true);

					InvalidateLayerRect(d, l, FALSE);
					//if(l->GetVisibility())
					//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
				}
			}
		}
		if(redraw)d->Redraw();
	}
}

/**
 * Sets an image file for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image file for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param filename the filename of the image file to use for this layer. Can be NULL to specify no image.
 * @remark UNICODE version of DeskletLayerSetImageFile
 * @see DeskletLayerSetImageFile
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerSetImageFileW(HWND hwndDesklet, unsigned int id, BOOL redraw, const WCHAR* filename)
{
	if(NULL == filename)
		DeskletLayerSetImageFile(hwndDesklet, id, redraw, NULL);
	else
		DeskletLayerSetImageFile(hwndDesklet, id, redraw, WStringToString(filename).c_str());
}


/**
 * Sets the rotation level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the rotation-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param rotation the new rotation value in degrees for the layer.
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerSetRotation(HWND hwndDesklet, unsigned int id, BOOL redraw, int rotation)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			if(l->GetRotation() != rotation)
			{
				// invalidate the old rectangle
				InvalidateLayerRect(d, l, FALSE);
				//if(l->GetVisibility())
				//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());

				l->SetRotation(rotation);
				d->GetDocklet()->SetIsDirty(true);

				// invalidate the new rectangle
				InvalidateLayerRect(d, l, FALSE);
				//if(l->GetVisibility())
				//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
			}
		}
		if(redraw)
			d->Redraw();
	}
}

/**
 * Gets the rotation level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the rotation-value of.
 * @return the rotation value in degrees for the layer. If (hwndDesklet, id) is invalid. 0 is returned.
 * @remark v2 ready
 */
int CALLBACK DeskletLayerGetRotation(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return 0;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return 0;

	return l->GetRotation();
}


/**
 * Sets the saturation level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set saturation hue-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param saturation the new saturation value of the layer.
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerSetSaturation(HWND hwndDesklet, unsigned int id, BOOL redraw, unsigned int saturation)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d != NULL)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			l->SetSaturation(saturation);
			d->GetDocklet()->SetIsDirty(true);

			InvalidateLayerRect(d, l, FALSE);
			//if(l->GetVisibility())
			//	d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
		}
		if(redraw)d->Redraw();
	}
}


/**
 * Gets the saturation level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set saturation hue-value of.
 * @return the saturation value of the layer. If (hwndDesklet, id) is invalid, 0 is be returned.
 * @remark v2 ready
 */
int CALLBACK DeskletLayerGetSaturation(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return 0;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return 0;

	return l->GetSaturation();
}


/**
 * Sets the visibility of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set visibility of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param visibility. iff TRUE the layer will be visible, otherwise not.
 * @remark v2 ready
 */
void  CALLBACK DeskletLayerSetVisibility(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL visibility)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(d)
	{
		Layer* l = GetLayerFromID(d, id);
		if(l != NULL)
		{
			// SEMI-FIX: only update when actually changing visability.
			if(l->GetVisibility() != (visibility != FALSE) )
			{
				l->SetVisibility(visibility!=FALSE);
				d->GetDocklet()->SetIsDirty(true);
	
				InvalidateLayerRect(d, l, TRUE);
				//d->GetDocklet()->AddUpdateRect(GetLayerRectangle(l, d), d->GetDocklet()->GetSize());
			}
		}
		if(redraw)d->Redraw();
	}
}


/**
 * Gets the visibility of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set visibility of.
 * return TRUE iff the layer's visibility is set to visible, otherwise not. If (hwndDesklet, id) is 
 *			invalid, FALSE will be returned.
 * @remark this function does not tell if a layer is really visible. For example, a child-layer might be
 *			 visible but its parent not. In that case, this function will return TRUE for 
 *			 the child-layer, while the child-layer is not being visible at all. See DeskletLayerIsLayerVisible().
 * @see DeskletLayerIsLayerVisible
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerGetVisibility(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	return l->GetVisibility() ? TRUE : FALSE;
}

/**
 * Checks if a layer is visible.
 * @remark a layer might have its visibility set to 'visible' (TRUE), but might be not visible in the desklet
 *			because its parent layer might be hidden.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set visibility of.
 * @return TRUE iff the layer is really visible in the desklet, otherwise FALSE. If (hwndDesklet, id)
 *					is invalid, FALSE will be returned.
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerIsLayerVisible(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	while(l->IsVisible() && l->GetParent() != NULL)
		l = l->GetParent();

	return l->IsVisible() ? TRUE : FALSE;
}


/**
 * Gets the parent-layer of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to get the parent for.
 * @return if not successfull or if the layer does not have a parent-layer, (UINT)-1 is returned,
 *			otherwise the id of the parent-layer is returned.
 * @remark v2 ready
 */
unsigned int CALLBACK DeskletLayerGetParent(HWND hwndDesklet, unsigned int id)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return (UINT)-1;

	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return (UINT)-1;	

	Layer* parent = l->GetParent();
	if(NULL == parent)
		return (UINT)-1;

	BOOL didFindParent = FALSE;
	UINT parentId = DeskletLayerGetLayer(hwndDesklet, l->GetName(), &didFindParent);
	if(!didFindParent)
		parentId = (UINT)-1;
	
	return parentId;
}

/**
 * Sets a tooltiptext for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the tooltip text for.
 * @param text the text to set the tooltip to, can be NULL to indicate an empty text.
 * @return TRUE iff successfull
 * @remark v2 ready
 */
BOOL CALLBACK DeskletLayerSetToolTipText(HWND hwndDesklet, unsigned int id, const WCHAR* text)
{
	Docklet* d = Docklet::FindDocklet(hwndDesklet);
	if(NULL == d)
		return FALSE;

	// make sure the layer is actually a layer
	Layer* l = GetLayerFromID(d, id);
	if(NULL == l)
		return FALSE;

	d->GetDocklet()->SetToolTipText(l, text != NULL ? text : L"");

	return TRUE;
}