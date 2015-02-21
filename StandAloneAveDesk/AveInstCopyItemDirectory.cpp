// AveInstCopyItemDirectory.cpp: implementation of the CAveInstCopyItemDirectory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstCopyItemDirectory.h"
#include "AveInstCopyItemFile.h"
#include "AveInstallerEnvironment.h"
#include "AveInstFileDirOperation.h"
#include "AveInstCopyItemAllFilesAndSubFolders.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstCopyItemDirectory::CAveInstCopyItemDirectory()
{

}

CAveInstCopyItemDirectory::~CAveInstCopyItemDirectory()
{

}

std::wstring CAveInstCopyItemDirectory::GetNewOutputPath(CAveInstallerEnvironment& env, const std::wstring& outputPath) 
{
	WCHAR dstPath[MAX_PATH] = {0};

	if(wcscmp(m_BaseFolder.c_str(),L"") != 0) // STL bug again...
	{
		if(_wcsicmp(m_BaseFolder.c_str(),L"desklets") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetDeskletsPath().c_str() );		
		else if(_wcsicmp(m_BaseFolder.c_str(),L"docklets") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetDockletsPath().c_str() );	
		else if(_wcsicmp(m_BaseFolder.c_str(),L"effects") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetEffectsPath().c_str() );	
		else if(_wcsicmp(m_BaseFolder.c_str(),L"avedesk") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetInstallPath().c_str() );	
		else if(_wcsicmp(m_BaseFolder.c_str(),L"labels") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetLabelsPath().c_str() );
		else if(_wcsicmp(m_BaseFolder.c_str(),L"languages") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetLanguagesPath().c_str() );
		else if(_wcsicmp(m_BaseFolder.c_str(),L"Themes") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetThemesPath().c_str() );
		else if(_wcsicmp(m_BaseFolder.c_str(),L"User") == 0)
			wcscpy(dstPath,env.GetAveDeskEnvironment().GetUserPath().c_str() );
		else // it's none of the default base folders, so it must be an alias!
			wcscpy(dstPath,env.GetFileAlias(m_BaseFolder).c_str() );		
	}
	else
	{
		wcscpy(dstPath,outputPath.c_str());
		if(wcscmp(m_DirName.c_str(),L"") != 0)
		{
			PathAppendW(dstPath,m_DirName.c_str() );
		}
	}

	return dstPath;
}

std::wstring CAveInstCopyItemDirectory::GetNewInputPath(CAveInstallerEnvironment& env, const std::wstring& inputPath)
{
	WCHAR srcPath[MAX_PATH] = {0};
	wcscpy(srcPath,inputPath.c_str() );

	if(wcscmp(m_DirName.c_str(),L"") != 0)
		PathAppendW(srcPath,m_DirName.c_str() );

	return srcPath;
	
}

bool CAveInstCopyItemDirectory::PerformOperation(CAveInstFileDirOperation& op, CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath)
{

	std::wstring srcPath = GetNewInputPath(env,inputPath);
	std::wstring dstPath = GetNewOutputPath(env,outputPath);

	bool res = op.DirectoryOperation(env,srcPath,dstPath);
	if(res)
	{
		for(std::list<boost::shared_ptr<CAveInstCopyItem> >::iterator iter = m_Items.begin(); iter != m_Items.end(); ++iter)
		{
			boost::shared_ptr<CAveInstCopyItem>& cpyItem = *iter;
			res &= cpyItem->PerformOperation(op,env,srcPath,dstPath);
		}
	}

	return res;
}


HRESULT CAveInstCopyItemDirectory::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	hRes = CAveInstCopyItem::LoadFromNode(node);

	if(NULL == node.p)
		return (hRes = E_FAIL);

	CComPtr<IXMLDOMNamedNodeMap> nodeMap;
	if(FAILED(hRes = node->get_attributes(&nodeMap)))
		return hRes;

	if(NULL == nodeMap.p)
		return (hRes = E_FAIL);


	CComBSTR bstrName( L"BaseFolder");
	XMLNode namedNode;
	if(FAILED(hRes = nodeMap->getNamedItem(bstrName,&namedNode)))
		return hRes;

	CComBSTR bstrBaseFolder;
	if(namedNode.p != NULL && SUCCEEDED( hRes = namedNode->get_text(&bstrBaseFolder)))
	{
		if(bstrBaseFolder != NULL)
			m_BaseFolder = static_cast<WCHAR*>(bstrBaseFolder);
	}
	
	CComBSTR bstrName2( L"Name");
	XMLNode namedNode2;
	if(FAILED(hRes = nodeMap->getNamedItem(bstrName2,&namedNode2)))
	return hRes;

	if(namedNode2.p != NULL)
	{
		CComBSTR bstrDirName;
		if(SUCCEEDED( hRes = namedNode2->get_text(&bstrDirName)))
		{
			if(bstrDirName != NULL)
				m_DirName = static_cast<WCHAR*>(bstrDirName);		
		}
	}

	CComPtr<IXMLDOMNodeList> nodeList;
	if(FAILED(hRes = node->get_childNodes(&nodeList)) || NULL == nodeList.p)
		return hRes;

	long lCount(0);
	if( FAILED( hRes = nodeList->get_length(&lCount) ) )
		return hRes;

	for(long lIndex = 0; lIndex <= lCount; ++lIndex)
	{
		XMLNode newNode;
		if(SUCCEEDED( hRes = nodeList->get_item(lIndex,&newNode)) && newNode.p != NULL )
		{
			CComBSTR bstrText;
			if(SUCCEEDED(newNode->get_nodeName(&bstrText)) && bstrText != NULL)
			{
				WCHAR* wText = static_cast<WCHAR*>(bstrText);
				if(_wcsicmp(wText,L"directory") == 0)
				{
					boost::shared_ptr<CAveInstCopyItem> cpyItem( new CAveInstCopyItemDirectory);
					if(SUCCEEDED(hRes = cpyItem->LoadFromNode(newNode)))
						m_Items.push_back(cpyItem);
				}
				else if(_wcsicmp(wText,L"file") == 0)
				{
					boost::shared_ptr<CAveInstCopyItem> cpyItem( new CAveInstCopyItemFile);
					if(SUCCEEDED(hRes = cpyItem->LoadFromNode(newNode)))
						m_Items.push_back(cpyItem);
				}
				else if(_wcsicmp(wText,L"CopyAll") == 0)
				{
					boost::shared_ptr<CAveInstCopyItem> cpyItem( new CAveInstCopyItemAllFilesAndSubFolders);
					if(SUCCEEDED(hRes = cpyItem->LoadFromNode(newNode)))
						m_Items.push_back(cpyItem);
				}
			}
		}
	}

	return S_OK;
}