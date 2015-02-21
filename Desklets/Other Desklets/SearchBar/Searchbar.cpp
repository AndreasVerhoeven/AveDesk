#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include "../DeskletSDK.h"
#include "../DeskletSDK.cpp"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")


#include <wininet.h>
#pragma comment(lib,"wininet.lib")

#ifndef __ATLBASE_H__
#include <ATLBASE.h>
#endif

#ifndef __ATLCONV_H__
#include <ATLCONV.H>
#endif

#include "resource.h"

#include <vector>
#include <algorithm>

using namespace Gdiplus;

LONG __stdcall EditProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
BOOL __stdcall BoxProc(HWND hDlg, UINT msg, WPARAM w, LPARAM l);


const timer_id = 30492;

const result_height = 40;

class SearchResult
{
private:
	std::wstring text;
	std::wstring link;
	std::wstring descr;

	bool isSel;

public:
	SearchResult(std::wstring text, std::wstring link, std::wstring descr=L"sample text..."):
	  text(text),link(link), descr(descr), isSel(false)
	{

	};

	std::wstring GetText()const{return text;};
	std::wstring GetLink()const{return link;};
	std::wstring GetDescription()const{return descr;};

	bool IsSelected()const{return isSel;};
	void SetSelected(bool val){isSel = val;};
};

DWORD WINAPI DataFetchThread( LPVOID lPData);


Color GetPrivateProfileColor(const char* section, const char* ini,int a, int r, int g, int b)
{
	return Color(	GetPrivateProfileInt(section,"ColorA",a,ini),
					GetPrivateProfileInt(section,"ColorR",r,ini),
					GetPrivateProfileInt(section,"ColorG",g,ini),
					GetPrivateProfileInt(section,"ColorB",b,ini)
				);

}

class SearchBar
{
private:
	HWND hwnd;
	HINSTANCE dll;

	bool barIsVisible;
	bool resultsAreVisible;


public:
	
	HWND boxHwnd;
	std::wstring text;

	std::vector<SearchResult>results;
	HANDLE threadHandle;

	Color titleColor,descrColor;


	SearchBar(HWND hwnd, HINSTANCE dll) : 
	hwnd(hwnd), dll(dll), barIsVisible(false), resultsAreVisible(false),
		boxHwnd(0), threadHandle(0),
		titleColor(255,0x45,0x8E,0xF1),descrColor(255,150,150,150)
	{
		//boxHwnd = CreateDialog(dll,MAKEINTRESOURCE(IDD_BOX),hwnd,BoxProc);
		//DialogBoxParam(dll,MAKEINTRESOURCE(IDD_BOX),hwnd,BoxProc,0);
		SetTimer(hwnd,timer_id,1000,0);

		char path[MAX_PATH+1] = {0};
		GetDLLPath(dll,path,MAX_PATH);
		PathAppend(path,"images/colors.ini");

		titleColor = GetPrivateProfileColor("Title",path,255,0x45,0x8E,0xF1);
		descrColor = GetPrivateProfileColor("Description",path,255,150,150,150);



	}

	SearchBar::~SearchBar()
	{
		if(IsWindow(boxHwnd))
		{
			DestroyWindow(boxHwnd);
		}
		if(threadHandle)
		{
			TerminateThread(threadHandle,0);
			CloseHandle(threadHandle);
			threadHandle = 0;
		}
	}

	void Select()
	{
		//ShowWindow(boxHwnd,SW_SHOW);
		if(IsBarVisible() )
			DialogBoxParam(dll,MAKEINTRESOURCE(IDD_BOX),0,BoxProc,(LPARAM)this);
	}

	HWND GetWindow()const{return hwnd;};
	HINSTANCE GetDLL()const{return dll;};



	void ReadFromIni(const char* ini, const char* group)
	{
	}

	void SaveToIni(const char* ini, const char* group)
	{
	}

	void SetupLayers();

	UINT SetupButtonLayer();
	UINT SetupBarLayer();
	UINT SetupResultsLayer();

	void ShowBar(bool doAnimation=true);
	void HideBar(bool doAnimation=true);

	bool IsBarVisible()const{return barIsVisible;};
	bool AreResultsVisible()const{return resultsAreVisible;};

	void SetResultsAreVisible(bool val){resultsAreVisible = val;};

	SIZE GetButtonSize();
	SIZE GetBarSize();

	SIZE GetResultsSize();

	UINT GetButtonLayerID();
	UINT GetBarLayerID();
	UINT GetResultsLayerID();

	void SetBarLayerWithText();
	void SetResultsLayerWithResults();

	void EnterWasHit()
	{
		if(!threadHandle)
		{
			DWORD dummy(0);
			threadHandle = CreateThread(0,0,DataFetchThread,(LPVOID)this,0,&dummy);
		}
		else
		{
			MessageBox(0,"Can you please wait a little more, I\'m still busy for you with processing the previous search command you gave me.\nThanks.","SearchBar Desklet",MB_ICONINFORMATION);
		}

		//std::wstring file = L"http://www.google.com/search?q=";
		//file += text;
		//ShellExecuteW(0,L"open",file.c_str(),0,0,SW_MAXIMIZE);


	}
};

void SearchBar::SetupLayers()
{
	// remove all layers that are there
	while(DeskletLayerCount(GetWindow() ) > 0)
	{
		DeskletLayerRemove(GetWindow(),0,FALSE);
	}
	SetupButtonLayer();
	SetupBarLayer();
	//DeskletRedraw(GetWindow());
}

WCHAR* wcsbecpy(WCHAR* dest,  WCHAR* const beg, WCHAR* const end)
{
	WCHAR* _str = beg;
	WCHAR* _desstr = dest;

	while(_str != end)*_desstr++ = *_str++;
	*_desstr = 0;
	return dest;
}

const l1 = wcslen(L"<p class=g><a href=");
const l2 = wcslen(L"</a>");
const l3 = wcslen(L"<font size=-1>");

DWORD WINAPI DataFetchThread( LPVOID lpData)
{
	SearchBar* sb  = (SearchBar*)lpData;
	if(!sb)
	{
		ExitThread(FALSE);
		return 0;
	}

	HINTERNET hInet = InternetOpen("AveSearchBar",INTERNET_OPEN_TYPE_PRECONFIG,0,0,0);
	if(hInet)
	{
		std::wstring file = L"http://www.google.com/search?q=";
		file += sb->text;


		HINTERNET hFile = InternetOpenUrlW(hInet,file.c_str(),0,0,INTERNET_FLAG_RELOAD,0);
		if(hFile)
		{
			std::wstring t;
			DWORD dwNumRead(0);
			do
			{
				char tmp[6400+2] = {0};
				dwNumRead=0;
				InternetReadFile(hFile,tmp,6400,&dwNumRead);
				if(dwNumRead > 0)
				{
					tmp[dwNumRead] = 0;
					WCHAR wTmp[6400+2] = {0};
					MultiByteToWideChar(CP_OEMCP,0,tmp,-1,wTmp,6400);
					t += wTmp;
				}

			}while(dwNumRead > 0);
			sb->results.clear();

			sb->results.push_back(SearchResult(L"Results from Google.com",file));

			WCHAR* curPos = const_cast<WCHAR*>(t.c_str());
			while(curPos = wcsstr(curPos,L"<p class=g>") )
			{
				
				WCHAR link[10024] = {0};
				WCHAR text[10024] = {0};
				WCHAR descr[10024]= {0};

				curPos += l1;

				WCHAR* t = curPos;
				WCHAR* endPos = wcsstr(t,L">");
				t = curPos;
				wcsncpy(link,t,endPos - curPos);
				curPos = wcsstr(curPos,L">");
				curPos++;

				endPos = wcsstr(curPos,L"</a>");
				wcsbecpy(text,curPos,endPos);

				curPos = wcsstr(curPos,L"<font size=-1>");
				curPos += l3;
				endPos = wcsstr(curPos,L"<br>");
				wcsbecpy(descr,curPos,endPos);

				curPos = endPos;
				curPos += l2;
				curPos++;

				sb->results.push_back(SearchResult(text,link,descr));
			}
			sb->SetResultsAreVisible(true);
			sb->SetResultsLayerWithResults();
			sb->ShowBar(false);

		}
		else
		{
			MessageBox(0,"Sorry to interrupt you, but I am not able to connect to the Google Website. Can you check for me if there is an active internetconnection?\nThanks.","SearchBar Desklet - Oopsie",MB_ICONERROR);
		}

		InternetCloseHandle(hInet);
	}

	CloseHandle(sb->threadHandle);
	sb->threadHandle = 0;
	ExitThread(FALSE);
	return 0;
}

UINT SearchBar::SetupButtonLayer()
{
	UINT bid = DeskletLayerAdd(GetWindow(), "SearchBar.Button");
	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/icon.png");
	DeskletLayerSetImageFile(GetWindow(),bid,FALSE,img);
	return bid;
}

UINT SearchBar::SetupBarLayer()
{
	UINT bid = DeskletLayerAdd(GetWindow(), "SearchBar.Bar");
	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/bar.png");
	DeskletLayerSetImageFile(GetWindow(),bid,FALSE,img);
	DeskletLayerSetVisibility(GetWindow(),bid,FALSE,FALSE);

	SIZE s = GetButtonSize();
	DeskletSetSize(GetWindow(),&s);
	return bid;
}


UINT SearchBar::SetupResultsLayer()
{
	UINT bid = DeskletLayerAdd(GetWindow(), "SearchBar.Results");
	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/results.png");
	DeskletLayerSetImageFile(GetWindow(),bid,FALSE,img);
	DeskletLayerSetVisibility(GetWindow(),bid,FALSE,FALSE);

	SIZE s = GetButtonSize();
	DeskletSetSize(GetWindow(),&s);
	return bid;
}

std::wstring StripHTMLJunk(std::wstring s)
{
	std::wstring ret;
	bool in_tag(false);
	for(int i=0;i<s.size();++i)
	{
		if(!in_tag)
		{
			if(s[i] == '<')
				in_tag = true;
			else
			{
				if(s[i] == '&' && i<s.size() -3 && s[i+3] == ';')
				{
					if(s[i+1] == 'g' && s[i+2] == 't')
						ret += '>';
					else if(s[i+1] == 's' && s[i+2] == 't')
						ret += '<';
					if(s[i+1] == 'g' && s[i+2] == 't')
						ret += '>';

					i += 4;
				}
				else if(s[i] == '&' && i<s.size() -4 && s[i+4] == ';')
				{
					if(s[i+1] == 'a' && s[i+2] == 'm' && s[i+3] == 'p')
						ret += '&';

					i+=5;
				}
				else if(s[i] == '&' && i<s.size() -5 && s[i+5] == ';')
				{
					if(s[i+1] == 'q' && s[i+2] == 'u' && s[i+3] == 'o' && s[i+4] =='t')
						ret += '\"';

					i+=6;
				}


				ret += s[i];
			}
		}
		else
		{
			if(s[i] == '>')
				in_tag = false;
		}
		if(i>48)
		{
			ret += L"...";
			break;
		}
	}
	return ret;
}

void SearchBar::SetResultsLayerWithResults()
{
	char indPath[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),indPath);
	PathAppend(indPath,"images/indicator.png");	
	Bitmap* indicator = DeskletLoadGDIPlusImage(indPath);

	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/results.png");	
	Bitmap* bmp = DeskletLoadGDIPlusImage(img);

	int w = bmp->GetWidth();
	int h = results.size()*result_height - 15;
	if(h & 1)h++;
	Bitmap* buffer = new Bitmap(w,h);

	Graphics g(buffer);
	//g.Clear(Color(255,255,255,255));
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
	//h-=15;
	g.DrawImage(bmp,Rect(0,0,w,h),0,0,w,h,UnitPixel);

	//FontFamily  fontFamily(L"Lucida Sans Unicode");
	Font font(L"Lucida Sans Unicode",10,0);
	Font font2(L"Lucida Sans Unicode",8,0);
	Font font3(L"Lucida Sans Unicode",10,1);
	SolidBrush  black(Color(255,0,0,0));

	SolidBrush titleBrush(titleColor);
	SolidBrush descrBrush(descrColor);

	Pen p(Color(255,200,200,200),1);

	//GraphicsPath path(FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);
	//format.SetLineAlignment(StringAlignmentCenter);
	format.SetAlignment(StringAlignmentNear);

	//HDC dc = g.GetHDC();

	if(results.size() > 0)
	{
		RectF rc(5,0,w,20);
		RectF rc2(6,1,w,20);
		
		//g.DrawString(results[0].GetText().c_str(),-1,&font3,rc2,&format,&shadow);
		g.DrawString(results[0].GetText().c_str(),-1,&font3,rc,&format,&titleBrush);
	}

	for(int i=1;i<results.size();++i)
	{
		if(results[i].IsSelected())
		{
			Pen bgPen(Color(200,0,0,0),1);
			SolidBrush bg(Color(150,200,200,200));
			g.DrawRectangle(&bgPen,12,(i-1)*result_height + 20 + 2,w-13,31);
			g.FillRectangle(&bg,12,(i-1)*result_height + 20 + 2,w-14,30);
		}

		RectF rc(30,(i-1)*result_height + 20,bmp->GetWidth(),18);
		
		g.DrawString(StripHTMLJunk(results[i].GetText()).c_str(),-1,&font,rc,&format,&titleBrush);

		RectF rc2(34,(i-1)*result_height +20+17,bmp->GetWidth(),15);
		g.DrawString(StripHTMLJunk(results[i].GetDescription()).c_str(),-1,&font2,rc2,&format,&descrBrush);

		if(i<results.size()-1)
			g.DrawLine(&p,10,(i-1)*result_height + 20 + 35,w-20,(i-1)*result_height + 20 + 35);

		g.DrawImage(indicator,13,(i-1)*result_height + 20 + 4,indicator->GetHeight(),indicator->GetWidth() );

	}

	//g.ReleaseHDC(dc);

	delete bmp;

	delete indicator;

	DeskletLayerSetImage(GetWindow(),GetResultsLayerID(),FALSE,buffer,TRUE);
}

void SearchBar::SetBarLayerWithText()
{
	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/bar.png");	
	Bitmap* bmp = DeskletLoadGDIPlusImage(img);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
	FontFamily  fontFamily(L"Lucida Sans Unicode");
	SolidBrush  black(Color(255,0,0,0));

	GraphicsPath path(FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);
	//format.SetLineAlignment(StringAlignmentCenter);
	format.SetAlignment(StringAlignmentNear);

	RectF rc(115,8,258-10,19);

	//WCHAR tmp[1024] = {0};
	//MultiByteToWideChar(CP_OEMCP,0,text.c_str(),-1,tmp,1024);
	path.AddString(text.c_str(), -1, &fontFamily, 0, 12, rc, &format);
	g.FillPath(&black, &path);

	if(text[0] != 0)
	{
		char img2[MAX_PATH] = {0};
		DeskletGetRelativeFolder(GetWindow(),img2);
		PathAppend(img2,"images/close.png");	
		Bitmap* bmp2 = DeskletLoadGDIPlusImage(img2);

		if(text[0] != 0 && bmp)
		{
			g.DrawImage(bmp2,115+258-15,6,bmp2->GetWidth(),bmp2->GetHeight());
		}
		
		delete bmp2;
	}


	DeskletLayerSetImage(GetWindow(),GetBarLayerID(),TRUE,bmp,TRUE);
}

UINT SearchBar::GetButtonLayerID()
{
	BOOL success(FALSE);
	UINT id = DeskletLayerGetLayer(GetWindow(),"SearchBar.Button",&success);
	if(!success)
		id = SetupButtonLayer();

	return id;
}

UINT SearchBar::GetBarLayerID()
{
	BOOL success(FALSE);
	UINT id = DeskletLayerGetLayer(GetWindow(),"SearchBar.Bar",&success);
	if(!success)
		id = SetupBarLayer();

	return id;
}

UINT SearchBar::GetResultsLayerID()
{
	BOOL success(FALSE);
	UINT id = DeskletLayerGetLayer(GetWindow(),"SearchBar.Results",&success);
	if(!success)
		id = SetupResultsLayer();

	return id;
}

SIZE SearchBar::GetButtonSize()
{
	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/icon.png");
	Bitmap* bmp = DeskletLoadGDIPlusImage(img);

	SIZE s = {0};
	if(bmp)
	{
		s.cx = bmp->GetWidth();
		s.cy = bmp->GetHeight();
	}

	delete bmp;

	return s;
}

SIZE SearchBar::GetBarSize()
{
	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/bar.png");
	Bitmap* bmp = DeskletLoadGDIPlusImage(img);

	SIZE s = {0};
	if(bmp)
	{
		s.cx = bmp->GetWidth();
		s.cy = bmp->GetHeight();
	}

	delete bmp;

	return s;
}

SIZE SearchBar::GetResultsSize()
{
	char img[MAX_PATH] = {0};
	DeskletGetRelativeFolder(GetWindow(),img);
	PathAppend(img,"images/results.png");
	Bitmap* bmp = DeskletLoadGDIPlusImage(img);

	SIZE s = {0};
	if(bmp)
	{
		s.cx = bmp->GetWidth();
		//s.cy = bmp->GetHeight();
	}
	s.cy = results.size() * result_height -15;
	if(s.cy & 1)s.cy++;

	delete bmp;

	return s;
}

void SearchBar::ShowBar(bool doAnimation)
{
	SIZE iconSize	= GetButtonSize();
	SIZE barSize	= GetBarSize();
	SIZE resultsSize= GetResultsSize();

	SIZE totalSize = {0};
	totalSize.cx = barSize.cx;
	totalSize.cy = barSize.cy + iconSize.cy;

	if(AreResultsVisible())
		totalSize.cy += resultsSize.cy;

	RECT r = {0};
	r.left = totalSize.cx - iconSize.cx;
	r.top  = 0;
	r.bottom = iconSize.cy;
	r.right = barSize.cx;
	DeskletLayerSetPlacement(GetWindow(),GetButtonLayerID(),FALSE,TRUE,&r,totalSize.cx,totalSize.cy);

	RECT r2 = {0};
	r2.left = 0;
	r2.right = barSize.cx;
	r2.top = iconSize.cy;
	if(AreResultsVisible() )
		r2.bottom = r2.top + barSize.cy;
	else
		r2.bottom = totalSize.cy;
	DeskletLayerSetPlacement(GetWindow(),GetBarLayerID(),FALSE,TRUE,&r2,totalSize.cx,totalSize.cy);


	if(AreResultsVisible())
	{
		RECT r4 = {0};
		r4.left = 0;
		r4.right = barSize.cx;
		r4.top = iconSize.cy + barSize.cy;
		r4.bottom = totalSize.cy;
		DeskletLayerSetPlacement(GetWindow(),GetResultsLayerID(),FALSE,TRUE,&r4,totalSize.cx,totalSize.cy);
		DeskletLayerSetVisibility(GetWindow(),GetResultsLayerID(),FALSE,TRUE);
	}
	else
	{
		DeskletLayerSetVisibility(GetWindow(),GetResultsLayerID(),FALSE,FALSE);
	}

	POINT curPos = {0};
	DeskletGetPosition(GetWindow(),&curPos);
	RECT r3 = {0};
	if(!IsBarVisible() )
		r3.left = curPos.x - (totalSize.cx - iconSize.cx);
	else
		r3.left = curPos.x;
	r3.right = r3.left + totalSize.cx;
	r3.top = curPos.y;
	r3.bottom = r3.top + totalSize.cy;
	DeskletMove(GetWindow(),&r3,FALSE);

	//SetBarLayerWithText();

	DeskletLayerSetVisibility(GetWindow(),GetBarLayerID(),FALSE,TRUE);

	if(doAnimation)
	{
		if(AreResultsVisible() )
		{
			for(int i=0;i<5;++i)
			{
				DeskletLayerSetAlpha(GetWindow(),GetResultsLayerID(),FALSE,i*50);
				DeskletLayerSetAlpha(GetWindow(),GetBarLayerID(),TRUE,i*50);
			}
		}
		else
		{
			for(int i=0;i<10;++i)
			{
				DeskletLayerSetAlpha(GetWindow(),GetBarLayerID(),TRUE,i*24);
			}
		}
	}
	DeskletLayerSetAlpha(GetWindow(),GetResultsLayerID(),FALSE,255);
	DeskletLayerSetAlpha(GetWindow(),GetBarLayerID(),TRUE,255);

	barIsVisible = true;
}

void SearchBar::HideBar(bool doAnimation)
{
	if(!barIsVisible)
		return;

	barIsVisible = false;

	SIZE iconSize	= GetButtonSize();
	SIZE barSize	= GetBarSize();

	SIZE totalSize = {0};
	totalSize.cx = barSize.cx;
	totalSize.cy = barSize.cy + iconSize.cy;

	SetBarLayerWithText();
	//DeskletLayerSetVisibility(GetWindow(),GetBarLayerID(),FALSE,TRUE);

	if(doAnimation)
	{
		if(AreResultsVisible() )
		{
			for(int i=0;i<5;++i)
			{
				DeskletLayerSetAlpha(GetWindow(),GetResultsLayerID(),FALSE,255-(i*50));
				DeskletLayerSetAlpha(GetWindow(),GetBarLayerID(),TRUE,255-(i*50));
			}		

		}
		else
		{
			for(int i=0;i<10;++i)
			{
				DeskletLayerSetAlpha(GetWindow(),GetBarLayerID(),TRUE,255-(i*24));
			}
		}
	}
	//DeskletLayerSetAlpha(GetWindow(),GetBarLayerID(),TRUE,255);
	DeskletLayerSetVisibility(GetWindow(),GetBarLayerID(),FALSE,FALSE);
	DeskletLayerSetVisibility(GetWindow(),GetResultsLayerID(),FALSE,FALSE);
	//DeskletLayerSetPlacement(GetWindow(),GetResultsLayerID(),FALSE,FALSE,&r,iconSize.cx,iconSize.cy);

	RECT r = {0};
	r.left = 0;
	r.top  = 0;
	r.bottom = iconSize.cy;
	r.right = iconSize.cx;
	DeskletLayerSetPlacement(GetWindow(),GetButtonLayerID(),FALSE,TRUE,&r,iconSize.cx,iconSize.cy);

	POINT curPos = {0};
	DeskletGetPosition(GetWindow(),&curPos);
	RECT r3 = {0};
	r3.left = curPos.x + totalSize.cx - iconSize.cx;
	r3.right = r3.left + iconSize.cx;
	r3.top = curPos.y;
	r3.bottom = r3.top + iconSize.cy;
	DeskletMove(GetWindow(),&r3,TRUE);
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"SearchBar");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,"Graphics by bzeitler, many thanks to Duckie from www.therealduckie.net for hints/graphics too.");
	*iVersion = 110;
}

SearchBar* __stdcall OnCreate(HWND hwnd, HINSTANCE dll, const char* ini, const char* group)
{
	SearchBar* sb = new SearchBar(hwnd,dll);
	
	if(ini && group)
		sb->ReadFromIni(ini,group);

	sb->SetupLayers();

	return sb;
}

BOOL __stdcall OnMouseOut(SearchBar* sb)
{
	for(int i=1;i<sb->results.size();++i)
	{
		if(sb->results[i].IsSelected())
		{
			sb->results[i].SetSelected(false);
			sb->SetResultsLayerWithResults();
			DeskletLayerSetAlpha(sb->GetWindow(),sb->GetResultsLayerID(),TRUE,255);
			break;
		}

	}
	return TRUE;
}

BOOL __stdcall OnMouseMove(SearchBar* sb, int dx, int dy)
{
	if(sb->IsBarVisible() && sb->AreResultsVisible() )
	{
		SIZE iconSize	= sb->GetButtonSize();
		SIZE barSize	= sb->GetBarSize();
		int y = dy - iconSize.cy - barSize.cy;
		if(y>0)
		{
			int oldSel = 0;
			for(int i=1;i<sb->results.size();++i)
			{
				if(sb->results[i].IsSelected())
				{
					oldSel = i;
					break;
				}
			}

			y -= 20;
			if(y<0)
			{
				if(oldSel>0)
				{
					sb->results[oldSel].SetSelected(false);
					sb->SetResultsLayerWithResults();
					DeskletLayerSetAlpha(sb->GetWindow(),sb->GetResultsLayerID(),TRUE,255);
				}
			}
			else
			{
				int index = (y+15) / result_height + 1;
				if(index < sb->results.size())
				{
					if(oldSel != index)
					{
						sb->results[oldSel].SetSelected(false);
						sb->results[index].SetSelected(true);
						sb->SetResultsLayerWithResults();
						DeskletLayerSetAlpha(sb->GetWindow(),sb->GetResultsLayerID(),TRUE,255);
					}
				}
				else if(oldSel != 0)
				{
					sb->SetResultsLayerWithResults();
					DeskletLayerSetAlpha(sb->GetWindow(),sb->GetResultsLayerID(),TRUE,255);
				}
			}
		}
	}
	return TRUE;
}


BOOL __stdcall OnLeftButtonClick(SearchBar* sb, POINT* p, SIZE* s)
{
	if(!sb->IsBarVisible())
	{
		sb->ShowBar();
		sb->Select();
	}
	else
	{
		SIZE s = sb->GetButtonSize();
		if(p->y < s.cy)
		{
			EndDialog(sb->boxHwnd,FALSE);
			//sb->HideBar();
		}
		else
		{
			if(sb->AreResultsVisible())
			{
				SIZE iconSize	= sb->GetButtonSize();
				SIZE barSize	= sb->GetBarSize();
				int y = p->y - iconSize.cy - barSize.cy;
				if(y>0)
				{
					y -= 20;
					if(y<20)
					{
						ShellExecuteW(0,L"open",sb->results[0].GetLink().c_str(),0,0,SW_MAXIMIZE);
					}
					else
					{
						int index = (y+15) / result_height +1;
						if(index < sb->results.size())
						{
							ShellExecuteW(0,L"open",sb->results[index].GetLink().c_str(),0,0,SW_MAXIMIZE);
						}
					}
					return TRUE;
				}
			}
			int y = p->y - s.cy;
			if(y>8 && y <30 && p->x > (363) && p->x < (363+20) )
			{
				sb->text = L"";
				SetDlgItemText(sb->boxHwnd,IDC_TEXT,"");
				sb->SetBarLayerWithText();
				if(sb->AreResultsVisible() )
				{
					for(int i=0;i<5;++i)
						DeskletLayerSetAlpha(sb->GetWindow(),sb->GetResultsLayerID(),TRUE,255-(i*50));
					sb->SetResultsAreVisible(false);
					sb->ShowBar(false);
				}
				SetWindowPos(sb->boxHwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE/*|SWP_NOACTIVATE*/		); 
				//SetFocus(sb->boxHwnd);
			}
			else
			{
				SetWindowPos(sb->boxHwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE/*|SWP_NOACTIVATE*/		); 
			}
		}
	}

	return TRUE;
}

void __stdcall OnProcessMessage(SearchBar* sb, HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	///if(msg == WM_LBUTTONUP)
	//{
	//	if(sb->IsBarVisible())
	//	{
	//		SetWindowPos(sb->boxHwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE/*|SWP_NOACTIVATE		*/); 
	//		//SetFocus(sb->boxHwnd);
	//	}
	//}
	if(msg == WM_KILLFOCUS)
	{
		if((HWND)w != sb->boxHwnd && (HWND)w != GetDlgItem(sb->boxHwnd,IDC_TEXT) )
		{
			EndDialog(sb->boxHwnd,FALSE);
			//sb->HideBar();
		}
	}
	else if(msg == WM_TIMER && w == timer_id)
	{
		char className[1024] ={0};
		GetClassName(GetActiveWindow(),className,1024);
		if(!sb->IsBarVisible() && (strcmp(className,"DockItemClass")==0 || GetActiveWindow() == 0))
		{
			GetClassName(GetForegroundWindow(),className,1024);
			if(strcmp(className,"#32768")!=0)
			{
				if(!sb->IsBarVisible())
					SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE|SWP_NOACTIVATE		); 
			}
		}
	}
	else if(msg == WM_WINDOWPOSCHANGING)
	{
		WINDOWPOS* wp = (WINDOWPOS*)l;
		wp->hwndInsertAfter = HWND_TOPMOST;
		//if(wp->hwndInsertAfter != HWND_TOPMOST)
		//{
 			//SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE	|SWP_NOACTIVATE	);
		//}
	}
}

void __stdcall OnSave(SearchBar* sb, const char* ini, const char* group, BOOL bUnused)
{
	sb->SaveToIni(ini,group);
}

void __stdcall OnDestroy(SearchBar* sb, HWND hwnd)
{
	sb->HideBar();
	delete sb;
}

BOOL __stdcall OnSelect(SearchBar* sb)
{
	//sb->Select();
	return TRUE;
}

BOOL __stdcall OnDeselect(SearchBar* sb)
{
/*	if(sb->IsBarVisible())
	{
		EndDialog(boxHwnd,FALSE);
		sb->HideBar();
	}
*/
	return TRUE;
}



LONG __stdcall EditProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch(msg)
	{
		case WM_KILLFOCUS:
		{
			SearchBar* sb = (SearchBar*)GetProp(GetParent(hwnd),"SB");
			if((HWND)w != sb->GetWindow())
				EndDialog(GetParent(hwnd),FALSE);
			//else
			//	SetWindowPos(GetParent(hwnd),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE|SWP_NOACTIVATE		); 
			return TRUE;
		}
		break;

		/*case WM_CHAR:
		{
			if(w==0x10 || w==0x13)
			{
				SearchBar* sb = (SearchBar*)GetProp(GetParent(hwnd),"SB");
				sb->EnterWasHit();
			}
			else
			{
				return CallWindowProc((WNDPROC)GetProp(GetParent(hwnd),"OldEditProc"),hwnd,msg,w,l);
			}
			return TRUE;
		}
		break;*/

		default:
			return CallWindowProc((WNDPROC)GetProp(GetParent(hwnd),"OldEditProc"),hwnd,msg,w,l);
		break;
	}
}

BOOL __stdcall BoxProc(HWND hDlg, UINT msg, WPARAM w, LPARAM l)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			//SetWindowPos(hDlg,HWND_TOPMOST,p.x+115,p.y+8+barSize.cy,258,19,f);
			SearchBar* sb = (SearchBar*)l;
			sb->boxHwnd = hDlg;
			SetDlgItemTextW(hDlg,IDC_TEXT,sb->text.c_str() );
			SetProp(hDlg,"SB",(HANDLE)sb);
			POINT p = {0};
			DeskletGetPosition(sb->GetWindow(),&p);
			SIZE barSize = sb->GetButtonSize();
			MoveWindow(GetDlgItem(hDlg,IDC_TEXT),0,0,258-10,19,FALSE);
			//MoveWindow(hDlg,p.x + 115,p.y + 8 + barSize.cy,258,19,FALSE);
			SetWindowPos(hDlg,HWND_TOPMOST,p.x+115,p.y+8+barSize.cy,258-10,19,0);
			SetProp(hDlg,"OldEditProc",(HANDLE)SetWindowLong(GetDlgItem(hDlg,IDC_TEXT),GWL_WNDPROC	,(LONG)EditProc));
		}
		break;


		/*case WM_CHAR:
		{
			SearchBar* sb = (SearchBar*)GetProp(hDlg,"SB");
			if(sb)
			{
				sb->EnterWasHit();
			}
		}*/
		break;

		case WM_COMMAND:
			switch(LOWORD(w))
			{
				case IDC_ENTER:
				{
					SearchBar* sb = (SearchBar*)GetProp(hDlg,"SB");
					if(sb)
					{
						sb->EnterWasHit();
					}
				}
				break;

				case IDC_TEXT:
					if(HIWORD(w)==EN_CHANGE)
					{
						SearchBar* sb = (SearchBar*)GetProp(hDlg,"SB");
						if(sb)
						{
							WCHAR tmp[1024] = {0};
							GetDlgItemTextW(hDlg,IDC_TEXT,tmp,1024);
							sb->text =tmp;
							sb->SetBarLayerWithText();
						}
					}
				break;
			}
		break;

		break;

		case WM_DESTROY:
		{
			SetWindowLong(GetDlgItem(hDlg,IDC_TEXT),GWL_WNDPROC	,(LONG)RemoveProp(hDlg,"OldEditProc"));
			SearchBar* sb = (SearchBar*)GetProp(hDlg,"SB");
			sb->HideBar();
			sb->boxHwnd = NULL;
		}
		break;
	}
	return FALSE;
}