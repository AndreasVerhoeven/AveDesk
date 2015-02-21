// DeskletAppearanceSheet.cpp: implementation of the CDeskletAppearanceSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeskletAppearanceSheet.h"

#include "helper_funcs.h"
#include "drawplugin.h"
#include "Od_Docklet_Instance.hpp"
#include "Docklet.h"
#include <math.h>
#include <memory>
#include "DeskletStyles.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeskletAppearanceSheet::CDeskletAppearanceSheet(Docklet* d,_U_STRINGorID title) :
	CPropertyPageImpl<CDeskletAppearanceSheet>(title),
	docklet(d),
	initMode(true)
{

}

CDeskletAppearanceSheet::~CDeskletAppearanceSheet()
{
	
}

extern void CALLBACK DeskletSetImageFileW(HWND hwndDocklet,const WCHAR *szImage);
LRESULT CDeskletAppearanceSheet::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDrop = reinterpret_cast<HDROP>(wParam);

	if(docklet->GetImageCannotBeSet() == false)
	{

		int count = DragQueryFile(hDrop,(UINT)-1,NULL,0);
		if(count > 0)
		{
			WCHAR fileName[MAX_PATH] = {0};
			DragQueryFile(hDrop,0,fileName,MAX_PATH);
			if(fileName[0] != L'\0')
			{
				DeskletSetImageFileW(docklet->GetHWnd(),fileName);
				SetImage();
				SetModified();
			}
		}
	}

	DragFinish(hDrop);

	return 0;
}

LRESULT CDeskletAppearanceSheet::OnUpdateImagePreview(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetImage();

	return 0;
}

void CDeskletAppearanceSheet::Notify(IObservable* source, bool paramsAreValid, LPARAM val1, LPARAM val2)
{
	if(paramsAreValid)
	{
		Docklet::OM_Event event = static_cast<Docklet::OM_Event>(val1);
		switch(event)
		{
			case Docklet::OM_AboutToBeRemoved:
			case Docklet::OM_AllDeskletsAboutToBeReleased:
				SendMessage(GetParent(),WM_CLOSE,0,0);
			break;

			case Docklet::OM_CaptionChanged:
				SetDlgItemTextW(IDC_CAPTION,docklet->GetLabel().GetMainCaption().c_str());
			break;

			case Docklet::OM_PositionChanged:
				m_spinX.SetPos(docklet->GetDocklet()->GetPos().x);
				m_spinY.SetPos(docklet->GetDocklet()->GetPos().y);
			break;

			case Docklet::OM_SizeChanged:
				m_spinWidth.SetPos(docklet->GetDocklet()->GetSize().cx);
				m_spinHeight.SetPos(docklet->GetDocklet()->GetSize().cy);
			break;

			case Docklet::OM_AlphaChanged:
				m_sliderAlpha.SetPos((int) (float(docklet->GetDocklet()->GetAlpha())/2.55f));
				m_spinAlpha.SetPos((int) (float(docklet->GetDocklet()->GetAlpha())/2.55f));
			break;

			case Docklet::OM_RotationChanged:
				m_sliderRotation.SetPos(docklet->GetDocklet()->GetRotation());
				m_spinRotation.SetPos(docklet->GetDocklet()->GetRotation());
			break;

			case Docklet::OM_ImageCannotBeSetChanged:
				::ShowWindow(GetDlgItem(IDSETIMAGE), val2 ? SW_HIDE : SW_SHOW);
				::ShowWindow(GetDlgItem(IDREMOVEIMAGE), val2 ? SW_HIDE : SW_SHOW);
				::PostMessage(m_hWnd,WM_UPDATEIMAGEPREVIEW,0,0);
			break;

			case Docklet::OM_Repaint:
				if(docklet->GetImageCannotBeSet())
					::PostMessage(m_hWnd, WM_UPDATEIMAGEPREVIEW,0,0);
			break;
		}

	}
}

BOOL CDeskletAppearanceSheet::OnDestroy ()
{
	docklet->SetAppearanceTabHWnd(NULL);
	if(!docklet->GetIsDefaultValues())
		docklet->Deregister(this);
	return TRUE;
}

void CDeskletAppearanceSheet::ScanDirectoryForEffects(std::wstring dir)
{
  WIN32_FIND_DATAW data;
	
	// find effects
  HANDLE h = FindFirstFileW(std::wstring(dir+L"*.effect").c_str(),&data);

	if(h!=INVALID_HANDLE_VALUE)
	{
		do
		{
			DrawPlugin* d = DrawPlugin::LoadPlugin(WStringToString(std::wstring(dir+data.cFileName) ).c_str() );
			
			if(d)
			{
				AFoundEffect f;
				f.filename = std::wstring(dir+data.cFileName);
				f.name = StringToWString(d->GetName());
				f.author = StringToWString(d->GetAuthor());
				f.comments = StringToWString(d->GetNotes());
				f.version = d->GetVersion();
				m_FoundEffects.push_back(f);
				d->Release();
			}
		}while(FindNextFileW(h,&data));
	}
	FindClose(h);

	
  // find effectlets
  h = FindFirstFileW(std::wstring(dir+L"*.effectlet").c_str(),&data);

	if(h!=INVALID_HANDLE_VALUE)
	{
		do
		{
			DrawPlugin* d = DrawPlugin::LoadPlugin(WStringToString( std::wstring(dir+data.cFileName) ).c_str() );
			
			if(d)
			{
				AFoundEffect f;
				f.filename = std::wstring(dir+data.cFileName);
				f.name = StringToWString(d->GetName());
				f.author = StringToWString(d->GetAuthor());
				f.comments = StringToWString(d->GetNotes());
				f.version = d->GetVersion();
				m_FoundEffects.push_back(f);
				d->Release();
			}
		}while(FindNextFileW(h,&data));
	}
	FindClose(h);


	h = FindFirstFileW(std::wstring(dir+L"*.*").c_str(),&data);

	if(h!=INVALID_HANDLE_VALUE)
	{
		do
		{	
			if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
			{
				if(wcscmp(data.cFileName,L".")!=0 && wcscmp(data.cFileName,L"..")!=0)
					ScanDirectoryForEffects(std::wstring((dir+data.cFileName)+L"\\").c_str());
			}
		}while(FindNextFileW(h,&data));
	}

	FindClose(h);

	return;
}

void CDeskletAppearanceSheet::FillEffectsBox()
{
	USES_CONVERSION;

	SetCursor(LoadCursor(0,IDC_WAIT));

	WCHAR appDir[MAX_PATH] = {0};
	wcscpy(appDir, _App->GetAveDeskDataDir().c_str());
	std::wstring dir = lowercaseW(appDir);
	
	ScanDirectoryForEffects(dir+L"Effects\\");
	for(std::vector<AFoundEffect>::iterator iter = m_FoundEffects.begin(); iter != m_FoundEffects.end(); ++iter)
	{
		AFoundEffect& fe = *iter;
		m_comboEffect.AddString(fe.name.c_str() );
	}

	SetCursor(LoadCursor(0,IDC_ARROW));
}

void inline CDeskletAppearanceSheet::InitSpinner(CUpDownCtrl& control,int id,int lower, int upper, int val)
{
	control = GetDlgItem(id);
	control.SetRange(lower,upper);
	control.SetPos(val);
}

void CDeskletAppearanceSheet::InitPositionAndSize()
{
	docklets::OdDockletInstance* inst = docklet->GetDocklet();

	InitSpinner(m_spinX,IDC_SPINX,-32000,+32000,inst->GetPos().x);
	InitSpinner(m_spinY,IDC_SPINY,-32000,+32000,inst->GetPos().y);
	InitSpinner(m_spinWidth,IDC_SPINWIDTH,0,+32000,inst->GetSize().cx);
	InitSpinner(m_spinHeight,IDC_SPINHEIGHT,0,+32000,inst->GetSize().cy);

	CheckDlgButton(IDC_LOCKPOSITION,docklet->GetLockPosition() ? TRUE : FALSE);
}

void CDeskletAppearanceSheet::InitStyle()
{
	m_comboStyle = GetDlgItem(IDC_STYLE);

	UINT curStyle = docklet->GetStyle();
	int selIndex = -1;

	CDeskletStyles* stylesColl = CDeskletStyles::GetInstance();
	if(stylesColl != NULL)
	{
		styles = stylesColl->GetStyles();
		for(std::map<std::wstring, UINT>::iterator iter = styles.begin(); iter != styles.end(); ++iter)
		{
			UINT index = m_comboStyle.AddString(iter->first.c_str());
			m_comboStyle.SetItemData(index, static_cast<DWORD_PTR>(iter->second));	
			if(iter->second == curStyle)
				selIndex = index;
		}
	}
	m_comboStyle.SetCurSel(selIndex);
	/*
	CString style;
	style.LoadString(IDS_STYLE0);
	m_comboStyle.AddString(style);
	style.LoadString(IDS_STYLE1);
	m_comboStyle.AddString(style);
	style.LoadString(IDS_STYLE2);
	m_comboStyle.AddString(style);
	style.LoadString(IDS_STYLE3);
	m_comboStyle.AddString(style);
	style.LoadString(IDS_STYLE4);
	m_comboStyle.AddString(style);
	m_comboStyle.SetCurSel( docklet->GetStyle() );
	*/
}

void CDeskletAppearanceSheet::InitEffect()
{
	USES_CONVERSION;

	docklets::OdDockletInstance* inst = docklet->GetDocklet();

	m_comboEffect = GetDlgItem(IDC_EFFECT);
	m_comboEffect.AddString(_T("") );
	FillEffectsBox();

	if(inst->GetDrawPlugin())
		m_comboEffect.SelectString(0, A2T( inst->GetDrawPlugin()->GetPlugin()->GetName() ) );
	else
		m_comboEffect.SelectString(0,  _T("") );
}

void CDeskletAppearanceSheet::InitAppearance()
{
	docklets::OdDockletInstance* inst = docklet->GetDocklet();
	
	InitSpinner(m_spinAlpha,IDC_SPINALPHA,0,100,(int) (float(inst->GetAlpha())/2.55f));
	InitSpinner(m_spinRotation,IDC_SPINROTATION,-180,180,inst->GetRotation());

	m_sliderAlpha = GetDlgItem(IDC_SLIDERALPHA);
	m_sliderAlpha.SetRange(0,100);
	m_sliderAlpha.SetPos((int) (float(inst->GetAlpha())/2.55f) );


	m_sliderRotation = GetDlgItem(IDC_SLIDERROTATION);
	m_sliderRotation.SetRange(-180,180);
	m_sliderRotation.SetTicFreq(15);
	m_sliderRotation.SetPos(inst->GetRotation() );


	InitStyle();
	InitEffect();

}

void CDeskletAppearanceSheet::OnHScroll(int nScrollCode, short int nPos, HWND hwndScrollBar)
{
	if(initMode)
		return;

	if(GetDlgItem(IDC_SLIDERALPHA)  == hwndScrollBar)
	{
		m_spinAlpha.SetPos( m_sliderAlpha.GetPos() );
		BYTE newAlpha = (BYTE)ceil(float( m_sliderAlpha.GetPos()) * 2.55f);
		docklet->GetDocklet()->SetAlpha(newAlpha);
		if(!docklet->GetIsDefaultValues() )
			docklet->SetAlpha(newAlpha);
		SetModified(true);
	}
	else if(GetDlgItem(IDC_SLIDERROTATION)  == hwndScrollBar)
	{
		m_spinRotation.SetPos( m_sliderRotation.GetPos() );
		docklet->GetDocklet()->SetRotation( m_sliderRotation.GetPos());
		if(!docklet->GetIsDefaultValues() )
			docklet->Redraw();
		SetModified(true);
	}
}

void CDeskletAppearanceSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(initMode)
		return;

	if(wNotifyCode == EN_CHANGE)
	{
			if(GetDlgItem(IDC_ALPHA) == hwndCtrl)
			{
				BYTE newAlpha = (BYTE)ceil(float( m_spinAlpha.GetPos()) * 2.55f);
				m_sliderAlpha.SetPos ( m_spinAlpha.GetPos() );
				docklet->GetDocklet()->SetAlpha(newAlpha);
				if(!docklet->GetIsDefaultValues() )
					docklet->SetAlpha(newAlpha);
			}
		else if(GetDlgItem(IDC_ROTATION) == hwndCtrl)
		{
			m_sliderRotation.SetPos( m_spinRotation.GetPos() );
			docklet->GetDocklet()->SetRotation( m_sliderRotation.GetPos());

			if(!docklet->GetIsDefaultValues() )
				docklet->Redraw();
		}
		else if(GetDlgItem(IDC_XPOS) == hwndCtrl || 
				GetDlgItem(IDC_YPOS) == hwndCtrl )
		{
			docklet->GetDocklet()->SetPos(m_spinX.GetPos(), m_spinY.GetPos(), false);	

			if(!docklet->GetIsDefaultValues())
			{
				RECT r = {0};
	
				::GetWindowRect(docklet->GetHWnd(),&r);
				POINT p2 = docklet->GetDocklet()->GetPos();
	
				if(docklet->GetDocklet()->GetDrawShadow())
				{
					p2.x -= Layer::ratio/10;
					p2.y -= Layer::ratio/10;
				}

				SIZE s = docklet->GetDocklet()->GetSize();
				POINT pr = {0,0};
				int rot = docklet->GetDocklet()->GetRotation();
				if(rot != 0)
				{
					FixRotation(rot,s,pr);
					p2.x -= pr.x;
					p2.y -= pr.y;
				}

				if(!docklet->GetIsDefaultValues() )
				{
					//SIZE s = SIZEFROMRECT(r);
					CWindowDC screenDC(NULL);
					UpdateLayeredWindow(docklet->GetHWnd(), screenDC, &p2, NULL, NULL, NULL, 0, NULL, 0);
					//::MoveWindow(docklet->GetHWnd(),p2.x,p2.y,SIZEFROMRECT(r).cx,SIZEFROMRECT(r).cy,FALSE);
					docklet->UpdateLabelPosition();
				}
			}

		}
		else if(GetDlgItem(IDC_WIDTH) == hwndCtrl||
				GetDlgItem(IDC_HEIGHT) == hwndCtrl )
		{
			docklet->GetDocklet()->SetSize(m_spinWidth.GetPos(), m_spinHeight.GetPos());
			if(!docklet->GetIsDefaultValues() )
				docklet->Redraw();
		}

		SetModified(true);
	}
	else if(wNotifyCode == BN_CLICKED && GetDlgItem(IDC_LOCKPOSITION) == hwndCtrl)
	{
		SetModified(true);
	}
	else if(wNotifyCode == CBN_SELCHANGE && (GetDlgItem(IDC_STYLE) == hwndCtrl || GetDlgItem(IDC_EFFECT) == hwndCtrl) )
	{
		SetModified(true);
	}
}

extern Bitmap* CALLBACK DeskletLoadGDIPlusImageW(const WCHAR*);
void CDeskletAppearanceSheet::SetImage()
{
	if(docklet->GetImageCannotBeSet() == false)
	{
	
		USES_CONVERSION;
	
		boost::shared_ptr<Bitmap> selBmp(DeskletLoadGDIPlusImageW(A2W(docklet->GetIconFileName())));
	
		CStatic img = GetDlgItem(IDC_IMAGE);
	
		RECT r = {0};
		img.GetClientRect(&r);
	
		SIZE s = {0};
		SIZEFROMRECT(s,r);
	
		Bitmap bmp(s.cx,s.cy);
		Graphics g(&bmp);
		ApplyQualityTo(QUALITY_BEST,&g);
		g.Clear(Color(GetSysColor(COLOR_BTNFACE)));
	
		if(selBmp.get())
		{
			g.DrawImage(selBmp.get(),5,5,s.cx-10,s.cy-10);
		}
	
		CBitmap newHBMP;
		CBitmap oldHBMP;
	
		bmp.GetHBITMAP(NULL,&newHBMP.m_hBitmap);
	
		oldHBMP = img.SetBitmap(newHBMP);
	}
	else
	{
		CStatic img = GetDlgItem(IDC_IMAGE);

		RECT r = {0};
		img.GetClientRect(&r);
	
		SIZE s = {0};
		SIZEFROMRECT(s,r);

		SIZE drawSize = {s.cx, s.cy};
		POINT drawPoint = {0, 0};
		SIZE curSize = docklet->GetDocklet()->GetSize();
		if(curSize.cx > curSize.cy && curSize.cy != 0)
		{
			float ratio = float(curSize.cx) / float(curSize.cy);
			drawSize.cy = int( float(drawSize.cy) / ratio );
			if(drawSize.cy % 2 == 1)
				drawSize.cy += 1;
			drawPoint.y = (s.cy - drawSize.cy) / 2;
		}
		else if(curSize.cy > curSize.cx && curSize.cx != 0)
		{
			float ratio = float(curSize.cy) / float(curSize.cx);
			drawSize.cx = int( float(drawSize.cx) / ratio );
			if(drawSize.cx % 2 == 1)
				drawSize.cx += 1;
			drawPoint.x = (s.cx - drawSize.cx) / 2;
		}

		Bitmap bmp(s.cx, s.cy);
		Graphics g(&bmp);

		if(GetIsRunningWin2k()  == TRUE)
			g.Clear(Color(::GetSysColor(COLOR_WINDOW)));

		if(docklet->GetDocklet()->GetIsDirty() && docklet->GetStyle() == STYLE_SHOWCASE)
			docklet->GetDocklet()->Draw();

		docklet->GetDocklet()->DrawCachedBitmap(g,drawSize,drawPoint);

		CBitmap newHBMP;
		CBitmap oldHBMP;
	
		bmp.GetHBITMAP(NULL,&newHBMP.m_hBitmap);
	
		oldHBMP = img.SetBitmap(newHBMP);

	}
}


BOOL CDeskletAppearanceSheet::OnInitDialog (HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	ToolTips_Init();

	SetDlgItemTextW(IDC_CAPTION,docklet->GetLabel().GetMainCaption().c_str() );

	DragAcceptFiles(TRUE);

	if(!docklet->GetIsDefaultValues())
	{
		SetImage();
	}
	else
	{
		::ShowWindow(GetDlgItem(IDC_IMAGE),SW_HIDE);
		::ShowWindow(GetDlgItem(IDSETIMAGE),SW_HIDE);
		::ShowWindow(GetDlgItem(IDREMOVEIMAGE),SW_HIDE);
		::ShowWindow(GetDlgItem(IDDELETEITEM),SW_HIDE);
		::ShowWindow(GetDlgItem(IDCONFIGEFFECT),SW_HIDE);
	}

	if(docklet->SupportsFeature(DESKLET_NO_CHANGE_IMAGE) || docklet->GetImageCannotBeSet())
	{
		::ShowWindow(GetDlgItem(IDSETIMAGE),SW_HIDE);
		::ShowWindow(GetDlgItem(IDREMOVEIMAGE),SW_HIDE);
		m_tt.DeleteTip(GetDlgItem(IDC_IMAGE));
	}

	if(docklet->SupportsFeature(DESKLET_NO_LABEL))
	{
		::ShowWindow(GetDlgItem(IDC_CAPTION_LABEL), SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_CAPTION), SW_HIDE);
	}

	if(docklet->SupportsFeature(DESKLET_NO_ROTATION))
	{
		::ShowWindow(GetDlgItem(IDC_SLIDERROTATION), SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_ROTATION_LABEL), SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_SPINROTATION), SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_ROTATION), SW_HIDE);
	}

	//RegisterDragDrop(GetDlgItem(IDC_IMAGE),dropTarget);

	docklet->SetAppearanceTabHWnd(m_hWnd);

	InitPositionAndSize();
	InitAppearance();

	if(!docklet->GetIsDefaultValues())
		docklet->Register(this);

	
	initMode = false;

	return 0;
}

int CDeskletAppearanceSheet::OnApply()
{
	SelectEffect();

	docklet->GetLockPosition() = IsDlgButtonChecked(IDC_LOCKPOSITION)==BST_CHECKED;
	if((int)docklet->GetStyle() != m_comboStyle.GetCurSel() )
	{
		UINT styleID = static_cast<UINT>(m_comboStyle.GetItemData(m_comboStyle.GetCurSel()));
		docklet->ApplyStyle(styleID);
	}

	WCHAR label[1024] = {0};
	GetDlgItemTextW(IDC_CAPTION,label,sizeof(label)/sizeof(label[0]));
	docklet->SetMainLabel(label);

	if(!docklet->GetIsDefaultValues())
	{
		docklet->UpdateLabelWindow();
		docklet->UpdateLabelPosition();
	}

	return TRUE;
}

extern BOOL CALLBACK DeskletBrowseForImageW(HWND hwndParent, WCHAR *szImage);

void CDeskletAppearanceSheet::OnSetImageButton(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	USES_CONVERSION;

	WCHAR tmp[MAX_PATH+1] = {0};
	wcscpy(tmp,A2W(docklet->GetIconFileName()));
	if(DeskletBrowseForImageW(GetParent(),tmp))
	{
		DeskletSetImageFileW(docklet->GetHWnd(),tmp);
		SetImage();
		SetModified();
	}
}

HRESULT CDeskletAppearanceSheet::Drop(IDataObject *obj,DWORD dwKeys, POINTL ptl, DWORD *pEffect)
{
/*	IDropTargetImpl<CDeskletAppearanceSheet>::Drop(obj,dwKeys,ptl,pEffect);
	
	FORMATETC etc = {0};
	etc.cfFormat = CF_HDROP;
	etc.tymed = TYMED_HGLOBAL;
	etc.lindex = -1;

	STGMEDIUM medium = {0};

	if(SUCCEEDED(obj->GetData(&etc,&medium) )
	{
		HDROP hDrop = static_cast<HDROP>( GlobalLock(medium.hGlobal) );

		int num = DragQueryFile(hDrop,-1,NULL,0);
		
		if(num > 0)
		{
			USES_CONVERSION;

			TCHAR file[MAX_PATH+1] = {0};
			DragQueryFile(hDrop,0,file,MAX_PATH);
			DeskletSetImageFileW(docklet->GetHWnd(),T2W(file) );
			SetImage();
				
		}
		GlobalUnlock(hDrop);
		ReleaseStgMedium(&medium);

	}
*/
	return S_OK;
}

void CDeskletAppearanceSheet::OnRemoveImageButton(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	DeskletSetImageFileW(docklet->GetHWnd(),NULL);
	SetImage();
	SetModified();
}

extern BOOL RemoveDeskletAndSaveConfig(HWND,BOOL,BOOL);


void CDeskletAppearanceSheet::OnDeleteItemButton(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	HWND hwnd = docklet->GetHWnd();
	if(RemoveDeskletAndSaveConfig(hwnd,TRUE,TRUE))
		::PropSheet_PressButton(GetParent(),PSBTN_CANCEL);
}

void CDeskletAppearanceSheet::SelectEffect()
{
	USES_CONVERSION;

	int selEffect;
	selEffect = m_comboEffect.GetCurSel();

	docklets::OdDockletInstance* inst = docklet->GetDocklet();

	DrawPluginInstance* dp = inst->GetDrawPlugin();

	if(selEffect != 0 && selEffect != CB_ERR)
	{
		if(selEffect > static_cast<INT>(m_FoundEffects.size()) )
			return;

		if(dp)
		{
			if(strcmp(dp->GetPlugin()->GetName(),W2A(m_FoundEffects[selEffect-1].name.c_str()))!=0)
			{
				DrawPlugin* plugin = DrawPlugin::LoadPlugin(W2A(m_FoundEffects[selEffect-1].filename.c_str()));
				if(plugin)
				{
					inst->SetDrawPlugin(new DrawPluginInstance(plugin,inst->GetWindow(),0,0));
					plugin->Release();
					delete dp;
					dp = 0;
					SetModified();
				}
			}
		}
		else
		{

			DrawPlugin* plugin = DrawPlugin::LoadPlugin(W2A(m_FoundEffects[selEffect-1].filename.c_str()));
			if(plugin)
			{
				inst->SetDrawPlugin(new DrawPluginInstance(plugin,inst->GetWindow(),0,0));
				plugin->Release();
				SetModified();
			}
		}
	}
	else
	{
		DrawPluginInstance* dp = inst->GetDrawPlugin();
		inst->SetDrawPlugin(0);
		delete dp;
		SetModified();
	}
}

void CDeskletAppearanceSheet::OnConfigureEffectButton(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(docklet->GetIsDefaultValues())
		return;

	SelectEffect();

	if(docklet)
	{
		DrawPluginInstance* dp = docklet->GetDocklet()->GetDrawPlugin();
		if(!(dp && dp->Configure(GetParent())))
		{
			CString str, str2;
			str.LoadString(IDS_NOCONFIGFOREFFECT);
			str2.LoadString(IDS_NOCONFIGFOREFFECTCAPTION);
			MessageBox(str,str2,MB_ICONINFORMATION);
		}
	}
}
