// filename:		AveDeskGlobalSettings.hpp
// Description:		Definition of class AveDeskGlobalSettings
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
#include <string>


// dirty MACRO that declares a property that is only used for storage. The
// property is kept private, but Get- and Setters are provided.
// NOTE: the access is modified to public after using this macro.
#define AGSPROPERTY(t,n) private: ##t m_##n; \
public:	void Set##n (##t const & v) { m_##n = v; }; \
		##t Get##n () {return m_##n;}

#include "IPersistable.h"

#include "ShowCase1.h"

extern CComObject<CShowCase> *_ShowCase;

class AveDeskGlobalSettings : public IPersistable
{
	private:
		AGSPROPERTY(int,MoveDelayInMs);
		AGSPROPERTY(int,ClickMode);
		AGSPROPERTY(int,GridX);
		AGSPROPERTY(int,GridY);
		AGSPROPERTY(int,GridXMargin);
		AGSPROPERTY(int,GridYMargin);
		AGSPROPERTY(bool,HoverSelection);
		AGSPROPERTY(UINT,HoverSelectionDelay);
		AGSPROPERTY(std::wstring,IconFileName);
		AGSPROPERTY(long,IconIndex);
		AGSPROPERTY(bool,Ask);
		AGSPROPERTY(bool,ShowTrayIcon);
		AGSPROPERTY(std::wstring,ThemeDir);
		AGSPROPERTY(std::wstring,ThemeFileName);
		AGSPROPERTY(bool,AutoSaveThemes);
		AGSPROPERTY(std::wstring, TrayAnimationFolder);
		AGSPROPERTY(bool,DoTrayAnimation);
		AGSPROPERTY(bool,AddPreviewInThemeFiles);
		AGSPROPERTY(bool, DisableFlipEffect);
		AGSPROPERTY(UINT, FlippingSpeed);
		AGSPROPERTY(bool, DisableCloseEffect);
		AGSPROPERTY(UINT, ClosingSpeed);
		AGSPROPERTY(bool, FadeOutWhileDoingClosingEffect);
		AGSPROPERTY(bool, First13Boot);
		AGSPROPERTY(UINT, IconPreviewSize);
		AGSPROPERTY(UINT, DeskletBrowserSplitterPos);

	private:
		bool m_GlobalSettingsIsVisible;

	private:
		bool m_NoMultipleUserAwareness;

	public:
		AveDeskGlobalSettings();

		bool IsNoMultipleUserAwareness()const;
		void SetNoMultipleUserAwareness(const bool val);

		// persistancy maps.
		BEGIN_LOADING_MAP()

			LOADABLE_PROP_PRIMITIVE(MoveDelayInMs);
			LOADABLE_PROP_PRIMITIVE(ClickMode);
			LOADABLE_PROP_PRIMITIVE(GridX);
			LOADABLE_PROP_PRIMITIVE(GridY);
			LOADABLE_PROP_PRIMITIVE(GridXMargin);
			LOADABLE_PROP_PRIMITIVE(GridYMargin);
			LOADABLE_PROP_PRIMITIVE(HoverSelection);
			LOADABLE_PROP_PRIMITIVE(HoverSelectionDelay);
			LOADABLE_PROP_PRIMITIVE(ShowTrayIcon);
			LOADABLE_PROP_PRIMITIVE(IconFileName);
			LOADABLE_PROP_PRIMITIVE(IconIndex);
			LOADABLE_PROP_PRIMITIVE(Ask);
			LOADABLE_PROP_PRIMITIVE(ThemeFileName);
			LOADABLE_PROP_PRIMITIVE(AutoSaveThemes);
			LOADABLE_CLASS_PTR_NAMED(_ShowCase,ShowCase);
			LOADABLE_PROP_PRIMITIVE(TrayAnimationFolder);
			LOADABLE_PROP_PRIMITIVE(DoTrayAnimation);
			LOADABLE_PROP_PRIMITIVE(AddPreviewInThemeFiles);
			LOADABLE_PROP_PRIMITIVE(DisableFlipEffect);
			LOADABLE_PROP_PRIMITIVE(FlippingSpeed);
			LOADABLE_PROP_PRIMITIVE(DisableCloseEffect);
			LOADABLE_PROP_PRIMITIVE(ClosingSpeed);
			LOADABLE_PROP_PRIMITIVE(FadeOutWhileDoingClosingEffect);
			LOADABLE_PROP_PRIMITIVE(First13Boot);
			LOADABLE_PROP_PRIMITIVE(IconPreviewSize);
			LOADABLE_PROP_PRIMITIVE(DeskletBrowserSplitterPos);

		END_LOADING_MAP()

		BEGIN_SAVING_MAP()

			SAVABLE_PROP_PRIMITIVE(MoveDelayInMs);
			SAVABLE_PROP_PRIMITIVE(ClickMode);
			SAVABLE_PROP_PRIMITIVE(GridX);
			SAVABLE_PROP_PRIMITIVE(GridY);
			SAVABLE_PROP_PRIMITIVE(GridXMargin);
			SAVABLE_PROP_PRIMITIVE(GridYMargin);
			SAVABLE_PROP_PRIMITIVE(HoverSelection);
			SAVABLE_PROP_PRIMITIVE(HoverSelectionDelay);
			SAVABLE_PROP_PRIMITIVE(ShowTrayIcon);
			SAVABLE_PROP_PRIMITIVE(IconFileName);
			SAVABLE_PROP_PRIMITIVE(IconIndex);
			SAVABLE_PROP_PRIMITIVE(Ask);
			SAVABLE_PROP_PRIMITIVE(ThemeFileName);
			SAVABLE_PROP_PRIMITIVE(AutoSaveThemes);
			SAVABLE_CLASS_PTR_NAMED(_ShowCase,ShowCase);
			SAVABLE_PROP_PRIMITIVE(TrayAnimationFolder);
			SAVABLE_PROP_PRIMITIVE(DoTrayAnimation);
			SAVABLE_PROP_PRIMITIVE(AddPreviewInThemeFiles);
			SAVABLE_PROP_PRIMITIVE(DisableFlipEffect);
			SAVABLE_PROP_PRIMITIVE(FlippingSpeed);
			SAVABLE_PROP_PRIMITIVE(DisableCloseEffect);
			SAVABLE_PROP_PRIMITIVE(ClosingSpeed);
			SAVABLE_PROP_PRIMITIVE(FadeOutWhileDoingClosingEffect);
			SAVABLE_PROP_PRIMITIVE(First13Boot);
			SAVABLE_PROP_PRIMITIVE(IconPreviewSize);
			SAVABLE_PROP_PRIMITIVE(DeskletBrowserSplitterPos);

		END_SAVING_MAP()

		bool SetIconFile(const WCHAR* file, long index, bool show); 

		int ShowDialog();
};

#endif//AVEDESK_GLOBAL_SETTINGS_HPP_393