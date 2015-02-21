// FoundDeskletEntry.h: interface for the CFoundDeskletEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOUNDDESKLETENTRY_H__BD62EA55_AEC8_4B60_8D7E_0A8DA7EF8549__INCLUDED_)
#define AFX_FOUNDDESKLETENTRY_H__BD62EA55_AEC8_4B60_8D7E_0A8DA7EF8549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "Docklet.h"

#include "DeskletPreviewBitmapRequestData.h"

class DockletDLL;
class DeskletSkinEnumerator;
class DeskletSkinInfo;

#define SEARCH_FLAG_NAME		0x01
#define SEARCH_FLAG_DESCRIPTION	0x02
#define SEARCH_FLAG_AUTHOR		0x04

#define SEARCH_FLAG_ALL			0xFFFFF

class CFoundDeskletEntry  
{
protected:
	std::wstring smallFileName;
	std::wstring bigFileName;

	HANDLE m_ThreadHandle;
	HWND m_CallbackHwnd;
	UINT m_Msg;
	BOOL m_Stop;

	PreviewBitmapRequestData requestDataSmall;
	PreviewBitmapRequestData requestDataLarge;

	HANDLE m_ThreadHandle2;
	HWND m_CallbackHwnd2;
	UINT m_Msg2;
	BOOL m_Stop2;
	BOOL m_TriedLoadingLarge;


	DockletDLL* dll;
	DeskletSkinEnumerator* enumerator;
	DeskletSkinInfo* skinInfo;

	std::wstring m_Name;
	std::wstring m_FinalDescription;
	HBITMAP m_hBmp;
	SIZE m_BitmapSize;

	HBITMAP m_hBmpLarge;
	SIZE m_LargeSize;
	BOOL largeBitmapLoadedCorrectly;

	static DWORD __stdcall ThisThreadFunc(LPVOID lpData);
	static DWORD __stdcall ThisThreadFunc2(LPVOID lpData);

	LPARAM m_lParam;

	DWORD CheckAnimSpeed();

public:
	CFoundDeskletEntry(DockletDLL* dll, DeskletSkinEnumerator* enumerator, DeskletSkinInfo* skinInfo);
	virtual ~CFoundDeskletEntry();

	virtual std::wstring GetName()const;
	virtual std::wstring GetDescription()const;
	virtual HBITMAP GetPreviewBitmap(){return m_hBmp;};
	virtual std::wstring GetRealName()const;

	virtual std::wstring GetAuthor()const;
	virtual std::wstring GetVersion()const;

	virtual void StartCreatingPreviewBitmap(const SIZE& s, HWND callbackHwnd, UINT msg);
	virtual void StopCreatingPreviewBitmap(DWORD timeOut);
	virtual BOOL IsRunning()const;

	virtual void StartCreatingLargePreviewBitmap(HWND callbackHwnd, UINT msg);
	virtual void StopCreatingLargePreviewBitmap(DWORD timeOut);
	virtual void WaitForLargeBitmapPreview(DWORD timeOut);
	virtual BOOL GetLargePreview(HBITMAP* hBmp, SIZE* s);
	virtual BOOL FailedLoadingLargePreview();

	virtual DWORD OnCreatePreview();
	virtual DWORD OnCreateLargePreview();

	virtual void SetLParam(LPARAM val) { m_lParam = val;};
	virtual LPARAM GetLParam()const{return m_lParam;};

	virtual SIZE GetSize(){return m_LargeSize;};

	virtual BOOL AreSmallAndLargePreviewTheSame()const;

	virtual BOOL MatchesSearchString(const std::wstring& search, DWORD flags);

	virtual Docklet* Instantiate(BOOL allowLoadFromStock, BOOL hiddenByDefault = FALSE, POINT* pt = NULL);
};

#endif // !defined(AFX_FOUNDDESKLETENTRY_H__BD62EA55_AEC8_4B60_8D7E_0A8DA7EF8549__INCLUDED_)
