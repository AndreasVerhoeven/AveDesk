// filename:		layer.cpp
// Description:		Implementation of class Layer
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "Layer.h"
#include "ColorMatrixHelper.h"
#include "image_funcs.h"

#undef USE_QUICK_FIX

int Layer::ratio = 40;

Layer::~Layer()
{
	// post: this layer object has been destroyed
	delete shadowBmp;
	if(ownsImage)
		delete image;

	delete rotatedLayer;
}

Layer::Layer(const char* _name, Layer* _parent) : image(0),
	rotation(0),
	alpha(255),
	hue(0),
	saturation(100),
	isVisible(true),
	useColorMatrix(false),
	usePlacement(false),
	x(0),
	y(0),
	sheight(128),
	swidth(128),
	scaleX(128),
	scaleY(128),
	isCanBeIcon(true),
	dropShadow(false),
	shadowX(4),
	shadowY(4),
	shadowAlpha(225),
	shadowBmp(0),
	isNotMovable(false),
	noAlphaBlendedHitTesting(false),
	isClickThrough(false),
	ownsImage(true),
	useImageSourceBounds(false),
	rotatedLayer(NULL),
	prevRotation(0),
	parent(_parent)
{
	// post: a Layer-object has been created
	strcpy(fileName,"");

	if(_name)
		strcpy(name,_name);
	else
		strcpy(name,"Unnamed");
}

void Layer::AddChild(Layer* l)
{
	subLayers.push_back(l);
}

BOOL Layer::IsVisible()
{
	Layer* l = this;
	while(l != NULL)
	{
		if(!l->GetVisibility())
			return FALSE;

		l = l->GetParent();
	}

	return TRUE;
}

void Layer::ConvertPointAndSize(POINT& point, SIZE& size, bool recursive)
{
	if(recursive)
	{
		if(GetParent() != NULL)
			GetParent()->ConvertPointAndSize(point, size, true);
	}


	if(rotation != 0)
		FixRotation(rotation, size, point);

	if(usePlacement)
	{
		REAL xs = (REAL)size.cx / (REAL)scaleX;
		REAL ys = (REAL)size.cy / (REAL)scaleY;

		RectF rf = RemapRectF(RectF((REAL)x,(REAL)y,(REAL)swidth, (REAL)sheight),size, scaleX, scaleY);

		float xP = rf.Width  / 2.0f + 0 + rf.X;
		float yP = rf.Height / 2.0f + 0 + rf.Y;

		Matrix m;
		m.RotateAt((float)(-rotation % 360),PointF(xP,yP));
		PointF pf((REAL)point.x, (REAL)point.y);
		m.TransformPoints(&pf, 1);
		point.x = (LONG)pf.X;
		point.y = (LONG)pf.Y;

		// old
		SIZE scale = {scaleX, scaleY};
		point = RemapPoint(point, scale, size.cx, size.cy);
		point.x -= x;
		point.y -= y;

		size.cx = swidth;
		size.cy = sheight;
	}
	else
	{
		if(rotation % 360 != 0)
			FixRotation(rotation %360, size, point);
	}
}

void Layer::SetUseImageSourceBounds(bool use, const RECT& r, const SIZE& s)
{
	useImageSourceBounds = use;
	if(useImageSourceBounds)
	{
		imageSourceBounds = r;
		imageSourceBoundsScale = s;
	}
}

void Layer::GetUseImageSourceBounds(bool& use, RECT& r, SIZE& s)
{
	use = useImageSourceBounds;
	r = imageSourceBounds;
	s = imageSourceBoundsScale;
}

SIZE Layer::GetImageSize()const
{
	SIZE imgSize = {0,0};

	if(image != NULL)
	{
		imgSize.cx = image->GetWidth();
		imgSize.cy = image->GetHeight();
	}

	return imgSize;
}

RectF Layer::GetImageRect()const
{
	SIZE imgSize = GetImageSize();
	if(useImageSourceBounds)
		return RemapRectF(RECTTORECTF(imageSourceBounds), imgSize, imageSourceBoundsScale.cx, imageSourceBoundsScale.cy);
	else
		return RectF(0.0f,0.0f, (REAL)imgSize.cx, (REAL)imgSize.cy);
}

BOOL Layer::GetHitTestedLayer(const POINT& point, const SIZE& size, Layer*& hitLayer, bool mustBeVisible)
{
	if(mustBeVisible && !GetVisibility())
		return FALSE;

	if(HitTest(point, size))
	{
		hitLayer = this;
		return TRUE;
	}
	else
	{
		hitLayer = NULL;
		return FALSE;
	}
}


bool Layer::HitTest(const POINT& point, const SIZE& size)
{
	// post: true has been returned iff the layer can be `hit` at point 
	//       when having the  desklet at size

	// a click through layer can never be hit-tested!
	if(GetIsClickThrough())
		return false;

	if(NULL == image)
	{
		return false;
	}

	SIZE s2 = size;
	POINT p = point;

	// first, fix up the rotation of the layer
	if(rotation %360 != 0)
	{
		if(!usePlacement)
		{
			FixRotation(rotation, s2, p);
		}
		else
		{
			REAL xs = (REAL)size.cx / (REAL)scaleX;
			REAL ys = (REAL)size.cy / (REAL)scaleY;

			RectF rf = RemapRectF(RectF((REAL)x,(REAL)y,(REAL)swidth, (REAL)sheight),size, scaleX, scaleY);

			float xP = rf.Width  / 2.0f + 0 + rf.X;
			float yP = rf.Height / 2.0f + 0 + rf.Y;

			Matrix m;
			m.RotateAt((float)(-rotation % 360),PointF(xP,yP));
			PointF pf((REAL)p.x, (REAL)p.y);
			m.TransformPoints(&pf, 1);
			p.x = (LONG)pf.X;
			p.y = (LONG)pf.Y;
		}
	}

	// two possibilities: a stretched layer (!usePlacement) or a placed layer (usePlacement)
	if(!usePlacement)
	{
		// check if the point is inside the layers rectangle
		RECT r = {0,0, size.cx, size.cy};
		if(PtInRect(&r,p))
		{
			RectF imgRect = GetImageRect();
			UINT layerWidth  = (UINT)imgRect.Width;
			UINT layerHeight = (UINT)imgRect.Height;

			SIZE layerSize = { layerWidth, layerHeight };

			// we now assume the desklet is the same size as this layer's image
			// by remapping the point.
			p = RemapPoint(p, layerSize, size.cx, size.cy);

			// check if the remapped point is within the image's boundaries
			if(p.x >= imgRect.X && p.x < (int)layerWidth && p.y >= imgRect.Y && p.y < (int)layerHeight)
			{
				if(noAlphaBlendedHitTesting)
				{
					return true;
				}
				else
				{
					// now, check the pixel at the remapped point in the image.
					// if it's alpha component is bigger than 0, the point was hit.
					// (the alpha needs to be multiplied by the layers overal alpha)
					Color c;
					((Bitmap*)image)->GetPixel(p.x + (INT)imgRect.X, p.y + (INT)imgRect.Y,&c);


					UINT pixelAlpha = c.GetAlpha() * alpha;
					pixelAlpha /= 255;
				
					return pixelAlpha > 0;
				}
			}
			else
			{
				// the point is not inside the layers image!
				return false;
			}
		}
		else
		{
			// the point is not inside the layers rectangle!
			return false;
		}
	}
	else
	{
		// first, the point remapped so that it can be match towards
		// the layers bounding rectangle
		SIZE wantedSize = {scaleX, scaleY};
		p = RemapPoint(p, wantedSize, size.cx, size.cy);
		
		RECT layerRect = {x, y, swidth + x, sheight + y}; 
		if(PtInRect(&layerRect, p))
		{
			// the remapped point is inside the layers bounding rectangle
			// so, it can be set transformed, so that it's relative
			// to this bounding rctangle.
			p.x -= x;
			p.y -= y;

			// because the point is inside the layer's bounding rectangle,
			// it can now be remapped on the layer's image's scale

			RectF imgRect = GetImageRect();

			SIZE transformedSize = {swidth, sheight};
			UINT imgWidth  = (UINT)imgRect.Width;
			UINT imgHeight = (UINT)imgRect.Height;
			SIZE imgSize = {imgWidth, imgHeight};
			p = RemapPoint(p, imgSize, swidth, sheight);//transformedSize, imgWidth, imgHeight);

			// OLD: if(p.x >= imgRect.X && p.x < imgWidth && p.y >= imgRect.Y && p.y < imgHeight)

			// check if the remapped point is within the image's boundaries
			if(p.x >= 0 && p.x < (int)imgWidth && p.y >= 0 && p.y < (int)imgHeight)
			{
				if(noAlphaBlendedHitTesting)
				{
					return true;
				}
				else
				{
					// now, check the pixel at the remapped point in the image.
					// if it's alpha component is bigger than 0, the point was hit.
					// (the alpha needs to be multiplied by the layers overal alpha)
					Color c;
					((Bitmap*)image)->GetPixel(p.x + (INT)imgRect.X, p.y + (INT)imgRect.Y,&c);


					UINT pixelAlpha = c.GetAlpha() * alpha;
					pixelAlpha /= 255;
				
					return pixelAlpha > 0;
				}
			}
			else
			{
				// the point is not inside the layers image!
				return false;
			}
		}
		else
		{
			// the point is not inside the layer's bounding rectangle
			return false;
		}
	}
}

void Layer::SetPlacement(bool use, RECT* size, int xScale, int yScale)
{
	// pre:  if use == true, than size is not 0
	// post: placement options has been set
	usePlacement = use;

	if(usePlacement)
	{
		if(size)
		{
			x = size->left;
			y = size->top;
			swidth = size->right - size->left;
			sheight = size ->bottom - size->top;
		}

		scaleX = xScale;
		scaleY = yScale;
	}
}

void Layer::DrawShadow(Graphics* g,int left, int top,int width, int height, REAL grayshade, REAL alpha, LONG color)
{
	// pre:  g and image are not 0
	// post: a shadow has been drawn
	if(!image || !isVisible || !g)return;

	if(!shadowBmp)
	{
		RectF imgRect = GetImageRect();

		Bitmap tmp(width+20,height+20);
		Graphics tmpG(&tmp);
		tmpG.DrawImage(image,Rect(10,10,width,height),(INT)imgRect.X, (INT)imgRect.Y, (INT)imgRect.Width, (INT)imgRect.Height,UnitPixel,0,0,0);
		shadowBmp = GetBlur(&tmp,4.0f);
	}

	ImageAttributes ia;	
	ColorMatrix cm = ColorMatrixHelper::GetIdentityMatrix();
	cm.m[4][0] = static_cast<float>GetRValue(color)	/ 255;
	cm.m[4][1] = static_cast<float>GetGValue(color)	/ 255;
	cm.m[4][2] = static_cast<float>GetBValue(color)	/ 255;
	cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = grayshade;
	cm.m[3][3] = alpha * ((float)this->alpha/255.0f);
	ia.SetColorMatrix(&cm);

	g->DrawImage(shadowBmp,Rect(left - 10,top-10,width+20,height+20),0,0,shadowBmp->GetWidth(),shadowBmp->GetHeight(),UnitPixel,&ia,0,0);

}

bool Layer::GetVisibility()
{
	// post: isVisible has been returned
	return isVisible;
}

void Layer::SetImage(Image* bmp, bool doesnotOwn)
{
	// post: img has been set to bmp

	delete shadowBmp;
	shadowBmp = 0;

	if(ownsImage)
	{
		delete image;
		image = 0;
	}

	ownsImage = !doesnotOwn;
	image = bmp;

	if(rotatedLayer != NULL)
		rotatedLayer->SetImage(NULL, false);

	/*
	if(clone)
		image = bmp->Clone();
	else
		image = bmp;
	*/
	
}

void Layer::SetColorMatrix(ColorMatrix* m)
{
	// post: cm has been set to m
	if(m)
	{
		cm = *m;
		useColorMatrix = true;
	}
	else
	{
		cm = ColorMatrixHelper::GetIdentityMatrix();
		useColorMatrix = false;
	}
}

void Layer::DrawShadowPartial(Graphics* g,int left, int top,int width, int height, REAL grayshade, REAL alpha, LONG color,
							  std::list<RECT>& rects)
{
	// pre:  g and image are not 0
	// post: a shadow has been drawn
	if(!image || !isVisible || !g)return;

	RectF imgRect = GetImageRect();

	if(!shadowBmp)
	{
		Bitmap tmp(width+20,height+20);
		Graphics tmpG(&tmp);
		tmpG.DrawImage(image,Rect(10,10,width,height),(INT)imgRect.X, (INT)imgRect.Y, (INT)imgRect.Width, (INT)imgRect.Height,UnitPixel,0,0,0);
		shadowBmp = GetBlur(&tmp,4.0f);
	}

	ImageAttributes ia;	
	ColorMatrix cm = ColorMatrixHelper::GetIdentityMatrix();
	cm.m[4][0] = static_cast<float>GetRValue(color)	/ 255;
	cm.m[4][1] = static_cast<float>GetGValue(color)	/ 255;
	cm.m[4][2] = static_cast<float>GetBValue(color)	/ 255;
	cm.m[0][0] = cm.m[1][1] = cm.m[2][2] = grayshade;
	cm.m[3][3] = alpha * ((float)this->alpha/255.0f);
	ia.SetColorMatrix(&cm);

	for(std::list<RECT>::iterator iter = rects.begin(); iter != rects.end(); ++iter)
	{
		RECT r = *iter;

		RectF rf = RECTTORECTF(r);

		SIZE deskletSize = {width, height};
		RectF bounds(0.0f, 0.0f, (REAL)width + 20.0f, (REAL)height + 20.0f);
		//bounds = RemapRectF(bounds, deskletSize, 1.0f, 1.0f);

		RectF intersect(bounds);
		RectF out(intersect);
					
		float lx = intersect.X - bounds.X;
		float ly = intersect.Y - bounds.Y;
		RectF fromRect(lx, ly, out.Width + lx, out.Height + ly);
					
		float rX = static_cast<float>(shadowBmp->GetWidth()) / RemapPointF((float)width+20.0f, width, width);
		float rY = static_cast<float>(shadowBmp->GetHeight()) / RemapPointF((float)height+20.0f, height, height);

		out.X += left - 10;
		out.Y += top - 10;

		RectF in(fromRect);
		in.X *= rX;
		in.Y *= rY;
		in.Width *= rX;
		in.Height *= rY;

		g->DrawImage(shadowBmp,out,in.X,in.Y,in.Width,in.Height,UnitPixel,&ia,0,0);
	}
}

//#define DEBUG_LAYERS
void Layer::DrawPartialRotation(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
						std::list<RECT>& rects)
{
	if(!isVisible || NULL == image || NULL == g || 0 == alpha)
		return;

	if(NULL == rotatedLayer)
		rotatedLayer = new Layer("dummyLayer");

	SizeF sf((REAL)swidth, (REAL)sheight);
	PointF pf((REAL)x, (REAL)y);
	FixRotationF(rotation, sf, pf);

	PointF pfDiff(pf.X - (INT)pf.X, pf.Y - (INT)pf.Y);

	SIZE s = {swidth,sheight};
	POINT p = {x, y};
	FixRotation(rotation,s,p);

	//RECT r= {x - p.x,y - p.y, x - p.x + s.cx+1, y - p.y + s.cy+1};
	//rotatedLayer->SetPlacement(true, &r, width, height);
	RECT r = {0,0, width, height};
	rotatedLayer->SetPlacement(true, &r, width, height);
	rotatedLayer->SetAlpha(alpha);
	rotatedLayer->SetHue(hue);
	rotatedLayer->SetSaturation(saturation);
	if(useColorMatrix)
		rotatedLayer->SetColorMatrix(&cm);
	else
		rotatedLayer->SetColorMatrix(NULL);
	rotatedLayer->SetVisibility(true);
	rotatedLayer->SetUseImageSourceBounds(useImageSourceBounds, imageSourceBounds, imageSourceBoundsScale);

	if(prevRotation != rotation || !rotatedLayer->CanBeDrawn())
	{	
		if(rotation==180)
		{
			int i(0);
		}

		Bitmap* rotatedBitmap = new Bitmap(width, height);
		Graphics rotatedG(rotatedBitmap);
		rotatedG.SetSmoothingMode(g->GetSmoothingMode());
		rotatedG.SetInterpolationMode(g->GetInterpolationMode());
		Draw(&rotatedG, 0, 0, width, height,oldMatrix);

		rotatedLayer->SetImage(rotatedBitmap, false);
		prevRotation = rotation;
		
		/*
		Bitmap* rotatedBitmap = new Bitmap(s.cx+1, s.cy+1);
		Graphics rotatedG(rotatedBitmap);
		rotatedG.SetSmoothingMode(g->GetSmoothingMode());
		rotatedG.SetInterpolationMode(g->GetInterpolationMode());
		Matrix m;
		//m.RotateAt((float)rotation,PointF((float)(s.cx)/2.0f + pfDiff.X,(float)(s.cy)/2.0f + pfDiff.Y));
		PointF pfCentre((float)(sf.Width)/2.0f,(float)(sf.Height)/2.0f);
		m.RotateAt((REAL)(rotation%360), pfCentre);
		rotatedG.SetTransform(&m);


		Bitmap* bmp = (Bitmap*)image;
		float origX = (float)pf.X;//float(s.cx) / 2.0f - float(bmp->GetWidth()) / 2.0f;
		float origY = (float)pf.Y;//float(s.cy) / 2.0f - float(bmp->GetHeight()) / 2.0f;
		// DEBUG: if(_stricmp(name, "sec") != 0)
			rotatedG.DrawImage(bmp, RectF(origX, origY, (REAL)bmp->GetWidth(), (REAL)bmp->GetHeight()), 0.0f, 0.0f, (REAL)bmp->GetWidth(), (REAL)bmp->GetHeight(), UnitPixel, NULL, NULL, NULL);
		// DEBUG: else
		//{
		//	SolidBrush brush(Color(255,255,0,0));
		//	rotatedG.FillRectangle(&brush, RectF(origX, origY, (REAL)bmp->GetWidth(), (REAL)bmp->GetHeight()));
		//}
		*/

		//PointF pfRotationFix((REAL)p.x - pf.X, (REAL)p.y - pf.Y);
		//rotatedLayer->SetExtraOffset(pfRotationFix);
		//
		

	}

	RECT outRect = {x - p.x,y - p.y, x - p.x + s.cx+1, y - p.y + s.cy+1};
	std::list<RECT> corRects;
	for(std::list<RECT>::iterator iter = rects.begin(); iter != rects.end(); ++iter)
	{
		RECT intersect = {0};
		if(::IntersectRect(&intersect, &(*iter), &outRect))
			corRects.push_back(intersect);
	}

	rotatedLayer->DrawPartial(g, left, top, width, height, oldMatrix, corRects);
}

void Layer::DrawPartial(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
						std::list<RECT>& rects)
{
	if(rotation % 360 != 0 || rotatedLayer)
	{
		DrawPartialRotation(g, left, top, width, height, oldMatrix, rects);
		return;
	}

	if(!isVisible || NULL == image || NULL == g || 0 == alpha)
		return;

	Matrix oldTMatrix;
	g->GetTransform(&oldTMatrix);

	if(rotation % 360 !=0)
	{
		Matrix* matrix = oldTMatrix.Clone();
		
		if(!usePlacement)
		{
			matrix->RotateAt((float)rotation,PointF((float)(width)/2+left,(float)(height)/2+top));
		}
		else
		{
			REAL xs = (REAL)width / (REAL)scaleX;
			REAL ys = (REAL)height / (REAL)scaleY;

			SIZE s = {width, height};
			RectF rf = RemapRectF(RectF((REAL)x,(REAL)y,(REAL)swidth, (REAL)sheight),s, scaleX, scaleY);

			float xP = rf.Width  / 2.0f + left + rf.X;
			float yP = rf.Height / 2.0f + top  + rf.Y;

			matrix->RotateAt((float)(rotation % 360),PointF(xP,yP));
		}
	
		g->SetTransform(matrix);

		delete matrix;
	}

	if(alpha!=255 || useColorMatrix || oldMatrix!=0 || hue != 0 || saturation != 100)
	{
		ColorMatrix matrix = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,cm);

		if(oldMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,*oldMatrix);

		if(alpha!=255)
			matrix.m[3][3] *= ((REAL)alpha/255.0f);

		if(hue != 0)
			matrix = ColorMatrixHelper::SetHue(matrix,(REAL)hue);

		if(saturation != 100)
			matrix = ColorMatrixHelper::SetSaturation(matrix,(REAL)saturation/100);

		ImageAttributes attr;
		attr.SetColorMatrix(&matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
		if(usePlacement)
		{
			RectF imgRect = GetImageRect();

			for(std::list<RECT>::iterator iter = rects.begin(); iter != rects.end(); ++iter)
			{
				RECT r = *iter;	
				RectF rf = RECTTORECTF(r);

				SIZE deskletSize = {width, height};
				RectF bounds((REAL)x,(REAL)y, (REAL)swidth, (REAL)sheight);
				bounds = RemapRectF(bounds, deskletSize, scaleX, scaleY);

				if(rotation % 360 != 0)
				{
					SIZE  s = {(LONG)rf.Width, (LONG)rf.Height};
					POINT p = {(LONG)rf.X, (LONG)rf.Y};
					FixRotation(rotation % 360, s, p);
					rf.X = (REAL)p.x;
					rf.Y = (REAL)p.y;
					rf.Width = (REAL)s.cy;
					rf.Height = (REAL)s.cx;
				}


				RectF intersect(rf);
				intersect.Intersect(bounds);
				if(!intersect.IsEmptyArea())
				{
					RectF out(intersect);
					
					float lx = intersect.X - bounds.X;
					float ly = intersect.Y - bounds.Y;
					RectF fromRect(lx, ly, out.Width, out.Height);
				
					float rX = static_cast<float>(imgRect.Width) / RemapPointF((float)swidth, width, scaleX);
					float rY = static_cast<float>(imgRect.Height) / RemapPointF((float)sheight, height, scaleY);

					out.X += (REAL)left + extraOffset.X;
					out.Y += (REAL)top + extraOffset.Y;

#ifdef USE_QUICK_FIX
			if(useImageSourceBounds && !(GetImageRect().X == 0 && GetImageRect().Width == image->GetWidth()))
				out.X++;
			if(useImageSourceBounds && !(GetImageRect().Y == 0 && GetImageRect().Height == image->GetHeight()))
				out.Y++;
#endif//USE_QUICK_FIX

					RectF in(fromRect);
					// in is still at images sizes, so we can directly add
					in.X *= rX;
					in.Y *= rY;
					in.X += imgRect.X;
					in.Y += imgRect.Y;
					in.Width *= rX;
					in.Height *= rY;
#ifdef DEBUG_LAYERS
					SolidBrush red(Color(255,255,0,0));
					g->FillRectangle(&red, out);
					Bitmap bmp2;
#endif // DEBUG_LAYERS
					g->DrawImage(image, out, in.X, in.Y, in.Width, in.Height, UnitPixel, &attr, NULL, NULL);

				}

			}
		}
		else
		{
			RectF imgRect = GetImageRect();

			for(std::list<RECT>::iterator iter = rects.begin(); iter != rects.end(); ++iter)
			{
				RECT r = *iter;

				RectF rf = RECTTORECTF(r);
				SIZE imgSize = {(LONG)imgRect.Width, (LONG)imgRect.Height};
				RectF fromRect = RemapRectF(rf, imgSize, static_cast<float>(width), static_cast<float>(height));

				// fromRect is now in image's sizes, so we can directly add
				fromRect.X += imgRect.X;
				fromRect.Y += imgRect.Y;
				
				RectF out(rf.X + float(left) + extraOffset.X, rf.Y + float(top) + extraOffset.Y,  rf.Width, rf.Height);
				
				g->DrawImage(image, out, fromRect.X, fromRect.Y, fromRect.Width, fromRect.Height, UnitPixel, &attr, NULL, NULL);
			}
		}
	}
	else
	{
		if(usePlacement)
		{
			RectF imgRect = GetImageRect();

			for(std::list<RECT>::iterator iter = rects.begin(); iter != rects.end(); ++iter)
			{
				RECT r = *iter;

				
				RectF rf = RECTTORECTF(r);

				SIZE deskletSize = {width, height};
				RectF bounds((REAL)x,(REAL)y, (REAL)swidth, (REAL)sheight);
				bounds = RemapRectF(bounds, deskletSize, scaleX, scaleY);

				if(rotation % 360 != 0)
				{
					SIZE  s = {(LONG)rf.Width, (LONG)rf.Height};
					POINT p = {(LONG)rf.X, (LONG)rf.Y};
					FixRotation(rotation % 360, s, p);
					rf.X = (REAL)p.x;
					rf.Y = (REAL)p.y;
					rf.Width = (REAL)s.cy;
					rf.Height = (REAL)s.cx;
				}

				RectF intersect(rf);
				intersect.Intersect(bounds);
				if(!intersect.IsEmptyArea())
				{
					RectF out(intersect);
					
					float lx = intersect.X - bounds.X;
					float ly = intersect.Y - bounds.Y;
					RectF fromRect(lx, ly, out.Width, out.Height);
				
					float rX = static_cast<float>(imgRect.Width) / RemapPointF((float)swidth, width, scaleX);
					float rY = static_cast<float>(imgRect.Height) / RemapPointF((float)sheight, height, scaleY);

					out.X += (REAL)left + extraOffset.X;
					out.Y += (REAL)top + extraOffset.Y;

#ifdef USE_QUICK_FIX
			if(useImageSourceBounds && !(GetImageRect().X == 0 && GetImageRect().Width == image->GetWidth()))
				out.X++;
			if(useImageSourceBounds && !(GetImageRect().Y == 0 && GetImageRect().Height == image->GetHeight()))
				out.Y++;
#endif//USE_QUICK_FIX

					RectF in(fromRect);
					// in is still at images sizes, so we can directly add
					in.X *= rX;
					in.Y *= rY;
					in.X += imgRect.X;
					in.Y += imgRect.Y;
					in.Width *= rX;
					in.Height *= rY;
					// DEBUG purposes
					//SolidBrush blue(Color(255,0,0,255));
					//g->FillRectangle(&blue, out);
					g->DrawImage(image, out, in.X, in.Y, in.Width, in.Height, UnitPixel, NULL, NULL, NULL);
				}

			}
		}
		else
		{
			RectF imgRect = GetImageRect();

			for(std::list<RECT>::iterator iter = rects.begin(); iter != rects.end(); ++iter)
			{
				RECT r = *iter;

				RectF rf = RECTTORECTF(r);
				SIZE imgSize = {(LONG)imgRect.Width, (LONG)imgRect.Height};
				RectF fromRect = RemapRectF(rf, imgSize, static_cast<float>(width), static_cast<float>(height));

				// fromRect is now in image's sizes, so we can directly add
				fromRect.X += imgRect.X;
				fromRect.Y += imgRect.Y;
				
				RectF out(rf.X + float(left) + extraOffset.X, rf.Y + float(top) + extraOffset.Y,  rf.Width, rf.Height);
				
				g->DrawImage(image, out, fromRect.X, fromRect.Y, fromRect.Width, fromRect.Height, UnitPixel, NULL, NULL, NULL);
			}
		}
	}

	if(rotation % 360 !=0)
	{
		g->SetTransform(&oldTMatrix);
	}
}

void Layer::Draw(Graphics* g,int left, int top, int width, int height,ColorMatrix* oldMatrix)
{
	// pre: g is not 0 and image is not 0
	// post: this layer has been drawn on g if it is visible

	if(!isVisible || NULL == image || NULL == g || 0 == alpha)
		return;


	//if(dropShadow)DrawShadow(g,shadowX,shadowY,width,height,100,shadowAlpha);

	Matrix oldTMatrix;
	g->GetTransform(&oldTMatrix);

	if(rotation % 360 !=0)
	{
		Matrix* matrix = oldTMatrix.Clone();
		
		if(!usePlacement)
		{
			matrix->RotateAt((float)rotation,PointF((float)(width)/2.0f+left,(float)(height)/2.0f+top));
		}
		else
		{
			REAL xs = (REAL)width / (REAL)scaleX;
			REAL ys = (REAL)height / (REAL)scaleY;

			SIZE s = {width, height};
			RectF rf = RemapRectF(RectF((REAL)x,(REAL)y,(REAL)swidth, (REAL)sheight),s, scaleX, scaleY);

			float xP = rf.Width  / 2.0f + left + rf.X;
			float yP = rf.Height / 2.0f + top  + rf.Y;

			matrix->RotateAt((float)(rotation % 360),PointF(xP,yP));
		}

	
		g->SetTransform(matrix);

		delete matrix;
	}

	if(alpha!=255 || useColorMatrix || oldMatrix!=0 || hue != 0 || saturation != 100)
	{
		ColorMatrix matrix = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,cm);

		if(oldMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,*oldMatrix);

		if(alpha!=255)
		{
			matrix.m[3][3] *= ((REAL)alpha/255.0f);
		}

		if(hue != 0)
			matrix = ColorMatrixHelper::SetHue(matrix,(REAL)hue);

		if(saturation != 100)
			matrix = ColorMatrixHelper::SetSaturation(matrix,(REAL)saturation/100);

		ImageAttributes attr;
		attr.SetColorMatrix(&matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
		if(usePlacement)
		{
			RectF imgRect = GetImageRect();

			REAL xs = (REAL)(width) / (REAL)scaleX;
			REAL ys = (REAL)(height) / (REAL)scaleY;

			g->DrawImage(image,RectF(((REAL)(x) * xs)+left, (REAL)(y) * ys+top, (REAL)swidth * xs, (REAL)sheight * ys),imgRect.X, imgRect.Y, imgRect.Width, imgRect.Height,UnitPixel,&attr,0,0);
		}
		else
		{
			RectF imgRect = GetImageRect();
			g->DrawImage(image,Rect(left,top,width,height),(INT)imgRect.X, (INT)imgRect.Y, (INT)imgRect.Width, (INT)imgRect.Height,UnitPixel,&attr,0,0);
		}
	}
	else
	{
		if(usePlacement)
		{
			RectF imgRect = GetImageRect();

			REAL xs = (REAL)width / (REAL)scaleX;
			REAL ys = (REAL)height / (REAL)scaleY;

			// FIX: apply clipping
			int clipX = -min(0, x);
			int clipY = -min(0, y);

			SIZE s = {width, height};
			RectF rf = RemapRectF(RectF((REAL)(x + clipX),(REAL)(y + clipY),(REAL)(swidth - clipX), (REAL)(sheight - clipY) ),s, scaleX, scaleY);
			rf.X += left;
			rf.Y += top;

			RectF clipRect(float(clipX), float(clipY), 0.0f, 0.0f);
			SIZE curSize={scaleX, scaleY};
			clipRect= RemapRectF(clipRect, curSize, float(scaleX), float(scaleY));
			imgRect.X += clipRect.X;
			imgRect.Y += clipRect.Y;
			imgRect.Width -= clipRect.X;
			imgRect.Height-= clipRect.Y;

			g->DrawImage(image, rf,  imgRect.X, imgRect.Y , imgRect.Width, imgRect.Height, UnitPixel, 0,0,0);
			//g->DrawImage(image,RectF((REAL)(x)* xs +left , (REAL)(y) * ys +top, (REAL)swidth * xs, (REAL)sheight * ys), imgRect.X, imgRect.Y, imgRect.Width, imgRect.Height, UnitPixel, 0,0,0);

		}
		else
		{
			RectF imgRect = GetImageRect();

			g->DrawImage(image,RectF((REAL)left,(REAL)top,(REAL)width,(REAL)height), imgRect.X, imgRect.Y, imgRect.Width, imgRect.Height, UnitPixel, 0, 0,0);
		}
	}

	if(rotation % 360 !=0)
	{
		g->SetTransform(&oldTMatrix);
	}
}

void Layer::SetVisibility(bool val)
{
	// post: isVisible has been set to val
	isVisible = val;
}

void Layer::SetRotation(int val)
{
	// post: rotation has been set to val
	rotation = val;
}

void Layer::SetAlpha(int val)
{
	// post: alpha has been set to alpha
	alpha = (BYTE)val;
}

void  Layer::SetFileName(const char* filename)
{
	// post: filename has been copied into fileName if filename is not 0,
	//		 otherwise fileName has been set to ""
	if(filename)
		strcpy(fileName,filename);
	else
		strcpy(fileName,"");
}

const char* Layer::GetFileName()
{
	// post: fileName has been returned
	return fileName;
}

void Layer::SetHue(int val)
{
	// post: hue has been set to val
	hue = val;
}

void Layer::SetSaturation(unsigned int val)
{
	// post: saturation has been set to val
	saturation = val;
}

void Layer::SetCanBeIcon(bool val)
{
	// post: isCanBeIcon has been set to val
	isCanBeIcon = val;
}

bool Layer::GetCanBeIcon()
{
	// post: isCanBeIcon has been returned
	return isCanBeIcon;
}

void Layer::SetShadow(bool val, int sX, int sY, int sA)
{
	// post: shadow has been set
	dropShadow = val;
	if(dropShadow)
	{
		shadowX		= sX;
		shadowY		= sY;
		shadowAlpha	=(BYTE)sA;
	}
}

void Layer::DeleteCachedShadow()
{
	// post: the cached shadow has been deleted
	if(shadowBmp)
	{
		delete shadowBmp;
		shadowBmp = 0;
	}
}