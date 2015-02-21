#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include "../DeskletSDK.h"
#include "../DeskletSDK.cpp"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "resource.h"

#include <vector>
#include <string>
#include <sstream>

BOOL CALLBACK EditDlgProc(HWND hDlg, UINT msg, WPARAM w, LPARAM l);
DWORD CALLBACK EditBoxThread(LPVOID lpData);
void RepaintEditControl(HWND hwnd);

HBITMAP GetEditControlBitmap(HWND hwnd)
{
	RECT r = {0};
	GetWindowRect(hwnd,&r);
	int width = r.right - r.left;
	int height= r.bottom - r.top;

	HideCaret(hwnd);
	HDC dc = CreateCompatibleDC(GetDC(hwnd));
	HBITMAP tempBmp = CreateCompatibleBitmap(GetDC(hwnd), width,height);
	HBITMAP bmp = (HBITMAP)SelectObject(dc, tempBmp);
	SendMessage(hwnd,WM_ERASEBKGND,(WPARAM)dc,0);
	SendMessage(hwnd,WM_PAINT,(WPARAM)dc,0);
	BitBlt(GetDC(hwnd), 0, 0, width, height, dc, 0, 0, SRCCOPY);
	SelectObject(dc, bmp);
	DeleteDC(dc);
	ReleaseDC(hwnd, GetDC(hwnd));
	ShowCaret(hwnd);

	return tempBmp;
}


class DashBoardSticky
{
private:
	HWND hwnd;
	HINSTANCE dll;
	std::wstring text;

	Bitmap* bmpBg;

	HBRUSH brush;
	HFONT  font;

	HANDLE hTemp;
	DWORD  threadId;

	POINT point;

	HWND editHwnd;

	bool isInBackMode;

public:
	DashBoardSticky(HWND hwnd, HINSTANCE dll) :
		hwnd(hwnd),
		dll(dll),
		brush(0),
		font(0),
		hTemp(0),
		threadId(0),
		editHwnd(0),
		isInBackMode(false)
	{
		char bg[MAX_PATH] = {0};
		DeskletGetRelativeFolder(hwnd,bg);
		PathAppend(bg,"images\\bg.png");
		bmpBg = DeskletLoadGDIPlusImage(bg);

		DeskletSetImageFile(hwnd,bg);
		SIZE s ={0};
		s.cx = bmpBg->GetWidth();
		s.cy = bmpBg->GetHeight();
		DeskletSetSize(hwnd,&s);

		LOGBRUSH lb = {0};
		lb.lbStyle = BS_HOLLOW;
		brush = CreateBrushIndirect(&lb);

		LOGFONT lf = {0};
		strcpy(lf.lfFaceName,"Arial");
		lf.lfQuality = ANTIALIASED_QUALITY;
		lf.lfHeight = 30;

		font = CreateFontIndirect(&lf);

		HANDLE h = CreateThread(0,0,EditBoxThread,(LPVOID)this,0,&threadId);
		SetTempHandle(h);
		SetThreadId(threadId);
		SleepEx(20,0);
		PostThreadMessage(threadId,WM_USER+1,0,0);

	}

	~DashBoardSticky()
	{
		delete bmpBg;
		bmpBg = 0;

		PostThreadMessage(threadId,WM_USER+2,0,0);
		SleepEx(20,TRUE);
		

		DeleteObject(brush);
		DeleteObject(font);
	}

	UINT GetTextLayerId()
	{
		BOOL success(FALSE);
		UINT layerId = DeskletLayerGetLayer(GetWindow(),"Sticky.Text",&success);
		if(!success)
			layerId = DeskletLayerAdd(GetWindow(),"Sticky.Text");

		return layerId;
	}

	void UpdateTextLayer()
	{
		SIZE s ={0};
		DeskletGetSize(GetWindow(),&s);
		Bitmap* bmp = new Bitmap(bmpBg->GetWidth(),bmpBg->GetHeight());//(Bitmap*)(((Image*)bmpBg)->Clone());

		RectF rc(11,10,s.cx - 40+16,s.cy - 40);

	/*	FontFamily  fontFamily(L"Arial");
		Font		gdiplusFont(L"Arial",20,0);
		SolidBrush  black(Color(255,0,0,0));

		GraphicsPath path(FillModeAlternate);
		StringFormat format(0, LANG_NEUTRAL);
		format.SetLineAlignment(StringAlignmentNear);
		format.SetAlignment(StringAlignmentNear);


		path.AddString(text.c_str(), -1, &fontFamily, 0, 24, rc, &format);
*/

		Graphics g(bmp);
		g.SetSmoothingMode(SmoothingModeAntiAlias);
		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
//		g.FillPath(&black,&path);


		RectF rc2 = rc;
		rc2.Width -= 16;
		rc2.Height-= 10;
		//SolidBrush white(Color(255,255,255,255));
		//g.FillRectangle(&white,rc2);
		//g.DrawImage(bmpBg,rc2,rc2.X,rc2.Y,rc2.Width,rc2.Height,UnitPixel);
//		g.DrawString(text.c_str(),-1,&gdiplusFont,rc,&format,&black);

		HDC dc = g.GetHDC();

		SelectObject(dc,GetFont());
		//SelectObject(dc,GetHollowBrush());
		SetBkMode(dc,TRANSPARENT);

		//RECT r = {0};
		//GetWindowRect(GetWindow(),&r);
		//r.left = 15;
		//r.top = 10;
		//r.right =s.cx - 40;
		//r.bottom = s.cy - 40;
		//DrawTextW(dc,text.c_str(),text.size(),&r,DT_WORDBREAK);
		HBITMAP hBmp = GetEditControlBitmap(GetDlgItem(GetEditHWND(),IDC_TEXT));

		RECT r = {0};
		GetWindowRect(GetDlgItem(GetEditHWND(),IDC_TEXT),&r);
		int width = r.right - r.left;
		int height= r.bottom - r.top;

		Bitmap* temp = new Bitmap(width,height);
		Graphics g2(temp);
		g2.Clear(Color(255,255,255,255));
		HDC dc2 = g2.GetHDC();

		SendMessage(GetDlgItem(GetEditHWND(),IDC_TEXT),WM_ERASEBKGND,(WPARAM)dc2,0);
		SendMessage(GetDlgItem(GetEditHWND(),IDC_TEXT),WM_PAINT,(WPARAM)dc2,0);

		g2.ReleaseHDC(dc2);

		g.ReleaseHDC(dc);

		g.DrawImage(temp,15,10,width,height);

		delete temp;


		DeskletLayerSetImage(GetWindow(),GetTextLayerId(),TRUE,bmp,TRUE);

	}

	HWND GetWindow()const{return hwnd;};
	HINSTANCE GetDll()const{return dll;};
	std::wstring GetText()const{return text;};
	void SetText(std::wstring s){text=s;};

	Bitmap* GetBackGroundImage()const{return bmpBg;};

	HBRUSH GetHollowBrush()const{return brush;};

	HFONT GetFont()const{return font;};

	HANDLE GetTempHandle()const{return hTemp;};
	void   SetTempHandle(HANDLE h){hTemp=h;};

	DWORD GetThreadId()const{return threadId;};
	void  SetThreadId(DWORD d){threadId=d;};

	POINT GetPoint()const{return point;};
	void SetPoint(POINT p){point = p;};

	HWND GetEditHWND()const{return editHwnd;};
	void SetEditHWND(HWND hwnd){editHwnd=hwnd;};

	bool GetIsInBackMode()const{return isInBackMode;};
	void SetIsInBackMode(bool b){isInBackMode=b;};

	void SetVisible(POINT p)
	{

		SendDlgItemMessage(GetEditHWND(),IDC_TEXT,EM_SETSEL,-1,-1);
		p.x -= 15;
		p.y -= 10;
		SendDlgItemMessage(GetEditHWND(),IDC_TEXT,WM_LBUTTONDOWN,0,MAKEWORD(p.x,p.y));
		SendDlgItemMessage(GetEditHWND(),IDC_TEXT,WM_LBUTTONUP,0,MAKEWORD(p.x,p.y));

		RECT r = {0};
		GetWindowRect(GetWindow(),&r);
			MoveWindow(GetDlgItem(GetEditHWND(),IDC_TEXT),0,0,r.right - r.left - 40, r.bottom - r.top - 40,FALSE);	
		MoveWindow(GetEditHWND(),r.left+15,r.top+10,r.right - r.left - 40, r.bottom - r.top - 40,FALSE);
		
		//ShowWindow(GetEditHWND(),SW_SHOW);
		SetWindowPos(GetEditHWND(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

		//SetWindowPos(GetEditHWND(),HWND_TOP,r.left+15,r.top+10,r.right - r.left - 40, r.bottom - r.top - 40, SWP_SHOWWINDOW);
	}
};


void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"DashBoardSticky");
	strcpy(szAuthor,"A cool guy");
	strcpy(szNotes,"Baabooo");
	*iVersion = 202;
}

std::wstring StringToWString(const std::string& str)
{
	// post: a multibyte string has been converted to a unicode wstring equivalent

	std::wstring ret;

	WCHAR* tmp = new WCHAR[ str.length() + 2];
	memset(static_cast<void*>(tmp),0,str.length()+2);

	MultiByteToWideChar(GetACP(),0,str.c_str(),-1,tmp,str.length()+1 );

	ret = tmp;

	delete[] tmp;

	return ret;
}
//--------------------------------------------------------------------------------------------

std::string WStringToString(const std::wstring& wstr)
{
	// post: a unicode wstring has been converted to a multibyte wtring equivalent
	std::string ret;

	char* tmp = new char[ wstr.length() + 2];
	memset(static_cast<void*>(tmp),0,wstr.length()+2);

	WideCharToMultiByte(GetACP(),0,wstr.c_str(),-1,tmp,wstr.length()+1,NULL,NULL );

	ret = tmp;

	delete[] tmp;

	return ret;
}

DashBoardSticky* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE dll,const char* ini, const char* section)
{
	DashBoardSticky* dbs = new DashBoardSticky(hwndDesklet,dll);

	DeskletActivateExtendedDragDrop(hwndDesklet);

	if(ini != NULL && section != NULL)
	{
		std::wstring wIni = StringToWString(ini);
		std::wstring wSection = StringToWString(section);

		WCHAR text[10024] = {0};
		GetPrivateProfileStringW(wSection.c_str(),L"StickyText",dbs->GetText().c_str(),text,sizeof(text)/sizeof(text[0]), wIni.c_str());
		dbs->SetText(text);
		SetDlgItemTextW(dbs->GetEditHWND(),IDC_TEXT,text );
		RepaintEditControl(GetDlgItem(dbs->GetEditHWND(),IDC_TEXT));
		POINT p = {-32000, - 32000};
		dbs->SetVisible(p);
		dbs->UpdateTextLayer();
	}


	return dbs;
}

void __stdcall OnSave(DashBoardSticky* dbs, const char* ini, const char* section, BOOL bUnused)
{
	std::wstring wIni = StringToWString(ini);
	std::wstring wSection = StringToWString(section);

	WritePrivateProfileStringW(wSection.c_str(), L"StickyText", dbs->GetText().c_str(), wIni.c_str());
}

void __stdcall OnDestroy(DashBoardSticky* dbs, HWND hwndDesklet)
{
	delete dbs;
}

DWORD CALLBACK EditBoxThread(LPVOID lpData)
{
	DashBoardSticky* dbs = (DashBoardSticky*)lpData;
	
	MSG msg = {0};
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message  == WM_USER+1)
		{
			HWND hwnd = CreateDialogParam(dbs->GetDll(),MAKEINTRESOURCE(IDD_EDIT),NULL,EditDlgProc,(LPARAM)dbs);	
			dbs->SetEditHWND(hwnd);
		}
		if(msg.message == WM_USER+2)
		{
			DestroyWindow(dbs->GetEditHWND() );
			break;
		}
	}

	CloseHandle(dbs->GetTempHandle());
	dbs->SetTempHandle(NULL);
	dbs->SetThreadId(0);

	ExitThread(TRUE);
	return TRUE;
};


void __stdcall OnProcessMessage(DashBoardSticky* dbs, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_SETFOCUS || msg == WM_LBUTTONDOWN)
	{
		POINT curPos = {0};
		GetCursorPos(&curPos);

		RECT r = {0};
		GetWindowRect(dbs->GetWindow(),&r);
		curPos.x -= r.left;
		curPos.y -= r.top;

		if(!dbs->GetIsInBackMode())
		{

			if(msg == WM_LBUTTONDOWN && curPos.x >=200 && curPos.x < 230 && curPos.y >= 170 && curPos.y < 205)
			{
				hwnd = dbs->GetWindow();
				DeskletLayerSetVisibility(hwnd,dbs->GetTextLayerId(),FALSE,FALSE);

				for(int i=0;i<6;++i)
				{
					std::stringstream ss;
					ss <<"anim" << i+1 << ".png";

					char bg[MAX_PATH] = {0};
					DeskletGetRelativeFolder(hwnd,bg);
					PathAppend(bg,"images//");
					PathAppend(bg,ss.str().c_str());
					DeskletSetImageFile(hwnd,bg);
					SleepEx(30,TRUE);
				}
				
				char bg[MAX_PATH] = {0};
				DeskletGetRelativeFolder(hwnd,bg);
				PathAppend(bg,"images//back.png");
				DeskletSetImageFile(hwnd,bg);

				dbs->SetIsInBackMode(true);
			}
			else if(curPos.x >=15 && curPos.x < 210 && curPos.y >= 10 && curPos.y < 180)
			{
				dbs->SetVisible(curPos);
			}
		}
		else if(msg == WM_LBUTTONDOWN && curPos.x >=150 && curPos.x < 210 && curPos.y >= 145 && curPos.y < 165)
		{
			hwnd = dbs->GetWindow();
			DeskletLayerSetVisibility(hwnd,dbs->GetTextLayerId(),FALSE,FALSE);

			for(int i=0;i<6;++i)
			{
				std::stringstream ss;
				ss <<"anim" << 6-i << ".png";

				char bg[MAX_PATH] = {0};
				DeskletGetRelativeFolder(hwnd,bg);
				PathAppend(bg,"images\\");
				PathAppend(bg,ss.str().c_str());
				DeskletSetImageFile(hwnd,bg);
				SleepEx(30,TRUE);
			}
				
			DeskletLayerSetVisibility(hwnd,dbs->GetTextLayerId(),FALSE,TRUE);
			char bg[MAX_PATH] = {0};
			DeskletGetRelativeFolder(hwnd,bg);
			PathAppend(bg,"images//bg.png");
			DeskletSetImageFile(hwnd,bg);

			dbs->SetIsInBackMode(false);
			dbs->SetVisible(curPos);
		}
	}
}

BOOL __stdcall OnMouseOn(DashBoardSticky* dbs)
{
	if(dbs->GetIsInBackMode())return FALSE;

	HWND hwnd = dbs->GetWindow();
	char bg[MAX_PATH] = {0};
	GetDLLPath(dbs->GetDll(),bg,MAX_PATH);
	PathAppend(bg,"\\images\\mouseover.png");
	DeskletSetImageFile(hwnd,bg);

	return TRUE;
}

BOOL __stdcall OnMouseOut(DashBoardSticky* dbs)
{
	if(dbs->GetIsInBackMode())return FALSE;

	HWND hwnd = dbs->GetWindow();
	char bg[MAX_PATH] = {0};
	GetDLLPath(dbs->GetDll(),bg,MAX_PATH);
	PathAppend(bg,"\\images\\bg.png");
	DeskletSetImageFile(hwnd,bg);

	return FALSE;
}

BOOL __stdcall OnLeftButtonClick(DashBoardSticky* dbs, POINT* p, SIZE* s)
{
	//DialogBoxParam(dbs->GetDll(),MAKEINTRESOURCE(IDD_EDIT),NULL,EditDlgProc,(LPARAM)dbs);	
	return TRUE;
}

LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if(msg == WM_ERASEBKGND)
	{
		DashBoardSticky* dbs = (DashBoardSticky*)GetProp(hwnd,"dbs");
		if(!dbs)return FALSE;

		HDC dc = (HDC)w;
		RECT r = {0};
		//GetUpdateRect(hwnd,&r,TRUE);
		//GetWindowRect(hwnd,&r);
		GetClipBox(dc,&r);
		Graphics g(dc);
		Rect rcDest(r.left,r.top,r.right - r.left, r.bottom - r.top);
		//Rect rcDest(0,0,r.right - r.left, r.bottom - r.top);
	
		g.DrawImage(dbs->GetBackGroundImage(),rcDest,rcDest.X + 15, rcDest.Y+10,rcDest.Width,rcDest.Height,UnitPixel);

		return 1;
	}
	/*else if(msg == WM_PAINT)
	{
		if(GetProp(hwnd,"IsPainting"))
		{
			WNDPROC oldProc = (WNDPROC)GetProp(hwnd,"oldProc");
			return CallWindowProc(oldProc,hwnd,msg,w,l);
		}
		else
		{
			RepaintEditControl(hwnd);
			return TRUE;
		}
	}*/
	else if(msg== WM_KILLFOCUS)
	{
		//EndDialog(GetParent(hwnd),FALSE);
		SendMessage(hwnd,EM_SETSEL,-1,-1);
		DashBoardSticky* dbs = (DashBoardSticky*)GetProp(hwnd,"dbs");
		if(dbs)
			dbs->UpdateTextLayer();
		ShowWindow(GetParent(hwnd),SW_HIDE);
		return TRUE;
	}
	else
	{
		WNDPROC oldProc = (WNDPROC)GetProp(hwnd,"oldProc");
		return CallWindowProc(oldProc,hwnd,msg,w,l);
	}
}

void RepaintEditControl(HWND hwnd)
{
	SetProp(hwnd,"IsPainting",(HANDLE)1);
	RECT r = {0};
	GetWindowRect(hwnd,&r);
	int width = r.right - r.left;
	int height= r.bottom - r.top;

	HideCaret(hwnd);
	HDC dc = CreateCompatibleDC(GetDC(hwnd));
	HBITMAP tempBmp = CreateCompatibleBitmap(GetDC(hwnd), width,height);
	HBITMAP bmp = (HBITMAP)SelectObject(dc, tempBmp);
	SendMessage(hwnd,WM_ERASEBKGND,(WPARAM)dc,0);
	SendMessage(hwnd,WM_PAINT,(WPARAM)dc,0);
	BitBlt(GetDC(hwnd), 0, 0, width, height, dc, 0, 0, SRCCOPY);
	SelectObject(dc, bmp);
	DeleteDC(dc);
	ReleaseDC(hwnd, GetDC(hwnd));
	DeleteObject(tempBmp);
	ShowCaret(hwnd);
	SetProp(hwnd,"IsPainting",(HANDLE)0);
}

BOOL CALLBACK EditDlgProc(HWND hDlg, UINT msg, WPARAM w, LPARAM l)
{
	DashBoardSticky* dbs = (DashBoardSticky*)GetProp(hDlg,"dbs");

	if(msg==WM_INITDIALOG)
	{
		SetProp(hDlg,"dbs",(HANDLE)l);
		dbs = (DashBoardSticky*)GetProp(hDlg,"dbs");
		if(!dbs)return FALSE;

		//SetClassLong(hDlg,GCL_HBRBACKGROUND,(LONG)CreateSolidBrush(RGB(255,244,144)));

		SetDlgItemTextW(hDlg,IDC_TEXT,dbs->GetText().c_str() );

		SendDlgItemMessage(hDlg,IDC_TEXT,WM_SETFONT,(WPARAM)dbs->GetFont(),FALSE);


		SendDlgItemMessage(hDlg,IDC_TEXT,EM_SETSEL,-1,-1);
		POINT p = dbs->GetPoint();
		p.x -= 15;
		p.y -= 10;
		SendDlgItemMessage(hDlg,IDC_TEXT,WM_LBUTTONDOWN,0,MAKEWORD(p.x,p.y));

		RECT r = {0};
		GetWindowRect(dbs->GetWindow(),&r);
		MoveWindow(hDlg,r.left+15,r.top+10,r.right - r.left - 40, r.bottom - r.top - 40,FALSE);
		MoveWindow(GetDlgItem(hDlg,IDC_TEXT),0,0,r.right - r.left - 40, r.bottom - r.top - 40,FALSE);


		HWND editHwnd = GetDlgItem(hDlg,IDC_TEXT);
		SetProp(editHwnd,"dbs",(HANDLE)dbs);
		SetProp(editHwnd,"oldProc",(HANDLE)GetWindowLong(editHwnd,GWL_WNDPROC));
		SetWindowLong(editHwnd,GWL_WNDPROC,(LONG)SubclassProc);

		ShowWindow(GetDlgItem(hDlg,IDC_TEXT),SW_SHOW);

		InvalidateRect(hDlg,0,TRUE);
		return TRUE;
	}
	else if(msg == WM_ERASEBKGND)
	{
		/*if(dbs)
		{
			PAINTSTRUCT ps = {0};
			HDC dc = (HDC)w;//BeginPaint(hDlg,&ps);
			RECT r = {0};
			GetClipBox(dc,&r);
	
			Graphics g(dc);
			Rect rcDest(r.left,r.top,r.right - r.left, r.bottom - r.top);
	
			g.DrawImage(dbs->GetBackGroundImage(),rcDest,rcDest.X + 15, rcDest.Y+10,rcDest.Width,rcDest.Height,UnitPixel);

			EndPaint(hDlg,&ps);
			ReleaseDC(hDlg,dc);
		}*/

		return 1;
	}
	else if(msg == WM_PAINT)
	{
		if(dbs)
		{
			PAINTSTRUCT ps = {0};
			HDC dc = BeginPaint(hDlg,&ps);
	
			Graphics g(dc);
			Rect rcDest(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
	
			g.DrawImage(dbs->GetBackGroundImage(),rcDest,rcDest.X + 15, rcDest.Y+10,rcDest.Width,rcDest.Height,UnitPixel);

			EndPaint(hDlg,&ps);
			ReleaseDC(hDlg,dc);
			return 1;
		}
	}
	else if(msg == WM_CTLCOLOREDIT)
	{
		if(dbs)
		{
			HDC dc = (HDC)w;
			SetBkMode(dc,TRANSPARENT);
			return (BOOL)dbs->GetHollowBrush();
		}
	}
	else if(msg == WM_COMMAND)
	{
		if(LOWORD(w) == IDC_TEXT && HIWORD(w) == EN_CHANGE)
		{
			RepaintEditControl(GetDlgItem(hDlg,IDC_TEXT));

			WCHAR txt[100024];
			txt[0]=0;
			GetDlgItemTextW(hDlg,IDC_TEXT,txt,sizeof(txt)/sizeof(txt[0]));
			dbs->SetText(txt);
			//dbs->UpdateTextLayer();
		}
	}
	else if(msg== WM_KILLFOCUS)
	{
		//EndDialog(hDlg,FALSE);
	}
	else if(msg == WM_DESTROY)
	{
		PostThreadMessage(dbs->GetThreadId(),WM_USER+2,0,0);
	}

	return FALSE;
}