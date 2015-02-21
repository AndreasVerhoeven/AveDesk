// shared_ptr.hpp: interface for the shared_ptr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHARED_PTR_HPP__80F72234_1543_4A2C_834F_772339F846F2__INCLUDED_)
#define AFX_SHARED_PTR_HPP__80F72234_1543_4A2C_834F_772339F846F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class template shared_ptr<T> 
{
	typedef T type;
	typedef type* pType

private:
	pType m_pointer;
	long*   m_refCount;

	void internal_destroy();
public:

public:
	shared_ptr(pType p);
	shared_ptr(shared_ptr<T>& const s_p);

	shared_ptr<T>& operator=(shared_ptr<T>& const s_p);
	shared_ptr<T>& operator=(pType p);

	virtual ~shared_ptr();

};

#endif // !defined(AFX_SHARED_PTR_HPP__80F72234_1543_4A2C_834F_772339F846F2__INCLUDED_)
