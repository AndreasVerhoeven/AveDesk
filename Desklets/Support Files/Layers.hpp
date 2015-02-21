//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef LAYERS_HPP_20C293AB93848F210EDD9119292C3938471A
#define LAYERS_HPP_20C293AB93848F210EDD9119292C3938471A

#include "DeskletSDK.h"

#include "Layer.hpp"

#include <map>
#include <string>

class CLayers
{
private:
	std::map<std::string,CLayer> m_layers;
	HWND m_ownerHWnd;

public:
	CLayers(HWND ownerHWnd);
	CLayers();

	HWND GetWindow()const;
	void SetOwnerWindow(HWND hWnd);

	CLayer& Add(std::string const& name);
	void   Remove(std::string const& name, BOOL bRedraw=FALSE);

	UINT Size()const;

	CLayer& operator[](std::string const& name);

	void MergeAll(BOOL bRedraw=FALSE);
	void MergeVisible(BOOL bRedraw=FALSE);

	void RemoveAll();

	BOOL Exists(std::string const& name)const;
};

#endif//LAYERS_HPP_20C293AB93848F210EDD9119292C3938471A