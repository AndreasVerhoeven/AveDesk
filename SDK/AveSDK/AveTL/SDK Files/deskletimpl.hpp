//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef DESKLETIMPL_HPP_20C293AB93848F210EDD9119292C3938471A
#define DESKLETIMPL_HPP_20C293AB93848F210EDD9119292C3938471A

#include "DeskletSDK.h"
#include "DeskletSDK.cpp"

#define AVE_DESKLET_USE_STD_STRING
#ifdef AVE_DESKLET_USE_STD_STRING
#include <string>
#endif//AVE_DESKLET_USE_STD_STRING

#include "layers.hpp"
#include "Layers.cpp"
#include "Layer.cpp"

#include "AveDeskEvents.hpp"

#include <cassert>

#include <vector>

#define USE_DEFAULT_DESKLET_CONSTRUCTOR(t) t(HWND hwnd, HINSTANCE hInstance, bool bNotLoaded) : CDeskletImpl<t>(hwnd,hInstance){}
#define DEFAULT_DESKLET_CONSTRUCTOR_PROTOTYPE(t) t(HWND hwnd, HINSTANCE hInstance, bool bNotLoaded);
#define DEFAULT_DESKLET_CONSTRUCTOR_BODY(t) t(HWND hwnd, HINSTANCE hInstance, bool bNotLoaded) : CDeskletImpl<t>(hwnd,hInstance)

template<class T> class CDeskletImpl
{
private:
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	CLayers		m_layers;

public:

	CDeskletImpl(HWND hWnd, HINSTANCE hInstance) :
		m_hWnd(hWnd),
		m_hInstance(hInstance),
		m_layers(hWnd)
	{
			assert(m_hWnd != NULL);
	}

	CDeskletImpl() :
		m_hWnd(NULL),
		m_hInstance(NULL)
	{

	}

	HWND GetWindow()const
	{
		return m_hWnd;
	}

	HINSTANCE GetHInstance()const
	{
		return m_hInstance;
	}

	const CLayers& GetLayers()const
	{
		return m_layers;
	}

	BOOL IsVisible()const
	{
		assert( m_hWnd != NULL);

		return ::DeskletIsVisible(m_hWnd);
	}

	BOOL GetRect(RECT& rcRect)const
	{
		assert( m_hWnd != NULL);

		return ::DeskletGetRect(m_hWnd,&rcRect);
	}

	#ifdef AVE_DESKLET_USE_STD_STRING

	std::string GetLabel()const
	{
		assert( m_hWnd != NULL);

		int size = ::DeskletGetLabel(m_hWnd,NULL);
		char buf = new char[size+1];
		memset(buf,NULL,size+1);

		int newSize = DeskletGetLabel(m_hWnd,buf);
		assert(newSize == size);

		std::string retVal = buf;
		delete[] buf;

		return retVal;
		
	}

	int SetLabel(std::string& label)
	{
		assert( m_hWnd != NULL);
		
		return ::DeskletSetLabel(m_hWnd,label.c_str() );
	}

	// the sublabel can only be SET!
	void SetSubLabel(std::string& subLabel)
	{
		assert( m_hWnd != NULL);
		
		::DeskletSetSubLabel(m_hWnd,subLabel.c_str() );
	}

	void SetImageFile(std::string imageFilePath)
	{
		assert( m_hWnd != NULL);

		::DeskletSetImageFile(m_hWnd,imageFilePath.c_str() );
	}

	void SetImageOverlayFile(std::string imageFilePath)
	{
		assert( m_hWnd != NULL);

		::DeskletSetImageOverlayFile(m_hWnd,imageFilePath.c_str() );
	}

	std::string GetRelativeFolder()const
	{
		assert( m_hWnd != NULL);

		char buf[MAX_PATH+2];
		memset(buf,NULL,MAX_PATH+2);

		::DeskletGetRelativeFolder(m_hWnd,buf);

		return std::string(buf);
	}

	std::string GetRootFolder()const
	{
		assert( m_hWnd != NULL);

		char buf[MAX_PATH+2];
		memset(buf,NULL,MAX_PATH+2);

		::DeskletGetRootFolder(m_hWnd,buf);

		return std::string(buf);
	}

	#endif//AVE_DESKLET_USE_STD_STRING

	void SetImage(Image* img, BOOL bAutomaticallyDeleteImage)
	{
		assert( m_hWnd != NULL);

		::DeskletSetImage(m_hWnd,img,bAutomaticallyDeleteImage);
	}

	void SetOverlayImage(Image* img, BOOL bAutomaticallyDeleteImage)
	{
		assert( m_hWnd != NULL);

		::DeskletSetOverlayImage(m_hWnd,img,bAutomaticallyDeleteImage);
	}

	// invalidates this object if the desklet is removed.
	BOOL Remove(BOOL bAsk)
	{
		assert( m_hWnd != NULL);
		
		return ::DeskletRemove(m_hWnd,bAsk);
	}

	void Hide()
	{
		assert( m_hWnd != NULL);
		
		::DeskletHide(m_hWnd);
	}

	void Show()
	{
		assert( m_hWnd != NULL);

		::DeskletShow(m_hWnd);
	}

	void SetPosition(POINT& p)
	{
		assert( m_hWnd != NULL);

		::DeskletSetPosition(m_hWnd,&p);
	}

	void SetSize(SIZE& s)
	{
		assert( m_hWnd != NULL);

		::DeskletSetSize(m_hWnd,&s);
	}

	void SetAlpha(BYTE btAlpha)
	{
		assert( m_hWnd != NULL);

		::DeskletSetAlpha(m_hWnd,btAlpha);
	}

	void SetRotation(INT iRotation)
	{
		assert( m_hWnd != NULL);

		::DeskletSetRotation(m_hWnd,iRotation);
	}

	void GetPosition(POINT& p)const
	{
		assert( m_hWnd != NULL);

		::DeskletGetPosition(m_hWnd,&p);
	}

	POINT GetPosition()const
	{
		assert( m_hWnd != NULL);

		POINT p = {0};
		::DeskletGetPosition(m_hWnd,&p);

		return p;
	}

	void GetSize(SIZE& s)const
	{
		assert( m_hWnd != NULL);

		::DeskletGetSize(m_hWnd,&s);
	}

	SIZE GetSize()const
	{
		assert( m_hWnd != NULL);

		SIZE s = {0};
		::DeskletGetSize(m_hWnd,&s);

		return s;
	}

	BYTE GetAlpha()const
	{
		assert( m_hWnd != NULL);

		return ::DeskletGetAlpha(m_hWnd);
	}

	INT GetRotation()const
	{
		assert( m_hWnd != NULL);

		return ::DeskletGetRotation(m_hWnd);
	}

	void ActivateExtendedDragDrop()
	{
		assert( m_hWnd != NULL);

		::DeskletActivateExtendedDragDrop(m_hWnd);
	}

	void DeactivateExtendedDragDrop()
	{
		assert( m_hWnd != NULL);

		::DeskletDeactivateExtendedDragDrop(m_hWnd);
	}

	void AddDragDropFormat(FORMATETC& format)
	{
		assert( m_hWnd != NULL);

		::DeskletAddDragDropFormat(m_hWnd,&format);
	}

	void RemoveDragDropFormat(FORMATETC& format)
	{
		assert( m_hWnd != NULL);

		::DeskletRemoveDragDropFormat(m_hWnd,format);
	}

	void Move(RECT& rcRect, BOOL bRedraw=TRUE)
	{
		assert( m_hWnd != NULL);
		
		::DeskletMove(m_hWnd,&rcRect,bRedraw);
	}


	HMENU GetDefaultMenu(int startId)
	{
		assert( m_hWnd != NULL);

		return ::DeskletGetDefaultMenu(m_hWnd,startId);
	}

	void CALLBACK DoDefaultMenuCommand(int startId,int cmd)
	{
		assert( m_hWnd != NULL);

		::DeskletDoDefaultMenuCommand(m_hWnd,startId,cmd);
	}

	int CALLBACK AddPropertySheet(HPROPSHEETPAGE hpg)
	{
		return ::DeskletAddPropertySheet(m_hWnd,hpg);
	}

	static T* Create(HWND hWnd, HINSTANCE hInstance,const char* ini, const char* group)
	{
		T* t = new T(hWnd,hInstance, !ini || !group);

		if(ini && group)
			t->ReadFromIni(ini,group);

		return t;

	}

	void ReadFromIni(const char* iniFile, const char* iniSection)
	{

	}

	void SaveToIni(const char* iniFile, const char* iniSection)
	{

	}


	BOOL LeftButtonClick(POINT *ptCursor, SIZE *sizeDesklet)
	{
		return FALSE;
	}

	BOOL  RightButtonClick(POINT *ptCursor, SIZE *sizeDesklet)
	{
		return FALSE;
	}


	BOOL  MouseMove(int x, int y)
	{
		return FALSE;
	}

	BOOL  MouseOn()
	{
		return FALSE;
	}


	BOOL  MouseOut()
	{
		return FALSE;
	}

	BOOL  DragEnter(IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
	{
		return FALSE;
	}

	BOOL  DragLeave()
	{
		return FALSE;
	}

	BOOL  DragOver(IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
	{
		return FALSE;
	}

	static BOOL RawDrop(T* t,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
	{
		BOOL bRet = FALSE;
		if(::IsDataObjectHDROP(obj))
		{
			std::vector<std::string>files;

			DropStruct* ds = LockHDROP(obj);
	
			int num = DragQueryFile(ds->hDrop, -1, 0, 0);
			for(int i=0;i<num;++i)
			{
				char buf[MAX_PATH] = {0};
				DragQueryFile(ds->hDrop,i,buf,MAX_PATH);	
				files.push_back(buf);
			}

			UnlockHDROP(ds);

			bRet |= t->FilesDrop(files,keys,pnt,effect);
		}

		FORMATETC fmtetc; 

		fmtetc.cfFormat = CF_TEXT; 
		fmtetc.ptd      = 0; 
		fmtetc.dwAspect = DVASPECT_CONTENT; 
		fmtetc.lindex   = -1; 
		fmtetc.tymed    = TYMED_HGLOBAL; 

		if(SUCCEEDED(obj->QueryGetData(&fmtetc)))
		{
		    STGMEDIUM medium; 
			HRESULT hr = obj->GetData(&fmtetc, &medium); 

			if (!FAILED(obj->GetData(&fmtetc, &medium))) 
			{ 
				std::string str;
				HGLOBAL hGlobal = medium.hGlobal;
				char* txt = (char*)GlobalLock(hGlobal);
				
				str = txt;

				GlobalUnlock(hGlobal); 
				ReleaseStgMedium(&medium); 

				bRet |= t->StringDrop(str,keys,pnt,effect);
			}
		}

		bRet |= t->Drop(obj,keys ,pnt ,effect);

		return bRet;
	}

	BOOL FilesDrop(std::vector<std::string>& files, DWORD keys ,POINTL pnt ,DWORD* effect)
	{
		return FALSE;
	}

	BOOL StringDrop(std::string& s, DWORD keys ,POINTL pnt ,DWORD* effect)
	{
		return FALSE;
	}

	BOOL Drop(IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
	{
		return FALSE;
	}

	void  ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{	
		return;
	}

	int  OnAddPropertySheets(int numPresent)
	{
		return numPresent;
	}

	BOOL  DragDropDataRequest(FORMATETC* pFetc, STGMEDIUM* pMedium)
	{
		return FALSE;
	}


	BOOL  Select()
	{
		return FALSE;
	}

	 BOOL  Deselect()
	{
		return FALSE;
	}

	BOOL  MouseWheelForward(DWORD keysDown, int numScrolls, int x, int y)
	{
		return FALSE;
	}

	BOOL  MouseWheelBackward(DWORD keysDown, int numScrolls, int x, int y)
	{
		return FALSE;
	}
};

#endif//DESKLETIMPL_HPP_20C293AB93848F210EDD9119292C3938471A
