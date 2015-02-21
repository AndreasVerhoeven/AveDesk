#pragma once

#include "DeskletProvider.h"

class CDeskletProviderFactory
{
public:
	CDeskletProviderFactory(void);
public:
	virtual ~CDeskletProviderFactory(void);
	virtual CDeskletProvider* CreateProvider(const std::wstring& name);
};
