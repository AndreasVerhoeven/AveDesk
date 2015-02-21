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

template<class T> CDeskletImpl
{
private:
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	CLayers		m_layers;

public:

	CDeskletImpl(HWND hWnd, HINSTANCE hInstance) :
		m_hWnd(hWnd),
		m_hInstance(hInstance)
	{
			::ASSERT(m_hWnd != NULL);
	}


	HWND GetWindow()const
	{
		return m_hWnd;
	}

	HINSTANCE GetHInstance()const
	{
		return m_hInstance;
	}

	const Layers& GetLayers()const
	{
		return m_layers;
	}

	BOOL IsVisible()const
	{
		::ASSERT( m_hWnd != NULL);

		return ::DeskletIsVisible(m_hWnd);
	}

	BOOL GetRect(RECT& rcRect)const
	{
		::ASSERT( m_hWnd != NULL);

		return ::DeskletGetRect(m_hWnd,&rcRect);
	}

	#ifdef AVE_DESKLET_USE_STD_STRING

	std::string GetLabel()const
	{
		::ASSERT( m_hWnd != NULL);

		int size = ::DeskletGetLabel(m_hWnd,NULL);
		char buf = new char[size+1];
		memset(buf,NULL,size+1);

		int newSize = DeskletGetLabel(m_hWnd,buf);
		::ASSERT(newSize == size);

		std::string retVal = buf;
		delete[] buf;

		return retVal;
		
	}

	int SetLabel(std::string& label)
	{
		::ASSERT( m_hWnd != NULL);
		
		return ::DeskletSetLabel(m_hWnd,label.c_str() );
	}

	// the sublabel can only be SET!
	void SetSubLabel(m_hWnd, std::string& subLabel)
	{
		::ASSERT( m_hWnd != NULL);
		
		::DeskletSetSubLabel(m_hWnd,subLabel.c_str() );
	}

	void SetImageFile(std::string imageFilePath)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetImageFile(m_hWnd,imageFilePath.c_str() );
	}

	void SetImageOverlayFile(std::string imageFilePath)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetImageOverlayFile(m_hWnd,imageFilePath.c_str() );
	}

	std::string GetRelativeFolder()const
	{
		::ASSERT( m_hWnd != NULL);

		char buf[MAX_PATH+2];
		memset(buf,NULL,MAX_PATH+2);

		::DeskletGetRelativeFolder(m_hWnd,buf);

		return std::strign(buf);
	}

	std::string GetRootFolder()const
	{
		::ASSERT( m_hWnd != NULL);

		char buf[MAX_PATH+2];
		memset(buf,NULL,MAX_PATH+2);

		::DeskletGetRootFolder(m_hWnd,buf);

		return std::strign(buf);
	}

	#endif//AVE_DESKLET_USE_STD_STRING

	void SetImage(Image* img, BOOL bAutomaticallyDeleteImage)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetImage(m_hWnd,img,bAutomaticallyDeleteImage);
	}

	void SetOverlayImage(Image* img, BOOL bAutomaticallyDeleteImage)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetOverlayImage(m_hWnd,img,bAutomaticallyDeleteImage);
	}

	// invalidates this object if the desklet is removed.
	BOOL Remove(BOOL bAsk)
	{
		::ASSERT( m_hWnd != NULL);
		
		return ::DeskletRemove(m_hWnd,bAsk);
	}

	void Hide()
	{
		::ASSERT( m_hWnd != NULL);
		
		::DeskletHide(m_hWnd);
	}

	void Show()
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletShow(m_hWnd);
	}

	void SetPosition(POINT& p)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetPosition(m_hWnd,&p);
	}

	void SetSize(SIZE& s)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetSize(m_hWnd,&s);
	}

	void SetAlpha(BYTE btAlpha)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetAlpha(m_hWnd,btAlpha);
	}

	void SetRotation(INT iRotation)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletSetRotation(m_hWnd,iRotation);
	}

	void GetPosition(POINT& p)const
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletGetPosition(m_hWnd,&p);
	}

	POINT GetPosition()const
	{
		::ASSERT( m_hWnd != NULL);

		POINT p = {0};
		::DeskletGetPosition(m_hWnd,&p);

		return p;
	}

	void GetSize(SIZE& s)const
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletGetSize(m_hWnd,&s);
	}

	SIZE GetSize()const
	{
		::ASSERT( m_hWnd != NULL);

		SIZE s = {0};
		::DeskletGetSize(m_hWnd,&s);

		return s;
	}

	BYTE GetAlpha()const
	{
		::ASSERT( m_hWnd != NULL);

		return ::DeskletGetAlpha(m_hWnd);
	}

	INT GetRotation()const
	{
		::ASSERT( m_hWnd != NULL);

		return ::DeskletGetRotation(m_hWnd);
	}

	void ActivateExtendedDragDrop()
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletActivateExtendedDragDrop(m_hWnd);
	}

	void DeactivateExtendedDragDrop()
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletDeactivateExtendedDragDrop(m_hWnd);
	}

	void AddDragDropFormat(FORMATETC& format)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletAddDragDropFormat(m_hWnd,&format);
	}

	void RemoveDragDropFormat(FORMATETC& format)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletRemoveDragDropFormat(m_hWnd,format);
	}

	void Move(RECT& rcRect, BOOL bRedraw=TRUE)
	{
		::ASSERT( m_hWnd != NULL);
		
		::DeskletMove(m_hWnd,&rcRect,bRedraw);
	}


	HMENU GetDefaultMenu(int startId);
	{
		::ASSERT( m_hWnd != NULL);

		return ::DeskletGetDefaultMenu(m_hWnd,startId);
	}

	void CALLBACK DoDefaultMenuCommand(int startId,int cmd)
	{
		::ASSERT( m_hWnd != NULL);

		::DeskletDoDefaultMenuCommand(m_hWnd,startId,cmd);
	}

	int CALLBACK AddPropertySheet(HPROPSHEETPAGE hpg)
	{
		return ::DeskletAddPropertySheet(m_hWnd,hpg);
	}
};

#endif//DESKLETIMPL_HPP_20C293AB93848F210EDD9119292C3938471A
