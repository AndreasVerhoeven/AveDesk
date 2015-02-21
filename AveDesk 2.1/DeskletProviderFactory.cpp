#include "stdafx.h"
#include "DeskletProviderFactory.h"
#include "InstalledDeskletProvider.h"
#include <algorithm>

CDeskletProviderFactory::CDeskletProviderFactory(void)
{
}

CDeskletProviderFactory::~CDeskletProviderFactory(void)
{
}

CDeskletProvider* CDeskletProviderFactory::CreateProvider(const std::wstring& name)
{
	std::wstring lcName(name);
	std::for_each(lcName.begin(), lcName.end(), tolower);

	if(name == L"installed")
		return new CInstalledDeskletProvider;
	
	return NULL;
}