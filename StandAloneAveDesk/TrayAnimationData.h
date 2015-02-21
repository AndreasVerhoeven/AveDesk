// TrayAnimationData.h: interface for the CTrayAnimationData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYANIMATIONDATA_H__69D8D288_043B_4A03_93F4_281C19C4C895__INCLUDED_)
#define AFX_TRAYANIMATIONDATA_H__69D8D288_043B_4A03_93F4_281C19C4C895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class CTrayAnimationData  
{

private:
	std::vector<HICON> m_Icons;

	std::wstring m_Author;
	std::wstring m_Comments;
	std::wstring m_FolderName;

	UINT m_InitialSpeedInMs;
	UINT m_MaximumSpeedInMs;
	UINT m_NumberOfRevolutionsNeededToRestore;

	CTrayAnimationData();

public:
	~CTrayAnimationData();

	static CTrayAnimationData* LoadFromFolder(const WCHAR* folder, const WCHAR* folderName);

	HICON GetIcon(UINT index);
	UINT GetNumberOfIcons()const;

	const std::wstring& GetAuthor()const;
	const std::wstring& GetComments()const;
	const std::wstring& GetFolderName()const;

	UINT GetInitialSpeedInMs()const;
	UINT GetMaximumSpeedInMs()const;
	UINT GetNumberOfRevolutionsNeededToRestore()const;

};

#endif // !defined(AFX_TRAYANIMATIONDATA_H__69D8D288_043B_4A03_93F4_281C19C4C895__INCLUDED_)
