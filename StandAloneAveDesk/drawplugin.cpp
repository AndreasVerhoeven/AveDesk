// Description:		Implementation of class DrawPlugin
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
#include "drawplugin.h"
#include "effectlet.h"
#include <io.h>
#include <sstream>
#include <string>


DrawPlugin::PluginsMap DrawPlugin::Plugins;

DrawPlugin* DrawPlugin::LoadPlugin(const char* dll)
{
	if(NULL == dll)
		return NULL;

	DrawPlugin* dp = NULL;
	
	std::wstringstream ws;
	ws << StringToWString(dll);
	std::wstring mutexName = ws.str();

	for(std::wstring::iterator wsIter = mutexName.begin(); wsIter != mutexName.end(); ++wsIter)
	{
		wchar_t& c = *wsIter;
		if(L'\\' == c || c == L'/')
			c = L'~';
	}

	HANDLE mutexHandle = OpenMutex(NULL, TRUE,  mutexName.c_str() );
	if( NULL != mutexHandle)
	{
		WaitForSingleObject(mutexHandle,INFINITE);	
		CloseHandle(mutexHandle);
	}
	else
	{
		mutexHandle = CreateMutex(NULL, TRUE, mutexName.c_str() );
	}


	char lowercaseDLL[MAX_PATH];
	strcpy(lowercaseDLL,dll);
	lowercase(lowercaseDLL);
	std::string filename = std::string(lowercaseDLL);

	PluginsMap::iterator iter = Plugins.find(filename);
	if(iter != Plugins.end() )
	{
		iter->second->AddRef();
		dp = iter->second;
	}
	else
	{
		DrawPlugin* plugin = new DrawPlugin(dll);
		if(plugin->IsValidPlugin())
		{
			dp = plugin;
		}
	}

	ReleaseMutex(mutexHandle);


	return dp;
}

DrawPlugin::DrawPlugin(const char* dll) :
	hMod(0), refCount(1)
{
	USES_CONVERSION;
    
  if(dll)
	{
		strcpy(name,"");
		strcpy(author,"");
		strcpy(notes,"");
    strcpy(versionStr,"");
		version = 0;
    type = -1;

		FILE *fp = fopen(dll, "rb");
    if( fp )
    {
      char hd[2] = {0,0};
      fread(hd, 2, 1, fp);
      fclose(fp);

      if( !strncmp(hd, "MZ", 2) )
        type = 0;
      else
        type = 1;
    }
    else
    {
      type = -1;
    }
    switch( type )
    {
    case 0:
      {
        hMod = LoadLibrary(StringToWString(dll).c_str());
		    if(hMod)
		    {
				    _OnInformation	= (HOnDrawPluginInfo)	GetProcAddress(hMod,"OnDrawPluginInfo");
				    _OnCreate		= (HOnCreate)			GetProcAddress(hMod,"OnCreate");

				    if(_OnInformation && _OnCreate)
				    {
					    char lowercaseDLL[MAX_PATH];
					    strcpy(lowercaseDLL,dll);
					    lowercase(lowercaseDLL);
					    filename = std::string(lowercaseDLL);

					    Plugins.insert( std::make_pair(filename,this) );

					    _OnInformation(name,author,notes,&version);
              sprintf(versionStr, "%ld.%ld", version / 10, version % 10);

					    _OnEvent		  = (HOnEvent)			  GetProcAddress(hMod,"OnEvent");
					    _OnDestroy		= (HOnDestroy)			GetProcAddress(hMod,"OnDestroy");
					    _OnConfigure	= (HOnConfigure)		GetProcAddress(hMod,"OnConfigure");
					    _OnSave			  = (HOnSave)				  GetProcAddress(hMod,"OnSave");
					    _BeforeDraw		= (HDrawEvent)			GetProcAddress(hMod,"BeforeDraw");
					    _AfterDraw		= (HAfterDrawEvent)	GetProcAddress(hMod,"AfterDraw");
              _OnSetParent  = NULL; // this one is for scripts only

              type = 0;
				    }
				    else
				    {
					    FreeLibrary(hMod);
					    hMod = 0;
              type = -1;
				    }
        }  
      }
      break;
    case 1:
      {
        doc.CoCreateInstance(CLSID_DOMDocument);
        doc->put_preserveWhiteSpace(VARIANT_TRUE);
        VARIANT_BOOL bSucc = VARIANT_FALSE;
        
		//CComBSTR bstrDll(A2W(dll));
		//CComVariant varDll(bstrDll);
        //HRESULT _hRes = doc->load(varDll, &bSucc);

		CComBSTR bstrDll(A2W(dll)), bstrXML;
		CComVariant varDll(bstrDll);
		FILE *fp = fopen(dll, "rb");
		int iLen = _filelength( _fileno(fp) );
		char *strTemp = new char[iLen+1];
		fread(strTemp, iLen, 1, fp);
		strTemp[iLen] = 0;
		fclose(fp);
		char* xmlStart = strstr(strTemp,"<?xml");
		if(!xmlStart)
			xmlStart = strTemp;

		//xmlStart = strstr(xmlStart+1,"<");
		bstrXML = xmlStart;
		delete [] strTemp;
        HRESULT _hRes = doc->loadXML(bstrXML, &bSucc);

        if( SUCCEEDED(_hRes) && bSucc )
        {
          type = 1;
          CComPtr<IXMLDOMNode> nod;

          // get name
          if( SUCCEEDED( doc->selectSingleNode(L"effectlet/info/name", &nod) ) && NULL != nod.p )
          {
            CComBSTR str = NULL;
            nod->get_text(&str);
            if( str )
            {
              strncpy(name, W2A(str), sizeof(name));
              name[sizeof(name)-1] = 0;
            }
            nod = NULL;
          }
          
          // get author
          if( SUCCEEDED( doc->selectSingleNode(L"effectlet/info/author", &nod) ) && NULL != nod.p )
          {
            CComBSTR str = NULL;
            nod->get_text(&str);
            if( str )
            {
              strncpy(author, W2A(str), sizeof(author));
              author[sizeof(author)-1] = 0;
            }
            nod = NULL;
          }
          
          // get notes
          if( SUCCEEDED( doc->selectSingleNode(L"effectlet/info/notes", &nod) ) && NULL != nod.p )
          {
            CComBSTR str = NULL;
            nod->get_text(&str);
            if( str )
            {
              strncpy(notes, W2A(str), sizeof(notes));
              notes[sizeof(notes)-1] = 0;
            }
            nod = NULL;
          }
          
          // get version
          if( SUCCEEDED( doc->selectSingleNode(L"effectlet/info/version", &nod) ) && NULL != nod.p )
          {
            CComBSTR str = NULL;
            nod->get_text(&str);
            if( str )
            {
              version = _wtoi(str);
              strncpy(versionStr, W2A(str), 15);
            }
            nod = NULL;
          }
          
					char lowercaseDLL[MAX_PATH];
					strcpy(lowercaseDLL,dll);
					lowercase(lowercaseDLL);
					filename = std::string(lowercaseDLL);

          _OnInformation= (HOnDrawPluginInfo) &CEffectLet::OnDrawPluginInfo;	
          _OnCreate		  = (HOnCreate)			    &CEffectLet::OnCreate;
          _OnEvent		  = (HOnEvent)			    &CEffectLet::OnEvent;
          _OnDestroy		= (HOnDestroy)			  &CEffectLet::OnDestroy;
          _OnConfigure	= (HOnConfigure)		  &CEffectLet::OnConfigure;
          _OnSave			  = (HOnSave)				    &CEffectLet::OnSave;
          _BeforeDraw		= (HDrawEvent)			  &CEffectLet::OnBeforeDraw;
          _AfterDraw		= (HAfterDrawEvent)	  &CEffectLet::OnAfterDraw;
          _OnSetParent  = (HOnSetParent)      &CEffectLet::OnSetParent;

					Plugins.insert( std::make_pair(filename,this) );


        }
        else
          type = -1;
      }
      break;
    }//switch
	}
}

bool DrawPlugin::IsValidPlugin()
{
	return type != -1; //hMod != 0; // || m_Script.p != NULL;
}

DrawPlugin::~DrawPlugin()
{
	Plugins.erase(filename);
	FreeLibrary(hMod);
}

void* DrawPlugin::Create(HWND hwndDesklet,const char* szIniFile,const char* szIniGroup)
{
	void *opaque = _OnCreate(hwndDesklet,hMod,szIniFile,szIniGroup);

  if( _OnSetParent )
    _OnSetParent(opaque, this);

  return opaque;
}

void  DrawPlugin::Save(void* pointer,const char* szIniFile,const char* szIniGroup)
{
	if(_OnSave)
		_OnSave(pointer,szIniFile,szIniGroup);
}

bool DrawPlugin::BeforeDraw(void* pointer, HWND hwndDesklet,Graphics* g,UINT left, UINT top, UINT width, UINT height, ColorMatrix* m, Matrix* matrix,BOOL* modified, BOOL isSelected, BOOL isMouseOn)
{
	if(_BeforeDraw)
	{
		_BeforeDraw(pointer,hwndDesklet,g,left,top,width,height,m,matrix,modified,isSelected,isMouseOn);
		return true;
	}
	else
	{
		return false;
	}
}

bool DrawPlugin::AfterDraw(void* pointer,HWND hwndDesklet,Graphics* g,Bitmap* bmp,UINT left, UINT top, UINT width, UINT height, ColorMatrix* m, Matrix* matrix,BOOL* modified, BOOL isSelected, BOOL isMouseOn)
{
	if(_AfterDraw)
	{
		_AfterDraw(pointer,hwndDesklet,g,bmp,left,top,width,height,m,matrix,modified,isSelected,isMouseOn);
		return true;
	}
	else
	{
		return false;
	}
}
bool DrawPlugin::Event(void* pointer, UINT event)
{
	if(_OnEvent)
	{
		_OnEvent(pointer,event);
		return true;
	}
	else
	{
		return false;
	}
}
bool DrawPlugin::Configure(void* pointer, HWND owner)
{
	if(_OnConfigure)
	{
		_OnConfigure(pointer,owner);
		return true;
	}
	else
	{
		return false;
	}
}

void DrawPlugin::Destroy(void* pointer)
{
	if(_OnDestroy)
		_OnDestroy(pointer);
}


const char* DrawPlugin::GetName() const
{
	return name;
}
const char* DrawPlugin::GetAuthor() const
{
	return author;
}
const char* DrawPlugin::GetNotes() const
{
	return notes;
}
int   DrawPlugin::GetVersion() const
{
	return version;
}
const char* DrawPlugin::GetStrVersion() const
{
	return versionStr;
}

ULONG DrawPlugin::AddRef()
{
	return ++refCount;
}
ULONG DrawPlugin::Release()
{
	--refCount;
	if(refCount == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return refCount;
	}
}