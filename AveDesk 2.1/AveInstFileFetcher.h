// AveInstFileFetcher.h: interface for the CAveInstFileFetcher class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTFILEFETCHER_H__0EE12713_5221_4287_BAA9_CF1AD885375D__INCLUDED_)
#define AFX_AVEINSTFILEFETCHER_H__0EE12713_5221_4287_BAA9_CF1AD885375D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DownloadableFileDescription.h"
#include "AveInstProgressIndicator.h"
#include "AveInstCancelable.h"

class CAveInstFileFetcher :
	public CAveInstCancelable
{
private:
	bool m_GoOn;
public:
	CAveInstFileFetcher();
	virtual ~CAveInstFileFetcher();

	virtual HRESULT FetchFile(CDownloadableFileDescription& descr,CAveInstProgressIndicator* indicator, const std::wstring& outputFile);
	virtual HRESULT FetchFileFromGUID(const std::wstring& guid, CAveInstProgressIndicator* indicator, const std::wstring& outputFile);
	virtual HRESULT FetchFileFromURL(const std::wstring& url, CAveInstProgressIndicator* indicator, const std::wstring& outputFile);
	virtual bool Cancel();

};

#endif // !defined(AFX_AVEINSTFILEFETCHER_H__0EE12713_5221_4287_BAA9_CF1AD885375D__INCLUDED_)
