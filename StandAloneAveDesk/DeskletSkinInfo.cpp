// DeskletSkinInfo.cpp: implementation of the DeskletSkinInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletSkinInfo.h"


void DeleteSkinInfoWrapper(SkinInfoWrapper* wrapper)
{
	if(wrapper)
	{
		if(sizeof(SkinInfo) == wrapper->dwSize)
		{
			delete wrapper;
		}
		else if(sizeof(SkinInfo2) == wrapper->dwSize)
		{
			SkinInfoWrapper2* wrapper2 = reinterpret_cast<SkinInfoWrapper2*>(wrapper);
			delete wrapper2;
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DeskletSkinInfo::DeskletSkinInfo() :
	m_Num(0), m_Version(0), m_Wrp(NULL), m_HasVersionAsString(false)
{
	
}

DeskletSkinInfo::DeskletSkinInfo(const SkinInfo& wrapper) :
	m_Num(0), m_Version(0), m_Wrp(NULL), m_HasVersionAsString(false)
{
	SetSkinInfo(wrapper);
}

DeskletSkinInfo::~DeskletSkinInfo()
{
	if( m_Wrp )
	{
		m_Wrp->Release();
		m_Wrp = NULL;
	}
}

void DeskletSkinInfo::Clear()
{
	m_Num = 0;
	m_Version = 0;
	m_Name = L"";
	m_Author = L"";
	m_Notes = L"";
	m_Version = 0;
	m_HasVersionAsString = false;
	m_VersionString = L"";
	if( m_Wrp )
	{
		m_Wrp->Release();
		m_Wrp = NULL;
	}
}

bool DeskletSkinInfo::HasVersionAsString()const
{
	return m_HasVersionAsString;
}

std::wstring DeskletSkinInfo::GetVersionAsString()const
{
	return m_VersionString;
}


bool DeskletSkinInfo::IsAssigned()const
{
	return !m_Name.empty();
}

std::wstring DeskletSkinInfo::GetKey()const
{
	return m_Key;
}

UINT DeskletSkinInfo::GetNum()const
{
	return m_Num;
}

std::wstring DeskletSkinInfo::GetName()const
{
	return m_Name;
}

std::wstring DeskletSkinInfo::GetAuthor()const
{
	return m_Author;
}

std::wstring DeskletSkinInfo::GetNotes()const
{
	return m_Notes;
}

int DeskletSkinInfo::GetVersion()const
{
	return m_Version;
}

bool DeskletSkinInfo::GetWrapperDisp(IDispatch **unkRet)
{
	if( NULL == m_Wrp )
	{
		CComObject<CDeskletSkin>::CreateInstance(&m_Wrp);
		if( m_Wrp )
		{
			m_Wrp->AddRef();
			m_Wrp->SetData(NULL, this);
		}
	}
	if( m_Wrp )
	{
		m_Wrp->QueryInterface(unkRet);
		return true;
	}
	return false;
}

SkinInfoWrapper* DeskletSkinInfo::GetSkinInfoWrapper()const
{
	
	SkinInfoWrapper* w = NULL;
	if(m_HasVersionAsString)
	{
		SkinInfoWrapper2* w2 = new SkinInfoWrapper2;
		w2->maxVersionStringLength = m_VersionString.length() + 1;
		w2->AllocateAssignedSize();
		wcscpy(w2->versionString, m_VersionString.c_str());

		w = reinterpret_cast<SkinInfoWrapper*>(w2);
	}
	else
	{
		w = new SkinInfoWrapper;
	}

	w->maxAuthorLength = m_Author.length() + 1;
	w->maxKeyLength = m_Key.length() + 1;
	w->maxNameLength = m_Name.length() + 1;
	w->maxNotesLength = m_Notes.length() + 1;
	w->AllocateAssignedSize();
	w->num = m_Num;
	w->version = m_Version;

	wcscpy(w->key,m_Key.c_str());
	wcscpy(w->author,m_Author.c_str());
	wcscpy(w->name,m_Name.c_str());
	wcscpy(w->notes,m_Notes.c_str());

	return w;
}

void DeskletSkinInfo::SetSkinInfo(const SkinInfo &wrapper)
{
	if(wrapper.key != NULL)
		m_Key = wrapper.key;

	m_Num = wrapper.num;

	if(wrapper.name != NULL)
		m_Name = wrapper.name;

	if(wrapper.author != NULL)
		m_Author = wrapper.author;

	if(wrapper.notes != NULL)
		m_Notes = wrapper.notes;

	m_Version  = wrapper.version;

	if(wrapper.dwSize == sizeof(SkinInfo2))
	{
		m_HasVersionAsString = true;

		SkinInfo2* skinInfo2 = const_cast<SkinInfo2*>(reinterpret_cast<const SkinInfo2*>(&wrapper));

		if(skinInfo2->versionString != NULL)
			m_VersionString = skinInfo2->versionString;
	}
	else
	{
		m_HasVersionAsString = false;
	}
}
