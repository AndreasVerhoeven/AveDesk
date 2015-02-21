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


#ifndef DRAW_PLUGIN_INSTANCE_H_2011911
#define DRAW_PLUGIN_INSTANCE_H_2011911

#include "drawplugin.h"
#include "ACTIVSCP.h"

class DrawPluginInstance
{
	private:
		void* pointer;
		DrawPlugin* plugin;

	public:
		DrawPluginInstance(DrawPlugin* plugin, HWND hwndDesklet,const char* szIniFile, const char* szIniGroup);
		~DrawPluginInstance();
		
		DrawPlugin* GetPlugin();
		bool BeforeDraw(HWND hwndDesklet,Graphics* g, UINT left, UINT top,UINT width, UINT height,ColorMatrix* m, Matrix* matrix,BOOL* modified, BOOL isSelected, BOOL isMouseOn);
		bool AfterDraw(HWND hwndDesklet,Graphics* g,Bitmap* bmp,UINT left, UINT top,UINT width, UINT height,ColorMatrix* m, Matrix* matrix,BOOL* modified, BOOL isSelected, BOOL isMouseOn); 
		bool Event(UINT event);
		bool Configure(HWND ownwer);
		void Save(const char* szIniFile,const char* szIniGroup);
};

#endif//DRAW_PLUGIN_INSTANCE_H_2011911
