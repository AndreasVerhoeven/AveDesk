// AveHyperLink.h: interface for the CAveHyperLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEHYPERLINK_H__70F05094_4A91_4164_AA92_4B3659CE68BE__INCLUDED_)
#define AFX_AVEHYPERLINK_H__70F05094_4A91_4164_AA92_4B3659CE68BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveLabel.h"

class CAveHyperLink;
typedef BOOL (__stdcall *OnLinkClick)(void* callbackData,CAveHyperLink* button, const POINT* pt, const SIZE* s, DWORD keys);
class CAveHyperLink : public CAveLabel  
{
protected:
	HCURSOR oldCursor;
	BOOL mouseIsOn;

	void* callbackData;
	OnLinkClick onLinkClick;

	CAveFont mouseOverFont;

public:
	CAveHyperLink(IAveControlContainer* container, std::string name);
	virtual ~CAveHyperLink();

	virtual MouseOn(const POINT* pt, const SIZE* s);
	virtual MouseOut();
	virtual BOOL MouseClick(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL Message(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);
	virtual BOOL MouseDown(const POINT* pt, const SIZE* s, DWORD keys);
	virtual BOOL MouseUp(const POINT* pt, const SIZE* s, DWORD keys);

	virtual void SetOnLinkClickCallback(void* data, OnLinkClick func);

	virtual CAveFont& GetFont();

};

#endif // !defined(AFX_AVEHYPERLINK_H__70F05094_4A91_4164_AA92_4B3659CE68BE__INCLUDED_)
