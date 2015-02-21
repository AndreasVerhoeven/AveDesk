//	Name:		Od Docklet Instance
//	Author:		Andreas Verhoeven
//	Date:		dec 16 2003, 18:48
//	Version:	1
//	Revision:	2
//	Comments:	<none>

#include "stdafx.h"
#define NO_LAYERED_WINDOW

#include "od_docklet_instance.hpp"

#include "docklet.h"
#include "ColorMatrixHelper.h"
#include "constants.h"

#include "DeskletSkinEnumerator.h"

#include <sstream>
#include "helper_funcs.h"
#include <math.h> // cmath unknown for vc6
#include <algorithm>

extern BOOL _noCachingOfBitmaps;

namespace docklets
{
	OdDockletInstance::OdDockletInstance(OdDocklet* docklet,DOCKLET_DATA* pointer, HWND hWnd) : 
		hWnd(hWnd), pointer(pointer),
		docklet(docklet), Alpha(255), useSaturation(false),useColorization(false), saturation(1.0f),
		quality(QUALITY_AVERAGE), drawShadow(false), shadowX(4), shadowY(4), shadowGray(0.2f),
		shadowAlpha(255), isSelected(false), isMouseOver(false), drawPlugin(0), rotation(0),
		xPos(0),yPos(0), width(128), height(128), hue(100), isDirty(false),
		shadowColor(0), m_PreviousMouseOverLayer((UINT)-1), m_PreviousMouseDownLayer((UINT)-1),
		m_CaptureLayer((UINT)-1), updateRegionLockCount(0)
	{
		InitializeCriticalSection(&m_CriticalSection);
		// post: an instance of the OdDocklet defined by docklet is created
		colorMatrix = ColorMatrixHelper::GetIdentityMatrix();

		// Add a default layer
		layers.push_back(new Layer("Default.Background"));
	}

	OdDockletInstance::~OdDockletInstance()
	{
		// post: the object is destroyed and docklet is destroyed too
		Destroy();

		for(std::vector<Layer*>::iterator iter=layers.begin(); iter != layers.end(); ++ iter)
			delete *iter;

		layers.clear();

		DeleteCriticalSection(&m_CriticalSection);
	}

	bool OdDockletInstance::SupportsFeature(DWORD flag)
	{
		// post: returns true iff flag is supported by this desklet.
		DWORD flags = 0;
		return GetFlags(flags) && ((flags & flag) != 0);

	}

	Layer* OdDockletInstance::GetLayer(unsigned int index)
	{
		// pre: < 0= index < Layers.size()
		// post: the layer with index has been returned

		return LayerFromId(index);
	}



	std::vector<Layer*>& OdDockletInstance::GetLayers()
	{
		// post: Layers has been returned
		return layers;
	}

	HWND OdDockletInstance::GetWindow()
	{
		// post: this docklets window has been returned
		return hWnd;
	};

	void OdDockletInstance::SetNormalImage(Image* img, bool clone)
	{
		// pre:  img is not null
		// post: the normal image is updated

		// First, we need to get a pointer to the background (=normal image)
		// layer
		Layer* l=0;
		if(layers.size() < 1)
		{
			// if there are no layers, we need to create one and add it to the list of layers
			l = new Layer("Default.Background");
			layers.push_back(l);
		}
		else
		{
			// there are layers present, so now find the right background layer.
			// this layer must be visible and must not have been set to 'cant be an icon'.
			for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
			{
				Layer* curLayer = *iter;
				if(curLayer->GetVisibility() && curLayer->GetCanBeIcon())
				{
					l = curLayer;
					break;
				}
			}
		}

		// if we found a layer, set its image to img
		if(l)
			l->SetImage(img,clone);

		// don't forget to redraw the docklet
		//Redraw();
		InvalidateAll();
		SetIsDirty(true);
	}

	void OdDockletInstance::SetOverlayImage(Image* img, bool clone)
	{
		// pre:  img is not null
		// post: the normal image is updated

		// get an pointer to the overlay image layer.
		// this is only for backward compatibility with OD Docklets,
		// so we don't search for a particular layer, we just use the
		// second layer.
		Layer* l=0;
		if(layers.size() < 2)
		{
			// if there is nog BG layer, add that too.
			if(layers.size() < 1)
			{
				l = new Layer("Default.Background");
				layers.push_back(l);
			}

			// there is no second layer, so add it
			l = new Layer("Default.Overlay");
			layers.push_back(l);
		}
		else
		{
			l = layers[1];
		}

		// set the layers image to img
		l->SetImage(img,clone);

		// redraw the docklet
		//Redraw();
		InvalidateAll();
		SetIsDirty(true);
	}

	// ave: 29/8 -begin
	void OdDockletInstance::Draw(Graphics& g, POINT& p, SIZE& s)
	{
		// post: A drag-'n-drop image has been drawn upon g.

		ApplyQualityTo(&g);

		// colour and position modification matrices. Maybe used by the DrawPlugin
		Matrix matrix(1, 0, 0, 1, 0, 0); 
		ColorMatrix cm = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorization)
			cm = colorMatrix;

		// only set the position modification matrix if either rotation is true
		// or if the drawplugin modified the matrix (faster drawing if not set).
		if(rotation!=0)
		{
			matrix.RotateAt((float)rotation,PointF((float)width/2,(float)height/2));
			g.SetTransform(&matrix);
		}

		bool shadowHasBeenDrawn(false);
		for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
		{
			Layer* l = *iter;
			if(!shadowHasBeenDrawn && l->GetVisibility() && l->GetCanBeIcon())
			{
				// if a shadow has not been drawn, and this layer is a candidate for a for a shadow to be drawn,
				// then draw it.
				l->DrawShadow(&g,shadowX+p.x,shadowY+p.y,s.cx,s.cy,shadowGray,shadowAlpha / 255.0f,shadowColor);
				shadowHasBeenDrawn = true;
			}
			l->Draw(&g,p.x,p.y,s.cx,s.cy,((!useColorization) ? 0: &cm ) );
		}
	}
	// ave: 29/8 -end

	void OdDockletInstance::DrawCachedBitmap(Graphics& g, const SIZE& s, const POINT& p)
	{
		ApplyQualityTo(&g);

		EnterCriticalSection(&m_CriticalSection);
		if(!drawShadow && !useColorization && rotation == 0 && drawPlugin == NULL && layers.size() == 1 &&
			layers[0]->GetVisibility() && layers[0]->CanBeDrawnDirectly(width,height)	)

		{
			g.DrawImage(layers[0]->GetBitmap(), Rect(p.x, p.y, s.cx, s.cy), 0,0, width, height, UnitPixel,0,0,0);
		}
		else if(m_CachedBitmap.get())
			g.DrawImage(m_CachedBitmap.get(), Rect(p.x, p.y, s.cx, s.cy), 0,0, m_CachedBitmap->GetWidth(), m_CachedBitmap->GetHeight(), UnitPixel,0,0,0);
		LeaveCriticalSection(&m_CriticalSection);
	}

	void OdDockletInstance::DrawInSimpleForm(Graphics& g, const SIZE& s, const POINT& p) 
	{
		ApplyQualityTo(&g);

		// colour and position modification matrices. Maybe used by the DrawPlugin
		Matrix matrix(1, 0, 0, 1, 0, 0); 
		ColorMatrix cm = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorization)
			cm = colorMatrix;

		// only set the position modification matrix if either rotation is true
		// or if the drawplugin modified the matrix (faster drawing if not set).
		if(rotation!=0)
		{
			matrix.RotateAt((float)rotation,PointF((float)width/2,(float)height/2));
			g.SetTransform(&matrix);
		}

		for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
		{
			Layer* l = *iter;
			l->Draw(&g,p.x,p.y,s.cx,s.cy,((!useColorization) ? 0: &cm ) );
		}
	}

	// ave: 06/6 -begin
	void OdDockletInstance::DrawDragDropImage(Graphics& g, const SIZE& s)
	{
		// post: A drag-'n-drop image has been drawn upon g.
		POINT p = {0, 0};
		DrawInSimpleForm(g,s,p);
	}
	// ave: 06/6 -end

	INT OdDockletInstance::LockUpdateRegion()
	{
		return ++updateRegionLockCount;
	}

	INT OdDockletInstance::UnlockUpdateRegion()
	{
		return --updateRegionLockCount;
	}

	BOOL OdDockletInstance::IsUpdateRegionLocked()const
	{
		return updateRegionLockCount > 0;
	}

	void OdDockletInstance::AddUpdateRect(const RECT& r, const SIZE& scale)
	{
		if(!IsUpdateRegionLocked())
		{
			RectF rf = RemapRectF(RECTTORECTF(r), GetSize(), scale.cx, scale.cy);
			RECT remappedRect = {0};
			remappedRect.left = (LONG)rf.X;
			remappedRect.right = (LONG)ceil(rf.X + rf.Width);
			remappedRect.top = (LONG)rf.Y;
			remappedRect.bottom = (LONG)ceil(rf.Y + rf.Height);
			//RECT remappedRect = RemapRect(r, GetSize(), static_cast<float>(scale.cx), static_cast<float>(scale.cy) );
			m_UpdateRegion.Add(remappedRect);
		}
	}
	
	POINT OdDockletInstance::GetCorrectedPos()const
	{
		int l = 0;//Layer::ratio/10;
		int t = 0;//Layer::ratio/10;

		if(drawShadow)
		{
			l = t = Layer::ratio/10;
		}

		POINT position = {xPos - l,yPos - t};

		if(rotation != 0)
		{
			SIZE s = {width, height};
			if(drawShadow)
			{
				s.cx += shadowX+20;
				s.cy += shadowY+20;
			}
			POINT p = {0};
			FixRotation(rotation,s,p);
			position.x -= p.x;
			position.y -= p.y;

		}

		return position;
	}

	void OdDockletInstance::SetUncorrectedPos(int x, int y)
	{
		int l = x;
		int t = y;

		if(drawShadow)
		{
			l += Layer::ratio/10;
			t += Layer::ratio/10;
		}

		if(rotation != 0)
		{
			SIZE s = {width, height};
			if(drawShadow)
			{
				s.cx += shadowX+20;
				s.cy += shadowY+20;

			}
			POINT p = {0,0};
			FixRotation(rotation,s,p);
			l += p.x;
			t += p.y;
		}

		SetPos(l,t);
	}

	void rotatePoint (float& x, float& y,float baseX, float baseY, float arc) 
	{ 
		float Dx = x - baseX; 
		float Dy = y - baseY; 

		if(Dx == 0 && Dy == 0)
			return;

		float D = sqrt(Dx * Dx + Dy * Dy);
		arc += atan2(Dy, Dx);

		x = baseX + D * cos(arc);
		y = baseY + D * sin(arc);
	}

	void OdDockletInstance::CorrectPoint(long& x, long& y)
	{
		int w = width;
		int h = height;

		if(drawShadow)
		{
			x -= Layer::ratio/10;
			y -= Layer::ratio/10;
		}
		
		if(rotation != 0)
		{
			const REAL pi = 3.14192f;
			const REAL rad = pi / 180.0f;


			SIZE s = {width,height};
			POINT p = {0};
			FixRotation(this->rotation,s,p);
			x -= p.x;
			y -= p.y;

			float fX = (float)x;
			float fY= (float)y;

			int rot = 360 - rotation;
			float radRotation = static_cast<float>(rot) / 180.0f * pi;
			float fW = static_cast<float>(width) / 2.0f;
			float fH = static_cast<float>(height) / 2.0f;
			if(drawShadow)
			{
				fW += static_cast<float>(Layer::ratio) / 10.0f;
				fH += static_cast<float>(Layer::ratio) / 10.0f;
			}
			rotatePoint(fX, fY, fW, fH, radRotation);
			//(origRotation <= 90 || (origRotation > 180 && origRotation <= 270)) ? alpha : beta);
			x = (long)fX;
			y = (long)fY;
		}

	}

	SIZE OdDockletInstance::GetCorrectedSize()const
	{
		SIZE size = {width,height};
		if(drawShadow)
		{
			size.cx += shadowX+20;
			size.cy += shadowY+20;
		}

		if(rotation != 0)
		{
			POINT p = {0,0};
			FixRotation(rotation,size,p);
		}


		return size;
	}

	Bitmap* OdDockletInstance::DrawLayers(const std::vector<UINT>& layersVisible, const SIZE& size)//, const SIZE& bmpSize)
	{
		int w = size.cx;
		int h = size.cy;

		int left = 0;
		int top  = 0;

		int origW = w;
		int origH = h;

		if(rotation != 0)
		{
			SIZE s = {w,h};
			POINT p = {0};
			FixRotation(rotation,s,p);
			w = s.cx;
			h = s.cy;
			left += p.x;
			top += p.y;
		}

		Bitmap* bmp = new Bitmap(w, h);
		Graphics g(bmp);
		ApplyQualityTo(&g);

			// colour and position modification matrices. Maybe used by the DrawPlugin
		Matrix matrix(1, 0, 0, 1, 0, 0); 
		ColorMatrix cm = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorization)
			cm = colorMatrix;

		// keeps track wether a drawplugin changed one of the matrices.
		BOOL modified(FALSE);

		// let a drawplugin modify the drawing process before the layers are drawn.
		if(drawPlugin)
			drawPlugin->BeforeDraw(GetWindow(),&g,left,top,width,height,&cm,&matrix,&modified,isSelected,isMouseOver);

		//SolidBrush br2(Color(255,0,255,0));
		//g.FillRectangle(&br2,left,top,w,h);

		// only set the position modification matrix if either rotation is true
		// or if the drawplugin modified the matrix (faster drawing if not set).
		if(rotation!=0 || modified)
		{
			matrix.RotateAt((float)rotation,PointF(static_cast<float>(w)/2.0f,static_cast<float>(h)/2.0f));

			g.SetTransform(&matrix);
		}


		//SolidBrush br(Color(255,255,0,0));
		//g.FillRectangle(&br,left,top,width+shadowX+20,height+shadowX+20);

		
		
		// draw each layer upon the temp. bitmap.
		UINT layerIndex(0);
		for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
		{
			Layer* l = *iter;

			bool oldVisibility = l->GetVisibility();

			bool tempVisibility = (std::find(layersVisible.begin(), layersVisible.end(), layerIndex) != layersVisible.end() );

			l->SetVisibility(tempVisibility);
			l->Draw(&g,left,top,origW,origH,((!useColorization && !modified) ? 0: &cm ) );

			l->SetVisibility(oldVisibility);

			layerIndex += 1;
		}


		// let a drawplugin modify the temp. bitmap after all layers have been drawn.
		if(drawPlugin)
			drawPlugin->AfterDraw(GetWindow(),&g,bmp,left,top,width,height,&cm,&matrix,&modified,isSelected,isMouseOver);

		return bmp;
	}

	void OdDockletInstance::InvalidateAll()
	{
		m_CachedBitmap = boost::shared_ptr<Bitmap>();
	}

	Bitmap* OdDockletInstance::Draw()
	{
		//ATLTRACE("OdDockletInstance::Draw()\n");
		if(!drawShadow && !useColorization && rotation == 0 && drawPlugin == NULL && layers.size() == 1 &&
			layers[0]->GetVisibility() && layers[0]->CanBeDrawnDirectly(width,height)	)

		{
			//ATLTRACE("    Draw() -> return layer 0\n");
			return layers[0]->GetBitmap();
		}


		if(m_UpdateRegion.IsEmpty() && !isDirty && m_CachedBitmap.get() != NULL)
		{
			//ATLTRACE("   Draw() -> return cachedbitmap\n");
			return m_CachedBitmap.get();
		}

		int w = width;
		int h = height;
		int left = 0;
		int top = 0;

		// adjust the sizes when we need to draw a shadow.
		if(drawShadow)
		{
			w += (shadowX+20);
			h += (shadowY+20);

			left += Layer::ratio/10;
			top += Layer::ratio/10;
		}

		if(rotation != 0)
		{
			SIZE s = {w,h};
			POINT p = {0};
			FixRotation(rotation,s,p);
			w = s.cx;
			h = s.cy;
			left += p.x;
			top += p.y;
		}

		boost::shared_ptr<Bitmap> bmp;
		bool usedOldBitmap(false);

		if(rotation%180 != 0 || _noCachingOfBitmaps || m_CachedBitmap.get() == NULL || m_CachedBitmap->GetWidth() != (UINT)w || m_CachedBitmap->GetHeight() != (UINT)h)
		{
			// create a temp-bitmap where we drawn upon.
			bmp = boost::shared_ptr<Bitmap>( new Bitmap(w,h) );
		}
		else
		{
			bmp = m_CachedBitmap;
			usedOldBitmap = true;
		}

		Graphics g(bmp.get());
		ApplyQualityTo(&g);

		// colour and position modification matrices. Maybe used by the DrawPlugin
		Matrix matrix(1, 0, 0, 1, 0, 0); 
		ColorMatrix cm = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorization)
			cm = colorMatrix;

		// keeps track wether a drawplugin changed one of the matrices.
		BOOL modified(FALSE);

		// let a drawplugin modify the drawing process before the layers are drawn.
		if(drawPlugin)
			drawPlugin->BeforeDraw(GetWindow(),&g,left,top,width,height,&cm,&matrix,&modified,isSelected,isMouseOver);


		// only set the position modification matrix if either rotation is true
		// or if the drawplugin modified the matrix (faster drawing if not set).
		if(rotation!=0 || modified)
		{
			//matrix.RotateAt((float)rotation,PointF((float)width/2,(float)height/2));
			if(drawShadow)
				matrix.RotateAt((float)rotation,PointF((float)(w-(shadowX+20))/2 + Layer::ratio/10,(float)(h-(shadowX+20))/2 + Layer::ratio/10));
			else
				matrix.RotateAt((float)rotation,PointF((float)w/2,(float)h/2));

			g.SetTransform(&matrix);
		}

		
		if(!usedOldBitmap)
		{
			// draw each layer upon the temp. bitmap.
			bool shadowHasBeenDrawn(!drawShadow);
			for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
			{
				Layer* l = *iter;
				if(!shadowHasBeenDrawn && l->GetVisibility() && l->GetCanBeIcon())
				{
					// if a shadow has not been drawn, and this layer is a candidate for a for a shadow to be drawn,
					// then draw it.
					l->DrawShadow(&g,shadowX+left,shadowY+top,width,height,shadowGray,shadowAlpha / 255.0f,shadowColor);
					shadowHasBeenDrawn = true;
				}
				l->Draw(&g,left,top,width,height,((!useColorization && !modified) ? 0: &cm ) );
			}
		}
		else
		{
			CompositingMode oldMode = g.GetCompositingMode();
			g.SetCompositingMode(CompositingModeSourceCopy);

			boost::shared_ptr<Region> region(m_UpdateRegion.GetRegion()->Clone());
			region->Translate(left, top);

			g.SetClip(region.get());
			std::list<RECT> rects = m_UpdateRegion.GetList();

			Layer* bgLayerUsedToErase = NULL;
			bool usedShadow(false);

			if(false)
			{
			// find first true background layer
			for(std::vector<Layer*>::iterator l_iter = layers.begin(); NULL == bgLayerUsedToErase && l_iter != layers.end(); ++l_iter)
			{
				Layer* l = *l_iter;
				if(l->GetVisibility())
				{
					if(!l->GetUsePlacement() && l->CanBeDrawn())
					{
						if(l->GetCanBeIcon() && drawShadow)
						{
							// use shadow
							usedShadow = true;
							//bgLayerUsedToErase = l;
						}
						else
						{
							usedShadow = false;
							bgLayerUsedToErase = l;
							l->DrawPartial(&g,left,top,width,height,((!useColorization && !modified) ? 0: &cm ), rects );
						}
					}
				}
			}
			}

			if(NULL == bgLayerUsedToErase)
			{
				g.SetPixelOffsetMode(PixelOffsetModeHalf);

				SolidBrush eraserBrush(Color(0,0,0, 0));
				for(std::list<RECT>::iterator rectIter = rects.begin(); rectIter != rects.end(); ++rectIter)
				{
					RECT r = *rectIter;
					SIZE s = SIZEFROMRECT(r);	
					Rect rect(r.left + left, r.top + top, s.cx, s.cy);
					g.FillRectangle(&eraserBrush, rect);
				}
			}

			g.SetCompositingMode(oldMode);
			g.SetPixelOffsetMode(PixelOffsetModeDefault);

			bool shadowHasBeenDrawn(!drawShadow);
			for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
			{
				Layer* l = *iter;

				if(!shadowHasBeenDrawn && l->GetVisibility() && l->GetCanBeIcon())
				{
					// if a shadow has not been drawn, and this layer is a candidate for a for a shadow to be drawn,
					// then draw it.
					if(!(bgLayerUsedToErase == l && usedShadow))
						l->DrawShadowPartial(&g,shadowX+left,shadowY+top,width,height,shadowGray,shadowAlpha / 255.0f,shadowColor, rects);
					shadowHasBeenDrawn = true;
				}

				if(!(bgLayerUsedToErase == l && !usedShadow))
					l->DrawPartial(&g,left,top,width,height,((!useColorization && !modified) ? 0: &cm ), rects );
			}

		}
	

		// let a drawplugin modify the temp. bitmap after all layers have been drawn.
		if(drawPlugin)
			drawPlugin->AfterDraw(GetWindow(),&g,bmp.get(),left,top,width,height,&cm,&matrix,&modified,isSelected,isMouseOver);

		EnterCriticalSection(&m_CriticalSection);
		m_CachedBitmap = bmp;
		LeaveCriticalSection(&m_CriticalSection);

		isDirty = false;

		m_UpdateRegion.Clear();

		return bmp.get();
	}

		bool OdDockletInstance::Select()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			SetSelected(true);
			if(GetDrawPlugin())
				GetDrawPlugin()->Event(EVENT_ONSELECT);

			return hWnd && docklet && docklet->Select(pointer);
		}

		bool OdDockletInstance::Deselect()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			SetSelected(false);
			if(GetDrawPlugin())
				GetDrawPlugin()->Event(EVENT_ONDESELECT);

			return hWnd && docklet && docklet->Deselect(pointer);
		}

		bool OdDockletInstance::ProcessMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->ProcessMessage(pointer,hWnd,uMsg,wParam,lParam);
		}

		LRESULT OdDockletInstance::ProcessMessageEx(UINT uMsg,WPARAM wParam,LPARAM lParam, BOOL& isHandled)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			if(hWnd && docklet)
			{
				return docklet->ProcessMessageEx(pointer,hWnd,uMsg,wParam,lParam,&isHandled);
			}
			else
			{
				isHandled = FALSE;
				return 0;
			}
		}

		bool OdDockletInstance::Save(const char* iniFile,const char* iniGroup, bool isForExport)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->Save(pointer,iniFile,iniGroup,isForExport);
		}

		Layer* OdDockletInstance::LayerFromId(UINT id)
		{
			if((UINT)-1 == id)
				return NULL;

			if(id < layers.size())
				return layers[id];

			return subLayers[id];
		}

		UINT OdDockletInstance::IdFromLayer(Layer* l)
		{
			if(NULL == l->GetParent())
			{
				UINT index(0);
				for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
				{
					Layer* iteredL = *iter;
					if(iteredL == l)
						return index;

					index++;
				}

			}
			else
			{
				for(std::map<UINT, Layer*>::iterator iter = subLayers.begin(); iter != subLayers.end(); ++iter)
				{
					if(iter->second == l)
						return iter->first;
				}
			}

			return (UINT)-1;
		}

		std::wstring OdDockletInstance::GetToolTipTextForLayer(const POINT& p, const SIZE& s)
		{
			//std::wstring tooltip = L"";
			//Layer* hittedLayer = NULL;
			UINT id = HitTestLayers(p, s, true, NULL);

			return GetToolTipTextForLayer(id);

			/*
			if(hittedLayer != NULL)
				tooltip = tooltipTexts[hittedLayer];

			while(tooltip.length() != 0 && hittedLayer != NULL && hittedLayer->GetParent() != NULL)
			{
				tooltip = tooltipTexts[hittedLayer->GetParent()];
				hittedLayer = hittedLayer->GetParent();
			}
				
			return tooltip;*/
		}

		std::wstring OdDockletInstance::GetToolTipTextForLayer(UINT id)
		{
			if((UINT)-1 == id)
				return L"";

			Layer* l = subLayers[id];
			if(NULL == l)
			{
				if(id < layers.size())
					l = layers[id];
			}

			std::wstring tooltip(tooltipTexts[l]);
			if(l != NULL)
			{
				const WCHAR* currentEntry = NULL;
				if(tooltip.length() > 0)
					currentEntry = tooltip.c_str();

				bool res = docklet->LayerGetToolTip(pointer, id, l->GetName(), currentEntry);
				if(res)
					tooltip = tooltipTexts[l];
			}
			//if(l != NULL)
			//	tooltip = StringToWString(l->GetName());

			while(tooltip.length() != 0 && l != NULL && l->GetParent() != NULL)
			{
				tooltip = tooltipTexts[l->GetParent()];
				l = l->GetParent();
			}

			return tooltip;

		}

		void OdDockletInstance::SetToolTipText(Layer*l, const std::wstring& ws)
		{
			tooltipTexts[l] = ws;
		}

		UINT OdDockletInstance::HitTestLayers(const POINT& p, const SIZE & s, bool mustBeVisible, Layer** pHittedLayer)
		{
			Layer* hittedLayer = NULL;
			if(pHittedLayer != NULL)
				*pHittedLayer = NULL;
			for(std::vector<Layer*>::reverse_iterator iter = layers.rbegin(); iter != layers.rend(); ++iter)
			{
				Layer*& l = *iter;
				if(l->GetHitTestedLayer(p, s, hittedLayer, mustBeVisible) && hittedLayer != NULL)
				{
					if(pHittedLayer != NULL)
						*pHittedLayer = hittedLayer;
					return IdFromLayer(hittedLayer);
				}
			}

			return static_cast<UINT>(-1);
		}

		Layer* OdDockletInstance::GetLayerAtPoint(const POINT& p, bool mustBeVisible, UINT* idVar)
		{
			SIZE  s = {width, height};

			POINT pt = p;
			CorrectPoint(pt.x, pt.y);

			Layer* hittedLayer = NULL;
			UINT id = HitTestLayers(pt, s, mustBeVisible, &hittedLayer);
			if(idVar != NULL)
				*idVar = id;

			return hittedLayer;
		}

		bool OdDockletInstance::MouseDown(POINT* pt, SIZE* size, DWORD keys)
		{
			POINT p = *pt;
			SIZE  s = *size;

			s.cx = width;
			s.cy = height;

			CorrectPoint(p.x, p.y);

			m_PreviousMouseDownLayer = (UINT)-1;
			Layer* layer = GetLayerAtPoint(*pt, true, &m_PreviousMouseDownLayer);
			if(layer != NULL)
			{
				POINT layerPoint = p;
				SIZE  layerSize  = s;
				layer->ConvertPointAndSize(layerPoint, layerSize);
				return hWnd && docklet && docklet->LayerMouseDown(pointer, m_PreviousMouseDownLayer, layer->GetName(), &layerPoint, &layerSize, keys);
			}
			else
			{
				return false;
			}
		}

		bool OdDockletInstance::MouseUp(POINT* pt, SIZE* size, DWORD keys)
		{
			POINT p = *pt;
			SIZE  s = *size;

			s.cx = width;
			s.cy = height;

			CorrectPoint(p.x, p.y);

			UINT oldLayer = m_PreviousMouseDownLayer;
			m_PreviousMouseDownLayer = (UINT)-1;
			if(oldLayer != -1)
			{
				Layer* layer = LayerFromId(oldLayer);
				POINT layerPoint = p;
				SIZE  layerSize  = s;
				if(layer != NULL)
				{	
					layer->ConvertPointAndSize(layerPoint, layerSize);
				}

				UINT newId = (UINT)-1;
				Layer* newLayer = GetLayerAtPoint(*pt, true, &newId);				

				bool retVal = false;
				if(layer != NULL)
					retVal =  hWnd && docklet && docklet->LayerMouseUp(pointer, oldLayer, layer->GetName(), &layerPoint, &layerSize, keys);


				if(false == retVal && newId == oldLayer && layer != NULL) // && retVal == false
					retVal = hWnd && docklet && docklet->LayerClick(pointer, newId, layer->GetName(), &layerPoint, &layerSize, keys);

				return retVal;
			}
			else
			{
				return false;
			}
		}

		bool OdDockletInstance::LeftButtonClick(POINT* ptCursor,SIZE* sizeDocklet)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			if(drawPlugin)
				drawPlugin->Event(EVENT_ONLEFTCLICK);

			POINT p = *ptCursor;
			SIZE  s = *sizeDocklet;
			//if(drawShadow)
			//{
				s.cx = width;
				s.cy = height;
			//}

			CorrectPoint(p.x, p.y);

			UINT newLayerId = HitTestLayers(p, s, true);
			//if(hWnd && docklet && docklet->LayerClick(

			return hWnd && docklet && docklet->LeftButtonClick(pointer,&p,&s);
		}
		bool OdDockletInstance::DoubleClick(POINT* ptCursor,SIZE* sizeDocklet)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided

			POINT p = *ptCursor;
			SIZE  s = *sizeDocklet;
			if(drawShadow)
			{
				s.cx = width;
				s.cy = height;
			}
			CorrectPoint(p.x, p.y);

			return hWnd && docklet && docklet->DoubleClick(pointer,&p,&s);
		}
		bool OdDockletInstance::LeftButtonHeld(POINT* ptCursor,SIZE* sizeDocklet)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided

			POINT p = *ptCursor;
			SIZE  s = *sizeDocklet;
			if(drawShadow)
			{
				s.cx = width;
				s.cy = height;
			}

			CorrectPoint(p.x, p.y);

			return hWnd && docklet && docklet->LeftButtonHeld(pointer,&p,&s);
		}

		bool OdDockletInstance::RightButtonClick(POINT* ptCursor,SIZE* sizeDocklet, DWORD keys)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			if(drawPlugin)
				drawPlugin->Event(EVENT_ONRIGHTCLICK);

			POINT p = *ptCursor;
			SIZE  s = *sizeDocklet;
			if(drawShadow)
			{
				s.cx = width;
				s.cy = height;
			}

			CorrectPoint(p.x, p.y);

			UINT lid = (UINT)-1;
			Layer* l = NULL;
			if(m_CaptureLayer != (UINT)-1)
				lid = m_CaptureLayer;
			else
				GetLayerAtPoint(*ptCursor, true, &lid);

			l = LayerFromId(lid);
			if(l != NULL)
			{
				POINT lP = p;
				SIZE  lS = s;

				l->ConvertPointAndSize(lP, lS);

				if(hWnd && docklet && docklet->LayerRightClick(pointer, lid, l->GetName(), &lP, &lS, keys))
					return true;
			}
			

			return hWnd && docklet && docklet->RightButtonClick(pointer,&p,&s);
		}

		bool OdDockletInstance::BeforeRemoval()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->BeforeRemoval(pointer);
		}

		bool OdDockletInstance::Configure()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			if(docklet && docklet->HasOnConfigure())
				return false;

			return hWnd && docklet && docklet->Configure(pointer);
		}

		bool OdDockletInstance::HasCustomOnConfigure()
		{
			// pre:  docklet is not NULL
			// post: true has been returned if the docklet has its own OnConfigure 
			//       function
			return docklet && docklet->HasOnConfigure();
		}

		bool OdDockletInstance::AcceptDropFiles()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->AcceptDropFiles(pointer);
		}
		bool OdDockletInstance::DropFiles(HDROP hDrop)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->DropFiles(pointer,hDrop);
		}
		bool OdDockletInstance::ExportFiles(char* fileRelativeOut, int iteration)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->ExportFiles(pointer,fileRelativeOut,iteration);
		}
		bool OdDockletInstance::MouseOn()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->MouseOn(pointer);
		}
		bool OdDockletInstance::MouseOut()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided

			
			bool handled = false;

			if(m_PreviousMouseOverLayer != (UINT)-1 && m_CaptureLayer == (UINT)-1)
			{
				if(m_PreviousMouseOverLayer != (UINT)-1)
				{
					Layer* oldLayer = LayerFromId(m_PreviousMouseOverLayer);
					if(oldLayer != NULL)
					{			
						if(hWnd && docklet)
							handled = docklet->LayerMouseOut(pointer, m_PreviousMouseOverLayer, oldLayer->GetName());
					}
				}
				m_PreviousMouseOverLayer = (UINT) -1;
			}

			if(!handled)
				handled = hWnd && docklet && docklet->MouseOut(pointer);

			return handled;
		}
		bool OdDockletInstance::MouseMove(int x, int y)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided

			POINT p = {x, y};
			SIZE  s = GetSize();

			long lx = x;
			long ly = y;
			CorrectPoint(lx, ly);
			x = lx;
			y = ly;

			bool handled = false;

			if(m_CaptureLayer != (UINT)-1)
			{				
				Layer* layer = LayerFromId(m_CaptureLayer);
				if(layer != NULL)
				{
					POINT layerPoint = {x, y};
					SIZE  layerSize  = s;
					layer->ConvertPointAndSize(layerPoint, layerSize);
					handled = docklet->LayerMouseMove(pointer, m_CaptureLayer, layer->GetName(), &layerPoint, &layerSize);
				}
			}
			else
			{
				UINT mouseOverLayer = (UINT) -1;
				Layer* layer = GetLayerAtPoint(p, true, &mouseOverLayer);

				if(mouseOverLayer != m_PreviousMouseOverLayer && m_PreviousMouseOverLayer != (UINT)-1)
				{
					Layer* oldLayer = LayerFromId(m_PreviousMouseOverLayer);
					if(oldLayer != NULL)
					{			
						if(hWnd && docklet)
							docklet->LayerMouseOut(pointer, m_PreviousMouseOverLayer, oldLayer->GetName());
					}

					m_PreviousMouseOverLayer = (UINT) -1;
				}

				if(layer != NULL)
				{
					POINT layerPoint = {x, y};
					SIZE  layerSize  = s;
					layer->ConvertPointAndSize(layerPoint, layerSize);

					if(mouseOverLayer != m_PreviousMouseOverLayer)
					{
						// don't forget to handle the mouseOut if we hovered over another layer
						m_PreviousMouseOverLayer = mouseOverLayer;
						handled = hWnd && docklet && docklet->LayerMouseOn(pointer, mouseOverLayer, layer->GetName(), &layerPoint, &layerSize);
					}
					else
					{
						handled = hWnd && docklet && docklet->LayerMouseMove(pointer, mouseOverLayer, layer->GetName(), &layerPoint, &layerSize);
					}
				}

				m_PreviousMouseOverLayer = mouseOverLayer;
			}

			if(!handled)
			handled =  hWnd && docklet && docklet->MouseMove(pointer,x,y);

			return handled;
		}

		bool OdDockletInstance::DragDropDataRequest(FORMATETC* format, STGMEDIUM* medium)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->DragDropDataRequest(pointer,format,medium);
		}
		bool OdDockletInstance::MouseWheelForward(WORD fKeysDown, int numOfScrolls, int x, int y)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided

			POINT p = {x, y};
			SIZE  s = GetSize();

			long lx = x;
			long ly = y;
			CorrectPoint(lx, ly);
			x = lx;
			y = ly;

			bool handled = false;

			if(m_CaptureLayer != (UINT)-1)
			{
				Layer* layer = LayerFromId(m_CaptureLayer);
				if(layer != NULL)
				{
					POINT layerPoint = {x, y};
					SIZE  layerSize  = s;
					layer->ConvertPointAndSize(layerPoint, layerSize);
					handled = docklet->LayerMouseWheelForward(pointer, m_CaptureLayer, layer->GetName(), &layerPoint, &layerSize, fKeysDown, numOfScrolls);
				}
			}
			else
			{
				UINT mouseWheelLayer  = (UINT)-1;
				Layer* layer = GetLayerAtPoint(p, true, &mouseWheelLayer);
				if(layer != NULL)
				{
					POINT layerPoint = p;
					SIZE  layerSize  = s;
					layer->ConvertPointAndSize(layerPoint, layerSize);
					handled = hWnd && docklet && docklet->LayerMouseWheelForward(pointer, mouseWheelLayer, layer->GetName(), &layerPoint, &layerSize, fKeysDown, numOfScrolls);
				}	
			}

			if(!handled)
				return hWnd && docklet && docklet->MouseWheelForward(pointer,fKeysDown,numOfScrolls,x,y);
			else
				return true;
		}

		bool OdDockletInstance::MouseWheelBackward(WORD fKeysDown, int numOfScrolls, int x, int y)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided

			POINT p = {x, y};
			SIZE  s = GetSize();

			long lx = x;
			long ly = y;
			CorrectPoint(lx, ly);
			x = lx;
			y = ly;

			bool handled = false;

			if(m_CaptureLayer != (UINT)-1)
			{
				Layer* layer = LayerFromId(m_CaptureLayer);
				if(layer != NULL)
				{
					POINT layerPoint = {x, y};
					SIZE  layerSize  = s;
					layer->ConvertPointAndSize(layerPoint, layerSize);
					handled = docklet->LayerMouseWheelBackward(pointer, m_CaptureLayer, layer->GetName(), &layerPoint, &layerSize, fKeysDown, numOfScrolls);
				}
			}
			else
			{
				UINT mouseWheelLayer  = (UINT)-1;
				Layer* layer = GetLayerAtPoint(p, true, &mouseWheelLayer);
				if(layer != NULL)
				{
					POINT layerPoint = p;
					SIZE  layerSize  = s;
					layer->ConvertPointAndSize(layerPoint, layerSize);
					handled = hWnd && docklet && docklet->LayerMouseWheelBackward(pointer, mouseWheelLayer, layer->GetName(), &layerPoint, &layerSize, fKeysDown, numOfScrolls);
				}	
			}

			if(!handled)
				return hWnd && docklet && docklet->MouseWheelBackward(pointer,fKeysDown,numOfScrolls,x,y);
			else
				return true;
		}

		bool OdDockletInstance::SetSkinW(ENUMERATOR_DATA* ed, const SkinInfo* info)
		{
			return hWnd && docklet && docklet->SetSkinW(ed,info);
		}

		bool OdDockletInstance::DragEnter(IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			
			CorrectPoint(pnt.x, pnt.y);


			return hWnd && docklet && docklet->DragEnter(pointer,obj,keys,pnt,effect);
		}

		int OdDockletInstance::AddPropertySheets(int numPresent)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->AddPropertySheets(pointer,numPresent);
		}

		bool OdDockletInstance::DragOver(DWORD keys,POINTL pnt,DWORD* effect)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			
			CorrectPoint(pnt.x, pnt.y);

			return hWnd && docklet && docklet->DragOver(pointer,keys,pnt,effect);
		}

		bool OdDockletInstance::DragLeave()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			return hWnd && docklet && docklet->DragLeave(pointer);
		}

		bool OdDockletInstance::Drop(IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect)
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the function is handled by the docklet when provided
			
			CorrectPoint(pnt.x, pnt.y);

			return hWnd && docklet && docklet->Drop(pointer,obj,keys,pnt,effect);
		}

		void OdDockletInstance::Destroy()
		{
			// pre:  docklet is not NULL and hWnd is not 0
			// post: the docklet is destroyed and the docklet-pointer is set to NULL
			//		  also the docklets window is destroyed


			if(hWnd && docklet)
			{
				delete drawPlugin;
				drawPlugin = 0;

				docklet->Destroy(pointer,hWnd);

				// destroy the window and set hWnd to 0 indicating that this docklet
				// is destroyed
				//DestroyWindow(hWnd);
				hWnd = 0;

				// set pointer to NULL
				pointer = 0;

				for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++iter)
					delete *iter;

				layers.clear();
			}
		}

		#define COPYMEMBER(n) this->n = that->n;

		void OdDockletInstance::CopyValues(OdDockletInstance* that)
		{
			COPYMEMBER(Alpha);
			COPYMEMBER(colorMatrix);
			COPYMEMBER(drawShadow);
			COPYMEMBER(effectFile);
			COPYMEMBER(height);
			COPYMEMBER(hue);
			COPYMEMBER(quality);
			COPYMEMBER(rotation);
			COPYMEMBER(saturation);
			COPYMEMBER(shadowX);
			COPYMEMBER(shadowY);
			COPYMEMBER(shadowGray);
			COPYMEMBER(shadowColor);
			COPYMEMBER(shadowAlpha);
			COPYMEMBER(useColorization);
			COPYMEMBER(useSaturation);
			COPYMEMBER(width);
			COPYMEMBER(xPos);
			COPYMEMBER(yPos);
		}

		void OdDockletInstance::SetPointer(DOCKLET_DATA *p)
		{
			// post: pointer = p

			pointer = p;
		}

		HWND OdDockletInstance::CreateDockletWindow(WNDPROC wndProc, HINSTANCE hInstance, HWND parent)
		{
			// pre:  wndProc and hInstance are not NULL
			// post: a handle to the window that is created will be returned

			// NOTE: this should create a layered window, but my MS VC 6.0 doesn't know
			//		 that API
			
			// NOTE: the x,y, cx and cy of the CreateWindow() function should be determined
			//		 elsewhere instead of being hardcoded
	
			#define WS_EX_LAYERED 0x00080000 
	
			if(!wndProc || !hInstance)return 0;
			
			
			// register the class
			WNDCLASSEX  c;
			ZeroMemory(&c,sizeof(c));
			c.cbSize = sizeof(WNDCLASSEX);
			c.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
			c.lpfnWndProc = wndProc;
			c.hInstance = hInstance;
			c.lpszMenuName = 0;
			c.lpszClassName = _T("DockItemClass");
			c.hCursor = LoadCursor(0,IDC_ARROW);
			
	
			RegisterClassEx(&c);
	
			return CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED,_T("DockItemClass"),_T("A Docklet"),
				WS_POPUP | WS_OVERLAPPED,0,0,192,192,parent,0,hInstance,0);
		}

		bool OdDockletInstance::IsActivated()
		{
			// post: true has been returned if this docklet is actived, otherwise false has
			//		 been returned
			return hWnd!=0;
		}

		void OdDockletInstance::Activate(const char* iniFile, const char* iniGroup,HWND hwnd, HINSTANCE hInstance)
		{
			// pre:  hwnd is not 0 and the docklet is not yet activated
			// post: this docklet is activated

			if(!hWnd && hwnd)
			{
				// this docklet is not yet activated, so we can activate it
				hWnd  = hwnd;

				// the window is created, so call the Docklets OnCreate ...
				pointer = docklet->OnCreate(hWnd,(hInstance)?hInstance:docklet->module,iniFile,iniGroup);
			}
		}

		HMODULE OdDockletInstance::GetModule()
		{
			// pre:  docklet is not NULL
			// post: the HMODULE from this docklet's DLL is returned
			if(docklet)
				return docklet->module;
			else
				return 0;
		}

		BYTE OdDockletInstance::GetAlpha()
		{
			// post: Alpha has been returned
			return Alpha;
		}

		void OdDockletInstance::SetAlpha(BYTE value, bool redraw)
		{
			// post: Alpha has been set to value and the docklet is redrawn
			//			if redraw is true
			Alpha = value;

			// NO SET IS DIRTY
			//if(redraw)Redraw();
		}

		void OdDockletInstance::SetPos(int x, int y, bool redraw)
		{
			// post: xPos = x, yPos = y and the docklet is redrawn
			//			if redraw is true

			if(x != xPos || y != yPos)
			{
				xPos = x;
				yPos = y;

				BOOL isHandled=FALSE;
				Message(WM_MOVE,0,MAKEWORD(xPos,yPos),isHandled);
			}
			
			//if(redraw)
			//	Redraw();
		}

		void OdDockletInstance::ResizeCachedBitmap()
		{
			if(!isDirty && m_CachedBitmap.get() != NULL)
				return;

			int w = width;
			int h = height;
			int left = 0;
			int	top = 0;

			// adjust the sizes when we need to draw a shadow.
			if(drawShadow)
			{
				w += (shadowX+20);
				h += (shadowY+20);
				left += Layer::ratio/10;
				top += Layer::ratio/10;
			}

		
			// create a temp-bitmap where we drawn upon.
			boost::shared_ptr<Bitmap> bmp( new Bitmap(w,h) );
			Graphics g(bmp.get());
			ApplyQualityTo(&g);

			g.DrawImage(m_CachedBitmap.get(),left,top,w,h);

			m_CachedBitmap = bmp;

			isDirty = false;
		}

		void OdDockletInstance::SetSize(int w, int h, bool redraw)
		{
			// post: width = w, height =h and the docklet is redawn
			//			if redraw is true

			if( w != width || h != height)
			{
				width = w;
				height = h;
				BOOL isHandled=FALSE;
				Message(WM_SIZE,0,MAKEWORD(width,height),isHandled);
				SetIsDirty(true);
				// needs more thought
				//ResizeCachedBitmap();
			}

			//if(redraw)Redraw();
		}

		SIZE OdDockletInstance::GetSize()
		{
			// post: the size of this docklet has been returned
			SIZE s = {width,height};
			return s;
		}

		POINT OdDockletInstance::GetPos()
		{
			// post: the position of this docklet has been returned
			POINT p = {xPos,yPos};
			return p;
		}

		int OdDockletInstance::GetRotation()
		{
			// post: rotation has been returned
			return rotation;
		}

		void OdDockletInstance::SetRotation(int r, bool redraw)
		{
			// post: rotation is set to r and the docklet is redrawn if
			//			redraw is true

			if(SupportsFeature(DESKLET_NO_ROTATION))
				r = 0;

			if(r != rotation)
			{
				rotation = r;
				InvalidateAll();
				SetIsDirty(true);
			}
			//if(redraw)Redraw();
		}

		void OdDockletInstance::GetInformation(char *szName,char *szAuthor,int *lpiVersion,char *szNotes)
		{
			// post: the function is handled
			if(docklet)
			{
				docklet->GetInformation(szName,szAuthor,lpiVersion,szNotes);
			}
		}

		void OdDockletInstance::SetSaturation(bool use, REAL val,bool redraw)
		{
			// post: useSaturation is set to use, saturation to val
			//			and the docklet is redrawn if the redraw is true

			useSaturation	= use;
			saturation		= val;
			InvalidateAll();
			SetIsDirty(true);
			//if(redraw)Redraw();
		}

		REAL OdDockletInstance::GetSaturation()
		{
			// post: saturation has been returned
			return saturation;
		}

		bool OdDockletInstance::GetUseSaturation()
		{
			// post: useSaturation has been returned
			return useSaturation;
		}

		void OdDockletInstance::ApplyQualityTo(Graphics* g)
		{
			// pre:  g is not NULL
			// post: the quality for this docklet is applied to graphics object g
			if(!g)return;

			switch(quality)
			{
				case QUALITY_LOWEST:
					g->SetSmoothingMode(SmoothingModeNone);
					g->SetInterpolationMode(InterpolationModeLowQuality);
				break;

				case QUALITY_LOW:
					g->SetSmoothingMode(SmoothingModeHighSpeed);
					g->SetInterpolationMode(InterpolationModeHighQuality);
				break;

				default: // DEFAULT TOO!
				case QUALITY_AVERAGE:
					g->SetSmoothingMode(SmoothingModeHighQuality);
					g->SetInterpolationMode(InterpolationModeHighQuality);
				break;

				case QUALITY_GOOD:
					g->SetSmoothingMode(/*SmoothingModeAntiAlias8x4*/SmoothingModeAntiAlias);
					g->SetInterpolationMode(InterpolationModeNearestNeighbor);
				break;

				case QUALITY_BEST:
					g->SetSmoothingMode(/*SmoothingModeAntiAlias8x8*/SmoothingModeAntiAlias);
					g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
				break;
			}

		}

		void OdDockletInstance::SetQuality(int val, bool redraw)
		{
			// pre:   QUALITY_LOWEST <= val <= QUALITY_BEST
			// post: quality has been set to val and the docklet is redrawn
			//		  if redraw is true

			if(val>= QUALITY_LOWEST && val <= QUALITY_BEST)
			{
				quality=val;
				InvalidateAll();
				SetIsDirty(true);
				//if(redraw)Redraw();
			}
		}

		int OdDockletInstance::GetQuality()
		{
			// post: quality has been returned
			return quality;
		}

		void OdDockletInstance::SetShadow(bool useShadow, int xOffset, int yOffset, REAL gray,
			INT alpha, LONG color, bool redraw)
		{
			// post: all shadow parameters are updated and the docklet
			//		 is redrawn if dredraw is true

			drawShadow	= useShadow;
			shadowX		= xOffset;
			shadowY		= yOffset;
			shadowGray	= gray;
			shadowAlpha	= alpha;
			shadowColor = color;

			InvalidateAll();
			SetIsDirty(true);

			//if(redraw)Redraw();
		}

		bool OdDockletInstance::GetDrawShadow()
		{
			// post: drawShadow has been returned
			return drawShadow;
		}

		int  OdDockletInstance::GetShadowXOffset()
		{
			// post: shadowX has been returned
			return shadowX;
		}

		int  OdDockletInstance::GetShadowYOffset()
		{
			// post: shadowY has been returned
			return shadowY;
		}

		REAL OdDockletInstance::GetShadowGray()
		{
			// post: shadowGray has been returned
			return shadowGray;
		}

		INT OdDockletInstance::GetShadowAlpha()
		{
			// post: shadowAlpha has been returned
			return shadowAlpha;
		}

		LONG OdDockletInstance::GetShadowColor()
		{
			// post: shadowColor has been returned
			return shadowColor;
		}

		int  OdDockletInstance::GetHue()
		{
			// post: hue has been returned
			return hue;
		}

		void OdDockletInstance::SetColorization(bool use, REAL satVal, int hueVal, bool redraw)
		{
			// post: useColorization has been set to use, saturation has been set to satVal,
			//		 hue has been set to hueVal, and the docklet is redrawn if redraw is true
			useColorization	= use;
			saturation		= satVal;
			hue				= hueVal;

			colorMatrix = ColorMatrixHelper::GetIdentityMatrix();
			colorMatrix = ColorMatrixHelper::SetSaturation(colorMatrix,saturation);
			colorMatrix = ColorMatrixHelper::SetHue(colorMatrix,(REAL)hue);

			SetIsDirty(true);

			//if(redraw)Redraw();
		}

		bool OdDockletInstance::GetUseColorization()
		{
			// post: useColorization has been returned
			return useColorization;
		}

		bool OdDockletInstance::GetModel(IDispatch **pVal)
		{
				// post: the function is handled
				if(docklet)
				{
					return docklet->GetModel(pointer, pVal);
				}
				else
				{
					return false;
				}
		 }

		bool OdDockletInstance::GetFlags(DWORD& flags)
		{
			if(hWnd && docklet)
			{
				return docklet->GetFlags(pointer, &flags);
			}
			else
			{
				return false;
			}
		}



		bool OdDockletInstance::VisibilityNotify(BOOL becomesVisible, DWORD flags)
		{
			// post: the function is handled
			return hWnd && docklet && docklet->VisibilityNotify(pointer, becomesVisible, flags);
		}

		void OdDockletInstance::SetSelected(bool val)
		{
			// post:  isSelected has been set to val
			isSelected = val;					
		}

		void OdDockletInstance::SetMouseOver(bool val)
		{
			// post:  isMouseOver has been set to val
			if(val != isMouseOver && drawPlugin != NULL)
			{
				isMouseOver = val;
				drawPlugin->Event(val ? EVENT_ONMOUSEON : EVENT_ONMOUSEOUT);
			}
		}

		DrawPluginInstance* OdDockletInstance::GetDrawPlugin()
		{
			// post:  drawPlugin has been returned
			return drawPlugin;
		}

		void OdDockletInstance::SetDrawPlugin(DrawPluginInstance* plugin)
		{
			// post:  drawPlugin has been set to plugin
			drawPlugin = plugin;


			if(!drawPlugin)
				effectFile = L"";
			else 
				effectFile = StringToWString(drawPlugin->GetPlugin()->GetFilename());
		}

		void OdDockletInstance::Activate(ILoader* loader,HWND hwnd, HINSTANCE hInstance, DeskletSkinEnumerator* ed, const SkinInfoWrapper* skinInfo)
			{
				// pre:  hwnd is not 0 and the docklet is not yet activated
				// post: this docklet is activated

				//OdDockletInstance* d = this;

				char* iniFile = NULL;
				char* iniSection = NULL;

				char _iniFile[MAX_PATH+1] = {0};
				char _iniSection[MAX_PATH]  = {0};

				if(!hWnd && hwnd)
				{
					// this docklet is not yet activated, so we can activate it
					hWnd  = hwnd;

					if(loader)
					{
						Load(*loader);

						char _iniFile[MAX_PATH+1] = {0};
						GetTempPathA(MAX_PATH,_iniFile);
						

						std::stringstream ws ;
						ws << this << GetTickCount(); // take pointer value is a unique value for the unique filename
									// to avoid race-conditions.
						PathAppendA(_iniFile,ws.str().c_str() );

						//GetTempFileNameA(tmpPath,"ave",(UINT)(this),_iniFile);

						strcpy(_iniSection,"TMP");

						iniFile = _iniFile;
						iniSection = _iniSection;

						//MessageBoxA(0,iniFile,"file",0);

						std::wstring ownerSettings;
						loader->LoadString(ownerSettings,L"ownerSettings");
						HANDLE h = CreateFileA(iniFile,GENERIC_WRITE, FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						if(INVALID_HANDLE_VALUE != h)
						{
							//CRITICAL_SECTION cs = {0};
							//InitializeCriticalSection(&cs);
							//EnterCriticalSection(&cs);
							char* tmp = new char[ownerSettings.length() + 2];
							memset(tmp,0,sizeof(char) * (ownerSettings.size()+2));
							WideCharToMultiByte(GetACP(),0,ownerSettings.c_str(),-1,tmp,ownerSettings.length()+1,NULL,NULL);
							//MessageBoxA(NULL,tmp,0,0);
							//fwrite((void*)tmp,ownerSettings.length(),1,f);
							DWORD numWritten(0);
							WriteFile(h,static_cast<LPCVOID>(tmp),ownerSettings.length(),&numWritten,NULL);

							//LeaveCriticalSection(&cs);
							//DeleteCriticalSection(&cs);

							delete[] tmp;

							CloseHandle(h);
						}
					}

					if(ed && skinInfo)
						pointer = docklet->CreateFromSkinW(hWnd,ed->GetPointer(),skinInfo);
					else
						pointer = docklet->OnCreate(hWnd,(hInstance)?hInstance:docklet->module,iniFile,iniSection);

					if(effectFile != L"")
					{
						//CRITICAL_SECTION cs= {0};
						//InitializeCriticalSection(&cs);
						//EnterCriticalSection(&cs);
						char dll[MAX_PATH+1] = {0};
						WideCharToMultiByte(GetACP(),0,effectFile.c_str(),-1,dll,MAX_PATH,0,0);

						char path[MAX_PATH] = {0};
						GetDLLPath(GetModuleHandle(0),path,MAX_PATH);
						strcat(path,dll);
						DrawPlugin* plugin = DrawPlugin::LoadPlugin(path);
						if(plugin)
						{
							SetDrawPlugin(new DrawPluginInstance(plugin,hWnd,iniFile,iniSection));
							plugin->Release();
						}
						//LeaveCriticalSection(&cs);
						//DeleteCriticalSection(&cs);
					}
					else
						drawPlugin = NULL;


					if(iniFile)
						DeleteFileA(iniFile);
				}
			}


			std::wstring OdDockletInstance::GetIconFileName()
			{
				std::vector<Layer*>& layers = GetLayers();
				for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++ iter)
				{
					Layer* l = *iter;
					if(l->GetVisibility() && l->GetCanBeIcon())
					{
						WCHAR img[MAX_PATH+1] = {0};
						MultiByteToWideChar(GetACP(),0,l->GetFileName(),-1,img,MAX_PATH);
						return img;
					}
				}
				return L"";
			}

			void OdDockletInstance::SetIconFileName(std::wstring filename)
			{

				std::vector<Layer*>& layers = GetLayers();
				for(std::vector<Layer*>::iterator iter = layers.begin(); iter != layers.end(); ++ iter)
				{
					Layer* l = *iter;
					if(l->GetVisibility() && l->GetCanBeIcon())
					{
						char img[MAX_PATH+1] = {0};
						WideCharToMultiByte(GetACP(),0,filename.c_str(),-1,img,MAX_PATH,0,0);
						l->SetFileName(img);
						if(img[0] != '\0')
						l->SetImage(DeskletLoadGDIPlusImageW(filename.c_str()),false);
						return;
					}
				}
			}


			HRESULT OdDockletInstance::Load(ILoader& loader)
			{
				USES_CONVERSION;
				std::wstring _tmp;

				LOADABLE_PRIMITIVE(Alpha);
				LOADABLE_PRIMITIVE(xPos);
				LOADABLE_PRIMITIVE(yPos);
				LOADABLE_PRIMITIVE(width);
				LOADABLE_PRIMITIVE(height);
				LOADABLE_PRIMITIVE(rotation);
				LOADABLE_PRIMITIVE(useColorization);
				LOADABLE_PRIMITIVE(saturation);
				LOADABLE_PRIMITIVE(hue);
				LOADABLE_PRIMITIVE(quality);
				LOADABLE_PRIMITIVE(drawShadow);
				LOADABLE_PRIMITIVE(shadowX);
				LOADABLE_PRIMITIVE(shadowY);
				LOADABLE_PRIMITIVE(shadowGray);
				LOADABLE_PRIMITIVE(shadowColor);
				LOADABLE_PRIMITIVE(shadowAlpha);

				colorMatrix = ColorMatrixHelper::GetIdentityMatrix();
				colorMatrix = ColorMatrixHelper::SetSaturation(colorMatrix,saturation);
				colorMatrix = ColorMatrixHelper::SetHue(colorMatrix,(REAL)hue);

				LOADABLE_PRIMITIVE(effectFile);

				std::wstring iconFileName; 
				LOADABLE_PRIMITIVE_NAMED(iconFileName,Image);
				SetIconFileName(iconFileName);

				return S_OK;
			}

			HRESULT OdDockletInstance::Save(ISaver& saver)
			{
				USES_CONVERSION;
				std::wstring _tmp;
				
				SAVABLE_PRIMITIVE(Alpha);
				SAVABLE_PRIMITIVE(xPos);
				SAVABLE_PRIMITIVE(yPos);
				SAVABLE_PRIMITIVE(width);
				SAVABLE_PRIMITIVE(height);
				SAVABLE_PRIMITIVE(rotation);
				SAVABLE_PRIMITIVE(useColorization);
				SAVABLE_PRIMITIVE(saturation);
				SAVABLE_PRIMITIVE(hue);
				SAVABLE_PRIMITIVE(quality);
				SAVABLE_PRIMITIVE(drawShadow);
				SAVABLE_PRIMITIVE(shadowX);
				SAVABLE_PRIMITIVE(shadowY);
				SAVABLE_PRIMITIVE(shadowGray);
				SAVABLE_PRIMITIVE(shadowColor);
				SAVABLE_PRIMITIVE(shadowAlpha);

				if(drawPlugin)
				{
					char path[MAX_PATH] = {0};
					GetDLLPath(GetModuleHandle(0),path,sizeof(path)/sizeof(path[0]));

					char effectPath[MAX_PATH] = {0};
					strcpy(effectPath,drawPlugin->GetPlugin()->GetFilename().c_str());
					strcpy(effectPath,effectPath+strlen(path));

					WCHAR dll[MAX_PATH+1] = {0};
					MultiByteToWideChar(GetACP(),0,	effectPath,-1,dll,MAX_PATH);


					effectFile = dll;
				}

				std::wstring iconFileName = GetIconFileName(); 
				SAVABLE_PRIMITIVE_NAMED(iconFileName,Image);

				SAVABLE_PRIMITIVE(effectFile);

				std::wstring ownerSettings;

				
				char iniFile[MAX_PATH+1] = {0};
				char tmpPath[MAX_PATH+1] = {0};
				GetTempPathA(MAX_PATH,iniFile);

				std::stringstream ws;
				ws << this << GetTickCount(); // use this pointer value as a unique name.

				PathAppendA(iniFile,ws.str().c_str() );

				//GetTempFileNameA(tmpPath,"ave",(DWORD)(this),iniFile);

				char* iniSection = "TMP";

				Save(iniFile,iniSection,false);
				if(drawPlugin)
					drawPlugin->Save(iniFile,iniSection);

				HANDLE h = CreateFileA(iniFile,GENERIC_READ, FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				//FILE* f = fopen(iniFile,"r");
				if(INVALID_HANDLE_VALUE != h)
				{ 
					DWORD dwSize = GetFileSize(h,NULL);
					if(dwSize > 0)
					{
						char* buf = new char[dwSize+2];
						memset((void*)buf,0,(dwSize+2) * sizeof(char));

						DWORD dwNumRead(0);
						ReadFile(h,static_cast<LPVOID>(buf),dwSize, &dwNumRead,NULL);

						WCHAR* wBuf = new WCHAR[dwSize+2];
						memset((void*)wBuf,0,(dwSize+2) * sizeof(WCHAR));
						MultiByteToWideChar(GetACP(),0,buf,-1,wBuf,dwSize+2);
						ownerSettings = wBuf;

						delete[] buf;
						delete[] wBuf;

					}
					else
					{
						ownerSettings = L"";
					}

					CloseHandle(h);
					DeleteFileA(iniFile);

				}
				SAVABLE_PRIMITIVE(ownerSettings);

				return S_OK;

			}

			LRESULT OdDockletInstance::Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& isHandled)
			{
				if(hWnd && docklet)
				{
					if(docklet->OnProcessMessageEx)
					{
						return ProcessMessageEx(uMsg,wParam,lParam,isHandled);
					}
					else
					{
						isHandled = FALSE;
						ProcessMessage(uMsg,wParam,lParam);
						return 0;
					}
				}
				else
				{
					isHandled = FALSE;
					return 0;
				}
			}
};