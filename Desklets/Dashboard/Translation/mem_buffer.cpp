// mem_buffer.cpp: implementation of the mem_buffer class.
//
//////////////////////////////////////////////////////////////////////

#include "mem_buffer.h"
#include <memory>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void mem_buffer::reallocate(int newLen)
{
	unsigned char* b = new unsigned char[newLen];
	if(buffer != NULL)
		memcpy(b, buffer, len);

	delete[] buffer;

	buffer = b;
	len = newLen;
}


mem_buffer::mem_buffer(size_t initLen) : dataLen(0), len(initLen), buffer(NULL)
{
	buffer = new unsigned char[initLen];
}

mem_buffer::~mem_buffer()
{
	empty();
}

size_t mem_buffer::length()const
{
	return dataLen;
};
unsigned char* mem_buffer::get_buffer()
{
	return buffer;
};
	
void mem_buffer::write(unsigned char* bytes, size_t num)
{
	if(num + dataLen > len)
		reallocate(len + num + 6400);

	memcpy(buffer + dataLen, bytes, num);
	dataLen += num;
}

void mem_buffer::empty()
{
	delete[] buffer;
	buffer = NULL;
	len = 0;
	dataLen = 0;
}