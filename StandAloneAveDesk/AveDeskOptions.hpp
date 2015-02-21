// filename:		GlobalSettingsMoveSheet.hpp
// Description:		Definition of class CGlobalSettingsMoveSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#ifndef AVEDESK_GLOBAL_SETTINGS_HPP_393
#define AVEDESK_GLOBAL_SETTINGS_HPP_393

#pragma once

#include "stdafx.h"
#include "constants.h"

class AveDeskGlobalSettings
{
	private:
		unsigned int moveDelayInMs;
		int clickMode;

		int gridX;
		int gridY;
		int gridXMargin;
		int gridYMargin;

		bool hoverSelection;
		unsigned int hoverSelectionDelay;

		char iconFile[MAX_PATH];
		long iconIndex;

		bool globalSettingsIsVisible;


	public:
		AveDeskGlobalSettings();
		ReadFromIni(const char* iniPath, const char* section);
		SaveToIni(const char* iniPath,const char* section);

		unsigned int  GetMoveDelayInMs()const{return moveDelayInMs;};
		void SetMoveDelayInMs(unsigned int val){moveDelayInMs = val;};

		int GetClickMode() { return clickMode;};
		void SetClickMode(int val){clickMode = val;};

		int GetGridX() {return gridX;};
		int GetGridY() {return gridY;};
		int GetGridXMargin() { return gridXMargin;};
		int GetGridYMargin() { return gridYMargin;};

		void SetGridX(int val) { gridX = val;};
		void SetGridY(int val) { gridY = val;};
		void SetGridXMargin(int val) { gridXMargin = val;};
		void SetGridYMargin(int val) { gridYMargin = val;};

		bool GetHoverSelection() { return hoverSelection;};
		unsigned int GetHoverSelectionDelay() { return hoverSelectionDelay;};

		void SetHoverSelection(bool val) { hoverSelection = val;};
		void SetHoverSelectionDelay(unsigned int val) { hoverSelectionDelay = val;};

		const char* GetIconFile() { return iconFile;};
		long GetIconIndex() { return iconIndex;};
		void SetIconFile(const char* val, long index){ strcpy(iconFile,val);iconIndex = index;};
};

#endif//AVEDESK_GLOBAL_SETTINGS_HPP_393
		int moveDelayInMs = 500;
		int clickMode = CLICKMODE_SINGLE;

		int gridX = 128;
		int gridY = 128;
		int gridXMargin = 0;
		int gridYMargin = 0;

		bool hoverSelection = false;
		UINT hoverSelectionDelay = 500;

		// some global variables that apply to settings
		char iconFile[MAX_PATH];
		LONG iconIndex(0);
