//	Name:		Od Docklet Instance class
//	Author:		Andreas Verhoeven
//	Date:		dec 16 2003, 18:45
//	Version:	1
//	Revision:	2
//	Comments:	<none>

#ifndef INCLUDE_GUARD_203919929191929119191_OD_DOCKLET_INSTANCE_HPP
#define INCLUDE_GUARD_203919929191929119191_OD_DOCKLET_INSTANCE_HPP

// include files
#include <windows.h>		// for common window stuff
#include "od_docklet.hpp"	// class definitin of OdDocklet

#include "constants.h"

#include "Layer.h"
#include <vector>
#include <memory>
#include <map>

#include <gdiplus.h>
using namespace Gdiplus;				
#pragma comment(lib, "gdiplus.lib")

#include "DrawpluginInstance.h"

#include "Ipersistable.h"

#include "ColorMatrixHelper.h"

#include "UpdateBounds.h"

class DeskletSkinEnumerator;

extern Bitmap* __stdcall DeskletLoadGDIPlusImageW(const WCHAR *szImage);

namespace docklets
{
	class OdDockletInstance : public IPersistable
	{
		private:

			HWND hWnd;				// window for the docklet
									// if this is 0, then the docklet is not active

			DOCKLET_DATA* pointer;	// pointer needed to send with the events

			std::vector<Layer*>layers;	// all layers for this docklet
			std::map<UINT, Layer*> subLayers; // all sublayers, for faster lookup

			std::map<Layer*, std::wstring> tooltipTexts;
	
			OdDocklet*	docklet;	// docklets events

			BYTE Alpha;				// the overal alpha used for the docklet

			int xPos;				// trivial
			int yPos;				// trivial

			int width;				// trivial
			int height;				// trivial

			int rotation;			// rotation angle

			// OBSOLETE
			bool useSaturation;		// should we use saturation, false->faster drawing

			bool useColorization;	// should we use colorization
			REAL saturation;		// saturation value
			INT  hue;				// hue value


			int quality;			// the quality of the icons drawn

			bool drawShadow;		// should we draw a shadow, false->faster drawing
			int  shadowX;			// x-offset for the shadow
			int  shadowY;			// y-offset for the shadow
			REAL shadowGray;		// the gray tint used for the shadow
			INT shadowAlpha;		// the alpha component for the shadow
			LONG shadowColor;		// the color for the shadow

			ColorMatrix colorMatrix;

			bool isSelected;
			bool isMouseOver;

			DrawPluginInstance* drawPlugin;

			std::wstring effectFile;

			bool isDirty;

			boost::shared_ptr<Bitmap> m_CachedBitmap;

			CRITICAL_SECTION m_CriticalSection;

			UINT m_PreviousMouseOverLayer;
			UINT m_PreviousMouseDownLayer;
			UINT m_CaptureLayer;

			CUpdateBounds m_UpdateRegion;
			INT updateRegionLockCount;

		public:

			std::wstring GetToolTipTextForLayer(const POINT& p, const SIZE& s);
			std::wstring GetToolTipTextForLayer(UINT id);
			void SetToolTipText(Layer*l, const std::wstring& ws);

			INT LockUpdateRegion();
			INT UnlockUpdateRegion();
			BOOL IsUpdateRegionLocked()const;

			UINT IdFromLayer(Layer* l);
			Layer* LayerFromId(UINT id);

			UINT HitTestLayers(const POINT& p, const SIZE & s, bool mustBeVisible, Layer** pHittedLayer = NULL);

			bool GetIsDirty()const { return isDirty;};
			void SetIsDirty(const bool b){ isDirty = b;};

			bool GetIsSelected() const { return isSelected;};

			std::wstring GetIconFileName();

			void SetIconFileName(std::wstring filename);

			void CopyValues(OdDockletInstance* that);

			void SetDLL(OdDocklet* dll)
			{
				docklet = dll;
			}

			bool SupportsFeature(DWORD flag);

			void AddUpdateRect(const RECT& r, const SIZE& scale);
			void InvalidateAll();

			POINT GetCorrectedPos()const;
			SIZE GetCorrectedSize()const;

			Bitmap* Draw();
			Bitmap* DrawLayers(const std::vector<UINT>& layersVisible, const SIZE& s);
			void ResizeCachedBitmap();

			DECLARE_PERSISTANCYNAME(DeskletImpl);

			virtual HRESULT Load(ILoader& loader);
			virtual HRESULT Save(ISaver& saver);

			void Activate(ILoader* loader,HWND hwnd, HINSTANCE hInstance=NULL,DeskletSkinEnumerator* ed = NULL, const SkinInfoWrapper* skinInfo=NULL);

			// constructors
			OdDockletInstance(OdDocklet* docklet,DOCKLET_DATA* p, HWND hwnd);
			~OdDockletInstance();

			// pointer setter
			void SetPointer(DOCKLET_DATA *p);
			DOCKLET_DATA* GetPointer() {return pointer;};

			// window getter
			HWND GetWindow();

			// image setters
			void SetNormalImage(Image* img, bool clone = true);
			void SetOverlayImage(Image* img, bool clone = true);

			// redraw functions
			//void Redraw(bool force=false);
			void Draw(Graphics& g, POINT& p, SIZE& s);

			// ave: 06/6: function to draw a d-'n-d image.
			void DrawDragDropImage(Graphics& g, const SIZE& s);
			void DrawInSimpleForm(Graphics& g, const SIZE& s, const POINT& p);
			void DrawCachedBitmap(Graphics& g, const SIZE& s, const POINT& p);

			// message functions
			bool ProcessMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
			bool Create(const char* iniFile, const char* iniGroup);
			bool Save(const char* iniFile,const char* iniGroup, bool isForExport);
			bool LeftButtonClick(POINT* ptCursor,SIZE* sizeDocklet);
			bool DoubleClick(POINT* ptCursor,SIZE* sizeDocklet);
			bool LeftButtonHeld(POINT* ptCursor,SIZE* sizeDocklet);
			bool RightButtonClick(POINT* ptCursor,SIZE* sizeDocklet, DWORD keys);
			bool Configure();
			bool AcceptDropFiles();
			bool DropFiles(HDROP hDrop);
			void GetInformation(char *szName,char *szAuthor,int *lpiVersion,char *szNotes);
			bool ExportFiles(char* fileRelativeOut, int iteration);
			bool MouseOn();
			bool MouseOut();
			bool MouseMove(int x, int y);
			bool MouseWheelForward(WORD fKeysDown, int numOfScrolls, int x, int y);
			bool MouseWheelBackward(WORD fKeysDown, int numOfScrolls, int x, int y);

			bool DragEnter(IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect);
			bool DragOver(DWORD keys,POINTL pnt,DWORD* effect);
			bool DragLeave();
			bool Drop(IDataObject* obj,DWORD keys,POINTL pnt,DWORD* effect);
			bool DragDropDataRequest(FORMATETC* format, STGMEDIUM* medium);
			LRESULT ProcessMessageEx(UINT uMsg,WPARAM wParam,LPARAM lParam, BOOL& isHandled);
			bool VisibilityNotify(BOOL becomesVisible, DWORD flags=0);
			bool BeforeRemoval();
			bool GetFlags(DWORD& flags);

			bool Select();
			bool Deselect();


			bool SetSkinW(ENUMERATOR_DATA* ed, const SkinInfo* info);

			// this will destroy the docklet totally!
			void Destroy();

			// this wil activate a not activated docklet
			void Activate(const char* iniFile, const char* iniGroup,HWND hwnd, HINSTANCE hInstance);

			// returns if the docklet is activated
			bool IsActivated();

			// returns the docklets module
			HMODULE GetModule();

			// get-setter functions
			BYTE GetAlpha();
			void SetAlpha(BYTE value, bool redraw=false);

			void SetPos(int x, int y, bool redraw=false);
			void SetSize(int w, int h, bool redraw=false);

			SIZE GetSize();
			POINT GetPos();

			int GetRotation();
			void SetRotation(int r,bool redraw=false);

			void SetSaturation(bool use, REAL val, bool redraw=false);
			REAL GetSaturation();
			bool GetUseSaturation();

			int  GetHue();
			void SetColorization(bool use, REAL satVal, int hueVal, bool redraw = false);
			bool GetUseColorization();

			bool GetModel(IDispatch **pVal);


			// static functions
			static HWND CreateDockletWindow(WNDPROC wndProc, HINSTANCE hInstance, HWND parent = 0);

			// quality functions
			void ApplyQualityTo(Graphics* g);
			void SetQuality(int val, bool redraw=false);
			int  GetQuality();

			// shadow functions
			void SetShadow(bool useShadow, int xOffset, int yOffset, REAL gray, INT alpha, LONG color = 0, bool redraw=false);
			bool GetDrawShadow();
			int  GetShadowXOffset();
			int  GetShadowYOffset();
			REAL GetShadowGray();
			INT GetShadowAlpha();
			LONG GetShadowColor();

			Layer* GetLayer(unsigned int index);
			std::vector<Layer*>& GetLayers();
			std::map<UINT, Layer*>& GetSubLayers(){return subLayers;};

			DrawPluginInstance* GetDrawPlugin();
			void SetDrawPlugin(DrawPluginInstance* plugin);

			void SetSelected(bool val);
			void SetMouseOver(bool val);

			int AddPropertySheets(int numPresent);

			LRESULT Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

			void SetUncorrectedPos(int x, int y);
			void CorrectPoint(long& x, long& y);

			bool HasCustomOnConfigure();

			Layer* GetLayerAtPoint(const POINT& p, bool mustBeVisible, UINT* idVar = NULL);

			bool MouseDown(POINT* pt, SIZE* size, DWORD keys);
			bool MouseUp(POINT* pt, SIZE* size, DWORD keys);

			UINT GetCaptureLayer()const{return m_CaptureLayer;};
			void SetCaptureLayer(UINT lid){m_CaptureLayer = lid;};
	};
};

#endif//INCLUDE_GUARD_203919929191929119191_OD_DOCKLET_INSTANCE_HPP