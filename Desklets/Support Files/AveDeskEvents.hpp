//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef AVEDESKEVENTS_HPP_20C293AB93848F210EDD9119292C3938471A
#define AVEDESKEVENTS_HPP_20C293AB93848F210EDD9119292C3938471A

#include <windows.h>
#include "DeskletSDK.h"

#define DESKLET_INFORMATION(a,b,c,d)  void __stdcall OnGetInformation(char* szName, char* szAuthor,int *piVersion, char *szNotes) \
{	\
	strcpy(szName,a);	\
	strcpy(szAuthor,b); \
	strcpy(szNotes,c); \
	*piVersion = d; \
}

template<class T> T* __stdcall OnCreate(HWND hwnd, HINSTANCE hInstance,const char* ini, const char* group)
{
	return T::Create(hwnd,hInstance,ini,group);
}
#define REQUIRED_EVENT_ONCREATE(t) template t* __stdcall OnCreate<t>(HWND, HINSTANCE, const char*, const char*);

template<class T> void __stdcall OnDestroy(T* t, HWND hWnd)
{
	delete t;
	t = NULL;
};
#define REQUIRED_EVENT_ONDESTROY(t) template void __stdcall OnDestroy<t>(t*,HWND);

template<class T> BOOL __stdcall OnLeftButtonClick(T* t ,POINT *ptCursor, SIZE *sizeDesklet)
{
	return t->LeftButtonClick(ptCursor,sizeDesklet);
}
#define EVENT_ONLEFTBUTTONCLICK(t) template BOOL __stdcall OnLeftButtonClick<t>(t*,POINT*,SIZE*);


template<class T> BOOL __stdcall OnRightButtonClick(T* t ,POINT *ptCursor, SIZE *sizeDesklet)
{
	return t->RightButtonClick(ptCursor,sizeDesklet);
}
#define EVENT_ONRIGHTBUTTONCLICK(t) template BOOL __stdcall OnRightButtonClick<t>(t*,POINT*,SIZE*);


template<class T> BOOL __stdcall OnMouseMove(T* t, int x, int y)
{
	return t->MouseMove(x,y);
}
#define EVENT_ONMOUSEMOVE(t) template BOOL __stdcall OnMouseMove<t>(t*,int,int);


template<class T> BOOL __stdcall OnMouseOn(T* t)
{
	return t->MouseOn();
}
#define EVENT_ONMOUSEON(t) template BOOL __stdcall OnMouseOn<t>(t*);


template<class T> BOOL __stdcall OnMouseOut(T* t)
{
	return t->MouseOut();
}
#define EVENT_ONMOUSEOUT(t) template BOOL __stdcall OnMouseOut<t>(t*);


template<class T> BOOL __stdcall OnDragEnter(T* t,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	return t->DragEnter(obj,keys,pnt,effect);
}
#define EVENT_ONDRAGENTER(t) template BOOL __stdcall OnDragEnter(t*,IDataObject*,DWORD,POINTL ,DWORD*);


template<class T> BOOL __stdcall OnDragLeave(T* t)
{
	return t->DragLeave();
}
#define EVENT_ONDRAGLEAVE(t) template BOOL __stdcall OnDragLeave<t>(t*);


template<class T> BOOL __stdcall OnDragOver(T* t,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	return t->DragOver(keys,pnt,effect);
}
#define EVENT_ONDRAGOVER(t) template BOOL __stdcall OnDragOver(t*,DWORD,POINTL ,DWORD*);


template<class T> BOOL __stdcall OnDrop(T* t,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	return T::RawDrop(t,obj,keys,pnt,effect);
}
#define EVENT_ONDROP(t) template BOOL __stdcall OnDrop(t*,IDataObject*,DWORD,POINTL ,DWORD*);


template<class T> void __stdcall OnSave(T* t, const char *szIni, const char *szIniGroup, BOOL bIsForExport)
{
	t->SaveToIni(szIni,szIniGroup);
}
#define EVENT_ONSAVE(t) template void __stdcall OnSave<t>(t*,const char*,const char*,BOOL);


template<class T>void __stdcall OnProcessMessage(T *t,HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	t->ProcessMessage(hwnd,msg,wParam,lParam);
}
#define EVENT_ONPROCESSMESSAGE(t) template void __stdcall OnProcessMessage<t>(t*,HWND,UINT,WPARAM,LPARAM);


template<class T> int __stdcall OnAddPropertySheets(T* t, int numPresent)
{
	return t->AddPropertySheets(numPresent);
}
#define EVENT_ONADDPROPERTYSHEETS(t) template int __stdcall OnAddPropertySheets<t>(t*,int);


template<class T> BOOL __stdcall OnDragDropDataRequest(T* t,FORMATETC* pFetc, STGMEDIUM* pMedium)
{
	return t->DragDropDataRequest(pFetc,pMedium);
}
#define EVENT_ONDRAGDROPDATAREQUEST(t) template BOOL __stdcall OnDragDropDataRequest<t>(t*,FORMATETC*, STGMEDIUM*);


template<class T> BOOL __stdcall OnSelect(T* t)
{
	return t->Select();
}
#define EVENT_ONSELECT(t) template BOOL __stdcall OnSelect<t>(t*);


template<class T> BOOL __stdcall OnDeselect(T* t)
{
	return t->Deselect();
}
#define EVENT_ONDESELECT(t) template BOOL __stdcall OnDeselect<t>(t*);


template<class T> BOOL __stdcall OnMouseWheelForward(T* t, DWORD keysDown, int numScrolls, int x, int y)
{
	return t->MouseWheelForward(keysDown, numScrolls, x, y);
}
#define EVENT_ONMOUSEWHEELFORWARD(t) BOOL __stdcall OnMouseWheelForward<t>(T*, DWORD,int, int, int);


template<class T> BOOL __stdcall OnMouseWheelBackward(T* t, DWORD keysDown, int numScrolls, int x, int y)
{
	return t->MouseWheelBackward(keysDown, numScrolls, x, y);
}
#define EVENT_ONMOUSEWHEELBACKWARD(t) BOOL __stdcall OnMouseWheelBackward<t>(T*, DWORD,int, int, int);

#endif//AVEDESKEVENTS_HPP_20C293AB93848F210EDD9119292C3938471A