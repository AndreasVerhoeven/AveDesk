// AveInstSuspendedDesklets.h: interface for the CAveInstSuspendedDesklets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVEINSTSUSPENDEDDESKLETS_H__B369AD8D_F048_4D3C_9C70_537181DBA18D__INCLUDED_)
#define AFX_AVEINSTSUSPENDEDDESKLETS_H__B369AD8D_F048_4D3C_9C70_537181DBA18D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <list>

class Docklet;

class CAveInstSuspendedDesklets  
{
public:
	typedef CComPtr<IXMLDOMNode> XMLNode;

	// the STL doesn't like the overloaded & operator of CComPtr<T>, so wrap it
	// up.
	struct XMLNodeWrapper
	{
	public:
		XMLNode node;
		XMLNodeWrapper() : node(NULL){}
		XMLNodeWrapper(XMLNode node_) : node(node_){}
	};
protected:
	long m_Count;

	std::list<XMLNodeWrapper> m_Desklets;

	void ResumeDesklets();
public:

	CAveInstSuspendedDesklets();
	virtual ~CAveInstSuspendedDesklets();

	virtual HRESULT Add(Docklet* d);
	virtual long AddCount();
	virtual long ReleaseCount();

	std::wstring GetFileName()const;

	int GetNumberOfSuspendedDesklets()const;
};

#endif // !defined(AFX_AVEINSTSUSPENDEDDESKLETS_H__B369AD8D_F048_4D3C_9C70_537181DBA18D__INCLUDED_)
