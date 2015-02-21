// filename:		layer.h
// Description:		Definition of class Layer
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!


#ifndef LAYER_H_303
#define LAYER_H_303

#include <windows.h>
#include <gdiplus.h>
#include "helper_funcs.h"
#include "olayer.h"
using namespace Gdiplus;

#include <list>
#include <vector>

class Layer : 
  public CoElement<Layer, CoLayer, ILayer>
{
	protected:
		Image* image;
		BOOL ownsImage;

		ColorMatrix cm;
		bool useColorMatrix;
		
		int rotation;
		BYTE alpha;

		int hue;
		int saturation;

		bool isVisible;

		char fileName[MAX_PATH];

		char name[MAX_PATH+MAX_PATH];

		bool usePlacement;
		int x;
		int y;
		int swidth;
		int sheight;

		int scaleX;
		int scaleY;

		bool isCanBeIcon;

		bool dropShadow;
		int shadowX;
		int shadowY;
		BYTE shadowAlpha;

		Bitmap* shadowBmp;

		bool isNotMovable;
		bool noAlphaBlendedHitTesting;
		bool isClickThrough;

		bool useImageSourceBounds;
		RECT imageSourceBounds;
		SIZE imageSourceBoundsScale;
	
		virtual void DrawPartialRotation(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
			std::list<RECT>& rects);

		Layer* rotatedLayer;
		int prevRotation;
		bool prevUseImageSourceBounds;
		RECT prevImageSourceBounds;
		SIZE prevImageSourceBoundsScale;

		Layer* parent;
		std::vector<Layer*> subLayers;

		PointF extraOffset;

	public:

		Layer(const char* name, Layer* parent = NULL);
		virtual ~Layer();

		virtual void SetUseImageSourceBounds(bool use, const RECT& r, const SIZE& s);
		virtual void GetUseImageSourceBounds(bool& use, RECT& r, SIZE& s);
		virtual RectF GetImageRect()const;

		virtual void SetExtraOffset(const PointF& pf){extraOffset=pf;};
		virtual PointF GetExtraOffset()const{return extraOffset;};

		virtual bool GetUsePlacement()const
		{
			return usePlacement;
		};
		virtual RECT GetPlacementRect()const
		{
			RECT r = {x, y, x + swidth, y + sheight};
			return r;
		}
		virtual SIZE GetPlacementScale()const
		{
			SIZE s = {scaleX, scaleY};
			return s;
		}

		virtual bool CanBeDrawn()const
		{
			return image != NULL;
		}

		virtual BYTE GetAlpha()const{return alpha;};
		virtual void GetColorMatrix(ColorMatrix& matrix){matrix = cm;};
		virtual int GetHue()const{return hue;};
		virtual int GetSaturation()const{return saturation;};

		virtual Layer* GetParent(){return parent;};

		virtual BOOL IsVisible();

		virtual void SetImage(Image* bmp, bool clone);
		virtual void SetColorMatrix(ColorMatrix* m);
		virtual void Draw(Graphics* g, int left, int top, int width, int height,ColorMatrix* oldMatrix);
		virtual void DrawShadow(Graphics* g,int left, int top,int width, int height, REAL grayshade, REAL alpha, LONG color);

		virtual void SetVisibility(bool val);
		virtual void SetRotation(int val);
		virtual void SetAlpha(int alpha);

		virtual void  SetFileName(const char* filename);
		virtual const char* GetFileName();

		virtual bool GetVisibility();

		virtual void SetHue(int val);
		virtual void SetSaturation(unsigned int val);

		virtual void SetPlacement(bool use, RECT* size, int xScale, int yScale);

		virtual const char* GetName(){return name;};
	

		virtual void SetIsNotMovable(bool val){isNotMovable = val;};
		virtual bool GetIsNotMovable()const{ return isNotMovable;};

		virtual void SetIsClickThrough(bool val){isClickThrough = val;};
		virtual bool GetIsClickThrough()const{ return isClickThrough;};

		virtual void SetNoAlphaBlendedHitTesting(bool val) { noAlphaBlendedHitTesting = val;};
		virtual bool GetNoAlphaBlendedHitTesting()const { return noAlphaBlendedHitTesting;};

		virtual void SetCanBeIcon(bool val);
		virtual bool GetCanBeIcon();

		virtual void SetShadow(bool val, int sX, int sY, int sA);
		virtual void DeleteCachedShadow();

		virtual bool CanBeDrawnDirectly(UINT w, UINT h)
		{
			return image && !usePlacement && !dropShadow && alpha == 255 &&
					isVisible && rotation == 0 && hue == 0 && saturation == 100 &&
						image->GetWidth() == w && image->GetHeight() == h
						&& (!useImageSourceBounds || GetImageRect().Equals(RectF(0.0f,0.0f, static_cast<float>(image->GetWidth()), static_cast<float>(image->GetHeight())))) ;
		}

		virtual Bitmap* GetBitmap()
		{
			return (Bitmap*)image;
		}

		virtual SIZE GetImageSize()const;

		virtual int GetRotation()const{return rotation;};

		static int ratio;

		virtual bool HitTest(const POINT& point, const SIZE& size);
		virtual void ConvertPointAndSize(POINT& point, SIZE& size, bool recursive=true);

		virtual BOOL GetHitTestedLayer(const POINT& point, const SIZE& size, Layer*& hitLayer, bool mustBeVisible);

		virtual void DrawPartial(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
			std::list<RECT>& rects);

		virtual void DrawShadowPartial(Graphics* g,int left, int top,int width, int height, REAL grayshade, REAL alpha, LONG color,
							  std::list<RECT>& rects);

		virtual void AddChild(Layer* l);
		virtual BOOL IsContainer()const{return FALSE;};

		virtual std::vector<Layer*>& GetSubLayers(){return subLayers;};

};

#endif//LAYER_H_303