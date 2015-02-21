// Description:		Definition of class DrawPlugin
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


#ifndef DRAW_PLUGIN_H_2011911
#define DRAW_PLUGIN_H_2011911

#include "avedesk.h"
#include <gdiplus.h>
#include <map>
#include <string>
#include "helper_funcs.h"

class DrawPluginInstance;

class DrawPlugin
{
private:
	enum DPType{ DP_INVALID = -1, DP_DLL = 0, DP_SCRIPT = 1};

	friend class DrawPluginInstance;

	char name[1024];
	char author[1024];
	char notes[1024];
	int  version;
  char versionStr[16];
  int  type; // -1 == invalid, 0 == dll, 1 == script

	std::string filename;

	HMODULE hMod;
	ULONG refCount;

  // effectlet related
  CComPtr<IXMLDOMDocument> doc;
	
	typedef void* (__stdcall *HOnCreate)(HWND,HINSTANCE,const char*,const char*);
	typedef void  (__stdcall *HOnSave)(void*,const char*,const char*);
	typedef void  (__stdcall *HOnDestroy)(void*);
	typedef void  (__stdcall *HOnConfigure) (void*,HWND);
	typedef void  (__stdcall *HOnEvent) (void*,UINT);
	typedef BOOL  (__stdcall *HDrawEvent)     (void*,HWND,Graphics* g,            UINT left, UINT top, UINT width, UINT height, ColorMatrix* m, Matrix* matrix,BOOL*,BOOL,BOOL);
	typedef BOOL  (__stdcall *HAfterDrawEvent)(void*,HWND,Graphics* g,Bitmap* bmp,UINT left, UINT top, UINT width, UINT height, ColorMatrix* m, Matrix* matrix,BOOL*,BOOL,BOOL);
	typedef void  (__stdcall *HOnDrawPluginInfo)(char*,char*,char*,int*);
  // script exclusive hidden API
  typedef void  (__stdcall *HOnSetParent)(void*, DrawPlugin*);

	HOnCreate			_OnCreate;
	HOnSave				_OnSave;
	HOnDestroy			_OnDestroy;
	HOnConfigure		_OnConfigure;
	HDrawEvent			_BeforeDraw;
	HAfterDrawEvent		_AfterDraw;
	HOnEvent			_OnEvent;
	HOnDrawPluginInfo	_OnInformation;

  HOnSetParent _OnSetParent;

	DrawPlugin(const char* dll);
	~DrawPlugin();

	void* Create(HWND hwndDesklet,const char* szIniFile,const char* szIniGroup);
	void  Save(void* pointer, const char* szIniFile,const char* szIniGroup);
	bool  BeforeDraw(void* pointer, HWND hwndDesklet,Graphics* g,UINT left, UINT top, UINT width, UINT height,ColorMatrix* m, Matrix* matrix,BOOL* modified, BOOL isSelected, BOOL isMouseOn);
	bool  AfterDraw(void* pointer,HWND hwndDesklet,Graphics* g,Bitmap* bmp,UINT left, UINT top, UINT width, UINT height,ColorMatrix* m, Matrix* matrix,BOOL* modified, BOOL isSelected, BOOL isMouseOn); 
	bool  Event(void* pointer, UINT event);
	bool  Configure(void* pointer,HWND owner);
	void  Destroy(void* pointer);

public:
	const char* GetName()const;
	const char* GetAuthor()const;
	const char* GetNotes()const;
	int   GetVersion()const;
  const char* GetStrVersion() const;

  IXMLDOMDocument* GetXML(){return doc;};

	std::string GetFilename(){return filename;};

	ULONG AddRef();
	ULONG Release();

	bool IsValidPlugin();

	typedef std::map<std::string,DrawPlugin*> PluginsMap;
	static PluginsMap Plugins;

	static DrawPlugin* LoadPlugin(const char* dll);

};

#endif//DRAW_PLUGIN_H_2011911