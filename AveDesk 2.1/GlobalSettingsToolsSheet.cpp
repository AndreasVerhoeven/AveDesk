// filename:		CGlobalSettingsToolsSheet.hpp
// Description:		Definition of class CGlobalSettingsToolsSheet
// Author:			Andreas Verhoeven
// Last modified:	31 July 2004, 19:58
// Version:			1
// Comments:
//					<none>
// Notes:
// 
//	(c) copyright Andreas Verhoeven 2004
//	Do not modify, reproduce or use this file without my permission!


#include "stdafx.h"
#include "GlobalSettingsToolsSheet.hpp"
#include "Tools.hpp"
#include "controldlg.h"
extern CTools _Tools;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CGlobalSettingsToolsSheet::CGlobalSettingsToolsSheet(_U_STRINGorID title) :
	CPropertyPageImpl<CGlobalSettingsToolsSheet>(title),
	initMode(false)
{
}

CGlobalSettingsToolsSheet::~CGlobalSettingsToolsSheet()
{

}

BOOL CGlobalSettingsToolsSheet::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    initMode = true;
    lb.Attach(GetDlgItem(IDC_LISTTOOLS));
    editDisplay.Attach(GetDlgItem(IDC_EDIT_CAPTION));
    editExecute.Attach(GetDlgItem(IDC_EDIT_COMMAND));
    editArguments.Attach(GetDlgItem(IDC_EDIT_ARGS));
    editLocation.Attach(GetDlgItem(IDC_EDIT_STARTUP_FOLDER));
    if( ReadData() )
    {
        lb.SetCurSel(0);
        UpdateView(0);
    }

	BOOL b = FALSE;
	OnListSelChange(0,0,0,b);

    initMode = false;
    return TRUE;
}

int CGlobalSettingsToolsSheet::ReadData()
{
    lb.ResetContent();
    for( UINT i = 0; i < _Tools.tools.size(); i++ )
    {
        lb.AddString( _Tools.tools[i].Display );
    }
    return _Tools.tools.size();
}

LRESULT CGlobalSettingsToolsSheet::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    int curSel = lb.GetCurSel();

	::EnableWindow(GetDlgItem(IDC_EDIT_CAPTION),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_EDIT_COMMAND),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_EDIT_ARGS),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_EDIT_STARTUP_FOLDER),curSel != LB_ERR);

	::EnableWindow(GetDlgItem(IDC_STATIC_MENU_CAPTION),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_STATIC_COMMAND),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_STATIC_ARGS),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_STATIC_STARTUP_FOLDER),curSel != LB_ERR);

	::EnableWindow(GetDlgItem(IDC_BUTTON_COMMAND),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_BUTTON_ARGS),curSel != LB_ERR);
	::EnableWindow(GetDlgItem(IDC_BUTTON_STARTUP_FOLDER),curSel != LB_ERR);

    if( curSel != LB_ERR )
        UpdateView(curSel);

    return 0;
}

//herd: I' such a lazy bastard 1
#define MAP_CMD(t) edit##t.SetWindowText(tl->t);
void CGlobalSettingsToolsSheet::UpdateView(int curSel)
{
    initMode = true;
    if( curSel <  static_cast<int>(_Tools.tools.size()) )
    {
        CTool *tl = &_Tools.tools[curSel];
    
        MAP_CMD(Display);
        MAP_CMD(Execute);
        MAP_CMD(Arguments);
        MAP_CMD(Location);
    }
    initMode = false;
}


// overridables
BOOL CGlobalSettingsToolsSheet::OnSetActive()
{
	return TRUE;
}

BOOL CGlobalSettingsToolsSheet::OnKillActive()
{
	return TRUE;
}

int CGlobalSettingsToolsSheet::OnApply()
{
    _Tools.SaveAll();
    _Dlg.UpdateMenu();
    return TRUE;
}

void CGlobalSettingsToolsSheet::OnReset()
{
    _Tools.ReadData();
}



BOOL CGlobalSettingsToolsSheet::OnQueryCancel()
{
	return TRUE;    // ok to cancel
}

void CGlobalSettingsToolsSheet::OnHelp()
{
}

void CGlobalSettingsToolsSheet::Invalidate()
{
    // I wonder why this isn't in the template class
    /*
    if( !initMode )
        PropSheet_Changed( ::GetParent((*this)), m_hWnd );
    it is!
    */
    SetModified();
}

//herd: I' such a lazy bastard 2
#define REMAP_CMD(t) edit##t.GetWindowText(tl->t.GetBuffer(MAX_PATH+1),MAX_PATH);tl->t.ReleaseBuffer()
LRESULT CGlobalSettingsToolsSheet::OnEditChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// read all changes into appropriate CTool
    int curSel = lb.GetCurSel();
    if( !initMode && LB_ERR != curSel )
    {
        CTool *tl = &_Tools.tools[curSel];
    
        REMAP_CMD(Display);
        REMAP_CMD(Execute);
        REMAP_CMD(Arguments);
        REMAP_CMD(Location);

        if( wID == IDC_EDIT_CAPTION )
        {
            lb.SetRedraw(FALSE);
            lb.DeleteString(curSel);
            lb.InsertString(curSel, tl->Display);
            lb.SetCurSel(curSel);
            lb.SetRedraw(TRUE);
        }
    }
    Invalidate();
    return 0;
}

void CGlobalSettingsToolsSheet::OnCommandButton(UINT wNotififyCode, int wID, HWND hwndCtrl)
{
	CString strCurrent;
	editExecute.GetWindowText(strCurrent.GetBuffer(MAX_PATH+1),MAX_PATH);
	CFileDialog fd(TRUE,NULL,strCurrent,OFN_HIDEREADONLY,_T("All Files(*.*)\0*.*\0\0"));
	if(fd.DoModal())
	{
		editExecute.SetWindowText(fd.m_szFileName);
		SetModified();
	}
	strCurrent.ReleaseBuffer();
}

void CGlobalSettingsToolsSheet::OnStartupButton(UINT wNotififyCode, int wID, HWND hwndCtrl)
{
	int curSel = lb.GetCurSel();
	if(curSel != LB_ERR)
	{
		CString caption;
		caption.LoadString(IDS_SELECTSTARTUPFOLDERCAPTION);
		CFolderDialog dlg(GetParent(),caption);
		if(dlg.DoModal())
		{
			_Tools.tools[curSel].Location = dlg.GetFolderPath();
			BOOL b = FALSE;
			OnEditChange(0,0,0,b);
		}
	}
}

void CGlobalSettingsToolsSheet::OnArgumentsButton(UINT wNotififyCode, int wID, HWND hwndCtrl)
{
	CString caption;
	caption.LoadString(IDS_TOOLSARGUMENTSHELPCAPTION);

	CString str;
	str.LoadString(IDS_TOOLSARGUMENTSHELPTEXT);

	::MessageBox(GetParent(),str,caption,MB_ICONINFORMATION);
}

LRESULT CGlobalSettingsToolsSheet::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int xPos = GET_X_LPARAM(lParam); 
	int yPos = GET_Y_LPARAM(lParam); 


	CMenu menu2;
	menu2.LoadMenu(IDR_POPUP);

	int curSel = lb.GetCurSel();
	CMenu menu = menu2.GetSubMenu(2);
	menu.EnableMenuItem(ID_TOOLSPOPUPMENU_REMOVE, MF_BYCOMMAND	 | ((curSel !=  LB_ERR )? MF_ENABLED : MF_GRAYED	));
	menu.EnableMenuItem(ID_TOOLSPOPUPMENU_ADDNEW, MF_BYCOMMAND	 | ((_Tools.tools.size() < 10) ? MF_ENABLED : MF_GRAYED	));
	menu.EnableMenuItem(ID_TOOLSPOPUPMENU_MOVEUP, MF_BYCOMMAND	 | ((curSel != LB_ERR && curSel != 0) ? MF_ENABLED : MF_GRAYED	));
	menu.EnableMenuItem(ID_TOOLSPOPUPMENU_MOVEDOWN, MF_BYCOMMAND	 | ((curSel != LB_ERR && curSel < static_cast<int>(_Tools.tools.size()-1)) ? MF_ENABLED : MF_GRAYED	));
	int cmd = static_cast<int>(menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, xPos, yPos, m_hWnd,0));
	if(ID_TOOLSPOPUPMENU_ADDNEW == cmd)
	{
		if(_Tools.tools.size() < 10)
		{
			CTool tool;
			tool.Display.LoadString(IDS_NEWTOOLNAME);
			_Tools.tools.push_back(tool);
			lb.AddString(tool.Display );
			lb.SetCurSel(_Tools.tools.size()-1);
			UpdateView(_Tools.tools.size()-1);

			BOOL b = FALSE;
			OnListSelChange(0,0,0,b);
		}
	}
	else if(ID_TOOLSPOPUPMENU_REMOVE == cmd)
	{
		int curSel = lb.GetCurSel();
		if(curSel != LB_ERR)
		{
			_Tools.tools.erase(_Tools.tools.begin() + curSel);
			lb.DeleteString(curSel);
			if(_Tools.tools.size() > 0)
			{
				if(curSel >= static_cast<int>(_Tools.tools.size()) )
					curSel = _Tools.tools.size() -1;

				lb.SetCurSel(curSel);
				UpdateView(curSel);
			}
			else
			{
				BOOL b = FALSE;
				OnListSelChange(0,0,0,b);
			}
		}
	}
	else if (ID_TOOLSPOPUPMENU_MOVEUP == cmd)
	{
		int curSel = lb.GetCurSel();
		if(curSel != LB_ERR && curSel != 0)
		{
			CTool tmp = _Tools.tools[curSel-1];
			_Tools.tools[curSel-1] = _Tools.tools[curSel];
			_Tools.tools[curSel] = tmp;
			
			lb.LockWindowUpdate();

			ReadData();
			lb.SetCurSel(curSel-1);

			lb.LockWindowUpdate(NULL);
		}
	}
	else if (ID_TOOLSPOPUPMENU_MOVEDOWN == cmd)
	{
		int curSel = lb.GetCurSel();
		if(curSel != LB_ERR && curSel < static_cast<int>(_Tools.tools.size()-1))
		{
			CTool tmp = _Tools.tools[curSel+1];
			_Tools.tools[curSel+1] = _Tools.tools[curSel];
			_Tools.tools[curSel] = tmp;
			
			lb.LockWindowUpdate();

			ReadData();
			lb.SetCurSel(curSel+1);


			lb.LockWindowUpdate(NULL);
		}
	}

	return 0;
}