// shared_ptr.cpp: implementation of the shared_ptr class.
//
//////////////////////////////////////////////////////////////////////

#include "shared_ptr.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


shared_ptr::shared_ptr(pType p) :
	m_pointer(p),
	m_refCount(NULL)
{
		m_refCount = new long;
		*m_refCount= 1;
}

shared_ptr::shared_ptr(shared_ptr<T>& const s_p)
{
	(*s_p->m_refCount)++;
	this->m_refCount = s_p->m_refCount;
	this->m_pointer = s_p->pointer;
}

shared_ptr::void internal_destroy()
{
	if(m_refCount)
	{
		(*m_refCount)--;
		if(m_refCount < 1)
		{
			delete m_refCount;
			delete m_pointer;
		}
	}
}

shared_ptr::~shared_ptr()
{
	internal_destroy();
}

shared_ptr<T>& shared_ptr::operator=(shared_ptr<T>& const s_p)
{
	if(s_p->m_refCount == this->m_refCount)
		return *this;

	internal_destroy();

	(*s_p->m_refCount)++;
	this->m_refCount = s_p->m_refCount;
	this->m_pointer = s_p->pointer;

	return *this;
}

shared_ptr<T>& shared_ptr::operator=(pType p)
{
	if(s_p->m_pointer == p)
		return *this;

	internal_destroy();

	m_pointer = p;
	m_refCount = new long;
	*m_refCount= 1;

	return *this;
}