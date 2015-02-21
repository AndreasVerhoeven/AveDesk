#pragma once
#include "deskletitem.h"

class CFoundDeskletEntry;
class CInstalledDeskletItem :
	public CDeskletItem
{
protected:
	CFoundDeskletEntry* entry;

public:
	CInstalledDeskletItem(CFoundDeskletEntry* entry);
public:

	static const UINT WM_USER_BIG_ICON = WM_USER + 957;
	static const UINT WM_USER_SMALL_ICON = WM_USER + 958;

	virtual ~CInstalledDeskletItem(void);

	virtual HBITMAP GetSmallIcon();
	virtual HBITMAP GetBigIcon();
	virtual SIZE GetSize();
	virtual BOOL AreSmallAndBigIconTheSame();

	virtual std::wstring GetName();
	virtual std::wstring GetAuthor();
	virtual std::wstring GetDescription();
	virtual std::wstring GetTypeName();
	virtual std::wstring GetCategoryName();

	virtual DWORD GetFlags();
	virtual std::wstring GetGUID();

	virtual void StartCreatingSmallIcon(const SIZE& s, HWND hwnd, UINT msg);
	virtual void StopCreatingSmallIcon();
	virtual void StartCreatingBigIcon(HWND hwnd, UINT msg);
	virtual void StopCreatingBigIcon();

	virtual BOOL StartingToInstantiate( IProgressIndicator* progress);
	virtual void StopToInstantiate();
	virtual HWND Instantiate(BOOL deskletInitHidden, POINT* pt, BOOL async, IProgressIndicator* progress);

	virtual bool Equals(CDeskletItem* item);
};
