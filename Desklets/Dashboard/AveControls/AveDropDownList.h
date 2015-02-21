// AveDropDownList.h: interface for the CAveDropDownList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEDROPDOWNLIST_H__E781F14D_389C_4D36_979C_DD1954ECFE38__INCLUDED_)
#define AFX_AVEDROPDOWNLIST_H__E781F14D_389C_4D36_979C_DD1954ECFE38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"
#include "AveFont.h"

#include <vector>
#include <string>

class CAveDropDownList;
typedef BOOL (__stdcall* OnSelChange)(void* data, CAveDropDownList* dropList, INT* newSel);

class CAveDropDownList : public IAveControl
{
protected:
	std::vector<std::wstring> items;
	int selItem;
	BOOL isDroppedDown;
	BOOL isPressed;
	BOOL mouseIsDown;

	void* callbackData;
	OnSelChange onSelChange;

	CAveFont font;

public:

	CAveDropDownList(IAveControlContainer* container, std::string name);
	virtual ~CAveDropDownList();

	virtual int  InsertItem(int index, const std::wstring& item);
	virtual int  AddItem(const std::wstring& item);
	virtual BOOL RemoveItem(int index);
	virtual int  FindItem(const std::wstring& item, int begin);
	virtual void Clear();
	virtual int  Count()const;

	virtual int GetSel()const;
	virtual int SetSel(int sel);

	virtual CAveFont& GetFont();

	virtual BOOL MouseDown(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseUp(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseClick(const POINT* pt, const SIZE* s, DWORD keys);

	virtual BOOL MouseOn(const POINT* pt, const SIZE* s);
	virtual BOOL MouseMove(const POINT* pt, const SIZE* s);
	virtual BOOL MouseOut();

	virtual BOOL MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);
	virtual BOOL MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls);

	virtual BOOL Message(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void Update();

	virtual BOOL DropDown();

	virtual void SetOnSelChange(void* data, OnSelChange func);

	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
};

#endif // !defined(AFX_AVEDROPDOWNLIST_H__E781F14D_389C_4D36_979C_DD1954ECFE38__INCLUDED_)
