// AveInstCopyItemFile.cpp: implementation of the CAveInstCopyItemFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AveInstCopyItemFile.h"
#include "AveInstallerEnvironment.h"
#include "AveInstFileDirOperation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveInstCopyItemFile::CAveInstCopyItemFile()
{

}

CAveInstCopyItemFile::~CAveInstCopyItemFile()
{

}

std::wstring CAveInstCopyItemFile::GetNewInputPath(CAveInstallerEnvironment& env, const std::wstring& inputPath)
{
	WCHAR srcPath[MAX_PATH] = {0};
	wcscpy(srcPath,inputPath.c_str() );
	PathAppendW(srcPath,m_FileName.c_str() );

	return srcPath;
}

std::wstring CAveInstCopyItemFile::GetNewOutputPath(CAveInstallerEnvironment& env, const std::wstring& outputPath)
{
	WCHAR destPath[MAX_PATH] = {0};
	wcscpy(destPath,outputPath.c_str() );

	std::wstring possibleAlias = env.GetFileAlias(m_FileName);

	if(possibleAlias == L"")
		PathAppendW(destPath, m_FileName.c_str() );
	else
		PathAppendW(destPath,possibleAlias.c_str() );

	return destPath;
}

bool CAveInstCopyItemFile::PerformOperation(CAveInstFileDirOperation& op, CAveInstallerEnvironment& env, const std::wstring& inputPath, const std::wstring& outputPath)
{
	std::wstring srcPath  = GetNewInputPath(env,inputPath);
	std::wstring destPath = GetNewOutputPath(env,outputPath);

	bool res = op.FileOperation(env, srcPath, destPath);

	if(res)
		CopyIntern(env,srcPath,destPath);

	return res;
}

HRESULT CAveInstCopyItemFile::LoadFromNode(XMLNode& node)
{
	HRESULT hRes = S_OK;

	if(NULL == node.p)
		return (hRes = E_FAIL);

	hRes = CAveInstCopyItem::LoadFromNode(node);

	CComBSTR bstrFileName;
	if(FAILED( hRes = node->get_text(&bstrFileName) ))
		return hRes;

	if(bstrFileName != NULL)
		m_FileName = static_cast<WCHAR*>(bstrFileName);

	return S_OK;
}