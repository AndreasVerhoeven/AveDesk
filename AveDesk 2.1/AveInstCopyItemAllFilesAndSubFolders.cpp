// AveInstCopyItemAllFilesAndSubFolders.cpp: implementation of the CAveInstCopyItemAllFilesAndSubFolders class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstCopyItemAllFilesAndSubFolders.h"
#include "AveInstFileDirOperation.h"
#include "AveInstallerEnvironment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstCopyItemAllFilesAndSubFolders::CAveInstCopyItemAllFilesAndSubFolders() :
	m_Recursive(true)
{

}

CAveInstCopyItemAllFilesAndSubFolders::~CAveInstCopyItemAllFilesAndSubFolders()
{

}

HRESULT CAveInstCopyItemAllFilesAndSubFolders::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	CComPtr<IXMLDOMNamedNodeMap> nodeMap;
	if(SUCCEEDED(hRes = node->get_attributes(&nodeMap)) && nodeMap.p != NULL)
	{
		CComBSTR bstrName( L"Recursively");
		XMLNode namedNode;
		if(SUCCEEDED(hRes = nodeMap->getNamedItem(bstrName,&namedNode)) && namedNode.p != NULL)
		{
			CComBSTR bstrRecursively;
			if(SUCCEEDED( hRes = namedNode->get_text(&bstrRecursively)))
			{
				if(bstrRecursively != NULL)
				{	
					WCHAR* recursively = static_cast<WCHAR*>(bstrRecursively);
					m_Recursive =	_wcsicmp(recursively,L"true")	== 0 || 
									wcscmp(recursively,L"1")		== 0||
									_wcsicmp(recursively,L"yes")		== 0;
				}
				else
				{
					m_Recursive = true;
				}
			}
		}
	}

	return (hRes = S_OK);
}

std::wstring CAveInstCopyItemAllFilesAndSubFolders::GetNewInputPath(CAveInstallerEnvironment& env, 
																	const std::wstring& inputPath)
{
	return L"";
}

std::wstring CAveInstCopyItemAllFilesAndSubFolders::GetNewOutputPath(CAveInstallerEnvironment& env, 
																	 const std::wstring& outputPath)
{
	return L"";
}

bool CAveInstCopyItemAllFilesAndSubFolders::PerformOperation(CAveInstFileDirOperation& op, 
															 CAveInstallerEnvironment& env, 
															 const std::wstring& inputPath, 
															 const std::wstring& outputPath)
{
	WCHAR path[MAX_PATH] = {0};
	wcscpy(path,inputPath.c_str());
	PathAppendW(path,L"*.*");

	WIN32_FIND_DATAW data = {0};
	HANDLE h = FindFirstFileW(path,&data);
	if(h != INVALID_HANDLE_VALUE)
	{
		bool res = true;

		do
		{
			if(_wcsicmp(data.cFileName,L".") != 0 && _wcsicmp(data.cFileName,L"..") != 0)
			{
				WCHAR newInputPath[MAX_PATH] = {0};
				wcscpy(newInputPath,inputPath.c_str());
				PathAppendW(newInputPath, data.cFileName);

				WCHAR newOutputPath[MAX_PATH] = {0};
				wcscpy(newOutputPath,outputPath.c_str());
				PathAppendW(newOutputPath, data.cFileName);

				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					res &= op.DirectoryOperation(env,newInputPath, newOutputPath);

					if(res && m_Recursive)
						res &= PerformOperation(op,env,newInputPath, newOutputPath);
				}
				else
				{
					res &= op.FileOperation(env,newInputPath, newOutputPath);
				}
			}

		}while(res && FindNextFileW(h,&data));

		FindClose(h);

		return res;
	}
	else
	{
		return false;
	}
}