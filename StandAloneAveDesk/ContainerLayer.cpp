// ContainerLayer.cpp: implementation of the ContainerLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContainerLayer.h"
#include "ColorMatrixHelper.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ContainerLayer::ContainerLayer(const char* name, Layer* parentLayer, bool isBuffered) : 
	Layer(name, parentLayer),
	isBuffered(isBuffered)
{

}

ContainerLayer::~ContainerLayer()
{

}

void ContainerLayer::SetIsBuffered(bool val, const SIZE* bufSize, const BOOL* append)
{
	isBuffered = val;
	if(isBuffered)
	{
		if(bufSize != NULL && append != NULL)
		{
			if(NULL == image || image->GetWidth() != static_cast<UINT>(bufSize->cx) || image->GetHeight() != static_cast<UINT>(bufSize->cy))
			{
				Bitmap* newBmp = new Bitmap(bufSize->cx, bufSize->cy);

				if(*append && image != NULL)
				{
					Graphics g(newBmp);
					g.DrawImage(image, Rect(0,0, bufSize->cx, bufSize->cy), 0, 0, bufSize->cx, bufSize->cy, UnitPixel, 0, 0, 0);		
					RECT r = {0};
					if(image->GetWidth() > static_cast<UINT>(bufSize->cx))
					{
						RECT r = {image->GetWidth(), 0, bufSize->cx, bufSize->cy};
						AddUpdateRect(r,*bufSize);
					}

					if(image->GetHeight() > static_cast<UINT>(bufSize->cy))
					{
						RECT r = {0, image->GetHeight(), bufSize->cx, bufSize->cy};
						AddUpdateRect(r, *bufSize);
					}
				}
				else
				{
					RECT r = {0,0,bufSize->cx, bufSize->cy};
					AddUpdateRect(r, *bufSize);
				}

				Layer::SetImage(newBmp, false);
			}
		}
	}
}

bool ContainerLayer::GetIsBuffered()const
{
	return isBuffered;
}


void ContainerLayer::SetImage(Image* bmp, bool clone)
{
	if(!clone)
		delete bmp;
}

void ContainerLayer::Draw(Graphics* g, int left, int top, int width, int height,ColorMatrix* oldMatrix)
{
	if(NULL == g || !GetVisibility() || 0 == alpha)
		return;

	Matrix oldTMatrix;
	g->GetTransform(&oldTMatrix);

	ColorMatrix matrix;
	ColorMatrix* pCm = NULL;
	if(alpha!=255 || useColorMatrix || oldMatrix!=NULL || hue != 0 || saturation != 100)
	{
		matrix = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,cm);

		if(oldMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,*oldMatrix);

		if(alpha!=255)
		{
			matrix.m[3][3] *= ((REAL)alpha / 255.0f);
		}

		if(hue != 0)
			matrix = ColorMatrixHelper::SetHue(matrix,(REAL)hue);

		if(saturation != 100)
			matrix = ColorMatrixHelper::SetSaturation(matrix,(REAL)saturation/100);

		pCm = &matrix;
	}

	if(rotation % 360 != 0)
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
			RectF rf = RemapRectF(RectF(static_cast<float>(x),static_cast<float>(y),
										static_cast<float>(swidth), static_cast<float>(sheight)),
										s, scaleX, scaleY);

			float xP = rf.Width  / 2.0f + left + rf.X;
			float yP = rf.Height / 2.0f + top  + rf.Y;

			matrix->RotateAt((float)(rotation % 360),PointF(xP,yP));
		}

	
		g->SetTransform(matrix);

		delete matrix;
	}

	if(!GetIsBuffered())
	{
		REAL xs = (REAL)width / (REAL)scaleX;
		REAL ys = (REAL)height / (REAL)scaleY;

		SIZE s = {width, height};
		RectF rf = RemapRectF(RectF(float(x),float(y),float(swidth), float(sheight)),s, scaleX, scaleY);
		rf.X += left;
		rf.Y += top;

		SIZE imgSize = {static_cast<LONG>(ceil(rf.Width)), static_cast<LONG>(ceil(rf.Height))};
		RectF imgRect;
		if(useImageSourceBounds)
			imgRect = RemapRectF(RECTTORECTF(imageSourceBounds), imgSize, imageSourceBoundsScale.cx, imageSourceBoundsScale.cy);
		else
			imgRect = RectF(0.0f,0.0f, REAL(imgSize.cx), REAL(imgSize.cy));


		for(std::vector<Layer*>::iterator iter = subLayers.begin(); iter != subLayers.end(); ++iter)
		{
			Layer* l = *iter;
			bool doDraw = true;

			/*
			RECT oldPlacementRect = l->GetPlacementRect();
			SIZE oldPlacementScale = l->GetPlacementScale();
			bool oldUsePlacement = l->GetUsePlacement();
			if(useImageSourceBounds)
			{
				if(!oldUsePlacement)
				{
					::SetRect(&oldPlacementRect, 0, 0, imgSize.cx, imgSize.cy);
					oldPlacementScale = imgSize;
				}

				RectF rf = RemapRectF(RECTTORECTF(oldPlacementRect), imgSize, oldPlacementScale.cx, oldPlacementScale.cy);
				RectF intersect(rf);
				intersect.Intersect(imgRect);
				doDraw = !intersect.IsEmptyArea();
				if(doDraw)
				{
					rf.X -= imgRect.X;
					rf.Y -= imgRect.Y;

					float xRatio = REAL(imgSize.cx) / imgRect.Width;
					float yRatio = REAL(imgSize.cy) / imgRect.Height;

					rf.X *= xRatio;
					rf.Y *= yRatio;
					rf.Width *= xRatio;
					rf.Height *= yRatio;
	
					RECT rcNew = {floor(rf.X), floor(rf.Y), floor(rf.X) + ceil(rf.Width), floor(rf.Y) + ceil(rf.Height)};
					l->SetPlacement(true, &rcNew, imgSize.cx, imgSize.cy);
				}
			}*/
			
		
			if(doDraw)
				l->Draw(g, (int)rf.X, (int)rf.Y, (int)rf.Width, (int)rf.Height, pCm);


		//	if(useImageSourceBounds)
		//		l->SetPlacement(oldUsePlacement, &oldPlacementRect, oldPlacementScale.cx, oldPlacementScale.cy);
		}
	}
	else
	{
		if(!m_UpdateRegion.IsEmpty())
			InternalUpdate(left, top, width, height, NULL);

		Layer::Draw(g, left, top, width, height, oldMatrix);
	}

	if(rotation % 360 != 0)
	{
		g->SetTransform(&oldTMatrix);
	}
}

void ContainerLayer::InternalUpdate(int left, int top, int width, int height, std::list<RECT>* rects)
{
	// step 1: convert rects to this layers size
	// step 2: offset rects to this layers position
	// step 3: intersect rects with this layers visible area (clipRect)
	// step 4: intersect rects with this layers UpdateRegion
	// step 5: draw layers
	// step 6: update updatebounds
	
	/*
	std::list<RECT> correctedRects;
	if(rects != NULL)
	{
		SIZE currentSize = {width, height};
		foreach(std::<RECT>::iterator iter = rects->begin(); iter != rects->end(); ++iter)
		{
			RectF r = *iter;
			r = RemapRect(r, currentSize, bufSize.cx, bufSize.cy);
		}
	}*/

}

	
bool ContainerLayer::HitTest(const POINT& point, const SIZE& size)
{

	if(!GetIsBuffered())
	{
		POINT p = point;
		SIZE s = size;
		ConvertPointAndSize(p, s, false);
		for(std::vector<Layer*>::reverse_iterator iter = subLayers.rbegin(); iter != subLayers.rend(); ++iter)
		{
			Layer* l = *iter;

			bool doTest = true;
			/*
			REAL xs = (REAL)size.cx / (REAL)scaleX;
			REAL ys = (REAL)size.cy / (REAL)scaleY;

			SIZE s = size;
			RectF rf = RemapRectF(RectF(x,y,swidth, sheight),s, scaleX, scaleY);
			rf.X += 0;
			rf.Y += 0;

			SIZE imgSize = {ceil(rf.Width), ceil(rf.Height)};
			RectF imgRect;
			if(useImageSourceBounds)
				imgRect = RemapRectF(RECTTORECTF(imageSourceBounds), imgSize, imageSourceBoundsScale.cx, imageSourceBoundsScale.cy);
			else
				imgRect = RectF(0,0, imgSize.cx, imgSize.cy);

			RECT oldPlacementRect = l->GetPlacementRect();
			SIZE oldPlacementScale = l->GetPlacementScale();
			bool oldUsePlacement = l->GetUsePlacement();
			if(useImageSourceBounds)
			{
				if(!oldUsePlacement)
				{
					::SetRect(&oldPlacementRect, 0, 0, imgSize.cx, imgSize.cy);
					oldPlacementScale = imgSize;
				}

				RectF rf = RemapRectF(RECTTORECTF(oldPlacementRect), imgSize, oldPlacementScale.cx, oldPlacementScale.cy);
				RectF intersect(rf);
				intersect.Intersect(imgRect);
				doTest = intersect.IsEmptyArea() != FALSE;
				if(doTest)
				{
					rf.X -= imgRect.X;
					rf.Y -= imgRect.Y;

					float xRatio = REAL(imgSize.cx) / imgRect.Width;
					float yRatio = REAL(imgSize.cy) / imgRect.Height;

					rf.X *= xRatio;
					rf.Y *= yRatio;
					rf.Width *= xRatio;
					rf.Height *= yRatio;
	
					RECT rcNew = {floor(rf.X), floor(rf.Y), floor(rf.X) + ceil(rf.Width), floor(rf.Y) + ceil(rf.Height)};
					l->SetPlacement(true, &rcNew, imgSize.cx, imgSize.cy);
				}
			}*/

			bool didHitTest = doTest && (l != NULL) && l->HitTest(p, s);

			//if(useImageSourceBounds)
			//	l->SetPlacement(oldUsePlacement, &oldPlacementRect, oldPlacementScale.cx, oldPlacementScale.cy);

			if(didHitTest)
				return true;
		}

		return false;
	}
	else
	{
		return Layer::HitTest(point, size);
	}
}


BOOL ContainerLayer::GetHitTestedLayer(const POINT& point, const SIZE& size, Layer*& hitLayer, bool mustBeVisible)
{
	if(HitTest(point, size))
	{
		POINT p = point;
		SIZE  s = size;
		ConvertPointAndSize(p, s, false);

		for(std::vector<Layer*>::reverse_iterator iter = subLayers.rbegin(); iter != subLayers.rend(); ++iter)
		{
			Layer* l = *iter;
			if(l != NULL && l->GetHitTestedLayer(p, s, hitLayer, mustBeVisible))
			{
				if(!mustBeVisible || (mustBeVisible && GetVisibility()))
					return TRUE;
			}
		}

		hitLayer = this;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

SIZE ContainerLayer::GetBufferSize()const
{
	return bufferSize;
}

void ContainerLayer::AddUpdateRect(const RECT& r, const SIZE& scale)
{
	if(GetIsBuffered())
	{
		RectF rf = RemapRectF(RECTTORECTF(r), GetBufferSize(), scale.cx, scale.cy);
		RECT remappedRect = {0};
		remappedRect.left = (LONG)rf.X;
		remappedRect.right = (LONG)ceil(rf.X + rf.Width);
		remappedRect.top = (LONG)rf.Y;
		remappedRect.bottom = (LONG)ceil(rf.Y + rf.Height);
		m_UpdateRegion.Add(remappedRect);
	}
}

void ContainerLayer::DrawPartialRotation(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
						std::list<RECT>& rects)
{

}

void ContainerLayer::DrawPartial(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
			std::list<RECT>& rects)
{
	if(rotation % 360 != 0)
	{
		DrawPartialRotation(g, left, top, width, height, oldMatrix, rects);
		return;
	}

	if(NULL == g || !GetVisibility() || 0 == alpha)
		return;

	Matrix oldTMatrix;
	g->GetTransform(&oldTMatrix);

	ColorMatrix matrix;
	ColorMatrix* pCm = NULL;
	if(alpha!=255 || useColorMatrix || oldMatrix!=NULL || hue != 0 || saturation != 100)
	{
		matrix = ColorMatrixHelper::GetIdentityMatrix();

		if(useColorMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,cm);

		if(oldMatrix)
			matrix = ColorMatrixHelper::Multiply(matrix,*oldMatrix);

		if(alpha!=255)
		{
			matrix.m[3][3] *= ((REAL)alpha / 255.0f);
		}

		if(hue != 0)
			matrix = ColorMatrixHelper::SetHue(matrix,(REAL)hue);

		if(saturation != 100)
			matrix = ColorMatrixHelper::SetSaturation(matrix,(REAL)saturation/100);

		pCm = &matrix;
	}

	if(rotation % 360 != 0)
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

	if(!GetIsBuffered())
	{
		RectF imgRect = GetImageRect();

		REAL xs = (REAL)width / (REAL)scaleX;
		REAL ys = (REAL)height / (REAL)scaleY;

		SIZE s = {width, height};
		RectF rf = RemapRectF(RectF((REAL)x,(REAL)y,(REAL)swidth, (REAL)sheight),s, scaleX, scaleY);
		rf.X;
		rf.Y;

		// convert the GLOBAL update rectangles to rectangles that are corrected to the current
		// layer size
		std::list<RECT> correctedRects;

		SIZE deskletSize = {width, height};
		RectF bounds((REAL)x,(REAL)y, (REAL)swidth, (REAL)sheight);
		bounds = RemapRectF(bounds, deskletSize, scaleX, scaleY);

		for(std::list<RECT>::iterator iter = rects.begin(); iter != rects.end(); ++iter)
		{
			RECT r = *iter;	
			RectF rf = RECTTORECTF(r);

			RectF intersect(rf);
			intersect.Intersect(bounds);
			if(!intersect.IsEmptyArea())
			{
				float newX = intersect.X - bounds.X;
				float newY = intersect.Y - bounds.Y;
				RECT corrRect = { (LONG)floor(newX), (LONG)floor(newY), 
					(LONG)ceil(newX + intersect.Width), (LONG)ceil(newY + intersect.Height)};
				correctedRects.push_back(corrRect);
			}
		

		}

		for(std::vector<Layer*>::iterator layerIter = subLayers.begin(); layerIter != subLayers.end(); ++layerIter)
		{
			Layer* l = *layerIter;
			l->DrawPartial(g, (int)(left + floor(bounds.X)), (int)(top + floor(bounds.Y)), (int)ceil(bounds.Width), (int)ceil(bounds.Height), pCm, correctedRects);
		}
	}
	else
	{
		if(!m_UpdateRegion.IsEmpty())
			InternalUpdate(left, top, width, height, &rects);

		Layer::DrawPartial(g, left, top, width, height, oldMatrix, rects);
	}

	if(rotation % 360 != 0)
	{
		g->SetTransform(&oldTMatrix);
	}
}