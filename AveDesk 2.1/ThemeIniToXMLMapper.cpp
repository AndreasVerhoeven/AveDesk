// ThemeIniToXMLMapper.cpp: implementation of the CThemeIniToXMLMapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThemeIniToXMLMapper.h"
#include "AveDeskGlobalSettings.hpp"
#include "helper_funcs.h"
#include "XMLSaver.h"
#include "Label.h"
#include "LabelBackground.h"

extern AveDeskGlobalSettings _globalSettings;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThemeIniToXMLMapper::CThemeIniToXMLMapper()
{

}

CThemeIniToXMLMapper::~CThemeIniToXMLMapper()
{

}

void CThemeIniToXMLMapper::MapOldThemeToNewTheme(const WCHAR* ini, const WCHAR* theme)
{
	if(NULL == ini || NULL == theme)
		return;

	char aIni[MAX_PATH] = {0};
	strcpy(aIni,WStringToString(ini).c_str());	

	int count = GetPrivateProfileIntA("Settings","DeskletCount",0,aIni);

	CXMLSaver saver(L"AveDesk");
	saver.AddNode(L"theme");

	for(int i = 0; i < count; ++i)
	{
		saver.AddNode(L"desklet");

		std::stringstream strStream;
		strStream << "Desklet-" << i;
		ReadDeskletAndMapItToXML(saver,aIni,strStream.str().c_str());
		saver.GoToParentNode();
		
	}

	saver.Save(theme);
}

void CThemeIniToXMLMapper::MapSettings(const WCHAR* ini, const WCHAR* xml)
{	
	if(NULL == ini || NULL == xml)
		return;

	char aIni[MAX_PATH] = {0};
	strcpy(aIni,WStringToString(ini).c_str());

	char* settings = "Settings";

	_globalSettings.SetShowTrayIcon( GetPrivateProfileIntA(settings,"ShowTrayIcon",_globalSettings.GetShowTrayIcon() ? TRUE : FALSE,aIni) == TRUE);
	_globalSettings.SetClickMode( GetPrivateProfileIntA(settings,"ClickMode",_globalSettings.GetClickMode(),aIni));
	_globalSettings.SetGridX( GetPrivateProfileIntA(settings,"GridX",_globalSettings.GetGridX(),aIni));
	_globalSettings.SetGridY( GetPrivateProfileIntA(settings,"GridY",_globalSettings.GetGridY(),aIni));
	_globalSettings.SetGridXMargin( GetPrivateProfileIntA(settings,"GridXMargin",_globalSettings.GetGridXMargin(),aIni));
	_globalSettings.SetGridYMargin( GetPrivateProfileIntA(settings,"GridYMargin",_globalSettings.GetGridYMargin(),aIni));
	_globalSettings.SetHoverSelection( GetPrivateProfileIntA(settings,"HoverSelection",_globalSettings.GetHoverSelection() ? TRUE : FALSE,aIni) == TRUE);
	_globalSettings.SetHoverSelectionDelay( GetPrivateProfileIntA(settings,"HoverSelectionDelayInMs",_globalSettings.GetHoverSelectionDelay(),aIni));

	_globalSettings.SetIconIndex(GetPrivateProfileIntA(settings,"IconIndex",_globalSettings.GetIconIndex(),aIni));

	char tmp[MAX_PATH] = {0};
	GetPrivateProfileStringA(settings,"IconFile",tmp,tmp,MAX_PATH,aIni);
	if(tmp[0] != '\0')
		_globalSettings.SetIconFileName(StringToWString(tmp));

	CXMLSaver saver(L"AveDesk");
	saver.AddNode(L"GlobalSettings");
	_globalSettings.Save(saver);
	saver.GoToParentNode();
	saver.AddNode(L"DefaultDeskletSettings");
	ReadDefaultDeskletAndMapItToXML(saver,aIni,"DEFAULT");
}

#define INI2LABEL_INT(o,m,d)						\
	lbl.##m( GetPrivateProfileIntA(section,o,d,ini));	

#define INI2LABEL_BOOL(o,m,d)												\
	lbl.##m( GetPrivateProfileIntA(section,o,d ? TRUE : FALSE,ini) == TRUE);	

void CThemeIniToXMLMapper::ReadLabelAndMapItToXML(ISaver& saver, const char* ini, const char* section)
{
	if(NULL == ini || NULL == section)
		return;

	Label lbl;

	char lblBuf[1024] = {0};
	GetPrivateProfileStringA(section,"Label","",lblBuf,1024,ini);
	lbl.SetMainCaption(StringToWString(lblBuf));

	INI2LABEL_BOOL("DeskletLabelShadow",GetMainLabelProperties().SetHasShadow,true);
	INI2LABEL_BOOL("DeskletSubLabelShadow",GetSubLabelProperties().SetHasShadow,true);
	INI2LABEL_INT("DeskletLabelRotation",SetRotation,0);
	INI2LABEL_INT("DeskletLabelVAlign",SetVAlign,0);
	INI2LABEL_INT("DeskletLabelHAlign",SetHAlign,1);
	INI2LABEL_INT("DeskletLabelWidth",SetWidth,200);
	INI2LABEL_INT("DeskletLabelHeight",SetHeight,200);
	INI2LABEL_INT("DeskletLabelMargin",SetMargin,5);
	INI2LABEL_INT("DeskletLabelPosition",SetPosition,0);
	INI2LABEL_BOOL("DeskletLabelTopDown",SetDrawTopDown,false);
	INI2LABEL_BOOL("DeskletLabelShowSub",SetShowSubLabel,true);


	char buf[MAX_PATH] = {0};
	GetPrivateProfileStringA(section,"DeskletLabelBackground","",buf,MAX_PATH,ini);
	lbl.SetBg( LabelBackground::LoadLabelBackground(StringToWString(buf)) );

	COLORREF cr = RGB( GetPrivateProfileIntA(section,"DeskletLabelColorR",0,ini),
							GetPrivateProfileIntA(section,"DeskletLabelColorG",0,ini),
							GetPrivateProfileIntA(section,"DeskletLabelColorB",0,ini)
					);

	Color c;
	c.SetFromCOLORREF(cr);
	lbl.GetMainLabelProperties().SetColor(c);
	INI2LABEL_INT("DeskletLabelStyle",GetMainLabelProperties().SetStyle,0);
	INI2LABEL_INT("DeskletLabelSize",GetMainLabelProperties().SetSize,0);
	INI2LABEL_INT("DeskletLabelShadowOffsetX",GetMainLabelProperties().GetShadowProperties().SetXOffset,0);
	INI2LABEL_INT("DeskletLabelShadowOffsetY",GetMainLabelProperties().GetShadowProperties().SetYOffset,0);
	cr = GetPrivateProfileIntA(section,"DeskletLabelShadowColor",0,ini);
	c.SetFromCOLORREF(cr);
	lbl.GetMainLabelProperties().GetShadowProperties().SetColor(c);
	//INI2LABEL_INT("DeskletLabelShadowAlpha",GetMainLabelProperties().GetShadowProperties().SetAlpha,255);

	GetPrivateProfileStringA(section,"DeskletLabelFont","Arial",buf,MAX_PATH,ini);
	lbl.GetMainLabelProperties().SetFont(StringToWString(buf));

	cr = RGB( GetPrivateProfileIntA(section,"DeskletSubLabelColorR",0,ini),
							GetPrivateProfileIntA(section,"DeskletSubLabelColorG",0,ini),
							GetPrivateProfileIntA(section,"DeskletSubLabelColorB",0,ini)
					);

	c.SetFromCOLORREF(cr);
	lbl.GetSubLabelProperties().SetColor(c);
	INI2LABEL_INT("DeskletSubLabelStyle",GetSubLabelProperties().SetStyle,0);
	INI2LABEL_INT("DeskletSubLabelSize",GetSubLabelProperties().SetSize,0);
	INI2LABEL_INT("DeskletLabelSubShadowOffsetX",GetSubLabelProperties().GetShadowProperties().SetXOffset,0);
	INI2LABEL_INT("DeskletLabelSubShadowOffsetY",GetSubLabelProperties().GetShadowProperties().SetYOffset,0);
	cr = GetPrivateProfileIntA(section,"DeskletLabelSubShadowColor",0,ini);
	c.SetFromCOLORREF(cr);
	lbl.GetSubLabelProperties().GetShadowProperties().SetColor(c);
	//INI2LABEL_INT("DeskletLabelShadowAlpha",GetSubLabelProperties().GetShadowProperties().SetAlpha,255);

	GetPrivateProfileStringA(section,"DeskletSubLabelFont","Arial",buf,MAX_PATH,ini);
	lbl.GetMainLabelProperties().SetFont(StringToWString(buf));

	saver.SavePersistableClass(lbl,L"label");
	
}

#define INI2XML(o,n,d)												\
	buf[0] = '\0';													\
	GetPrivateProfileStringA(section,o,d,buf,MAX_PATH,ini);			\
	saver.SaveString(StringToWString(buf),n);						

void CThemeIniToXMLMapper::ReadDefaultDeskletAndMapItToXML(ISaver& saver, const char* ini, const char* section)
{
	if(NULL == ini || NULL == section)
		return;

	char buf[MAX_PATH] = {0};

	INI2XML("xPos",L"xPos","0");
	INI2XML("yPos",L"yPos","0");
	INI2XML("Width",L"width","128");
	INI2XML("Height",L"height","128");
	INI2XML("LockPosition",L"lockPosition","0");
	INI2XML("Alpha",L"Alpha","255");
	INI2XML("Rotation",L"rotation","0");
	INI2XML("Style",L"style","3");
	INI2XML("LabelType",L"labelType","0");
	INI2XML("LabelOnlyOnMouseOver",L"labelOnlyOnMouseOver","0");
	INI2XML("UseColorization",L"iseColorization","0");
	INI2XML("Saturation",L"saturation","0");
	INI2XML("Hue",L"hue","0");
	INI2XML("IgnoreLeftClicks",L"ignoreLeftClicks","0");
	INI2XML("IgnoreRightClicks",L"ignoreRightClicks","0");
	INI2XML("IsCT",L"isClickThru","0");
	INI2XML("STG",L"snapToGrid","0");
	INI2XML("DrawShadow",L"drawShadow","0");
	INI2XML("ShadowX",L"shadowX","4");
	INI2XML("ShadowY",L"shadowY","4");
	INI2XML("ShadowGray",L"shadowGray","100");
	INI2XML("ShadowAlpha",L"shadowAlpha","255");
	INI2XML("ShadowColor",L"shadowColor","0");
	INI2XML("Image",L"Image","");
	INI2XML("Effect",L"effectFile","");
	
	ReadLabelAndMapItToXML(saver,ini,section);
}

void CThemeIniToXMLMapper::ReadDeskletAndMapItToXML(ISaver& saver, const char* ini, const char* section)
{
	if(NULL == ini || NULL == section)
		return;

	char buf[MAX_PATH] = {0};


	INI2XML("DeskletFile",L"DLL","");
	INI2XML("DeskletX",L"xPos","0");
	INI2XML("DeskletY",L"yPos","0");
	INI2XML("DeskletWidth",L"width","128");
	INI2XML("DeskletHeight",L"height","128");
	INI2XML("DeskletLockPosition",L"lockPosition","0");
	INI2XML("DeskletAlpha",L"Alpha","255");
	INI2XML("DeskletRotation",L"rotation","0");
	INI2XML("DeskletStyle",L"style","3");
	INI2XML("DeskletLabelType",L"labelType","0");
	INI2XML("DeskletLabelOnlyOnMouseOver",L"labelOnlyOnMouseOver","0");
	INI2XML("DeskletUseColorization",L"iseColorization","0");
	INI2XML("DeskletSaturation",L"saturation","0");
	INI2XML("DeskletHue",L"hue","0");
	INI2XML("DeskletIgnoreLeftClicks",L"ignoreLeftClicks","0");
	INI2XML("DeskletIgnoreRightClicks",L"ignoreRightClicks","0");
	INI2XML("DeskletIsCT",L"isClickThru","0");
	INI2XML("DeskletSTG",L"snapToGrid","0");
	INI2XML("DeskletDrawShadow",L"drawShadow","0");
	INI2XML("DeskletShadowX",L"shadowX","4");
	INI2XML("DeskletShadowY",L"shadowY","4");
	INI2XML("DeskletShadowGray",L"shadowGray","100");
	INI2XML("DeskletShadowAlpha",L"shadowAlpha","255");
	INI2XML("DeskletShadowColor",L"shadowColor","0");
	INI2XML("Image",L"Image","");
	INI2XML("DeskletEffect",L"effectFile","");
	INI2XML("DeskletQuality",L"quality","2");
	
	ReadLabelAndMapItToXML(saver,ini,section);

	std::wstring output;

	char str[32000] = {0};
	GetPrivateProfileSectionA(section,str,32000,ini);
	char* ptr = str;
	while(*ptr != '\0')
	{
		output += StringToWString(ptr) + L"\n";
		ptr = ptr + strlen(ptr) + 1;
	}

	saver.SaveString(output,L"ownerSettings");

}
