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

CLayers::CLayers(HWND ownerHWnd) :
	m_ownerHWnd(ownerHWND)
{

}


Layer& CLayers::Add(std::string& const name)
{
	std::map<std::string,CLayer>::iterator iter = m_layers.find(name);

	if(iter != m_layers.end() )
		throw new std::exception(std::string("The layer with the name \"")+name+"\" already exists.");

	UINT layerId = ::DeskletLayerAdd(m_ownerHWnd,name.c_str() );

	BOOL success(FALSE);
	UINT newLayerId = DeskletLayerGetLayer(m_ownerHWND,name.c_str(),&success);

	if(!success || layerId != newLayerId)
		throw new std::exception(std::string("The layer with the name \"")+name+"\" was not added.");	

	m_layers.add( std::make_pair(name,Layer(name,*this)) );

	return m_layers[name];
}

void  CLayers::Remove(std::string& const name, BOOL bRedraw)
{
	BOOL success(FALSE);
	UINT layerId = ::DeskletLayerGetLayer(m_ownerHWnd, name.c_str(),&success);
	if(success)
		::DeskletLayerRemove(m_ownerHWnd,layerId,btRedraw);

	std::map<std::string,CLayer>::iterator iter = m_layers.find(name);
	if(iter != m_layers.end() )
		m_layers.erase( iter );
}

UINT CLayers::Size()const
{
	return ::DeskletLayerCount(m_ownerHWND);
}

const Layer& CLayers::operator[](std::string& const name);
{
	std::map<std::string,CLayer>::iterator iter = m_layers.find(name);
	if(iter == m_layers.end() )
	{
		BOOL success(FALSE);
		UINT layerId = ::DeskletLayerGetLayer(m_ownerHWnd, name.c_str(),&success);
		if(success)
			m_layers.add( std::make_pair(name,Layer(name,*this)) );

		iter =  m_layers.find(name);
	}

	if(iter == m_layers.end() )
		throw new std::exception(std::string("The layer with the name \"")+name+"\" was not found.");	

	return *iter;

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