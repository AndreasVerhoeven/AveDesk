// filename:		labelbackground.h
// Description:		Definition of class LabelBackground
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					<none>
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#ifndef LABELBACKGROUND_H___122
#define LABELBACKGROUND_H___122


#include <windows.h>
#include "callback_funcs.h"
#include "helper_funcs.h"
#include <vector>

#include "IPersistable.h"

// getting dirty (changing access-modifiers in this macro, beware!), the herd style.
#define LABELBACKGROUNDPROPERTY(t,n) private: ##t m_##n; \
public:	void Set##n (##t const & v) { m_##n = v; }; \
		##t Get##n () {return m_##n;}

class LabelBackground : public IPersistable
{
private:
	//LABELBACKGROUNDPROPERTY(std::wstring,Name);

	private:

		static std::vector<LabelBackground*> LabelBackground::LabelBackgrounds;
		
		std::wstring name;
		//char name[MAX_PATH];
		Gdiplus::Bitmap* left;
		Gdiplus::Bitmap* mid;
		Gdiplus::Bitmap* right;

		Gdiplus::Bitmap* td_top;
		Gdiplus::Bitmap* td_mid;
		Gdiplus::Bitmap* td_bottom;

		Gdiplus::Bitmap* sel_left;
		Gdiplus::Bitmap* sel_mid;
		Gdiplus::Bitmap* sel_right;

		Gdiplus::Bitmap* sel_td_top;
		Gdiplus::Bitmap* sel_td_mid;
		Gdiplus::Bitmap* sel_td_bottom;

		int leftWidth;
		int leftHeight;
		int middleWidth;
		int middleHeight;
		int rightWidth;
		int rightHeight;

		int marginTop;
		int marginLeft;
		int marginRight;
		int marginBottom;

		int defaultWidth;
		int defaultHeight;

		int subLabelMargin;

		int sel_leftWidth;
		int sel_leftHeight;
		int sel_middleWidth;
		int sel_middleHeight;
		int sel_rightWidth;
		int sel_rightHeight;

		int sel_marginTop;
		int sel_marginLeft;
		int sel_marginRight;
		int sel_marginBottom;

		int sel_defaultWidth;
		int sel_defaultHeight;
		int sel_subLabelMargin;

		bool supportsTopDown;
		bool supportsSelectionState;

		bool hasNormalLabel;

		bool sel_noShadowForNormalLabel;
		bool noShadowForNormalLabel;

		int ref_count;

		LabelBackground(std::wstring dirname);
		~LabelBackground();


		Bitmap* GetLeft(bool sel = false) const;
		int GetLeftHeight(bool sel = false) const;
		int GetLeftWidth(bool sel = false) const;

		Bitmap* GetMid(bool sel = false) const;
		int GetMiddleHeight(bool sel = false)const;
		int GetMiddleWidth(bool sel = false) const;


		Bitmap* GetRight(bool sel = false) const;
		int GetRightHeight(bool sel = false)const;
		int GetRightWidth(bool sel = false) const;


	public:

		static LabelBackground* LoadLabelBackground(std::wstring name);
		static void ReleaseAll();
		
		void Release();
		void AddRef();

		std::wstring  GetName(){return name;};

		bool SupportsSelectState()const;
		bool HasNormalLabel()const;
		bool GetNoShadowForNormalLabel(bool sel = false)const;

		void TransformRect(Gdiplus::Rect& rect, bool sel = false)const;
		void TransformRectTopDown(Gdiplus::Rect& rect, bool sel = false) const;

		void Draw(Gdiplus::Graphics* graphics,Gdiplus::Rect& rc, bool sel = false) const;
		void DrawTopDown(Gdiplus::Graphics* graphics,Gdiplus::Rect& rc, bool sel = false) const;

		int GetMarginBottom(bool sel= false)const;
		int GetMarginTop(bool sel = false)const;
		int GetMarginLeft(bool sel = false)const;
		int GetMarginRight(bool sel = false)const;

		int GetSubLabelMargin(bool sel = false) const;

		void LabelBackground::GetThumbnail(Graphics& g,const SIZE& s, bool sel = false);

	public:
		
		DECLARE_PERSISTANCYNAME(LabelBackground);

		BEGIN_LOADING_MAP()
			LOADABLE_PRIMITIVE(name);
		END_LOADING_MAP()

		BEGIN_SAVING_MAP()
			SAVABLE_PRIMITIVE(name);
		END_SAVING_MAP()
};

#endif//LABELBACKGROUND_H___122