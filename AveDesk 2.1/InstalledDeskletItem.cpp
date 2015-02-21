#include "stdafx.h"
#include "InstalledDeskletItem.h"
#include "FoundDeskletEntry.h"
#include "Docklet.h"

CInstalledDeskletItem::CInstalledDeskletItem(CFoundDeskletEntry* entry) : entry(entry)
{
	ATLASSERT(entry != NULL);
}

CInstalledDeskletItem::~CInstalledDeskletItem(void)
{
	if(entry != NULL)
	{
		entry->StopCreatingPreviewBitmap(INFINITE);
		entry->StopCreatingLargePreviewBitmap(INFINITE);
	}
}

bool CInstalledDeskletItem::Equals(CDeskletItem* item)
{
	CInstalledDeskletItem* that = dynamic_cast<CInstalledDeskletItem*>(item);
	if(NULL == that)
		return false;

	return this->entry == that->entry;
}

HBITMAP CInstalledDeskletItem::GetSmallIcon()
{
	if(entry != NULL)
		return entry->GetPreviewBitmap();
	else
		return NULL;
}

HBITMAP CInstalledDeskletItem::GetBigIcon()
{
	SIZE s = {0};
	HBITMAP retVal = NULL;
	if(entry != NULL)
		entry->GetLargePreview(&retVal, &s);

	return retVal;
}

SIZE CInstalledDeskletItem::GetSize()
{
	SIZE s = {0};
	if(entry != NULL)
		s = entry->GetSize();

	return s;
}

BOOL CInstalledDeskletItem::AreSmallAndBigIconTheSame()
{
	return entry->AreSmallAndLargePreviewTheSame();
}

std::wstring CInstalledDeskletItem::GetName()
{
	if(entry != NULL)
		return entry->GetName();
	else
		return L"";
}

std::wstring CInstalledDeskletItem::GetAuthor()
{
	if(entry != NULL)
		return entry->GetAuthor();
	else
		return L"";
}

std::wstring CInstalledDeskletItem::GetDescription()
{
	if(entry != NULL)
		return entry->GetDescription();
	else
		return L"";
}
std::wstring CInstalledDeskletItem::GetTypeName()
{
	if(entry != NULL)
		return entry->GetRealName();
	else
		return L"";
}

std::wstring CInstalledDeskletItem::GetCategoryName()
{
	if(entry != NULL)
		return entry->GetRealName();
	else
		return L"";
}

DWORD CInstalledDeskletItem::GetFlags()
{
	DWORD flags(0);
	// TODO: implement
	return flags;
}
std::wstring CInstalledDeskletItem::GetGUID()
{
	// TODO: implement
	std::wstring hash;
	return hash;
}

void CInstalledDeskletItem::StartCreatingSmallIcon(const SIZE& s,HWND hwnd, UINT msg)
{
	if(entry != NULL)
	{
		entry->SetLParam((LPARAM)this);
		entry->StartCreatingPreviewBitmap(s, hwnd, msg);
	}
}

void CInstalledDeskletItem::StopCreatingSmallIcon()
{
	if(entry != NULL)
	{
		entry->SetLParam((LPARAM)this);
		entry->StopCreatingPreviewBitmap(INFINITE);
	}
}

void CInstalledDeskletItem::StartCreatingBigIcon(HWND hwnd, UINT msg)
{
	if(entry != NULL)
	{
		if(NULL == m_SmallIcon  || !entry->AreSmallAndLargePreviewTheSame())
			entry->StartCreatingLargePreviewBitmap(hwnd, msg);
	}
}

void CInstalledDeskletItem::StopCreatingBigIcon()
{
	if(entry != NULL)
		entry->StopCreatingLargePreviewBitmap(INFINITE);
}

BOOL CInstalledDeskletItem::StartingToInstantiate(IProgressIndicator* progress)
{
	if(progress != NULL)
		progress->NotifyDone();
	return TRUE;
}

void CInstalledDeskletItem::StopToInstantiate()
{
	return;
}

HWND CInstalledDeskletItem::Instantiate(BOOL deskletInitHidden, POINT* pt, BOOL async, IProgressIndicator* progress)
{
	Docklet* d = entry->Instantiate(FALSE, deskletInitHidden, pt);
	if(NULL == d)
	{
		if(progress != NULL)
			progress->NotifyError(L"");
		return NULL;
	}
		
	if(!async)
		d->WaitForDockletBeingInitialized(INFINITE);

	if(progress != NULL)
		progress->NotifyDone();
	return d->GetHWnd();
}