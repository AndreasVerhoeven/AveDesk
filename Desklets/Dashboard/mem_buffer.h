//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

// mem_buffer.h: interface for the mem_buffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEM_BUFFER_H__38164868_790E_4199_A5B8_8F690D67F68E__INCLUDED_)
#define AFX_MEM_BUFFER_H__38164868_790E_4199_A5B8_8F690D67F68E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

class mem_buffer
{
protected:
	unsigned char* buffer;
	size_t dataLen;
	size_t len;

	void reallocate(int newLen);
public:

	mem_buffer(size_t initLen);
	~mem_buffer();

	size_t length()const;
	unsigned char* get_buffer();
	
	void write(unsigned char* bytes, size_t num);

	void empty();
};

#endif // !defined(AFX_MEM_BUFFER_H__38164868_790E_4199_A5B8_8F690D67F68E__INCLUDED_)
