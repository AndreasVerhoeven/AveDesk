// filename:		labelbackground.cpp
// Description:		Implementation of class LabelBackground
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!


#include "stdafx.h"
#include "labelbackground.h"

#include "Application.h"
extern CComObject<CApplication>* _App;

std::vector<LabelBackground*> LabelBackground::LabelBackgrounds;

extern Bitmap* CALLBACK DeskletLoadGDIPlusImageW(const WCHAR* szImage);

LabelBackground::LabelBackground(std::wstring dirname) : 
	ref_count(1),
	left(0),
	right(0),
	mid(0),
	td_top(0),
	td_bottom(0),
	td_mid(0),
	sel_left(0),
	sel_right(0),
	sel_mid(0),
	sel_td_top(0),
	sel_td_bottom(0),
	sel_td_mid(0),
	supportsSelectionState(false),
	leftWidth(0),
	leftHeight(0),
	middleWidth(0),
	middleHeight(0),
	rightHeight(0), marginTop(0), marginLeft(0), marginBottom(0),
	marginRight(0), supportsTopDown(true), subLabelMargin(0),
	noShadowForNormalLabel(false), sel_noShadowForNormalLabel(false)
	
{

	// pre:  dirname is not 0
	// post: A new Labelbackground object has been created from the background files
	//		 in Labels\<dirName>

		name = dirname;

		WCHAR filename[MAX_PATH+1];
		wcscpy(filename, _App->GetAveDeskDataDir().c_str());
		wcscat(filename,L"Labels\\");
		wcscat(filename,dirname.c_str());
		wcscat(filename,L"\\values.ini");

		hasNormalLabel = GetPrivateProfileIntW(L"Normal",L"HasNormal",TRUE,filename) == TRUE;
		if(hasNormalLabel)
		{
			std::wstring leftName(L"Labels\\");
			leftName += dirname;
			leftName += L"\\left.png";
			left  = DeskletLoadGDIPlusImageW( leftName.c_str() );

			std::wstring midName(L"Labels\\");
			midName += dirname;
			midName += L"\\mid.png";
			mid  = DeskletLoadGDIPlusImageW( midName.c_str() );

			std::wstring rightName(L"Labels\\");
			rightName += dirname;
			rightName += L"\\right.png";
			right  = DeskletLoadGDIPlusImageW( rightName.c_str() );

			leftWidth	= GetPrivateProfileIntW(L"Left",L"Width",10,filename);
			leftHeight	= GetPrivateProfileIntW(L"Left",L"Height",10,filename);
			middleWidth	= GetPrivateProfileIntW(L"Middle",L"Width",10,filename);
			middleHeight= GetPrivateProfileIntW(L"Middle",L"Height",10,filename);
			rightWidth	= GetPrivateProfileIntW(L"Right",L"Width",10,filename);
			rightHeight	= GetPrivateProfileIntW(L"Right",L"Height",10,filename);
	
			marginTop	= GetPrivateProfileIntW(L"Margin",L"Top",10,filename);
			marginLeft	= GetPrivateProfileIntW(L"Margin",L"Left",10,filename);
			marginRight	= GetPrivateProfileIntW(L"Margin",L"Right",10,filename);
			marginBottom= GetPrivateProfileIntW(L"Margin",L"Bottom",10,filename);

			defaultWidth= GetPrivateProfileIntW(L"Default",L"Width",10,filename);
			defaultHeight=GetPrivateProfileIntW(L"Default",L"Height",10,filename);

			supportsTopDown = GetPrivateProfileIntW(L"Extra",L"SupportsTopDown",FALSE,filename)==TRUE;

			subLabelMargin = GetPrivateProfileIntW(L"SubLabel",L"Margin",GetMarginBottom(false),filename);

			noShadowForNormalLabel = GetPrivateProfileIntW(L"Normal",L"NoShadowForNormalLabel",FALSE,filename) == TRUE;
		}


		// selection stuff
		supportsSelectionState = GetPrivateProfileInt(L"Selection",L"HasSelection",FALSE,filename) == TRUE;

		if(supportsSelectionState)
		{
			std::wstring leftName = L"Labels\\";
			leftName += dirname;
			leftName += L"\\sel_left.png";
			sel_left  = DeskletLoadGDIPlusImageW( leftName.c_str() );

			std::wstring midName = L"Labels\\";
			midName += dirname;
			midName += L"\\sel_mid.png";
			sel_mid  = DeskletLoadGDIPlusImageW( midName.c_str() );

			std::wstring rightName =L"Labels\\";
			rightName += dirname;
			rightName += L"\\sel_right.png";
			sel_right  = DeskletLoadGDIPlusImageW( rightName.c_str() );

			sel_leftWidth	= GetPrivateProfileIntW(L"SelectionLeft",L"Width",10,filename);
			sel_leftHeight	= GetPrivateProfileIntW(L"SelectionLeft",L"Height",10,filename);
			sel_middleWidth	= GetPrivateProfileIntW(L"SelectionMiddle",L"Width",10,filename);
			sel_middleHeight= GetPrivateProfileIntW(L"SelectionMiddle",L"Height",10,filename);
			sel_rightWidth	= GetPrivateProfileIntW(L"SelectionRight",L"Width",10,filename);
			sel_rightHeight	= GetPrivateProfileIntW(L"SelectionRight",L"Height",10,filename);
	
			sel_marginTop	= GetPrivateProfileIntW(L"SelectionMargin",L"Top",10,filename);
			sel_marginLeft	= GetPrivateProfileIntW(L"SelectionMargin",L"Left",10,filename);
			sel_marginRight	= GetPrivateProfileIntW(L"SelectionMargin",L"Right",10,filename);
			sel_marginBottom= GetPrivateProfileIntW(L"SelectionMargin",L"Bottom",10,filename);

			sel_defaultWidth= GetPrivateProfileIntW(L"SelectionDefault",L"Width",10,filename);
			sel_defaultHeight=GetPrivateProfileIntW(L"SelectionDefault",L"Height",10,filename);

			sel_subLabelMargin = GetPrivateProfileIntW(L"SelectionSubLabel",L"Margin",GetMarginBottom(true),filename);

			sel_noShadowForNormalLabel = GetPrivateProfileIntW(L"Selection",L"NoShadowForNormalLabel",FALSE,filename) == TRUE;
		}

		if(left)
		{
			td_top = (Bitmap*)(((Image*)left)->Clone());
			td_top->RotateFlip(Rotate90FlipNone);
		}

		if(mid)
		{
			td_mid = (Bitmap*)(((Image*)mid)->Clone());
			td_mid->RotateFlip(Rotate90FlipNone);
		}

		if(right)
		{
			td_bottom = (Bitmap*)(((Image*)right)->Clone());
			td_bottom->RotateFlip(Rotate90FlipNone);
		}

		if(sel_left)
		{
			sel_td_top = (Bitmap*)(((Image*)sel_left)->Clone());
			sel_td_top->RotateFlip(Rotate90FlipNone);
		}

		if(sel_mid)
		{
			sel_td_mid = (Bitmap*)(((Image*)sel_mid)->Clone());
			sel_td_mid->RotateFlip(Rotate90FlipNone);
		}

		if(sel_right)
		{
			sel_td_bottom = (Bitmap*)(((Image*)sel_right)->Clone());
			sel_td_bottom->RotateFlip(Rotate90FlipNone);
		}
}
	
LabelBackground::~LabelBackground()
{
	// post: this Labelbackground object has been destroyed and all its memory
	//			has been released
	delete left;
	delete mid;
	delete right;

	delete td_top;
	delete td_mid;
	delete td_bottom;

	delete sel_left;
	delete sel_mid;
	delete sel_right;

	delete sel_td_top;
	delete sel_td_mid;
	delete sel_td_bottom;
}

int LabelBackground::GetSubLabelMargin(bool sel) const
{
	return (sel && supportsSelectionState) ? sel_subLabelMargin : subLabelMargin;
}

bool LabelBackground::HasNormalLabel()const
{
	return hasNormalLabel;
}

bool LabelBackground::GetNoShadowForNormalLabel(bool sel)const
{
	return (sel && supportsSelectionState) ? sel_noShadowForNormalLabel : noShadowForNormalLabel;
}

int LabelBackground::GetMarginBottom(bool sel)const
{
	return (sel && supportsSelectionState) ? sel_marginBottom : marginBottom;
}

int LabelBackground::GetMarginTop(bool sel)const
{
	return (sel && supportsSelectionState) ? sel_marginTop : marginTop;
}

int LabelBackground::GetMarginLeft(bool sel)const
{
	return (sel && supportsSelectionState) ? sel_marginLeft : marginLeft;
}


int LabelBackground::GetMarginRight(bool sel)const
{
	return (sel && supportsSelectionState) ? sel_marginRight : marginRight;
}


Bitmap* LabelBackground::GetLeft(bool sel ) const
{
	return (sel && supportsSelectionState) ? sel_left : left;
}


int LabelBackground::GetLeftHeight(bool sel ) const
{
	return (sel && supportsSelectionState) ? sel_leftHeight : leftHeight;
}

int LabelBackground::GetLeftWidth(bool sel ) const
{
	return (sel && supportsSelectionState) ? sel_leftWidth : leftWidth;
}

Bitmap* LabelBackground::GetMid(bool sel ) const
{
	return (sel && supportsSelectionState) ? sel_mid : mid; 
}

int LabelBackground::GetMiddleHeight(bool sel )const
{
	return (sel && supportsSelectionState) ? sel_middleHeight : middleHeight;
}

int LabelBackground::GetMiddleWidth(bool sel ) const
{
	return (sel && supportsSelectionState) ? sel_middleWidth : middleWidth;
}

Bitmap* LabelBackground::GetRight(bool sel) const
{
	return (sel && supportsSelectionState) ? sel_right : right;
}

int LabelBackground::GetRightHeight(bool sel)const
{
	return (sel && supportsSelectionState) ? sel_rightHeight : rightHeight;
}

int LabelBackground::GetRightWidth(bool sel ) const
{
	return (sel && supportsSelectionState) ? sel_rightWidth : rightWidth;
}

bool LabelBackground::SupportsSelectState()const
{
	return supportsSelectionState;
}

void LabelBackground::TransformRect(Gdiplus::Rect& rect, bool sel)const
{
	if(!sel)
	{
		rect = Gdiplus::Rect(rect.GetLeft()+marginLeft+leftWidth,
		rect.GetTop() + marginTop,
		rect.GetRight() - rightWidth -marginRight -(rect.GetLeft()+marginLeft+leftWidth) ,
		rect.GetBottom() - marginBottom);
	}
	else
	{
		rect = Gdiplus::Rect(rect.GetLeft()+sel_marginLeft+sel_leftWidth,
		rect.GetTop() + marginTop,
		rect.GetRight() - sel_rightWidth -sel_marginRight -(rect.GetLeft()+sel_marginLeft+sel_leftWidth) ,
		rect.GetBottom() - sel_marginBottom);
	}
};

void LabelBackground::TransformRectTopDown(Gdiplus::Rect& rect, bool sel) const
{
		if(!sel)
		{
			rect = Gdiplus::Rect(rect.GetLeft()+marginBottom,
			rect.GetTop() + marginLeft + leftWidth,
			rect.GetRight() - marginTop,
			rect.GetBottom() - rightWidth - marginRight);
		}
		else
		{
			rect = Gdiplus::Rect(rect.GetLeft()+sel_marginBottom,
			rect.GetTop() + sel_marginLeft + sel_leftWidth,
			rect.GetRight() - sel_marginTop,
			rect.GetBottom() - sel_rightWidth - sel_marginRight);
		}
}

void LabelBackground::ReleaseAll()
{
	// post: all existing LabelBackground objects has been destroyed and their memory
	//		 has been released
	
	for(std::vector<LabelBackground*>::iterator iter = LabelBackgrounds.begin();iter!=LabelBackgrounds.end();++iter)
	{
		delete *iter;
	}

	LabelBackgrounds.clear();
}
		
void LabelBackground::Release()
{
	// post: This LabelBackground object has been released. If this LabelBackground object
	//		 is not used anymore (ref_count == 0 ) then it has been deleted

	if(--ref_count == 0)
	{
	  for(std::vector<LabelBackground*>::iterator iter = LabelBackgrounds.begin();iter!=LabelBackgrounds.end();++iter)
	  {
		  if(name == (*iter)->name)
		  {
			LabelBackgrounds.erase(iter);
			delete this;
			break;
		  }
	  }
	}
}

void LabelBackground::AddRef()
{
	// pre: this LabelBackground reference count has been increased

	ref_count++;
}

LabelBackground* LabelBackground::LoadLabelBackground(std::wstring name)
{
	// pre:  name is not 0
	// post: A Labelbackground object has been returned that is based on the background
	//		 files in the Labels\<name>  folder. If this background file is already loaded,
	//		 then a pointer to the already created LabelBackground has been returned,
	//		 otherwise a newly created LabelBackground object has been returned

/*	for(std::vector<LabelBackground*>::iterator iter = LabelBackgrounds.begin();iter!=LabelBackgrounds.end();++iter)
	{
		if(name == (*iter)->name)
		{
			//!herd
			(*iter)->AddRef();
			return *iter;
		}
	}
	*/

	//!herd
	LabelBackground *lbl = new LabelBackground(name);
	LabelBackgrounds.push_back(lbl);
	lbl->AddRef();
	return lbl;
}

void LabelBackground::DrawTopDown(Graphics* graphics,Rect& rc, bool sel)const
{
	// pre:  graphics is not 0
	// post: this LabelBackground has been drawn topdown on graphics according to rc

	if(!graphics)return;
	InterpolationMode ipm = graphics->GetInterpolationMode();
	graphics->SetInterpolationMode(InterpolationModeLowQuality);

	SmoothingMode mode = graphics->GetSmoothingMode();
	graphics->SetSmoothingMode(SmoothingModeNone);

	int height = rc.GetBottom() - rc.GetTop() + marginLeft + marginRight;
	int width = rc.GetRight() - rc.GetLeft() + marginTop + marginBottom;

	int l = rc.GetLeft() - marginBottom;
	int t = rc.GetTop() - marginLeft;

	if(td_top)
	{
		t -= td_top->GetHeight();
		graphics->DrawImage(td_top,l,t,width,td_top->GetHeight());
		//height-=td_top->GetHeight();
		t += td_top->GetHeight();

	}

	if(td_mid)
	{
		int imgHeight = td_mid->GetHeight();
		int numImagesNeeded = height / imgHeight +1;
		Bitmap bmp(width,imgHeight);
		Graphics g(&bmp);
		g.SetInterpolationMode(InterpolationModeLowQuality);
		g.SetSmoothingMode(SmoothingModeNone);
		g.DrawImage(td_mid,0,0,width,imgHeight);

		for(int i=0;i<numImagesNeeded;++i)
		{
			if(i!=numImagesNeeded-1)
			{
				graphics->DrawImage(&bmp,Rect(l,t,width,imgHeight),
							0,0,width,imgHeight,UnitPixel);
				t += imgHeight;
			}
			else
			{
				graphics->DrawImage(&bmp,l,t,
							0,0,width,height % imgHeight,UnitPixel);
						t = height + rc.GetBottom() - marginLeft;
			}
		
		}


		graphics->DrawImage(td_mid,Rect(l, t,width,height),0,0,td_mid->GetWidth(),td_mid->GetHeight(),UnitPixel,0,0,0);
	}

	if(td_bottom)
	{
		t = rc.GetBottom() + marginRight;
		graphics->DrawImage(td_bottom,Rect(l,t,width,td_bottom->GetHeight()),0,0,td_bottom->GetWidth(),td_bottom->GetHeight(),UnitPixel,0,0,0);

	}


	graphics->SetSmoothingMode(mode);
	graphics->SetInterpolationMode(ipm);
}

void LabelBackground::GetThumbnail(Graphics& g, const SIZE& s, bool sel)
{
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	//g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	int l = 0;
	int t = 0;
	int w = s.cx;
	int h = s.cy;

	Bitmap* imgLeft = GetLeft(sel);
	if(imgLeft != NULL)
	{
		g.DrawImage(imgLeft, Rect(l,t, imgLeft->GetWidth(), h), 0,0, imgLeft->GetWidth(), imgLeft->GetHeight(), UnitPixel, 0, 0, 0);
		l += imgLeft->GetWidth();
		w -= imgLeft->GetWidth();
	}

	Bitmap* imgRight = GetRight(sel);
	if(imgRight != NULL)
	{
		g.DrawImage(imgRight, Rect(w, t, imgRight->GetWidth(),  h), 0, 0, imgRight->GetWidth(), imgRight->GetHeight(), UnitPixel, 0, 0, 0);
		w -= imgRight->GetWidth();
	}

	Bitmap* imgMid = GetMid(sel);
	if(imgMid != NULL)
	{
		InterpolationMode ipm = g.GetInterpolationMode();
		g.SetInterpolationMode(InterpolationModeLowQuality);

		g.SetSmoothingMode(SmoothingModeNone);

		int imgWidth = imgMid->GetWidth();
		int numImagesNeeded = 0;
		if(imgWidth > 0)
			numImagesNeeded = w / imgWidth + 1;

		Bitmap bmpTemp(imgWidth,h);
		Graphics g2(&bmpTemp);
		g2.SetInterpolationMode(InterpolationModeLowQuality);
		g2.SetSmoothingMode(SmoothingModeNone);
		g2.DrawImage(imgMid, Rect(0,0,imgWidth,h), 0, 0, imgWidth, imgMid->GetHeight(), UnitPixel, 0, 0, 0);
		//g.DrawImage(imgMid,0,0,imgWidth,h);
		
		for(int i=0;i<numImagesNeeded;++i)
		{
			if(i!=numImagesNeeded-1)
			{

				g.DrawImage(&bmpTemp, Rect(l, t, imgWidth, h), 0, 0, imgWidth, bmpTemp.GetHeight(), UnitPixel, 0, 0, 0);
				//g.DrawImage(&bmpTemp,l,t,
				//	0,0,imgWidth,h,UnitPixel);
				w -= imgWidth;
				l += imgWidth;
			}
			else
			{
				//g.DrawImage(&bmpTemp, Rect(0,0, w, h), 0, 0, w, imgMid->GetHeight(), UnitPixel , 0, 0, 0);
				g.DrawImage(&bmpTemp, Rect(l,t,w,h), 0, 0, w, bmpTemp.GetHeight(), UnitPixel, 0, 0, 0);
				//g.DrawImage(&bmpTemp,l,t,
				//	0,0,w ,h,UnitPixel);
				//l = w + rc.GetLeft() - GetMarginLeft(sel);
			}
			
		}

		//g.SetSmoothingMode(mode);
		g.SetInterpolationMode(ipm);
	}
	
};

void LabelBackground::Draw(Graphics* graphics,Rect& rc, bool sel)const
{
	if(!graphics)
		return;

	InterpolationMode ipm = graphics->GetInterpolationMode();
	graphics->SetInterpolationMode(InterpolationModeLowQuality);

	int height = rc.GetBottom() - rc.GetTop();
	int width  = rc.GetRight() - rc.GetLeft() + GetMarginLeft(sel) + GetMarginRight(sel);

	if(height < 4)
		return;	// no drawing yet, too small area

	
	height += GetMarginBottom(sel) + GetMarginTop(sel);

	if(GetLeft(sel))
	{
		graphics->DrawImage(GetLeft(sel), rc.GetLeft() - GetMarginLeft(sel)-GetLeftWidth(sel),
				rc.GetTop() - GetMarginTop(sel),
				GetLeftWidth(sel),
				height);
	}

	if(GetRight(sel))
	{
		graphics->DrawImage(GetRight(sel),rc.GetRight() + GetMarginRight(sel),
			rc.GetTop() - GetMarginTop(sel),
			GetRightWidth(sel),
			height);
	}

	int lleft = rc.GetLeft() - GetMarginLeft(sel);
	if(GetMid(sel))
	{
		SmoothingMode mode = graphics->GetSmoothingMode();
		graphics->SetSmoothingMode(SmoothingModeNone);

		int imgWidth = GetMid(sel)->GetWidth();
		int numImagesNeeded = 0;
		if(imgWidth > 0)
			numImagesNeeded = width / imgWidth +1;

		Bitmap bmp(imgWidth,height);
		Graphics g(&bmp);
		g.SetInterpolationMode(InterpolationModeLowQuality);
		g.SetSmoothingMode(SmoothingModeNone);
		g.DrawImage(GetMid(sel),0,0,imgWidth,height);
		
		for(int i=0;i<numImagesNeeded;++i)
		{
			if(i!=numImagesNeeded-1)
			{

				graphics->DrawImage(&bmp,lleft,rc.GetTop()-GetMarginTop(sel),
					0,0,imgWidth,height,UnitPixel);
				lleft += imgWidth;
			}
			else
			{
				graphics->DrawImage(&bmp,lleft,rc.GetTop()-GetMarginTop(sel),
					0,0,width - (lleft - (rc.GetLeft() - GetMarginLeft(sel))) ,height,UnitPixel);
				lleft = width + rc.GetLeft() - GetMarginLeft(sel);
			}
		}

		graphics->SetSmoothingMode(mode);
	}

	graphics->SetInterpolationMode(ipm);
}

#if 0
void LabelBackground::Draw(Graphics* graphics,Rect& rc)const
{
	// pre:  graphics is not 0
	// post: this LabelBackground has been drawn on graphics according to rc

	if(graphics)
	{
		InterpolationMode ipm = graphics->GetInterpolationMode();
		graphics->SetInterpolationMode(InterpolationModeLowQuality);

		int height = rc.GetBottom() - rc.GetTop();
		int width = rc.GetRight() - rc.GetLeft() + marginLeft + marginRight;

		if( height < 4)
		{
			// draw with default sizes
			// NOT IMPLEMENTED YET!
		}
		else
		{
			height += marginBottom + marginTop;
			if(left)
			{
				graphics->DrawImage(left, rc.GetLeft() - marginLeft-leftWidth,
					rc.GetTop() - marginTop,
					leftWidth,
					height);
			}

			if(right)
			{
				graphics->DrawImage(right,rc.GetRight() + marginRight,
					rc.GetTop() - marginTop,
					rightWidth,
					height);
			}

			int left = rc.GetLeft() - marginLeft;
			if(mid)
			{
				int imgWidth = mid->GetWidth();
				int numImagesNeeded = width / imgWidth +1;

				Bitmap bmp(imgWidth,height);
				Graphics g(&bmp);
				g.SetInterpolationMode(InterpolationModeLowQuality);
				g.SetSmoothingMode(SmoothingModeNone);
				/*g.DrawImage(mid,Rect(0,0,imgWidth,height),0,0,mid->GetWidth(),mid->GetHeight(),
					UnitPixel,&ia,0,0);*/
				g.DrawImage(mid,0,0,imgWidth,height);

				SmoothingMode mode = graphics->GetSmoothingMode();
				graphics->SetSmoothingMode(SmoothingModeNone);
				
				for(int i=0;i<numImagesNeeded;++i)
				{
					if(i!=numImagesNeeded-1)
					{

						graphics->DrawImage(&bmp,left,rc.GetTop()-marginTop,
							0,0,imgWidth,height,UnitPixel);
						left += imgWidth;
					}
					else
					{
						graphics->DrawImage(&bmp,left,rc.GetTop()-marginTop,
							0,0,width - (left - (rc.GetLeft() - marginLeft)) ,height,UnitPixel);
						left = width + rc.GetLeft() - marginLeft;
					}
				}
				
				graphics->SetSmoothingMode(mode);
			}
		}
		graphics->SetInterpolationMode(ipm);
	}
}
#endif