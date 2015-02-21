// filename:		DeskletLabelSheet.cpp
// Description:		Implementation of class CDeskletLabelSheet
// Author:			Andreas Verhoeven
// Last modified:	4 June 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!

#include "stdafx.h"
#include "DeskletLabelSheet.hpp"
#include "Docklet.h"
#include "AveDesk.h"
#include "Label.h"
#include "ConfigShadowWindow.hpp"
#include "ConfigSubShadowWindow.hpp"
#include "ShadowPropertiesDlg.h"

#include <vector>
#include <string>
#include <algorithm>
#include <memory>


CDeskletLabelSheet::CDeskletLabelSheet(Docklet* d, _U_STRINGorID title) :
	CPropertyPageImpl<CDeskletLabelSheet>(title), docklet(d), initMode(true),
	m_LabelThumbnailThreadHandle(NULL), m_LabelThumbnailThreadShouldStop(TRUE), m_HasSetSelectedLabel(FALSE)
{

}

LRESULT CDeskletLabelSheet::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_LabelThumbnailThreadShouldStop = TRUE;
	if(m_LabelThumbnailThreadHandle != NULL)
	{
		for(;;)
		{
			 MSG msg = {0};
			if(PeekMessage(&msg,m_hWnd,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				DWORD result(WAIT_OBJECT_0);
				HANDLE h = m_LabelThumbnailThreadHandle;
				 if(h != NULL)
					 result = WaitForSingleObject(h, 0);

				 if(WAIT_OBJECT_0 == result)
					 break;
			}
		}
	}

	return 0;
}

LRESULT CDeskletLabelSheet::OnNewLabelThumbnail(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LABELTHUMBNAILPARAM* thumbnailParams = reinterpret_cast<LABELTHUMBNAILPARAM*>(lParam);

	if(thumbnailParams != NULL)
	{

		SIZE s = {0};
		m_ImageList.GetIconSize(s);
		ImageList_Add(m_ImageList.m_hImageList, thumbnailParams->normalBitmap, NULL);
		ImageList_Add(m_ImageList.m_hImageList, thumbnailParams->selectedBitmap, NULL);

		m_comboBackground.AddString(thumbnailParams->name.c_str());
		m_comboBackground.SetItemHeight(thumbnailParams->index + 1, s.cy + 2); // index + 1, because index == 0 is the empty string/label

		int progress = static_cast<int>((thumbnailParams->index * 100) / thumbnailParams->total);
		m_comboBackground.SetLoadingProgress(progress);

		delete thumbnailParams;
	}
	else
	{
		if(docklet->GetLabel().GetBg() != NULL)
		{
			SendDlgItemMessage(IDC_LABELBACKGROUND,CB_SELECTSTRING,0,(LPARAM)docklet->GetLabel().GetBg()->GetName().c_str());
			/*
			if(_wcsicmp(docklet->GetLabel().GetBg()->GetName().c_str(), thumbnailParams->name.c_str()) == 0)
			{
				SendDlgItemMessage(IDC_LABELBACKGROUND,CB_SELECTSTRING,0,(LPARAM)thumbnailParams->name.c_str());
				m_HasSetSelectedLabel = TRUE;
			}*/
		}

	
		m_comboBackground.StopLoadingProgressAndFadeIn();
		//m_comboBackground.SetIsLoading(false);

		// all items have been loaded at this point
		m_HasSetSelectedLabel = TRUE;
	}

	return 0;
}

DWORD CALLBACK CDeskletLabelSheet::LabelThumbnailThreadFunc(LPVOID lpData)
{
	CDeskletLabelSheet* labelSheet = reinterpret_cast<CDeskletLabelSheet*>(lpData);


	std::vector<std::wstring> backgrounds;

	WCHAR path[MAX_PATH];
	GetDLLPathW(_Module.GetModuleInstance(),path,sizeof(path)/sizeof(path[0]));
	wcscat(path,L"Labels\\*.*");

	WIN32_FIND_DATA data;

	HANDLE h = FindFirstFile(path,&data);

	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(wcscmp(data.cFileName,L".")!=0 && wcscmp(data.cFileName,L"..")!=0 && data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				backgrounds.push_back(data.cFileName);
				//m_comboBackground.AddString(data.cFileName);
			}
		}while(FindNextFileW(h,&data) &&!labelSheet->m_LabelThumbnailThreadShouldStop);

		FindClose(h);
	}

	std::sort(backgrounds.begin(), backgrounds.end());


	int index(0);
	int count(backgrounds.size());
	std::vector<std::wstring>::iterator iter = backgrounds.begin(); 
	while(iter != backgrounds.end() && !labelSheet->m_LabelThumbnailThreadShouldStop)
	{
		std::wstring &bgName = *iter;
		LabelBackground* bg = LabelBackground::LoadLabelBackground(bgName);

		index++;
		if(bg != NULL)
		{
			SIZE s = {0};
			labelSheet->m_ImageList.GetIconSize(s);

			Bitmap bmp(s.cx, s.cy);
			Graphics g(&bmp);

			if(GetIsRunningWin2k()  == TRUE)
				g.Clear(Color(::GetSysColor(COLOR_WINDOW)));

			bg->GetThumbnail(g, s, false);


			LABELTHUMBNAILPARAM* thumbnailParams = new LABELTHUMBNAILPARAM;
			thumbnailParams->index = index;
			thumbnailParams->name = bgName;
			thumbnailParams->total = count;

			CBitmap hBmp;
			bmp.GetHBITMAP(NULL,&thumbnailParams->normalBitmap.m_hBitmap);

			if(hBmp == NULL)
			{
				g.Clear(Color(::GetSysColor(COLOR_WINDOW)));

				bmp.GetHBITMAP(NULL,&hBmp.m_hBitmap);
			}


			Bitmap bmp2(s.cx, s.cy);
			Graphics g2(&bmp2);
			if(GetIsRunningWin2k()  == TRUE)
				g.Clear(Color(::GetSysColor(COLOR_HIGHLIGHT)));

			bg->GetThumbnail(g2, s, true);

			CBitmap hBmp2;
			bmp2.GetHBITMAP(NULL,&thumbnailParams->selectedBitmap.m_hBitmap);

			bg->Release();

			::SendMessage(labelSheet->m_hWnd, WM_ONNEWLABELTHUMBNAIL, 0L, reinterpret_cast<LPARAM>(thumbnailParams));
		}

		iter++;
	}

	// post a NULL message to indicate we are done!
	::SendMessage(labelSheet->m_hWnd, WM_ONNEWLABELTHUMBNAIL, 0L, 0L);


	//CloseHandle(labelSheet->m_LabelThumbnailThreadHandle);
	labelSheet->m_LabelThumbnailThreadHandle = NULL;

	ExitThread(2);

	return 0;
}

CDeskletLabelSheet::~CDeskletLabelSheet()
{

}

void CDeskletLabelSheet::FillLabelBackgroundComboBox()
{
	m_HasSetSelectedLabel = FALSE;
	m_LabelThumbnailThreadShouldStop = FALSE;
	m_comboBackground.SetIsLoading(true);
	m_comboBackground.EnableWindow(FALSE);
	DWORD dummy(0);
	this->m_LabelThumbnailThreadHandle = CreateThread(NULL,0,LabelThumbnailThreadFunc,reinterpret_cast<LPVOID>(this),0,&dummy);
}

LRESULT CDeskletLabelSheet::OnDropSettingsNotify(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	SetModified();
	bHandled = TRUE;
	return 1;
}

BOOL CDeskletLabelSheet::OnInitDialog (HWND hwndFocus, LPARAM lParam)
{
	initMode = true;

	Label& label = docklet->GetLabel();

	m_comboLabelType				= GetDlgItem(IDC_LABELTYPE);
	m_checkOnlyVisibleOnMouseOver	= GetDlgItem(IDC_LABELONLYONMOUSEOVER);
	m_checkSubLabel					= GetDlgItem(IDC_SUBLABEL);
	m_checkShadow					= GetDlgItem(IDC_SHADOW);
	m_checkShadow2					= GetDlgItem(IDC_SHADOW2);
	m_comboBackground.SubclassWindow(GetDlgItem(IDC_LABELBACKGROUND));
	m_comboHorAlign					= GetDlgItem(IDC_HALIGN2);
	m_comboVerAlign					= GetDlgItem(IDC_VALIGN2);
	m_editRotation					= GetDlgItem(IDC_ROTATION);
	m_checkTopDown					= GetDlgItem(IDC_TOPDOWN);
	m_comboPosition					= GetDlgItem(IDC_POSITION);
	m_editMargin					= GetDlgItem(IDC_MARGIN);
	m_editMaxWidth					= GetDlgItem(IDC_WIDTH);
	m_editMaxHeight					= GetDlgItem(IDC_HEIGHT);
	m_spinRotation					= GetDlgItem(IDC_ROTATIONSPINNER);
	m_spinRotation.SetRange(0,360);
	m_spinRotation.SetPos(label.GetRotation());


	m_MainShadowPropsDS = boost::shared_ptr<CDropSettingsImpl< CShadowProperties> >( new CDropSettingsImpl<CShadowProperties>(label.GetMainLabelProperties().GetShadowProperties() ) );
	m_MainShadowPropsDS->AddRef();
	m_MainShadowPropsDS->SubclassWindow ( GetDlgItem(IDCONFIGSHADOW));
	m_MainShadowPropsDS->SetNotifyHWnd(m_hWnd);

	m_SubShadowPropsDS = boost::shared_ptr<CDropSettingsImpl< CShadowProperties> >( new CDropSettingsImpl<CShadowProperties>(label.GetSubLabelProperties().GetShadowProperties() ) );
	m_SubShadowPropsDS->AddRef();
	m_SubShadowPropsDS->SubclassWindow ( GetDlgItem(IDCONFIGSUBSHADOW));
	m_SubShadowPropsDS->SetNotifyHWnd(m_hWnd);

	m_MainLabelPropsSettings = boost::shared_ptr<CDropSettingsImpl< CLabelProperties> >( new CDropSettingsImpl<CLabelProperties>(label.GetMainLabelProperties() ) );
	m_MainLabelPropsSettings->AddRef();
	m_MainLabelPropsSettings->SubclassWindow ( GetDlgItem(IDC_CAPTIONGROUP));
	m_MainLabelPropsSettings->SetNotifyHWnd(m_hWnd);

	m_LabelDS = boost::shared_ptr<CDropSettingsImpl< Label> >( new CDropSettingsImpl<Label>(label) );
	m_LabelDS->AddRef();
	m_LabelDS->SubclassWindow ( m_hWnd);
	m_LabelDS->SetNotifyHWnd(m_hWnd);

	CString buf;
	buf.LoadString(IDS_LABELTYPE0);
	m_comboLabelType.AddString(buf);
	buf.LoadString(IDS_LABELTYPE2);
	m_comboLabelType.AddString(buf);
	m_comboLabelType.SetCurSel(docklet->GetLabelType());

	buf.LoadString(IDS_HALIGN0);
	m_comboHorAlign.AddString(buf);
	buf.LoadString(IDS_HALIGN1);
	m_comboHorAlign.AddString(buf);
	buf.LoadString(IDS_HALIGN2);
	m_comboHorAlign.AddString(buf);
	m_comboHorAlign.SetCurSel(label.GetHAlign());

	buf.LoadString(IDS_VALIGN0);
	m_comboVerAlign.AddString(buf);
	buf.LoadString(IDS_VALIGN1);
	m_comboVerAlign.AddString(buf);
	buf.LoadString(IDS_VALIGN2);
	m_comboVerAlign.AddString(buf);
	m_comboVerAlign.SetCurSel(label.GetVAlign());

	buf.LoadString(IDS_LABELPOSITION0);
	m_comboPosition.AddString(buf);
	buf.LoadString(IDS_LABELPOSITION1);
	m_comboPosition.AddString(buf);
	buf.LoadString(IDS_LABELPOSITION2);
	m_comboPosition.AddString(buf);
	buf.LoadString(IDS_LABELPOSITION3);
	m_comboPosition.AddString(buf);
	m_comboPosition.SetCurSel(label.GetPosition());

	SetDlgItemInt(IDC_ROTATION,label.GetRotation(),TRUE);
	SetDlgItemInt(IDC_MARGIN,label.GetMargin(),TRUE);
	SetDlgItemInt(IDC_WIDTH,label.GetWidth(),FALSE);
	SetDlgItemInt(IDC_HEIGHT,label.GetHeight(),FALSE);
	m_checkTopDown.SetCheck(label.GetDrawTopDown());

	m_checkOnlyVisibleOnMouseOver.SetCheck(docklet->GetLabelOnlyOnMouseOver());
	m_checkSubLabel.SetCheck(label.GetShowSubLabel());

	CLabelProperties mainLabelProps = label.GetMainLabelProperties();
	CLabelProperties subLabelProps = label.GetSubLabelProperties();

	m_checkShadow.SetCheck(mainLabelProps.GetHasShadow() );
	m_checkShadow2.SetCheck(subLabelProps.GetHasShadow() );

	m_butNormalColor.SubclassWindow(GetDlgItem(IDSELECTCOLOR));
	COLORREF cr = mainLabelProps.GetColor().ToCOLORREF();
	m_butNormalColor.SetColor(cr);
	m_butNormalColor.SetDefaultColor(cr);
	m_butNormalColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_butNormalColor.SetCustomText(IDS_MORECOLORS);


	COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
	::GetComboBoxInfo(m_comboBackground, &info);
	SIZE comboEditSize = SIZEFROMRECT(info.rcItem);

	m_ImageList.Create(comboEditSize.cx,20,ILC_COLOR32,1, 1);
	SIZE s = {0};
	m_ImageList.GetIconSize(s);

	m_comboBackground.SetItemHeight(-1,s.cy + 2);
	m_comboBackground.SetImageList(m_ImageList);
	m_comboBackground.SetDroppedWidth( s.cx);
	m_comboBackground.SetItemHeight(0,s.cy + 2);
	m_comboBackground.AddString(TEXT(""));
	FillLabelBackgroundComboBox();
	//m_comboBackground.SelectString(0,(label.bg)?label.bg->GetName():"");
	if(NULL == label.GetBg())
	{
		SendDlgItemMessage(IDC_LABELBACKGROUND,CB_SELECTSTRING,0,(LPARAM)L"" );	
		m_HasSetSelectedLabel = TRUE;
	}
	//SendDlgItemMessage(IDC_LABELBACKGROUND,CB_SELECTSTRING,0,(LPARAM) ( label.GetBg() ? label.GetBg()->GetName().c_str() : L"") );


	m_butSubColor.SubclassWindow(GetDlgItem(IDSELECTCOLOR2));
	COLORREF subCr = subLabelProps.GetColor().ToCOLORREF();
	m_butSubColor.SetColor(subCr);
	m_butSubColor.SetDefaultColor(subCr);
	m_butSubColor.SetDefaultText(IDS_CURRENTCOLOR);
	m_butSubColor.SetCustomText(IDS_MORECOLORS);

	initMode = false;

	return TRUE;
}

void CDeskletLabelSheet::OnEditValue(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	if(!initMode && (wNotifyCode == BN_CLICKED || wNotifyCode == EN_CHANGE || wNotifyCode ==CBN_SELCHANGE))
	{
		if(GetDlgItem(IDC_LABELBACKGROUND) == hwndCtrl)
			m_HasSetSelectedLabel = TRUE;

		SetModified();
	}

}

BOOL CDeskletLabelSheet::OnEditColor(LPNMHDR notifyData)
{
	if(notifyData->code == CPN_SELCHANGE)
	{
		SetModified();
	}
	return TRUE;
}

LRESULT CDeskletLabelSheet::OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MEASUREITEMSTRUCT* pMis = (MEASUREITEMSTRUCT*)lParam;
	if(pMis)
	{
		if( SendDlgItemMessage(IDC_LABELBACKGROUND,CB_GETDROPPEDSTATE,0,0) )
			pMis->itemHeight = 30;
	}

	return 0;
}

bool CDeskletLabelSheet::SelectFontProps(CLabelProperties& lp)
{
	CWindowDC screenDC(NULL);

	LOGFONTW lf = {0};
	wcscpy(lf.lfFaceName,lp.GetFont().c_str() );
	lf.lfHeight = lp.GetSize() * GetDeviceCaps(screenDC, LOGPIXELSY) / 72;
	lf.lfItalic = (BYTE)(lp.GetStyle() & FontStyleItalic);
	lf.lfStrikeOut = (BYTE)(lp.GetStyle() & FontStyleStrikeout);
	lf.lfUnderline = (BYTE)(lp.GetStyle() & FontStyleUnderline);
	lf.lfWeight = ((BYTE)lp.GetStyle() & FontStyleBold) ? FW_BOLD : FW_REGULAR;
	CHOOSEFONTW cf = {0};
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &lf;
	cf.hwndOwner = ::GetActiveWindow();
	cf.hInstance = _Module.GetModuleInstance();
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.iPointSize = lp.GetSize()*10;

	if(!ChooseFontW(&cf))return false;

	int style = FontStyleRegular;
	if(lf.lfItalic) style |= FontStyleItalic;
	if(lf.lfStrikeOut) style |= FontStyleStrikeout;
	if(lf.lfUnderline) style |= FontStyleUnderline;
	if(lf.lfWeight >= FW_BOLD) style |= FontStyleBold;

	lp.SetStyle(style);
	lp.SetFont(lf.lfFaceName);
	lp.SetSize(cf.iPointSize/10 );

	return true;
}

void CDeskletLabelSheet::OnSelectFont(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	CLabelProperties lp = docklet->GetLabel().GetMainLabelProperties();
	if(SelectFontProps(lp) )
	{
		docklet->GetLabel().SetMainLabelProperties(lp);
			SetModified();
	}

/*	HDC screenDC = GetDC(NULL);

	//char tempfont[1024] = {0};
	//WideCharToMultiByte(CP_ACP,0,docklet->GetLabel().Font_,-1,tempfont,sizeof(tempfont)/sizeof(tempfont[0]),0,0);
	LOGFONTW lf = {0};
	wcscpy(lf.lfFaceName,lp.GetFont().c_str() );
	lf.lfHeight = lp.Size_ * GetDeviceCaps(::screenDC, LOGPIXELSY) / 72;
	lf.lfItalic = lp.Style & FontStyleItalic;
	lf.lfStrikeOut = label.Style & FontStyleStrikeout;
	lf.lfUnderline = docklet->GetLabel().Style & FontStyleUnderline;
	lf.lfWeight = (docklet->GetLabel().Style & FontStyleBold) ? FW_BOLD : FW_REGULAR;
	CHOOSEFONT cf = {0};
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &lf;
	cf.hwndOwner = ::GetActiveWindow();
	cf.hInstance = _Module.GetModuleInstance();
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.iPointSize = docklet->GetLabel().Size_*10;

	ReleaseDC(NULL,0);
	if(!ChooseFont(&cf))return;

	SetModified();

	docklet->GetLabel().Style = FontStyleRegular;
	if(lf.lfItalic) docklet->GetLabel().Style |= FontStyleItalic;
	if(lf.lfStrikeOut) docklet->GetLabel().Style |= FontStyleStrikeout;
	if(lf.lfUnderline) docklet->GetLabel().Style |= FontStyleUnderline;
	if(lf.lfWeight >= FW_BOLD) docklet->GetLabel().Style |= FontStyleBold;

	MultiByteToWideChar(CP_ACP,0,lf.lfFaceName,-1,docklet->GetLabel().Font_,sizeof(docklet->GetLabel().Font_)/sizeof(docklet->GetLabel().Font_[0]));
	docklet->GetLabel().Size_ = cf.iPointSize/10;
*/
}

void CDeskletLabelSheet::OnSelectSubFont(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	CLabelProperties lp = docklet->GetLabel().GetSubLabelProperties();
	if(SelectFontProps(lp) )
	{
		docklet->GetLabel().SetSubLabelProperties(lp);
			SetModified();
	}

/*	char tempfont[1024] = {0};
	WideCharToMultiByte(CP_ACP,0,docklet->GetLabel().subFont,-1,tempfont,sizeof(tempfont)/sizeof(tempfont[0]),0,0);
	LOGFONT lf = {0};
	strcpy(lf.lfFaceName,tempfont);
	lf.lfHeight = docklet->GetLabel().subSize * GetDeviceCaps(::GetDC(0), LOGPIXELSY) / 72;
	lf.lfItalic = docklet->GetLabel().subStyle & FontStyleItalic;
	lf.lfStrikeOut = docklet->GetLabel().subStyle & FontStyleStrikeout;
	lf.lfUnderline = docklet->GetLabel().subStyle & FontStyleUnderline;
	lf.lfWeight = (docklet->GetLabel().subStyle & FontStyleBold) ? FW_BOLD : FW_REGULAR;

	CHOOSEFONT cf = {0};
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &lf;
	cf.hwndOwner = ::GetActiveWindow();
	cf.hInstance = _Module.GetModuleInstance();
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.iPointSize = docklet->GetLabel().subSize*10;
	if(!ChooseFont(&cf))return;

	SetModified();

	docklet->GetLabel().subStyle = FontStyleRegular;
	if(lf.lfItalic) docklet->GetLabel().subStyle |= FontStyleItalic;
	if(lf.lfStrikeOut) docklet->GetLabel().subStyle |= FontStyleStrikeout;
	if(lf.lfUnderline) docklet->GetLabel().subStyle |= FontStyleUnderline;
	if(lf.lfWeight >= FW_BOLD) docklet->GetLabel().subStyle |= FontStyleBold;

	MultiByteToWideChar(CP_ACP,0,lf.lfFaceName,-1,docklet->GetLabel().subFont,sizeof(docklet->GetLabel().subFont)/sizeof(docklet->GetLabel().subFont[0]));
	docklet->GetLabel().subSize = cf.iPointSize/10;
*/
}

void CDeskletLabelSheet::OnShadowSettings(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	
	CShadowProperties& sp = docklet->GetLabel().GetMainLabelProperties().GetShadowProperties();
	CShadowPropertiesDlg dlg(sp,IDS_MAINLABELSHADOWDLGTITLE);

	if(dlg.DoModal() != IDCANCEL )
		SetModified();
}

void CDeskletLabelSheet::OnSubShadowSettings(UINT wNotifyCode, int wID, HWND hwndCtrl)
{
	CShadowProperties& sp = docklet->GetLabel().GetSubLabelProperties().GetShadowProperties();
	CShadowPropertiesDlg dlg(sp,IDS_MAINLABELSHADOWDLGTITLE);

	if(dlg.DoModal() != IDCANCEL )
		SetModified();
}

int CDeskletLabelSheet::OnApply()
{
	Label& label = docklet->GetLabel();

	docklet->GetLabelType() = m_comboLabelType.GetCurSel();
	docklet->GetLabelOnlyOnMouseOver() = m_checkOnlyVisibleOnMouseOver.GetCheck() == TRUE;

	if(!docklet->GetIsDefaultValues())
	{
		if(docklet->GetLabelType() != LABEL_REGULAR || docklet->GetLabelOnlyOnMouseOver())
		{
			label.ShowWindow(SW_HIDE);
		}
		else
		{
			label.ShowWindow(SW_SHOWNA);
		}
	}

	if(m_HasSetSelectedLabel)
	{

		WCHAR tmp[MAX_PATH+1] = {0};
		::SendDlgItemMessageW(m_hWnd,IDC_LABELBACKGROUND,WM_GETTEXT,MAX_PATH,(LPARAM)tmp);
		std::wstring lblBg = tmp;

		LabelBackground*& bg = label.GetBg();
		if(lblBg == L"")
		{
			if(bg)
			{
				bg->Release();
				bg = NULL;
			}
		}
		else
		{
			if(bg )
			{
				bg->Release();
				bg = LabelBackground::LoadLabelBackground(lblBg);
			}
			else
			{
				bg = LabelBackground::LoadLabelBackground(lblBg);	
			}
		}
	}

	label.SetDrawTopDown( m_checkTopDown.GetCheck() == TRUE);
	label.SetShowSubLabel( m_checkSubLabel.GetCheck() == TRUE);

	label.SetVAlign( m_comboVerAlign.GetCurSel() );
	label.SetHAlign( m_comboHorAlign.GetCurSel() );
	label.SetPosition( m_comboPosition.GetCurSel() );
	
	label.GetMainLabelProperties().SetHasShadow( m_checkShadow.GetCheck() == TRUE );
	label.GetSubLabelProperties().SetHasShadow( m_checkShadow2.GetCheck() == TRUE );

	label.SetAlpha( (BYTE)GetDlgItemInt(IDC_ALPHA,NULL,FALSE) );
	label.SetRotation( GetDlgItemInt(IDC_ROTATION,0,FALSE) );

	label.SetWidth(	 GetDlgItemInt(IDC_WIDTH,0,FALSE)	);
	label.SetHeight( GetDlgItemInt(IDC_HEIGHT,0,FALSE)	);
	label.SetMargin( GetDlgItemInt(IDC_MARGIN,0,TRUE)	);
	
	COLORREF cr = m_butNormalColor.GetColor();
	m_butNormalColor.SetDefaultColor(cr);
	Color c;
	c.SetFromCOLORREF(cr);
	label.GetMainLabelProperties().SetColor(c);

	COLORREF crSub = m_butSubColor.GetColor();
	m_butSubColor.SetDefaultColor(crSub);
	Color cSub;
	cSub.SetFromCOLORREF(crSub);
	label.GetSubLabelProperties().SetColor(cSub);

	if(!docklet->GetIsDefaultValues() )
	{
		docklet->UpdateLabelPosition();
		docklet->UpdateLabelWindow();
	}

	return TRUE;
}
