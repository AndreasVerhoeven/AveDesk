// DeskletAppearanceSheet.h: interface for the CDeskletAppearanceSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKLETAPPEARANCESHEET_H__349F806F_81C7_4A44_892F_698DDAA709A7__INCLUDED_)
#define AFX_DESKLETAPPEARANCESHEET_H__349F806F_81C7_4A44_892F_698DDAA709A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include <map>

#include "resource.h"

class Docklet;

#include "IObserver.h"
#include "ToolTipImpl.h"


// a data-storage class for a Found effect file used by the AppearanceSheet.
class AFoundEffect
{
public:
	std::wstring filename;
	std::wstring name;
	std::wstring author;
	std::wstring comments;
	int version;
};


#define WM_UPDATEIMAGEPREVIEW WM_USER + 39038

class CDeskletAppearanceSheet : 
	//public CComObjectRootEx<CComSingleThreadModel>,
	public CPropertyPageImpl<CDeskletAppearanceSheet>,
	public IObserver
	//public IDropTargetImpl<CDeskletAppearanceSheet>
{
private:
	Docklet* docklet;
	bool initMode;

	CUpDownCtrl	m_spinX;
	CUpDownCtrl	m_spinY;
	CUpDownCtrl	m_spinWidth;
	CUpDownCtrl	m_spinHeight;
	CUpDownCtrl	m_spinAlpha;
	CUpDownCtrl	m_spinRotation;

	CTrackBarCtrl m_sliderAlpha;
	CTrackBarCtrl m_sliderRotation;

	CComboBox	m_comboStyle;
	CComboBox	m_comboEffect;

	std::vector<AFoundEffect> m_FoundEffects;

	std::map<std::wstring, UINT> styles;

	void FillEffectsBox();
	void ScanDirectoryForEffects(std::wstring dir);

	void InitSpinner(CUpDownCtrl& control,int id,int lower, int upper, int val);
	void InitPositionAndSize();
	void InitAppearance();
	void InitStyle();
	void InitEffect();

	void SelectEffect();

	void SetImage();

public:
    enum { IDD = IDD_APPEARANCE };

	CDeskletAppearanceSheet(Docklet* d,_U_STRINGorID title=IDS_TAB0);
    ~CDeskletAppearanceSheet();

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam);
	void OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnSetImageButton(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnRemoveImageButton(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnDeleteItemButton(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnConfigureEffectButton(UINT wNotifyCode, int wID, HWND hwndCtrl);
	void OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar); 

	BOOL OnDestroy ();


	HRESULT Drop(IDataObject *obj,DWORD dwKeys, POINTL ptl, DWORD *pEffect);

	BEGIN_TOOLTIP_MAP()
		DEF_TOOLTIP(IDC_CAPTION, IDS_TOOLTIP_APPEARANCE_CAPTION)
		DEF_TOOLTIP(IDC_XPOS, IDS_TOOLTIP_APPEARANCE_XPOS)
		DEF_TOOLTIP(IDC_YPOS, IDS_TOOLTIP_APPEARANCE_YPOS)
		DEF_TOOLTIP(IDC_WIDTH, IDS_TOOLTIP_APPEARANCE_WIDTH)
		DEF_TOOLTIP(IDC_HEIGHT, IDS_TOOLTIP_APPEARANCE_HEIGHT)
		DEF_TOOLTIP(IDC_SLIDERALPHA, IDS_TOOLTIP_APPEARANCE_ALPHA)
		DEF_TOOLTIP(IDC_ALPHA, IDS_TOOLTIP_APPEARANCE_ALPHA)
		DEF_TOOLTIP(IDC_SLIDERROTATION, IDS_TOOLTIP_APPEARANCE_ROTATION)
		DEF_TOOLTIP(IDC_ROTATION, IDS_TOOLTIP_APPEARANCE_ROTATION)
		DEF_TOOLTIP(IDC_STYLE, IDS_TOOLTIP_APPEARANCE_STYLE)
		DEF_TOOLTIP(IDC_EFFECT, IDS_TOOLTIP_APPEARANCE_EFFECT)
		DEF_TOOLTIP(IDCONFIGEFFECT, IDS_TOOLTIP_APPEARANCE_CONFIGEFFECT)
		DEF_TOOLTIP(IDSETIMAGE, IDS_TOOLTIP_APPEARANCE_SETIMAGE)
		DEF_TOOLTIP(IDREMOVEIMAGE, IDS_TOOLTIP_APPEARANCE_REMOVEIMAGE)
		DEF_TOOLTIP(IDDELETEITEM, IDS_TOOLTIP_APPEARANCE_REMOVEDESKLET)
		DEF_TOOLTIP(IDC_IMAGE, IDS_TOOLTIP_APPEARANCE_IMAGE)
		DEF_TOOLTIP(IDC_LOCKPOSITION, IDS_TOOLTIP_APPEARANCE_LOCKPOSITION)
	END_TOOLTIP_MAP()


	BEGIN_MSG_MAP_EX(CDeskletAdvancedSheet)
		COMMAND_ID_HANDLER_EX(IDSETIMAGE,OnSetImageButton)
		COMMAND_ID_HANDLER_EX(IDREMOVEIMAGE,OnRemoveImageButton)
		COMMAND_ID_HANDLER_EX(IDDELETEITEM,OnDeleteItemButton)
		COMMAND_ID_HANDLER_EX(IDCONFIGEFFECT,OnConfigureEffectButton)

		COMMAND_ID_HANDLER_EX(IDC_CAPTION,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_XPOS,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_YPOS,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_WIDTH,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_HEIGHT,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ALPHA,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_ROTATION,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_STYLE,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_EFFECT,OnEditValue)
		COMMAND_ID_HANDLER_EX(IDC_LOCKPOSITION,OnEditValue)

		

		MSG_WM_HSCROLL(OnHScroll)
        MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)

		MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles)
		MESSAGE_HANDLER(WM_UPDATEIMAGEPREVIEW,OnUpdateImagePreview)
        CHAIN_MSG_MAP(CPropertyPageImpl<CDeskletAppearanceSheet>)
		REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdateImagePreview(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    int OnApply();

	virtual void Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2);

};

#endif // !defined(AFX_DESKLETAPPEARANCESHEET_H__349F806F_81C7_4A44_892F_698DDAA709A7__INCLUDED_)

