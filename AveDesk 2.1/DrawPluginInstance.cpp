// Description:		Definition of class DrawPluginInstance
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
//
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!		

#include "stdafx.h"
#include "drawplugininstance.h"

DrawPluginInstance::DrawPluginInstance(DrawPlugin* plugin, HWND hwndDesklet,
	const char* szIniFile, const char* szIniGroup)
	: plugin(plugin)
{

	pointer = plugin->Create(hwndDesklet,szIniFile,szIniGroup);
	plugin->AddRef();
}
DrawPluginInstance::~DrawPluginInstance()
{
	if(plugin)
	{
		plugin->Destroy(pointer);
		plugin->Release();
	}
}
		
DrawPlugin* DrawPluginInstance::GetPlugin()
{
	return plugin;
}

bool DrawPluginInstance::BeforeDraw(HWND hwndDesklet,Graphics* g,UINT left, UINT top, UINT width, UINT height, ColorMatrix* m, Matrix* matrix,BOOL* modified, BOOL isSelected, BOOL isMouseOn)
{
	return plugin && plugin->BeforeDraw(pointer,hwndDesklet,g,left, top, width,height,m,matrix,modified,isSelected,isMouseOn);
}

bool DrawPluginInstance::AfterDraw(
  HWND hwndDesklet,
  Graphics* g,  Bitmap* bmp,
  UINT left, UINT top, UINT width, UINT height,
  ColorMatrix* m, Matrix* matrix,
  BOOL* modified, BOOL isSelected, BOOL isMouseOn
  )
{
	return plugin && plugin->AfterDraw(pointer,
    hwndDesklet, g, bmp,
    left, top, width, height,
    m, matrix,
    modified,isSelected,isMouseOn);
}

bool DrawPluginInstance::Event(UINT event)
{
	return plugin && plugin->Event(pointer,event);
}

bool DrawPluginInstance::Configure(HWND owner)
{
	return plugin && plugin->Configure(pointer,owner);
}

void DrawPluginInstance::Save(const char* szIniFile,const char* szIniGroup)
{
	if(plugin)plugin->Save(pointer,szIniFile,szIniGroup);
}