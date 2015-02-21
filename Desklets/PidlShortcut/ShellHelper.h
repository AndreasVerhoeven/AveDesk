// ShellHelper.h: interface for the CShellHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLHELPER_H__6BCC60A5_2E8E_4B48_BE30_BB29B18F843A__INCLUDED_)
#define AFX_SHELLHELPER_H__6BCC60A5_2E8E_4B48_BE30_BB29B18F843A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pidl.h"
#include <string>
#include <vector>

class CShellHelper  
{
private:
	CShellHelper();
	~CShellHelper();

public:

	static void GetListOfFileNamesFromShellIdList(LPIDA pida, std::vector<std::wstring>& filenames, int& length);
	static void GetListOfFileNamesFromHDROP(HDROP hDrop, std::vector<std::wstring>& filenames, int& length);
	static HRESULT GetListOfFileNamesFromIDataObject(IDataObject *obj, std::vector<std::wstring>& filenames, int& length);

};

#endif // !defined(AFX_SHELLHELPER_H__6BCC60A5_2E8E_4B48_BE30_BB29B18F843A__INCLUDED_)
