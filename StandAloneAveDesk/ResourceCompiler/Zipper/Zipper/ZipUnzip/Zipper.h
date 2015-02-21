// Zipper.h: interface for the CZipper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZIPPER_H__4249275D_B50B_4AAE_8715_B706D1CA0F2F__INCLUDED_)
#define AFX_ZIPPER_H__4249275D_B50B_4AAE_8715_B706D1CA0F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Z_FileInfo
{
	int nFileCount;
	int nFolderCount;
	DWORD dwUncompressedSize;
};

class CZipper  
{
public:
	CZipper(const char* szFilePath = NULL, const char* szRootFolder = NULL, bool bAppend = FALSE);
	virtual ~CZipper();

	// simple interface
	static bool ZipFile(const char* szFilePath); // saves as same name with .zip
	static bool ZipFolder(const char* szFilePath, bool bIgnoreFilePath); // saves as same name with .zip
	
	// works with prior opened zip
	bool AddFileToZip(const char* szFilePath, bool bIgnoreFilePath = FALSE);
	bool AddFileToZip(const char* szFilePath, const char* szRelFolderPath); // replaces path info from szFilePath with szFolder
	bool AddFolderToZip(const char* szFolderPath, bool bIgnoreFilePath = FALSE, bool recursive=true);

	// extended interface
	bool OpenZip(const char* szFilePath, const char* szRootFolder = NULL, bool bAppend = FALSE);
	bool CloseZip(); // for multiple reuse
	void GetFileInfo(Z_FileInfo& info);
	
protected:
	void* m_uzFile;
	char m_szRootFolder[MAX_PATH + 1];
	Z_FileInfo m_info;

protected:
	void PrepareSourcePath(char* szPath);
};

#endif // !defined(AFX_ZIPPER_H__4249275D_B50B_4AAE_8715_B706D1CA0F2F__INCLUDED_)
