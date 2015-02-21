//
//	This file is part of the DeskletSDK for AveDesk 1.1
//
//	(c) copyright Andreas Verhoeven 2004
//
//

#ifndef LAYERS_HPP_20C293AB93848F210EDD9119292C3938471A
#define LAYERS_HPP_20C293AB93848F210EDD9119292C3938471A

#include "DeskletSDK.h"
#include "DeskletSDK.cpp"

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

	Layer& Add(std::string& const name);
	void   Remove(std::string& const name, BOOL bRedraw=FALSE);

	UINT Size()const;

	const Layer& operator[](std::string& const name);

	void MergeAll(BOOL bRedraw=FALSE);
	void MergeVisible(BOOL bRedraw=FALSE);
};

unsigned int CALLBACK DeskletLayerGetLayer(HWND hwndDesklet, const char* name, BOOL* success);
unsigned int CALLBACK DeskletLayerCount(HWND hwndDesklet);
unsigned int CALLBACK DeskletLayerAdd(HWND hwndDesklet, const char* name);
void  CALLBACK DeskletLayerRemove(HWND hwndDesklet, unsigned int id, BOOL redraw);
void  CALLBACK DeskletLayerMergeAll(HWND hwndDesklet, BOOL redraw);
void  CALLBACK DeskletLayerMergeVisible(HWND hwndDesklet,  BOOL redraw);



#endif//LAYERS_HPP_20C293AB93848F210EDD9119292C3938471A