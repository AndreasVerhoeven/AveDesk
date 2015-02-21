// DeskletSkinEnumerator.cpp: implementation of the DeskletSkinEnumerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletSkinEnumerator.h"
#include "DockletDLL.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DeskletSkinEnumerator::DeskletSkinEnumerator(DockletDLL* dll) :
	m_Dll(dll),
	m_P(NULL),
	m_Num(0),
	m_RefCount(0)
{
	if(m_Dll)
		m_Dll->AddRef();

	m_P = m_Dll->GetDocklet()->CreateSkinEnumeratorW();
}

DeskletSkinEnumerator::~DeskletSkinEnumerator()
{
	m_Dll->GetDocklet()->DestroySkinEnumeratorW(m_P);

	if(m_Dll)
		m_Dll->Release();
}

DeskletSkinEnumerator* DeskletSkinEnumerator::Create(DockletDLL* dll)
{
	DeskletSkinEnumerator* dse = new DeskletSkinEnumerator(dll);
	dse->AddRef();
	return dse;
}

ULONG DeskletSkinEnumerator::AddRef()
{
	m_RefCount++;
	return m_RefCount;
}

ULONG DeskletSkinEnumerator::Release()
{
	m_RefCount--;

	if(m_RefCount == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return m_RefCount;
	}
}

bool DeskletSkinEnumerator::Next(DeskletSkinInfo* info)
{
	const int NO_MORE_SKINS = FALSE;
	const int MORE_SKINS	= TRUE;
	const int BUF_TOO_SMALL	= -1;
	const int JUDGE_WANTS_IT_DIFFERENT = -2;
	const int JUDGE_DOESNOT_WANT_IT_DIFFERENT = -3;

	SkinInfoWrapper* wrapper = new SkinInfoWrapper;

	INT res;
	do
	{
		wrapper->num = m_Num;

		res = m_Dll->GetDocklet()->GetSkinW(m_P,wrapper);

		if(res == BUF_TOO_SMALL)
			wrapper->AllocateAssignedSize();

		else if(res == JUDGE_WANTS_IT_DIFFERENT)
		{
			DeleteSkinInfoWrapper(wrapper);
			wrapper = reinterpret_cast<SkinInfoWrapper*>(new SkinInfoWrapper2);
		}
		else if(res == JUDGE_DOESNOT_WANT_IT_DIFFERENT)
		{
			DeleteSkinInfoWrapper(wrapper);
			wrapper = new SkinInfoWrapper;
		}

	}while(res == BUF_TOO_SMALL || res == JUDGE_WANTS_IT_DIFFERENT || res == JUDGE_DOESNOT_WANT_IT_DIFFERENT);

	m_Num++;

	if(res == MORE_SKINS)
		info->SetSkinInfo(*wrapper);

	DeleteSkinInfoWrapper(wrapper);

	return res == MORE_SKINS;
}

void DeskletSkinEnumerator::Reset()
{
	m_Num = 0;
}