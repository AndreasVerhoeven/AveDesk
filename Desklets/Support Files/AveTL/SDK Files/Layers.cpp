//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef LAYERS_CPP_20C293AB93848F210EDD9119292C3938471A
#define LAYERS_CPP_20C293AB93848F210EDD9119292C3938471A


#include "Layers.hpp"

#include <exception>
#include <string>
#include <cassert>

CLayers::CLayers(HWND ownerHWnd) :
	m_ownerHWnd(ownerHWnd)
{

}

CLayers::CLayers() :
	m_ownerHWnd(NULL)
{

}

HWND CLayers::GetWindow()const
{
	return m_ownerHWnd;
}

void CLayers::SetOwnerWindow(HWND hWnd)
{
	assert(m_ownerHWnd == NULL);

	m_ownerHWnd = hWnd;

	m_layers.clear();
}

CLayer& CLayers::Add(std::string const& name)
{
	std::map<std::string,CLayer>::iterator iter = m_layers.find(name);

	if(iter != m_layers.end() )
		throw new std::exception(std::string(std::string("The layer with the name \"")+name+"\" already exists.").c_str());

	UINT layerId = ::DeskletLayerAdd(m_ownerHWnd,name.c_str() );

	BOOL success(FALSE);
	UINT newLayerId = DeskletLayerGetLayer(m_ownerHWnd,name.c_str(),&success);

	if(!success || layerId != newLayerId)
		throw new std::exception(std::string(std::string("The layer with the name \"")+name+"\" was not added.").c_str());	

	m_layers.insert( std::make_pair(name,CLayer(this,name)) );

	return m_layers[name];
}

void  CLayers::Remove(std::string const& name, BOOL bRedraw)
{
	BOOL success(FALSE);
	UINT layerId = ::DeskletLayerGetLayer(m_ownerHWnd, name.c_str(),&success);
	if(success)
		::DeskletLayerRemove(m_ownerHWnd,layerId,bRedraw);

	std::map<std::string,CLayer>::iterator iter = m_layers.find(name);
	if(iter != m_layers.end() )
		m_layers.erase( iter );
}

UINT CLayers::Size()const
{
	return ::DeskletLayerCount(m_ownerHWnd);
}

const CLayer& CLayers::operator[](std::string const& name)
{
	std::map<std::string,CLayer>::iterator iter = m_layers.find(name);
	if(iter == m_layers.end() )
	{
		BOOL success(FALSE);
		UINT layerId = ::DeskletLayerGetLayer(m_ownerHWnd, name.c_str(),&success);
		if(success)
			m_layers.insert( std::make_pair(name,CLayer(this,name)) );

		iter =  m_layers.find(name);
	}

	if(iter == m_layers.end() )
		throw new std::exception(std::string(std::string("The layer with the name \"")+name+"\" was not found.").c_str());	

	return iter->second;

}

void CLayers::MergeAll(BOOL bRedraw)
{
	m_layers.clear();

	::DeskletLayerMergeAll(m_ownerHWnd,bRedraw);
}

void CLayers::MergeVisible(BOOL bRedraw)
{
	m_layers.clear();

	::DeskletLayerMergeVisible(m_ownerHWnd,bRedraw);
}

#endif//LAYERS_CPP_20C293AB93848F210EDD9119292C3938471A