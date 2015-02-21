// IAveResources.h: interface for the IAveResources class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAVERESOURCES_H__C6DCEE4A_73D4_4A52_BCB8_5DC2B49BB376__INCLUDED_)
#define AFX_IAVERESOURCES_H__C6DCEE4A_73D4_4A52_BCB8_5DC2B49BB376__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "controls_header.h"

class IAveResources  
{
	std::string dirName;
public:
	IAveResources(const char* dirName);
	virtual ~IAveResources();

	virtual Bitmap* GetBitmap(const char* resName);
	virtual std::string GetFilename(const char* resName);

};

#endif // !defined(AFX_IAVERESOURCES_H__C6DCEE4A_73D4_4A52_BCB8_5DC2B49BB376__INCLUDED_)
