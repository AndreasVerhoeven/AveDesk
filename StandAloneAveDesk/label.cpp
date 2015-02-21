// filename:		label.cpp
// Description:		Implementation  of class Label defined in label.h
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
#include "label.h"
#include <windows.h>
#include "helper_funcs.h"
#include "docklet.h"

#include <memory>

#include "image_funcs.h"

#define INITPROP(n,v) m_##n = v;

Label::Label(Docklet* d) :m_Bg(NULL), m_DockletParent(d), m_IsDirty(true)
{
	// post: a Label object has been created
	INITPROP(Bg,NULL);
	INITPROP(Alpha,255);
	INITPROP(Rotation,0);
	INITPROP(VAlign,0);
	INITPROP(HAlign,1);
	INITPROP(Width,200);
	INITPROP(Height,75);
	INITPROP(Margin,5);
	INITPROP(Position,0);
	INITPROP(DrawTopDown,false);
	INITPROP(ShowSubLabel,true);
	INITPROP(MainCaption,L"Unnamed");
	INITPROP(SubCaption,L"");
	INITPROP(FadingIn,false);
}

HWND Label::SetupWindow(HWND hWndParent)
{
	RECT r = {0};
	Create(hWndParent,r,_T("AveLabel"),WS_POPUP | WS_OVERLAPPED,WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	// we dont want this window to show up in the taskbar
	ShowWindow(SW_HIDE);
	SetParent(NULL);
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW,0);

	EnsureWindowIsLayered();

	return m_hWnd;
}

Label::~Label()
{
	if(m_hWnd != NULL)
		DestroyWindow();
}

bool Label::NeedsRedrawOnSelect()const
{
	return m_Bg && m_Bg->SupportsSelectState();	
}


HRESULT Label::Load(ILoader& loader)
{
	USES_CONVERSION;
	std::wstring _tmp;

	LOADABLE_PROP_CLASS(MainLabelProperties);
	LOADABLE_PROP_CLASS(SubLabelProperties);
	LOADABLE_PROP_PRIMITIVE(Alpha);
	LOADABLE_PROP_PRIMITIVE(Rotation);
	LOADABLE_PROP_PRIMITIVE(VAlign);
	LOADABLE_PROP_PRIMITIVE(HAlign);
	LOADABLE_PROP_PRIMITIVE(Width);
	LOADABLE_PROP_PRIMITIVE(Height);
	LOADABLE_PROP_PRIMITIVE(Margin);
	LOADABLE_PROP_PRIMITIVE(Position);
	LOADABLE_PROP_PRIMITIVE(DrawTopDown);
	LOADABLE_PROP_PRIMITIVE(ShowSubLabel);
	LOADABLE_PROP_PRIMITIVE(MainCaption);
	LOADABLE_PROP_PRIMITIVE(SubCaption);

	if(m_Bg)
		m_Bg->Release();

	std::wstring bgName;
	LOADABLE_PRIMITIVE_NAMED(bgName,Background);

	if(bgName != L"")
		m_Bg = LabelBackground::LoadLabelBackground(bgName);
	else
		m_Bg = 0;

	return S_OK;
}

HRESULT Label::Save(ISaver& saver)
{
	USES_CONVERSION;
	std::wstring _tmp;

	SAVABLE_PROP_CLASS(MainLabelProperties);
	SAVABLE_PROP_CLASS(SubLabelProperties);
	SAVABLE_PROP_PRIMITIVE(Alpha);
	SAVABLE_PROP_PRIMITIVE(Rotation);
	SAVABLE_PROP_PRIMITIVE(VAlign);
	SAVABLE_PROP_PRIMITIVE(HAlign);
	SAVABLE_PROP_PRIMITIVE(Width);
	SAVABLE_PROP_PRIMITIVE(Height);
	SAVABLE_PROP_PRIMITIVE(Margin);
	SAVABLE_PROP_PRIMITIVE(Position);
	SAVABLE_PROP_PRIMITIVE(DrawTopDown);
	SAVABLE_PROP_PRIMITIVE(ShowSubLabel);
	SAVABLE_PROP_PRIMITIVE(MainCaption);
	SAVABLE_PROP_PRIMITIVE(SubCaption);

	std::wstring bgName;

	if(GetBg())
		bgName = GetBg()->GetName();

	SAVABLE_PRIMITIVE_NAMED(bgName,Background);

	return S_OK;

}

void Label::ReadFromIni(const char* group, const char* ini)
{
	// TODO: ADD IMPLEMENTATION FOR CINILoader

	WCHAR wGroup[MAX_PATH+1] = {0};
	MultiByteToWideChar(GetACP(),0,group,-1,wGroup,MAX_PATH);

	WCHAR wIni[MAX_PATH+1] = {0};
	MultiByteToWideChar(GetACP(),0,ini,-1,wIni,MAX_PATH);

//	INIPersister<psLoading> iniLoader(wIni,wGroup);
//	iniLoader.LoadOrSavePersistableClass(*this,L"Label");

	// post: this Labels values are read from the file ini from section group
/*
	//Alpha	= GetPrivateProfileInt(group,"DeskletLabelAlpha",255,ini);
	Shadow	= GetPrivateProfileInt(group,"DeskletLabelShadow",1,ini);
	Rotation= GetPrivateProfileInt(group,"DeskletLabelRotation",0,ini);
	VAlign	= GetPrivateProfileInt(group,"DeskletLabelVAlign",0,ini);
	HAlign	= GetPrivateProfileInt(group,"DeskletLabelHAlign",1,ini);
	Width	= GetPrivateProfileInt(group,"DeskletLabelWidth",200,ini);
	Height	= GetPrivateProfileInt(group,"DeskletLabelHeight",75,ini);
	Margin	= GetPrivateProfileInt(group,"DeskletLabelMargin",5,ini);
	ColorR	= GetPrivateProfileInt(group,"DeskletLabelColorR",0,ini);
	ColorG	= GetPrivateProfileInt(group,"DeskletLabelColorG",0,ini);
	ColorB	= GetPrivateProfileInt(group,"DeskletLabelColorB",0,ini);
	ColorA	= GetPrivateProfileInt(group,"DeskletLabelColorA",255,ini);
	Style	= GetPrivateProfileInt(group,"DeskletLabelStyle",0,ini);
	Size_	= GetPrivateProfileInt(group,"DeskletLabelSize",20,ini);
	Position= GetPrivateProfileInt(group,"DeskletLabelPosition",0,ini);
	DrawBlackOutline = 0;
	//DrawBlackOutline= GetPrivateProfileInt(group,"DeskletLabelDrawBlackOutline",0,ini);
	shadowOffsetX	= GetPrivateProfileInt(group,"DeskletLabelShadowOffsetX",7,ini);
	shadowOffsetY	= GetPrivateProfileInt(group,"DeskletLabelShadowOffsetY",7,ini);
	shadowColor		= GetPrivateProfileInt(group,"DeskletLabelShadowColor",0,ini);
	shadowAlpha		= GetPrivateProfileInt(group,"DeskletLabelShadowAlpha",255,ini);

	subShadowOffsetX= GetPrivateProfileInt(group,"DeskletLabelSubShadowOffsetX",7,ini);
	subShadowOffsetY= GetPrivateProfileInt(group,"DeskletLabelSubShadowOffsetY",7,ini);
	subShadowColor	= GetPrivateProfileInt(group,"DeskletLabelSubShadowColor",0,ini);
	subShadowAlpha	= GetPrivateProfileInt(group,"DeskletLabelSubShadowAlpha",255,ini);

	type1CompatibleDrawing = GetPrivateProfileInt(group,"DeskletLabelType1Comp",FALSE,ini) == TRUE;
	drawTopDown = GetPrivateProfileInt(group,"DeskletLabelTopDown",FALSE,ini) == TRUE;
	showSublabel = GetPrivateProfileInt(group,"DeskletLabelShowSub",TRUE,ini) == TRUE;

	subColorR	= GetPrivateProfileInt(group,"DeskletSubLabelColorR",0,ini);
	subColorG	= GetPrivateProfileInt(group,"DeskletSubLabelColorG",0,ini);
	subColorB	= GetPrivateProfileInt(group,"DeskletSubLabelColorB",0,ini);
	subColorA	= GetPrivateProfileInt(group,"DeskletSubLabelColorA",255,ini);
	subStyle	= GetPrivateProfileInt(group,"DeskletSubLabelStyle",0,ini);
	subSize		= GetPrivateProfileInt(group,"DeskletSubLabelSize",14,ini);
	subShadow	= GetPrivateProfileInt(group,"DeskletSubLabelShadow",1,ini);

	char tmp[1024];
	GetPrivateProfileString(group,"DeskletLabelFont","Arial",tmp,sizeof(tmp)/sizeof(tmp[0]),ini);
	MultiByteToWideChar(CP_ACP,0,tmp,-1,Font_,sizeof(Font_)/sizeof(Font_[0]));

	GetPrivateProfileString(group,"DeskletSubLabelFont","Verdana",tmp,sizeof(tmp)/sizeof(tmp[0]),ini);
	MultiByteToWideChar(CP_ACP,0,tmp,-1,subFont,sizeof(subFont)/sizeof(subFont[0]));

	GetPrivateProfileString(group,"DeskletLabelBackground","",tmp,sizeof(tmp)/sizeof(tmp[0]),ini);

	if(bg)
		bg->Release();

	if(strcmp(tmp,"")!=0)
		bg = LabelBackground::LoadLabelBackground(tmp);
	else
		bg = 0;
*/
}

void Label::WriteToIni(const char* group, const char* ini)
{
	// TODO: ADD IMPLEMENTATION FOR CINISaver
	WCHAR wGroup[MAX_PATH+1] = {0};
	MultiByteToWideChar(GetACP(),0,group,-1,wGroup,MAX_PATH);

	WCHAR wIni[MAX_PATH+1] = {0};
	MultiByteToWideChar(GetACP(),0,ini,-1,wIni,MAX_PATH);

//	INIPersister<psSaving> iniSaver(wIni,wGroup);
//	iniSaver.LoadOrSavePersistableClass(*this,L"Label");

	// post: this Labels values are written to the file ini in section group
/*
	//WritePrivateProfileInt(group,"DeskletLabelAlpha",Alpha,ini);
	WritePrivateProfileInt(group,"DeskletLabelShadow",Shadow,ini);
	WritePrivateProfileInt(group,"DeskletLabelRotation",Rotation,ini);
	WritePrivateProfileInt(group,"DeskletLabelVAlign",VAlign,ini);
	WritePrivateProfileInt(group,"DeskletLabelHAlign",HAlign,ini);
	WritePrivateProfileInt(group,"DeskletLabelWidth",Width,ini);
	WritePrivateProfileInt(group,"DeskletLabelHeight",Height,ini);
	WritePrivateProfileInt(group,"DeskletLabelMargin",Margin,ini);
	WritePrivateProfileInt(group,"DeskletLabelColorR",ColorR,ini);
	WritePrivateProfileInt(group,"DeskletLabelColorG",ColorG,ini);
	WritePrivateProfileInt(group,"DeskletLabelColorB",ColorB,ini);
	WritePrivateProfileInt(group,"DeskletLabelColorA",ColorA,ini);
	WritePrivateProfileInt(group,"DeskletLabelStyle",Style,ini);
	WritePrivateProfileInt(group,"DeskletLabelSize",Size_,ini);
	WritePrivateProfileInt(group,"DeskletLabelPosition",Position,ini);
	//WritePrivateProfileInt(group,"DeskletLabelDrawBlackOutline",DrawBlackOutline,ini);
	WritePrivateProfileInt(group,"DeskletLabelShadowOffsetX",shadowOffsetX,ini);
	WritePrivateProfileInt(group,"DeskletLabelShadowOffsetY",shadowOffsetY,ini);
	WritePrivateProfileInt(group,"DeskletLabelShadowColor",shadowColor,ini);
	WritePrivateProfileInt(group,"DeskletLabelShadowAlpha",shadowAlpha,ini);

	WritePrivateProfileInt(group,"DeskletLabelType1Comp",type1CompatibleDrawing ? TRUE : FALSE,ini);
	WritePrivateProfileInt(group,"DeskletLabelTopDown",drawTopDown ? TRUE : FALSE,ini);
	WritePrivateProfileInt(group,"DeskletLabelShowSub",showSublabel ? TRUE : FALSE,ini);

	WritePrivateProfileInt(group,"DeskletSubLabelColorR",subColorR,ini);
	WritePrivateProfileInt(group,"DeskletSubLabelColorG",subColorG,ini);
	WritePrivateProfileInt(group,"DeskletSubLabelColorB",subColorB,ini);
	WritePrivateProfileInt(group,"DeskletSubLabelColorA",subColorA,ini);
	WritePrivateProfileInt(group,"DeskletSubLabelStyle",subStyle,ini);
	WritePrivateProfileInt(group,"DeskletSubLabelSize",subSize,ini);
	WritePrivateProfileInt(group,"DeskletSubLabelShadow",subShadow,ini);
	WritePrivateProfileInt(group,"DeskletLabelSubShadowOffsetX",subShadowOffsetX,ini);
	WritePrivateProfileInt(group,"DeskletLabelSubShadowOffsetY",subShadowOffsetY,ini);
	WritePrivateProfileInt(group,"DeskletLabelSubShadowColor",subShadowColor,ini);
	WritePrivateProfileInt(group,"DeskletLabelSubShadowAlpha",subShadowAlpha,ini);

	char tmp[1024]  = {0};
	WideCharToMultiByte(CP_ACP,0,Font_,-1,tmp,sizeof(tmp)/sizeof(tmp[0]),0,0);
	WritePrivateProfileString(group,"DeskletLabelFont",tmp,ini);

	WideCharToMultiByte(CP_ACP,0,subFont,-1,tmp,sizeof(tmp)/sizeof(tmp[0]),0,0);
	WritePrivateProfileString(group,"DeskletSubLabelFont",tmp,ini);

	WritePrivateProfileString(group,"DeskletLabelBackground",(bg)?bg->GetName():"",ini);
*/
}


void Label::DrawTopDown(Graphics *graphics)
{
	// pre:  graphics is not 0
	// post: szText has been drawn on graphics topdown
/*
	OLECHAR text[800] = {0};
	OLECHAR tmpText[800] = {0};

	wcscpy(text

	int len = strlen(szText);
	int i=0;
	int j=0;
	while(i < 799 && j < len)
	{
		text[i] = tmpText[j];
		i++;
		text[i] = '\n';
		i++;
		j++;

	}
	text[i] = '\0';

	OLECHAR tmpLabel[2048] = {0};
	i=0;
	j=0;
	while(i < 1024 && m_SubCaption[j]!=0)
	{
		tmpLabel[i] = m_SubCaption[j];
		i++;
		tmpLabel[i] = '\n';
		i++;
		j++;
	}
	tmpLabel[i] = '\0';


	FontFamily  fontFamily(m_MainLabelProperties.GetFont());
	FontFamily  subFontFamily(m_SubLabelProperties.GetFont());
	SolidBrush  color(m_MainLabelProperties.GetColor());
	SolidBrush  subColor(m_SubLabelProperties.GetColor());

	GraphicsPath path(FillModeAlternate);
	GraphicsPath path2(FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);

	if(m_HAlign == 1)format.SetAlignment(StringAlignmentCenter);
	if(m_HAlign == 0)format.SetAlignment(StringAlignmentNear);
	if(m_HAlign == 2)format.SetAlignment(StringAlignmentFar);
	if(m_VAlign == 1)format.SetLineAlignment(StringAlignmentCenter);
	if(m_VAlign == 0)format.SetLineAlignment(StringAlignmentNear);
	if(m_VAlign == 2)format.SetLineAlignment(StringAlignmentFar);

	Rect rect(0,0,m_Width,m_Height);
	Rect sRect(rect.GetLeft(),rect.GetTop(),rect.GetRight(),rect.GetBottom());

	if(HAlign == 2)
	{
		format.SetAlignment(StringAlignmentFar);
		path2.AddString(tmpLabel,-1,&subFontFamily,subStyle,subSize,rect,&format);	
		Rect rc;
		path2.GetBounds(&rc);
		rect = Rect(0,0, m_Width - (rc.GetRight() - rc.GetLeft()) - (mBg ? mBg->GetMarginTop()+8 : 8) ,m_Height);
	}

	if(bg)bg->TransformRectTopDown(rect);

	path.AddString(text, -1, &fontFamily, Style, Size_, rect, &format);

	Rect rc;
	path.GetBounds(&rc);
	
	if(bg)bg->DrawTopDown(graphics,rc);

	//SIZE sizeMovement = {0, 0};
	//GraphicsPath pathShaddow(FillModeAlternate);

	if(HAlign != 2)
	{	
		Rect rect2(rc.GetRight() + (bg ? bg->GetMarginTop()+8 : 8),rect.GetTop(),Width  - rc.GetRight(),rect.GetBottom() - rect.GetTop());
		//format.SetLineAlignment(StringAlignmentNear);
		path2.AddString(tmpLabel,-1,&subFontFamily,subStyle,subSize,rect2,&format);
	}

	if(Shadow)
	{
		float ratio = 2.0;

		Size s;
		sRect.GetSize(&s);
		Bitmap bmp(s.Width,s.Height);
		Graphics tmp(&bmp);
		
		tmp.SetSmoothingMode(SmoothingModeAntiAlias);
		tmp.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		Matrix mx(1.0f/ratio,0,0,1.0f/ratio,0.7f,0.7f);
        tmp.SetTransform(&mx);
		Rect shadowRect(rect.GetLeft()+sizeMovement.cx, rect.GetTop()+sizeMovement.cy, (rect.GetRight()-rect.GetLeft()), (rect.GetBottom()-rect.GetTop()));
		pathShaddow.AddString(text, -1, &fontFamily, Style, Size, shadowRect, &format);
		tmp.FillPath(&black, &pathShaddow);
		InterpolationMode im = graphics->GetInterpolationMode();
		graphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		graphics->DrawImage(&bmp,sRect,0,0,s.Width/ratio,s.Height/ratio,UnitPixel,0,0,0);
		graphics->SetInterpolationMode(im);
	}

	if(Shadow || (subShadow && showSublabel &&  subLabel[0] != 0))
	{
		float ratio = 2.0;

    Size s;
		sRect.GetSize(&s);
		Bitmap bmp(s.Width,s.Height);
		Graphics tmp(&bmp);
		
		tmp.SetSmoothingMode(SmoothingModeAntiAlias);
		tmp.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		if(Shadow)
		{
			float offsetX = (float)shadowOffsetX / 10;
			float offsetY = (float)shadowOffsetY / 10;

			Matrix mx(1.0f/ratio,0,0,1.0f/ratio,offsetX,offsetY);
			tmp.SetTransform(&mx);
			Color c(shadowAlpha,GetRValue(shadowColor),GetGValue(shadowColor),GetBValue(shadowColor));
			SolidBrush b(c);
			tmp.FillPath(&b, &path);
		}
		if(subShadow && showSublabel && subLabel[0] != 0)
		{
			float offsetX = (float)subShadowOffsetX / 10;
			float offsetY = (float)subShadowOffsetY / 10;
			//tmp.SetTransform(0);
			Matrix mx(1.0f/ratio,0,0,1.0f/ratio,offsetX,offsetY);
			tmp.SetTransform(&mx);

			Color c(subShadowAlpha,GetRValue(subShadowColor),GetGValue(subShadowColor),GetBValue(subShadowColor));
			SolidBrush b(c);
			tmp.FillPath(&b, &path2);
		}

		InterpolationMode im = graphics->GetInterpolationMode();
		graphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		graphics->DrawImage(&bmp,sRect,0,0,s.Width/ratio,s.Height/ratio,UnitPixel,0,0,0);
		graphics->SetInterpolationMode(im);
	}


	if(showSublabel && subLabel[0] != 0)
		graphics->FillPath(&subColor, &path2);

	graphics->FillPath(&color, &path);

	//if(DrawBlackOutline)
	//	graphics->DrawPath(&blackOutline, &path);
	*/
}

Bitmap* Label::GetCachedBitmap()
{
	return m_CachedBitmap.get();
}

void Label::UpdateLabel(UINT quality, BYTE Alpha, bool sel)
{
	Bitmap* bmp = new Bitmap(m_Width, m_Height);
	Graphics g(bmp);

	Bitmap* finalBmp = bmp;
	
	ApplyQualityTo(quality,&g);
	Draw(&g, sel);

	bool newImage(false);
	newImage = true;
	finalBmp = new Bitmap(m_Width,m_Height);
	Graphics finalG(finalBmp);

	Matrix matrix(1, 0, 0, 1, 0, 0); 
	matrix.RotateAt(static_cast<float>(m_Rotation),PointF(static_cast<float>(m_Width/2), static_cast<float>(m_Height/2) ));
	finalG.SetTransform(&matrix);
	ApplyQualityTo(quality,&g);
	finalG.DrawImage(bmp,0,0,bmp->GetWidth(),bmp->GetHeight());

	SIZE s = {m_Width,m_Height};
	POINT p = {0};

	if(m_DockletParent)
		p = m_DockletParent->GetLabelPos();
	SetBitmap(finalBmp,Alpha,&p,&s );

	m_IsDirty = false;

	if(newImage)
	{
		m_CachedBitmap = boost::shared_ptr<Bitmap>( finalBmp);		
		delete bmp;
	}
	else
	{
		m_CachedBitmap = boost::shared_ptr<Bitmap>( bmp);
	}
 }


extern bool _noAntiAliasFonts;
void Label::Draw(Graphics *graphics, bool sel)
{
	// pre:  graphics is not 0
	// post: szText has been drawn on graphics according to this labels settings

	if(!graphics)return;

	if(m_DrawTopDown)
	{
		DrawTopDown(graphics);
		return;
	}

	graphics->SetTextRenderingHint(TextRenderingHintSystemDefault);
	graphics->SetTextContrast(100);

	bool normalLabelHasShadow = this->m_MainLabelProperties.GetHasShadow();
	if(m_Bg && m_Bg->GetNoShadowForNormalLabel(sel) )
		normalLabelHasShadow = false;

	//OLECHAR text[800] = {0};
	//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, -1, text, sizeof(text));


	FontFamily  fontFamily(m_MainLabelProperties.GetFont().c_str());
	FontFamily  subFontFamily(m_SubLabelProperties.GetFont().c_str());
	SolidBrush  color(m_MainLabelProperties.GetColor());
	SolidBrush  subColor(m_SubLabelProperties.GetColor());

	FontFamily* fontFamPtr    = &fontFamily;
	FontFamily* subFontFamPtr = &subFontFamily;

	if(!fontFamily.IsAvailable())
		fontFamPtr =  const_cast<FontFamily*>(FontFamily::GenericSansSerif());
	if(!subFontFamily.IsAvailable())
		subFontFamPtr = const_cast<FontFamily*>(FontFamily::GenericSansSerif());	
	
	GraphicsPath path(FillModeAlternate);
	GraphicsPath path2(FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);

	if(m_HAlign == 1)format.SetAlignment(StringAlignmentCenter);
	if(m_HAlign == 0)format.SetAlignment(StringAlignmentNear);
	if(m_HAlign == 2)format.SetAlignment(StringAlignmentFar);
	if(m_VAlign == 1)format.SetLineAlignment(StringAlignmentCenter);
	if(m_VAlign == 0)format.SetLineAlignment(StringAlignmentNear);
	if(m_VAlign == 2)format.SetLineAlignment(StringAlignmentFar);

	Rect rect(0,0,m_Width,m_Height);
	Rect sRect(rect.GetLeft(),rect.GetTop(),rect.GetRight(),rect.GetBottom());


	InterpolationMode ipm = graphics->GetInterpolationMode();
	SmoothingMode mode = graphics->GetSmoothingMode();

	if(::_noAntiAliasFonts)
		graphics->SetSmoothingMode(SmoothingModeNone);

	if(m_VAlign == 2)
	{
		format.SetLineAlignment(StringAlignmentFar);

		path2.AddString(m_SubCaption.c_str(),-1,subFontFamPtr,m_SubLabelProperties.GetStyle(),(REAL)m_SubLabelProperties.GetSize(),rect,&format);	
 
		if(m_ShowSubLabel && m_SubCaption[0] != 0)
			AveDrawString(graphics,&subColor,m_SubCaption.c_str(),subFontFamPtr,(FontStyle) m_SubLabelProperties.GetStyle(),(REAL)m_SubLabelProperties.GetSize(),rect,&format,m_SubLabelProperties.GetHasShadow(), m_SubLabelProperties.GetShadowProperties());


		Rect rc;
		path2.GetBounds(&rc);
		rect = Rect(rect.GetLeft(),rect.GetTop(),rect.GetRight(),rect.Height - ( rc.Height + (m_Bg ? m_Bg->GetSubLabelMargin(sel) : 0) ) );
	}

	if(m_Bg)m_Bg->TransformRect(rect,sel);
	path.AddString(m_MainCaption.c_str(), -1, fontFamPtr, m_MainLabelProperties.GetStyle() , (REAL)m_MainLabelProperties.GetSize(), rect, &format);

	Rect rc;
	path.GetBounds(&rc);

	if(::_noAntiAliasFonts)
		graphics->SetSmoothingMode(mode);
	

	if(m_Bg)
	{
		if(sel || m_Bg->HasNormalLabel()  )
			m_Bg->Draw(graphics,rc,sel);
	}

	graphics->SetInterpolationMode(ipm);
	graphics->SetSmoothingMode(mode);

	if(::_noAntiAliasFonts)
		graphics->SetSmoothingMode(SmoothingModeNone);

	if(m_VAlign != 2)
	{
		Rect rect2(rect.GetLeft(),rc.GetBottom()+ (m_Bg ? m_Bg->GetSubLabelMargin() : 0),rect.GetRight()-rect.GetLeft(),m_Height - rc.GetBottom());

		boost::shared_ptr<StringFormat>tmpFormat ( format.Clone() );
		tmpFormat->SetLineAlignment(StringAlignmentNear);
		path2.AddString(m_SubCaption.c_str(),-1,subFontFamPtr,m_SubLabelProperties.GetStyle(),(REAL)m_SubLabelProperties.GetSize(),rect2,&format);

		if(m_ShowSubLabel &&  m_SubCaption[0] != 0)
			AveDrawString(graphics,&subColor,m_SubCaption.c_str(),subFontFamPtr,(FontStyle) m_SubLabelProperties.GetStyle(),(REAL)m_SubLabelProperties.GetSize(),rect2,&(*tmpFormat),m_SubLabelProperties.GetHasShadow(), m_SubLabelProperties.GetShadowProperties());
	}
	

	//if(normalLabelHasShadow)
		AveDrawString(graphics,&color,m_MainCaption.c_str(),fontFamPtr,(FontStyle) m_MainLabelProperties.GetStyle(),(REAL)m_MainLabelProperties.GetSize(),rect,&format,normalLabelHasShadow,m_MainLabelProperties.GetShadowProperties());
	//else
	//	graphics->FillPath(&color, &path);

	/*if(!m_SubLabelProperties.GetHasShadow() && m_ShowSubLabel && m_SubCaption[0] != 0)
	{
		graphics->FillPath(&subColor, &path2);
	}*/

	if(::_noAntiAliasFonts)
		graphics->SetSmoothingMode(mode);

}


LRESULT Label:: OnLeftButtonDoubleClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_DockletParent != NULL)
	{
		LPARAM newLParam = MAKELONG(-1,-1);
		return m_DockletParent->SendMessage(uMsg,wParam, newLParam);
	}
	else
	{
		return 0;
	}
}

LRESULT Label:: OnLeftButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_DockletParent != NULL)
	{
		LPARAM newLParam = MAKELONG(-1,-1);
		return m_DockletParent->SendMessage(uMsg,wParam, newLParam);
	}
	else
	{
		return 0;
	}
}

LRESULT Label::OnRightButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if(m_DockletParent != NULL)
	{
		LPARAM newLParam = MAKELONG(-1,-1);
		return m_DockletParent->SendMessage(uMsg,wParam, newLParam);
	}
	else
	{
		return 0;
	}
}

LRESULT Label::OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_DockletParent != NULL)
	{
		LPARAM newLParam = MAKELONG(-1,-1);
		return m_DockletParent->SendMessage(uMsg,wParam, newLParam);
	}
	else
	{
		return 0;
	}

	return 0;
}

LRESULT Label::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled= TRUE;

	if(m_DockletParent)
	{
		if(wParam == FADER_TIMER)
		{
			int labelAlphaAddition = 30;//m_DockletParent->GetDocklet()->GetAlpha() / 5;
			if(GetFadingIn() == true)
			{
				ShowWindow(SW_SHOWNA);
				if(((INT)GetAlpha()) + labelAlphaAddition >= m_DockletParent->GetDocklet()->GetAlpha())
				{
					SetAlpha( m_DockletParent->GetDocklet()->GetAlpha() );
					KillTimer(FADER_TIMER);
				}
				else
				{
					SetAlpha((BYTE)(GetAlpha() + labelAlphaAddition) );
				}
			}
			else
			{
				if(((INT)GetAlpha()) - labelAlphaAddition < 0)
				{
					ShowWindow(SW_HIDE);
					SetAlpha(0);
					KillTimer(FADER_TIMER);
				}
				else
				{
					SetAlpha((BYTE)(GetAlpha() - labelAlphaAddition));
				}
			}
			UpdateAlphaOnly(GetAlpha() );
			//m_DockletParent->UpdateLabelWindow();
		}
	}
	return 1;
}

LRESULT Label::OnWindowsPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_DockletParent)
	{
		bHandled= TRUE;
		WINDOWPOS* w = (WINDOWPOS*)lParam;
		if(!m_DockletParent->GetIsShowCased() &&  m_DockletParent->GetStyle() == STYLE_BOTTOM)
		{
			w->hwndInsertAfter = HWND_BOTTOM;
		}
	}

	return 1;
}

void Label::UpdateAlphaOnly(BYTE b)
{
	if(m_IsDirty || m_CachedBitmap.get() == NULL)
		m_DockletParent->UpdateLabelWindow();
	else
	{
		if(b!=255)
			CLayeredWindowImpl<Label>::SetAlpha(b);
		else
		{
			SIZE s = { m_CachedBitmap->GetWidth(), m_CachedBitmap->GetHeight() };
			SetBitmap(m_CachedBitmap.get(),255,&m_DockletParent->GetLabelPos(),&s);
		}
	}
}