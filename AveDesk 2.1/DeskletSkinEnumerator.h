// DeskletSkinEnumerator.h: interface for the DeskletSkinEnumerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETSKINENUMERATOR_H__79FB0043_2910_4AF0_9E85_4969AB20459A__INCLUDED_)
#define AFX_DESKLETSKINENUMERATOR_H__79FB0043_2910_4AF0_9E85_4969AB20459A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DockletDLL;

//#include "DeskletSkinInfo.h"
class DeskletSkinInfo;

#include<string>

class DeskletSkinEnumerator  
{
private:
	DockletDLL* m_Dll;
	void* m_P;
	UINT m_Num;

	ULONG m_RefCount;

	DeskletSkinEnumerator(DockletDLL* dll);
	~DeskletSkinEnumerator();
public:

	static DeskletSkinEnumerator* Create(DockletDLL* dll);
	ULONG AddRef();
	ULONG Release();


	bool Next(DeskletSkinInfo* info);
	void Reset();

	void* GetPointer()const{return m_P;};

};

#endif // !defined(AFX_DESKLETSKINENUMERATOR_H__79FB0043_2910_4AF0_9E85_4969AB20459A__INCLUDED_)
