// AveLabel.h: interface for the CAveLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVELABEL_H__8589D20F_7BD3_43F2_8A49_D6A60DC644D5__INCLUDED_)
#define AFX_AVELABEL_H__8589D20F_7BD3_43F2_8A49_D6A60DC644D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IAveControl.h"
#include "AveFont.h"

class CAveLabel : public IAveControl  
{
protected:
	CAveFont font;
	std::wstring text;

public:
	CAveLabel();
	virtual ~CAveLabel();

	CAveLabel(IAveControlContainer* container, std::string name);

	virtual void SetText(const std::wstring ws);
	virtual std::wstring GetText()const;

	virtual void UpdateSize();

	virtual CAveFont& GetFont();
	void Update();

	virtual DWORD_PTR InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

};

#endif // !defined(AFX_AVELABEL_H__8589D20F_7BD3_43F2_8A49_D6A60DC644D5__INCLUDED_)
