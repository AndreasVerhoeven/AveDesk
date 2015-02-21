// AveInstAction.h: interface for the CAveInstAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTACTION_H__386A3188_CD11_4126_ADE1_B701A3331970__INCLUDED_)
#define AFX_AVEINSTACTION_H__386A3188_CD11_4126_ADE1_B701A3331970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAveInstallerEnvironment;

class CAveInstAction  
{
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

	CAveInstAction();
	virtual ~CAveInstAction();

	virtual HRESULT LoadFromNode(XMLNode& node) = 0;
	virtual HRESULT DoAction(CAveInstallerEnvironment& env) = 0;

};

#endif // !defined(AFX_AVEINSTACTION_H__386A3188_CD11_4126_ADE1_B701A3331970__INCLUDED_)
