// AveInstShowBitmapAction.h: interface for the CAveInstShowBitmapAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTSHOWBITMAPACTION_H__DA9F970A_4F00_4376_97B6_A371D47FA8B1__INCLUDED_)
#define AFX_AVEINSTSHOWBITMAPACTION_H__DA9F970A_4F00_4376_97B6_A371D47FA8B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AveInstAction.h"

#include <string>

class CAveInstShowBitmapAction : public CAveInstAction  
{
protected:
	std::wstring m_BitmapFileName;

public:
	CAveInstShowBitmapAction();
	virtual ~CAveInstShowBitmapAction();

	virtual HRESULT LoadFromNode(XMLNode& node);
	virtual HRESULT DoAction(CAveInstallerEnvironment& env);
};

#endif // !defined(AFX_AVEINSTSHOWBITMAPACTION_H__DA9F970A_4F00_4376_97B6_A371D47FA8B1__INCLUDED_)
