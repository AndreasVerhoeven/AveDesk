// filename:		docklet.h
// Description:		Definition of class Docklet
// Author:			Andreas Verhoeven
// Last modified:	19 January 2004, 20:00
// Version:			1
// Comments:
//					<none>
// Notes:
//					This class is a wrapper class for the OdDockletInstance class
//					and handles additional settings only used by the GUI.
//					In some way, this class is a decorator.
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#ifndef DOCKLET_H__590683292
#define DOCKLET_H__590683292

#include "avedesk.h"
#include "constants.h"
#include "helper_funcs.h"
#include "od_docklet_instance.hpp"
#include "Label.h"
#include "dockletdll.h"
#include <oleidl.h>
#include <prsht.h>
#include "desklet.h"
#include "FlipAnimation.h"
#include "SuckyAnimation.h"
#include "AveToolTip.h"

#include "IPersistable.h"

#include "LayeredWindowImpl.hpp"


#include "Application.h"
#include "ShowCase1.h"

#include "DropTargetImpl.hpp"
#include "DropSourceImpl.hpp"
#include "DataObjectImpl.hpp"

#include "IObservable.h"

extern CComObject<CApplication> *_App;

#define WM_AVEACTION WM_USER + 3495
#define WM_AVESAVEACTION WM_USER + 3496
#define AVEACTION_FADEIN	2
#define AVEACTION_FADEOUT	4
#define AVEACTION_REMOVE	8
//#define AVEACTION_SAVE	16
#define AVEACTION_CONFIGDLG	32
#define AVEACTION_CONFIGURE	64
#define AVEACTION_REMOVEANIMATED 128
#define AVEACTION_GETSAFEOBJECT 256
#define AVEACTION_GETSAFEELEM 512
#define AVEACTION_REDRAW	1024


class CCloseButton;

struct PROPSHEETPARAMS
{
	HWND owner;
	PropertyDialogStruct* pds;
	bool noAddPropSheets;
};

#pragma pack(push, 1)

struct FlipParams;
typedef void (CALLBACK* DeskletFlipCallback)(void*, const FlipParams*);

struct FlipParams
{
	BOOL   useLayers;			// indicates if layers are used. Not using layers, will use
								// the bitmaps - all layers are destroyed when flipping
								// with bitmaps. Flipping with bitmaps is provided
								// for convenience with 1-layer desklets.

	SIZE*  oldDeskletSize;		// optional
	UINT*  oldLayersVisible;	// optional
	DWORD  oldLayersVisibleCount;// mandatory if oldLayersVisible != NULL

	SIZE*  newDeskletSize;		// optional
	UINT*  newLayersVisible;	// if useLayers, then mandatory
	DWORD  newLayersVisibleCount;// mandatory if newLayersVisibile != NULL

	Bitmap* oldBitmap;			// optional
	Bitmap* newBitmap;			// if !useLayers, then mandatory

	BOOL    flipToBack;			// if true, then it flips to the back, otherwise to the front

	DeskletFlipCallback callback; // can be used to be notified when the flip is done
	void*       callbackData; // can be used for the callback as the first parameter
};
#pragma pack(pop)


class DESKLETFLIPDATA
{
public:
	DESKLETFLIPDATA()
	{
		ZeroMemory(&params, sizeof(params));
		rotation = 0.0f;
		rotationSpeed = 1.0f;
		isFlipping = FALSE;
		timerId = 0;
		timerInterval = 20;
		animation = NULL;
		newBitmap = NULL;
		oldBitmap = NULL;
		xCentre = 0;
		yCentre = 0;
		newDeskletSize.cx = 0;
		newDeskletSize.cy = 0;
		oldDeskletSize.cx = 0;
		oldDeskletSize.cy = 0;
	}
	

	FlipParams params;
	float      rotation;
	float      rotationSpeed;
	BOOL       isFlipping;
	UINT       timerId;
	UINT       timerInterval;
	CFlipAnimation* animation;
	Bitmap* newBitmap;
	Bitmap* oldBitmap;
	std::vector<UINT> oldLayersVisible;
	std::vector<UINT> newLayersVisible;
	int xCentre;
	int yCentre;

	SIZE newDeskletSize;
	SIZE oldDeskletSize;
};

class DESKLETSUCKDATA
{
public:
	DESKLETSUCKDATA()
	{
		timerId       = 0;
		timerInterval = 20;
		animation     = NULL;
		bitmap        = NULL;
		close         = true;
	}

	UINT timerId;
	UINT timerInterval;
	CSuckyAnimation* animation;
	Bitmap* bitmap;
	bool close;
};

class DeskletSkinEnumerator;

class Docklet : 
  public IDropTargetImpl<Docklet>,
  public IDropSourceImpl<Docklet>,
  public IDataObjectImpl<Docklet>,
  public CLayeredWindowImpl<Docklet>,
  public CoElement<Docklet, CDesklet, IDesklet>,
  public IPersistable,
  public IObservable
{
	public:

	/*BEGIN_COM_MAP(Docklet)
		COM_INTERFACE_ENTRY(IDropTarget)
		COM_INTERFACE_ENTRY(IDropSource)
		COM_INTERFACE_ENTRY(IDataObject)
	END_COM_MAP()*/
	
	// IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

	// IDropTargetImpl overrides
    virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *, DWORD, POINTL, DWORD *);
    virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD, POINTL, DWORD *);
    virtual HRESULT STDMETHODCALLTYPE DragLeave(void);
    virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *, DWORD, POINTL, DWORD *);

	// IDataObjectImpl
	HRESULT DataRequest(FORMATETC *pFormatetc, STGMEDIUM *pmedium);

public:

	typedef std::map<HWND,Docklet*> DockletsMap;
	static DockletsMap Docklets;

	static Docklet* defaultDocklet;

	// default values
	static char		defaultImage[MAX_PATH];

	DeskletSkinInfo m_SkinInfo;
private:

	static CObservable* s_GlobalDockletsEventNotifier;

	long m_nRefCnt;							// trivial

	bool m_isDefaultValues;

	docklets::OdDockletInstance* docklet;	// pointer to OdDockletInstance. See classes.txt for more
	
	Label label;							// Label-data.
	UINT labelType;							// Will be replaced by better Label class
	bool	labelOnlyOnMouseOver;			// idem
	HWND	labelHwnd;						// idem
	POINT	labelPos;						// idem

	UINT style;							
	std::wstring styleName;

	bool moving;							// flag stating if this desklet is being moved
	bool mouseIsOver;						// flag stating if the mouse is over this desklet
	char image[MAX_PATH];					// current bg-image of this desklet					

	bool ignoreLeftClicks;				// some trivial properties
	bool ignoreRightClicks;				// ..
	bool isClickThru;					// ..

	bool isVisible;						// ..
	bool snapToGrid;					// ..

	bool lockPosition;	

	DockletDLL* dll;

	char effectPath[MAX_PATH];
	std::vector<HPROPSHEETPAGE> propertyPages;

	bool isShowCased;

	bool isInDropMode;

	BYTE oldAlpha;

	void SetupWindow();
	void SetupLabel();

	bool isSelected;

	CAveToolTip toolTip;

	static DWORD __stdcall ThisThreadFunc(LPVOID lpData);
	DWORD threadId;
	HANDLE threadHandle;
	HANDLE isInitializedEvent;

	bool isInDestroyedState;
	bool noRedraw;
	bool noFadeIn;

	HANDLE faderThread;
	bool stopFading;
	bool isFadingIn;
	bool isCrashed;
	bool isDestroying;
	
	HCURSOR normalCursor;
	HCURSOR hoverCursor;
	HCURSOR oldCursor;

	DWORD fadeMs;
	
	bool m_ImageCannotBeSet;

	UINT m_DeskletClickingMode;

	DESKLETFLIPDATA m_FlippingData;
	DESKLETSUCKDATA m_SuckingData;

	POINT m_ClosePoint;
	SIZE  m_ClosePointScale;
	bool  m_UseClosePoint;

	CCloseButton* m_CloseButton;

	POINT m_StartMovingCursorPos;
	POINT m_StartMovingDeskletPos;

	MSG m_Msg;

public:
	static DWORD __stdcall FaderThread(LPVOID lpData);
	void AsyncFade(bool fadingIn, DWORD durationInMs);

	Docklet(DockletDLL* dockletDLL, bool isDefaultValues=false);
	~Docklet();

	HCURSOR ChangeCursor(HCURSOR hCursor);

	HWND GetHwndInsertAfterFromStyle();

	Label& GetLabel(){return label;};
	
	bool& GetLockPosition(){return lockPosition;};
	bool& GetIsVisible(){return isVisible;};
	bool& GetSnapToGrid(){return snapToGrid;};

	bool& GetMouseIsOver(){return mouseIsOver;};
	bool& GetIgnoreLeftClicks(){return ignoreLeftClicks;};

	UINT GetStyle(){return style;};

	UINT& GetLabelType(){return labelType;};
	bool& GetLabelOnlyOnMouseOver(){return labelOnlyOnMouseOver;};
	HWND& GetLabelHwnd(){return labelHwnd;};
	POINT& GetLabelPos(){return labelPos;};

	void SetClickThru(bool val);
	bool GetClickThru();
	void SetIgnoreRightClicks(bool val);
	bool GetIgnoreRightClicks();

	void SetDeskletClickingMode(UINT m) { m_DeskletClickingMode = m;};
	UINT GetDeskletClickingMode()const{ return m_DeskletClickingMode;};

	void SaveToIni(const char* groupName, const char* iniFile);
	void ReadFromIni(HWND hwnd, const char* iniFile, const char* iniGroup);
	void ApplyDefaults();
	void ApplyStyle(UINT s, bool doNotChangeZOrder=false);
	docklets::OdDockletInstance* GetDocklet();

	//static Docklet* LoadDocklet(const char* filename, const char* iniFile, const char* iniGroup, bool makeVisible, WNDPROC lblProc);
	static Docklet* LoadDocklet(const WCHAR* filename, ILoader* loader, DeskletSkinEnumerator* ed, SkinInfoWrapper* skinInfo, bool  dontMakeVisible);
	static Docklet* LoadDocklet(const WCHAR* filename, ILoader* loader, DeskletSkinEnumerator* ed, SkinInfoWrapper* skinInfo, bool  dontMakeVisible, POINT offsetPos);

	static void ReleaseAll();
	static void RepairDesktopWindows();
	static void FadeAllOut();
	static void FadeAllLoadedIn();
	static Docklet* FindDocklet(HWND hwnd);

	static UINT clipFormat;

	const char* GetIconFileName();
	void  SetIconFileName(const char* filename);

	bool GetIsDefaultValues()const{return m_isDefaultValues;};

	//void UpdateLabelWindow(const char* txt);
	void UpdateLabelWindow();
	void UpdateLabelPosition();

	int AsyncDeskletPropertySheet(HWND owner, PropertyDialogStruct* pds=NULL, bool noAddPropSheet=false, BOOL dontWait=FALSE);
	int DeskletPropertySheet(HWND owner, PropertyDialogStruct* pds=0, bool noAddPropSheet=false);
	int AddPropertyPage(HPROPSHEETPAGE hpg);

	void SetVisibility(bool v);
	bool GetIsShowCased()const{return isShowCased;};
	void SetIsShowCased(bool b){isShowCased=b;};

	//!herd
	HWND GetHWnd(){return GetDocklet()->GetWindow();}
	DockletDLL *GetDLL(){ return dll; }

	void SetAppearanceTabHWnd(HWND hwnd) { appearanceTabHwnd = hwnd;};
	HWND GetAppearanceTabHWnd()const{ return appearanceTabHwnd;};

	bool GetIsInitialized()const { return isInitialized;};


	void Redraw(bool force=false);

	void SetAlpha(BYTE bAlpha);

	void StartMoving();
	void OnMovingMouse();
	void EndMoving();

	// some extra meta data, too lazy to write encapsule them.
	bool isInitialized;
	bool configDlgIsVisible;
	HWND hiddenParentHwnd;
	HANDLE h;
	HWND propHwnd;
	HWND appearanceTabHwnd;

	BOOL WaitForDockletBeingInitialized(DWORD dwMilliseconds);

	DECLARE_PERSISTANCYNAME(Desklet);

	BEGIN_LOADING_MAP()
		LOADABLE_CLASS(label);
		LOADABLE_PRIMITIVE(labelType);
		LOADABLE_PRIMITIVE(labelOnlyOnMouseOver);
		LOADABLE_PRIMITIVE(ignoreLeftClicks);
		LOADABLE_PRIMITIVE(ignoreRightClicks);
		LOADABLE_PRIMITIVE(isClickThru);
		//LOADABLE_PRIMITIVE(isVisible);
		LOADABLE_PRIMITIVE(snapToGrid);
		LOADABLE_PRIMITIVE(lockPosition);
		LOADABLE_PRIMITIVE(style);
		LOADABLE_PRIMITIVE(styleName);
		LOADABLE_PRIMITIVE(m_DeskletClickingMode);
		LOADABLE_CLASS_PTR_UNNAMED(docklet);
	END_LOADING_MAP()

	HRESULT SaveAsync(ISaver &saver);

	BEGIN_SAVING_MAP()
		SAVABLE_CLASS(label);
		SAVABLE_PRIMITIVE(labelType);
		SAVABLE_PRIMITIVE(labelOnlyOnMouseOver);
		SAVABLE_PRIMITIVE(ignoreLeftClicks);
		SAVABLE_PRIMITIVE(ignoreRightClicks);
		SAVABLE_PRIMITIVE(isClickThru);
		//SAVABLE_PRIMITIVE(isVisible);
		SAVABLE_PRIMITIVE(snapToGrid);
		SAVABLE_PRIMITIVE(lockPosition);
		SAVABLE_PRIMITIVE(style);
		SAVABLE_PRIMITIVE(styleName);
		SAVABLE_PRIMITIVE(m_DeskletClickingMode);
		SAVABLE_CLASS_PTR_UNNAMED(docklet);
	END_SAVING_MAP()

	BEGIN_MSG_MAP(Docklet)
		MESSAGE_HANDLER(WM_AVEACTION, OnAveAction)
		MESSAGE_HANDLER(WM_AVESAVEACTION, OnAveSaveAction)

		if((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) ||  uMsg == WM_NCMOUSEMOVE) 
		{
			toolTip.RelayEvent(m_hWnd, uMsg, wParam, lParam);
		}

		if(uMsg == WM_NOTIFY)
		{
			toolTip.RelayNotify(m_hWnd, wParam, lParam);
		}

		if(uMsg != WM_AVEACTION && uMsg != WM_AVESAVEACTION && docklet && isInitialized)
		{
			BOOL isHandled = FALSE;
			LRESULT res = docklet->Message(uMsg,wParam,lParam,isHandled);
			if(isHandled)
				return res;
		}

		// message handlers
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLeftButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRightButtonUp)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)

		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLeftButtonDoubleClick)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING,OnWindowsPosChanging )
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)

		//MESSAGE_HANDLER(WM_NCDESTROY,OnDestroy )
		// other stuff

		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CLayeredWindowImpl<Docklet>)
	END_MSG_MAP()

	//LRESULT OnClose(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// Prototypen von Behandlungsroutinen:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	// message handlers
	LRESULT OnClose(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRightButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void virtual OnFinalMessage(HWND);


	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonDoubleClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWindowsPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAveAction(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAveSaveAction(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// support functions
	HRESULT StartDragDrop(POINT pt);
	HMENU GetDefaultMenu(int startId);
	void DoDefaultMenuCommand(int startId,int cmd);

	void FadeIn(DWORD ms);
	void FadeOut(DWORD ms);

	void Remove();
	void RemoveWithAnimation();

	void SetNoRedraw(bool val);

	HANDLE GetIsInitializedEventHandle();

	static CRITICAL_SECTION cs;

	static size_t Size();

	static void CALLBACK SelectionTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);

	HRESULT SaveToSingleFile(const WCHAR* fileName);
	static HRESULT LoadFromXMLNode(CComPtr<IXMLDOMNode>& node,Docklet** d, BOOL makeVisible);
	static HRESULT LoadFromSingleFile(const WCHAR* fileName, Docklet** d, BOOL makeVisible);
	std::wstring GetRelativeDLLFile();

	public:
		enum OM_Event {
						OM_DeskletAdded,
						OM_AboutToBeRemoved,
						OM_CaptionChanged,
						OM_VisibilityChanged,
						OM_PositionChanged,
						OM_SizeChanged,
						OM_AlphaChanged,
						OM_RotationChanged,
						OM_AllDeskletsAboutToBeReleased,
						OM_SkinChanged,
						OM_Repaint,
						OM_ImageCannotBeSetChanged
					};

		int SetMainLabel(const std::wstring& val);

	private:
		void NotifyEvent(OM_Event event);
		static void NotifyGlobalDockletsEvent(OM_Event event, LPARAM val2);

	public:
		static IObservable* GetGlobalDockletsEventNotifier();

		HANDLE GetThreadHandle(){ return threadHandle;};

		void SetSkinInfo(SkinInfo* wrapper);
		DeskletSkinInfo& GetSkinInfo();

		bool Configure();

		void SetImageCannotBeSet(bool value);
		bool GetImageCannotBeSet()const;

		UINT GetRealClickingMode()const;

		BOOL StartFlipping(const FlipParams* params);
		void EndFlipping();
		void DoNextFlip();

		BOOL StartSucking(bool close);
		void EndSucking();
		void DoNextSuckStep();

		void SetClosePoint(const POINT& p, const int xScale, const int yScale);
		SIZE GetClosePointScale()const;
		POINT GetClosePoint()const;

		void SetUseClosePoint(bool val);
		bool GetUseClosePoint()const;

		DWORD GetThreadID()const{return threadId;};

		bool BeforeRemoval();

		bool SupportsFeature(DWORD flag);

};

#endif//DOCKLET_H__590683292